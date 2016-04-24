#ifndef _COLLISION_GROUP_H
#define _COLLISION_GROUP_H

#include "Collidable.hpp"
#include <memory>

namespace collision {

	class CollisionGroup {
	private:
		std::vector<std::weak_ptr<Collidable>> mMembers;

		//Checks first against second using SAT, setting minTranslation to be the seperating force and returning true if collision occurred.
		//In the case that no collision occurrs, minTranslation is untouched.
		bool checkPair(Collidable* first, Collidable* second, sf::Vector2f &minTranslation) const;

		void cullExpiredMembers();

	public:
		CollisionGroup();
		~CollisionGroup();

		void check(float deltaTime);
		void add(std::shared_ptr<Collidable> const &entry);

		bool checkAgainst(std::shared_ptr<Collidable> &other);
	};
}
#endif