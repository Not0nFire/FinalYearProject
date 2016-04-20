#include <include/Minion.hpp>

Minion::Minion(tinyxml2::XMLElement* xml) :
Pawn(xml->FirstChildElement("Pawn")),
mMonetaryValue(atoi(xml->FirstChildElement("MoneyValue")->GetText())),
mReachedEndOfPath(false),
M_DESTINATION_WEIGHT(atof(xml->FirstChildElement("PathingWeights")->Attribute("destination"))),
M_SEPARATION_WEIGHT(atof(xml->FirstChildElement("PathingWeights")->Attribute("separation"))),
M_COHESION_WEIGHT(atof(xml->FirstChildElement("PathingWeights")->Attribute("cohesion"))),
M_SEEK_WEIGHT(atof(xml->FirstChildElement("PathingWeights")->Attribute("enemy_seek")))
{
	_ASSERT(std::string(xml->Name()) == "Minion");
}

Minion::~Minion() {
	////remove ourselves from the flock
	//if (mFlock) {
	//	auto itr = find(mFlock->begin(), mFlock->end(), self);
	//	if (itr != mFlock->end()) {
	//		mFlock->erase(itr);
	//	}
	//}
}

sf::Vector2f Minion::separation() const {
	static float desiredSeparation = 50.f;

	sf::Vector2f steer(0, 0);
	float count = 0;

	for (auto itr = mFlock->begin();
		itr != mFlock->end();
		++itr)
	{
		if (auto flockMember = itr->lock()){
			if (self.lock() != flockMember && flockMember->getFaction() == getFaction()) {
				sf::Vector2f diff = this->getPosition() - flockMember->getPosition();
				float distance = thor::length(diff);
				if (distance < desiredSeparation)
				{
					//diff = thor::unitVector(diff);
					diff /= distance;
					steer += diff;
					++count;
				}
			}//end if(this != *itr)
		}
	}//end for

	//average out the steering
	if (count > 1)
	{
		steer /= count;
	}

	return steer;
}

sf::Vector2f Minion::cohesion() const {
	static float neighbourDist = 100;
	sf::Vector2f sum(0, 0);
	float count = 0;

	for (auto itr = mFlock->begin();
		itr != mFlock->end();
		++itr)
	{
		if (auto flockMember = itr->lock()) {
			if (self.lock() != flockMember) {
				//Is the flockmember close enough to us?
				sf::Vector2f diff = flockMember->getPosition() - getPosition();
				float distance = thor::length(diff);
				if (distance < neighbourDist)
				{
					diff /= distance;
					sum += diff;
					++count;
				}
			}
		}
	}

	if (count > 0)
	{
		sum /= count;

		return sum;
	}

	return sf::Vector2f(0, 0);
}

void Minion::doMarch(sf::Vector2f const& goalDisplacement, float secondsElapsed) {
	sf::Vector2f steer = thor::unitVector(goalDisplacement) * M_DESTINATION_WEIGHT;
	if (mFlock) {
		sf::Vector2f sep = separation();
		sf::Vector2f coh = cohesion();
	
		steer += sep * M_SEPARATION_WEIGHT;
		steer += coh * M_COHESION_WEIGHT;
	
	
		steer += seekEnemy() * M_SEEK_WEIGHT;
	
		turnToFaceDirection(getPosition() + steer);
	}
	
	Pawn::doMarch(steer, secondsElapsed);
}

sf::Vector2f Minion::seekEnemy() {
	static float range = 120.f;

	sf::Vector2f result = sf::Vector2f();

	//if combat target is null, dead or far away: find a new target
	if (mCombatTarget.expired()|| mCombatTarget.lock()->isDead() || thor::length(mCombatTarget.lock()->getPosition() - getPosition()) > range) {

		for (auto itr = mFlock->begin();
			itr != mFlock->end();
			++itr)
		{
			//Find a nearby enemy
			if (auto flockMember = itr->lock()) {
				if (flockMember != self.lock()) {
					auto displacement = flockMember->getPosition() - getPosition();
					auto distance = thor::length(displacement);
					if (distance > 0 && distance <= range) {

						stopWaiting();	//cancel out any remaining wait time

						////if the member is the same faction as us...
						//if (flockMember->getFaction() == getFaction())
						//{
						//	auto target = flockMember->getCombatTarget();
						//	if (nullptr != target) {
						//		//flockmember is an ally. Attack their target.
						//		beTaunted(flockMember->getCombatTarget());
						//		displacement = mCombatTarget->getPosition() - getPosition();
						//		setDestination(mCombatTarget->getPosition());
						//		mState = MARCHING;
						//		result = thor::unitVector(displacement);
						//	}
						//}
						//else //member is an enemy
						if (flockMember->getFaction() != getFaction())
						{
							//flockmember is an enemy. Attack them.
							beTaunted(flockMember);
							setDestination(mCombatTarget.lock()->getPosition());
							mState = MARCHING;
							
							result = thor::unitVector(displacement);
						}
					}//end if (length(displacement) <= range)
				}//end if (flockmember != self)
			}//end if (flockmember = itr->lock)
		}//end for (itr)
	}//end if (mCombatTarget)
	else if(auto target = mCombatTarget.lock()){
		//return the direction to the our target
		result = thor::unitVector(target->getPosition() - getPosition());
	}

	return result;
}

void Minion::setPath(std::shared_ptr<const Node> const& pathNode) {
	mPathNode = pathNode;
	mPathWaypoint = pathNode->getPoint();
}

void Minion::clearPath() {
	mPathNode.reset();
}

void Minion::addToFlock(std::shared_ptr<std::list<std::weak_ptr<Pawn>>> const& flock) {
	_ASSERT(!self.expired());
	flock->emplace_back(self.lock());
	mFlock = flock;
}

void Minion::update(sf::Time const& elapsedTime) {

	if (thor::length(getDestination() - getPosition()) <= 40.f) {
		//If the destination we've reached is the one given to us by the path...
		if (getDestination() == mPathWaypoint) {
			//Travel along path.
			if (auto node = mPathNode.lock()) {
				auto nextNode = node->getNext();
				if (nextNode) {
					mPathNode = nextNode;
					mPathWaypoint = nextNode->getPoint();
				}
				else {
					mReachedEndOfPath = true;
				}
			}
		}// end if (destination == path waypoint)

		//if waypoint is a non-zero location
		if (mPathWaypoint != sf::Vector2f()) {
			setDestination(mPathWaypoint);
		}
	}

	Pawn::update(elapsedTime);

	//If we're standing around idle
	if (mState == IDLE) {
		//Keep an eye out for enemies we can attack
		seekEnemy();
	}
}

unsigned Minion::getMonetaryValue() const {
	return mMonetaryValue;
}

bool Minion::reachedEndOfPath() const {
	return mReachedEndOfPath;
}
