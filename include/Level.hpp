#ifndef LEVEL_HPP
#define LEVEL_HPP

#include <vector>
#include <list>
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
#include <include/Camera.hpp>
#include <SFML/Audio.hpp>
#include <include/UnitFactory.hpp>
#include <include/TinyXML2/tinyxml2.h>
#include <include/Towers/UnitTower.hpp>
#include <include/ResourceManager.hpp>
#include <include/Abilities/MagicMissileAbility.hpp>
#include <include/Abilities/RaiseDeadAbility.hpp>
#include <include/Abilities/HealAbility.hpp>
#include <include/Gui/Button.hpp>

using std::shared_ptr;

/*!
\brief Playable level of the game.
*/
class Level : public I_Scene{
private:
	//Each button is paired with an ability, which will be executed when the button is clicked
	std::list<std::pair<gui::Button, std::unique_ptr<Ability>>> mAbilityList;

	shared_ptr<Pawn> mHero;
	shared_ptr<std::list<shared_ptr<Pawn>>> mPawns;
	shared_ptr<collision::CollisionGroup> mCollisionGroup;

	//! List of ranged towers in the level
	std::vector<shared_ptr<tower::Tower>> mTowers;

	std::mutex mMutex;

	//! Visual backdrop of level
	sf::Sprite mBackground;

	//! Camera that follows mHero.
	Camera mCamera;

	//std::unique_ptr<HUD> mHud;
	
	//! Quadtree used to decide where towers can be placed.
	shared_ptr<Quadtree<unsigned char>> terrainTree;

	shared_ptr<ProjectileManager> mProjectileManager;

	//! Tool for placing ranged towers.
	std::unique_ptr<TowerPlacer> mTowerPlacer;

	//! Path that Minions will follow.
	shared_ptr<Path> mPath;

	//! Amount of money at the player's disposal. Earned by killing Minions, spent onbuilding towers.
	shared_ptr<int> mMoney;
	//! Number of enemies that can make it through the level alive before losing.
	shared_ptr<int> mLivesRemaining;
	//! Bounds of the level. Pawns outside this are killed outright and mLivesRemaining is decremented every time it happens.
	sf::FloatRect mBounds;

	//! RenderTexture used for persisting dead bodies without clogging the update.
	sf::RenderTexture mUnderlayTex;
	//! Sprite used to draw mUnderlayTex to the screen.
	sf::Sprite mUnderlaySpr;

	bool mIsLost, mIsWon;

	//! Background music of the level.
	sf::Music mBgMusic;

	//! Level id. CURRENTLY UNUSED.
	const int mId;

	//! Scene to go to if player completes this level.
	const std::string mNextScene;

	//! Flock of enemy minions.
	shared_ptr<std::list<Minion*>> mMinionFlock;

	//! Compares the y position of two actors for the purpose of sorting the draw order
	static bool compareDepth(shared_ptr<Actor> const &A, shared_ptr<Actor> const &B);

	/*!
	\brief Used to handle unfired projectile that are given to the projectile manager.
	Fires the projectile at the nearest enemy pawn.
	\param projectile The projectile to fire. Must be unfired.
	*/
	void autofireProjectile(shared_ptr<Projectile> const& projectile) const;

	/*!
	\brief Initializes abilities and their buttons.
	*/
	void setupAbilities();
	
public:
	/*!
	\param _relWindow RenderWindow to be used for getting relative mouse position.
	*/
	/*Level(sf::RenderWindow const* _relWindow, std::shared_ptr<sfg::SFGUI> sfgui);
	Level(sf::RenderWindow const* _relWindow, std::shared_ptr<sfg::SFGUI> sfgui, const char* xmlPath);*/
	Level(tinyxml2::XMLElement* root);
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