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
	frameDelay = 1000 / framesPerSecond;

	mThread = thread( bind(&Renderer::render, this) );
	//render();
}

void Renderer::stopRenderLoop() {
	mLoopOngoing = false;
	mThread.join();
}

void Renderer::render() {
	sf::Clock clock;

	mWindow.setActive(true);

	while (mLoopOngoing) {

		mMutex.lock();	//Block until ownership can be obtained

		mWindow.clear();

		mSceneToRender->draw(mWindow);

		mMutex.unlock();	//Release the mutex

		mWindow.display();

		clock.restart();

		this_thread::sleep_for(chrono::milliseconds(frameDelay));

	}//end while(mLoop)
}

thread& Renderer::getThread() {
	return mThread;
}

const sf::RenderWindow& Renderer::getWindow() const {
	return mWindow;
}

void Renderer::setScene(I_Scene* newScene) {
	mSceneToRender = newScene;
}


bool Renderer::pollEvent(sf::Event& event) {
	return mWindow.pollEvent(event);
}