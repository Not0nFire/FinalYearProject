#ifndef _LEVEL_
#define _LEVEL_

#include <vector>
#include <SFML/Graphics.hpp>
#include <include/Scene.hpp>
#include <include/Pawn.hpp>
#include <include/Hero.hpp>
#include <include/Collision/CollisionGroup.hpp>
#include <include/Towers/BasicTower.h>
#include <include/Quadtree.hpp>
#include <include/TerrainInterpreter.h>
#include <include/Towers/TowerPlacer.hpp>

class Level : public I_Scene{
private:

	Hero* mHero;
	std::vector<Pawn*> mPawns;
	collision::CollisionGroup mCollisionGroup;
	sf::RenderWindow const* relWindow;	/*!< for getting mouse position */

	std::vector<tower::BasicTower*> mTowers;

	boost::mutex mMutex;

	sf::Sprite backgroundTEMP;

	std::shared_ptr<Quadtree<unsigned char>> terrainTree;
	TowerPlacer mTowerPlacer;
	
public:
	/*!
	\param _relWindow RenderWindow to be used for getting relative mouse position.
	*/
	Level(sf::RenderWindow const* _relWindow);
	~Level();
	
	bool I_Scene::handleEvent(sf::Event &Event ) override;
	void I_Scene::update(sf::Time const &elapsedTime) override;
	void I_Scene::draw(sf::RenderWindow &w) override;

	//bool loadFromXML(const char *path); //returns true if no errors
};
#endif