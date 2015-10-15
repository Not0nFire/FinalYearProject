#include <include\Pawn.h>

Pawn::Pawn() {

}
Pawn::Pawn(sf::Texture &texture) {

}
Pawn::Pawn(const char* xml) {

}
Pawn::~Pawn() {

}

bool Pawn::decay(sf::Time const &elapsedTime) {
	mDecayTime -= elapsedTime;
	return mDecayTime > sf::Time::Zero;
}

void Pawn::turnToFaceDestination() {

}

sf::Vector2f Pawn::getDestination() const {
	return mDestination;
}
void Pawn::setDestination(sf::Vector2f const &destination) {
	mDestination = destination;
}

Pawn::MovementSpeed Pawn::getMovementSpeed() const {
	return mMovementSpeed;
}
void Pawn::setMovementSpeed(Pawn::MovementSpeed newSpeed) {
	mMovementSpeed = newSpeed;
}

void Pawn::update(sf::Time const &elapsedTime) {
	switch (mState) {
	case IDLE:
		animate(elapsedTime);
	case MARCHING:
	case ATTACKING:
	case STUNNED:
	case DYING:
	case DEAD:
		decay(elapsedTime);
	default:
		break;
	}
}

void Pawn::kill() {
	mState = State::DYING;
}
void Pawn::heal(int healAmount) {

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
}

bool Pawn::hasDecayed() const {
	return mDecayed;
}