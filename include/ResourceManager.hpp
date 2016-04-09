#ifndef _RESOURCE_MANAGER_H
#define _RESOURCE_MANAGER_H
#include <memory>
#include <map>
#include <mutex>
#include <Thor/Animations.hpp>
#include <include/TinyXML2/tinyxml2.h>

/*!
\brief Simple SFML resource manager.
*/
template<typename T>
class ResourceManager {
private:
	static ResourceManager<T>* mInstance;
	ResourceManager<T>();

	/*!
	Map of strings to unique resource pointers.
	*/
	std::map< std::string, std::unique_ptr<T>> mResources;

	std::mutex mResourceMutex;

public:
	~ResourceManager<T>();

	/*!
	\brief Instanciates (or returns the instance of) the ResourceManager.
	Instanciates (or gets the existing instance of) the ResourceManager<T> where T = an SFML resource type.
	Each instance of ResourceManager manages a different type of resource (e.g. ResourceManager<Texture>, ResourceManager<Font>).
	*/
	static ResourceManager<T>* instance();
	
	/*!
	\brief Gets a resource by its path in the file system.
	If the resource specified by path has not already been loaded:
		Loads the resource, puts the resource into the map (with path as the key), returns the loaded resource.
	If the key "path" exists in the map it means the resource has already been loaded, so just return the resource.
	\param path The location of the resource in the file system.
	\returns A reference to the requested resource.
	*/
	T& get(std::string const &path);
};

template<typename T>
ResourceManager<T>* ResourceManager<T>::mInstance = nullptr;

template<typename T>
ResourceManager<T>::ResourceManager() {

}

template<typename T>
ResourceManager<T>::~ResourceManager() {

}

template <typename T>
ResourceManager<T>* ResourceManager<T>::instance() {
	if (!mInstance) {
		mInstance = new ResourceManager<T>();
	}
	return mInstance;
}

template<typename T>
T& ResourceManager<T>::get(std::string const &path) {
	//lock the mutex
	std::lock_guard<std::mutex> lock(mResourceMutex);

	//if path does not exist as a key in the map
	if (!mResources.count(path)) {
		//load resource from path
		std::unique_ptr<T> res(new T);
		res->loadFromFile(path);

		//put the resource into the map (move it instead of copying)
		mResources.insert(std::make_pair(path, std::move(res)));
	}

	//return reference to the texture
	return *mResources.at(path);
}//unlock the mutex here when lock_guard goes out of scope

//Specialized template for Thor's FrameAnimations.
template<>
inline thor::FrameAnimation& ResourceManager<thor::FrameAnimation>::get(std::string const &path) {
	//Lock the mutex
	std::lock_guard<std::mutex> lock(mResourceMutex);

	//If path does not exist as a key in the map
	if (!mResources.count(path)) {
		std::unique_ptr<thor::FrameAnimation> anim(new thor::FrameAnimation());
		
		//Load animation from path
		using namespace tinyxml2;
		XMLDocument doc;
		XMLError result = doc.LoadFile(path.c_str());
		if (result != XML_NO_ERROR) {
			throw result;
		}

		XMLElement* root = doc.FirstChildElement("Animation");

		//For each <Frame> element under <Animation>
		for (XMLElement* frameElement = root->FirstChildElement("Frame");
			frameElement != nullptr;
			frameElement = frameElement->NextSiblingElement("Frame"))
		{
			//Get relative duration of frame
			float duration = atof(frameElement->FirstChildElement("Duration")->GetText());

			//Get rectangle to be used as frame
			sf::IntRect subrect = sf::IntRect(
				atoi(frameElement->FirstChildElement("X")->GetText()),
				atoi(frameElement->FirstChildElement("Y")->GetText()),
				atoi(frameElement->FirstChildElement("Width")->GetText()),
				atoi(frameElement->FirstChildElement("Height")->GetText())
				);

			//Add the frame to the animation
			anim->addFrame(duration, subrect);
		}

		//Put the animation into the map (move it instead of copying)
		mResources.insert(make_pair(path, move(anim)));
	}

	//return reference to the animation
	return *mResources.at(path);
}//Unlock the mutex here when lock_guard goes out of scope
#endif