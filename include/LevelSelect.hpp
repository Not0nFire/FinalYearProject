#ifndef _LEVEL_SELECT_HPP
#define LEVEL_SELECT_HPP

#include "include/Scene.hpp"
#include <SFML/Audio/Music.hpp>

class LevelSelect : public I_Scene {
public:
	LevelSelect(tinyxml2::XMLElement* xml);
	virtual ~LevelSelect();

	bool handleEvent(sf::Event& Event) override;
	void update(sf::Time const& elapsedTime) override;
	void draw(sf::RenderWindow& w) override;
	void cleanup() override;
private:
	//! Graphical map upon which the level buttons will be displayed.
	sf::Sprite mBackground;

	//! Background music for this scene
	sf::Music mMusic;


};
#endif