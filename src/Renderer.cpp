#include <include/Renderer.h>

Renderer::Renderer(sf::VideoMode mode, std::string const &title, sf::Uint32 style, sf::ContextSettings& settings)
	: mWindow(mode, title, style, settings) {
}

Renderer::~Renderer() {
	
}

bool Renderer::checkIsNull(void* ptr) {
	return ptr == nullptr;
}


void Renderer::cullNullDrawables() {
	//Erase-Remove idiom
	mActorsToDraw.erase(
		std::remove_if(
			mActorsToDraw.begin(),
			mActorsToDraw.end(),
			checkIsNull)
		);

	/*mGuiLayer.erase(
		std::remove_if(
		mGuiLayer.begin(),
		mGuiLayer.end(),
		checkIsNull)
		);*/
}

void Renderer::startRenderLoop(const unsigned int framesPerSecond) {
	mLoopOngoing = true;

	//Calculate delay (in milliseconds) from frames per second
	frameDelay = 1000 / framesPerSecond;

	mThread = thread(&render);
}

void Renderer::stopRenderLoop() {
	mLoopOngoing = false;
	mThread.join();
}

void Renderer::render() {
	sf::Clock clock;
	unsigned int elapsedMilliseconds;

	while (mLoopOngoing) {

		mMutex.lock();	//Block until ownership can be obtained (we don't want another thread modifying our vectors while we iterate through them)

		cullNullDrawables();

		//iterate through vector and draw each item to the window
		auto end = mActorsToDraw.end();
		for (auto itr = mActorsToDraw.begin();
			 itr != end;
			 ++itr) {
			
			mWindow.draw(**itr);

		}//end if(itr)

		mMutex.unlock();	//Release the mutex; We're finished with the vectors for the moment.

		//We don't want the thread to sleep any longer than it has to,
		//so we subtract the elapsed time from the desired frame delay to get a more accurate sleep time.
		elapsedMilliseconds = clock.getElapsedTime().asMilliseconds();
		clock.restart();

		//sleep until we need to render again
		this_thread::sleep_for(
			chrono::milliseconds(frameDelay - elapsedMilliseconds)
			);
	}//end while(mLoop)
}

thread& Renderer::getThread() {
	return mThread;
}

bool Renderer::addActor(Actor* a) {

	lock_guard<boost::mutex> lock(mMutex);	//mMutex is unlocked when the lock_guard object goes out of scope (unlocked in destructor)
	mActorsToDraw.push_back(a);
}

bool Renderer::removeDrawable(Actor* a) {

	lock_guard<boost::mutex> lock(mMutex);
	mActorsToDraw.erase(
		std::find(
			mActorsToDraw.begin(),
			mActorsToDraw.end(),
			a)
		);
}

//bool Renderer::addGuiElement(sf::Drawable* e) {
//	lock_guard<boost::mutex> lock(mMutex);
//	//
//}

//bool Renderer::removeGuiElement(sf::Drawable* e) {
//	lock_guard<boost::mutex> lock(mMutex);
//	
//}