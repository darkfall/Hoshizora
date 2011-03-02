/*
封装并统一了从硬盘或内存读取文件的操作
Written by 月灵剑士
*/

#include "MyFileReader.h"

void MyFileReader::Open(char* Src)
{
	if (Status == MFR_FILE_OPEN) MyFileReader::Close();
	fin = fopen(Src, "rb");
	Status = MFR_FILE_OPEN;
}

void MyFileReader::Close()
{
	fclose(fin);
	fin = NULL;
	Status = MFR_NO_FILE;
}

unsigned long MyFileReader::Read(void* dest, unsigned int Bytes)
{
	if (Status == MFR_FILE_OPEN)
	{
		return fread(dest, 1, Bytes, fin);
	}
	else
	{
		throw std::logic_error("No file aviliable");
	}
	return -1;
}

void MyFileReader::Seek(int Origin, int Offset)
{
	if (Status == MFR_FILE_OPEN)
	{
		switch(Origin)
		{
			case MFR_SEEK_SET:
				if (Offset < 0) throw std::invalid_argument("Offset must greater than 0 when Origin is MFR_SEEK_SET");
				fseek(fin, Offset, SEEK_SET);
				break;
			case MFR_SEEK_CUR:
				fseek(fin, Offset, SEEK_CUR);
				break;
			case MFR_SEEK_END:
				if (Offset > 0) throw std::invalid_argument("Offset must less than 0 when Origin is MFR_SEEK_END");
				fseek(fin, Offset, SEEK_END);
				break;
		}
	}
	else
	{
		throw std::logic_error("No file aviliable");
	}
}

unsigned long MyFileReader::Tell()
{
	if (Status == MFR_FILE_OPEN)
	{
		return ftell(fin);
	}
	else
	{
		throw std::logic_error("No file aviliable");
	}
	return -1;
}

unsigned long MyFileReader::GetLength()
{
	if (Status == MFR_FILE_OPEN)
	{
		unsigned long cur = ftell(fin);
		fseek(fin, 0, SEEK_END);
		unsigned long len = ftell(fin);
		fseek(fin, cur, SEEK_SET);
		return len;
	}
	else
	{
		throw std::logic_error("No file aviliable");
	}
	return -1;
}

//-------------------------文件操作与内存操作的境界-------------------------

void MyMemFileReader::SetLength(unsigned long len)
{
	if (Status == MFR_NO_FILE)
	{
		if (len < 0) throw std::invalid_argument("Invalid length.");
		Length = len;
	}
	else
	{
		throw std::logic_error("Set length failed.A file is already open.");
	}
}

void MyMemFileReader::SetTrusteeship(bool trust)
{
	if (Status == MFR_NO_FILE)
	{
		Trusteeship = trust;
	}
	else
	{
		throw std::logic_error("Set trusteeship failed.A file is already open.");
	}
}

void MyMemFileReader::Open(char* Src)
{
	if (Length > 0)
	{
		if (Status == MFR_FILE_OPEN) MyMemFileReader::Close();
		if (Trusteeship)//有托管
		{
			fin = new unsigned char[Length];
			memcpy(fin, Src, Length);
		}
		else
		{
			fin = (unsigned char*)Src;
		}
		Status = MFR_FILE_OPEN;
	}
	else
	{
		throw std::logic_error("Please call SetLength() before open a file in memory.");
	}
}

void MyMemFileReader::Close()
{
	if (Status = MFR_FILE_OPEN)
	{
		Status = MFR_NO_FILE;
		SetLength(0);
		if (fin && Trusteeship) delete[] fin;
		fin = NULL;
		Cursor = 0;
	}
}

unsigned long MyMemFileReader::Read(void *dest, unsigned int Bytes)
{
	if (Status == MFR_FILE_OPEN)
	{
		if (Length == Cursor) return 0;
		unsigned long acBytes = (((Bytes) <= (Length - Cursor)) ? (Bytes) : (Length - Cursor));
		memcpy(dest, &fin[Cursor], acBytes);
		Cursor += acBytes;
		return acBytes;
	}
	else
	{
		throw std::logic_error("No file aviliable");
	}
	return -1;
}

void MyMemFileReader::Seek(int Origin, int Offset)
{
	if (Status == MFR_FILE_OPEN)
	{
		switch(Origin)
		{
			case MFR_SEEK_SET:
				if (Offset < 0) throw std::invalid_argument("Offset must greater than 0 when Origin is MFR_SEEK_SET");
				Cursor = (((Offset) <= (Length)) ? (Offset) : (Length));
				break;
			case MFR_SEEK_CUR:
				if (Offset >= 0)
				{
					Cursor = (((Cursor + Offset) <= (Length)) ? (Cursor + Offset) : (Length));
				}
				else
				{
					Cursor = (((Cursor + Offset) > 0) ? (Cursor + Offset) : 0);
				}
				break;
			case MFR_SEEK_END:
				if (Offset > 0) throw std::invalid_argument("Offset must less than 0 when Origin is MFR_SEEK_END");
				Cursor = (((Length + Offset) > 0) ? (Length + Offset) : 0);
				break;
		}
	}
	else
	{
		throw std::logic_error("No file aviliable");
	}
}

unsigned long MyMemFileReader::Tell()
{
	if (Status == MFR_FILE_OPEN)
	{
		return Cursor;
	}
	else
	{
		throw std::logic_error("No file aviliable");
	}
	return -1;
}

unsigned long MyMemFileReader::GetLength()
{
	if (Status == MFR_FILE_OPEN)
	{
		return Length;
	}
	else
	{
		throw std::logic_error("No file aviliable");
	}
	return -1;
}
