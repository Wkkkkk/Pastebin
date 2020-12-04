#include <iostream>
#include <chrono>
#include <ctime>    
#include <iomanip>
#include <sstream>

#include <absl/strings/str_split.h>
#include <boost/beast/version.hpp>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>

#include "md5.h"
#include "request_handler.h"

using namespace Pastebin;
using json = nlohmann::json;
using parse_error = nlohmann::detail::parse_error;

RequestHandler::result_pair
RequestHandler::dispatch(const http::request<http::string_body>& request)
{
    std::cout << "dispatch request: " << request.method() << " "
        << request.target() << " " << request.body() << std::endl;
    result_pair result;

    if (request.method() == http::verb::get) {
        result =  handle_get_request(request);
    }
    else if (request.method() == http::verb::post) {
        result = handle_post_request(request);
    }

    return result;
}

RequestHandler::result_pair 
RequestHandler::handle_get_request(const http::request<http::string_body>& request)
{
    std::string short_link = retrive_short_link_from_request_target(request);
    if (short_link.empty()) {
        return { RequestHandler::STATUS::not_found, short_link };
    }

    // generate response json
    json j;

    // retrieve paste from sqlite
    if (const auto result = DBManager::get_instance().find_paste_with_shortlink(short_link)) {
        const auto& [expire_at, created_at, content] = result.value();

        j["content"] = content;
        j["created_at"] = created_at;
        j["expiration_length_in_minutes"] = expire_at;
    }
    else {
        return { RequestHandler::STATUS::not_found, short_link };
    }

    return { RequestHandler::STATUS::ok, j.dump() };
}

std::string RequestHandler::retrive_short_link_from_request_target(const http::request<http::string_body>& request)
{
    std::string result;

    std::string target = request.target().to_string();
    std::vector<std::string> parts = absl::StrSplit(target, "?");
    if (parts.size() == 2) {
        std::vector<std::string> all_parameters = absl::StrSplit(parts[1], "&");

        for (std::string one_parameter : all_parameters) {
            std::vector<std::string> parameter_pair = absl::StrSplit(one_parameter, "=");
            if (parameter_pair.size() == 2) {
                if (parameter_pair[0] == "shortlink") {
                    std::string short_link = parameter_pair[1];

                    result = short_link;
                    break;
                }
            }
        }
    }

    return result;
}

RequestHandler::result_pair 
RequestHandler::handle_post_request(const http::request<http::string_body>& request)
{
    // retrive input
    const auto [time_to_expire, content] = retrive_time_content_from_request(request);
    if (time_to_expire == 0 || content.empty()) {
        return { RequestHandler::STATUS::illegal_request, 
            "Can't retrive request with time [" + std::to_string(time_to_expire) + "] and content: [" + content + "]" };
    }
    
    // generate short link for request
    const auto [create_time, shortlink] = generate_create_time_and_md5();

    // save paste
    bool insert_result = DBManager::get_instance().insert_paste(shortlink, create_time, time_to_expire, content);
    if (!insert_result) {
        return { RequestHandler::STATUS::illegal_request,
          "Fail to store request with time [" + std::to_string(time_to_expire) + "] and content: [" + content + "]" };
    }

    // generate response
    json j;
    j["shortlink"] = shortlink;

    return { RequestHandler::STATUS::ok, j.dump() };
}

std::pair<int, std::string>
RequestHandler::retrive_time_content_from_request(const http::request<http::string_body>& request)
{
    json body;

    try
    {
        body = json::parse(request.body());
    }
    catch (parse_error ex)
    {
        return { 0, "" };
    }

    int time_to_expire;
    std::string content;
    // special iterator member functions for objects
    for (json::iterator it = body.begin(); it != body.end(); ++it) {

        if (it.key() == "expiration_length_in_minutes") {
            time_to_expire = it.value();
        }

        if (it.key() == "paste_contents") {
            content = it.value();
        }
    }

    return { time_to_expire, content };
}

std::pair<std::string, std::string> RequestHandler::generate_create_time_and_md5()
{
    time_t now_time = time(NULL);    
    char str[26];
    ctime_s(str, sizeof str, &now_time);
    std::string time_str(str);
    
    std::string md5_str = md5_N(time_str);

    return { time_str, md5_str };
}
