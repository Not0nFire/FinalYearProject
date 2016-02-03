#ifndef _MINION_H
#define _MINION_H

#include <include/Pawn.hpp>
#include <include/Pathing/Path.hpp>

class Minion : public Pawn {
private:
	//Variable pointer to const object.
	const Node* mPathNode;

	std::shared_ptr<std::list<Minion*>> mFlock;

	sf::Vector2f separation() const;
	sf::Vector2f cohesion() const;

	const unsigned int mMonetaryValue;

protected:
	void doMarch(sf::Vector2f const& goalDisplacement, float secondsElapsed) override;

public:
	Minion(tinyxml2::XMLElement* xml);
	virtual ~Minion();

	/*!
	\brief Sets the path that the Minion will follow.
	\param pathNode Variable pointer to const Node object.
	*/
	void setPath(const Node* pathNode);

	void addToFlock(std::shared_ptr<std::list<Minion*>> flock);

	virtual void update(sf::Time const &elapsedTime) override;

	unsigned int getMonetaryValue() const;
};
#endif