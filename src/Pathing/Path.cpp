#include <include/Pathing/Path.hpp>

Path::Path() :
mRoot(new Node(0.f, 130.f, 80.f))
{
	mRoot
		->append(new Node(250.f, 190.f, 100.f))
		->append(new Node(360.f, 370.f, 70.f))
		->append(new Node(580.f, 380.f, 20.f))
		->append(new Node(780.f, 180.f, 50.f))
		->append(new Node(1024.f, 200.f, 100.f));
		//->append(mRoot);
}

Path::~Path() {
	delete mRoot;
}

Node* Path::getNext(Node* current) {
	return current->getNext();
}

Node* Path::begin() const {
	return mRoot;
}
