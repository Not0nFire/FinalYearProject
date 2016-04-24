template<typename T>
T const& Settings::get(std::string const& name) {
	return boost::get<T>(instance()->mMap[name]);
}

template<typename T>
void Settings::set(std::string const& name, T const& value) {
	instance()->mMap[name] = value;
}
