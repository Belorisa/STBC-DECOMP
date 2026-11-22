/* 
 *
 * File: darray.c
 * ---------------
 *	David Wright
 *	10/8/98
 * 
 *  See darray.h for function descriptions
 *
 *  Retail's copy is an older SDK version than IGN's 2011 release (the
 *  chosen source): it has no asserts, no ArrayGetDataPtr, ArraySetDataPtr,
 *  ArrayMapBackwards, ArrayMapBackwards2 or ArrayClear; ArrayNew allocates
 *  the list whatever the capacity, ArrayNth has no range check, and
 *  ArrayAppend and ArraySearch don't test for a null array.
 */
#include <stdlib.h>
#include <string.h>
#include "darray.h" 

#ifdef _MFC_MEM_DEBUG
#define _CRTDBG_MAP_ALLOC 1
#include <crtdbg.h>
#endif


#define DEF_GROWBY 8



// STRUCTURES
struct DArrayImplementation
{
	int count, capacity;
	int elemsize;
	int growby;
	ArrayElementFreeFn elemfreefn;
	void *list; //array of elements
};

// PROTOTYPES
static void *mylsearch(const void *key, void *base, int count, int size,
					 ArrayCompareFn comparator);
static void *mybsearch(const void *elem, void *base, int num, int elemsize, 
					   ArrayCompareFn comparator, int *found);
// FUNCTIONS

/* FreeElement
 * Frees the element at position N in the array
 */
// FUNCTION: STBC 0x6AD210
static void FreeElement(DArray array, int n)
{
	if (array->elemfreefn != NULL)
		array->elemfreefn(ArrayNth(array,n));
}

/* ArrayGrow
 * Reallocates the array to a new size, incresed by growby
 */
// FUNCTION: STBC 0x6AD2E0
static void ArrayGrow(DArray array)
{
	array->capacity +=  array->growby;
	array->list = realloc(array->list, (size_t) array->capacity * array->elemsize);
}

/* SetElement
 * Sets the element at pos to the contents of elem
 */
// FUNCTION: STBC 0x6AD310
static void SetElement(DArray array, const void *elem, int pos)
{

	memcpy(ArrayNth(array,pos), elem, (size_t)array->elemsize);
}

// FUNCTION: STBC 0x6AD180
DArray ArrayNew(int elemSize, int numElemsToAllocate, 
                ArrayElementFreeFn elemFreeFn)
{
	DArray array;

	array = (DArray) malloc(sizeof(struct DArrayImplementation));
	if (numElemsToAllocate == 0)
		numElemsToAllocate = DEF_GROWBY;
	array->count	= 0;
	array->capacity = numElemsToAllocate;;
	array->elemsize = elemSize;
	array->growby	= numElemsToAllocate;
	array->elemfreefn = elemFreeFn;
	array->list = malloc((size_t)array->capacity * array->elemsize);

	return array;
}

// FUNCTION: STBC 0x6AD1D0
void ArrayFree(DArray array)
{
	int i;
	
	for (i = 0; i < array->count; i++)
	{
		FreeElement(array, i);
	}
	free(array->list);
	free(array);
}


// FUNCTION: STBC 0x6AD230
int ArrayLength(const DArray array)
{
	return array->count;
}

// FUNCTION: STBC 0x6AD240
void *ArrayNth(DArray array, int n)
{
	return (char *)array->list + array->elemsize*n;
}

/* ArrayAppend
 * Just do an Insert at the end of the array
 */
// FUNCTION: STBC 0x6AD250
void ArrayAppend(DArray array, const void *newElem)
{
	ArrayInsertAt(array, newElem, array->count);
}

// FUNCTION: STBC 0x6AD270
void ArrayInsertAt(DArray array, const void *newElem, int n)
{

	if (array->count == array->capacity)
		ArrayGrow(array);
	array->count++;
	if (n < array->count - 1) //if we aren't appending
		memmove(ArrayNth(array, n+1), ArrayNth(array,n), 
				(size_t)(array->count - 1 - n) * array->elemsize);
	SetElement(array, newElem, n);
}

// FUNCTION: STBC 0x6AD350
void ArrayInsertSorted(DArray array, const void *newElem, ArrayCompareFn comparator)
{
	int n;
	void *res;
	int found;


	res=mybsearch(newElem, array->list,	array->count, array->elemsize, comparator, &found);
	n = (((char *)res - (char *)array->list) / array->elemsize);
	ArrayInsertAt(array, newElem, n);
}


// FUNCTION: STBC 0x6AD390
void ArrayRemoveAt(DArray array, int n)
{

	if (n < array->count - 1) //if not last element
		memmove(ArrayNth(array,n),ArrayNth(array,n+1),
			    (size_t)(array->count - 1 - n) * array->elemsize);
	array->count--;
}

// FUNCTION: STBC 0x6AD3E0
void ArrayDeleteAt(DArray array, int n)
{

	FreeElement(array,n);
	ArrayRemoveAt(array, n);
}


// FUNCTION: STBC 0x6AD400
void ArrayReplaceAt(DArray array, const void *newElem, int n)
{

	FreeElement(array, n);
	SetElement(array, newElem,n);
}


// FUNCTION: STBC 0x6AD430
void ArraySort(DArray array, ArrayCompareFn comparator)
{
	qsort(array->list, (size_t)array->count, (size_t)array->elemsize, comparator);
}

// FUNCTION: STBC 0x6AD450
int ArraySearch(DArray array, const void *key, ArrayCompareFn comparator, 
                  int fromIndex, int isSorted)
{
	void *res;
	int found = 1;
	if (array->count == 0)
		return NOT_FOUND;

   	if (isSorted)
		res=mybsearch(key, ArrayNth(array,fromIndex),
					array->count - fromIndex, array->elemsize, comparator, &found);
	else
		res=mylsearch(key, ArrayNth(array, fromIndex), 
					  array->count - fromIndex, array->elemsize, comparator);
	if (res != NULL && found)
		return (((char *)res - (char *)array->list) / array->elemsize);
	else
		return NOT_FOUND;
}


// FUNCTION: STBC 0x6AD4E0
void ArrayMap(DArray array, ArrayMapFn fn, void *clientData)
{
	int i;


	for (i = 0; i < array->count; i++)
		fn(ArrayNth(array,i), clientData);
		
}

// FUNCTION: STBC 0x6AD520
void * ArrayMap2(DArray array, ArrayMapFn2 fn, void *clientData)
{
	int i;
	void * pcurr;


	for (i = 0; i < array->count; i++)
	{
		pcurr = ArrayNth(array,i);
		if(!fn(pcurr, clientData))
			return pcurr;
	}

	return NULL;
}

/* mylsearch
 * Implementation of a standard linear search on an array, since we
 * couldn't use lfind
 */
// FUNCTION: STBC 0x6AD560
static void *mylsearch(const void *key, void *base, int count, int size,
					 ArrayCompareFn comparator)
{
	int i;
	for (i = 0; i < count; i++)
	{
		if (comparator(key, (char *)base + size*i) == 0)
			return (char *)base + size*i;
	}
	return NULL;
}

/* mybsearch
 * Implementation of a bsearch, since its not available on all platforms
 */
// FUNCTION: STBC 0x6AD5B0
static void *mybsearch(const void *elem, void *base, int num, int elemsize, ArrayCompareFn comparator, int *found)
{
	int L, H, I, C;
	

	L = 0;
	H = num - 1;
	*found = 0;
	while (L <= H)
	{
		I = (L + H) >> 1;
		C = comparator(((char *)base) + I * elemsize,elem);
		if (C == 0)
			*found = 1;
		if (C < 0) 
			L = I + 1;
		else
		{
			H = I - 1;
		}
	}
	return ((char *)base) + L * elemsize;
}
