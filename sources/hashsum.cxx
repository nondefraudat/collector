#include <collector/hashsum.hxx>
#include <openssl/evp.h>
#include <iomanip>
#include <sstream>

using namespace std;

namespace nd {
namespace collector {

hashsum::~hashsum() {
	if (_context) { EVP_MD_CTX_free(_context); }
}

bool hashsum::init() {
	_context = EVP_MD_CTX_new();
	if (!_context) { return false; }
	return ( EVP_DigestInit_ex(_context,
			EVP_sha256(), nullptr) != 1 );
}

string hashsum::build_result() const {
	unsigned char hash[EVP_MAX_MD_SIZE];
	unsigned int hash_size;
	if ( EVP_DigestFinal(_context, hash, &hash_size) != 1 ) {
		return string();
	}
	stringstream ss;
	ss << hex << setfill('0');
	for (unsigned int i = 0; i < hash_size; i++) {
		ss << setw(2) << static_cast<int>(hash[i]);
	}
	return ss.str();
}

} // collector
} // nd
