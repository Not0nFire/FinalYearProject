#include <include/ObjectPool.h>
#include <iostream>

template <class T>
ObjectPool<T>::ObjectPool() {
}

template <class T>
ObjectPool<T>::~ObjectPool() {

	if (inUse.size() > 0) {
		std::cout << "WARNING! Destructor called with object still in use!" << std::endl;
	}

	for (auto itr = inUse.begin(); itr != inUse.end(); ++itr) {
		delete *itr;
	}
	inUse.clear();

	for (auto itr = free.begin(); itr != free.end(); ++itr) {
		delete *itr;
	}
	free.clear();
}

template <class T>
T* ObjectPool<T>::request() {
	//if no free objects, create a new one
	if (free.size() == 0) {
		inUse.push_back(new T());
	} else {
		//push a free object onto the inUse list and remove it from the free list
		inUse.push_back(free.back());
		free.pop_back();
	}

	return inUse.back();
}

template <class T>
template <typename Func, class... Args>
T* ObjectPool<T>::request(Func predicate, Args&& ...ctorArgs) {
	auto itr = std::find(free.begin(), free.end(), predicate);
	if (itr == free.end()) {
		inUse.emplace_back(ctorArgs);
	}

	//Stopped coding here --> there has to be a better way than using ellipses!
}

template <class T>
void ObjectPool<T>::release(T* val) {
	inUse.remove(val);
	free.push_back(val);
}
