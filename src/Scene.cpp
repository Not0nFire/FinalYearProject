#include "include/Scene.hpp"

SceneProxy::SceneProxy(std::function<I_Scene*()> const& sceneCreationFunc) :
mScene(nullptr),
mCreatSceneFunc(sceneCreationFunc)
{
	//empty ctor body
}

SceneProxy::~SceneProxy() {
	delete mScene;
}

bool SceneProxy::handleEvent(sf::Event& sfEvent) {
	std::lock_guard<std::mutex> lock(mMutex);
	if (mScene == nullptr)
	{
		mScene = mCreatSceneFunc();
	}

	return mScene->handleEvent(sfEvent);
}

void SceneProxy::update(sf::Time const& elapsedTime) {
	std::lock_guard<std::mutex> lock(mMutex);
	if (mScene == nullptr)
	{
		mScene = mCreatSceneFunc();
	}

	mScene->update(elapsedTime);
}

void SceneProxy::draw(sf::RenderWindow& w) {
	std::lock_guard<std::mutex> lock(mMutex);
	if (mScene == nullptr)
	{
		mScene = mCreatSceneFunc();
	}

	mScene->draw(w);
}

void SceneProxy::cleanup() {
	std::lock_guard<std::mutex> lock(mMutex);
	//assumes mScene has been created
	mScene->cleanup();
}

void SceneProxy::loadNow() {
	std::lock_guard<std::mutex> lock(mMutex);
	_ASSERT(mScene == nullptr);

	mScene = mCreatSceneFunc();
}

I_Scene* SceneProxy::getScene() {
	std::lock_guard<std::mutex> lock(mMutex);
	return mScene;
}