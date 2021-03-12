#include "predef.h"
#include "Stream.h"

#ifndef _STREAM_WRAPPER_
#define _STREAM_WRAPPER_

// StreamWrapper
// 
// 

class StreamWrapper
{
public:
	Stream* GetReadStream() { return readStream; }
	Stream* GetWriteStream() { return writeStream; }

	int ReadPosition() { return readStream->GetPosition(); }
	int WritePosition() { return writeStream->GetPosition(); }

	StreamWrapper()
	{
		readStream = new Stream();
		writeStream = new Stream();
	}

	~StreamWrapper()
	{
		if (readStream != nullptr)
		{
			delete readStream;
		}

		if (writeStream != nullptr)
		{
			delete writeStream;
		}
	}

	void SetStream(char* buffer)
	{
		readStream->SetStreamSource(buffer);
		writeStream->SetStreamSource(buffer);
	}

	template<typename type>
	type Read() { return (type)readStream->ReadValue(); }
	int Read(char* val, int count) { return readStream->ReadBytes(val, count); }

	template<typename type>
	void Write(type val) { writeStream->WriteValue(val); }
	void WriteBytes(const char* val, int count) { writeStream->WriteBytes(val, count); }

private:
	Stream* readStream;
	Stream* writeStream;


};

#endif // !_STREAMMANAGER_
