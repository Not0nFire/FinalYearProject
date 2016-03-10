#include <include/Collision/Collidable.hpp>
#include <Thor/Vectors.hpp>
#include <iostream>

namespace collision {

	Collidable::Collidable(sf::Shape* mask, sf::Vector2f offset) :
		mMask(mask),
		mOffset(offset)
	{
		mMask->setFillColor(sf::Color::Magenta);
	}

	Collidable::Collidable(Collidable const& other) :
		mOffset(other.mOffset)
	{
		//dynamic cast other shape to convex
		sf::ConvexShape* shape = dynamic_cast<sf::ConvexShape*>(other.mMask);
		if (shape != nullptr)
		{
			//if it works, copy it as convex shape
			mMask = new sf::ConvexShape(*shape);
		}
		else
		{
			//if it didnt work, it must be a circle shape, copy it as that
			mMask = new sf::CircleShape(*(dynamic_cast<sf::CircleShape*>(other.mMask)));
		}

		mMask->setFillColor(sf::Color::Magenta);
	}

	Collidable::Collidable(tinyxml2::XMLElement* xml) {
		_ASSERT(std::string(xml->Name()) == "Collidable");

		std::string shapeType = xml->Attribute("type");

		//Construct different sf::Shape derivative depending on shapeType as specified in the xml
		if (shapeType == "Circle") {

			//Construct circle shape
			mMask = new sf::CircleShape(
				atof(xml->FirstChildElement("Radius")->GetText()),	//Get radius of shape from xml
				atoi(xml->Attribute("pointCount"))	//Get point count of shape from xml
				);

		} else if (shapeType == "Convex") {

			//Construct convex shape with points as specified in the xml
			sf::ConvexShape* shape = new sf::ConvexShape(atoi(xml->Attribute("pointCount")));

			for (tinyxml2::XMLElement* pointElement = xml->FirstChildElement("Point");
				pointElement != nullptr;
				pointElement = pointElement->NextSiblingElement("Point"))
			{
				//Set the position of each point.
				shape->setPoint(
					atoi(pointElement->Attribute("index")),
						sf::Vector2f(
						atof(pointElement->Attribute("x")), 
						atof(pointElement->Attribute("y"))
					)
				);//end shape->setPoint(..)
			}//end for(pointElement)

			mMask = shape;			
		} else {
			throw "Invalid shapeType";
		}

		//Get the offset of the shape (if one has been specified)
		tinyxml2::XMLElement* offsetElement = xml->FirstChildElement("Offset");
		if (offsetElement != nullptr) {
			mOffset = sf::Vector2f(atof(offsetElement->Attribute("x")), atof(offsetElement->Attribute("y")));
		}

		sf::FloatRect bounds = mMask->getLocalBounds();
		mMask->setOrigin(bounds.width * 0.5f, bounds.height * 0.5f);
	}

	Collidable::~Collidable() {
		delete mMask;
	}

	void Collidable::updateCollidableMask(sf::Vector2f newPosition) {
		mMask->setPosition(newPosition + mOffset);
	}

	const sf::Shape* Collidable::getMask() const {
		return mMask;
	}

	void Collidable::setMask(sf::Shape* mask, sf::Vector2f offset) {
		delete mMask;
		mMask = mask;
		mOffset = offset;
	}

	void Collidable::onCollide(Collidable* other, sf::Vector2f const &mtv) {
		mMask->setFillColor(sf::Color::Red);
	}

	void Collidable::setDebugColour(sf::Color const &c) {
		mMask->setFillColor(c);
	}

	void Collidable::debug_draw(sf::RenderTarget& target) {
		target.draw(*mMask);
	}

	void Collidable::getAxies(std::vector<sf::Vector2f>& axiesVector) {
		int pointCount = mMask->getPointCount();
		for (int i = 0; i < pointCount; ++i)
		{
			sf::Vector2f p1 = mMask->getPoint(i);
			sf::Vector2f p2 = mMask->getPoint(i + 1 == pointCount ? 0 : i + 1);
			sf::Vector2f edge = p1 - p2;
			//sf::Vector2f normal = sf::Vector2f(-edge.y, edge.x);
			axiesVector.push_back(sf::Vector2f(-edge.y, edge.x));
		}
	}

	size_t Collidable::getPointCount() const {
		return mMask->getPointCount();
	}

	sf::Vector2f Collidable::projectOntoAxis(sf::Vector2f & axis) const {
		if (thor::length(axis) != 1)
		{
			//normalize the axis
			axis = thor::unitVector(axis);
		}

		float min = thor::dotProduct(axis, mMask->getPosition() + mMask->getPoint(0));
		float max = min;
		float p;

		size_t pointCount = mMask->getPointCount();
		for (size_t i = 1; i < pointCount; ++i)
		{
			p = thor::dotProduct(axis, mMask->getPosition() + mMask->getPoint(i));

			if (p < min)
			{
				min = p;
			}
			else if (p > max)
			{
				max = p;
			}
		}

		return sf::Vector2f(min, max);
	}
}//namespace