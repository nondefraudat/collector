#include <database.hxx>
#include <collector.hxx>
#include <openssl/sha.h>
#include <filesystem>
#include <cassert>

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
		else { succeed &= capture_file(e.path()); }
	}
	return succeed;
}

bool collector::capture_file(file f) {
	auto new_path = _config.collection_path()/f.hash();
	for (const auto &hash : _database.hash_by_size(f.size())) {
		if (f.hash() == hash) {
			file t(new_path);
			f.remove();
			return t.create_symlink(f.path());
		}
	}	
	if (_database.add(f) != -1) {
		return f.move_to(new_path);
	}
	return false;
}

} // collector
} // nd