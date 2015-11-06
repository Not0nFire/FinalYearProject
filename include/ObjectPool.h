#ifndef _OBJECT_POOL_H
#define _OBJECT_POOL_H

#include <list>

//Simple ObjectPool implementation with little error checking.
//Beware the elipses!
template< class T >
class ObjectPool<T> {
private:
	std::list<T*> inUse;
	std::list<T*> free;

public:
	ObjectPool<T>();
	~ObjectPool<T>();

	//Request an object from the pool
	T* request();

	template <typename Func, class... Args>		//Avoid using elipses if at all possible! Danger Danger!
	T* request(Func predicate, Args&&... ctorArgs);	//Using elipses so that objects without default constructors can be created.

	//Return an object back to the pool
	void release(T* val);
};
#endif