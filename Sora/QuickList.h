#ifndef QUICK_LIST
#define QUICK_LIST

#include <list>
#include <assert.h>

template<typename T, int allocSize>
class QuickList;

template<typename T, int allocSize>
class QuickListIterator
{
	friend class QuickList<T, allocSize>;
public:
	QuickListIterator(): ptr_(NULL) {};

	T & operator * ()
		{	return ptr_->data;	};

	T * operator -> ()
		{	return &ptr_->data;	};

	QuickListIterator<T, allocSize> & operator ++ ()
		{	ptr_ = ptr_->next; return *this;	};

	QuickListIterator<T, allocSize> & operator -- ()
		{	ptr_ = ptr_->prev; return *this;	};

	bool operator == (QuickListIterator<T, allocSize> const &a)
		{	return a.ptr_ == this->ptr_;	};

	bool operator != (QuickListIterator<T, allocSize> const &a)
		{	return a.ptr_ != this->ptr_;	};

	bool operator () ()
		{	return ptr_->prev & ptr_->next;	};

private:
	typedef typename QuickList<T, allocSize>::ValueType ValueType;
	ValueType *ptr_;

	QuickListIterator(ValueType *ptr): ptr_(ptr) {};
};

/*
	@brief container QuickList designed for bullet container
	T: data type, allocSize: pData = new( allocSize/sizeof(T) )
*/
template<typename T, int allocSize = 4048>
class QuickList
{
public:
	QuickList();

	typedef struct ValueType_
	{
		T data;
		ValueType_ *prev;
		ValueType_ *next;
	} ValueType;
	typedef QuickListIterator<T, allocSize> Iterator;

	typedef ValueType value_type;
	typedef Iterator iterator;

	void push_back(T const &a);
	void push_front(T const &a);
	void pop_back();
	void pop_front();

	T & back();
	T & front();

	Iterator insert(Iterator it_where, T const &a);
	Iterator erase(Iterator it);

	Iterator begin();
	Iterator end();

	void eraseAll();
	void clear();

	size_t size();

private:
	//ARRAY_SIZE>=2
	static const unsigned long ARRAY_SIZE = allocSize/sizeof(T) + 2;

	typedef struct
	{
		ValueType data[ARRAY_SIZE];
	} DataArray;
	typedef std::list<DataArray> DataArrayList;
	DataArrayList dataArrayList;

	size_t size_;

	//ValueType *pHead_;
	//ValueType *pRear_;
	ValueType beg_;
	ValueType end_;

	ValueType *pFreeHead_;
	ValueType *pFreeRear_;
	//alloc new space

	void alloc();
};

template<typename T, int allocSize>
QuickList<T, allocSize>::QuickList()
	: pFreeHead_(NULL), pFreeRear_(NULL), size_(0)
{
	beg_.prev = NULL;
	beg_.next = &end_;
	end_.prev = &beg_;
	end_.next = NULL;

	this->alloc();

	return;
}


template<typename T, int allocSize>
void QuickList<T, allocSize>::push_back(T const &a)
{
	assert( pFreeHead_ && pFreeRear_ && pFreeHead_!=pFreeRear_ );

	//pValue = pFreeHead_
	pFreeHead_->data = a;

	ValueType *pNewFreeHead = pFreeHead_->next;

	//modify data link, pNewRear = pFreeHead_
	ValueType *&pRear = end_.prev;
	if(pRear != &beg_)		//not empty
	{
		pRear->next = pFreeHead_;

		pFreeHead_->prev = pRear;
		pFreeHead_->next = &end_;

		pRear = pFreeHead_;
	}
	else		//empty
	{
		assert(beg_.next==&end_);

		pFreeHead_->prev = &beg_;
		pFreeHead_->next = &end_;

		beg_.next = pFreeHead_;
		end_.prev = pFreeHead_;
	}

	//modify free link, remove head
	pFreeHead_ = pNewFreeHead;
	pFreeHead_->prev = NULL;

	//if need new space
	if( pFreeHead_==pFreeRear_ )
	{
		this->alloc();
	}

	++this->size_;
}


template<typename T, int allocSize>
void QuickList<T, allocSize>::push_front(T const &a)
{
	assert( pFreeHead_ && pFreeRear_ && pFreeHead_!=pFreeRear_ );

	//pValue = pFreeHead_
	pFreeHead_->data = a;

	ValueType *pNewFreeHead = pFreeHead_->next;

	//modify data link, pNewHead = pFreeHead_
	ValueType *&pHead = beg_.next;
	if(pHead != &end_)		//not empty
	{
		pHead->prev = pFreeHead_;

		pFreeHead_->next = pHead;
		pFreeHead_->prev = &beg_;

		pHead = pFreeHead_;
	}
	else		//empty
	{
		assert(end_.prev==&beg_);

		pFreeHead_->prev = &beg_;
		pFreeHead_->next = &end_;

		beg_.next = pFreeHead_;
		end_.prev = pFreeHead_;
	}

	//modify free link, remove head
	pFreeHead_ = pNewFreeHead;
	pFreeHead_->prev = NULL;

	//if need new space
	if( pFreeHead_==pFreeRear_ )
	{
		this->alloc();
	}

	++this->size_;
}


template<typename T, int allocSize>
void QuickList<T, allocSize>::pop_back()
{
	assert( pFreeHead_ && pFreeRear_ && pFreeHead_!=pFreeRear_ );

	ValueType *&pRear = end_.prev;
	ValueType *pOldRear = pRear;

	//modify data link, pNewRear = pRear->prev
	assert(pRear!=&beg_);
	pRear = pRear->prev;
	pRear->next = &end_;

	//modify free link, add to rear
	pFreeRear_->next = pOldRear;

	pOldRear->prev = pFreeRear_;
	pOldRear->next = NULL;

	pFreeRear_ = pOldRear;

	--this->size_;

	return;
}


template<typename T, int allocSize>
void QuickList<T, allocSize>::pop_front()
{
	assert( pFreeHead_ && pFreeRear_ && pFreeHead_!=pFreeRear_ );

	ValueType *&pHead = beg_.next;
	ValueType *pOldHead = pHead;

	//modify data link, pNewRear = pRear->prev
	assert(pHead!=&end_);
	pHead = pHead->next;
	pHead->next = &end_;

	//modify free link, add to rear
	pFreeRear_->next = pOldHead;

	pOldHead->prev = pFreeRear_;
	pOldHead->next = NULL;

	pFreeRear_ = pOldHead;

	--this->size_;

	return;

}


template<typename T, int allocSize>
inline T & QuickList<T, allocSize>::back()
{
	assert(end_.prev!=&beg_);
	return end_.prev->data;
}


template<typename T, int allocSize>
inline T & QuickList<T, allocSize>::front()
{
	assert(beg_.next!=&end_);
	return beg_.next->data;
}


template<typename T, int allocSize>
typename QuickList<T, allocSize>::Iterator QuickList<T, allocSize>::insert(Iterator it_where, T const &a)
{
	assert( pFreeHead_ && pFreeRear_ && pFreeHead_!=pFreeRear_ );

	ValueType *pWhere = it_where.ptr_;

	assert(pWhere && pWhere!=&beg_);		//pWhere be valid and not rend

	ValueType *pNewFreeHead = pFreeHead_->next;

	ValueType *pValue = pFreeHead_;
	pValue->data = a;

	//modify data link, pValue = pFreeHead_
	pWhere->prev->next = pValue;
	pValue->prev = pWhere->prev;
	pValue->next = pWhere;
	pWhere->prev = pValue;

	//modify free link, remove head
	pFreeHead_ = pNewFreeHead;
	pFreeHead_->prev = NULL;

	//if need new space
	if( pFreeHead_==pFreeRear_ )
	{
		this->alloc();
	}	

	++this->size_;

	return Iterator(pValue);
}


template<typename T, int allocSize>
typename QuickList<T, allocSize>::Iterator QuickList<T, allocSize>::erase(Iterator it)
{
	assert( pFreeHead_ && pFreeRear_ && pFreeHead_!=pFreeRear_ );

	ValueType *pValue = it.ptr_;

	assert(pValue && pValue!=&beg_&& pValue!=&end_);		//pValue be valid

	ValueType *pNextValue = pValue->next;

	//modify data link, pNewRear = pFreeHead_
	ValueType *pPrev = pValue->prev;
	ValueType *pNext = pValue->next;

	pPrev->next = pNext;
	pNext->prev = pPrev;

	//modify free link, add to rear
	pFreeRear_->next = pValue;

	pValue->prev = pFreeRear_;
	pValue->next = NULL;

	pFreeRear_ = pValue;

	--this->size_;

	return Iterator(pNextValue);
}


template<typename T, int allocSize>
inline typename QuickList<T, allocSize>::Iterator QuickList<T, allocSize>::begin()
{
	return Iterator(beg_.next);
}


template<typename T, int allocSize>
inline typename QuickList<T, allocSize>::Iterator QuickList<T, allocSize>::end()
{
	return Iterator(&end_);
}



template<typename T, int allocSize>
void QuickList<T, allocSize>::eraseAll()
{
	if( beg_.next==&end_ )		//empty
	{
		assert(end_.prev == &beg_);
		return;
	}
	
	//add data link to rear of free link

	//modify free link, add to rear
	ValueType *&pHead = beg_.next;
	ValueType *&pRear = end_.next;

	pFreeRear_->next = pHead;

	pHead->prev = pFreeRear_;
	pRear->next = NULL;

	pFreeRear_ = pRear;

	//clear data link
	pHead = &end_;
	pRear = &beg_;

	this->size_ = 0;

	return;
}


template<typename T, int allocSize>
void QuickList<T, allocSize>::clear()
{
	this->pFreeHead_ = NULL;
	this->pFreeRear_ = NULL;

	dataArrayList.clear();

	this->alloc();

	this->size_ = 0;

	return;
}


template<typename T, int allocSize>
inline size_t QuickList<T, allocSize>::size()
{
	return size_;
}


template<typename T, int allocSize>
void QuickList<T, allocSize>::alloc()
{
	dataArrayList.push_back( DataArray() );
	DataArray &dataArray = dataArrayList.back();

	//link new array
	dataArray.data[0].next = dataArray.data + 1;

	for(size_t i=1; i<ARRAY_SIZE-1; ++i)
	{
		dataArray.data[i].next = dataArray.data + i + 1;
		dataArray.data[i].prev = dataArray.data + i - 1;
	}

	dataArray.data[ARRAY_SIZE-1].prev = dataArray.data + ARRAY_SIZE-1 -1;

	//link free rear to new array
	if( pFreeRear_ )
	{
		pFreeRear_->next = & dataArray.data[0];

		dataArray.data[0].prev = pFreeRear_;
	}
	else		//empty
	{
		assert( this->pFreeHead_ == NULL );		//because pFreeRear_==NULL

		pFreeHead_ = & dataArray.data[0];
		pFreeHead_->prev = NULL;
	}

	//modify pFreeRear_
	pFreeRear_ = dataArray.data + ARRAY_SIZE-1;
	pFreeRear_->next = NULL;

	assert( pFreeHead_ && pFreeRear_ && pFreeHead_!=pFreeRear_ );

	return;
}


#endif
