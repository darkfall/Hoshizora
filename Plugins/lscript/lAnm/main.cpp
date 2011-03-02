#include <iostream>

#include "lAnm.h"

int main (int argc, char * const argv[]) {
    lAnmPacker* packer = new lAnmPacker;	
	int r = packer->pack("patchouli.txt", "patchouli.lanm");
	if(!r) 
		printf("Error packing anm \n");
	else {
		printf("Pack succeed\n");
	}
	lAnm* amn = packer->unpack("patchouli.lanm");
	if(!amn)
		printf("error unpacking\n");
	
    return 0;
}
