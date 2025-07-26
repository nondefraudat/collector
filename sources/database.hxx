#pragma once

#include <file.hxx>
#include <filesystem>
#include <list>

class sqlite3;

namespace nd {
namespace collector {

class database {
public:
	database() { }
	~database();

	bool load(const std::filesystem::path &db_path);

	int64_t add(const file &f);

	std::list<std::string> hash_by_size(size_t file_size);

private:
	sqlite3 *_db = nullptr;	

	bool init_tables();
};

}
}
