#ifndef QUADTREE_HPP
#define QUADTREE_HPP

#include <functional>
#include <SFML/Graphics.hpp>

//! Simple Quadtree implementation using templates.
template<class T>
class Quadtree {
private:
	sf::IntRect mBounds;
	T mData; //data at this node
	unsigned int mLevel; //how far down the tree this node is

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

	/*!
	* \brief Subdivides the quadtree on a given predicate.
	* param predicate Function to decide if the passed ndoe should be subdivided
	*/
	void subdivide(std::function<bool(Quadtree<T>* node)> const &predicate);

	/*!
	* \brief Gets the data at the given position.
	* \param x Location to query on the horizontal axis
	* \param y Location to query on the vertical axis.
	* \param dataOut[out] Data at the queried position.
	* \returns True if the point(x,y) is inside the bound of this node.
	*/
	bool query(int x, int y, T& dataOut);
};

template<class T>
Quadtree<T>::Quadtree(int x, int y, unsigned int width, unsigned int height, Quadtree* parent) :
mBounds(x, y, width, height),
mData(),
mParent(parent),
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
void Quadtree<T>::subdivide(std::function<bool(Quadtree* node)> const &predicate) {
	_ASSERT(mIsLeaf);

	//if this node should subdivide...
	if (predicate(this)) {
		const float halfWidth = mBounds.width / 2.f;
		const float halfHeight = mBounds.height / 2.f;

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

		//Subdivide child nodes
		NW->subdivide(predicate);
		NE->subdivide(predicate);
		SW->subdivide(predicate);
		SE->subdivide(predicate);
	}
}

template <class T>
bool Quadtree<T>::query(int x, int y, T& dataOut) {
	bool containsPoint = mBounds.contains(x, y);

	//If this node contains the point...
	if (containsPoint) {
		//If this node is a leaf...
		if (mIsLeaf) {
			//...set the data to be (a ptr to) this node's data
			dataOut = mData;
		} else {
			//else query each child node in turn (if the previous child node did not contain the point)
			if (!NW->query(x, y, dataOut)) {
				if (!NE->query(x, y, dataOut)) {
					if (!SW->query(x, y, dataOut)) {
						SE->query(x, y, dataOut);
					}//end if !SW->query
				}//end if !NE->query
			}//end if !NW->query
		}//end else !leaf
	}//end if containsPoint
	return containsPoint;
}
#endif	//include guard
