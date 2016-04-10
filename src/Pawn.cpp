#include <include\Pawn.hpp>

#define GET_ELEMENT(str) xml->FirstChildElement(str)->GetText()
Pawn::Pawn(tinyxml2::XMLElement* xml) :
Actor(xml->FirstChildElement("Actor")),
mFaction(std::string(GET_ELEMENT("Faction")) == "ENEMY" ? Faction::ENEMY : Faction::PLAYER),
mState(IDLE),
M_MAX_HEALTH(atoi(GET_ELEMENT("Health"))),
mArmour(atof(GET_ELEMENT("Armour"))),
mMagicResist(atof(GET_ELEMENT("MagicResist"))),
mDamageType(std::string(GET_ELEMENT("DamageType")) == "PHYSICAL" ? Damage::Type::PHYSICAL : Damage::Type::MAGICAL),
mAttackRange(atoi(GET_ELEMENT("AttackRange"))),
mMovementSpeed(atoi(GET_ELEMENT("MovementSpeed"))),
mAttackDamage(atoi(GET_ELEMENT("AttackDamage"))),
mAttacksPerSecond(atof(GET_ELEMENT("AttacksPerSecond"))),
mTimeSinceAttack(FLT_MAX),
mCombatTarget(nullptr),
mStunDuration(sf::seconds(0.f))
{
	_ASSERT(std::string(xml->Name()) == "Pawn");

	mHealth = M_MAX_HEALTH;

	mAttackSound.setBuffer(ResourceManager<sf::SoundBuffer>::instance()->get(GET_ELEMENT("AttackSound")));

	playAnimation("idle", true);
}

Pawn::~Pawn() {

}

//bool Pawn::decay(sf::Time const &elapsedTime) {
//	mDecayTime -= elapsedTime;
//	return mDecayTime > sf::Time::Zero;
//}

void Pawn::turnToFaceDestination() {
	sf::Vector2f scale = getScale();
	float posX = getPosition().x;
	float faceThis = mState == ATTACKING ? mCombatTarget->getPosition().x : mDestination.x;	//face target if attacking

	//mirror the sprite, making it face the right way
	if ((faceThis < posX && scale.x > 0) ||
		faceThis > posX && scale.x < 0)
	{
		setScale(scale.x * -1, scale.y);
	}
}

void Pawn::calculateAnimation() {
	switch (mState)
	{
	case IDLE:
		if (isPlayingAnimation() && getPlayingAnimation() != "idle")
			playAnimation("idle", true);
		break;
	case MARCHING:
		if (isPlayingAnimation() && getPlayingAnimation() != "walk")
			playAnimation("walk", true);
		break;
	case ATTACKING:
		if (isPlayingAnimation() && getPlayingAnimation() != "attack")
			playAnimation("attack", true);
		break;
	case STUNNED: break;
	case DEAD:
		if (isPlayingAnimation() && getPlayingAnimation() != "death")
			playAnimation("death", false);
		break;
	default: break;
	}
}

void Pawn::calculateState(sf::Vector2f const &goalDisplacement) {
	//don't bother updating state if dead
	if (mState != State::DEAD && mState != State::STUNNED) {

		//attack if in range
		if (mCombatTarget != nullptr &&
			!mCombatTarget->isDead() &&
			(thor::length(this->getPosition() - mCombatTarget->getPosition()) <= mAttackRange)
			)
		{
				mState = State::ATTACKING;
		}

		//proceed toward goal
		else if (thor::length(goalDisplacement) > 5) {
			mState = State::MARCHING;

		}

		else {
			mState = State::IDLE;
		}
	}
}

void Pawn::doAttack(float secondsElapsed) {
	_ASSERT(mState == State::ATTACKING);

	//Check if we have a target
	if (mCombatTarget) {
		mTimeSinceAttack += secondsElapsed;

		//Check if it's time to attack.
		if (mTimeSinceAttack >= 1 / mAttacksPerSecond) {
			//Deal damage to our target
			mCombatTarget->takeDamage(mAttackDamage, mDamageType);
			mTimeSinceAttack = 0.0f;
			mAttackSound.play();
			setDebugColour(sf::Color::Yellow);
		}
		else if (mTimeSinceAttack >= 1 / mAttacksPerSecond / 2.0f){
			setDebugColour(sf::Color::Green);
		}
	}
	else {
		std::cout << "State is ATTACKING, but no combat target!" << std::endl;
	}
}

void Pawn::doMarch(sf::Vector2f const& goalDisplacement, float secondsElapsed) {
	//move towards destination
	if (thor::length(goalDisplacement) > 0) {
		move(
			thor::unitVector(goalDisplacement) * static_cast<float>(mMovementSpeed) * secondsElapsed
			);
	}
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
//	boost::lock_guard<boost::mutex> lock(mMutex);

	sf::Vector2f distanceToGoal = mDestination - getPosition();

	calculateState(distanceToGoal);

	switch (mState) {
	case IDLE:
		setDebugColour(sf::Color::Black);
		break;
	case MARCHING:
		doMarch(distanceToGoal, elapsedTime.asSeconds());
		setDebugColour(sf::Color::Cyan);
		break;
	case ATTACKING:
		doAttack(elapsedTime.asSeconds());
		break;
	case STUNNED:
		setDebugColour(sf::Color::Red);
		mTimeStunned += elapsedTime;
		if (mTimeStunned >= mStunDuration)
		{
			mState = IDLE;
		}
		break;
	case DEAD:
		setDebugColour(sf::Color::White);
		break;
	default:
		setDebugColour(sf::Color::Magenta);
		break;
	}

	turnToFaceDestination();

	calculateAnimation();

	animate(elapsedTime);

	updateCollidableMask(getPosition());
}

void Pawn::kill() {
	mHealth = 0;
	mState = State::DEAD;
	if (mOnDeathFunction) {
		mOnDeathFunction(this);
	}
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
		if (mOnDeathFunction) {
			mOnDeathFunction(this);
		}
	}

	//return true if damage killed us
	return isDead;
}

bool Pawn::takeDamage(int amount, Damage::Type type, std::shared_ptr<Pawn> const &sender) {
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
void Pawn::stun(sf::Time const& duration) {
	//apply new time if it is higher than current (don't let stuns cancel each other out!)
	bool applyStun = duration > (mStunDuration - mTimeStunned);
	if (applyStun) {
		mStunDuration = duration;
		mTimeStunned = sf::seconds(0.f);
		mState = STUNNED;
	}
}

void Pawn::beTaunted(std::shared_ptr<Pawn> const &taunter) {
		mCombatTarget = taunter;
}

bool Pawn::offerTarget(std::shared_ptr<Pawn> const &target) {
	bool acceptedTarget = true;

	if (target->getFaction() == this->getFaction()) {
		acceptedTarget = false;
	}
	else if (thor::length(target->getPosition() - this->getPosition()) > mAttackRange) {
		acceptedTarget = false;
	}
	else if (target->isDead()) {
		acceptedTarget = false;
	}
	else if (target.get() == this) {
		//std::cout << "Offered self as target..." << std::endl;
		acceptedTarget = false;
	}
	else if (mCombatTarget == nullptr) {
		beTaunted(target);
	}
	//if current target is dead or far away...
	else if (mCombatTarget->isDead() || thor::length(mCombatTarget->getPosition() - this->getPosition()) > mAttackRange) {
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

void Pawn::onCollide(std::shared_ptr<Collidable> &other, sf::Vector2f const& mtv) {
	auto projectile = std::dynamic_pointer_cast<Projectile, Collidable>(other);
	if (projectile) {
		takeDamage(projectile->getDamage(), projectile->getDamageType());
		printf("Projectile hit me! %p\n", other.get());
	} else {
		auto pawn = std::dynamic_pointer_cast<Pawn, Collidable>(other);
		if (pawn)
		{
			offerTarget(pawn);
		}
		move(mtv);
	}
}

void Pawn::setOnDeath(std::function<void(Pawn*)> const& callback) {
	mOnDeathFunction = callback;
}

//bool Pawn::hasDecayed() const {
//	return mDecayed;
//}