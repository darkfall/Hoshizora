#include <iostream>

#include "lAnm.h"

int main (int argc, char * const argv[]) {
    lAnmPacker* packer = new lAnmPacker;
	int id = packer->addNode(0);
	packer->addNodeTex(id, 0, 0, 64, 64);
	packer->addNodeTex(id, 0, 64, 64, 64);
	packer->addNodeTex(id, 0, 128, 64, 64);
	id = packer->addNode(1);
	packer->addNodeTex(id, 64, 0, 64, 64);
	packer->addNodeTex(id, 64, 64, 64, 64);
	
	int r = packer->pack("cake2.png", "cake.lanm");
	if(!r) 
		printf("Error packing anm \n");
	else {
		printf("Pack succeed\n");
	}
	packer->clear();
	
	r = packer->pack("cake.anms", "cake2.png", "cake.lanm.2");
	if(!r) 
		printf("Error packing anm \n");
	else {
		printf("Pack succeed\n");
	}
	
	r = packer->unpackToFile("cake.lanm", "cake.png");
	if(!r)
		printf("unpack error\n");
	else {
		printf("unpack succeed\n");
	}
	
	r = packer->pack("cake.lanm.txt", "cake.png", "cake.lanm.3");


	
    return 0;
}
