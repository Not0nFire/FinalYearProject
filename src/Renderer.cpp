#include <include/Renderer.hpp>

Renderer::Renderer(I_Scene* _sceneToRender, sf::VideoMode mode, std::string const &title, sf::Uint32 style, sf::ContextSettings& settings)
	: mWindow(mode, title, style, settings), mSceneToRender(_sceneToRender) {

	mWindow.setActive(false);
}

Renderer::~Renderer() {
	
}

void Renderer::startRenderLoop(const unsigned int framesPerSecond) {
	mLoopOngoing = true;

	//Calculate delay (in milliseconds) from frames per second
	frameDelay = std::chrono::milliseconds(1000 / framesPerSecond);

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

	while (mLoopOngoing) {

		//fixed timestep
		if (clock.now() - lastFrameTime >= frameDelay) {
			lastFrameTime = clock.now();

			mMutex.lock();	//Block until ownership can be obtained

			mWindow.clear();

			mSceneToRender->draw(mWindow);

			mWindow.display();

			mMutex.unlock();	//Release the mutex
		}

	}//end while(mLoop)

	mWindow.close();
}

std::thread& Renderer::getThread() {
	return mThread;
}

const sf::RenderWindow& Renderer::getWindow() const {
	return mWindow;
}

void Renderer::setScene(I_Scene* newScene) {
	std::lock_guard<std::mutex> lock(mMutex);
	mSceneToRender = newScene;
}


bool Renderer::pollEvent(sf::Event& event) {
	std::lock_guard<std::mutex> lock(mMutex);
	return mWindow.pollEvent(event);
}