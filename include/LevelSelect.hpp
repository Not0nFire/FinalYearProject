#ifndef LEVEL_SELECT_HPP
#define LEVEL_SELECT_HPP

#include "include/Scene.hpp"
#include "include/Actor.hpp"

class LevelSelect : public I_Scene {
public:
	LevelSelect(tinyxml2::XMLElement* xml);
	virtual ~LevelSelect();
private:
	sf::Sprite mBackground;

};
#endif