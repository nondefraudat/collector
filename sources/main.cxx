#include <collector/collector.hxx>
#include <logger/logger.hxx>

#include <cstring>
#include <cassert>

using namespace nd;
using namespace std;

int main(int argc, char *argv[]) {
	assert(argc > 1);
	collector::collector app;
	assert(app.setup());
	if (argc == 3 && (strcmp(argv[1], "info") == 0) ) {
		return int(!app.show_info(argv[2]));
	}	
	string path = argv[1];
	logger::log("[main]: capturing path \"", path, "\"...");
	return int( !app.capture(path) );
}
