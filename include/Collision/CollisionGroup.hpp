#ifndef _COLLISION_GROUP_H
#define _COLLISION_GROUP_H

#include "Collidable.hpp"
//#include "CollisionInfo.hpp"

namespace collision {

	class CollisionGroup {
	private:
		std::vector<Collidable*> mMembers;
		std::vector<Collidable*>::iterator mItr;

		void cullNullMembers();

		//Checks first against second using SAT, setting minTranslation to be the seperating force and returning true if collision occurred.
		//In the case that no collision occurrs, minTranslation is untouched.
		bool checkPair(Collidable* first, Collidable* second, sf::Vector2f &minTranslation);

	public:
		CollisionGroup();
		~CollisionGroup();

		void check();
		void add(Collidable* entry);
		void remove(Collidable* entry);
	};
}
#endif