#ifndef SETTINGS_HPP
#define SETTINGS_HPP
#include <memory>
#include <string>
#include <map>
#include <boost/variant.hpp>
#include <SFML/System/Vector2.hpp>

namespace tinyxml2 {
	class XMLPrinter;
	class XMLElement;
}

class Settings {
public:
	~Settings();

	template<typename T>
	static T const& get(std::string const& name);

	template<typename T>
	static void set(std::string const& name, T const& value);

	static std::string const& getString(std::string const& name);
	static bool getBool(std::string const& name);
	static int getInt(std::string const& name);
	static unsigned getUnsigned(std::string const& name);
	static float getFloat(std::string const& name);
	static sf::Vector2i const& getVector2i(std::string const& name);

	static void save();

private:
	Settings();

	static std::unique_ptr<Settings> const& instance();
	static std::unique_ptr<Settings> mInstance;

	typedef boost::variant<std::string, bool, int, unsigned, float, sf::Vector2i> settingValue;
	//Used in tandem with boost::variant::which() to get the type the variant holds.
	enum ValueTypes {
		//These must be in same order as the settingValue variant typedef.
		STRING = 0,
		BOOL,
		INTEGER,
		UNSIGNED,
		FLOAT,
		VECTOR2
	};
	
	std::map<std::string, settingValue> mMap;

	void parse(tinyxml2::XMLElement* xml);

	void commit() const;

	void printError(std::string const& preMsg, errno_t err) const;

	void writeString(tinyxml2::XMLPrinter& printer, std::string const& mapKey) const;
	void writeBool(tinyxml2::XMLPrinter& printer, std::string const& mapKey) const;
	void writeInteger(tinyxml2::XMLPrinter& printer, std::string const& mapKey) const;
	void writeUnsigned(tinyxml2::XMLPrinter& printer, std::string const& mapKey) const;
	void writeFloat(tinyxml2::XMLPrinter& printer, std::string const& mapKey)const;
	void writeVector2(tinyxml2::XMLPrinter& printer, std::string const& mapKey)const;
};
#include "Settings.inl"
#endif