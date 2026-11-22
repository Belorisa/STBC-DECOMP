/*
===============================================================================

	NiSortAdjustNode.h

	A node that changes how its subtree is sorted: it inherits the
	renderer's sorter, draws the subtree unsorted, or sorts it with its own
	accumulator.

	Evidence: the NiRTTI (0x9A2370, built at 0x805A10) is named
	"NiSortAdjustNode" with NiNode's as its base; the vtable at 0x89A01C has
	NiNode's 44 slots and adds none. The creation function (0x805E40)
	allocates 0xF0 bytes; the constructor, inlined there, clears the mode at
	0xE8 and the smart pointer at 0xEC. Display (0x805BA0) switches the
	camera's renderer's sorter on the mode, 1 dropping it and 2 putting the
	node's accumulator in its place; the modes and member names follow the
	Gamebryo lineage, which keeps the class.

===============================================================================
*/

#ifndef __NISORTADJUSTNODE_H__
#define __NISORTADJUSTNODE_H__

#include "NiNode.h"
#include "NiAccumulator.h"
#include "NiStreamMacros.h"

/*
================
NiSortAdjustNodeLoadRecord

The accumulator's link ID, read by LoadBinary (0x805FE0) and resolved by
LinkObject (0x806070). The class name is invented.

Evidence: CreateObject (0x805E40) allocates 0x5C bytes and stores the vptr
0x89A0CC after NiNode's record.
================
*/
// FUNCTION: STBC 0x805F80 ??_GNiSortAdjustNodeLoadRecord@@UAEPAXI@Z
// FUNCTION: STBC 0x805FA0 ??1NiSortAdjustNodeLoadRecord@@UAE@XZ
class NiSortAdjustNodeLoadRecord : public NiNodeLoadRecord {
public:
	unsigned int			accumulatorLinkID;
};

// FUNCTION: STBC 0x806320 ??_GNiSortAdjustNode@@UAEPAXI@Z
class NiSortAdjustNode : public NiNode {
public:
	// FUNCTION: STBC 0x806310 ?GetRTTI@NiSortAdjustNode@@UBEPBVNiRTTI@@XZ
	virtual const NiRTTI *	GetRTTI() const { return &rtti; }

	// cloning
	virtual NiObject *		CreateClone();

	// streaming
	virtual void			LoadBinary( NiStream &stream );
	virtual void			LinkObject( NiStream &stream );
	virtual bool			RegisterStreamables( NiStream &stream );
	virtual void			SaveBinary( NiStream &stream );
	virtual bool			IsEqual( NiObject *object );

	virtual void			Display( NiCamera *camera );

	// FUNCTION: STBC 0x806350 ??1NiSortAdjustNode@@UAE@XZ
	virtual					~NiSortAdjustNode() {}

	// how the subtree is sorted
	enum SortingMode {
		SORTING_INHERIT,	// with the renderer's sorter
		SORTING_OFF,		// unsorted
		SORTING_SUBSORT		// with the node's own accumulator
	};

							NiSortAdjustNode() { sortingMode = SORTING_INHERIT; accumulator = 0; }

	void					SetSortingMode( SortingMode mode ) { sortingMode = mode; }
	SortingMode				GetSortingMode() const { return sortingMode; }
	void					SetAccumulator( NiAccumulator *accumulator_ ) { accumulator = accumulator_; }
	NiAccumulator *			GetAccumulator() const { return accumulator; }

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

protected:
	void					CopyMembers( NiSortAdjustNode *dest );

	SortingMode				sortingMode;
	NiPointer<NiAccumulator>	accumulator;	// the sorter of SORTING_SUBSORT
};

NiRegisterStream( NiSortAdjustNode );

#endif /* !__NISORTADJUSTNODE_H__ */
