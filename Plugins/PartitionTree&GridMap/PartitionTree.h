/*
 *  PartitionTree.h
 *  PartitionTree
 *
 *  Created by Griffin Bu on 4/2/11.
 *  Copyright 2011 Studio GameMaster. All rights reserved.
 *
 */

#ifndef SORA_PARTITION_TREE_H_
#define SORA_PARTITION_TREE_H_

#include <list>
#include <iostream>

#include <cassert>

namespace sora {
    
    template<typename ElementType, int minPartitionSize>
    class PartitionTree;
    
    template<typename ElementType, int minPartitionSize>
    class PartitionTreeIterator {
        friend class PartitionTree<ElementType, minPartitionSize>;
        
    public:
        typedef typename PartitionTree<ElementType, minPartitionSize>::PARTITION_NODE ValueType;
        typedef typename PartitionTree<ElementType, minPartitionSize>::PARTITION_ELEMENT_ITERATOR ElementTypeIterator;
        
        typedef PartitionTreeIterator<ElementType, minPartitionSize> ITERATOR;
        
        PartitionTreeIterator(): _ValuePtr(NULL) {}
        
        ITERATOR& operator++() {
            if(_itIndex != _ValuePtr->elements.end()-1) {
                ++_itIndex;
            } else {
                if(_ValuePtr->nextNode != NULL)
                    _ValuePtr = _ValuePtr->nextNode;
            }
            return *this;
        }
        
        ITERATOR& operator--() {
            if(_itIndex != _ValuePtr->elements.begin()) {
                --_itIndex;
            } else {
                if(_ValuePtr->prevNode != NULL) 
                    _ValuePtr = _ValuePtr->prevNode;
            }
            return *this;
        }
        
        ElementType& operator*() {
            return _itIndex->element;
        }
        ElementType* operator&() {
            return &_itIndex->element;
        }
        ElementType& operator->() {
            return _itIndex->element;
        }
        
    private:
        ValueType _ValuePtr;
        ElementTypeIterator _itIndex;
        
        PartitionTreeIterator(ValueType* pVal): _ValuePtr(pVal) {
            assert(_ValuePtr != NULL && _ValuePtr->isLeaf());
            _itIndex = _ValuePtr->elements.begin();
        }
    };
	
	/*
	 A special Quadtree for fast iteration but slower insertion
	 */
	template<typename ElementType, int minPartitionSize=8>
	class PartitionTree {
	public:
		struct ELEMENT_CONT {
			int x, y;
			ElementType element;
			
			ELEMENT_CONT(int _x, int _y, ElementType _element): x(_x), y(_y), element(_element) {}
		};
		
		typedef std::list<ELEMENT_CONT> PARTITION_ELEMENT;
		typedef typename std::list<ELEMENT_CONT>::iterator PARTITION_ELEMENT_ITERATOR;
		
		struct PARTITION;
		struct PARTITION_NODE {
			int x, y;
			PARTITION_ELEMENT elements;
			
			PARTITION* next;
            PARTITION* parent;
			
			PARTITION_NODE(): next(NULL), x(0), y(0), nextNode(NULL), prevNode(NULL), parent(NULL) {}
			~PARTITION_NODE() { 
				if(!isLeaf()) {
					delete next;
					next = 0;
				}
			}
			
			void set(int _x, int _y) { x = _x; y = _y; }
			bool isLeaf() { return (next == NULL); }
			size_t size() { 
				if(isLeaf()) {
					return elements.size();
				}
				return 0;
			}
            
            PARTITION_NODE* nextNode;
            PARTITION_NODE* prevNode;
		};
		
		struct PARTITION {
			PARTITION_NODE upperLeft;     // x1, y1
			PARTITION_NODE upperRight;	  // x2, y1
			PARTITION_NODE lowerRight;    // x2, y2
			PARTITION_NODE lowerLeft;	  // x1, y2
			
			int midx, midy;
		};
		
		PartitionTree(int x1, int y1, int x2, int y2) {
			root = _initPartition(x1, y1, x2, y2);
            total_size = 0;
		}
	
		~PartitionTree() {
            if(root)
                delete root;
		}
		
		void clear() {
			int x1 = root->upperLeft.x,
				y1 = root->upperLeft.y,
				x2 = root->lowerRight.x,
				y2 = root->lowerRight.y;
			if(root) 
				delete root;
			
			root = _initPartition(x1, y1, x2, y2);
		}
		
		/* 
			insert an element into the Tree
			@param x & y, the coordinate of the element to insert
			@param element, the element to insert
			@retval, the action succeed or not, if not, maybe x, y is beyond the root partition size
		 */
		bool insert(int x, int y, ElementType element) {
			// coordinate beyond root partition
			if(x < root->upperLeft.x || x > root->lowerRight.x ||
			   y < root->upperLeft.y || y > root->lowerRight.y)
				return false;
			
			_insertElement(root, x, y, element);
			total_size += 1;
			return true;
		}
		
		/* 
			remove an element from the tree
			@param x & y, the coordinate of the element to remove
			@param restrict, if this is false, then the remove action would try to remove all elements in a minimun partition
							 otherwise would search for element that has the exact same coordinate to remove
		 */
		void remove(int x, int y, bool bRestrict=true) {
			_removePartition(root, x, y, bRestrict);
		}
		
		void get(int x, int y, PARTITION_ELEMENT& cont, bool bRestrict=true) {
			_findPartition(root, x, y, cont, bRestrict);
		}
		
		size_t size() const { return total_size; }
		
	private:
		inline PARTITION* _initPartition(int x1, int y1, int x2, int y2) {
			PARTITION* part = new PARTITION;
			assert(part != NULL);
			
			// coordinate must be upperLeft(x1, y1), lowerRight(x2, y2);
			if(x1 > x2) {
				std::swap(x1, x2);
				std::swap(y1, y2);
			}
			
			part->upperLeft.set(x1, y1);
			part->upperRight.set(x2, y1);
			part->lowerRight.set(x2, y2);
			part->lowerLeft.set(x1, y2);
			
			part->midx = (x1+x2)/2; part->midy = (y1+y2)/2;
			return part;
		}
		
		inline void _insertElement(PARTITION* part, int x, int y, ElementType element) {
			if(x <= part->midx && y <= part->midy) {
				_insertElementInNode(part, &part->upperLeft, x, y, element);
				return;
			}
			if(x >= part->midx && y <= part->midy) {
				_insertElementInNode(part, &part->upperRight, x, y, element);
				return;
			}
			if(x >= part->midx && y >= part->midy) {
				_insertElementInNode(part, &part->lowerRight, x, y, element);
				return;
			}
			if(x <= part->midx && y >= part->midy) {
				_insertElementInNode(part, &part->lowerLeft, x, y, element);
			}
		}
		
		inline void _insertElementInNode(PARTITION* part, PARTITION_NODE* node, int x, int y, ElementType element) {
			if(!node->isLeaf()) {
				_insertElement(node->next, x, y, element);
				return;
			}

			int w = abs(part->midx - node->x), h = abs(part->midy - node->y);
        
			// minimum partition size
			if(w <= PART_SIZE && h <= PART_SIZE) {
				node->elements.push_back(ELEMENT_CONT(x, y, element));
			} else {
				if(node->next == NULL) {
					node->next = _initPartition(node->x, node->y, part->midx, part->midy);
                    node->parent = part;
                }
				_insertElement(node->next, x, y, element);
			}
		}
		
		inline void _findPartition(PARTITION* part, int x, int y, PARTITION_ELEMENT& cont, bool bRestrict) {
			if(x <= part->midx && y <= part->midy) {
				_findElementInNode(&part->upperLeft, x, y, cont, bRestrict);
				return;
			}
			if(x >= part->midx && y <= part->midy) {
				_findElementInNode(&part->upperRight, x, y, cont, bRestrict);
				return;
			}
			if(x >= part->midx && y >= part->midy) {
				_findElementInNode(&part->lowerRight, x, y, cont, bRestrict);
				return;
			}
			if(x <= part->midx && y >= part->midy) {
				_findElementInNode(&part->lowerLeft, x, y, cont, bRestrict);
			}
		}
		
		inline void _findElementInNode(PARTITION_NODE* node, int x, int y, PARTITION_ELEMENT& cont, bool bRestrict) {
			if(node->isLeaf()) {
				PARTITION_ELEMENT_ITERATOR itElement = node->elements.begin();
				while(itElement != node->elements.end()) {
					if(bRestrict) {
						if(itElement->x == x && itElement->y == y) {
							cont.push_back(*itElement);
						}
					} else {
						cont.push_back(*itElement);
					}
					
					++itElement;
				}
			} else {
				_findPartition(node->next, x, y, cont, bRestrict);
			}
		}
		
		inline void _removePartition(PARTITION* part, int x, int y, bool bRestrict) {
			if(x <= part->midx && y <= part->midy) {
				_removeElementInNode(&part->upperLeft, x, y, bRestrict);
				return;
			}
			if(x >= part->midx && y <= part->midy) {
				_removeElementInNode(&part->upperRight, x, y, bRestrict);
				return;
			}
			if(x >= part->midx && y >= part->midy) {
				_removeElementInNode(&part->lowerRight, x, y, bRestrict);
				return;
			}
			if(x <= part->midx && y >= part->midy) {
				_removeElementInNode(&part->lowerLeft, x, y, bRestrict);
			}
		}
		
		inline void _removeElementInNode(PARTITION_NODE* node, int x, int y, bool bRestrict) {
			if(node->isLeaf()) {
				if(bRestrict) {
					PARTITION_ELEMENT_ITERATOR itElement = node->elements.begin();
					while(itElement != node->elements.end()) {
						if(itElement->x == x && itElement->y == y) {
							node->elements.erase(itElement);
                            total_size -= 1;
						}
						++itElement;
					}
				} else {
                    total_size -= node->elements.size();
					node->elements.clear();
				}
			} else {
				_removePartition(node->next, x, y, bRestrict);
			}
		}
		
		static const int PART_SIZE = minPartitionSize;
		
		PARTITION* root;
		
		size_t total_size;
	};
	
} // namespace sora


#endif