#ifndef _QUADTREE_H
#define _QUADTREE_H

#include <SFML/Graphics/Rect.hpp>
#include <memory>
#include <functional>
#include <vector>
using std::vector;

template<class T>
class Quadtree {
private:
	typedef std::shared_ptr<T*> T_ptr;
	enum Node {
		NW = 0,
		NE = 1,
		SW = 2,
		SE = 3,
		NODE_COUNT = 4
	};

	bool mIsLeaf;
	Quadtree<T_ptr>* mNodes;
	vector<T_ptr> mItems;
	sf::IntRect mBounds;

	bool contains(const T_ptr item, bool* whollyContained) const;

	static const unsigned int MAX_ITEMS;

public:
	Quadtree<T>(int left, int top, int width, int height);
	Quadtree<T>(sf::IntRect const &bounds);
	~Quadtree<T>();

	void insert(T_ptr item);
	void clear();

	vector<T*> query(float x, float y);
	bool contains(int x, int y) const;

	void subdivide(vector<T_ptr> items, std::function<bool(T_ptr)> predicate);
};

template <class T>
Quadtree<T>::Quadtree(int left, int top, int width, int height) :
mIsLeaf(true),
mNodes(nullptr),
mItems(),
mBounds(left, top, width, height)
{
	mItems.reserve(Node::NODE_COUNT);
}

template <class T>
Quadtree<T>::Quadtree(sf::IntRect const &bounds) :
mIsLeaf(true),
mNodes(nullptr),
mItems(),
mBounds(bounds)
{
	mItems.reserve(Node::NODE_COUNT);
}

template <class T>
Quadtree<T>::~Quadtree() {
}

template <class T>
void Quadtree<T>::insert(T_ptr item) {
	if (mIsLeaf) {
		mItems.push_back(item);
	}


}

template <class T>
void Quadtree<T>::clear() {
	mItems.clear();

	if (!mIsLeaf) {
		for (int i = 0; i < Node::NODE_COUNT; ++i) {
			mNodes[i].clear();
		}
	}
}

template <class T>
vector<T*> Quadtree<T>::query(float x, float y) {
	_ASSERT(mBounds.contains(x, y));

	vector<T*> returnedItems(mItems);

	for (int i = 0; i < Node::NODE_COUNT; ++i) {
		if (mNodes[i].contains(x, y)) {
			returnedItems.push_back(mNodes[i].query(x, y));
			break;	//Breaking because only one of the child nodes will contain the x,y point
		}
	}

	return returnedItems;
}

template <class T>
bool Quadtree<T>::contains(int x, int y) const {
	return mBounds.contains(x, y);
}

template <class T>
bool Quadtree<T>::contains(const T_ptr item, bool* whollyContained) const {
	bool isContained = false;

	if (mBounds.intersects(item->getBounds())) {
		isContained = true;

		if (whollyContained != nullptr) {
			auto itemBounds = item->getBounds();
			*whollyContained = (
				itemBounds.left >= mBounds.left &&
				itemBounds.top >= mBounds.top &&
				itemBounds.left + itemBounds.width <= mBounds.left + mBounds.width &&
				itemBounds.top + itemBounds.height <= mBounds.top + mBounds.height
				);
		}//end if(!nullptr)

	}//end if(intersects)

	return isContained;
}

template <class T>
void Quadtree<T>::subdivide(vector<T_ptr> items, std::function<bool(T_ptr)> predicate) {
	throw "Not Implemented -- Quadtree<T>::subdivide(...)";
}
#endif