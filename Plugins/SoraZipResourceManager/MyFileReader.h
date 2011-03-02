/*
封装并统一了从硬盘或内存读取文件的操作
Written by 月灵剑士
*/

#ifndef MY_FILE_READER_H
#define MY_FILE_READER_H

#define MFR_NO_FILE 0
#define MFR_FILE_OPEN 1

#define MFR_SEEK_SET -1
#define MFR_SEEK_CUR 0
#define MFR_SEEK_END 1

#include<cstdio>
#include<cstring>
#include<stdexcept>

class MyFileReaderBasis
{
public:
	MyFileReaderBasis(){Status = MFR_NO_FILE;};
	virtual void Open(char* Src) = 0;
	virtual void Close() = 0;
	virtual unsigned long Read(void* dest,unsigned int Bytes) = 0;//返回实际读取的字节数
	virtual void Seek(int Origin, int Offset) = 0;
	virtual unsigned long Tell() = 0;
	virtual unsigned long GetLength() = 0;
	int GetStatus(){return Status;};
protected:
	int Status;
};

class MyFileReader : public MyFileReaderBasis
{
public:
	MyFileReader(){fin = NULL;};
	virtual void Open(char* Src);
	virtual void Close();
	virtual unsigned long Read(void* dest,unsigned int Bytes);
	virtual void Seek(int Origin, int Offset);
	virtual unsigned long Tell();
	virtual unsigned long GetLength();
private:
	FILE* fin;
};

class MyMemFileReader : public MyFileReaderBasis
{
public:
	MyMemFileReader(){fin = NULL; Cursor = 0; Length = 0; Trusteeship = false;};
	void SetLength(unsigned long len);
	void SetTrusteeship(bool trust);
	virtual void Open(char* Src);
	virtual void Close();
	virtual unsigned long Read(void* dest,unsigned int Bytes);
	virtual void Seek(int Origin, int Offset);
	virtual unsigned long Tell();
	virtual unsigned long GetLength();
private:
	unsigned char* fin;
	unsigned int Cursor;
	unsigned long Length;
	bool Trusteeship;
};

#endif
