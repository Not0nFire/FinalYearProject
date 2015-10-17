#ifndef _RENDERER_H
#define _RENDERER_H

#include <include/Actor.h>
#include <boost/atomic.hpp>
#include <boost/thread.hpp>

using namespace boost;

class Renderer {
private:
	typedef std::vector<Actor*>::iterator actorItr;

	sf::RenderWindow mWindow;
	//TODO: camera (sf::View?)

	//TODO: vector<Drawable*> mGuiLayer;
	std::vector<Actor*> mActorsToDraw;	//Collection of Actors to be draw onto the RenderWindow

	static bool checkIsNull(void* ptr);	//Predicate for checking if a pointer is null (used in cullNullDrawables).
	void cullNullDrawables();	//Removes all null pointers from vectors using the erase-remove idiom.

	atomic<bool> mLoopOngoing;	//True if want to keep rendering, false if we want rendering to stop.
	unsigned int frameDelay;	//Delay between frames in milliseconds

	thread mThread;	//Thread that rendering takes place on.

	boost::mutex mMutex;

	void render();





	sf::Texture testT;
	Actor* testA;





public:
	Renderer(sf::VideoMode mode,	//RenderWindow ctor arguments
			std::string const &title,
			sf::Uint32 style = sf::Style::Default,
			sf::ContextSettings& settings = sf::ContextSettings()
	);

	~Renderer();

	void startRenderLoop(const unsigned int framesPerSecond);
	void stopRenderLoop();

	boost::thread& getThread();

	bool addActor(Actor* a);	//Returns true if pointer not already in vector and was successfully added
	bool removeDrawable(Actor* a);	//Returns true if pointer was in vector and was successfully removed
	bool addGuiElement(sf::Drawable* e);
	bool removeGuiElement(sf::Drawable* e);
};
#endif