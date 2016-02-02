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
#include <SFML/Audio.hpp>
#include <include/UnitFactory.hpp>
#include <include/TinyXML2/tinyxml2.h>

class Level : public I_Scene{
private:

	Hero* mHero;
	std::vector<Pawn*> mPawns;
	collision::CollisionGroup mCollisionGroup;
	sf::RenderWindow const* relWindow;	/*!< for getting mouse position */

	std::vector<tower::BasicTower*> mTowers;

	boost::mutex mMutex;

	sf::Sprite backgroundTEMP;

	std::unique_ptr<HUD> mHud;
	
	std::shared_ptr<Quadtree<unsigned char>> terrainTree;
	std::unique_ptr<TowerPlacer> mTowerPlacer;

	Path mPath;

	int mLivesRemaining;
	sf::FloatRect mBounds;

	bool mIsLost, mIsWon;

	sf::Music mBgMusic;

	const int mId;

	const std::string mNextScene;

	//! Compares the y position of two actors for the purpose of sorting the draw order
	static bool compareDepth(Actor* A, Actor* B);
	
public:
	/*!
	\param _relWindow RenderWindow to be used for getting relative mouse position.
	*/
	/*Level(sf::RenderWindow const* _relWindow, std::shared_ptr<sfg::SFGUI> sfgui);
	Level(sf::RenderWindow const* _relWindow, std::shared_ptr<sfg::SFGUI> sfgui, const char* xmlPath);*/
	Level(tinyxml2::XMLElement* root, sf::RenderWindow const* _relWindow, std::shared_ptr<sfg::SFGUI> sfgui);
	~Level();
	
	bool I_Scene::handleEvent(sf::Event &Event ) override;
	void I_Scene::update(sf::Time const &elapsedTime) override;
	void I_Scene::draw(sf::RenderWindow &w) override;
	void I_Scene::cleanup() override;

	//signal<void()> onWin, onLose;
	bool isWon() const;
	bool isLost() const;

	int getID() const;
	std::string getNextScene() const;

	//bool loadFromXML(const char *path); //returns true if no errors
};
#endif