#ifndef _QUADTREE_H
#define _QUADTREE_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <functional>
#include <vector>
using std::vector;

template<class T>
class Quadtree {
private:
	typedef std::shared_ptr<T*> T_ptr;
	typedef std::unique_ptr<Quadtree<T>> Node;
	
	enum Nodes {
		NW = 0,
		NE,
		SW,
		SE,
		NODE_COUNT
	};

	bool mIsLeaf;

	vector<T_ptr> mItems;
	sf::IntRect mBounds;

	vector<Node> mNodes;

	bool contains(const T_ptr item, bool* whollyContained) const;

#ifdef _DEBUG
	sf::RectangleShape mShape;
#endif	//_DEBUG

public:
	Quadtree<T>(int left, int top, int width, int height);
	Quadtree<T>(sf::IntRect const &bounds);
	~Quadtree<T>();

	//Inserts an item into the tree at the deepest node that wholly contains the object
	void insert(T_ptr item);

	//Clears all items from this node and all child nodes
	void clear();

	//Recursively gets all items in this node and in all child nodes.
	vector<T_ptr>&& getItems() const;

	vector<T_ptr>&& query(float x, float y);
	bool contains(int x, int y) const;

	//Subdivides this node and all child nodes upon a given predicate.
	void subdivide(std::function<bool(Quadtree* node)> predicate);

	sf::IntRect getBounds() const;

#ifdef _DEBUG
	void draw(sf::RenderTarget &target);
#endif	//_DEBUG
};

template <class T>
Quadtree<T>::Quadtree(int left, int top, int width, int height) :
mIsLeaf(true),
mItems(),
mBounds(left, top, width, height)
{
	mNodes.reserve(NODE_COUNT);
#ifdef _DEBUG
	mShape = sf::RectangleShape(sf::Vector2f(mBounds.width, mBounds.height));
	mShape.setPosition(mBounds.left, mBounds.top);
	mShape.setFillColor(sf::Color::Transparent);
	mShape.setOutlineColor(sf::Color::Magenta);
	mShape.setOutlineThickness(1.f);
#endif	//_DEBUG
}

template <class T>
Quadtree<T>::Quadtree(sf::IntRect const &bounds) :
mIsLeaf(true),
mItems(),
mBounds(bounds)
{
	mNodes.reserve(NODE_COUNT);
#ifdef _DEBUG
	mShape = sf::RectangleShape(sf::Vector2f(mBounds.width, mBounds.height));
	mShape.setPosition(mBounds.left, mBounds.top);
	mShape.setFillColor(sf::Color::Transparent);
	mShape.setOutlineColor(sf::Color::Magenta);
	mShape.setOutlineThickness(1.f);
#endif	//_DEBUG
}

template <class T>
Quadtree<T>::~Quadtree() {
}

template <class T>
void Quadtree<T>::insert(T_ptr item) {
	if (mIsLeaf) {
		mItems.push_back(item);
	} else {
		//give item to a child node
	}
}

template <class T>
void Quadtree<T>::clear() {
	mItems.clear();

	if (!mIsLeaf) {
		for (Node n : mNodes) {
			n->clear();
		}
	}
}

template <class T>
vector<typename Quadtree<T>::T_ptr>&& Quadtree<T>::getItems() const {
	vector<T_ptr> returnedItems = vector<T_ptr>(mItems);

	for (Node n : mNodes) {
		returnedItems.emplace_back(n->getItems());
	}

	return std::move(returnedItems);
}

template <class T>
vector<typename Quadtree<T>::T_ptr>&& Quadtree<T>::query(float x, float y) {
	_ASSERT(mBounds.contains(x, y));

	vector<T*> returnedItems(mItems);

	for (Node n : mNodes) {
		if (n->contains(x, y)) {
			returnedItems.push_back(n->query(x, y));
			break;	//Breaking because only one of the child nodes will contain the x,y point
		}
	}

	return std::move(returnedItems);
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
void Quadtree<T>::subdivide(std::function<bool(Quadtree* node)> predicate) {

	if (mIsLeaf && predicate(this)) {
		mIsLeaf = false;

		//Create child nodes
		unsigned int halfWidth = mBounds.width / 2;
		unsigned int halfHeight = mBounds.height / 2;
		mNodes[NW] = new Quadtree<T>(mBounds.left, mBounds.top, halfWidth, halfHeight);
		mNodes[NE] = new Quadtree<T>(mBounds.left + halfWidth, mBounds.top, halfWidth, halfHeight);
		mNodes[SW] = new Quadtree<T>(mBounds.left, mBounds.top + halfHeight, halfWidth, halfHeight);
		mNodes[SE] = new Quadtree<T>(mBounds.left + halfWidth, mBounds.top + halfHeight, halfWidth, halfHeight);

		for (T_ptr item : mItems) {
			for (Node n : mNodes) {
				
				//Move items to child nodes (only if wholly contained??).
				//(if we give this node the item, break so that we don't try to give it to another node)

			}//end for(each node)
		}//end for(each item)
	}//end if(leaf && predicate)
}

template <class T>
sf::IntRect Quadtree<T>::getBounds() const {
	return mBounds;
}

#ifdef _DEBUG
template <class T>
void Quadtree<T>::draw(sf::RenderTarget& target) {
	if (mIsLeaf) {
		target.draw(mShape);
	} else {
		for (Node n : mNodes) {
			n->draw(target);
		}
	}
}
#endif	//_DEBUG
#endif	//include guard