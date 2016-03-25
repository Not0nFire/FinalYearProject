#ifndef PROFILE_SELECT_SCENE_HPP
#define PROFILE_SELECT_SCENE_HPP

#include <SFML/Graphics.hpp>
#include "Scene.hpp"
#include "Gui/Button.hpp"

/*!
\class ProfileSelect
\brief Scene for selecting which player profile to use.
*/
class ProfileSelect : public I_Scene {
public:
	//! Constructs a ProfileSelect scene from xml.
	ProfileSelect(tinyxml2::XMLElement* xml);
	virtual ~ProfileSelect();

	bool handleEvent(sf::Event& Event) override;
	void update(sf::Time const& elapsedTime) override;
	void draw(sf::RenderWindow& w) override;
	void cleanup() override;

private:
	sf::Sprite mBackground;

	gui::Button mProfileOneBtn, mProfileTwoBtn, mProfileThreeBtn;
};
#endif