#include <include/Settings.hpp>
#include <include/Constants.h>
#include <include/TinyXML2/tinyxml2.h>
#include <iostream>
#include <SFML/Window/VideoMode.hpp>

std::unique_ptr<Settings> Settings::mInstance = nullptr;

std::string const& Settings::getString(std::string const& name) {
	return boost::get<std::string const&>(instance()->mMap[name]);
}

bool Settings::getBool(std::string const& name) {
	return boost::get<bool>(instance()->mMap[name]);
}

int Settings::getInt(std::string const& name) {
	return boost::get<int>(instance()->mMap[name]);
}

unsigned Settings::getUnsigned(std::string const& name) {
	return boost::get<unsigned>(instance()->mMap[name]);
}

float Settings::getFloat(std::string const& name) {
	return boost::get<float>(instance()->mMap[name]);
}

sf::Vector2i const& Settings::getVector2i(std::string const& name) {
	return boost::get<sf::Vector2i const&>(instance()->mMap[name]);
}

Settings::~Settings() {
}

Settings::Settings() {
	using namespace tinyxml2;

	XMLDocument doc;
	auto result = doc.LoadFile(Constants::Strings::getSettingsFilePath().c_str());
	if (result != XML_NO_ERROR)
	{
		throw result;
	}

	auto element = doc.FirstChildElement("Settings")->FirstChildElement("Resolution");
	//Handle default resolution
	if (nullptr == element) {
		//Default to desktop resolution
		auto mode = sf::VideoMode::getDesktopMode();
		mMap["Resolution"] = sf::Vector2i(mode.width, mode.height);

		element = doc.FirstChildElement("Settings")->FirstChildElement();
	}

	while (nullptr != element)
	{
		parse(element);
		element = element->NextSiblingElement();
	}
}

std::unique_ptr<Settings> const& Settings::instance() {
	if (!mInstance)
	{
		mInstance = std::unique_ptr<Settings>(new Settings());
	}

	return mInstance;
}

void Settings::parse(tinyxml2::XMLElement* xml) {
	const std::string type = xml->Attribute("type");
	const std::string name = xml->Name();

	if (type == "STRING")
	{
		mMap[name] = xml->Attribute("value");
	}
	else if (type == "BOOL")
	{
		bool value;
		xml->QueryBoolAttribute("value", &value);
		mMap[name] = value;
	}
	else if (type == "INTEGER") {
		int value;
		xml->QueryIntAttribute("value", &value);
		mMap[name] = value;
	}
	else if (type == "UNSIGNED")
	{
		unsigned value;
		xml->QueryUnsignedAttribute("value", &value);
		mMap[name] = value;
	}
	else if (type == "FLOAT")
	{
		float value;
		xml->QueryFloatAttribute("value", &value);
	}
	else if (type == "VECTOR2") {
		sf::Vector2i value;
		xml->QueryIntAttribute("x", &value.x);
		xml->QueryIntAttribute("y", &value.y);
		mMap[name] = value;
	}
	else
	{
		std::cout << "\aSETTINGS ERROR:\tUnknown setting type in xml." << std::endl;
	}
}

void Settings::commit() const {
	//Write settings to file
}
