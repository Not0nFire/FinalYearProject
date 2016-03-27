#ifndef ABILITY_HPP
#define ABILITY_HPP

#include <string>
#include <SFML/Graphics.hpp>
#include <memory>
#include <list>
#include <include/TinyXML2/tinyxml2.h>
#include <include/ProjectileManager.hpp>

class Pawn;

/*!
\class Ability
\brief Base class for usable abilities or spells.
Abilities must be initialized through xml.
*/
class Ability : public sf::Drawable {
public:
	/*!
	\brief Constructs an Ability from xml.
	Only description is read from xml in this base class,
	derived classes will require more.
	\param xml An <Ability> xml tag, containing a <Description> tag.
	*/
	explicit Ability(tinyxml2::XMLElement* xml);
	virtual ~Ability();

	/*!
	\brief 
	\param user Raw pointer to user of ability. Can be null.
	\remarks The raw Pawn pointer is not intended to be stored.
	*/
	virtual void execute(Pawn* user) = 0;

	/*!
	\brief Checks if ability is active and does update logic if it is.
	\param elapsedTime The amount of time since the last update.
	*/
	void update(sf::Time const& elapsedTime);

	/*!
	\brief Gets a user-friendly description of the ability, as it might appear in a tooltip, etc.
	\returns The ability's decription.
	*/
	const std::string& getDescription() const;

	/*!
	\brief Gets the user-friendly name of the ability.
	\returns The ability's name.
	*/
	const std::string& getName() const;

	/*!
	\brief Sets the pawn list to be used when the Ability wishes to add Pawns to the game.
	\param list Shared pointer to a list of shared pawns.
	*/
	void setPawnList(std::shared_ptr<std::list<std::shared_ptr<Pawn>>> const& list);

	/*!
	\brief Sets the ProjectileManager to be used when the Ability wishes to add projectiles to the game.
	\param manager Shared pointer to a ProjectileManager, the Ability can give projectiles to this.
	*/
	void setProjectileManager(std::shared_ptr<ProjectileManager> const& manager);

protected:
	void activate();
	void deactivate();

	/*!
	\brief Gets whether the ability has been activated or not.
	\returns True if the ability has been actived (execute() has been called).
	*/
	bool isActive() const;
	
	/*!
	\brief Does everything necessary to update the ability.
	Only called if the ability is active.
	*/
	virtual void doUpdateLogic(float deltaSeconds) = 0;

	//! Draws the ability to the render target.
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override = 0;

	/*!
	\brief Adds a Pawn to the game.
	\param pawn The Pawn to push into the list.
	\see Level.hpp
	*/
	void spawnPawn(std::shared_ptr<Pawn> const& pawn) const;

	/*!
	\brief Adds a projectile to the game.
	\param projectile The projectile to give to the ProjectileManager.
	*/
	void spawnProjectile(std::shared_ptr<Projectile> const& projectile) const;
private:
	//! True if the ability has been activated and should be updated.
	bool mIsActive;

	//! User-friendly description of ability.
	const std::string mDescription;

	//! User-friendly name of ability.
	const std::string mName;

	std::shared_ptr<std::list<std::shared_ptr<Pawn>>> mPawnList;
	std::shared_ptr<ProjectileManager> mProjectileManager;
};
#include <include/Pawn.hpp>
#endif