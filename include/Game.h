#ifndef _GAME_H
#define _GAME_H

#include <boost/thread.hpp>
#include <boost/atomic.hpp>
#include <SFML/System.hpp>

using namespace boost;

class Game {
private:
	thread mUpdateThread;
	thread mDrawThread;
	void updateLoop();

	atomic<bool> mUpdating, mDrawing;	//bools for loops

	void stopDrawing();

	//sf::RenderWindow window;
public:
	Game();
	~Game();

	int run();
};
#endif