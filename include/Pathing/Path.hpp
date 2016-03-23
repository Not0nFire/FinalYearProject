#ifndef _NODE_PATH_H
#define _NODE_PATH_H

#include <include/TinyXML2/tinyxml2.h>

#include "Node.hpp"
#include <memory>

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
	explicit Path(tinyxml2::XMLElement* xml);

	~Path();

	//! Returns the next node in the path after current.
	static std::shared_ptr<Node> getNext(std::shared_ptr<Node> current);

	//! Returns the starting node of the path.
	std::shared_ptr<Node> begin() const;

private:
	std::shared_ptr<Node> mRoot;
};
#endif