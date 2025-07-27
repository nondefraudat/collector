#pragma once

#include <cstddef>
#include <openssl/evp.h>
#include <string>

namespace nd {
namespace collector {

class hashsum {
public:
	hashsum() = default;
	~hashsum();

	bool init();
	inline bool add(const char bytes[], size_t count);
	inline std::string get() const;

private:
	mutable std::string _result_cache;
	EVP_MD_CTX *_context = nullptr;

	std::string build_result() const;
};

bool hashsum::add(const char *bytes, size_t count) {
	return EVP_DigestUpdate(_context, bytes, count);
}

std::string hashsum::get() const {
	return _result_cache.empty()
			? _result_cache = build_result()
			: _result_cache;
}

} // collector
} // nd
