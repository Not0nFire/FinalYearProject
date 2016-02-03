#ifndef _HUD_H
#define _HUD_H

#include <SFML/Graphics.hpp>

#include <include/Pawn.hpp>

#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>

namespace HUD_Detail
{
	class HealthBarStatic {
	protected:
		Pawn* mPawnToTrack;
		sfg::ProgressBar::Ptr mBar;
		const float M_MAX_HEALTH;

	public:
		HealthBarStatic(Pawn* pwn, sf::Vector2f const &location, sf::Vector2f const &size, sfg::Desktop &desktop);

		virtual void update();

		void hide();
		void show();
	};

	class HealthBarFollowing : public HealthBarStatic{
	protected:
		sf::Vector2f mOffset;
	public:
		HealthBarFollowing(Pawn* pwn, sf::Vector2f const &offset, sf::Vector2f const &size, sfg::Desktop &desktop);

		virtual void update() override;
	};

	class LabelImagePair {
	protected:
		sf::Sprite mImage;
		sf::Text mText;
		std::shared_ptr<int> mValueToTrack;
	public:
		LabelImagePair(std::shared_ptr<int> valueToTrack, sf::Texture const & tex, sf::Font const &fnt, sf::Vector2f const &position, sf::Vector2f offset, bool imageFirst = true);
		virtual void update();
		virtual void draw(sf::RenderWindow &w);
	};
}

class HUD {
public:
	HUD(std::shared_ptr<sfg::SFGUI> sfgui);	//Construct from xml when tinyXML implemented [hardcoded for now]
	~HUD();

	//! Updates the HUD. Elapsed time is required to update sfg::Desktop.
	void update(sf::Time const &elapsedTime);

	//! Draws all HUD elements to the specified window.
	void draw(sf::RenderWindow &window) const;

	//! Adds a health bar that follows a pawn.
	void addHealthBar(Pawn* pawn, sf::Vector2f const &offset, sf::Vector2f const &size = sf::Vector2f(50.f, 5.f));

	//! Adds a bar that tracks a pawns health.
	void addHealthBarStatic(Pawn* pawn, sf::Vector2f const &location, sf::Vector2f const &size);

	/// Adds an image at the specified window location.
	/// \param filePath The location of the image in the filesystem.
	/// \param location Position of the image on the screen.
	void addImage(std::string const &filePath, sf::Vector2u location);

	/// Adds a repeating texture at the specified window location with width and height.
	/// \param filePath The location of the image in the filesystem.
	/// \param location The position of the image on the screen.
	/// \param widthAndHeight The width and height of the image to be displayed on the screen.
	void addImageArray(std::string const &filePath, sf::Vector2u location, sf::Vector2u widthAndHeight);

	//! Adds an image with label that tracks a value (e.g. coin image and amount of money)
	template<typename ValueType>
	void addImageWithLabel(sf::Texture const & tex, sf::Font const &fnt, sf::Vector2f const &position, sf::Vector2f const &offset, std::shared_ptr<ValueType> labelValue);

	void show();
	void hide();
	bool isShown() const;

private:
	//! HUD cannot be copy constructed
	HUD(HUD const &other) = delete;

	std::shared_ptr<sfg::SFGUI> mSFGUI;
	std::list<sfg::Widget::Ptr> mWidgets;

	std::list<HUD_Detail::HealthBarStatic*> mHealthBars;
	std::list<HUD_Detail::LabelImagePair*> mLabelImagePairs;

	sfg::Desktop mDesktop;

	bool mIsShown;
};

template <typename ValueType>
void HUD::addImageWithLabel(sf::Texture const & tex, sf::Font const &fnt, sf::Vector2f const &position, sf::Vector2f const &offset, std::shared_ptr<ValueType> labelValue) {
	mLabelImagePairs.push_back(new HUD_Detail::LabelImagePair(labelValue, tex, fnt, position, offset));
}
#endif
