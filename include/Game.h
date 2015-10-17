#ifndef _GAME_H
#define _GAME_H

#include <boost/thread.hpp>
#include <include/Renderer.h>

using namespace boost;

class Game {
private:
	Renderer mRenderer;

public:
	Game();
	~Game();

	int run();
};
#endif