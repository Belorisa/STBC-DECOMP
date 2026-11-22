/*
===============================================================================

	TGPoolManager.cpp

	The pool allocator behind every game allocation. This unit allocates its
	own buffers from the C runtime heap directly (the CRT malloc, realloc and
	free at 0x85B028, 0x85B536 and 0x85ADC4), so it undoes the game-wide
	macros of TGMemory.h.

	The TGMEMORY_DEBUG build kept the allocating file and line of every
	object and could dump them; in this build the dump functions are empty
	and DisplayFile says so.

===============================================================================
*/

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <windows.h>

#include "TGPoolManager.h"
#include "NiMain.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x00718480 _$E413
// FUNCTION: STBC 0x007184a0 _$E412

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00718340 _$E32
// FUNCTION: STBC 0x00718350 _$E29
// FUNCTION: STBC 0x00718360 _$E31
// FUNCTION: STBC 0x00718370 _$E30
// FUNCTION: STBC 0x00718380 _$E38
// FUNCTION: STBC 0x00718390 _$E35
// FUNCTION: STBC 0x007183a0 _$E37
// FUNCTION: STBC 0x007183b0 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x007183c0 _$E45
// FUNCTION: STBC 0x007183d0 _$E44
// FUNCTION: STBC 0x007183e0 _$E49
// FUNCTION: STBC 0x007183f0 _$E48
// FUNCTION: STBC 0x00718400 _$E60
// FUNCTION: STBC 0x00718410 _$E59
// FUNCTION: STBC 0x00718420 _$E64
// FUNCTION: STBC 0x00718430 _$E63
// FUNCTION: STBC 0x00718440 _$E68
// FUNCTION: STBC 0x00718450 _$E67
// FUNCTION: STBC 0x00718460 _$E72
// FUNCTION: STBC 0x00718470 _$E71
// FUNCTION: STBC 0x007184b0 _$E196
// FUNCTION: STBC 0x007184c0 _$E195
// FUNCTION: STBC 0x007184d0 _$E138
// FUNCTION: STBC 0x007184e0 _$E137
// FUNCTION: STBC 0x007184f0 _$E151
// FUNCTION: STBC 0x00718500 _$E150
// FUNCTION: STBC 0x00718510 _$E201
// FUNCTION: STBC 0x00718520 _$E200
// FUNCTION: STBC 0x00718530 _$E206
// FUNCTION: STBC 0x00718540 _$E205
// FUNCTION: STBC 0x00718550 _$E210
// FUNCTION: STBC 0x00718560 _$E209
// FUNCTION: STBC 0x00718570 _$E214
// FUNCTION: STBC 0x00718580 _$E213
// FUNCTION: STBC 0x00718590 _$E221
// FUNCTION: STBC 0x007185a0 _$E220
// FUNCTION: STBC 0x007185b0 _$E246
// FUNCTION: STBC 0x007185c0 _$E245
// FUNCTION: STBC 0x007185d0 _$E250
// FUNCTION: STBC 0x007185e0 _$E249
// FUNCTION: STBC 0x007185f0 _$E96
// FUNCTION: STBC 0x00718600 _$E95
// FUNCTION: STBC 0x00718610 _$E232
// FUNCTION: STBC 0x00718620 _$E231
// FUNCTION: STBC 0x00718630 _$E239
// FUNCTION: STBC 0x00718640 _$E238
// FUNCTION: STBC 0x00718650 _$E262
// FUNCTION: STBC 0x00718660 _$E261
// FUNCTION: STBC 0x00718670 _$E266
// FUNCTION: STBC 0x00718680 _$E265
// FUNCTION: STBC 0x00718690 _$E306
// FUNCTION: STBC 0x007186a0 _$E305
// FUNCTION: STBC 0x007186b0 _$E312
// FUNCTION: STBC 0x007186c0 _$E311
// FUNCTION: STBC 0x007186d0 _$E254
// FUNCTION: STBC 0x007186e0 _$E253
// FUNCTION: STBC 0x007186f0 _$E258
// FUNCTION: STBC 0x00718700 _$E257
// FUNCTION: STBC 0x00718710 _$E270
// FUNCTION: STBC 0x00718720 _$E269
// FUNCTION: STBC 0x00718730 _$E274
// FUNCTION: STBC 0x00718740 _$E273
// FUNCTION: STBC 0x00718750 _$E278
// FUNCTION: STBC 0x00718760 _$E277
// FUNCTION: STBC 0x00718770 _$E282
// FUNCTION: STBC 0x00718780 _$E281
// FUNCTION: STBC 0x00718790 _$E286
// FUNCTION: STBC 0x007187a0 _$E285
// FUNCTION: STBC 0x007187b0 _$E290
// FUNCTION: STBC 0x007187c0 _$E289
// FUNCTION: STBC 0x007187d0 _$E294
// FUNCTION: STBC 0x007187e0 _$E293
// FUNCTION: STBC 0x007187f0 _$E298
// FUNCTION: STBC 0x00718800 _$E297
// FUNCTION: STBC 0x00718810 _$E101
// FUNCTION: STBC 0x00718820 _$E100
// FUNCTION: STBC 0x00718830 _$E302
// FUNCTION: STBC 0x00718840 _$E301
// FUNCTION: STBC 0x00718850 _$E106
// FUNCTION: STBC 0x00718860 _$E105
// FUNCTION: STBC 0x00718870 _$E316
// FUNCTION: STBC 0x00718880 _$E315
// FUNCTION: STBC 0x00718890 _$E320
// FUNCTION: STBC 0x007188a0 _$E319
// FUNCTION: STBC 0x007188b0 _$E324
// FUNCTION: STBC 0x007188c0 _$E323
// FUNCTION: STBC 0x007188d0 _$E328
// FUNCTION: STBC 0x007188e0 _$E327
// FUNCTION: STBC 0x007188f0 _$E115
// FUNCTION: STBC 0x00718900 _$E114
// FUNCTION: STBC 0x00718910 _$E332
// FUNCTION: STBC 0x00718920 _$E331
// FUNCTION: STBC 0x00718930 _$E336
// FUNCTION: STBC 0x00718940 _$E335
// FUNCTION: STBC 0x00718950 _$E340
// FUNCTION: STBC 0x00718960 _$E339
// FUNCTION: STBC 0x00718970 _$E121
// FUNCTION: STBC 0x00718980 _$E120
// FUNCTION: STBC 0x00718990 _$E126
// FUNCTION: STBC 0x007189a0 _$E125
// FUNCTION: STBC 0x007189b0 _$E156
// FUNCTION: STBC 0x007189c0 _$E155
// FUNCTION: STBC 0x007189d0 _$E344
// FUNCTION: STBC 0x007189e0 _$E343
// FUNCTION: STBC 0x007189f0 _$E161
// FUNCTION: STBC 0x00718a00 _$E160
// FUNCTION: STBC 0x00718a10 _$E348
// FUNCTION: STBC 0x00718a20 _$E347
// FUNCTION: STBC 0x00718a30 _$E352
// FUNCTION: STBC 0x00718a40 _$E351
// FUNCTION: STBC 0x00718a50 _$E166
// FUNCTION: STBC 0x00718a60 _$E165
// FUNCTION: STBC 0x00718a70 _$E356
// FUNCTION: STBC 0x00718a80 _$E355
// FUNCTION: STBC 0x00718a90 _$E131
// FUNCTION: STBC 0x00718aa0 _$E130
// FUNCTION: STBC 0x00718ab0 _$E360
// FUNCTION: STBC 0x00718ac0 _$E359
// FUNCTION: STBC 0x00718ad0 _$E364
// FUNCTION: STBC 0x00718ae0 _$E363
// FUNCTION: STBC 0x00718af0 _$E171
// FUNCTION: STBC 0x00718b00 _$E170
// FUNCTION: STBC 0x00718b10 _$E176
// FUNCTION: STBC 0x00718b20 _$E175
// FUNCTION: STBC 0x00718b30 _$E368
// FUNCTION: STBC 0x00718b40 _$E367
// FUNCTION: STBC 0x00718b50 _$E372
// FUNCTION: STBC 0x00718b60 _$E371
// FUNCTION: STBC 0x00718b70 _$E376
// FUNCTION: STBC 0x00718b80 _$E375
// FUNCTION: STBC 0x00718b90 _$E380
// FUNCTION: STBC 0x00718ba0 _$E379
// FUNCTION: STBC 0x00718bb0 _$E384
// FUNCTION: STBC 0x00718bc0 _$E383
// FUNCTION: STBC 0x00718bd0 _$E388
// FUNCTION: STBC 0x00718be0 _$E387
// FUNCTION: STBC 0x00718bf0 _$E392
// FUNCTION: STBC 0x00718c00 _$E391
// FUNCTION: STBC 0x00718c10 _$E396
// FUNCTION: STBC 0x00718c20 _$E395
// FUNCTION: STBC 0x00718c30 _$E181
// FUNCTION: STBC 0x00718c40 _$E180
// FUNCTION: STBC 0x00718c50 _$E400
// FUNCTION: STBC 0x00718c60 _$E399
// FUNCTION: STBC 0x00718c70 _$E186
// FUNCTION: STBC 0x00718c80 _$E185
// FUNCTION: STBC 0x00718c90 _$E191
// FUNCTION: STBC 0x00718ca0 _$E190

// The pools' own buffers come from the C runtime heap.
#undef malloc
#undef realloc
#undef free

// The instance's construction (its constructor is empty) and its destruction at exit.
// FUNCTION: STBC 0x007177d0 _$E405
// FUNCTION: STBC 0x007177e0 _$E402
// FUNCTION: STBC 0x007177f0 _$E404
// FUNCTION: STBC 0x00717800 _$E403

// DATA: STBC 0x0099c478
TGPoolManager g_kPoolManager;

// DATA: STBC 0x0099c480
unsigned int TGPoolManager::numPools;
// DATA: STBC 0x0099c484
TGPool *TGPoolManager::pools;
// DATA: STBC 0x0099c488
unsigned int TGPoolManager::numUsedObjects;
// DATA: STBC 0x0099c48c
unsigned int TGPoolManager::usedObjectBytes;
// DATA: STBC 0x0099c490
unsigned int TGPoolManager::numPooledObjects;
// DATA: STBC 0x0099c494
unsigned int TGPoolManager::pooledBytes;
// DATA: STBC 0x0099c498
unsigned int TGPoolManager::numBlocks;
// DATA: STBC 0x0099c49c
unsigned int TGPoolManager::blockBytes;
// DATA: STBC 0x0099c4a0
unsigned int TGPoolManager::bytesInUse;
// DATA: STBC 0x0099c4a4
unsigned int TGPoolManager::highWaterBytes;
// DATA: STBC 0x0099c4a8
unsigned int TGPoolManager::lastHighWaterDump;
// DATA: STBC 0x0099c4ac
unsigned int TGPoolManager::highWaterDumpStart;
// DATA: STBC 0x0095c390
bool TGPoolManager::isAlive = true;
// DATA: STBC 0x0095c394
unsigned int TGPoolManager::highWaterDumpStep = 0xFFFFF;
// DATA: STBC 0x0095c398
unsigned short TGPoolManager::initialGrowth = 2;

/*
================
TGPoolBlock::GetSlot

The address of slot index.
================
*/
// FUNCTION: STBC 0x00717810
void *TGPoolBlock::GetSlot( int index ) {
	return data + objectSize * index;
}

/*
================
TGPoolManager::~TGPoolManager

From here on a pool is purged as soon as its last object is freed.
================
*/
// FUNCTION: STBC 0x00717830
TGPoolManager::~TGPoolManager( void ) {
	isAlive = false;
	Purge();
}

/*
================
TGPoolManager::AddBytesInUse

Counts bytes newly in use and, once the use passes highWaterDumpStart,
dumps the allocations at every new high water mark highWaterDumpStep
bytes above the last one.
================
*/
inline void TGPoolManager::AddBytesInUse( unsigned int bytes ) {
	bytesInUse += bytes;
	if ( highWaterBytes < bytesInUse ) {
		highWaterBytes = bytesInUse;
		if ( highWaterDumpStart != 0 && bytesInUse > highWaterDumpStart && bytesInUse - lastHighWaterDump > highWaterDumpStep ) {
			lastHighWaterDump = bytesInUse;
			g_kPoolManager.DumpFile( "HighWaterUsage.mem" );
		}
	}
}

/*
================
StoreSize

Writes an allocation's size header and returns the memory after it.
================
*/
static inline void *StoreSize( void *header, size_t size ) {
	*(size_t *)header = size;
	return (size_t *)header + 1;
}

/*
================
TGPoolManager::Unknown717840

Allocates size bytes: game code's malloc.
================
*/
// FUNCTION: STBC 0x00717840
void *TGPoolManager::Unknown717840( size_t size, const char *file, int line ) {
	if ( size == 0 ) {
		size = 1;
	}
	if ( size & 3 ) {
		size += 4 - ( size & 3 );
	}

	void *block;
	if ( size > MAX_POOLED_SIZE ) {
		block = StoreSize( malloc( size + sizeof( size_t ) ), size );
		numBlocks++;
		blockBytes += size;
		AddBytesInUse( size );
	} else {
		block = StoreSize( GetPool( size + sizeof( size_t ) )->Allocate( file, line ), size );
	}
	return block;
}

/*
================
TGPoolManager::Unknown717920

Allocates count zeroed elements of size bytes: game code's calloc.
================
*/
// FUNCTION: STBC 0x00717920
void *TGPoolManager::Unknown717920( size_t count, size_t size, const char *file, int line ) {
	size_t total = count * size;
	void *block = Unknown717840( total, file, line );
	if ( total != 0 ) {
		memset( block, 0, total );
	}
	return block;
}

/*
================
TGPoolManager::Unknown717960

Frees a block of Unknown717840: game code's free.
================
*/
// FUNCTION: STBC 0x00717960
void TGPoolManager::Unknown717960( void *block ) {
	if ( block != NULL ) {
		size_t *header = (size_t *)block - 1;
		size_t size = *header;
		if ( size > MAX_POOLED_SIZE ) {
			numBlocks--;
			blockBytes -= size;
			bytesInUse -= size;
			free( header );
			return;
		}
		FindPool( size + sizeof( size_t ) )->Free( header );
	}
}

/*
================
TGPoolManager::Unknown7179C0

Resizes a block of Unknown717840: game code's realloc. A pooled block
that shrinks by less than half, and by less than 32 bytes, stays where
it is.
================
*/
// FUNCTION: STBC 0x007179c0
void *TGPoolManager::Unknown7179C0( void *block, size_t size, const char *file, int line ) {
	if ( block == NULL && size == 0 ) {
		return NULL;
	}
	if ( size & 3 ) {
		size += 4 - ( size & 3 );
	}

	size_t oldSize = 0;
	if ( block != NULL ) {
		size_t *header = (size_t *)block - 1;
		oldSize = *header;
		if ( oldSize == size ) {
			return block;
		}
		if ( oldSize > MAX_POOLED_SIZE && size > MAX_POOLED_SIZE ) {
			header = (size_t *)realloc( header, size + sizeof( size_t ) );
			*header++ = size;
			size_t growth = size - oldSize;
			blockBytes += growth;
			AddBytesInUse( growth );
			return header;
		}
	}

	if ( oldSize <= MAX_POOLED_SIZE && size <= MAX_POOLED_SIZE && oldSize >= size && size >= oldSize / 2 && oldSize - size < 32 ) {
		return block;
	}

	void *newBlock = NULL;
	if ( size != 0 ) {
		newBlock = Unknown717840( size, file, line );
		if ( block != NULL ) {
			memcpy( newBlock, block, oldSize < size ? oldSize : size );
		}
	}
	if ( block != NULL ) {
		Unknown717960( block );
	}
	return newBlock;
}

/*
================
TGPoolManager::FindPool

The pool of objectSize, by binary search, or NULL.
================
*/
// FUNCTION: STBC 0x00717b20
TGPool *TGPoolManager::FindPool( unsigned int objectSize ) {
	int low = 0;
	int high = numPools - 1;
	while ( high >= low ) {
		int mid = ( low + high ) >> 1;
		if ( objectSize == pools[mid].objectSize ) {
			return &pools[mid];
		}
		if ( objectSize < pools[mid].objectSize ) {
			high = mid - 1;
		} else {
			low = mid + 1;
		}
	}
	return NULL;
}

/*
================
TGPoolManager::GetPool

The pool of objectSize; a new one is inserted in order when there is none.
================
*/
// FUNCTION: STBC 0x00717b70
TGPool *TGPoolManager::GetPool( unsigned int objectSize ) {
	int low = 0;
	int high = numPools - 1;
	while ( high >= low ) {
		int mid = ( low + high ) >> 1;
		if ( objectSize == pools[mid].objectSize ) {
			return &pools[mid];
		}
		if ( objectSize < pools[mid].objectSize ) {
			high = mid - 1;
		} else {
			low = mid + 1;
		}
	}

	pools = (TGPool *)realloc( pools, ( numPools + 1 ) * sizeof( TGPool ) );
	if ( (int)numPools > low ) {
		memmove( &pools[low + 1], &pools[low], ( numPools - low ) * sizeof( TGPool ) );
	}
	numPools++;
	pools[low].Init( objectSize );
	return &pools[low];
}

/*
================
TGPoolManager::Purge

Releases the pools that have no object in use.
================
*/
// FUNCTION: STBC 0x00717c30
void TGPoolManager::Purge( void ) {
	int count = numPools;
	for ( int i = numPools - 1; i >= 0; i-- ) {
		if ( pools[i].numUsed == 0 ) {
			pools[i].Unknown717F50();
			count--;
			if ( count > i ) {
				memmove( &pools[i], &pools[i + 1], ( count - i ) * sizeof( TGPool ) );
			}
		} else {
			pools[i].Unknown7181E0();
		}
	}

	if ( count == 0 ) {
		if ( pools != NULL ) {
			free( pools );
			pools = NULL;
		}
		numPools = 0;
	} else if ( count < numPools ) {
		// BUG: resizes the array to its old size, not to count pools.
		pools = (TGPool *)realloc( pools, numPools * sizeof( TGPool ) );
		numPools = count;
	}
}

/*
================
TGPoolManager::WalkObjects

Calls callback for every pooled object in use. Only the SWIG wrapper
TGPoolManager_WalkObjects calls it.
================
*/
// FUNCTION: STBC 0x00717cf0
void TGPoolManager::WalkObjects( TGPoolObjectCallback callback, void *userData ) {
	for ( unsigned int i = 0; i < numPools; i++ ) {
		pools[i].Unknown718260( callback, userData );
	}
}

/*
================
TGPoolManager::PrintPoolSummary

Writes the counters and one line per pool to the debugger.
================
*/
// FUNCTION: STBC 0x00717d30
void TGPoolManager::PrintPoolSummary( void ) {
	char buffer[256];

	sprintf( buffer, "\n**************************** TGPoolManager summary ****************************\nTGPoolManager: %d Pool%s of objects still in existence\n",
		numPools, numPools == 1 ? "" : "s" );
	OutputDebugString( buffer );
	sprintf( buffer, "\t\t%d used object%s\t\t%d used object bytes\t",
		numUsedObjects, numUsedObjects == 1 ? "" : "s", usedObjectBytes );
	OutputDebugString( buffer );
	sprintf( buffer, "(%d unused object%s, %d bytes in remaining buffers)\n",
		numPooledObjects, numPooledObjects == 1 ? "" : "s", pooledBytes );
	OutputDebugString( buffer );
	sprintf( buffer, "\t\t%d block%s\t\t\t%d block bytes\n",
		numBlocks, numBlocks == 1 ? "" : "s", blockBytes );
	OutputDebugString( buffer );
	sprintf( buffer, "\t\t%d total bytes leaked\n\n", bytesInUse );
	OutputDebugString( buffer );

	for ( unsigned int i = 0; i < numPools; i++ ) {
		sprintf( buffer, "\tPool of object size %d:\n\t\t%d total objects for %d total bytes.  %d remaining in use for %d used bytes\n",
			pools[i].objectSize, pools[i].numObjects, pools[i].totalBytes, pools[i].numUsed, pools[i].usedBytes );
		OutputDebugString( buffer );
	}
	OutputDebugString( "*******************************************************************************\n" );
}

/*
================
TGPoolManager::Unknown717EA0

Empty in this build; nothing calls it.
================
*/
// FUNCTION: STBC 0x00717ea0
void TGPoolManager::Unknown717EA0( int, int ) {
}

/*
================
TGPoolManager::PrintFull

Empty without TGMEMORY_DEBUG.
================
*/
// FUNCTION: STBC 0x00717eb0
void TGPoolManager::PrintFull( void ) {
}

/*
================
TGPoolManager::PrintSummary

Empty without TGMEMORY_DEBUG.
================
*/
// FUNCTION: STBC 0x00717ec0
void TGPoolManager::PrintSummary( void ) {
}

/*
================
TGPoolManager::DumpFile

Empty without TGMEMORY_DEBUG.
================
*/
// FUNCTION: STBC 0x00717ed0
void TGPoolManager::DumpFile( char *fileName ) {
}

/*
================
TGPoolManager::DumpObjects

Empty without TGMEMORY_DEBUG.
================
*/
// FUNCTION: STBC 0x00717ee0
void TGPoolManager::DumpObjects( char *fileName ) {
}

/*
================
TGPoolManager::DumpBlocks

Empty without TGMEMORY_DEBUG.
================
*/
// FUNCTION: STBC 0x00717ef0
void TGPoolManager::DumpBlocks( char *fileName ) {
}

/*
================
TGPoolManager::DisplayFile
================
*/
// FUNCTION: STBC 0x00717f00
void TGPoolManager::DisplayFile( char *fileName, bool ) {
	OutputDebugString( "Define TGMEMORY_DEBUG if you want to see mem-info.\n" );
}

/*
================
TGPool::Init
================
*/
// FUNCTION: STBC 0x00717f10
void TGPool::Init( unsigned int objectSize ) {
	this->objectSize = objectSize;
	numObjects = 0;
	totalBytes = 0;
	numUsed = 0;
	usedBytes = 0;
	numBlocks = 0;
	blocks = NULL;
	freeList = NULL;
	growth = TGPoolManager::initialGrowth;
	maxGrowth = MAX_POOL_GROWTH;
}

/*
================
TGPool::Unknown717F50

Releases every block and the block array. totalBytes is left as it was.
================
*/
// FUNCTION: STBC 0x00717f50
void TGPool::Unknown717F50( void ) {
	if ( numBlocks != 0 ) {
		for ( unsigned int i = 0; i < numBlocks; i++ ) {
			unsigned int count = blocks[i].numObjects;
			numObjects -= count;
			unsigned int bytes = objectSize * count;
			TGPoolManager::numPooledObjects -= count;
			TGPoolManager::pooledBytes -= bytes;
			TGPoolManager::bytesInUse -= bytes;
			blocks[i].Free();
		}
		free( blocks );
	}
}

/*
================
TGPoolBlock::Init
================
*/
// FUNCTION: STBC 0x00717fd0
void TGPoolBlock::Init( unsigned short numObjects, unsigned short objectSize ) {
	this->numObjects = numObjects;
	this->objectSize = objectSize;
	data = (char *)malloc( numObjects * objectSize );
}

/*
================
TGPoolBlock::Free
================
*/
// FUNCTION: STBC 0x00718000
void TGPoolBlock::Free( void ) {
	if ( data != NULL ) {
		free( data );
	}
}

/*
================
TGPool::Allocate

Takes a slot off the free list. An empty pool grows by a block holding as
many objects as are in use (growth for the first block), at most
maxGrowth.
================
*/
// FUNCTION: STBC 0x00718010
void *TGPool::Allocate( const char *file, int line ) {
	if ( freeList == NULL ) {
		unsigned int count = numUsed;
		if ( count == 0 ) {
			count = growth;
		}
		if ( count > maxGrowth ) {
			count = maxGrowth;
		}

		numBlocks++;
		blocks = (TGPoolBlock *)realloc( blocks, numBlocks * sizeof( TGPoolBlock ) );
		blocks[numBlocks - 1].Init( count, objectSize );
		for ( int i = count - 1; i >= 0; i-- ) {
			Free( blocks[numBlocks - 1].GetSlot( i ) );
		}

		// Free counted the new slots as released; take them back.
		numUsed += count;
		usedBytes += objectSize * count;
		TGPoolManager::numUsedObjects += count;
		TGPoolManager::usedObjectBytes += objectSize * count;

		unsigned int bytes = objectSize * count;
		numObjects += count;
		totalBytes += bytes;
		TGPoolManager::numPooledObjects += count;
		TGPoolManager::pooledBytes += bytes;
		TGPoolManager::AddBytesInUse( bytes );
	}

	void *object = freeList;
	freeList = *(void **)object;
	numUsed++;
	usedBytes += objectSize;
	TGPoolManager::numUsedObjects++;
	TGPoolManager::usedObjectBytes += objectSize;
	return object;
}

/*
================
TGPool::Free

Returns a slot to the free list. Once the pool manager is gone, an empty
pool is purged at once.
================
*/
// FUNCTION: STBC 0x00718180
void TGPool::Free( void *object ) {
	*(void **)object = freeList;
	freeList = object;
	numUsed--;
	usedBytes -= objectSize;
	TGPoolManager::numUsedObjects--;
	TGPoolManager::usedObjectBytes -= objectSize;
	if ( numUsed == 0 && !TGPoolManager::isAlive ) {
		g_kPoolManager.Purge();
	}
}

/*
================
TGPool::Unknown7181E0

Releases the blocks' buffers of a pool with no object in use. Purge only
calls it for pools that have objects in use, so it never does anything.
================
*/
// FUNCTION: STBC 0x007181e0
void TGPool::Unknown7181E0( void ) {
	if ( numUsed == 0 ) {
		for ( unsigned int i = 0; i < numBlocks; i++ ) {
			unsigned int count = blocks[i].numObjects;
			numObjects -= count;
			unsigned int bytes = objectSize * count;
			TGPoolManager::numPooledObjects -= count;
			TGPoolManager::pooledBytes -= bytes;
			TGPoolManager::bytesInUse -= bytes;
			blocks[i].Free();
		}
		freeList = NULL;
	}
}

/*
================
TGPool::Unknown718260

Calls callback for each slot in use, that is, each slot not on the free
list. Found free slots are moved out of the part of the list still to
be searched.
================
*/
// FUNCTION: STBC 0x00718260
void TGPool::Unknown718260( TGPoolObjectCallback callback, void *userData ) {
	if ( numUsed == 0 ) {
		return;
	}

	unsigned int remaining = numUsed;
	void **head = (void **)freeList;
	void **tail = head;
	for ( unsigned int b = 0; b < numBlocks; b++ ) {
		for ( unsigned int i = 0; i < blocks[b].numObjects; i++ ) {
			void **prev = tail;
			void **node = head;
			void *object = blocks[b].GetSlot( i );
			for ( ; node != NULL; prev = node, node = (void **)*node ) {
				if ( node == object ) {
					if ( head == node ) {
						tail = head;
						head = (void **)*node;
					} else {
						*prev = *node;
						*node = freeList;
						freeList = node;
					}
					break;
				}
			}
			if ( node == NULL ) {
				callback( object, objectSize, userData );
				if ( --remaining == 0 ) {
					return;
				}
			}
		}
	}
}
