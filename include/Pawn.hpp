#ifndef _PAWN_H
#define _PAWN_H

#include <boost/signals2.hpp>
#include <include/Actor.hpp>
#include <include/Damage.hpp>
#include <Thor/Vectors.hpp>

using namespace boost::signals2;

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
protected:
#pragma region Fields
	Faction mFaction; /*!< The faction that the Pawn belongs to */

	enum State {
		IDLE,
		MARCHING,
		ATTACKING,
		STUNNED,
		DEAD
	};
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
	Pawn* mCombatTarget;

	sf::Time mStunDuration;

	sf::Vector2f mDestination;

	//sf::Time mDecayTime;
	//bool mDecayed;
#pragma endregion

#pragma region Methods

	//bool decay(sf::Time const &elapsedTime);
	void turnToFaceDestination();

#pragma endregion

#pragma region Signals
	//signal<void(Pawn&)> onDecayed;

	//signal<void(Pawn&)> onStateChanged;
#pragma endregion

public:
	Pawn(sf::Texture &texture, Faction faction);
	//Pawn(const char* xml);	//todo: #include tinyXML
	virtual ~Pawn();

	/*!
	Gets the Pawn's destination.
	*/
	sf::Vector2f getDestination() const;

	/*!
	Sets the Pawn's destination. Pawns currently move straight toward their destination.
	\param destination The Pawn's new destination.
	*/
	void setDestination(sf::Vector2f const &destination);
	
	int getMovementSpeed() const;
	void setMovementSpeed(int newSpeed);

	virtual void update(sf::Time const &elapsedTime);

	/*!
	\brief Kills the Pawn outright.
	Sets the Pawn's health to 0 and the state to DEAD.
	*/
	void kill();

	/*!
	Applies damage to the Pawn, taking into account Armour/MagicResist
	\param amount The amount of damage to take.
	\param type The type of damage to take.
	*/
	bool takeDamage(int amount, Damage::Type type);
	bool takeDamage(int amount, Damage::Type type, Pawn* sender);
	void heal(int amount);
	void stun(sf::Time duration);

	void beTaunted(Pawn* taunter);
	bool offerTarget(Pawn* target);	//temporary until chairscript (target acquisition) (returns true if target accepted)
	bool targetIsDead() const;

	State getState() const;
	bool isDead() const;

	Faction getFaction() const;

	virtual void onCollide(Collidable* other, sf::Vector2f const &mtv) override;
};
#endif