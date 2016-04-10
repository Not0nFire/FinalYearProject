#include <include/Collision/CollisionGroup.hpp>
#include <Thor/Vectors.hpp>
#include <iostream>

namespace collision {
	CollisionGroup::CollisionGroup() {
		
	}

	CollisionGroup::~CollisionGroup() {
		
	}

	bool CollisionGroup::checkPair(std::shared_ptr<Collidable> const &first, std::shared_ptr<Collidable> const &second, sf::Vector2f &minTranslation) const {
		if (first == second) {
			return false;	//don't check the same object against itself
		}
		assert(first->getMask() != second->getMask());

		bool collisionOccured = true;

		std::vector<sf::Vector2f> axies;
		axies.reserve(first->getPointCount() + second->getPointCount());

		//get the axies of each Collidable's mask
		first->getAxies(axies);
		second->getAxies(axies);

		float overlap = FLT_MAX;
		sf::Vector2f mtv;	//minimum translation vector

		for (sf::Vector2f& axis : axies)
		{
			//Project both shapes onto this axis
			sf::Vector2f A = first->projectOntoAxis(axis);
			sf::Vector2f B = second->projectOntoAxis(axis);

			float axisOverlap = 0;

			//Check for overlap between the projected shapes
			if (A.x >= B.x && A.x <= B.y)
			{
				axisOverlap = B.y - A.x;
			}
			else if (A.y >= B.x && A.y <= B.y)
			{
				axisOverlap = A.y - B.x;
			}
			else
			{
				//Overlap must occur on all axies or else collision did not occur.
				collisionOccured = false;
				break;
			}

			//If the overlap on this axis is smaller than the one we have already
			if (axisOverlap < overlap)
			{
				overlap = axisOverlap;

				//Overlap on this axis is smallest so far, so update the mtv to use this axis
				mtv = axis;
			}
		}//foreach

		if (collisionOccured)
		{
			//We multiply the (normalized) axis by the overlap to get an accurate minimum translation vector (to seperate the object with the smallest force)
			minTranslation = thor::unitVector(mtv) * overlap;
		}

		return collisionOccured;
	}

	void CollisionGroup::check() {

		sf::Vector2f mtv;	//minimum translation vector
		for (auto &first : mMembers)
		{
			for (auto &second : mMembers)	//Inefficient, pairs are checked twice and each axis is used at least twice.
			{
				if (checkPair(first, second, mtv))
				{
					first->onCollide(second, -mtv);
					second->onCollide(first, mtv);
				}
			}//for second
		}//for first
	}

	void CollisionGroup::add(std::shared_ptr<Collidable> const &entry) {
		mMembers.push_back(entry);
	}

	void CollisionGroup::remove(std::shared_ptr<Collidable> const &entry) {
		auto end = mMembers.end();
		auto itr = std::remove(mMembers.begin(), end, entry);
		if (itr != end) {
			mMembers.erase(itr);
		}
		else
		{
			std::cout << "CollisionGroup: Tried to erase list element, but could not find it." << std::endl;
		}
	}

	void CollisionGroup::checkAgainst(std::shared_ptr<Collidable> &other) {
		sf::Vector2f mtv;
		for (auto &member : mMembers)
		{
			if (checkPair(other, member, mtv))
			{
				member->onCollide(other, mtv);
				other->onCollide(member, -mtv);
			}
		}
	}
}
