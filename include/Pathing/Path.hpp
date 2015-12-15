#ifndef _NODE_PATH_H
#define _NODE_PATH_H

#include "Node.hpp"

/*!
A one-way path of nodes.
*/
class Path {
public:
	//! Constructs a hard-coded path.
	Path();

	/*!
	Intended for use with tinyXML2.
	Constructs a path from xml.
	*/
	Path(const char* xmlPath);

	~Path();

	static Node* getNext(Node* current);

	Node* begin() const;

private:
	Node* mRoot;
};
#endif