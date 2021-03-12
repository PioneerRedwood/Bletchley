#include "DataStorage.h"

DataStorage* DataStorage::instance = nullptr;

DataStorage::DataStorage()
{

}

DataStorage::~DataStorage()
{

}

void		DataStorage::Reset()
{
	keyValue.clear();
}

bool		DataStorage::GetBoolValue(std::string key, bool d)
{
	std::string ret = GetValue(key);
	if (ret.empty())
	{
		return d;
	}

	return ret == "true" ? true : false;
}

int			DataStorage::GetIntValue(std::string key, int d)
{
	std::string ret = GetValue(key);
	if (ret.empty())
	{
		return d;
	}

	return atoi(ret.c_str());
}

float		DataStorage::GetFloatValue(std::string key, float d)
{
	std::string ret = GetValue(key);
	if (ret.empty())
	{
		return d;
	}

	return (float)atof(ret.c_str());
}

void		DataStorage::SetValue(std::string key, bool value)
{
	if (value)
	{
		SetValue(key, std::string("true"));
	}
	else
	{
		SetValue(key, std::string("false"));
	}

	return;
}

void		DataStorage::SetValue(std::string key, int value)
{
	char valueStr[256];
	snprintf(valueStr, sizeof(valueStr), "%d", value);

	SetValue(key, valueStr);
}

void		DataStorage::SetValue(std::string key, float value)
{
	char valueStr[256];
	snprintf(valueStr, sizeof(valueStr), "%f", value);

	SetValue(key, valueStr);
}

std::string	DataStorage::GetValue(std::string key)
{
	std::map<std::string, std::string>::iterator iter = keyValue.find(key);
	if (iter != keyValue.end())
	{
		return iter->second;
	}

	return std::string("");
}

void		DataStorage::SetValue(std::string key, std::string value)
{
	std::map<std::string, std::string>::iterator iter = keyValue.find(key);
	if (iter == keyValue.end())
	{
		keyValue.insert(std::make_pair(key, value));
		return;
	}

	iter->second = value;
}