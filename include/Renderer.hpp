#ifndef _RENDERER_H
#define _RENDERER_H

#include <include/Actor.hpp>
#include <boost/atomic.hpp>
#include <boost/thread.hpp>
#include "Scene.hpp"
#include <SFGUI/SFGUI.hpp>

using namespace boost;

class Renderer {
private:
	sf::RenderWindow mWindow;

	atomic<bool> mLoopOngoing;	//True if want to keep rendering, false if we want rendering to stop.
	unsigned int frameDelay;	//Delay between frames in milliseconds

	I_Scene* mSceneToRender;

	thread mThread;	//Thread that rendering takes place on.

	boost::mutex mMutex;

	sfg::SFGUI mSFGUI;	//There should only be one SFGUI in the program and it should not be global.

	void render();

public:
	Renderer(I_Scene* sceneToRender,
			sf::VideoMode mode,	//RenderWindow ctor arguments
			std::string const &title,
			sf::Uint32 style = sf::Style::Titlebar,
			sf::ContextSettings& settings = sf::ContextSettings()
	);

	~Renderer();

	void startRenderLoop(const unsigned int framesPerSecond);
	void stopRenderLoop();

	boost::thread& getThread();
	const sf::RenderWindow& getWindow() const;

	void setScene(I_Scene* newScene);

	bool pollEvent(sf::Event &event);
};
#endif