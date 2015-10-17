#if _DEBUG
#pragma comment(lib, "sfml-graphics-d.lib")
#pragma comment(lib, "sfml-main-d.lib")
#pragma comment(lib, "sfml-network-d.lib")
#pragma comment(lib, "sfml-system-d.lib")
#pragma comment(lib, "sfml-window-d.lib")
#else
#pragma comment(lib, "sfml-graphics.lib")
#pragma comment(lib, "sfml-main.lib")
#pragma comment(lib, "sfml-network.lib")
#pragma comment(lib, "sfml-system.lib")
#pragma comment(lib, "sfml-window.lib")
#endif

//#include <include/Game.h>
#include <boost/bind.hpp>
#include <boost/thread.hpp>

void testFunc(int i) {
	std::cout << "test " << i << std::endl;
}

//class testClass {
//private:
//	boost::thread t;
//public:
//	testClass() {
//		t = boost::thread(boost::bind(testClass::memberFunc, this, rand() % 10));
//	}
//
//	void memberFunc(int i) {
//		testFunc(i);
//	}
//};

int main() {
	srand(time(NULL));
	boost::thread t = boost::thread(boost::bind(testFunc, rand() % 10));
	std::cin.get();
}