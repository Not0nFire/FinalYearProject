#include <include/Hero.hpp>

Hero::Hero(sf::Texture &texture) : Pawn(texture){
	mMovementSpeed = 100;
	mMask->setOutlineThickness(1.0f);
}

Hero::~Hero() {
}

void Hero::onCollide(Collidable* other, sf::Vector2f const& mtv) {
	mMask->setOutlineColor(sf::Color(rand() % 255, rand() % 255, rand() % 255));
	collision::Collidable::onCollide(other, mtv);	//call base method
}