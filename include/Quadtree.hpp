#ifndef _QUADTREE_H
#define _QUADTREE_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <functional>
#include <vector>
#include <sstream>
using std::vector;

class Quadtree {
private:
	sf::IntRect mBounds;
	unsigned char mData;	//bitfield
	unsigned int mLevel;

	Quadtree *mParent;
	Quadtree *NW, *NE, *SW, *SE;	//child nodes
	bool mIsLeaf;

public:
	Quadtree(int x, int y, unsigned int width, unsigned int height, Quadtree* parent = nullptr);

	~Quadtree();

	sf::IntRect getBounds() const;
	unsigned char getData() const;
	unsigned int getLevel() const;
	bool isLeaf() const;

	void setData(unsigned char newData);

	void subdivide(std::function<bool(Quadtree* node)> &predicate);

	void appendToStringStream(std::stringstream &ss);
};

inline Quadtree::Quadtree(int x, int y, unsigned int width, unsigned int height, Quadtree* parent) :
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

inline Quadtree::~Quadtree() {
	//delete child nodes
	if (NW) delete NW;
	if (NE) delete NE;
	if (SW) delete SW;
	if (SE) delete SE;
}

inline sf::IntRect Quadtree::getBounds() const {
	return mBounds;
}

inline unsigned char Quadtree::getData() const {
	return mData;
}

inline unsigned Quadtree::getLevel() const {
	return mLevel;
}

inline bool Quadtree::isLeaf() const {
	return mIsLeaf;
}

inline void Quadtree::setData(unsigned char newData) {
	mData = newData;
}

//EXAMPLE PREDICATE
//	unsigned char nodeData = node->getData();
//	if((nodeData & TerrainData.Grass) && (nodeData & TerrainData.Path)) {
//		return true;
//	}
inline void Quadtree::subdivide(std::function<bool(Quadtree* node)> &predicate) {
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

inline void Quadtree::appendToStringStream(std::stringstream& ss) {
	if (mLevel == 0) {
		ss << "QuadTree {\n";
	}
	for (int i = 0; i < mLevel; ++i) {
		ss << "\t";
	}

	ss << "~ " << (mData & 0x01 ? "G" : "") << (mData & 0x02 ? "P" : "") << '\n';

	if (NW) {
		NW->appendToStringStream(ss);
		NE->appendToStringStream(ss);
		SW->appendToStringStream(ss);
		NE->appendToStringStream(ss);
	}
	if (mLevel == 0) {
		ss << "}";
	}
}
#endif	//include guard