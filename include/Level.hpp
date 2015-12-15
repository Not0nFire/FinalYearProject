#ifndef _LEVEL_
#define _LEVEL_

#include <vector>
#include <SFML/Graphics.hpp>
#include <include/Scene.hpp>
#include <include/Minion.hpp>
#include <include/Hero.hpp>
#include <include/Collision/CollisionGroup.hpp>
#include <include/Towers/BasicTower.h>
#include <include/HUD.hpp>
#include <include/Quadtree.hpp>
#include <include/TerrainInterpreter.h>
#include <include/Towers/TowerPlacer.hpp>
#include <include/Pathing/Path.hpp>

class Level : public I_Scene{
private:

	Hero* mHero;
	std::vector<Pawn*> mPawns;
	collision::CollisionGroup mCollisionGroup;
	sf::RenderWindow const* relWindow;	/*!< for getting mouse position */

	std::vector<tower::BasicTower*> mTowers;

	boost::mutex mMutex;

	sf::Sprite backgroundTEMP;

	HUD mHud;
	
	std::shared_ptr<Quadtree<unsigned char>> terrainTree;
	TowerPlacer mTowerPlacer;

	Path mPath;
	
public:
	/*!
	\param _relWindow RenderWindow to be used for getting relative mouse position.
	*/
	Level(sf::RenderWindow const* _relWindow, std::shared_ptr<sfg::SFGUI> sfgui);
	~Level();
	
	bool I_Scene::handleEvent(sf::Event &Event ) override;
	void I_Scene::update(sf::Time const &elapsedTime) override;
	void I_Scene::draw(sf::RenderWindow &w) override;
	void I_Scene::cleanup() override;

	//bool loadFromXML(const char *path); //returns true if no errors
};
#endif