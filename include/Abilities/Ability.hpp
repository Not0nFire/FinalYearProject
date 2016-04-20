#ifndef ABILITY_HPP
#define ABILITY_HPP

#include <string>
#include <SFML/Graphics.hpp>
#include <memory>
#include <list>
#include <include/TinyXML2/tinyxml2.h>
#include <include/ProjectileManager.hpp>
#include <include/Gui/Button.hpp>

class Pawn;
class Minion;

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
	\brief Calls doExecuteLogic() if ability has fully cooled since last use.
	\param user Raw pointer to user of ability. Can be null.
	\remarks The raw Pawn pointer is not intended to be stored.
	\see doUpdateLogic()
	\see M_COOLDOWN
	*/
	virtual void execute(Pawn* user);

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

	bool canCast() const;

	//! Gets the remaining time, in seconds, before the ability can be used again.
	float getRemainingCooldown() const;

	//! The time, in seconds, that the ability's cooldown lasts.
	float getTotalCooldown() const;

	sf::Time const& getCastDuration() const;

	/*!
	\brief Gets whether the ability has been activated or not.
	\returns True if the ability has been actived (execute() has been called).
	*/
	bool isActive() const;

	/*!
	\brief Sets the pawn list to be used when the Ability wishes to find pawns in the game.
	The pawn list cannot be used for directly adding Pawns to the game.
	\param list Shared pointer to a list of shared pawns.
	*/
	void setPawnList(std::shared_ptr<const std::list<std::shared_ptr<Pawn>>> const& list);

	/*!
	\brief Sets the ProjectileManager to be used when the Ability wishes to add projectiles to the game.
	\param manager Shared pointer to a ProjectileManager, the Ability can give projectiles to this.
	*/
	void setProjectileManager(std::shared_ptr<ProjectileManager> const& manager);

	/*!
	\brief Sets the callback used for adding units to the game.
	Adding a unit to the pawn list won't add flocking or collision, so we've a callback that will.
	\param callback The function called when an ability want to spawn a unit.
	*/
	void setSpawnCallback(std::function<void(std::shared_ptr<Minion>)> const& callback);

	bool checkHotkey(char key) const;
	char getHotkey() const;
	void setHotkey(char hotkey);

protected:
	void activate();
	void deactivate();

	/*!
	\brief
	\param user Pointer to the pawn that used the ability.
	*/
	virtual void doExecuteLogic(Pawn* user) = 0;
	
	/*!
	\brief Does everything necessary to update the ability.
	Only called if the ability is active.
	*/
	virtual void doUpdateLogic(sf::Time const& deltaTime) = 0;

	//! Calls doDraw() if ability is active
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override final;

	virtual void doDraw(sf::RenderTarget& target, sf::RenderStates states) const = 0;

	/*!
	\brief Adds a Pawn to the game.
	\param pawn The Pawn to push into the list.
	\see Level.hpp
	*/
	void spawnMinion(std::shared_ptr<Minion> const& pawn) const;

	/*!
	\brief Adds a projectile to the game.
	\param projectile The projectile to give to the ProjectileManager.
	*/
	void spawnProjectile(std::shared_ptr<Projectile> const& projectile) const;
private:
	//! True if the ability has been activated and should be updated.
	bool mIsActive;

	//! User-friendly description of ability.
	const std::string M_DESCRIPTION;

	//! User-friendly name of ability.
	const std::string M_NAME;

	//! Time it takes to use the ability.
	const sf::Time M_CAST_TIME;

	//! How many seconds must pass before the ability can be used subsequently.
	const float M_COOLDOWN;

	//! The number of seconds since the ability was last deactivated.
	float mSecondsSinceCast;

	//! The sound played when the ability is executed.
	sf::Sound mExecutionSound;

	//! Keyboard shortcut to activate this ability
	char mHotkey;

	std::shared_ptr<const std::list<std::shared_ptr<Pawn>>> mPawnList;
	std::shared_ptr<ProjectileManager> mProjectileManager;
	std::function<void(std::shared_ptr<Minion>)> mSpawnUnitCallback;
};
#endif