#ifndef _PATH_NODE_H
#define _PATH_NODE_H

#include <SFML/Graphics.hpp>

class Node {
public:
	Node(float x, float y, float radius);

	//! Deletes itself and the next node.
	~Node();

	//! Returns the centre point of the node.
	sf::Vector2f getCentre() const;

	//! Returns a random point contained within the node.
	sf::Vector2f getPoint() const;

	//! Returns the next node in the path.
	Node* getNext() const;

	//! Recursively appends a node to the path. (i.e. If this node already has a next, it falls through to that node...)
	//! \returns The node that was appended.
	Node* append(Node* newNode);

private:
	sf::Vector2f mPosition;
	float mRadius;
	Node* mNext;
};
#endif