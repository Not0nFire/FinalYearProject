#include <include\Pawn.hpp>
#include <boost/thread/lock_guard.hpp>

Pawn::Pawn(sf::Texture &texture, Faction faction) :
Actor(texture, new sf::CircleShape(20, 8), sf::Vector2f(-20.0f, 10.0f)),
mFaction(faction),
mState(State::IDLE),
M_MAX_HEALTH(100),
mArmour(Damage::Reduction::NONE),
mMagicResist(Damage::Reduction::NONE),
mDamageType(Damage::Type::PHYSICAL),
mAttackRange(50),
mMovementSpeed(50),
mAttackDamage(10),
mAttacksPerSecond(1.0f),
mTimeSinceAttack(FLT_MAX),
mStunDuration(),
mDestination(),
mCombatTarget(nullptr)
{
	mHealth = M_MAX_HEALTH;
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

	//don't bother updating state if dead
	if (mState != State::DEAD) {
		//proceed toward goal
		if (thor::length(distanceToGoal) > mAttackRange) {
			mState = State::MARCHING;
			mCombatTarget = nullptr;
		}
		//attack if in range
		else if (mCombatTarget != nullptr) {
			if (thor::length(this->getPosition() - mCombatTarget->getPosition()) <= mAttackRange) {
				mState = State::ATTACKING;
			}
		} else {
			mState = State::IDLE;
		}
	}

	switch (mState) {
	case IDLE:
		setDebugColour(sf::Color::Black);
		break;
	case MARCHING:
		//move towards destination
		move(
			thor::unitVector(distanceToGoal) * static_cast<float>(mMovementSpeed)* elapsedTime.asSeconds()
			);
		setDebugColour(sf::Color::Cyan);
		break;
	case ATTACKING:
		//Check if we have a target
		if (mCombatTarget) {
			mTimeSinceAttack += elapsedTime.asSeconds();

			//Check if it's time to attack.
			if (mTimeSinceAttack >= 1/mAttacksPerSecond) {
				//Deal damage to our target
				mCombatTarget->takeDamage(mAttackDamage, mDamageType, this);
				mTimeSinceAttack = 0.0f;
				setDebugColour(sf::Color::Yellow);
			}
			else if (mTimeSinceAttack >= 1/mAttacksPerSecond / 2.0f){
				setDebugColour(sf::Color::Green);
			}
		}
		else {
			std::cout << "State is ATTACKING, but no combat target!" << std::endl;
		}
		break;
	case STUNNED:
		setDebugColour(sf::Color::Red);
		break;
	case DEAD:
		setDebugColour(sf::Color::White);
		break;
	default:
		setDebugColour(sf::Color::Magenta);
		break;
	}

	updateCollidableMask(getPosition());
}

void Pawn::kill() {
	mHealth = 0;
	mState = State::DEAD;
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

	bool isDead = mHealth <= 0;

	if (isDead) {
		mState = State::DEAD;
	}

	//return true if damage killed us
	return isDead;
}

bool Pawn::takeDamage(int amount, Damage::Type type, Pawn* sender) {
	if (mCombatTarget == nullptr) {
		mCombatTarget = sender;
	} else if (thor::length(mCombatTarget->getPosition() - this->getPosition()) > mAttackRange) {
		mCombatTarget = sender;
	}

	return takeDamage(amount, type);
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

void Pawn::beTaunted(Pawn* taunter) {
	mCombatTarget = taunter;
}

bool Pawn::offerTarget(Pawn* target) {
	bool acceptedTarget = true;
	if (target->isDead()) {
		acceptedTarget = false;
	}
	else if (target == this) {
		//std::cout << "Offered self as target..." << std::endl;
		acceptedTarget = false;
	}
	else if (mCombatTarget == nullptr) {
		beTaunted(target);
	}
	else if (mCombatTarget->isDead()) {
		beTaunted(target);
	}

	return acceptedTarget;
}

bool Pawn::targetIsDead() const {
	bool isDead = true;

	//nullptr is good as dead
	if (mCombatTarget) {	//	!= nullptr
		isDead = mCombatTarget->isDead();
	}
	return isDead;
}

int Pawn::getHealth() const {
	return mHealth;
}

Pawn::State Pawn::getState() const {
	return mState;
}

bool Pawn::isDead() const {
	return mState == State::DEAD;
}

Pawn::Faction Pawn::getFaction() const {
	return mFaction;
}

void Pawn::onCollide(Collidable* other, sf::Vector2f const& mtv) {
	Projectile* projectile = dynamic_cast<Projectile*>(other);
	if (projectile) {
		takeDamage(projectile->getDamage(), projectile->getDamageType());
		std::cout << "\nTower hit" << std::endl;
	} else {
		move(mtv *.5f);	//By no means perfect but it prevents all pawns piling onto each other and that's all we need right now.
	}
}

//bool Pawn::hasDecayed() const {
//	return mDecayed;
//}