#include <include/Hero.hpp>

Hero::Hero(sf::Texture &texture) : Pawn(texture){
	mMovementSpeed = 100;
}

Hero::~Hero() {
}

void Hero::onCollide(Collidable* other, sf::Vector2f const& mtv) {
	collision::Collidable::onCollide(other, mtv);	//call base method
}