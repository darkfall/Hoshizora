//
//  SoraArray.h
//  Sora
//
//  Created by Ruiwei Bu on 10/2/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraArray_h
#define Sora_SoraArray_h

#include "SoraPlatform.h"

namespace sora {
  
    /**
     * A Simple expandable array class
     * Can get the data pointer directly
     * Used with most collection classes
     **/
    
    template<typename T>
    class SoraArray {
    public:
        typedef T* iterator;
        typedef SoraArray<T> self_type;
        
        SoraArray();
        SoraArray(uint32 capcity, uint32 grow);
        SoraArray(uint32 size, uint32 grow, const T& value);
        SoraArray(const SoraArray<T>& rhs);
        ~SoraArray();
        
        void operator=(const SoraArray<T>& rhs);
        T& operator[](uint32 index) const;
        
        bool operator==(const SoraArray<T>& rhs) const;
        bool operator!=(const SoraArray<T>& rhs) const;
        
        void append(const T& elm);
        void appendArray(const SoraArray<T>& rhs);
        
        void reserve(uint32 num);
        uint32 size() const;
        uint32 capacity() const;
        
        T& front() const;
        T& back() const;
        
        bool empty() const;
        
        void erase(uint32 index);
        iterator erase(iterator iter);
        
        void insert(uint32 index, const T& elm);
        void insert(const T& elm);
        
        void push_back(const T& elm);
        void push_front(const T& elm);
        void pop_back();
        void pop_front();
                
        iterator begin() const;
        iterator end() const;
        
        void clear();
        void reset();
        
        iterator find(const T& elm) const;
        uint32 findIndex(const T& elm) const;
        
        void fill(uint32 start, uint32 num, const T& elm);
        void realloc(uint32 size, uint32 grow);
        
    private:
        void destroyAll();
        void destroy(T* elm);
        void copy(const SoraArray<T>& src);
        void grow();
        void growTo(uint32 capacity);
        void move(uint32 index1, uint32 index2);
        
        uint32 mGrow;
        uint32 mCapacity;
        uint32 mSize;
        T* mElements;
    };
    
    template<typename T>
    SoraArray<T>::SoraArray():
    mGrow(0),
    mCapacity(0),
    mSize(0),
    mElements(0) {
        
    }
    
    template<typename T>
    SoraArray<T>::SoraArray(uint32 capacity, uint32 grow):
    mGrow(grow),
    mCapacity(capacity),
    mSize(0) {
        if(capacity != 0) {
            this->mElements = sora_malloc(sizeof(T) * capacity);
        } else 
            this->mElements = 0;
    }
    
    template<typename T>
    SoraArray<T>::SoraArray(uint32 size, uint32 grow, const T& value):
    mGrow(grow),
    mCapacity(size),
    mSize(size) {
        if(size > 0) {
            this->mElements = (T*)sora_malloc(sizeof(T) * this->mCapacity);
            for(int32 i = 0; i < this->mCapacity; ++i) {
                this->mElements[i] = value;
            }
        } else {
            this->mElements = 0;
        }
    }
    
    template<typename T>
    void SoraArray<T>::copy(const SoraArray<T>& src) {
        sora_assert(mElements == 0);
        
        this->mGrow = src.mGrow;
        this->mCapacity = src.mCapacity;
        this->mSize = src.mSize;
        if(this->mCapacity > 0) {
            this->mElements = (T*)sora_malloc(sizeof(T) * this->mCapacity);
            for(int32 i=0; i<this->mSize; ++i) {
                this->mElements[i] = src.mElements[i];
            }
        }
    }
    
    template<typename T>
    void SoraArray<T>::clear() {
        this->mGrow = 0;
        this->mCapacity = 0;
        this->mSize = 0;
        if(this->mElements) {
            sora_free(this->mElements);
            this->mElements = 0;
        }
    }
    
    template<typename T>
    void SoraArray<T>::destroy(T* ele) {
        ele->~T();
    }
    
    template<typename T>
    SoraArray<T>::SoraArray(const SoraArray<T>& rhs):
    mGrow(0),
    mCapacity(0),
    mSize(0),
    mElements(0) {
        this->copy(rhs);
    }
    
    template<typename T>
    SoraArray<T>::~SoraArray() {
        this->clear();
    }
    
    template<typename T>
    void SoraArray<T>::realloc(uint32 capacity, uint32 grow) {
        this->destroyAll();
        this->mGrow = grow;
        this->mCapacity = capacity;
        this->mSize = 0;
        if(this->mCapacity > 0) {
            this->mElements = (T*)sora_malloc(sizeof(T) * this->mCapacity);
        } else {
            this->mElements = 0;
        }
    }
    
    template<typename T>
    void SoraArray<T>::operator=(const SoraArray<T>& rhs) {
        if(this != &rhs) {
            this->clear();
            this->copy(rhs);
        }
    }
    
    template<typename T>
    void SoraArray<T>::growTo(uint32 newCapacity) {
        T* newArray = (T*)sora_malloc(sizeof(T) * newCapacity);
        if(this->mElements) {
            for(int32 i=0; i<mSize; ++i) {
                newArray[i] = this->mElements[i];
            }
            
            sora_free(this->mElements);
        }
        this->mElements = newArray;
        this->mCapacity = newCapacity;
    }
    
    template<typename T>
    void SoraArray<T>::grow() {
        uint32 toSize;
        if(this->mCapacity == 0) {
            if(this->mGrow == 0)
                toSize = 1;
            else {
                toSize = this->mGrow;
            }
        } else {
            toSize = this->mCapacity * 2;
        }
        this->growTo(toSize);
    }
    
    template<typename T>
    void SoraArray<T>::move(uint32 from, uint32 to) {
        sora_assert(this->mElements);
        sora_assert(from < this->mSize);
        
        if(from == to)
            return;
        
        uint32 num = this->mSize - from;
        uint32 neededSize = to + num;
        while(neededSize < this->mCapacity) {
            this->grow();
        }
        
        if(from > to) {
            // backward move
            int32 i;
            for(i=0; i<num; ++i) {
                this->mElements[to + i] = this->mElements[from + i];
            }
            
            for(i=(from+i)-1; i<this->mSize; ++i) {
                this->destroy(&(this->mElements[i]));
            }
        } else {
            int i=0;
            for(i=num-1; i>=0; --i) {
                this->mElements[to + i] = this->mElements[from + i];
            }
            
            for(i=int(from); i<int(to); ++i) {
                this->destroy(&(this->mElements[i]));
            }
        }
        
        this->mSize = to + num;
    }
    
    template<typename T>
    void SoraArray<T>::append(const T& elemt) {
        if(this->mSize == this->mCapacity) {
            this->grow();
        }
        sora_assert(this->mElements);
        this->mElements[this->mSize++] = elemt;
    }
    
    template<typename T>
    void SoraArray<T>::push_back(const T& elemt) {
        this->append(elemt);
    }
    
    template<typename T>
    void SoraArray<T>::pop_back() {
        sora_assert(this->mElements);
        sora_assert(this->mSize > 0);
        
        this->destroy(this->mElements[this->mSize-1]);
        this->mSize--;
    }
    
    template<typename T>
    void SoraArray<T>::appendArray(const SoraArray<T>& rhs) {
        uint32 size = rhs.size();
        for(int32 i=0; i<size; ++i) {
            this->append(rhs[i]);
        }
    }
    
    template<typename T>
    void SoraArray<T>::reserve(uint32 num) {
        sora_assert(num > 0);
        uint32 neededCapacity = this->mSize + num;
        if(neededCapacity > this->mCapacity) {
            this->growTo(neededCapacity);
        }
    }
    
    template<typename T>
    uint32 SoraArray<T>::size() const {
        return this->mSize;
    }
    
    template<typename T>
    uint32 SoraArray<T>::capacity() const {
        return this->mCapacity;
    }
    
    template<typename T>
    T& SoraArray<T>::operator[](uint32 index) const {
        sora_assert(index < this->mSize);
        sora_assert(this->mElements);
        
        return this->mElements[index];
    }
    
    template<typename T>
    bool SoraArray<T>::operator==(const SoraArray<T>& rhs) const {
        if(rhs.size() == this->size()) {
            for(int32 i=0; i<this->mSize(); ++i) {
                if(!(this->mElements[i] == rhs.mElements[i]))
                    return false;
            }
            return true;
        }
        return false;
    }
    
    template<typename T>
    bool SoraArray<T>::operator!=(const SoraArray<T>& rhs) const {
        return !(*this == rhs);
    }
    
    template<typename T>
    T& SoraArray<T>::front() const {
        sora_assert(this->mElements);
        sora_assert(this->mSize > 0);
        
        return this->mElements[0];
    }
    
    template<typename T>
    T& SoraArray<T>::back() const {
        sora_assert(this->mElements);
        sora_assert(this->mSize > 0);
        
        return this->mElements[this->mSize - 1];
    }
    
    template<typename T>
    bool SoraArray<T>::empty() const {
        return this->mSize == 0;
    }
    
    template<typename T>
    typename SoraArray<T>::iterator SoraArray<T>::erase(typename SoraArray<T>::iterator iter) {
        sora_assert(this->mElements);
        sora_assert(iter < (this->mElements + this->mSize));
        sora_assert(iter >= this->mElements);
        
        this->erase(uint32(iter - this->mElements));
        return iter;
    }
    
    template<typename T>
    void SoraArray<T>::erase(uint32 index) {
        sora_assert(this->mElements);
        sora_assert(index < this->mSize);
        
        if(index == this->mSize-1) {
            this->destroy(&(this->mElements[index]));
            this->mSize--;
        } else {
            this->move(index+1, index);
        }
    }
    
    template<typename T>
    void SoraArray<T>::insert(uint32 index, const T& elemt) {
        sora_assert(index <= this->mSize);
        
        if(index == this->mSize) 
            this->append(elemt);
        else {
            this->move(index, index+1);
            this->mElements[index] = elemt;
        }
    }
    
    template<typename T>
    void SoraArray<T>::pop_front() {
        sora_assert(this->mElements);
        sora_assert(this->mSize >= 1);
        
        if(this->mSize == 1)
            this->clear();
        else {
            this->move(1, 0);
        }
    }
    
    template<typename T>
    void SoraArray<T>::destroyAll() {
        for(int32 i=0; i<this->mSize; ++i) 
            this->destroy(&(this->mElements[i]));
        this->mSize = 0;
    }
    
    template<typename T>
    void SoraArray<T>::reset() {
        this->mSize = 0;
    }
    
    template<typename T>
    typename SoraArray<T>::iterator SoraArray<T>::begin() const {
        return this->mElements;
    }
    
    template<typename T>
    typename SoraArray<T>::iterator SoraArray<T>::end() const {
        return this->mElements + this->mSize;
    }
    
    template<typename T>
    typename SoraArray<T>::iterator SoraArray<T>::find(const T& elemt) const {
        for (int32 i=0; i<this->mSize; ++i) {
            if(this->mElements[i] == elemt)
                return &(this->mElements[index]);
        }
        return 0;
    }
    
    template<typename T>
    uint32 SoraArray<T>::findIndex(const T& elemt) const {
        for(int32 i=0; i<this->mSize; ++i) {
            if(this->mElements[i] == elemt)
                return i;
        }
        return -1;
    }
    
    template<typename T>
    void SoraArray<T>::fill(uint32 index, uint32 num, const T& elemt) {
        if(index + num > this->mSize)
            this->growTo(index + num);
        
        for(int32 i=0; i<index+num; ++i) {
            this->mElements[i] = elemt;
        }
    }
    
    template<typename T>
    void SoraArray<T>::push_front(const T& elemt) {
        if(this->mSize == 0) {
            grow();
            this->mElements[0] = elemt;
            this->mSize++;
        } else {
            this->move(0, 1);
            this->mElements[0] = elemt;
            this->mSize++;
        }
    }
    
} // namespace sora

#endif
