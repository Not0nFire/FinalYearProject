#ifndef SETTINGS_HPP
#define SETTINGS_HPP
#include <memory>
#include <string>
#include <map>
#include <boost/variant.hpp>

namespace tinyxml2 {
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

private:
	Settings();

	static std::unique_ptr<Settings> const& instance();
	static std::unique_ptr<Settings> mInstance;

	typedef boost::variant<std::string, bool, int, unsigned, float> settingValue;
	
	std::map<std::string, settingValue> mMap;

	void parse(tinyxml2::XMLElement* xml);

	void commit() const;
};
#include "Settings.inl"
#endif