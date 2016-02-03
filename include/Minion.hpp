#ifndef _MINION_H
#define _MINION_H

#include <include/Pawn.hpp>
#include <include/Pathing/Path.hpp>

/*!
\brief Pawn that follows a path, keeps its distance from flockmembers and has a monetary reward for killing it.
*/
class Minion : public Pawn {
private:
	//Variable pointer to const object.
	const Node* mPathNode;

	//! Pointer to flock of Minions.
	std::shared_ptr<std::list<Minion*>> mFlock;

	//! Steers Minion away from local flockmates.
	sf::Vector2f separation() const;
	sf::Vector2f cohesion() const;	//!< CURRENTLY UNUSED. Not fully implemented. Possible unneeded.

	//! Amout of money to award player when this Minion dies.
	const unsigned int mMonetaryValue;

protected:
	//! Calls separation() to steer away from local flockmates and then invokes Pawn::doMarch()
	void doMarch(sf::Vector2f const& goalDisplacement, float secondsElapsed) override;

public:
	Minion(tinyxml2::XMLElement* xml);
	virtual ~Minion();	//!< Removes self from mFlock

	/*!
	\brief Sets the path that the Minion will follow.
	\param pathNode Variable pointer to const Node object.
	*/
	void setPath(const Node* pathNode);

	//! Adds ourself to the flock and stores a shared pointer to the flock.
	void addToFlock(std::shared_ptr<std::list<Minion*>> flock);

	//! Extends Pawn::update() to make Minion follow path.
	virtual void update(sf::Time const &elapsedTime) override;

	//! Gets the amount of money that this Minion is worth.
	unsigned int getMonetaryValue() const;
};
#endif