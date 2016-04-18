#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <atomic>
#include <mutex>
#include <chrono>
#include <thread>
#include <functional>
#include "SceneManager.hpp"
#include "Cursor.h"

/*!
\brief Manages rendering in it's own thread.
*/
class Renderer {
public:
	/*!
	\brief Creates a render window.
	*/
#if defined _DEBUG //Default to fullscreen windowed while debugging
	Renderer(std::string const &title,	//RenderWindow ctor arguments
		sf::VideoMode mode = { 1000, 1000 },//sf::VideoMode::getDesktopMode(),
		sf::Uint32 style = sf::Style::None,
		sf::ContextSettings settings = sf::ContextSettings()
		);
	
#else
	Renderer(std::string const &title,	//RenderWindow ctor arguments
		sf::VideoMode mode = sf::VideoMode::getDesktopMode(),
		sf::Uint32 style = sf::Style::Fullscreen,
		sf::ContextSettings settings = sf::ContextSettings()
		);
#endif

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

	bool pollEvent(sf::Event &event);

private:
	sf::RenderWindow mWindow;

	std::atomic<bool> mLoopOngoing;	//!< True if want to keep rendering, false if we want rendering to stop.
	std::chrono::milliseconds mFrameDelay;

	std::thread mThread;	//!< Thread that rendering takes place on.

	std::mutex mMutex;

	void render();
};
#endif