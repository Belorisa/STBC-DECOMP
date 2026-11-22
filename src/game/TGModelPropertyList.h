/*
===============================================================================

	TGModelPropertyList.h

	The members of TGModelPropertyList that the SWIG wrappers TGModelPropertyList_* call. Only what the
	wrappers prove is declared: no data members and no virtual functions yet.

===============================================================================
*/

#ifndef __TGMODELPROPERTYLIST_H__
#define __TGMODELPROPERTYLIST_H__

#include "NiTList.h"

class TGModelPropertyInstance;

// A list of the property instances. 0x20 bytes. Evidence: the constructor
// (0x6EDFF0) builds the list base at 0x04 (an NiTList with two items per
// block, inline), stores the vtable 0x896204 (one slot, the destructor) and
// clears the iterator at 0x1C; the destructor (0x6EE040) converts this to
// the list base with a NULL test, as for a base class, before freeing the
// blocks. TGBeginIteration, TGGetNext and TGDestroy (0x6EE090 to 0x6EE0C0)
// walk and free it.
class TGModelPropertyList : public NiTList<TGModelPropertyInstance *> {
public:
					TGModelPropertyList( void );
	virtual			~TGModelPropertyList( void );

	void	TGBeginIteration( void );
	TGModelPropertyInstance *	TGGetNext( void );
	void	TGDestroy( void );
	bool	TGDoneIterating( void ) { return iterator ? false : true; }
	int		TGGetNumItems( void ) { return GetSize(); }

protected:
	NiTListIterator	iterator;						// 0x1C the position TGGetNext returns next (invented name)
};

#endif /* !__TGMODELPROPERTYLIST_H__ */
