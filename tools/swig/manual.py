# Hand-written header additions, merged into the generated class headers by gen.py.
MANUAL = {
    'TGPoolManager': {
        'note': [
            '\tThe memory manager every game allocation goes through. The SWIG global',
            '\tg_kPoolManager (its getter returns 0x99C478) is the one instance, and its',
            '\tsetter copies a single byte: the class has no data members and no',
            '\tvirtual functions; its counters are static members.',
        ],
        'members': [
            '\t// The two allocation entry points have no name evidence yet. Game code',
            '\t// reaches them through the malloc and free macros in TGMemory.h.',
            '\tvoid *\tUnknown717840( size_t size, const char *file, int line );',
            '\tvoid\tUnknown717960( void *block );',
        ],
        'pre': ['#include <stddef.h>', ''],
        'post': ['extern TGPoolManager g_kPoolManager;', ''],
    },
}
