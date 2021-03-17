#ifndef _DATASTORAGE_
#define _DATASTORAGE_

#include "predef.h"

class DataStorage
{
public:
	static DataStorage* instance;
	static DataStorage* GetInstance()
	{
		if (instance == nullptr)
		{
			instance = new DataStorage();
		}
		return instance;
	}

	void Reset();

	bool			GetBoolValue(std::string key, bool d = false);
	int				GetIntValue(std::string key, int d = 0);
	float			GetFloatValue(std::string key, float d = 0.f);
	std::string		GetValue(std::string key);

	void			SetValue(std::string key, bool value);
	void			SetValue(std::string key, int value);
	void			SetValue(std::string key, float vlaue);
	void			SetValue(std::string key, std::string value);

private:
	DataStorage();
	~DataStorage();

	std::map<std::string, std::string> keyValue;
};

#endif