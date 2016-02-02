#ifndef _MINION_H
#define _MINION_H

#include <include/Pawn.hpp>
#include <include/Pathing/Path.hpp>

class Minion : public Pawn {
private:
	//Variable pointer to const object.
	const Node* mPathNode;

public:
	Minion(tinyxml2::XMLElement* xml);
	virtual ~Minion();

	/*!
	\brief Sets the path that the Minion will follow.
	\param pathNode Variable pointer to const Node object.
	*/
	void setPath(const Node* pathNode);

	virtual void update(sf::Time const &elapsedTime) override;
};
#endif