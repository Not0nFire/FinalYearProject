#include <include/Squad.hpp>

Squad::Squad() {
}

Squad::Squad(int vectorReserve) {
	mMembers.reserve(vectorReserve);
}

Squad::~Squad() {
	
}

unsigned Squad::getSize() const {
	return mMembers.size();
}

void Squad::recruit(Minion&& newMember) {
	mMembers.push_back(newMember);
}

inline void Separation(std::vector<Minion> &boids) {
	float desiredSeparation = 20;

	for (int i = 0; i < boids.size(); i++) {
		sf::Vector2f steer(0, 0);
		int count = 0;

		for (int j = 0; j < boids.size(); j++) {
			sf::Vector2f diff = boids[j].getPosition() - boids[i].getPosition();
			float d = thor::length(diff);

			if (d > 0 && d < desiredSeparation) {
				diff = thor::unitVector(diff);
				steer += diff;
				count++;
			}
		}

		if (count > 0) {
			steer /= static_cast<float>(count);
		}

		if (thor::length(steer) > 0) {
			steer = thor::unitVector(steer);
			steer *= static_cast<float>(boids[i].getMovementSpeed());
			boids[i].setDestination(boids[i].getDestination() + steer);
		}
	}
}

void Squad::update(sf::Time const& elapsedTime) {
	Separation(mMembers);
	for (Minion member : mMembers) {
		member.update(elapsedTime);
	}
}