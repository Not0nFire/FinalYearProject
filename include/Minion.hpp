#ifndef _MINION_H
#define _MINION_H

#include <include/Pawn.hpp>
#include <include/Pathing/Path.hpp>
#include <list>

/*!
\brief Pawn that follows a path, keeps its distance from flockmembers and has a monetary reward for killing it.
*/
class Minion : public Pawn {
private:
	std::weak_ptr<const Node> mPathNode;
	sf::Vector2f mPathWaypoint;

	//! Shared pointer to flock.
	std::shared_ptr<std::list<std::weak_ptr<Pawn>>> mFlock;

	//! Steers Minion away from local flockmates.
	sf::Vector2f separation() const;
	//! Steers Minion toward local flockmates.
	sf::Vector2f cohesion() const;

	//! Amout of money to award player when this Minion dies.
	const unsigned int mMonetaryValue;

	bool mReachedEndOfPath;

	const float M_DESTINATION_WEIGHT;
	const float M_SEPARATION_WEIGHT;
	const float M_COHESION_WEIGHT;
	const float M_SEEK_WEIGHT;
	

protected:
	//! Calls separation() to steer away from local flockmates and then invokes Pawn::doMarch()
	void doMarch(sf::Vector2f const& goalDisplacement, float secondsElapsed) override;

	sf::Vector2f seekEnemy();

public:
	Minion(tinyxml2::XMLElement* xml);
	virtual ~Minion();	//!< Removes self from mFlock

	/*!
	\brief Sets the path that the Minion will follow.
	\param pathNode First node in that path that we should follow.
	*/
	void setPath(std::shared_ptr<const Node> const& pathNode);
	void clearPath();

	//! Adds ourself to the flock and stores a shared pointer to the flock.
	void addToFlock(std::shared_ptr<std::list<std::weak_ptr<Pawn>>> const& flock);

	//! Extends Pawn::update() to make Minion follow path.
	virtual void update(sf::Time const &elapsedTime) override;

	//! Gets the amount of money that this Minion is worth.
	unsigned int getMonetaryValue() const;

	bool reachedEndOfPath() const;
};
#endif