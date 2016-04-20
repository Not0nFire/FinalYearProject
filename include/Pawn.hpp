#ifndef _PAWN_H
#define _PAWN_H

#include <include/Actor.hpp>
#include <include/Damage.hpp>
#include <Thor/Vectors.hpp>
#include <include/Projectile.h>
#include <SFML/Audio.hpp>
#include <queue>

/*!
\brief Base class for all units in the game. If it walks and attacks, it's a Pawn.
Walks toward a destination, attacks any hostile pawns it encounters. Dies when health reaches 0.
*/
class Pawn : public Actor {
public:
	enum Faction {
		PLAYER,
		ENEMY,
		NEUTRAL
	};

	enum State {
		IDLE,
		MARCHING,
		ATTACKING,
		STUNNED,
		DEAD
	};

#pragma region Methods
	/*!
	\brief Constructs a pawn from an xml element.
	Parses members from child xml tags.
	Requires child <Actor> tag for base class construction.
	\param xml A <Pawn> xml tag, containing an <Actor> tag and tags for all member variables.
	*/
	Pawn(tinyxml2::XMLElement* xml);
	virtual ~Pawn();

	void makeSelfAware(std::shared_ptr<Pawn> const& smartThis);

	/*!
	Gets the Pawn's destination.
	*/
	sf::Vector2f getDestination() const;

	/*!
	\brief Sets the Pawn's destination.
	\remarks Pawns move straight toward their destination.
	\param destination The Pawn's new destination.
	*/
	void setDestination(sf::Vector2f const &destination);

	int getMovementSpeed() const;
	void setMovementSpeed(int newSpeed);

	/*!
	\brief Updates the Pawn.
	State becomes MARCHING is goal is further than attack range.
	State becomes ATTACKING if has a target and target is within attack range.
	Else state become IDLE.

	Moves toward goal if MARCHING.
	Attacks target if ATTACKING, taking into account attacks per second.
	Does nothing if state is somethig else.

	Calls updateCollidableMask with current position at end of method.

	\param elapsedTime The amount of time elapsed since last update tick.
	*/
	virtual void update(sf::Time const &elapsedTime);

	/*!
	\brief Kills the Pawn outright.
	Sets the Pawn's health to 0 and the state to DEAD.
	*/
	void kill();

	/*!
	Applies damage to the Pawn, taking into account Armour/MagicResist.
	\param amount The amount of damage to take.
	\param type The type of damage to take.
	*/
	bool takeDamage(int amount, Damage::Type type);

	/*!
	\brief Same as takeDamage()
	Used when taking damage from another pawn, so that we can choose to retaliate if we so wish.
	\param amount The amount of damage to take.
	\param type The type of damage to take.
	\param sender The dealer of damage, the disher of pain.
	*/
	bool takeDamage(int amount, Damage::Type type, std::shared_ptr<Pawn> const &sender);

	/*!
	\brief Heals for the specified amount.
	Heals for the specified amount. Cannot heal beyond max health.
	\param amount The number of health points to heal for.
	*/
	void heal(int amount);

	/*!
	\brief Get stunned.
	Become stunned for the specified amount of time.
	Stuns do not stack. If a subsequant stun is applied, the stun with the longer duration is used.
	i.e. A stun of 1 second cannot cancel out a 5 second stun, nor does it becoe a 6 second stun.
	\param duration The length of the stun.
	*/
	void stun(sf::Time const& duration);

	/*!
	\brief Become forced to attack th taunter.
	Changes combat target to be the taunter. Has no associated duration so subsequent taunts will override each other.
	\param taunter The new target for this Pawn.
	*/
	void beTaunted(std::shared_ptr<Pawn> const &taunter);

	/*!
	\brief Offer a potential target to the Pawn.
	Offer a target to the Pawn, who then decides whether to accept it or not.
	\remarks Temporary until ChaiScript (target acquisition)
	\param target A possible combat target.
	\returns True if the target was accepted.
	*/
	bool offerTarget(std::shared_ptr<Pawn> const &target);	//temporary until chairscript (target acquisition) (returns true if target accepted)

	/*!
	Checks if the Pawn's combat target is dead.
	\returns True if target is dead.
	*/
	bool targetIsDead() const;

	/*!
	\brief Disallows the pawn to march for the specified time.
	\param second The number of second to wait.
	*/
	void wait(float seconds);

	/*
	Returns the current health of the pawn.
	*/
	int getHealth() const;

	/*!
	\brief Gets the current state of the Pawn.
	\returns State enum
	*/
	State getState() const;

	/*!
	Gets whether the Pawn is dead or not.
	Does not take into account health, only checks current state.
	\returns True if the Pawn is dead.
	*/
	bool isDead() const;

	/*!
	Gets the Pawn's faction. (NEUTRAL, PLAYER, ENEMY)
	\returns Faction enum
	*/
	Faction getFaction() const;

	void setOnDeath(std::function<void(Pawn*)> const& callback);

	std::shared_ptr<Pawn> const& getCombatTarget() const;

	Damage::Reduction const& getArmour() const;
	Damage::Reduction const& getMagicResist() const;

	sf::Color getBloodColor() const;

	/*!
	\brief Called whenever the Pawn collides with something.
	Virtual method that is invoked whenver this Pawn collides with something.
	Moves the Pawn a portion of the minimum translation vector.
	\param other The object we collided with.
	\param mtv The Minimum Translation Vector for the collision.
	*/
	virtual void onCollide(std::shared_ptr<Collidable> &other, sf::Vector2f const &mtv);
#pragma endregion

protected:
#pragma region Fields
	Faction mFaction; /*!< The faction that the Pawn belongs to */
	State mState;

	int mHealth;
	const int M_MAX_HEALTH;
	Damage::Reduction mArmour;
	Damage::Reduction mMagicResist;
	Damage::Type mDamageType;
	float mAttackRange;

	int mMovementSpeed;

	int mAttackDamage;
	float mAttacksPerSecond; /*!< \remarks 1.0f == 1 attack per second */
	float mTimeSinceAttack;	/*!< \remarks 1.0f == 1 second */
	std::shared_ptr<Pawn> mCombatTarget;

	sf::Time mStunDuration;
	sf::Time mTimeStunned;

	sf::Vector2f mDestination;

	sf::Sound mAttackSound;

	std::weak_ptr<Pawn> self;
#pragma endregion

#pragma region Methods
	void turnToFaceDirection(sf::Vector2f const& dir);

	virtual void calculateAnimation();

	virtual void calculateState(sf::Vector2f const &goalDisplacement);

	virtual void doAttack(float secondsElapsed);

	virtual void doMarch(sf::Vector2f const &goalDisplacement, float secondsElapsed);

	void stopWaiting();

#pragma endregion

private:
	sf::Color mBloodColor;

	float mSecondsToWait;

	//! The time to wait between flipping the sprite.
	const float mTurnCooldown;
	//! How long it has been since the sprite was flipped.
	float mSecondsSinceTurn;

	std::function<void(Pawn*)> mOnDeathFunction;
};
#endif