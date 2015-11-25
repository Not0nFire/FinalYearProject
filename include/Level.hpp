#ifndef _LEVEL_
#define _LEVEL_

#include <vector>
#include <SFML/Graphics.hpp>
#include <include/Scene.hpp>
#include <include/Pawn.hpp>
#include <include/Hero.hpp>
#include <include/Collision/CollisionGroup.hpp>
#include <include/Towers/BasicTower.h>

class Level : public I_Scene{
private:

	Hero* mHero;
	std::vector<Pawn*> mPawns;
	collision::CollisionGroup mCollisionGroup;
	sf::RenderWindow const* relWindow;	//for getting mouse position

	tower::BasicTower mTower;

	boost::mutex mMutex;

	sf::Sprite backgroundTEMP;
	
public:
	Level(sf::RenderWindow const* _relWindow);
	~Level();
	
	bool I_Scene::handleEvent(sf::Event &Event ) override;
	void I_Scene::update(sf::Time const &elapsedTime) override;
	void I_Scene::draw(sf::RenderWindow &w) override;

	//bool loadFromXML(const char *path); //returns true if no errors

	Hero*  getHero() const;
};
#endif