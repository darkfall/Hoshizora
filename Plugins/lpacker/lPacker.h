/*
 *  lPacker.h
 *  lpack
 *
 *  Created by GriffinBu on 12/8/10.
 *  Copyright 2010 GriffinBu(Project L). All rights reserved.
 *
 */

/*
 12-08 2010, rev 0.1, first draft
 12-08 2010, rev 0.2, add data handling functions
 12-08 2010, rev 0.3, add version set
 */

#ifndef _LPACKER_H_
#define _LPACKER_H_

#include <vector>
#include <string>

typedef unsigned char	uint8;
typedef unsigned short	uint16;
typedef unsigned int	uint32;

#define LPACKER_HEADER "LPCK" // first 4 byte
#define LPACKER_MAJOR_VER 0
#define LPACKER_MINOR_VER 3

struct LPACKER_HEAD {
	uint8 majorVer; // 1 -5
	uint8 minorVer; // 1 -6
	
	uint32 fileNum; // 4 -7-10
	
	LPACKER_HEAD(): majorVer(255), minorVer(255) {}
};

typedef struct {
	uint32 offset;		// 4
	uint32 size;		// 4
	uint32 origSize;	// 4
	
	uint16 nameRefNum;	// 2
	
	std::string sName;  // temporary name
} LPACKER_NODE; // 14

// Info Part Size = 10+14*fileNum;
// Body Part Size = E(0-fileNum)(fileSize)
// String Table Size = 2*fileNum+E(0-fileNum)(filePathSize)

class lPacker {
public:
	lPacker();
	virtual ~lPacker();
	
	/*
	 add a file to pack
	 @param pstrFile, the path of the zip file
	 */
	void addFile(const char* pstrFile);
	/*
	 del a file from pack list or pack
	 @param pstrFile, the path or the name of the file
	 */
	void delFile(const char* pstrFile);
	
	/*
	 pack the files in the pack list to a packed file
	 @param pstrDest, the dest path of the packed file
	 @retval, return 1 = succeed, otherwise failed
	 */
	int pack(const char* pstrDest);
	
	/*
	 virtual data getter
	 @param pf, the file pointer
	 @param size, reference to the size of the file, must be set
	 @param origSize, reference to the orginal size of the file, must be set
	 @retval, pointer to the data to be written to the pack
	 
	 inheritate the class and implement the function to handle the data before add to the pack when packing
	 such as compression
	 */
	virtual void* getData(FILE* pf, uint32& size, uint32& origSize);
	
	/*
	 set the version of the packer, would write to pack file to use as an identifier between different packers
	 @param major, major version
	 @param minor, minor version
	 */
	void setVer(uint32 major, uint32 minor);
	
private:	
	int end(FILE* pf);
		
	typedef std::vector<std::string> PACK_LIST;
	typedef PACK_LIST::iterator PACK_LIST_IT;
	PACK_LIST toPackFiles;
	PACK_LIST failedFiles;
	
	LPACKER_HEAD header;
	
	typedef std::vector<LPACKER_NODE> PACK_FILE_LIST;
	typedef PACK_FILE_LIST::iterator PACK_FILE_LIST_IT;
	PACK_FILE_LIST packNodes;
};

class lPackLoader {
public:
	lPackLoader();
	virtual ~lPackLoader();
	
	/*
	 load a pack packed by lPacker
	 @param pstrFilePath, the path of the pack
	 @retval, return 1 = succeed, otherwise failed
	 */
	int loadPack(const char* pstrFilePath);
	
	/*
	 close the pack file
	 */
	void close();

	/*
	 get the original size of a file in the pack
	 @param pstrFileName, the name of the file
	 @retval, the size of the file in pack
	 */
	uint32 getFileSize(const char* pstrFileName);
	/*
	 get a file's data in the pack
	 @param pstrFileName, the name of the file
	 @param pReadData, pointer of the memory block to read the file data in, managed by user
	 @param readSize, the size to read
	 @retval, read size, if not equal to readSize or equals to 0 then error happened
	 */
	uint32 getFile(const char* pstrFileName, void* pReadData);
	
	
	/*
	 virtual data reader
	 @param pDataIn, the pointer to read data
	 @param pDataOut, the pointer to handled data
	 @param size, size of read data, equals to sizeof(pDataIn)
	 
	 pDataIn would be free after calling, remember copy the data to pDataOut
	
	 ****
		sizeof(pDataIn) = NODE.size
		sizeof(pDataOut) = NODE.origSize
	 
	 inheritate the class and implement the function to handle the data before return with getFile
	 such as decompression
	 */
	virtual uint32 getData(void* pDataIn, void* pDataOut, LPACKER_NODE node);
		
	/*
	 set the version of the packloader, would compare with the version number written in the pack
	 @param major, major version
	 @param minor, minor version
	 */
	void setVer(uint32 major, uint32 minor);
	
private:
	typedef std::vector<LPACKER_NODE> PACK_FILE_LIST;
	typedef PACK_FILE_LIST::iterator PACK_FILE_LIST_IT;
	PACK_FILE_LIST packNodes;
	
	FILE* packFile;
	
	LPACKER_HEAD header;
};


#endif // _LPACKER_H_