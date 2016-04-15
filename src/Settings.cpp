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

void Settings::save() {
	instance()->commit();
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

	auto element = doc.FirstChildElement("Settings")->FirstChildElement();

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
	using namespace tinyxml2;

	std::string const& path = Constants::Strings::getSettingsFilePath();

	//Open the file in write mode.
	FILE* profile;
	errno_t error;

	//Write to a temporary file and replace original at the end. (Don't screw up the old saved settings if saving fails!)
	if ((error = fopen_s(&profile, (path + ".tmp").c_str(), "wt")) != 0) {
		//Something went wrong when opening file.
		printError("Error opening file:", error);
	}
	else {
		//File opened successfully.

		XMLPrinter printer = XMLPrinter(profile);
		//Create the root <Profile> tag
		printer.OpenElement("Settings");


		//Write map contents
		for (auto const& entry : mMap) {
			auto typeIndex = entry.second.which();
			switch (typeIndex) {
				case STRING:
					writeString(printer, entry.first);
					break;

				case BOOL:
					writeBool(printer, entry.first);
					break;

				case INTEGER:
					writeInteger(printer, entry.first);
					break;

				case UNSIGNED:
					writeInteger(printer, entry.first);
					break;

				case FLOAT:
					writeFloat(printer, entry.first);
					break;

				case VECTOR2:
					writeVector2(printer, entry.first);
					break;

				default:
					printf("\aSettings::commit(): Uknown variant type! variant::which() returned %d\n", typeIndex);
					break;
			}
		}

		printer.CloseElement();	//</Settings>

		//Close the file
		fclose(profile);

		//Try to remove the old file...
		error = remove(path.c_str());
		if (error != 0) {
			//...print the error if it fails.
			printError("Error removing old settings file:", error);
		}
		else {
			//...rename the ".tmp" from the new file.
			error = rename((path + ".tmp").c_str(), path.c_str());
			if (error != 0) {
				printError("Error renaming settings file:", error);
			}
		}
	}
}

void Settings::writeString(tinyxml2::XMLPrinter& printer, std::string const& mapKey) const {
	printer.OpenElement(mapKey.c_str());
	printer.PushAttribute("type", "STRING");
	printer.PushAttribute("value", boost::get<std::string>(mMap.at(mapKey)).c_str());
	printer.CloseElement();
}

void Settings::writeBool(tinyxml2::XMLPrinter& printer, std::string const& mapKey) const {
	printer.OpenElement(mapKey.c_str());
	printer.PushAttribute("type", "BOOL");
	printer.PushAttribute("value", boost::get<bool>(mMap.at(mapKey)));
	printer.CloseElement();
}

void Settings::writeInteger(tinyxml2::XMLPrinter& printer, std::string const& mapKey) const{
	printer.OpenElement(mapKey.c_str());
	printer.PushAttribute("type", "INTEGER");
	printer.PushAttribute("value", boost::get<int>(mMap.at(mapKey)));
	printer.CloseElement();
}

void Settings::writeUnsigned(tinyxml2::XMLPrinter& printer, std::string const& mapKey) const {
	printer.OpenElement(mapKey.c_str());
	printer.PushAttribute("type", "UNSIGNED");
	printer.PushAttribute("value", boost::get<unsigned>(mMap.at(mapKey)));
	printer.CloseElement();
}

void Settings::writeFloat(tinyxml2::XMLPrinter& printer, std::string const& mapKey) const {
	printer.OpenElement(mapKey.c_str());
	printer.PushAttribute("type", "FLOAT");
	printer.PushAttribute("value", boost::get<float>(mMap.at(mapKey)));
	printer.CloseElement();
}

void Settings::writeVector2(tinyxml2::XMLPrinter& printer, std::string const& mapKey) const {
	printer.OpenElement(mapKey.c_str());
	printer.PushAttribute("type", "VECTOR2");

	auto const& vec = boost::get<sf::Vector2i>(mMap.at(mapKey));
	printer.PushAttribute("x", vec.x);
	printer.PushAttribute("y", vec.y);
	printer.CloseElement();
}

void Settings::printError(std::string const& preMsg, errno_t err) const {
	auto buffer = new char[100];
	strerror_s(buffer, 100, err);
	printf("%s %s", preMsg.c_str(), buffer);
	delete[] buffer;
}
