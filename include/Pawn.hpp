#ifndef _PAWN_H
#define _PAWN_H

#include <boost/signals2.hpp>
#include <include/Actor.hpp>
#include <include/Damage.hpp>
#include <Thor/Vectors.hpp>

using namespace boost::signals2;

class Pawn : public Actor {
protected:
#pragma region Fields

	enum State {
		IDLE,
		MARCHING,
		ATTACKING,
		STUNNED,
		DYING,
		DEAD
	};
	State mState;

	int mHealth;
	const int M_MAX_HEALTH;
	Damage::Reduction mArmour;
	Damage::Reduction mMagicResist;
	Damage::Type mDamageType;


	int mMovementSpeed;

	int mAttackDamage;
	float mAttackSpeed; //1.0f == 1 attack per second

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
	signal<void(Pawn&)> onStateChanged;
#pragma endregion

public:

	Pawn(sf::Texture &texture);
	//Pawn(const char* xml);	//todo: #include tinyXML
	virtual ~Pawn();

	sf::Vector2f getDestination() const;
	void setDestination(sf::Vector2f const &destination);
	
	int getMovementSpeed() const;
	void setMovementSpeed(int newSpeed);

	virtual void update(sf::Time const &elapsedTime);

	void kill();
	bool takeDamage(int amount, Damage::Type type);
	void heal(int amount);
	void stun(sf::Time duration);

	State getState() const;
	void setState(State newState);

	//bool hasDecayed() const;

	virtual void onCollide(Collidable* other, sf::Vector2f const &mtv);

};
#endif