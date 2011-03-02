#include <iostream>

#include "lZipPacker.h"

int main (int argc, char * const argv[]) {
	lZipPacker* zipPacker = new lZipPacker;
	zipPacker->addFile("llexer.h");
	zipPacker->addFile("llexer.cpp");
	zipPacker->addFile("main.cpp");
	zipPacker->pack("deflate.pack");
	
	lZipPackLoader* zipLoader = new lZipPackLoader;
	zipLoader->loadPack("deflate.pack");
	unsigned long size = zipLoader->getFileSize("llexer.h");
	void* data = malloc(size);
	if(data) {
		zipLoader->getFile("llexer.h", data);
		//printf("%s\n", (char*)data);
	}
	
    /*lPacker* packer = new lPacker;
	packer->addFile("llexer.h");
	packer->addFile("llexer.cpp");
	packer->addFile("main.cpp");
	
	if(packer->pack("./dest.test")) {
	
		lPackLoader* loader = new lPackLoader;
		int	r = loader->loadPack("./dest.test");
		if(r) {
			loader->printInfo();
			
			unsigned long size = loader->getFileSize("llexer.h");
			if(size != 0) {
				void* data = malloc(size);
				if(data) {
					unsigned long rsize = loader->getFile("llexer.h", data, size);
					if(rsize != size) 
						printf("read failed, toReadSize=%d, readSize=%d\n", size, rsize);
					else {
						printf("\n%s", (char*)data);
					}
				}
			}
		}
		else
			printf("critical error\n");
	
	}*/
	return 0;
}
