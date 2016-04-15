#ifndef CREDITS_SCENE_HPP
#define CREDITS_SCENE_HPP

#include "Scene.hpp"
#include <SFML/Audio/Music.hpp>
#include "Gui/Button.hpp"

class Credits : public I_Scene {
public:
	Credits(tinyxml2::XMLElement* root);
	~Credits();

	bool handleEvent(sf::Event& evnt) override;
	void update(sf::Time const& elapsedTime) override;
	void draw(sf::RenderWindow& w) override;
	void cleanup() override;

private:
	gui::Button mBackButton;
	sf::Music mBgMusic;
	sf::RenderTexture mRenderTex;
	sf::Sprite mCredits, mBackdrop;
	sf::View mView;
	const float mBaseScrollSpeed, mSpeedMultiplier;
	bool mSpeedUp;
};
#endif