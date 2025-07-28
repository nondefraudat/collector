#include <collector/database.hxx>
#include <collector/collector.hxx>

#include <openssl/evp.h>
#include <openssl/sha.h>
#include <filesystem>
#include <cassert>
#include <iostream>

using namespace std;
namespace fs = filesystem;

namespace nd {
namespace collector {

bool collector::setup() {
	fs::create_directories(_config.collection_path());
	return _database.load(_config.database_path());
}

bool collector::capture(const fs::path &target, bool recursive) {
	if (!exists(target)) { return false; }
	if (is_regular_file(target)) { return capture_file(target); }
	if (!is_directory(target)) { return false; }
	bool succeed = true;
	for (const auto &e : fs::directory_iterator(target) ) {
		if (is_directory(e) && recursive) { succeed &= capture(e, recursive); }
		else if (fs::is_regular_file(e)) { succeed &= capture_file(e.path()); }
	}
	return succeed;
}

bool collector::show_info(const fs::path &target) {
	auto id = _database.id_by_hash(target.stem());
	if (id == -1) {
		cout << target.stem() << " uncaptured!" << endl;
		return false;
	}
	for (const auto &entry : _database.file_info(id)) {
		cout << entry.first << ": " << entry.second << ";" << endl;	
	}
	return true;
}

bool collector::capture_file(file f) {
	auto new_path = _config.collection_path()/f.hash();
	for (const auto &hash : _database.hash_by_size(f.size())) {
		if (f.hash() == hash) {
			// file t(new_path);
			// f.remove();
			return true; // t.create_symlink(f.path());
		}
	}	
	int64_t file_id;
	if ( (file_id = _database.add(f)) != -1 ) {
		auto file_path = f.path();
		_database.add_info(file_id, "name", file_path.stem());
		_database.add_info(file_id, "format", file_path.extension());
		_database.add_info(file_id, "size", to_string(f.size()) + "B");
		return f.move_to(new_path);
	}
	return false;
}

} // collector
} // nd