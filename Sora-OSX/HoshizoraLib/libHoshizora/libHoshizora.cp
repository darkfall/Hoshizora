/*
 *  libHoshizora.cp
 *  libHoshizora
 *
 *  Created by Ruiwei Bu on 7/17/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include <iostream>
#include "libHoshizora.h"
#include "libHoshizoraPriv.h"

void libHoshizora::HelloWorld(const char * s)
{
	 libHoshizoraPriv *theObj = new libHoshizoraPriv;
	 theObj->HelloWorldPriv(s);
	 delete theObj;
};

void libHoshizoraPriv::HelloWorldPriv(const char * s) 
{
	std::cout << s << std::endl;
};

