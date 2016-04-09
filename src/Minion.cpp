#include <include/Minion.hpp>

Minion::Minion(tinyxml2::XMLElement* xml) :
Pawn(xml->FirstChildElement("Pawn")),
mMonetaryValue(atoi(xml->FirstChildElement("MoneyValue")->GetText()))
{
	_ASSERT(std::string(xml->Name()) == "Minion");
}

Minion::~Minion() {
	//remove ourselves from the flock
	if (mFlock) {
		auto itr = std::find(mFlock->begin(), mFlock->end(), this);
		if (itr != mFlock->end()) {
			mFlock->erase(itr);
		}
	}
}

sf::Vector2f Minion::separation() const {
	static float desiredSeparation = 50.f;

	sf::Vector2f steer(0, 0);
	float count = 0;

	//For every predator in the flock, check if it's too close
	for (auto itr = mFlock->begin();
		itr != mFlock->end();
		++itr)
	{
		Minion* flockMember = *itr;
		if (this != flockMember && flockMember->getFaction() == this->getFaction()) {
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
		Minion* flockMember = *itr;
		if (this != flockMember) {
			//Is the flockmember close enough to us?
			sf::Vector2f diff = this->getPosition() - flockMember->getPosition();
			float distance = thor::length(diff);
			if (distance < neighbourDist)
			{
				sum += diff;
				++count;
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
		sf::Vector2f steer = thor::unitVector(goalDisplacement);
	if (mFlock) {
		sf::Vector2f sep = separation();
		sf::Vector2f coh = cohesion();

		steer += sep;
		//steer += coh;
	}

	Pawn::doMarch(steer, secondsElapsed);
}

void Minion::setPath(std::shared_ptr<const Node> &pathNode) {
	mPathNode = pathNode;
}

void Minion::addToFlock(std::shared_ptr<std::list<Minion*>> flock) {
	flock->push_back(this);
	mFlock = flock;	//store a reference to the flock
}

void Minion::update(sf::Time const& elapsedTime) {

	//Travel along path.
	if (auto node = mPathNode.lock()) {
		if (thor::length(getDestination() - getPosition()) <= 20.f) {
			auto nextNode = node->getNext();
			if (nextNode) {
				mPathNode = nextNode;
				setDestination(nextNode->getPoint());
			}
		}
	}

	Pawn::update(elapsedTime);
}

unsigned Minion::getMonetaryValue() const {
	return mMonetaryValue;
}
