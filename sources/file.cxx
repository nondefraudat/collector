#include <file.hxx>
#include <filesystem>
#include <openssl/crypto.h>
#include <openssl/evp.h>
#include <fstream>
#include <sstream>
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
	EVP_MD_CTX* ctx = EVP_MD_CTX_new();
	unsigned char md_value[EVP_MAX_MD_SIZE];
	unsigned int md_len;

	EVP_DigestInit_ex(ctx, EVP_sha256(), nullptr);
	{
		std::ifstream file(_path, std::ios::binary);
		if (!file.is_open()) { return string(); }
		constexpr uint32_t buffer_size = 268'435'456; // 256MB
		vector<char> buffer(buffer_size);
		while (file.good()) {
			file.read(buffer.data(), buffer_size);
			EVP_DigestUpdate(ctx, buffer.data(), file.gcount()); 
		}
	}	
	EVP_DigestFinal_ex(ctx, md_value, &md_len);
	EVP_MD_CTX_free(ctx);
  
	stringstream ss;
	for(auto value : md_value) {
		ss << std::hex << std::setw(2) << std::setfill('0')
				<< static_cast<int>(value);
	}
	return ss.str();
}

}
}
