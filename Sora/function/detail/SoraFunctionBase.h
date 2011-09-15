//
//  SoraFunctionImpl.h
//  Hoshizora
//
//  Created by Robert Bu on 8/15/11.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//
#ifndef SoraFunctionImpl_h
#define SoraFunctionImpl_h

#include "SoraTypeTraits.h"

namespace sora {
    
    namespace detail {
        
        union any_ptr {
            void* obj_ptr;
            const void* const_obj_ptr;
            
            void (*func_ptr)();
            
            char data;
        };
        
        struct function_ptr_tag {};
        struct function_obj_tag {};
        struct member_ptr_tag {};
        struct function_obj_ref_tag {};
        
        template<typename F>
        class get_function_tag {
            typedef typename if_c<(is_pointer<F>::value),
                                    function_ptr_tag,
                                    function_obj_tag>::type ptr_or_obj_tag;
            
            typedef typename if_c<(is_member_pointer<F>::value),
                                    member_ptr_tag,
                                    ptr_or_obj_tag>::type ptr_or_obj_or_mem_tag;
            
            typedef typename if_c<(is_reference_wrapper<F>::value),
                                    function_obj_ref_tag,
                                    ptr_or_obj_or_mem_tag>::type or_ref_tag;
            
        public:
            typedef or_ref_tag type;
        };  
        
        inline any_ptr make_any_pointer(void* o) {
            any_ptr ptr;
            ptr.obj_ptr = o;
            return ptr;
        }
        
        inline any_ptr make_any_pointer(const void* o) {
            any_ptr ptr;
            ptr.const_obj_ptr = o;
            return ptr;
        }
        
        inline any_ptr make_any_pointer(void (*fn)()) {
            any_ptr ptr;
            ptr.func_ptr = fn;
            return ptr;
        }
        
    } // namespace sora
    
} // namespace sora


#endif
