#include <include/Hero.hpp>

Hero::Hero(sf::Texture &texture) : Pawn(texture, Faction::PLAYER){
	mMovementSpeed = 100;
	mHealth = 9000;
	mAttackDamage = 1;
	mAttacksPerSecond = 2.0f;
}

Hero::~Hero() {
}

void Hero::onCollide(Collidable* other, sf::Vector2f const& mtv) {
	collision::Collidable::onCollide(other, mtv);	//call base method
}