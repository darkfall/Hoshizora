/*
 *  tcmalloc_test.h
 *  Sora
 *
 *  Created by GriffinBu on 1/22/11.
 *  Copyright 2011 Griffin Bu(Project L). All rights reserved.
 *
 */

#ifndef TC_MALLOC_TEST_H_
#define TC_MALLOC_TEST_H_

#include "SoraPlatform.h"
#include "SoraObject.h"
#include "SoraCore.h"

#include <mach/mach.h>
#include <mach/mach_time.h>
#include <tcmalloc/tcmalloc.h>

namespace sora {
	
	const int TCMALLOC_TESTSIZE = 1000000;

	class tcmalloc_test {
	public:
		static void test() {
			uint64_t t = mach_absolute_time();
			for(size_t i=0; i<TCMALLOC_TESTSIZE; ++i) {
				SoraObject* obj = (SoraObject*)tc_new(sizeof(SoraObject));
				tc_delete(obj);
			}
			uint64_t t2 = mach_absolute_time();

			printf("tc_malloc %d SoraObject, time=%d\n", TCMALLOC_TESTSIZE, t2-t);
			
			t = mach_absolute_time();
			for(size_t i=0; i<TCMALLOC_TESTSIZE; ++i) {
				SoraObject* obj = (SoraObject*)malloc(sizeof(SoraObject));
				free(obj);
			}
			t2 = mach_absolute_time();
			printf("cpp new %d SoraObject, time=%d\n", TCMALLOC_TESTSIZE, t2-t);
		}
	};

} // namespace sora

#endif