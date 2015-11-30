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


#include <include/Game.hpp>
#include <include/Quadtree.hpp>

unsigned char GRASS = 0x01;
unsigned char PATH = 0x02;
unsigned int maxLevel = 9;

bool myPredicate(Quadtree* node) {
	bool subdivide = false;

	if (node->getLevel() > 2) {
		node->setData(PATH);
	}

	unsigned char nodeData = node->getData();
	if ((nodeData & GRASS) && (nodeData & PATH) && node->getLevel() < maxLevel) {
		subdivide = true;
	}

	return subdivide;
}

int main() {

	Quadtree myTree(0.f, 0.f, 1000.f, 1000.f);
	myTree.setData(GRASS | PATH);
	std::function<bool(Quadtree*)> f = &myPredicate;
	myTree.subdivide(f);
	std::stringstream ss = std::stringstream();
	myTree.appendToStringStream(ss);
	std::cout << ss.str() << std::endl;
	std::cin.get();
	
	Game game;
	return game.run();
}