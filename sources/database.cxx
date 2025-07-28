#include <collector/database.hxx>
#include <sqlite3.h>
#include <cassert>

using namespace std;
namespace fs = filesystem;

namespace nd {
namespace collector {

database::~database() {
	if (_database) {
		sqlite3_close(_database);
		_database = nullptr;
	}
}

bool database::load(const fs::path &db_path) {
	if (sqlite3_open(db_path.c_str(), &_database)) {
		return false;
	}
	return init_tables();
}

int64_t database::add(const file &f) {
	static constexpr auto request =
	"INSERT INTO files (hash, size) VALUES (?, ?);";

	sqlite3_stmt *stmt;
	if (sqlite3_prepare(_database, request, -1,
			&stmt, nullptr)) {
		return -1;
	}

	const auto &hash = f.hash();
	sqlite3_bind_text(stmt, 1, hash.c_str(), hash.size(), SQLITE_STATIC);
	sqlite3_bind_int64(stmt, 2, f.size());

	if (sqlite3_step(stmt) != SQLITE_DONE) {
		return -1;
	}

	int64_t id = sqlite3_last_insert_rowid(_database);
	sqlite3_finalize(stmt);
	return id;
}

bool database::add_info(int64_t file_id,
		const string &title, const string &value) {
	static constexpr auto request =
	"INSERT INTO file_infos (file_id, title, value)"
	"    VALUES (?, ?, ?);";

	sqlite3_stmt *statement;
	if (sqlite3_prepare(_database, request,
			-1, &statement, nullptr)) {
		return false;
	}

	sqlite3_bind_int64(statement, 1, file_id);
	sqlite3_bind_text(statement, 2, title.c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_text(statement, 3, value.c_str(), -1, SQLITE_STATIC);

	return ( sqlite3_step(statement) == SQLITE_DONE );
}

list<string> database::hash_by_size(size_t size) {
	static constexpr auto request =
	"SELECT hash FROM files WHERE size = ?;";

	list<string> hash_list;

	sqlite3_stmt *stmt;
	if (sqlite3_prepare(_database, request, -1,
			&stmt, nullptr)) {
		return hash_list;
	}

	sqlite3_bind_int64(stmt, 1, size);
	
	while (sqlite3_step(stmt) == SQLITE_ROW) {
		hash_list.emplace_back(
				reinterpret_cast<const char *>(
						sqlite3_column_text(stmt, 0) )  );
	}

	sqlite3_finalize(stmt);
	return hash_list;
}

int64_t database::id_by_hash(const string &hash) {
	static constexpr auto request =
	"SELECT id FROM files WHERE hash = ?;";

	sqlite3_stmt *statement;
	if ( sqlite3_prepare_v2(_database, request, -1,
			&statement, nullptr) != SQLITE_OK ) {
		return -1;
	}

	sqlite3_bind_text(statement, 1, hash.c_str(), hash.size(), SQLITE_STATIC);

	list<int64_t>ids;
	while (sqlite3_step(statement) == SQLITE_ROW) {
		ids.emplace_back( sqlite3_column_int64(statement, 0) );
	}

	sqlite3_finalize(statement);
	return ( ids.empty() ? -1 : ids.front() );
}

map<string, string> database::file_info(int64_t file_id) {
	static constexpr auto request =
	"SELECT title, value FROM file_infos WHERE file_id = ?;";

	map<string, string> file_info;

	sqlite3_stmt *statement;
	if (sqlite3_prepare(_database, request,
			-1, &statement, nullptr)) {
		return file_info;
	}

	sqlite3_bind_int64(statement, 1, file_id);

	while ( sqlite3_step(statement) == SQLITE_ROW ) {
		string title = reinterpret_cast<const char *>(
				sqlite3_column_text(statement, 0));
		string value = reinterpret_cast<const char *>(
				sqlite3_column_text(statement, 1));
		file_info[title] = value;
	}

	sqlite3_finalize(statement);
	return file_info;
}

bool database::init_tables() {
	return
	init_table_files() &&
	init_table_file_infos();
}

bool database::init_table_files() {
	static constexpr auto request =
	"CREATE TABLE IF NOT EXISTS files ("
	"  id INTEGER PRIMARY KEY AUTOINCREMENT,"
	"  hash TEXT NOT NULL,"
	"  size INTEGER );";
	return init_table(request);
}

bool database::init_table_file_infos() {
	static constexpr auto request =
	"CREATE TABLE IF NOT EXISTS file_infos ("
	"  file_id INTEGER,"
	"  title TEXT NOT NULL,"
	"  value TEXT NOT NULL,"
	"  PRIMARY KEY (file_id, title) );";
	return init_table(request);
}

bool database::init_table(const char *request) {
	char *error_message;
	if (sqlite3_exec(_database, request,
			nullptr, nullptr, &error_message)) {
		sqlite3_free(error_message);
		return false;
	}
	return true;
}

} // collector
} // nd
