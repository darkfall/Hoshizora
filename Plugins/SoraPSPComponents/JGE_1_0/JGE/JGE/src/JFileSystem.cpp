//-------------------------------------------------------------------------------------
//
// JGE++ is a hardware accelerated 2D game SDK for PSP/Windows.
//
// Licensed under the BSD license, see LICENSE in JGE root for details.
// 
// Copyright (c) 2007 James Hui (a.k.a. Dr.Watson) <jhkhui@gmail.com>
// 
//-------------------------------------------------------------------------------------

#pragma warning(disable : 4786)

#include "../include/JGE.h"
#include "../include/JFileSystem.h"
#include "tinyxml/tinyxml.h"
#include "unzip/unzip.h"


#include <stdio.h>
#include <vector>
#include <map>
#include <string>

JFileSystem* JFileSystem::mInstance = NULL;

JFileSystem* JFileSystem::GetInstance()
{
	if (mInstance == NULL)
	{
		mInstance = new JFileSystem();
	}

	return mInstance;
}


void JFileSystem::Destroy()
{
	if (mInstance)
	{
		delete mInstance;
		mInstance = NULL;
	}
}


JFileSystem::JFileSystem()
{
	mZipAvailable = false;
#ifdef WIN32
	mFile = NULL;
#else
	mFile = -1;
#endif
	mPassword = NULL;
	mZipFile = NULL;
	mFileSize = 0;

	mResourceRoot = "Res/";				// default root folder 
}


JFileSystem::~JFileSystem()
{
	if (mZipAvailable && mZipFile != NULL)
		unzCloseCurrentFile(mZipFile);
}


bool JFileSystem::AttachZipFile(const string &zipfile, char *password /* = NULL */)
{
	if (mZipAvailable && mZipFile != NULL)
	{
		if (mZipFileName != zipfile)
			unzCloseCurrentFile(mZipFile);		// close the previous zip file
	}
	
	mZipFileName = zipfile;
	mPassword = password;

	mZipFile = unzOpen(mZipFileName.c_str());

	if (mZipFile != NULL)
	{
		mZipAvailable = true;
		return true;
	}

	return false;
}


void JFileSystem::DetachZipFile()
{
	if (mZipAvailable && mZipFile != NULL)
	{
		unzCloseCurrentFile(mZipFile);
	}
	
	mZipFile = NULL;
	mZipAvailable = false;
}


bool JFileSystem::OpenFile(const string &filename)
{

	string path = mResourceRoot + filename;

	if (mZipAvailable && mZipFile != NULL)
	{
		if (unzLocateFile(mZipFile, path.c_str(), 0) != UNZ_OK)
			return false;

		char filenameInzip[256];
		unz_file_info fileInfo;

		if (unzGetCurrentFileInfo(mZipFile, &fileInfo, filenameInzip, sizeof(filenameInzip), NULL, 0, NULL, 0) == UNZ_OK)
			mFileSize = fileInfo.uncompressed_size;
		else
			mFileSize = 0;
		
		return (unzOpenCurrentFilePassword(mZipFile, mPassword) == UNZ_OK);
	}
	else
	{
		#ifdef WIN32
			mFile = fopen(path.c_str(), "rb");
			if (mFile != NULL)
			{
				fseek(mFile, 0, SEEK_END);
				mFileSize = ftell(mFile);
				fseek(mFile, 0, SEEK_SET);
				return true;
			}
		#else
			mFile = sceIoOpen(path.c_str(), PSP_O_RDONLY, 0777);
			if (mFile > 0)
			{
				mFileSize = sceIoLseek(mFile, 0, PSP_SEEK_END);
				sceIoLseek(mFile, 0, PSP_SEEK_SET);
				return true;
			}
		#endif		
		
			
	}
	
	return false;
			
}


void JFileSystem::CloseFile()
{
	if (mZipAvailable && mZipFile != NULL)
		return;

	#ifdef WIN32
		if (mFile != NULL)
			fclose(mFile);
	#else
		if (mFile > 0)
			sceIoClose(mFile);
	#endif
}


int JFileSystem::ReadFile(void *buffer, int size)
{
	if (mZipAvailable && mZipFile != NULL)
	{
		return unzReadCurrentFile(mZipFile, buffer, size);
	}
	else
	{
		#ifdef WIN32
			return fread(buffer, 1, size, mFile);
		#else
			return sceIoRead(mFile, buffer, size);
		#endif
	}
}


int JFileSystem::GetFileSize()
{
	return mFileSize;
}


void JFileSystem::SetResourceRoot(const string& resourceRoot)
{
	mResourceRoot = resourceRoot;
}
