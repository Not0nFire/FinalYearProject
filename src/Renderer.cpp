#include <include/Renderer.hpp>

Renderer::Renderer(std::string const &title, sf::VideoMode mode, sf::Uint32 style, sf::ContextSettings settings) :
mWindow(mode, title, style, settings)
{
	mWindow.setActive(false);
}

Renderer::~Renderer() {
	
}

void Renderer::startRenderLoop(const unsigned int framesPerSecond) {
	mLoopOngoing = true;

	//Calculate delay (in milliseconds) from frames per second
	mFrameDelay = std::chrono::milliseconds(1000 / framesPerSecond);

	mThread = std::thread( std::bind(&Renderer::render, this) );
	//render();
}

void Renderer::stopRenderLoop() {
	mLoopOngoing = false;
	mThread.join();
}

void Renderer::render() {
	std::chrono::steady_clock clock;
	auto lastFrameTime = clock.now();

	mWindow.setActive(true);
	mWindow.resetGLStates();	//required for sfgui (text displays as blocks without this line)

	mWindow.setMouseCursorVisible(false);

	auto originalView = mWindow.getView();

	while (mLoopOngoing) {

		//fixed timestep
		if (clock.now() - lastFrameTime >= mFrameDelay) {
			lastFrameTime = clock.now();

			//Scope the lock_guard
			{
				std::lock_guard<std::mutex> lock(mMutex); //Block until ownership can be obtained
				mWindow.clear();
				SceneManager::instance()->drawCurrentScene(mWindow);
			}

			mWindow.setView(originalView);
			Cursor::draw(mWindow);

			mWindow.display();
			
		}

	}//end while(mLoop)

	mWindow.close();
}

bool Renderer::pollEvent(sf::Event& event) {
	std::lock_guard<std::mutex> lock(mMutex);
	return mWindow.pollEvent(event);
}