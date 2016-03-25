#ifndef _COLLISION_GROUP_H
#define _COLLISION_GROUP_H

#include "Collidable.hpp"
#include <memory>
//#include "CollisionInfo.hpp"

namespace collision {

	class CollisionGroup {
	private:
		std::vector<std::shared_ptr<Collidable>> mMembers;
		std::vector<std::shared_ptr<Collidable>>::iterator mItr;

		//Checks first against second using SAT, setting minTranslation to be the seperating force and returning true if collision occurred.
		//In the case that no collision occurrs, minTranslation is untouched.
		bool checkPair(std::shared_ptr<Collidable> const &first, std::shared_ptr<Collidable> const &second, sf::Vector2f &minTranslation) const;

	public:
		CollisionGroup();
		~CollisionGroup();

		void check();
		void add(std::shared_ptr<Collidable> const &entry);
		void remove(std::shared_ptr<Collidable> const &entry);

		void checkAgainst(std::shared_ptr<Collidable> &other);
	};
}
#endif