#include "ConfigManager.h"

#include <iostream>
#include <string>

ConfigManager::ConfigManager(const std::string& file) : _currentSection{ "GLOBAL" }
{
	Load(file);
}

void ConfigManager::Load(const std::string& file)
{
	std::ifstream data{ file };

	if (!data.is_open())
	{
		std::cerr << "ERROR: Couldn't open the file!";
		return;
	}

	std::string content;
	while (std::getline(data, content))
	{
		if (content.empty())
			continue;

		std::string_view parseData{ content };
		if (parseData.front() == '[' && parseData.back() == ']')
		{
			_currentSection = parseData.substr(1, parseData.size() - 2);
			continue;
		}

		std::size_t eq = parseData.find('=');
		if (eq != std::string::npos)
		{
			std::string_view key = parseData.substr(0, eq);
			std::string_view value = parseData.substr(eq + 1);

			std::size_t foundFirst = key.find_first_not_of(" \t\f\v\n\r");
			std::size_t foundLast = key.find_last_not_of(" \t\f\v\n\r");
			if (foundFirst != std::string::npos)
				key = key.substr(foundFirst, foundLast - foundFirst + 1);

			foundFirst = value.find_first_not_of(" \t\f\v\n\r");
			foundLast = value.find_last_not_of(" \t\f\v\n\r");
			if (foundFirst != std::string::npos)
				value = value.substr(foundFirst, foundLast - foundFirst + 1);

			_configData[_currentSection].emplace(key, value);
		}
	}
}

std::string ConfigManager::GetString(const std::string& section, const std::string& key)
{
	if (auto it = _configData.find(section); it != _configData.end())
		if (auto innerIt = it->second.find(key); innerIt != it->second.end())
			return innerIt->second;

	return "";
}