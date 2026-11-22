/*
===============================================================================

	TGPoolManager.h

	The memory manager every game allocation goes through. The SWIG global
	g_kPoolManager (its getter returns 0x99C478) is the one instance, and its
	setter copies a single byte: the class has no data members and no
	virtual functions. Its state is static: the methods read and write fixed
	addresses (0x99C480..0x99C4AC) and never use their this pointer.

	Allocations of up to MAX_POOLED_SIZE bytes come from fixed-size pools, one
	per rounded size; larger ones go to the C runtime heap. Every allocation
	is preceded by a size_t holding its rounded size, which is how
	Unknown717960 finds the pool to return it to. The counters feed
	PrintPoolSummary's report, whose strings name them.

	TGPool and TGPoolBlock have no name evidence (no SWIG wrapper, no string
	names them): both names are invented, and so are the members' names,
	which follow the summary's wording.

===============================================================================
*/

#ifndef __TGPOOLMANAGER_H__
#define __TGPOOLMANAGER_H__

#include <stddef.h>

// Allocations above this size bypass the pools (cmp 0x80 in every path).
#define MAX_POOLED_SIZE		0x80

// The largest number of objects a pool adds in one block.
#define MAX_POOL_GROWTH		64

// Called for each object still in use; see TGPool::Unknown718260.
typedef void ( *TGPoolObjectCallback )( void *object, unsigned int objectSize, void *userData );

/*
===============================================================================

	TGPoolBlock

	One buffer of a pool: numObjects slots of objectSize bytes. 8 bytes.

===============================================================================
*/

class TGPoolBlock {
public:
	void			Init( unsigned short numObjects, unsigned short objectSize );
	void			Free( void );
	void *			GetSlot( int index );

	unsigned short	numObjects;		// 0x00
	unsigned short	objectSize;		// 0x02 bytes per slot, size header included
	char *			data;			// 0x04 from the C runtime heap
};

/*
===============================================================================

	TGPool

	The pool of one object size: its blocks and a free list threaded through
	the unused slots (each free slot's first word points at the next).
	0x24 bytes; TGPoolManager keeps them in an array sorted by objectSize.

===============================================================================
*/

class TGPool {
public:
	void			Init( unsigned int objectSize );
	void			Unknown717F50( void );
	void *			Allocate( const char *file, int line );
	void			Free( void *object );
	void			Unknown7181E0( void );
	void			Unknown718260( TGPoolObjectCallback callback, void *userData );

	unsigned int	objectSize;		// 0x00 slot size, size header included
	unsigned int	numUsed;		// 0x04 objects handed out
	unsigned int	usedBytes;		// 0x08
	unsigned int	numObjects;		// 0x0C slots in all blocks
	unsigned int	totalBytes;		// 0x10
	unsigned int	numBlocks;		// 0x14
	TGPoolBlock *	blocks;			// 0x18 from the C runtime heap
	void *			freeList;		// 0x1C
	unsigned short	growth;			// 0x20 objects in the first block
	unsigned short	maxGrowth;		// 0x22 objects in any later block, at most
};

/*
===============================================================================

	TGPoolManager

===============================================================================
*/

class TGPoolManager {
public:
					~TGPoolManager( void );

	// The allocation entry points have no name evidence yet. Game code
	// reaches them through the malloc and free macros in TGMemory.h.
	void *			Unknown717840( size_t size, const char *file, int line );
	void *			Unknown717920( size_t count, size_t size, const char *file, int line );
	void			Unknown717960( void *block );
	void *			Unknown7179C0( void *block, size_t size, const char *file, int line );

	void			Purge( void );
	void			WalkObjects( TGPoolObjectCallback callback, void *userData );
	void			PrintPoolSummary( void );
	void			Unknown717EA0( int, int );
	void			PrintFull( void );
	void			PrintSummary( void );
	void			DumpFile( char *fileName );
	void			DumpObjects( char *fileName );
	void			DumpBlocks( char *fileName );
	void			DisplayFile( char *fileName, bool );

	static unsigned int		numPools;
	static TGPool *			pools;				// sorted by objectSize
	static unsigned int		numUsedObjects;		// pooled objects handed out
	static unsigned int		usedObjectBytes;
	static unsigned int		numPooledObjects;	// slots in all pools
	static unsigned int		pooledBytes;
	static unsigned int		numBlocks;			// allocations above MAX_POOLED_SIZE
	static unsigned int		blockBytes;
	static unsigned int		bytesInUse;			// pools and blocks together
	static unsigned int		highWaterBytes;
	static unsigned int		lastHighWaterDump;
	static unsigned int		highWaterDumpStart;	// never set: 0 turns the dumps off
	static bool				isAlive;			// cleared by the destructor
	static unsigned int		highWaterDumpStep;
	static unsigned short	initialGrowth;

	// Classes with their own operator new and delete (TGObject, TGMessage and
	// the rest: about 970 sites) allocate from the pool of their size
	// directly, without a size header: AllocateObject( size, "UNKNOWN", 0 )
	// and FindPool( size )->Free( object ).
	TGPool *		FindPool( unsigned int objectSize );
	TGPool *		GetPool( unsigned int objectSize );

	// The pooled operator new's allocation. Where a function's inline budget
	// runs out, it is called out of line (the copy at 0x40F030, 30 callers).
	// The name is invented.
	// FUNCTION: STBC 0x40F030 ?AllocateObject@TGPoolManager@@QAEPAXIPBDH@Z @game/SetClass
	void *			AllocateObject( size_t size, const char *file, int line ) { return GetPool( size )->Allocate( file, line ); }

private:
	friend class	TGPool;

	static void		AddBytesInUse( unsigned int bytes );
};

extern TGPoolManager g_kPoolManager;

// The class-level allocation of pooled classes (TGMessage, TGObject and the
// rest: about 970 sites): objects come from the pool of their exact size,
// without a size header. The macro's name is invented.
#define TGPOOL_ALLOCATION \
	void *			operator new( size_t size ) { return g_kPoolManager.AllocateObject( size, TGMEMORY_FILE, TGMEMORY_LINE ); } \
	void			operator delete( void *object, size_t size ) { g_kPoolManager.FindPool( size )->Free( object ); }

#endif /* !__TGPOOLMANAGER_H__ */
