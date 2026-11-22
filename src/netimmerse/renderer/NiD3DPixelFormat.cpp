// NiD3DPixelFormat.cpp -- the table of pixel formats met (invented file name)
//
// Retail keeps these in a unit of their own after NiTextureManager's: the
// table's statics, its NiTArray copy, the locale pair and the <iostream> quads
// (0x7D7B70-0x7D7D20). NiTextureManager::VirtualSlot00 is the unit's only
// code, ahead of its statics.

#include "NiRTLib.h"
#include "NiTextureManager.h"
#include "NiD3DPixelFormat.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x007d7ca0 _$E43
// FUNCTION: STBC 0x007d7cc0 _$E17
// FUNCTION: STBC 0x007d7ce0 _$E15
// FUNCTION: STBC 0x007d7cf0 _$E23
// FUNCTION: STBC 0x007d7d10 _$E21

// built by this unit's static initializer (0x7D7BB0); another of its initialisers
// (0x7D7C20) registers the exit-time cleanup that deletes every entry
// FUNCTION: STBC 0x007d7bb0 _$E31
// FUNCTION: STBC 0x007d7bf0 _$E28
// DATA: STBC 0x9A1430 ?descs@NiDDPixelDescTable@@1V?$NiTArray@PBVNiDDPixelDesc@@@@A
NiTArray<const NiDDPixelDesc *> NiDDPixelDescTable::descs;

/*
================
NiDDPixelDescTableCleanup

Deletes the table's entries at exit; slot 0 is never used. The class has
only its destructor: retail's static initialiser 0x7D7C20 registers
0x7D7C30, the destructor expanded, with atexit, and nothing constructs it
(DamageTool.exe's copy of the unit has the same pair, _$E35 and _$E33,
after the table's own). The class and object names are invented.
================
*/
class NiDDPixelDescTableCleanup {
public:
	~NiDDPixelDescTableCleanup() {
		for ( unsigned int i = 1; i < NiDDPixelDescTable::descs.GetSize(); i++ ) {
			delete NiDDPixelDescTable::descs.GetAt( i );
		}
	}
};

// FUNCTION: STBC 0x007d7c20 _$E36
// FUNCTION: STBC 0x007d7c30 _$E34
static NiDDPixelDescTableCleanup cleanup;

/*
================
NiTextureManager::VirtualSlot00

Starts a new frame: clears the per-frame counters.
================
*/
// FUNCTION: STBC 0x7D7B70
void NiTextureManager::VirtualSlot00() {
	unknownA0 = 1;
	unknownA4++;
	unknown94 = 0;
	unknown98 = 0;
	unknown90 = 0;
	unknown9C = 0;
}
