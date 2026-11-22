/*
===============================================================================

	FocusManager.h

	The manager of the keyboard focus's tab order: a list of the IDs of the
	objects Tab visits, in order.

	Evidence: the constructor (0x44D1E0) builds the list at 0x04 (two items
	per block) and stores the vtable 0x88B66C; the destructor (0x44D210)
	stores it again and frees the list. The vtable has two slots and no
	destructor: the save (0x44D7A0) writes the count and each ID, the load
	(0x44D7E0) reads them back.

===============================================================================
*/

#ifndef __FOCUSMANAGER_H__
#define __FOCUSMANAGER_H__

#include "NiTList.h"

class TGUIObject;

class TGStream;

class FocusManager {
public:
					FocusManager( void );
					~FocusManager( void );

	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );

	void	AddObjectToTabOrder( TGUIObject * );
	void	RemoveObjectFromTabOrder( TGUIObject * );
	void	RemoveAllObjects( void );
	void	RemoveAllObjectsUnder( TGUIObject * );
	void	Tab( void );

protected:
	// Gives the object the focus in each of its parents, up to the root
	// (0x44D620; invented name).
	void	FocusObject( TGUIObject *object, bool layout );

	NiTList<long>		tabOrder;			// 0x04 the IDs of the objects Tab visits, in order (invented name)
};

// The focus manager (the SWIG variable g_kFocusManager).
extern FocusManager	g_kFocusManager;

#endif /* !__FOCUSMANAGER_H__ */
