#ifndef _NODE_PATH_H
#define _NODE_PATH_H

#include <include/TinyXML2/tinyxml2.h>

#include "Node.hpp"

/*!
\brief A one-way path of nodes.
Uses a recursive structure.
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

	//! Returns the next node in the path after current.
	static Node* getNext(Node* current);

	//! Returns the starting node of the path.
	Node* begin() const;

private:
	Node* mRoot;
};
#endif