#include <collector.hxx>
#include <cstring>
#include <iostream>
#include <cassert>

using namespace nd::collector;
using namespace std;

int main(int argc, char *argv[]) {
	assert(argc > 1);
	collector app;
	assert(app.setup());
	if (argc == 3 && (strcmp(argv[1], "info") == 0) ) {
		return int(!app.show_info(argv[2]));
	}	
	string path = argv[1];
	cout << "capturing path \"" << path << "\"..." << endl;
	return int( !app.capture(path) );
}
