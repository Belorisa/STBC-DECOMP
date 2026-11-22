/*
===============================================================================

	NiTSet.h

	A growable set of plain elements: a heap block, its size, the used
	count and a grow-by step, behind a vtable whose one slot is the scalar
	deleting destructor. The name follows the Gamebryo lineage, whose NiTSet
	keeps the same block, size and used count; this older version adds the
	vtable and the grow-by step.

	Evidence: NiBezierPatch's buffer allocation (0x832C30) builds 0x14-byte
	sets of vertex positions (vtable 0x89A95C), colors (0x89A954) and
	triangle indices (0x89A890), storing the size at 0x08, the grow-by step
	2 at 0x10, a zero used count at 0x0C and then a heap block of size
	elements at 0x04; NiBezierMesh's LoadBinary (0x831730) builds the
	index set for its edges. The destructors (0x834BA0, 0x834BD0, 0x831FC0)
	free the block.

===============================================================================
*/

#ifndef __NITSET_H__
#define __NITSET_H__

// FUNCTION: STBC 0x834BA0 ??_G?$NiTSet@VNiPoint3@@@@UAEPAXI@Z @netimmerse/NiBezierPatch
// FUNCTION: STBC 0x834BD0 ??_G?$NiTSet@VNiColorA@@@@UAEPAXI@Z @netimmerse/NiBezierPatch
// FUNCTION: STBC 0x831FC0 ??_G?$NiTSet@G@@UAEPAXI@Z @netimmerse/NiBezierMesh
template <class T> class NiTSet {
public:
					NiTSet( unsigned int maxSize_, unsigned int growBy_ );
	virtual			~NiTSet() { delete [] base; }

	unsigned int	GetSize() const { return size; }
	unsigned int	GetMaxSize() const { return maxSize; }
	// FUNCTION: STBC 0x8541A0 ?GetAt@?$NiTSet@VNiPoint3@@@@QBE?AVNiPoint3@@I@Z @netimmerse/NiBezierPatchEvaluate
	// FUNCTION: STBC 0x83A550 ?GetAt@?$NiTSet@VNiColorA@@@@QBE?AVNiColorA@@I@Z @netimmerse/NiBezierTriangle2
	T				GetAt( unsigned int index ) const { return base[index]; }
	T *				GetBase() const { return base; }
	unsigned int	Add( T item );	// returns the element's index
	void			RemoveAll() { size = 0; }
	void			Realloc( unsigned int newMaxSize );

protected:
	T *				base;		// heap block of maxSize elements
	unsigned int	maxSize;	// allocated elements
	unsigned int	size;		// used elements
	unsigned int	growBy;		// elements added when full
};

/*
================
NiTSet::NiTSet
================
*/
template <class T> inline NiTSet<T>::NiTSet( unsigned int maxSize_, unsigned int growBy_ ) {
	maxSize = maxSize_;
	growBy = growBy_;
	size = 0;
	base = new T[maxSize];
}

/*
================
NiTSet::Realloc

Moves the elements into a block of newMaxSize, unless that is the current
size or would not hold them (Gamebryo lineage name).
================
*/
// (retail keeps the NiBezierCylinder unit's copies of the NiPoint3 and
// NiColorA instances; NiBezierPatch's evaluators emit the same bodies)
// FUNCTION: STBC 0x850F70 ?Realloc@?$NiTSet@VNiPoint3@@@@QAEXI@Z @netimmerse/NiBezierCylinder
// FUNCTION: STBC 0x850FE0 ?Realloc@?$NiTSet@VNiColorA@@@@QAEXI@Z @netimmerse/NiBezierCylinder
// FUNCTION: STBC 0x838CC0 ?Realloc@?$NiTSet@G@@QAEXI@Z @netimmerse/NiBezierTriangle
template <class T> inline void NiTSet<T>::Realloc( unsigned int newMaxSize ) {
	// the size test nested, as Gamebryo 1.1.2's (whose inner test is an
	// assert); the nesting sets the inline cost that NiBezierCylinder's and
	// NiBezierRectangle's VirtualSlot44 expand it by
	if ( newMaxSize != maxSize ) {
		if ( newMaxSize >= size ) {
			T *oldBase = base;
			maxSize = newMaxSize;
			base = new T[maxSize];
			memcpy( base, oldBase, size * sizeof( T ) );
			delete [] oldBase;
		}
	}
}

/*
================
NiTSet::Add

Appends the element, growing the block first when it is full, and
returns its index.
================
*/
// FUNCTION: STBC 0x850DC0 ?Add@?$NiTSet@VNiPoint3@@@@QAEIVNiPoint3@@@Z @netimmerse/NiBezierCylinder
// FUNCTION: STBC 0x850E20 ?Add@?$NiTSet@VNiColorA@@@@QAEIVNiColorA@@@Z @netimmerse/NiBezierCylinder
// FUNCTION: STBC 0x838C10 ?Add@?$NiTSet@G@@QAEIG@Z @netimmerse/NiBezierTriangle
template <class T> inline unsigned int NiTSet<T>::Add( T item ) {
	while ( size >= maxSize ) {
		Realloc( size * growBy );
	}
	base[size] = item;
	return size++;
}

#endif /* !__NITSET_H__ */
