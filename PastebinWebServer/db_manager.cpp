#include "db_manager.h"

using namespace Pastebin;

inline sql::connection_config DBManager::get_config() {
	sql::connection_config config;
	config.path_to_database = ":memory:";
	config.flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
	config.debug = false;

	return config;
}

DBManager::DBManager() : db(get_config()) { 
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

bool DBManager::insert_paste(const std::string& shortlink, 
	const std::string& create_time, int time_to_expire, const std::string& content){
	const auto new_paste = Pastebin::Pastes{};

	try
	{
		db(insert_into(new_paste).set(
			new_paste.expirationLengthInMinutes = time_to_expire,
			new_paste.creationAt = create_time,
			new_paste.shortlink = shortlink,
			new_paste.pastePath = content));
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
	
	for (const auto& row : db(select(all_of(paste)).from(paste).where(paste.shortlink == short_link)))
	{
		int64_t expire_at = row.expirationLengthInMinutes;
		std::string create_at = row.creationAt;
		std::string content = row.pastePath;
		
		return std::make_tuple(expire_at, create_at, content);
	}

	return {};
}
