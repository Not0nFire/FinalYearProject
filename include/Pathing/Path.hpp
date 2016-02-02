#ifndef _NODE_PATH_H
#define _NODE_PATH_H

#include <include/TinyXML2/tinyxml2.h>

#include "Node.hpp"

/*!
A one-way path of nodes.
*/
class Path {
public:
	//! Constructs a hard-coded path.
	Path();

	/*!
	Constructs a path from xml.
	*/
	explicit Path(const tinyxml2::XMLElement* const xml);

	~Path();

	static Node* getNext(Node* current);

	Node* begin() const;

private:
	Node* mRoot;
};
#endif