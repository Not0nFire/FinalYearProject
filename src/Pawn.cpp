#include <include\Pawn.hpp>
#include <boost/thread/lock_guard.hpp>

Pawn::Pawn(sf::Texture &texture) : Actor(texture), M_MAX_HEALTH(100) {
}
//Pawn::Pawn(const char* xml) {
//
//}
Pawn::~Pawn() {

}

//bool Pawn::decay(sf::Time const &elapsedTime) {
//	mDecayTime -= elapsedTime;
//	return mDecayTime > sf::Time::Zero;
//}

void Pawn::turnToFaceDestination() {

}

sf::Vector2f Pawn::getDestination() const {
	return mDestination;
}
void Pawn::setDestination(sf::Vector2f const &destination) {
	mDestination = destination;
}

int Pawn::getMovementSpeed() const {
	return mMovementSpeed;
}
void Pawn::setMovementSpeed(int newSpeed) {
	mMovementSpeed = newSpeed;
}

void Pawn::update(sf::Time const &elapsedTime) {
	boost::lock_guard<boost::mutex> lock(mMutex);

	sf::Vector2f distanceToGoal = mDestination - getPosition();
	if (thor::length(distanceToGoal) > 0) {
		move(
			thor::unitVector(distanceToGoal) * static_cast<float>(mMovementSpeed) * elapsedTime.asSeconds()
			);
	}

	updateCollidableMask(getPosition());

	switch (mState) {
	case IDLE:
		//animate(elapsedTime);
	case MARCHING:
	case ATTACKING:
	case STUNNED:
	case DYING:
	case DEAD:
		//decay(elapsedTime);
	default:
		break;
	}
}

void Pawn::kill() {
	mHealth = 0;
	mState = State::DYING;
}

bool Pawn::takeDamage(int amount, Damage::Type type) {
	//apply relevent damage reduction
	switch (type) {
		case Damage::PHYSICAL:
			amount *= mArmour;
			break;
		case Damage::MAGICAL:
			amount *= mMagicResist;
			break;
	}//switch

	//apply damage
	mHealth -= amount;

	//return true if damage killed us
	return mHealth <= 0;
}

void Pawn::heal(int healAmount) {
	//apply heal
	mHealth += healAmount;

	//cap health
	if (mHealth > M_MAX_HEALTH) {
		mHealth = M_MAX_HEALTH;
	}
}
void Pawn::stun(sf::Time duration) {
	//apply new time if it is higher than current (don't let stuns cancel each other out!)
	mStunDuration =
		duration > mStunDuration ? duration : mStunDuration;
	mState = State::STUNNED;
}

Pawn::State Pawn::getState() const {
	return mState;
}
void Pawn::setState(State newState) {
	mState = newState;
	onStateChanged(*this);

}

void Pawn::onCollide(Collidable* other, sf::Vector2f const& mtv) {
	move(mtv *.25f);	//By no means perfect but it prevents all pawns piling onto each other and that's all we need right now.
}

//bool Pawn::hasDecayed() const {
//	return mDecayed;
//}