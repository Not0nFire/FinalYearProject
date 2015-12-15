#include <include/Pathing/Node.hpp>

Node::Node(float x, float y, float radius) :
mPosition(x, y),
mRadius(radius)
{
	
}

Node::~Node() {
	delete mNext;
}

sf::Vector2f Node::getCentre() const {
	return mPosition;
}

sf::Vector2f Node::getPoint() const {
	float x = mPosition.x + mRadius * cosf(rand() % 360);
	float y = mPosition.y + mRadius * sinf(rand() % 360);

	//printf("Random point in circle (%f, %f) with radius %f: (%f, %f).\n", mPosition.x, mPosition.y, mRadius, x, y);

	return sf::Vector2f(x, y);
}

Node* Node::getNext() const {
	return mNext;
}

Node* Node::append(Node* newNode) {

	if (mNext) {
		mNext->append(newNode);
	} else {
		mNext = newNode;
	}

	return newNode;
}
