#include <collector/file.hxx>
#include <collector/hashsum.hxx>
#include <fstream>
#include <system_error>
#include <vector>

using namespace std;
namespace fs = filesystem;

namespace nd {
namespace collector {

bool file::move_to(const fs::path &path) {
	error_code error;
	// fs::rename(_path, path, error);
	// if (error) { return false; }
	// _path = path;	
	// return true;
	bool succeed = fs::copy_file(_path, path);
	_path = path;
	return succeed;
}

bool file::create_symlink(const fs::path &path) {
	return true;
	// error_code error;
	// fs::create_symlink(_path, path, error);
	// return bool(error);
}

string file::calculate_hash() const {
	static constexpr uint32_t buffer_size = 268'435'456; // 256MB

	std::ifstream file(_path, std::ios::binary);
	if (!file.is_open()) { return string(); }
	
	vector<char> buffer(buffer_size);

	hashsum hs;
	hs.init();

	while (file.good()) {
		file.read(buffer.data(), buffer_size);
		hs.add(buffer.data(), file.gcount()); 
	}
  
	return hs.get();
}

} // collector
} // nd
