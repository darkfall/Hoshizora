//
//  GridMap.h
//  PartitionTree
//
//  Created by Griffin Bu on 4/2/11.
//  Copyright 2011 Studio GameMaster. All rights reserved.
//

#ifndef SORA_GRID_MAP_H_
#define SORA_GRID_MAP_H_

#include <vector>
#include <cassert>

namespace sora {
    
    template<typename ElementType, int GridSize>
    class GridMap;
    
    template<typename ElementType, int GridSize>
    class GridIterator {
        friend class GridMap<ElementType, GridSize>;
        
    public:
        typedef GridIterator<ElementType, GridSize> ITERATOR;
        typedef typename GridMap<ElementType, GridSize>::GRID_ELEMENTS GridElement;
        
        typedef typename GridMap<ElementType, GridSize>::GridValue ValueType;
        typedef typename GridMap<ElementType, GridSize>::GRID_ELEMENTS_ITERATOR ValueIterator;
        
        typedef typename GridMap<ElementType, GridSize>::ElementValue ElementValue;
        
        GridIterator(): _ValuePtr(NULL) {}
        GridIterator(const ITERATOR& rhs) {
            _ValuePtr = rhs._ValuePtr;
            itIndex =  rhs.itIndex;
        }
        
        ITERATOR& operator ++() {
            assert(_ValuePtr != NULL);
			if(itIndex != _ValuePtr->elements.end()-1 && itIndex != _ValuePtr->elements.end()) {
				++itIndex;
			} else {
				_ValuePtr = _ValuePtr->next;
				while(_ValuePtr->elements.size() == 0 && _ValuePtr->next != NULL) {
					_ValuePtr = _ValuePtr->next;
				}
                itIndex = _ValuePtr->elements.begin();
			}
            return *this;
        }
        ITERATOR& operator --() {
            assert(_ValuePtr != NULL);
			if(itIndex != _ValuePtr->elements.begin()) {
				--itIndex;
			} else {
				_ValuePtr = _ValuePtr->prev;
				while(_ValuePtr->elements.size() == 0 && _ValuePtr->prev != NULL) {
					_ValuePtr = _ValuePtr->prev;
				}
                itIndex = _ValuePtr->elements.begin();
			}
            return *this;
        }
        
        bool operator ==(const ITERATOR& rhs) {
            assert(_ValuePtr != NULL);

            if(_ValuePtr == rhs._ValuePtr)
                return itIndex == rhs.itIndex;
            return false;
        }
        bool operator !=(const ITERATOR& rhs) {
            assert(_ValuePtr != NULL);
            
            if(_ValuePtr == rhs._ValuePtr)
                return itIndex != rhs.itIndex;
            return true;
        }
        
        ElementType operator[] (size_t index) {
            assert(index <= _ValuePtr->elements.size());
            return _ValuePtr->elements[index].value;
        }
        
        ElementType& operator->() {
            assert(_ValuePtr->elements.size() != 0);
            return itIndex->value;
        }
        
        ElementType* operator& () {
            assert(_ValuePtr->elements.size() != 0);
            return &(itIndex->value);
        }
        ElementType& operator* () {
            assert(_ValuePtr->elements.size() != 0);
            return itIndex->value;
        }
        
        ElementValue data() const {
            return *itIndex;
        }
        
        ITERATOR& operator+(int count) {
            for(int i=0; i<count; ++i)
                ++*this;
            return *this;
        }
        
        ITERATOR& operator-(int count) {
            for(int i=0; i<count; ++i)
                --*this;
            return *this;
        }
        
    private:
        ValueType* _ValuePtr;
        ValueIterator itIndex;
		
        GridIterator(ValueType* val): _ValuePtr(val) {
			while(_ValuePtr->elements.size() == 0 && _ValuePtr->next != NULL)
				_ValuePtr = _ValuePtr->next;
            itIndex = _ValuePtr->elements.begin();
		}
    };
    
    template<typename ElementType, int GridSize=16>
    class GridMap {
    public:
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
        
        typedef GridIterator<ElementType, GridSize> Iterator;
        typedef Iterator iterator;
        
        GridMap(int GridWidth, int GridHeight) {
            GRID_WIDTH = GridWidth/GridSize;
            GRID_HEIGHT = GridHeight/GridSize;
            
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
            
            total_size = 0;
        }
        ~GridMap() { 
			delete []grid;
		}
        
        void insert(int x, int y, ElementType element) {
            grid[x/GridSize+y/GridSize*GRID_HEIGHT].elements.push_back(ElementValue(x, y, element));
            total_size += 1;
        }
 
        const GRID_ELEMENTS& get(int x, int y) {
            return grid[x/GridSize+y/GridSize*GRID_HEIGHT];
        }
        
        void remove(int x, int y) {
			int px = x/GridSize;
			int py = y/GridSize;
            int pos = px+py*GRID_HEIGHT;
            assert(pos < GRID_WIDTH*GRID_HEIGHT);
            
			for(int i=0; i<grid[pos].elements.size(); ++i) {
				if(grid[pos].elements[i].x == x && grid[pos].elements[i].y == y) {
					grid[pos].elements.erase(grid[pos].elements.begin()+i);
					total_size -= 1;
				}
			}
        }
        
        iterator find(int x, int y) {
            int px = x/GridSize;
			int py = y/GridSize;
            int pos = px+py*GRID_HEIGHT;
            if(pos >= GRID_WIDTH*GRID_HEIGHT)
                return end();
            
            int i = 0;
            while((grid[pos].elements[i].x != x || grid[pos].elements[i].y != y) && i < grid[pos].elements.size()) 
                ++i;
            if(i == grid[pos].elements.size())
                return end();
            iterator _pos = iterator(&grid[pos]);
            return _pos+i;
        }
        
        iterator erase(iterator pos) {
            if(pos == end())
                return pos;
            
            pos.itIndex = pos._ValuePtr->elements.erase( pos.itIndex );
            if(pos._ValuePtr->elements.size() == 0)
                ++pos;
            total_size -= 1;
            return pos;
        }
        
        iterator erase(iterator _begin, iterator _end) {
            iterator _pos = _begin;

            if(_pos._ValuePtr == _end._ValuePtr) {
                _pos._ValuePtr->elements.erase(_begin.itIndex, _end.itIndex);
            } else {
                while(_pos != _end && _pos != end()) {
                    _pos = erase(_pos);
                }
            }
            return _pos;
        }
        
        iterator begin() {
			if(total_size != 0)
				return iterator(&grid[0]);
			else return iterator(&_end);
        }
        
        iterator end() {
            return iterator(&_end);
        }
        
        ElementType front() {
            assert(total_size != 0);

            return *begin();
        }
        
        ElementType back() {
            assert(total_size != 0);
            
            iterator _back = end();
            --_back;
            return *_back;
        }
        
        size_t size() const { return total_size; }
        size_t grid_size() const { return GRID_WIDTH * GRID_HEIGHT; }
        
    private:
        unsigned int GRID_WIDTH;
        unsigned int GRID_HEIGHT;
		
        GridValue *grid;
        GridValue _end;
        
        size_t total_size;
    };
    
} // namespace sora


#endif
