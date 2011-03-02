#include "sorainit.hpp"

#pragma comment(linker, "/NODEFAULTLIB:libcmt.lib")
#pragma comment(linker, "/NODEFAULTLIB:libcmtd.lib")

int APIENTRY WinMain(HINSTANCE hInstance,
					   HINSTANCE hPrevInstance,
					   LPSTR    lpCmdLine,
					   int       nCmdShow) {

	soratest::init();
	soratest::run();
	soratest::halt();

	return 0;
}