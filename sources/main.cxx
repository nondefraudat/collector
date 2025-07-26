#include <collector.hxx>
#include <iostream>
#include <cassert>

using namespace nd::collector;
using namespace std;

int main(int argc, char *argv[]) {
	collector app;
	assert(argc > 1);
	std::string path = argv[1];
	cout << "capturing path \"" << path << "\"..." << endl;
	assert(app.setup());
	cout << ( app.capture(path) ? "captured" : "failed" ) << endl;
	return EXIT_SUCCESS;
}
