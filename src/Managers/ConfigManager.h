#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include <fstream>
#include <map>
#include <algorithm>
#include <string>
#include <iostream>

class ConfigManager
{
private:
	std::string _currentSection;
	std::map<std::string, std::map<std::string, std::string>> _configData;

	void Load(const std::string& file);
public:
	ConfigManager(const std::string& file);

	template <typename T>
	T GetValue(const std::string& section, const std::string& key);

	std::string GetString(const std::string& section, const std::string& key);
};

template <typename T>
T ConfigManager::GetValue(const std::string& section, const std::string& key)
{
	std::string value = GetString(section, key);
	if (value.empty())
	{
		std::cout << "ERROR: Incorrect value: " + value + " section-key: " + section + "-" + key + " in Settings.ini\n";
		return T{};
	}

	T output{};
	if constexpr (std::is_same_v<T, int>)
		output = std::stoi(value);
	else if constexpr (std::is_same_v<T, float>)
		output = std::stof(value);
	else if constexpr (std::is_same_v<T, bool>)
	{
		std::transform(value.begin(), value.end(), value.begin(), [](unsigned char c) { return std::tolower(c); });
		if (value == "false")
			output = false;
		else if (value == "true")
			output = true;
		else
		{
			std::cout << "ERROR: Wrong bool value\n";
			return T{};
		}
	}

	return output;
}

#endif