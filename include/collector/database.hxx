#pragma once

#include <collector/file.hxx>
#include <filesystem>
#include <list>
#include <map>

class sqlite3;

namespace nd {
namespace collector {

class database {
public:
	database() { }
	~database();

	bool load(const std::filesystem::path &db_path);

	int64_t add(const file &f);
	bool add_info(int64_t file_id,
			const std::string &title, const std::string &value);

	std::list<std::string> hash_by_size(size_t file_size);
	int64_t id_by_hash(const std::string &hash);
	std::map<std::string, std::string> file_info(int64_t file_id);

private:
	sqlite3 *_database = nullptr;	

	bool init_tables();

	inline bool init_table_files();
	inline bool init_table_file_infos();
	inline bool init_table(const char *request);
};

}
}
