#ifndef _QUADTREE_H
#define _QUADTREE_H

#include <functional>
#include <SFML/Graphics.hpp>

template<class T>
class Quadtree {
private:
	sf::IntRect mBounds;
	T mData;	//bitfield
	unsigned int mLevel;

	Quadtree<T> *mParent;
	Quadtree<T> *NW, *NE, *SW, *SE;	//child nodes
	bool mIsLeaf;

public:
	Quadtree<T>(int x, int y, unsigned int width, unsigned int height, Quadtree* parent = nullptr);

	~Quadtree<T>();

	sf::IntRect getBounds() const;
	T getData() const;
	unsigned int getLevel() const;
	bool isLeaf() const;

	void setData(T newData);

	void subdivide(std::function<bool(Quadtree<T>* node)> &predicate);
};

template<class T>
Quadtree<T>::Quadtree(int x, int y, unsigned int width, unsigned int height, Quadtree* parent) :
mBounds(x, y, width, height),
mData(),
mParent( parent ),
mIsLeaf(true)
{
	NW = NE = SW = SE = nullptr;

	if (parent) {	//if we have a parent, our level is one more than theirs
		mLevel = parent->getLevel() + 1;
	} else {
		mLevel = 0;	//we are the root node
	}
}

template<class T>
Quadtree<T>::~Quadtree() {
	//delete child nodes
	if (NW) delete NW;
	if (NE) delete NE;
	if (SW) delete SW;
	if (SE) delete SE;
}

template<class T>
sf::IntRect Quadtree<T>::getBounds() const {
	return mBounds;
}

template<class T>
T Quadtree<T>::getData() const {
	return mData;
}

template<class T>
unsigned Quadtree<T>::getLevel() const {
	return mLevel;
}

template<class T>
bool Quadtree<T>::isLeaf() const {
	return mIsLeaf;
}

template<class T>
void Quadtree<T>::setData(T newData) {
	mData = newData;
}

//EXAMPLE PREDICATE
//	unsigned char nodeData = node->getData();
//	if((nodeData & TerrainData.Grass) && (nodeData & TerrainData.Path)) {
//		return true;
//	}
template<class T>
void Quadtree<T>::subdivide(std::function<bool(Quadtree* node)> &predicate) {
	_ASSERT(mIsLeaf);

	if (predicate(this)) {
		float halfWidth = mBounds.width / 2.f;
		float halfHeight = mBounds.height / 2.f;

		mIsLeaf = false;

		//Create child nodes (setting parent to be this)
		NW = new Quadtree(mBounds.left, mBounds.top, halfWidth, halfHeight, this);
		NE = new Quadtree(mBounds.left + halfWidth, mBounds.top, halfWidth, halfHeight, this);
		SW = new Quadtree(mBounds.left, mBounds.top + halfHeight, halfWidth, halfHeight, this);
		SE = new Quadtree(mBounds.left + halfWidth, mBounds.top + halfHeight, halfWidth, halfHeight, this);

		NW->setData(mData);
		NE->setData(mData);
		SW->setData(mData);
		SE->setData(mData);

		NW->subdivide(predicate);
		NE->subdivide(predicate);
		SW->subdivide(predicate);
		SE->subdivide(predicate);
	}
}
#endif	//include guard