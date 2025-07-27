#pragma once

#include <config.hxx>
#include <database.hxx>
#include <filesystem>

namespace nd {
namespace collector {

class collector {
public:
	collector(const config &cfg = config())
			: _config(cfg) { }

	bool setup();

	bool capture(const std::filesystem::path &target,
			bool recursive = false);

private:
	config _config;
	database _database;

	bool capture_file(file f);
};

}
}
