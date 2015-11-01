#ifndef _LEVEL_
#define _LEVEL_

#include <vector>
#include <SFML/Graphics.hpp>
#include <include/Scene.hpp>
#include <include/Pawn.hpp>
#include <include/Hero.hpp>
#include <include/Collision/CollisionGroup.hpp>

class Level : public I_Scene{
private:

	Hero* mHero;
	std::vector<Pawn*> mPawns;
	collision::CollisionGroup mCollisionGroup;

	boost::mutex mMutex;
	
public:
	Level();
	~Level();
	
	bool I_Scene::handleEvent(sf::Event &Event ) override;
	void I_Scene::update(sf::Time const &elapsedTime) override;
	void I_Scene::draw(sf::RenderWindow &w) override;

	//bool loadFromXML(const char *path); //returns true if no errors

	Hero*  getHero() const;
};
#endif