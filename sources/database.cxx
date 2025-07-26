#include <cassert>
#include <database.hxx>
#include <sqlite3.h>

using namespace std;
namespace fs = filesystem;

namespace nd {
namespace collector {

database::~database() {
	if (_db) {
		sqlite3_close(_db);
		_db = nullptr;
	}
}

bool database::load(const fs::path &db_path) {
	if (sqlite3_open(db_path.c_str(), &_db)) {
		return false;
	}
	return init_tables();
}

int64_t database::add(const file &f) {
	static constexpr auto request =
	"INSERT INTO files (hash, size) VALUES (?, ?);";

	sqlite3_stmt *stmt;
	if (sqlite3_prepare(_db, request, -1, &stmt, nullptr)) {
		return -1;
	}

	sqlite3_bind_text(stmt, 1, f.hash().c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_int64(stmt, 2, f.size());

	if (sqlite3_step(stmt) != SQLITE_DONE) {
		return -1;
	}

	int64_t id = sqlite3_last_insert_rowid(_db);
	sqlite3_finalize(stmt);
	return id;
}

list<string> database::hash_by_size(size_t size) {
	static constexpr auto request =
	"SELECT hash FROM files WHERE size = ?;";

	list<string> hash_list;

	sqlite3_stmt *stmt;
	if (sqlite3_prepare(_db, request, -1, &stmt, nullptr)) {
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

bool database::init_tables() {
	static constexpr auto request =
	"CREATE TABLE IF NOT EXISTS files ("
	"  id INTEGER PRIMARY KEY AUTOINCREMENT,"
	"  hash TEXT NOT NULL,"
	"  size INTEGER);";

	char *error_message;
	if (sqlite3_exec(_db, request,
			nullptr, nullptr, &error_message)) {
		sqlite3_free(error_message);
		return false;
	}
	return true;
}

} // collector
} // nd
