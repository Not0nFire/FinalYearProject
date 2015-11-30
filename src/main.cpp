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
#include <include/TerrainInterpreter.h>

unsigned int count;

bool myPredicate(Quadtree* node) {
	bool subdivide = false;

	TerrainInterpreter tI = TerrainInterpreter("./res/img/terrain.bmp");
	sf::IntRect nB = node->getBounds();

	node->setData(tI.interpretArea(nB.left, nB.top, nB.width, nB.height));

	if ((node->getData() & TerrainInterpreter::GRASS) && (node->getData() & TerrainInterpreter::PATH)) {
		subdivide = true;
		count++;
		std::cout << count << std::endl;
	}

	return subdivide;
}

int main() {
	count = 0;
	Quadtree myTree(0.f, 0.f, 1000.f, 1000.f);

	std::function<bool(Quadtree*)> f = &myPredicate;
	myTree.subdivide(f);

	std::stringstream ss = std::stringstream();
	myTree.appendToStringStream(ss);
	std::cout << ss.str() << std::endl;

	std::cin.get();
	
	Game game;
	return game.run();
}