#include "Stream.h"

Stream::Stream()
{
	buffer = new char[2048];
	position = 0;
}

int			Stream::ReadBytes(char* val, int count)
{
	if (position < count)
	{
		return 0;
	}
	else if (position == count)
	{
		errno_t err;
		err = memcpy_s(val, count, buffer, count);
		
		if (err == 0)
		{
			return count;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		int readBytes = 0;

		for (int i = 0; i < count; ++i)
		{
			val[i] = buffer[position--];
			readBytes++;
		}

		return readBytes;
	}
}

void		Stream::WriteBytes(const char* val, int count)
{
	for (int i = 0; i < count; ++i)
	{
		buffer[position++] = val[i];
	}
}