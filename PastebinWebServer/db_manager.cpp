#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>

#include "db_manager.h"

using namespace Pastebin;
using json = nlohmann::json;

inline sql::connection_config DBManager::get_config() {
	sql::connection_config config;
	config.path_to_database = ":memory:";
	config.flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
	config.debug = false;

	return config;
}

DBManager::DBManager() 
	: db(get_config()) 
	, inst()
	, conn(mongo::uri("mongodb+srv://Wkkkkk:wkkkkk@cluster0.wqx0r.mongodb.net/Pastebin?retryWrites=true&w=majority"))
	, collection(conn["Pastebin"]["Test"])
{ 
	db.execute(" \
		CREATE TABLE IF NOT EXISTS pastes ( \
			shortlink text NOT NULL, \
			expiration_length_in_minutes integer NOT NULL, \
			creation_at TEXT DEFAULT CURRENT_TIMESTAMP, \
			paste_path text NOT NULL, \
			PRIMARY KEY(shortlink) \
			)");
}

DBManager::~DBManager() { }

DBManager& Pastebin::DBManager::get_instance()
{
	static DBManager instance;
	return instance;
}

bool DBManager::insert_paste(const std::string & short_link,
	const std::string& create_time, int time_to_expire, const std::string& content){
	const auto new_paste = Pastebin::Pastes{};

	try
	{
		// insert properties into sqlite
		db(insert_into(new_paste).set(
			new_paste.expirationLengthInMinutes = time_to_expire,
			new_paste.creationAt = create_time,
			new_paste.shortlink = short_link,
			new_paste.pastePath = content));

		// insert content into mongo
		bsoncxx::builder::stream::document document{};
		document << "short_link" << short_link << "content" << content;

		collection.insert_one(document.view());
	}
	catch (std::runtime_error ex)
	{
		return false;
		std::cerr << "insert error: "  << ex.what() << std::endl;
	}

	return true;
}

std::optional<DBManager::SearchResult>
DBManager::find_paste_with_shortlink(const std::string& short_link)
{
	const auto paste = Pastebin::Pastes{};
	
	// find porperty
	for (const auto& row : db(select(all_of(paste)).from(paste).where(paste.shortlink == short_link)))
	{
		int64_t expire_at = row.expirationLengthInMinutes;
		std::string create_at = row.creationAt;

		// find content
		{
			bsoncxx::builder::stream::document document{};
			document << "short_link" << short_link;

			auto result = collection.find_one(document.view());
			if (result) {
				
				json body = json::parse(bsoncxx::to_json(*result));
				std::string content = body["content"];

				return std::make_tuple(expire_at, create_at, content);
			}
		}
	}

	return {};
}
