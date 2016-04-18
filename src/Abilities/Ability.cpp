#include <include/Abilities/Ability.hpp>

Ability::Ability(tinyxml2::XMLElement* xml) :
mIsActive(false),
M_DESCRIPTION(xml->FirstChildElement("Description")->GetText()),
M_NAME(xml->FirstChildElement("Name")->GetText()),
M_CAST_TIME(sf::seconds(atof(xml->FirstChildElement("CastTime")->GetText()))),
M_COOLDOWN(xml->FirstChildElement("Cooldown")->FloatAttribute("seconds")),
mSecondsSinceCast(M_COOLDOWN),
mHotkey(xml->Attribute("hotkey")[0])
{
	_ASSERT(xml->Name() == std::string("Ability"));

	std::string soundPath = xml->FirstChildElement("Sound")->Attribute("path");
	auto &buffer = ResourceManager<sf::SoundBuffer>::instance()->get(soundPath);
	mExecutionSound.setBuffer(buffer);
}

Ability::~Ability() {
}

void Ability::execute(Pawn* user) {
	if (canCast()) {
		user->stun(M_CAST_TIME);	//Stun the user for the duration of the cast time.
		doExecuteLogic(user);	//Do ability-specific execution logic.
		activate();
		mExecutionSound.setPosition(sf::Vector3f(user->getPosition().x, user->getPosition().y, 0.f));
		mExecutionSound.play();
	}
}

void Ability::update(sf::Time const& elapsedTime) {
	if (isActive())
	{
		doUpdateLogic(elapsedTime);
	}
	else {
		mSecondsSinceCast += elapsedTime.asSeconds();
	}
}

const std::string& Ability::getDescription() const{
	return M_DESCRIPTION;
}

const std::string& Ability::getName() const {
	return M_NAME;
}

bool Ability::canCast() const {
	return !mIsActive && mSecondsSinceCast >= M_COOLDOWN;
}

float Ability::getRemainingCooldown() const {
	return M_COOLDOWN - mSecondsSinceCast;
}

float Ability::getTotalCooldown() const {
	return M_COOLDOWN;
}

void Ability::setPawnList(std::shared_ptr<const std::list<std::shared_ptr<Pawn>>> const& list) {
	mPawnList = list;
}

void Ability::setSpawnCallback(std::function<void(std::shared_ptr<Minion>)> const& callback) {
	mSpawnUnitCallback = callback;
}

bool Ability::checkHotkey(char key) const {
	return mHotkey == key;
}

char Ability::getHotkey() const {
	return mHotkey;
}

void Ability::setHotkey(char hotkey) {
	mHotkey = hotkey;
}

void Ability::setProjectileManager(std::shared_ptr<ProjectileManager> const& manager) {
	mProjectileManager = manager;
}

void Ability::activate() {
	mIsActive = true;
}

void Ability::deactivate() {
	mIsActive = false;
	mSecondsSinceCast = 0.f;	//Cooldown starts when the ability finishes casting
}

sf::Time const& Ability::getCastDuration() const {
	return M_CAST_TIME;
}

bool Ability::isActive() const {
	return mIsActive;
}

void Ability::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	if (mIsActive) {
		doDraw(target, states);
	}
}

void Ability::spawnMinion(std::shared_ptr<Minion> const& unit) const {
	_ASSERT(mSpawnUnitCallback);

	mSpawnUnitCallback(unit);
}

void Ability::spawnProjectile(std::shared_ptr<Projectile> const& projectile) const {
	mProjectileManager->give(projectile);
}
