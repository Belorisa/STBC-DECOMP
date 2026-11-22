/* 
 *
 * File: hashtable.c
 * ---------------
 *	David Wright
 *	10/8/98
 *
 * See hashtable.h for function comments
 * Implmentation is straight-forward, using a fixed dynamically allocated
 * array for the buckets, and a DArray for each individual bucket
 *
 * Retail's copy is an older SDK version than IGN's 2011 release (the chosen
 * source): it has no TableMapSafe, TableMapSafe2 or TableClear, and no
 * function tests for a null table.
 */

#include <stdlib.h>
#include <string.h>
#include "darray.h"
#include "hashtable.h"

#ifdef _MFC_MEM_DEBUG
#define _CRTDBG_MAP_ALLOC 1
#include <crtdbg.h>
#endif


#include <assert.h>


struct HashImplementation 
{
	DArray *buckets;
	int nbuckets;
	TableElementFreeFn freefn;
	TableHashFn hashfn;
	TableCompareFn compfn;
};

// FUNCTION: STBC 0x6ACB00
HashTable TableNew(int elemSize, int nBuckets, 
                   TableHashFn hashFn, TableCompareFn compFn, 
 					 TableElementFreeFn freeFn)
{
	return TableNew2(elemSize, nBuckets, 4, hashFn, compFn, freeFn);
}
// FUNCTION: STBC 0x6ACB30
HashTable TableNew2(int elemSize, int nBuckets, int nChains,
                   TableHashFn hashFn, TableCompareFn compFn, 
 					 TableElementFreeFn freeFn)
{
	HashTable table;
	int i;

	assert(hashFn);
	assert(compFn);
	assert(elemSize);
	assert(nBuckets);

	table = (HashTable)malloc(sizeof(struct HashImplementation));
	assert(table);
	
	table->buckets = (DArray *)malloc(nBuckets * sizeof(DArray));
	assert(table->buckets);
	for (i = 0; i < nBuckets; i++) //ArrayNew will assert if allocation fails
		table->buckets[i] = ArrayNew(elemSize, nChains, freeFn);
	table->nbuckets = nBuckets;
	table->freefn = freeFn;
	table->compfn = compFn;
	table->hashfn = hashFn;

	return table;
}


// FUNCTION: STBC 0x6ACBA0
void TableFree(HashTable table)
{
	int i;
	
	assert(table);

	for (i = 0 ; i < table->nbuckets ; i++)
		ArrayFree(table->buckets[i]);
	free(table->buckets);
	free(table);
}


// FUNCTION: STBC 0x6ACBE0
int TableCount(HashTable table)
{
	int i, count = 0;
	
	assert(table);

	for (i = 0 ; i < table->nbuckets ; i++)
		count += ArrayLength(table->buckets[i]);
	
	return count;
}


// FUNCTION: STBC 0x6ACC10
void TableEnter(HashTable table, const void *newElem)
{
	int hash, itempos;
	
	assert(table);

	hash = table->hashfn(newElem, table->nbuckets);
	itempos = ArraySearch(table->buckets[hash], newElem, table->compfn, 0,0);
	if (itempos == NOT_FOUND)
		ArrayAppend(table->buckets[hash], newElem);
	else
		ArrayReplaceAt(table->buckets[hash], newElem, itempos);
}

// FUNCTION: STBC 0x6ACC70
int TableRemove(HashTable table, const void *delElem)
{
	int hash, itempos;
	
	assert(table);

	hash = table->hashfn(delElem, table->nbuckets);
	itempos = ArraySearch(table->buckets[hash], delElem, table->compfn, 0,0);
	if (itempos == NOT_FOUND)
		return 0;
	else
		ArrayDeleteAt(table->buckets[hash], itempos);
	return 1;
}

// FUNCTION: STBC 0x6ACCC0
void *TableLookup(HashTable table, const void *elemKey)
{
	int hash, itempos;
	
	assert(table);

	hash = table->hashfn(elemKey, table->nbuckets);
	itempos = ArraySearch(table->buckets[hash], elemKey, table->compfn, 0,
						  0);
	if (itempos == NOT_FOUND)
		return NULL;
	else
		return ArrayNth(table->buckets[hash], itempos);
}


// FUNCTION: STBC 0x6ACD10
void TableMap(HashTable table, TableMapFn fn, void *clientData)
{
	int i;
	
	assert(table);
	assert(fn);

	for (i = 0 ; i < table->nbuckets ; i++)
		ArrayMap(table->buckets[i], fn, clientData);
	
}

// FUNCTION: STBC 0x6ACD50
void * TableMap2(HashTable table, TableMapFn2 fn, void *clientData)
{
	int i;
	void * pcurr;
	
	assert(fn);
	
	for (i = 0 ; i < table->nbuckets ; i++)
	{
		pcurr = ArrayMap2(table->buckets[i], fn, clientData);
		if(pcurr)
			return pcurr;
	}

	return NULL;
}
