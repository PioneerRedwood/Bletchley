#ifndef _STREAM_
#define _STREAM_

#include <string>

/// <summary>
/// 4바이트 마다 자료를 읽거나 쓰는데 도움을 주는 클래스
/// System.IO.Stream의 클래스를 참고
/// 
/// Read & Write 구분 짓자..
/// </summary>

class Stream
{
public:
	Stream();
	~Stream() { if (buffer != nullptr) delete[] buffer; }

	int GetPosition() { return position; }

	int ReadBytes(char* val, int count);
	void WriteBytes(const char* val, int count);

private:
	int position = 0;
	char* buffer;
};

#endif // _STERAM_