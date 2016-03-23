#ifndef ABILITY_HPP
#define ABILITY_HPP

#include <include/Collision/Collidable.hpp>
#include <include/Collision/CollisionGroup.hpp>

class Ability : public collision::Collidable {
public:
	Ability();
	virtual ~Ability();

	virtual void execute(collision::CollisionGroup const &group) = 0;

	virtual void onCollide(Collidable* other, sf::Vector2f const& mtv) override = 0;
};
#endif