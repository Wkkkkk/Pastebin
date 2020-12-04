#pragma once

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>

#include "db_manager.h"

namespace beast = boost::beast;     // from <boost/beast.hpp>
namespace http = beast::http;       // from <boost/beast/http.hpp>

// Business logic
namespace Pastebin {

// Handle all user cases
class RequestHandler {
public:
	enum class STATUS {
		ok,
		not_found,
		illegal_request
	};

	const static size_t URL_LENGTH = 5;
	using result_pair = std::pair<STATUS, std::string /* response_body */>;

	RequestHandler() = default;

	result_pair dispatch(const http::request<http::string_body>& request);

private:

	result_pair handle_get_request(const http::request<http::string_body>& request);
	std::string retrive_short_link_from_request_target(const http::request<http::string_body>& request);

	result_pair handle_post_request(const http::request<http::string_body>& request);
	std::pair<int, std::string> retrive_time_content_from_request(const http::request<http::string_body>& request);
	std::pair<std::string, std::string> generate_create_time_and_md5();
};

}