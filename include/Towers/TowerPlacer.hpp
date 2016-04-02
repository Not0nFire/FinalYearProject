#ifndef _TOWER_PLACER_H
#define _TOWER_PLACER_H

#include <memory>

#include <SFML/Graphics.hpp>

#include <include/ResourceManager.hpp>
#include <include/Quadtree.hpp>
#include <include/TerrainInterpreter.h>

#include "BasicTower.h"
#include "MageTower.hpp"
#include "UnitTower.hpp"

#include <include/ProjectileManager.hpp>

#include <include/Collision/CollisionGroup.hpp>

typedef Quadtree<unsigned char> TerrainTree;

using std::shared_ptr;

class TowerPlacer {
public:
	enum TowerType {
		ARROW,
		MAGIC,
		UNIT
	};

	/*!
	\brief	Constructs an instance of TowerPlacer.
	\param	terrainTree	Quadtree used to determine where is valid for tower placement.
	\param	projectileMgr	Shared pointer to a ProjectileManager. Created towers will add their projectiles to this.
	\param	path Path onto which UnitTowers will send their spawned units.
	\param	flock Flock into which UnitTowers will add their spawned units.
	*/
	TowerPlacer(shared_ptr<TerrainTree> const &terrainTree, shared_ptr<ProjectileManager> const &projectileMgr, shared_ptr<Path> const &path, shared_ptr<std::list<Minion*>> &flock);
	virtual ~TowerPlacer();

	/*!
	\brief	If location is valid for tower placement,
			puts a tower into the container.
	*/
	shared_ptr<tower::Tower> place();

	/*!
	\brief	Updates position to match mouse and calculates validity.
	*/
	virtual void update(sf::Vector2i mousePosition);

	/*!
	\brief	Sets the TowerPlacer to be active.
	Must be called before update, draw or place do anything.
	\param type The type of tower to be placed.
	*/
	void activate(TowerType type);

	/*!
	\brief	Draws the tower overlay to the render target.
	\remarks If debugging, also draws the collision mask.
	*/
	void draw(sf::RenderTarget &renderTarget) const;

	bool isActive() const;

protected:
	//! The type of tower that will be placed
	TowerType mTowerType;

	//! If this is false, calls to methods other than activate() will do nothing.
	bool mIsActive;

	//! True if the current location is valid for tower placement.
	bool mIsValid;

	//! The Quadtree<unsigned char> that will be used to check points against terrain for validity.
	shared_ptr<TerrainTree> mTerrainTree;

	//! A ghost tower that show where the tower would be placed and colored to show validity.
	sf::Sprite mOverlay;

	//! Shape used to test for validity with the terrain tree.
	sf::Shape* mMask;

	/*!
	\brief Checks if the current location is valid for placement.
	*/
	virtual void checkValidity();

	/*!
	\brief Sets the color of the overlay to red or green depending on validity.
	*/
	void calculateColor();

private:
	static const sf::Color mValidColor;
	static const sf::Color mInvalidColor;

	static const std::string mArrowTowerDefPath;
	static const std::string mMagicTowerDefPath;
	static const std::string mUnitTowerDefPath;

	shared_ptr<ProjectileManager> mProjectileManager;
	shared_ptr<Path> mPath;
	shared_ptr<std::list<Minion*>> mFlock;

	//! Used to prevent the player from placing towers on top of each other.
	collision::CollisionGroup mTowerCollisionGroup;
};
#endif