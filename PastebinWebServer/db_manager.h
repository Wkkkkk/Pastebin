#pragma once
#include <tuple>
#include <optional>

#include <sqlpp11/sqlite3/sqlite3.h>
#include <sqlpp11/sqlpp11.h>

#include "pastes.h"

namespace sql = sqlpp::sqlite3;

namespace Pastebin
{

	class DBManager
	{
	private:
		sql::connection_config get_config();

		DBManager();
		~DBManager();
		DBManager(const DBManager&) = delete;
		DBManager& operator=(const DBManager&) = delete;

		sql::connection db;

	public:
		using SearchResult = std::tuple <int /*expire_time*/, std::string /*create_at*/, std::string /*content*/>;
		static DBManager& get_instance();

		bool insert_paste(const std::string& shortlink, const std::string& create_time, int time_to_expire, const std::string& content);
		std::optional<SearchResult> find_paste_with_shortlink(const std::string& short_link);
	};
}