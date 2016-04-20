#include <include/Pathing/Path.hpp>

Path::Path() :
mRoot(std::make_shared<Node>(0.f, 130.f, 80.f))
{
	mRoot
		->append(std::make_shared<Node>(250.f, 190.f, 100.f))
		->append(std::make_shared<Node>(360.f, 370.f, 70.f))
		->append(std::make_shared<Node>(580.f, 380.f, 20.f))
		->append(std::make_shared<Node>(780.f, 180.f, 50.f))
		->append(std::make_shared<Node>(1024.f, 200.f, 100.f));
		//->append(mRoot);
}

Path::Path(tinyxml2::XMLElement* xml) {
	//The XMLElement is constant, the pointer value is NOT.
	auto xmlnode = xml->FirstChildElement("Node");

	//We'll be using atof to convert the c-strings that GetText() returns into useful floats.
	mRoot = std::make_shared<Node>(
		atof(xmlnode->FirstChildElement("X")->GetText()),
		atof(xmlnode->FirstChildElement("Y")->GetText()),
		atof(xmlnode->FirstChildElement("Radius")->GetText())
		);
	
	//Iterate through all sibling elements of <Node> under <Path>
	for (xmlnode = xmlnode->NextSiblingElement();
		xmlnode != nullptr;
		xmlnode = xmlnode->NextSiblingElement()) {

		//Append new node onto end of path (append is recursive)
		mRoot->append(std::make_shared<Node>(
			atof(xmlnode->FirstChildElement("X")->GetText()),
			atof(xmlnode->FirstChildElement("Y")->GetText()),
			atof(xmlnode->FirstChildElement("Radius")->GetText())
			));
	}//end for loop
}

Path::~Path() {
}

std::shared_ptr<Node> Path::getNext(std::shared_ptr<Node> current) {
	return current->getNext();
}

std::shared_ptr<const Node> Path::begin() const {
	return mRoot;
}
