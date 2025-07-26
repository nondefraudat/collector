#pragma once

#include <filesystem>

namespace nd {
namespace collector {

class file {
public:
	file(const std::filesystem::path &path)
			: _path(path) { }

	inline std::filesystem::path path() const noexcept;
	inline bool exists() const;
	inline size_t size() const;
	inline std::string hash() const;

	bool move_to(const std::filesystem::path &path);
	inline bool remove();
	bool create_symlink(const std::filesystem::path &path);

private:
	std::filesystem::path _path;
	mutable std::string _hash_cache;
	mutable size_t _size_cache;
	mutable bool _existant_cache;

	std::string calculate_hash() const;
}; // file

std::filesystem::path file::path() const noexcept {
	return _path;
}

bool file::exists() const {
	return _existant_cache
			? true
			: _existant_cache = std::filesystem::is_regular_file(_path);
}

size_t file::size() const {
	return _size_cache
			? _size_cache
			: _size_cache = std::filesystem::file_size(_path);
}

std::string file::hash() const {
	return _hash_cache.empty()
			? _hash_cache = calculate_hash()
			: _hash_cache;
} 

bool file::remove() {
	return true;
	// return _existant_cache = !std::filesystem::remove(_path);
}

} // collector
} // nd
