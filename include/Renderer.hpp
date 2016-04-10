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
private:
	sf::RenderWindow mWindow;

	std::atomic<bool> mLoopOngoing;	/*!< True if want to keep rendering, false if we want rendering to stop. */
	std::chrono::milliseconds frameDelay;

	std::thread mThread;	/*!< Thread that rendering takes place on. */

	std::mutex mMutex;

	void render();

public:
	/*!
	Creates a render window and sets the scene to be rendered.
	*/
	Renderer(sf::VideoMode mode,	//RenderWindow ctor arguments
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

	std::thread& getThread();
	const sf::RenderWindow& getWindow() const;

	void setScene(SceneProxy* newScene);

	bool pollEvent(sf::Event &event);
};
#endif