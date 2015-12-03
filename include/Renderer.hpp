#ifndef _RENDERER_H
#define _RENDERER_H

#include <include/Actor.hpp>
#include <boost/atomic.hpp>
#include <boost/thread.hpp>
#include <chrono>
#include "Scene.hpp"

using namespace boost;

/*!
Manages rendering in it's own thread.
*/
class Renderer {
private:
	sf::RenderWindow mWindow;

	atomic<bool> mLoopOngoing;	/*!< True if want to keep rendering, false if we want rendering to stop. */
	std::chrono::milliseconds frameDelay;

	I_Scene* mSceneToRender;

	thread mThread;	/*!< Thread that rendering takes place on. */

	boost::mutex mMutex;

	void render();

public:
	/*!
	Creates a render window and sets the scene to be rendered.
	*/
	Renderer(I_Scene* sceneToRender,
			sf::VideoMode mode,	//RenderWindow ctor arguments
			std::string const &title,
			sf::Uint32 style = sf::Style::Titlebar,
			sf::ContextSettings& settings = sf::ContextSettings()
	);

	~Renderer();

	/*!
	\brief Begins rendering on a seperate thread.
	Starts the render loop, creating a thread that runs the render() method until stopRenderLoop() is called.
	*/
	void startRenderLoop(const unsigned int framesPerSecond);

	/*!
	\brief Stops render loop.
	Sets render loop condition to false and waits for thread to join.
	*/
	void stopRenderLoop();

	boost::thread& getThread();
	const sf::RenderWindow& getWindow() const;

	void setScene(I_Scene* newScene);

	bool pollEvent(sf::Event &event);
};
#endif