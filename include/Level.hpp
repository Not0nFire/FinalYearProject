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
#include <include/TinyXML2/tinyxml2.h>
#include <include/Towers/UnitTower.hpp>
#include <include/WaveController.hpp>
#include <include/ResourceManager.hpp>
#include <include/Abilities/MagicMissileAbility.hpp>
#include <include/Abilities/RaiseDeadAbility.hpp>
#include <include/Abilities/HealAbility.hpp>
#include <include/Gui/AbilityButton.hpp>
#include <include/BloodSystem.hpp>
#include <include/HUD.hpp>
#include <include/Gui/DialogueBox.hpp>
#include "Gui/CostButton.hpp"
#include <include/Constants.h>

using std::shared_ptr;

/*!
\brief Playable level of the game.
*/
class Level : public I_Scene{
private:
	gui::DialogueBox mPauseDialogue;

	//Each button is paired with an ability, which will be executed when the button is clicked
	std::list<std::pair<gui::AbilityButton, shared_ptr<Ability>>> mAbilityList;

	std::map<TowerPlacer::TowerType, std::unique_ptr<gui::CostButton>> mTowerButtons;

	shared_ptr<Pawn> mHero;
	shared_ptr<std::list<shared_ptr<Pawn>>> mPawns;
	shared_ptr<collision::CollisionGroup> mCollisionGroup;

	//! List of towers in the level
	std::vector<shared_ptr<tower::Tower>> mTowers;

	//std::mutex mMutex;

	//! Visual backdrop of level
	sf::Sprite mBackground;

	//! Camera that follows mHero.
	Camera mCamera;

	Hud mHud;
	
	//! Quadtree used to decide where towers can be placed.
	shared_ptr<Quadtree<unsigned char>> terrainTree;

	shared_ptr<ProjectileManager> mProjectileManager;

	//! Tool for placing towers.
	std::unique_ptr<TowerPlacer> mTowerPlacer;

	//! Path that Minions will follow.
	shared_ptr<Path> mPath;

	//! Amount of money at the player's disposal. Earned by killing Minions, spent on building towers.
	shared_ptr<int> mMoney;
	//! Number of enemies that can make it through the level alive before losing.
	shared_ptr<int> mLivesRemaining;
	//! Bounds of the level. Pawns outside this are killed outright and mLivesRemaining is decremented every time it happens.
	sf::FloatRect mBounds;

	//! RenderTexture used for persisting dead bodies and blood without clogging the update.
	sf::RenderTexture mUnderlayTex;

	//! Sprite used to draw mUnderlayTex to the screen.
	sf::Sprite mUnderlaySpr;

	//! Draws an object to the underlay texture.
	void drawToUnderlay(sf::Drawable const& drawable);

	bool mIsLost, mIsWon;

	//! Background music of the level.
	sf::Music mBgMusic;

	//! Level id. CURRENTLY UNUSED.
	const int mId;

	//! Scene to go to if player completes this level.
	const std::string mNextScene;

	//! Flock of enemy minions.
	shared_ptr<std::list<std::weak_ptr<Pawn>>> mFlock;

	//! Controller for spawning groups of units units after delays
	WaveController mWaveController;

	BloodSystem mBloodSystem;

	void onPawnDeath(Pawn* pawn);

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

	void setupTowerButtons();

	
	void spawnMinion(shared_ptr<Minion> const& unit, bool setPath=true, bool addFlock=true, bool addCollision=true) const;

	void processPauseMenuResult();

	bool updatePawns(sf::Time const& elapsedTime);
	//! Calls update and shoot on all towers.
	void updateTowers(sf::Time const& elapsedTime);
	//! Plays the background music if it is not playing.
	void ensureMusicPlaying();
	//! Removes expired weak_ptrs from the list.
	void cleanPawnFlock() const;
	
public:
	Level(XMLElement* root);
	~Level();
	
	bool handleEvent(sf::Event &Event ) override;
	void update(sf::Time const &elapsedTime) override;
	void draw(sf::RenderWindow &w) override;
	void cleanup() override;

	//signal<void()> onWin, onLose;
	bool isWon() const;
	bool isLost() const;

	int getID() const;
	std::string getNextScene() const;

	//bool loadFromXML(const char *path); //returns true if no errors
};
#endif