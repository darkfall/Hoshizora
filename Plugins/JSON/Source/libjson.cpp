/*
    This is the implementation of the C interface to libjson
    This file may be included in any C++ application, but it will 
    be completely ignored if JSON_LIBRARY isn't defined.  The 
    only reason JSON_LIBRARY should be defined is when compiling libjson
    as a library
*/
#include "../libjson.h"
#ifdef JSON_LIBRARY

    #include "JSONNode.h"
    #include "JSONWorker.h"
    #include <stdexcept>  //some methods throw exceptions
    #ifdef JSON_MEMORY_MANAGE
	   auto_expand StringHandler; 
	   auto_expand_node NodeHandler;
	   #define MANAGER_INSERT(x) NodeHandler.insert(x)
    #else
	   #define MANAGER_INSERT(x) x
    #endif

    static const json_char * EMPTY_CSTRING = JSON_TEXT("");

    inline json_char * toCString(const json_string & str){
	   const size_t len((str.length() + 1) * sizeof(json_char));
	   #ifdef JSON_MEMORY_MANAGE
		  return (json_char *)StringHandler.insert(memcpy(json_malloc<json_char>(len), str.c_str(), len));
	   #else
		  return (json_char *)memcpy(json_malloc<json_char>(len), str.c_str(), len);
	   #endif
    }
    
    /*
	   stuff that's in namespace libjson
    */
    void json_free(void * str){
	   JSON_ASSERT_SAFE(str, JSON_TEXT("freeing null ptr"), return;);
	   #ifdef JSON_MEMORY_MANAGE
		  StringHandler.remove(str);
	   #endif
	   libjson_free<void>(str);
    }
    
    void json_delete(JSONNODE * node){
	   JSON_ASSERT_SAFE(node, JSON_TEXT("deleting null ptr"), return;);
	   #ifdef JSON_MEMORY_MANAGE
		  NodeHandler.remove(node);
	   #endif
	   JSONNode::deleteJSONNode((JSONNode *)node); 
    }

    #ifdef JSON_MEMORY_MANAGE
	   void json_free_all(void){
		  StringHandler.clear();
	   }
	   
	   void json_delete_all(void){
		  NodeHandler.clear();
	   }
    #endif
    
    JSONNODE * json_parse(json_const json_char * json){
	   JSON_ASSERT_SAFE(json, JSON_TEXT("null ptr to json_parse"), return 0;);
	   try {
		  //use this constructor to simply copy reference instead of copying the temp
		  return MANAGER_INSERT(JSONNode::newJSONNode_Shallow(JSONWorker::parse(TOCONST_CSTR(json))));
	   } catch (std::invalid_argument){}
	   return 0;
    }

    json_char * json_strip_white_space(json_const json_char * json){
	   JSON_ASSERT_SAFE(json, JSON_TEXT("null ptr to json_strip_white_space"), return 0;);
	   return toCString(JSONWorker::RemoveWhiteSpaceAndComments(TOCONST_CSTR(json)));
    }

    #ifdef JSON_VALIDATE
	   JSONNODE * json_validate(json_const json_char * json){
		  JSON_ASSERT_SAFE(json, JSON_TEXT("null ptr to json_validate"), return 0;);
		  try {
			 //use this constructor to simply copy reference instead of copying the temp
			 return MANAGER_INSERT(JSONNode::newJSONNode_Shallow(JSONWorker::validate(TOCONST_CSTR(json))));
		  } catch (std::invalid_argument){}
		  return 0;
	   }
    #endif

    #if defined JSON_DEBUG && !defined JSON_STDERROR
	   //When libjson errors, a callback allows the user to know what went wrong
	   void json_register_debug_callback(json_error_callback_t callback){
		  JSONDebug::register_callback(callback);
	   }
    #endif

    #ifdef JSON_MUTEX_CALLBACKS
	   #ifdef JSON_MUTEX_MANAGE
		  void json_register_mutex_callbacks(json_mutex_callback_t lock, json_mutex_callback_t unlock, json_mutex_callback_t destroy, void * manager_lock){
			 JSONNode::register_mutex_callbacks(lock, unlock, manager_lock);
			 JSONNode::register_mutex_destructor(destroy);
		  }

	   #else
		  void json_register_mutex_callbacks(json_mutex_callback_t lock, json_mutex_callback_t unlock, void * manager_lock){
			 JSONNode::register_mutex_callbacks(lock, unlock, manager_lock);
		  }
	   #endif

	   void json_set_global_mutex(void * mutex){
		  JSONNode::set_global_mutex(mutex);
	   }

	   void json_set_mutex(JSONNODE * node, void * mutex){
		  JSON_ASSERT_SAFE(node, JSON_TEXT("null node to json_set_mutex"), return;);
		  ((JSONNode*)node) -> set_mutex(mutex);
	   }

	   void json_lock(JSONNODE * node, int threadid){
		  JSON_ASSERT_SAFE(node, JSON_TEXT("null node to json_lock"), return;);
		  ((JSONNode*)node) -> lock(threadid);   
	   }

	   void json_unlock(JSONNODE * node, int threadid){
		  JSON_ASSERT_SAFE(node, JSON_TEXT("null node to json_unlock"), return;);
		  ((JSONNode*)node) -> unlock(threadid);   
	   }
    #endif

    #ifdef JSON_MEMORY_CALLBACKS
	   void json_register_memory_callbacks(json_malloc_t mal, json_realloc_t real, json_free_t fre){
		  JSONMemory::registerMemoryCallbacks(mal, real, fre);
	   }
    #endif


    /*
	stuff that's in class JSONNode
	*/
    //ctors
    JSONNODE * json_new_a(json_const json_char * name, json_const json_char * value){
	   if (!name) name = EMPTY_CSTRING;
	   JSON_ASSERT_SAFE(value, JSON_TEXT("null value to json_new_a"), value = EMPTY_CSTRING;);
	   #ifdef JSON_MEMORY_CALLBACKS
		  return MANAGER_INSERT(new(json_malloc<JSONNode>(1)) JSONNode(TOCONST_CSTR(name), json_string(TOCONST_CSTR(value))));
	   #else
		  return MANAGER_INSERT(new JSONNode(TOCONST_CSTR(name), json_string(TOCONST_CSTR(value))));
	   #endif 
    }

    JSONNODE * json_new_i(json_const json_char * name, long value){
	   if (!name) name = EMPTY_CSTRING;	   
	   #ifdef JSON_MEMORY_CALLBACKS
		  return MANAGER_INSERT(new(json_malloc<JSONNode>(1)) JSONNode(TOCONST_CSTR(name), value));
	   #else
		  return MANAGER_INSERT(new JSONNode(TOCONST_CSTR(name), value));
	   #endif 
    }

    JSONNODE * json_new_f(json_const json_char * name, json_number value){
	   if (!name) name = EMPTY_CSTRING;	   
	   #ifdef JSON_MEMORY_CALLBACKS
		  return MANAGER_INSERT(new(json_malloc<JSONNode>(1)) JSONNode(TOCONST_CSTR(name), value));
	   #else
		  return MANAGER_INSERT(new JSONNode(TOCONST_CSTR(name), value));
	   #endif 
    }

    JSONNODE * json_new_b(json_const json_char * name, json_bool_t value){
	   if (!name) name = EMPTY_CSTRING;	   
	   #ifdef JSON_MEMORY_CALLBACKS
		  return MANAGER_INSERT(new(json_malloc<JSONNode>(1)) JSONNode(TOCONST_CSTR(name), (bool)value));
	   #else
		  return MANAGER_INSERT(new JSONNode(TOCONST_CSTR(name), (bool)value));
	   #endif 
    }

    JSONNODE * json_new(char type){
	   #ifdef JSON_MEMORY_CALLBACKS
		  return MANAGER_INSERT(new(json_malloc<JSONNode>(1)) JSONNode(type));
	   #else
		  return MANAGER_INSERT(new JSONNode(type));
	   #endif 
    }

    JSONNODE * json_copy(json_const JSONNODE * orig){
	   JSON_ASSERT_SAFE(orig, JSON_TEXT("null orig to json_copy"), return 0;);
	   #ifdef JSON_MEMORY_CALLBACKS
		  return MANAGER_INSERT(new(json_malloc<JSONNode>(1)) JSONNode(*((JSONNode*)orig)));
	   #else
		  return MANAGER_INSERT(new JSONNode(*((JSONNode*)orig)));
	   #endif 
    }

    JSONNODE * json_duplicate(json_const JSONNODE * orig){
	   JSON_ASSERT_SAFE(orig, JSON_TEXT("null orig to json_duplicate"), return 0;);
	   return MANAGER_INSERT(JSONNode::newJSONNode_Shallow(((JSONNode*)orig) -> duplicate()));
    }

    //assignment
    void json_set_a(JSONNODE * node, json_const json_char * value){
	   JSON_ASSERT_SAFE(node, JSON_TEXT("null node to json_set_a"), return;);
	   JSON_ASSERT_SAFE(value, JSON_TEXT("null value to json_set_a"), value = EMPTY_CSTRING;);
	   *((JSONNode*)node) = json_string(TOCONST_CSTR(value));
    }

    void json_set_i(JSONNODE * node, long value){
	   JSON_ASSERT_SAFE(node, JSON_TEXT("null node to json_set_i"), return;);
	   *((JSONNode*)node) = value;
    }

    void json_set_f(JSONNODE * node, json_number value){
	   JSON_ASSERT_SAFE(node, JSON_TEXT("null node to json_set_f"), return;);
	   *((JSONNode*)node) = value;
    }

    void json_set_b(JSONNODE * node, json_bool_t value){
	   JSON_ASSERT_SAFE(node, JSON_TEXT("null node to json_set_b"), return;);
	   *((JSONNode*)node) = (bool)value;
    }

    void json_set_n(JSONNODE * node, json_const JSONNODE * orig){
	   JSON_ASSERT_SAFE(node, JSON_TEXT("null node to json_set_n"), return;);
	   JSON_ASSERT_SAFE(orig, JSON_TEXT("null node to json_set_n"), return;);
	   *((JSONNode*)node) = *((JSONNode*)orig);
    }


    //inspectors
    char json_type(json_const JSONNODE * node){
	   JSON_ASSERT_SAFE(node, JSON_TEXT("null node to json_type"), return JSON_NULL;);
	   return ((JSONNode*)node) -> type();
    }

    json_index_t json_size(json_const JSONNODE * node){
	   JSON_ASSERT_SAFE(node, JSON_TEXT("null node to json_size"), return 0;);
	   return ((JSONNode*)node) -> size();
    }

    json_bool_t json_empty(json_const JSONNODE * node){
	   JSON_ASSERT_SAFE(node, JSON_TEXT("null node to json_empty"), return true;);
	   return (json_bool_t)(((JSONNode*)node) -> empty());
    }

    json_char * json_name(json_const JSONNODE * node){
	   JSON_ASSERT_SAFE(node, JSON_TEXT("null node to json_name"), return toCString(EMPTY_CSTRING););
	   return toCString(((JSONNode*)node) -> name());
    }

    #ifdef JSON_COMMENTS
	   json_char * json_get_comment(json_const JSONNODE * node){
		  JSON_ASSERT_SAFE(node, JSON_TEXT("null node to json_get_comment"), return toCString(EMPTY_CSTRING););
		  return toCString(((JSONNode*)node) -> get_comment());
	   }
    #endif

    json_char * json_as_string(json_const JSONNODE * node){
	   JSON_ASSERT_SAFE(node, JSON_TEXT("null node to json_as_string"), return toCString(EMPTY_CSTRING););
	   return toCString(((JSONNode*)node) -> as_string());
    }

    long json_as_int(json_const JSONNODE * node){
	   JSON_ASSERT_SAFE(node, JSON_TEXT("null node to json_as_int"), return 0;);
	   return ((JSONNode*)node) -> as_int();
    }

    json_number json_as_float(json_const JSONNODE * node){
	   JSON_ASSERT_SAFE(node, JSON_TEXT("null node to json_as_float"), return 0.0f;);
	   return ((JSONNode*)node) -> as_float();
    }

    json_bool_t json_as_bool(json_const JSONNODE * node){
	   JSON_ASSERT_SAFE(node, JSON_TEXT("null node to json_as_bool"), return false;);
	   return (json_bool_t)(((JSONNode*)node) -> as_bool());
    }

    JSONNODE * json_as_node(json_const JSONNODE * node){
	   JSON_ASSERT_SAFE(node, JSON_TEXT("null node to json_as_node"), return 0;);
	   return MANAGER_INSERT(JSONNode::newJSONNode_Shallow(((JSONNode*)node) -> as_node()));
    }

    JSONNODE * json_as_array(json_const JSONNODE * node){
	   JSON_ASSERT_SAFE(node, JSON_TEXT("null node to json_as_array"), return 0;);
	   return MANAGER_INSERT(JSONNode::newJSONNode_Shallow(((JSONNode*)node) -> as_array()));
    }

    #ifdef JSON_BINARY
	   void * json_as_binary(json_const JSONNODE * node, unsigned long * size){
		  JSON_ASSERT_SAFE(node, JSON_TEXT("null node to json_as_binary"), if (size){*size = 0;} return 0;);
		  const std::string result = ((JSONNode*)node) -> as_binary();
		  const size_t len = result.length();
		  if (size) *size = (json_index_t)len;
		  #ifdef JSON_SAFE
			 if (result.empty()) return 0;
		  #endif
		  #ifdef JSON_MEMORY_MANAGE
			 return StringHandler.insert(memcpy(json_malloc<char>(len), result.data(), len));
		  #else
			 return memcpy(json_malloc<char>(len), result.data(), len);
		  #endif
	   }
    #endif

    #ifdef JSON_WRITER
	   json_char * json_write(json_const JSONNODE * node){
		  JSON_ASSERT_SAFE(node, JSON_TEXT("null node to json_write"), return toCString(EMPTY_CSTRING););
		  return toCString(((JSONNode*)node) -> write());
	   }

	   json_char * json_write_formatted(json_const JSONNODE * node){
		  JSON_ASSERT_SAFE(node, JSON_TEXT("null node to json_write_formatted"), return toCString(EMPTY_CSTRING););
		  return toCString(((JSONNode*)node) -> write_formatted());
	   }
    #endif

    //modifiers
    void json_set_name(JSONNODE * node, json_const json_char * name){
	   JSON_ASSERT_SAFE(node, JSON_TEXT("null node to json_set_name"), return;);
	   JSON_ASSERT_SAFE(name, JSON_TEXT("null name to json_set_name"), name = EMPTY_CSTRING;);
	   ((JSONNode*)node) -> set_name(TOCONST_CSTR(name));
    }

    #ifdef JSON_COMMENTS
	   void json_set_comment(JSONNODE * node, json_const json_char * comment){
		  JSON_ASSERT_SAFE(node, JSON_TEXT("null node to json_set_comment"), return;);
		  JSON_ASSERT_SAFE(comment, JSON_TEXT("null name to json_set_comment"), comment = EMPTY_CSTRING;);
		  ((JSONNode*)node) -> set_comment(TOCONST_CSTR(comment));
	   }
    #endif

    void json_clear(JSONNODE * node){
	   JSON_ASSERT_SAFE(node, JSON_TEXT("null node to json_clear"), return;);
	   ((JSONNode*)node) -> clear();
    }

    void json_nullify(JSONNODE * node){
	   JSON_ASSERT_SAFE(node, JSON_TEXT("null node to json_nullify"), return;);
	   ((JSONNode*)node) -> nullify();
    }

    void json_swap(JSONNODE * node, JSONNODE * node2){
	   JSON_ASSERT_SAFE(node, JSON_TEXT("null node to json_swap"), return;);
	   JSON_ASSERT_SAFE(node2, JSON_TEXT("null node to json_swap"), return;);
	   ((JSONNode*)node) -> swap(*(JSONNode*)node2);
    }

    void json_merge(JSONNODE * node, JSONNODE * node2){
	   JSON_ASSERT_SAFE(node, JSON_TEXT("null node to json_merge"), return;);
	   JSON_ASSERT_SAFE(node2, JSON_TEXT("null node to json_merge"), return;);
	   ((JSONNode*)node) -> merge(*(JSONNode*)node2);
    }

    #ifndef JSON_PREPARSE
	   void json_preparse(JSONNODE * node){
		  JSON_ASSERT_SAFE(node, JSON_TEXT("null node to json_preparse"), return;);
		  ((JSONNode*)node) -> preparse();
	   }
    #endif

    #ifdef JSON_BINARY
	   void json_set_binary(JSONNODE * node, json_const void * data, unsigned long length){
		  JSON_ASSERT_SAFE(node, JSON_TEXT("null node to json_swap"), return;);
		  JSON_ASSERT_SAFE(data, JSON_TEXT("null data to json_set_binary"), *((JSONNode*)node) = EMPTY_CSTRING; return;);
		  ((JSONNode*)node) -> set_binary((unsigned char *)data, (size_t)length);
	   }
    #endif

    void json_cast(JSONNODE * node, char type){
	   JSON_ASSERT_SAFE(node, JSON_TEXT("null node to json_cast"), return;);
	   ((JSONNode*)node) -> cast(type);
    }

    //children access
    void json_reserve(JSONNODE * node, json_index_t siz){
	   JSON_ASSERT_SAFE(node, JSON_TEXT("null node to json_reserve"), return;);
	   ((JSONNode*)node) -> reserve(siz);
    }

    JSONNODE * json_at(JSONNODE * node, unsigned int pos){
	   JSON_ASSERT_SAFE(node, JSON_TEXT("null node to json_at"), return 0;);
	   try {
		  return &((JSONNode*)node) -> at(pos);
	   } catch (std::out_of_range){}
	   return 0;
    }

    JSONNODE * json_get(JSONNODE * node, json_const json_char * name){
	   JSON_ASSERT_SAFE(node, JSON_TEXT("null node to json_get"), return 0;);
	   JSON_ASSERT_SAFE(name, JSON_TEXT("null node to json_get.  Did you mean to use json_at?"), return 0;);
	   try {
		  return &((JSONNode*)node) -> at(TOCONST_CSTR(name));
	   } catch (std::out_of_range){}
	   return 0;
    }


	#ifdef JSON_CASE_INSENSITIVE_FUNCTIONS
	   JSONNODE * json_get_nocase(JSONNODE * node, json_const json_char * name){
		  JSON_ASSERT_SAFE(node, JSON_TEXT("null node to json_at_nocase"), return 0;);
		  JSON_ASSERT_SAFE(name, JSON_TEXT("null name to json_at_nocase"), return 0;);
		  try {
			 return &((JSONNode*)node) -> at_nocase(TOCONST_CSTR(name));
		  } catch (std::out_of_range){}
		  return 0;
	   }

	   JSONNODE * json_pop_back_nocase(JSONNODE * node, json_const json_char * name){
		  JSON_ASSERT_SAFE(node, JSON_TEXT("null node to json_pop_back_nocase"), return 0;);
		  JSON_ASSERT_SAFE(name, JSON_TEXT("null name to json_pop_back_nocase"), return 0;);
		  return MANAGER_INSERT(((JSONNode*)node) -> pop_back_nocase(TOCONST_CSTR(name)));
	   }
    #endif

    void json_push_back(JSONNODE * node, JSONNODE * node2){
	   JSON_ASSERT_SAFE(node, JSON_TEXT("null node to json_push_back"), return;);
	   JSON_ASSERT_SAFE(node2, JSON_TEXT("null node2 to json_push_back"), return;);
	   #ifdef JSON_MEMORY_MANAGE
		  NodeHandler.remove(node2);
	   #endif
	   ((JSONNode*)node) -> push_back((JSONNode*)node2);
    }

    JSONNODE * json_pop_back_at(JSONNODE * node, unsigned int pos){
	   JSON_ASSERT_SAFE(node, JSON_TEXT("null node to json_pop_back_i"), return 0;);
	   return MANAGER_INSERT(((JSONNode*)node) -> pop_back(pos));
    }

    JSONNODE * json_pop_back(JSONNODE * node, json_const json_char * name){
	   JSON_ASSERT_SAFE(node, JSON_TEXT("null node to json_pop_back"), return 0;);
	   JSON_ASSERT_SAFE(name, JSON_TEXT("null name to json_pop_back.  Did you mean to use json_pop_back_at?"), return 0;);
	   return MANAGER_INSERT(((JSONNode*)node) -> pop_back(TOCONST_CSTR(name)));
    }
    
    #ifdef JSON_ITERATORS
	   JSONNODE_ITERATOR json_find(JSONNODE * node, json_const json_char * name){
		  return (JSONNODE_ITERATOR)(((JSONNode*)node) -> find(TOCONST_CSTR(name)));
	   }

	   #ifdef JSON_CASE_INSENSITIVE_FUNCTIONS
		  JSONNODE_ITERATOR json_find_nocase(JSONNODE * node, json_const json_char * name){
			 return (JSONNODE_ITERATOR)(((JSONNode*)node) -> find_nocase(TOCONST_CSTR(name)));
		  }
	   #endif

	   JSONNODE_ITERATOR json_erase(JSONNODE * node, JSONNODE_ITERATOR it){
		  return (JSONNODE_ITERATOR)(((JSONNode*)node) -> erase((JSONNode**)it));
	   }

	   JSONNODE_ITERATOR json_erase_multi(JSONNODE * node, JSONNODE_ITERATOR start, JSONNODE_ITERATOR end){
		  return (JSONNODE_ITERATOR)(((JSONNode*)node) -> erase((JSONNode**)start, (JSONNode**)end));
	   }

	   JSONNODE_ITERATOR json_insert(JSONNODE * node, JSONNODE_ITERATOR it, JSONNODE * node2){
		  #ifdef JSON_MEMORY_MANAGE
			 NodeHandler.remove(node2);
		  #endif
		  return (JSONNODE_ITERATOR)(((JSONNode*)node) -> insert((JSONNode**)it, (JSONNode*)node2));
	   }

	   JSONNODE_ITERATOR json_insert_multi(JSONNODE * node, JSONNODE_ITERATOR it, JSONNODE_ITERATOR start, JSONNODE_ITERATOR end){
		  return (JSONNODE_ITERATOR)(((JSONNode*)node) -> insert((JSONNode**)it, (JSONNode**)start, (JSONNode**)end));
	   }

	   //iterator functions
	   JSONNODE_ITERATOR json_begin(JSONNODE * node){
		  return (JSONNODE_ITERATOR)(((JSONNode*)node) -> begin());
	   }
	   
	   JSONNODE_ITERATOR json_end(JSONNODE * node){
		  return (JSONNODE_ITERATOR)(((JSONNode*)node) -> end());
	   }
    #endif

    //comparison
    json_bool_t json_equal(JSONNODE * node, JSONNODE * node2){
	   JSON_ASSERT_SAFE(node, JSON_TEXT("null node to json_equal"), return false;);
	   JSON_ASSERT_SAFE(node2, JSON_TEXT("null node2 to json_equal"), return false;);
	   return (json_bool_t)(*((JSONNode*)node) == *((JSONNode*)node2));
    }

#endif //JSON_LIBRARY
