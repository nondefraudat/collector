#pragma once

#include <filesystem>

namespace nd {
namespace collector {

class config {
public:
	config() : _data_path("/tmp/test") { }

	inline std::filesystem::path
	collection_path() const noexcept {
		return _collection_path.empty()
				? _collection_path = _data_path/_collection_dir
				: _collection_path;
 	}
	inline std::filesystem::path
	database_path() const noexcept {
		return _database_path.empty()
				? _database_path = _data_path/_database_name
				: _database_path;
	}

private:
	static constexpr auto _database_name = "database.sqlite3";
	static constexpr auto _collection_dir = "collection";

	std::filesystem::path _data_path;
	mutable std::filesystem::path
	_collection_path, _database_path;
};

}
}