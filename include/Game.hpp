#ifndef _GAME_H
#define _GAME_H

#include <include/Renderer.hpp>
#include <include/Level.hpp>

using namespace boost;
using namespace signals2;

class Game {
private:
	Renderer mRenderer;
	signal<void(sf::Vector2f)> onMouseClick;

	bool mRun, mPaused;

	void handleEvent(sf::Event &event);

	Level mLevel;

public:
	Game();
	~Game();

	int run();
};
#endif