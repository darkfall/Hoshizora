//
//  GridMap.h
//  PartitionTree
//
//  Created by Griffin Bu on 4/2/11.
//  Copyright 2011 Studio GameMaster. All rights reserved.
//

#ifndef SORA_GRID_MAP_H_
#define SORA_GRID_MAP_H_

#include <list>
#include <vector>
#include <cassert>

namespace sora {
    
    template<typename ElementType, int GridWidth, int GridHeight, int GridSize>
    class GridMap;
    
    template<typename ElementType, int GridWidth, int GridHeight, int GridSize>
    class GridIterator {
        friend class GridMap<ElementType, GridWidth, GridHeight, GridSize>;
        
    public:
        typedef GridIterator<ElementType, GridWidth, GridHeight, GridSize> ITERATOR;
        typedef typename GridMap<ElementType, GridWidth, GridHeight, GridSize>::GRID_ELEMENTS GridElement;
        
        GridIterator(): _ValuePtr(NULL), iIndex(0) {}
        
        ITERATOR& operator ++() {
            assert(_ValuePtr != NULL);
			if(iIndex < _ValuePtr->elements.size()-1) {
				++iIndex;
			} else {
				iIndex = 0;
				_ValuePtr = _ValuePtr->next;
				while(_ValuePtr->elements.size() == 0 && _ValuePtr->next != NULL) {
					_ValuePtr = _ValuePtr->next;
				}
			}
            return *this;
        }
        ITERATOR& operator --() {
            assert(_ValuePtr != NULL);
			if(iIndex > 0) {
				--iIndex;
			} else {
				iIndex = 0;
				_ValuePtr = _ValuePtr->prev;
				while(_ValuePtr->elements.size() == 0 && _ValuePtr->prev != NULL) {
					_ValuePtr = _ValuePtr->prev;
				}
			}
            return *this;
        }
        
        bool operator ==(const ITERATOR& rhs) {
            return _ValuePtr == rhs._ValuePtr;
        }
        bool operator !=(const ITERATOR& rhs) {
            return _ValuePtr != rhs._ValuePtr;
        }
        
        ElementType operator[] (size_t index) {
            assert(index <= _ValuePtr->elements.size());
            return _ValuePtr->elements[index].value;
        }
        
        ElementType& operator->() {
            return _ValuePtr->elements[iIndex].value;
        }
        
        ElementType* operator& () {
            return &_ValuePtr->elements[iIndex].value;
        }
        ElementType& operator* () {
            return _ValuePtr->elements[iIndex].value;
        }
        
    private:
        typedef typename GridMap<ElementType, GridWidth, GridHeight, GridSize>::GridValue ValueType;
        ValueType* _ValuePtr;
        size_t iIndex;
		
        GridIterator(ValueType* val): _ValuePtr(val), iIndex(0) {
			while(_ValuePtr->elements.size() == 0 && _ValuePtr->next != NULL)
				_ValuePtr = _ValuePtr->next;
		}
    };
    
    template<typename ElementType, int GridWidth, int GridHeight, int GridSize=16>
    class GridMap {
    public:
        static const int GRID_WIDTH = GridWidth/GridSize;
        static const int GRID_HEIGHT = GridHeight/GridSize;
		
		typedef struct ElementValue_ {
			ElementType value;
			int x;
			int y;
			
			ElementValue_(int _x, int _y, ElementType _value): x(_x), y(_y), value(_value) {}
		} ElementValue;
        
        typedef std::vector<ElementValue> GRID_ELEMENTS;
		typedef typename std::vector<ElementValue>::iterator GRID_ELEMENTS_ITERATOR;

        typedef struct GridValue_ {
            GRID_ELEMENTS elements;
            GridValue_* next;
            GridValue_* prev;
			
			GridValue_(): next(NULL), prev(NULL) {}
        } GridValue;
        
        typedef GridIterator<ElementType, GridWidth, GridHeight, GridSize> Iterator;
        typedef Iterator iterator;
        
        GridMap() { 
			grid = new GridValue[GRID_WIDTH*GRID_HEIGHT];
            for(int i=0; i<GRID_HEIGHT; ++i) {
                for(int j=0; j<GRID_WIDTH; ++j) {
                    if(i == 0 && j == 0) {
                        grid[j+i*GRID_HEIGHT].prev = NULL;
                        grid[j+i*GRID_HEIGHT].next = &grid[1];
                    } else if(i == GRID_HEIGHT-1 && j == GRID_WIDTH-1) {
                        grid[j+i*GRID_HEIGHT].next = &_end;
                        grid[j+i*GRID_HEIGHT].prev = &grid[GRID_WIDTH*GRID_HEIGHT-2];
                    } else {
						grid[j+i*GRID_HEIGHT].next = &grid[j+i*GRID_HEIGHT+1];
						grid[j+i*GRID_HEIGHT].prev = &grid[j+i*GRID_HEIGHT-1];
                    }
                }
            }
            _end.next = NULL;
            _end.prev = &grid[GRID_WIDTH*GRID_HEIGHT-1];
        }
        ~GridMap() { 
			delete []grid;
		}
        
        inline void insert(int x, int y, ElementType element) {
            grid[x/GridSize+y/GridSize*GRID_HEIGHT].elements.push_back(ElementValue(x, y, element));
            total_size += 1;
        }
 
        inline const GRID_ELEMENTS& get(int x, int y) {
            return grid[x/GridSize+y/GridSize*GRID_HEIGHT];
        }
        
        inline void remove(int x, int y) {
			int px = x/GridSize;
			int py = y/GridSize;
			for(int i=0; i<grid[px+py*GRID_HEIGHT].elements.size(); ++i) {
				if(grid[px+py*GRID_HEIGHT].elements[i].x == x && grid[px+py*GRID_HEIGHT].elements[i].y == y) {
					grid[px+py*GRID_HEIGHT].elements.erase(grid[px+py*GRID_HEIGHT].elements.begin()+i);
					total_size -= 1;
				}
			}
        }
        
        iterator begin() {
			if(total_size != 0)
				return iterator(&grid[0]);
			else return iterator(&_end);
        }
        
        iterator end() {
            return iterator(&_end);
        }
        
        size_t size() const { return total_size; }
        size_t grid_size() const { return GRID_WIDTH * GRID_HEIGHT; }
        
    private:
        GridValue *grid;
        GridValue _end;
        
        size_t total_size;
    };
    
} // namespace sora


#endif
