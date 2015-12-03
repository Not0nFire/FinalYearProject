#ifndef _RESOURCE_MANAGER_H
#define _RESOURCE_MANAGER_H
#include <boost/thread.hpp>
#include <memory>
#include <map>

/*!
Simple SFML resource manager.
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

	boost::mutex mResourceMutex;

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
	T& get(std::string path);
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
T& ResourceManager<T>::get(std::string path) {
	//lock the mutex
	boost::lock_guard<boost::mutex> lock(mResourceMutex);

	//if path does not exist as a key in the map
	if (!mResources.count(path)) {
		//load texture from path
		std::unique_ptr<T> res(new T);
		res->loadFromFile(path);

		//put the texture into the map (move it instead of copying)
		mResources.insert(std::make_pair(path, std::move(res)));
	}

	//return reference to the texture
	return *mResources.at(path);
}//unlock the mutex here when lock_guard goes out of scope
#endif