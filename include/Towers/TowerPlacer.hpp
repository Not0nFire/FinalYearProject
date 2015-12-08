#ifndef _TOWER_PLACER_H
#define _TOWER_PLACER_H

#include <memory>

#include <SFML/Graphics.hpp>

#include <include/ResourceManager.hpp>
#include <include/Quadtree.hpp>
#include <include/TerrainInterpreter.h>

#include "BasicTower.h"

typedef Quadtree<unsigned char> TerrainTree;

class TowerPlacer {
public:
	TowerPlacer(std::shared_ptr<TerrainTree> terrainTree, std::vector<tower::BasicTower*> *towerContainer);
	~TowerPlacer();

	/*!
	If location is valid for tower placement,
	emplaces a tower into the container.
	*/
	bool place();

	/*!
	Updates position to match mouse and calculates validity.
	*/
	virtual void update(sf::Vector2i mousePosition);

	/*!
	Sets the TowerPlacer to be active.
	Must be called before update, draw or place do anything.
	*/
	void activate();

	/*!
	Draws the tower overlay to the render target.
	\remarks If debugging, also draws the collision mask.
	*/
	void draw(sf::RenderTarget &renderTarget);

protected:

	//! If this is false, calls to methods other than activate() will do nothing.
	bool mIsActive;

	//! True if the current location is valid for tower placement.
	bool mIsValid;

	//! The Quadtree<unsigned char> that will be used to check points against terrain for validity.
	std::shared_ptr<TerrainTree> mTerrainTree;

	//! A ghost tower that show where the tower would be placed and colored to show validity.
	sf::Sprite mOverlay;

	//! Shape used to test for validity with the terrain tree.
	sf::Shape* mMask;

	//! The (ptr to) container of tower pointers, into which we will emplace new towers.
	std::vector<tower::BasicTower*> *mTowerContainer;

	/*!
	Checks if the current location is valid for placement.
	*/
	virtual void checkValidity();

	/*!
	Sets the color of the overlay to red or green depending on validity.
	*/
	void calculateColor();

private:
	static const sf::Color mValidColor;
	static const sf::Color mInvalidColor;
};
#endif