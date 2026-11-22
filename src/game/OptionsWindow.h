/*
===============================================================================

	OptionsWindow.h

	The options menu's main window (MWT_OPTIONS). It remembers which of
	its panes were visible, to show them again, and the object that had
	the focus.

	0x6C bytes. Evidence: the top window's constructor (0x50C430) builds
	it inline on a pooled 0x6C-byte block: MainWindow's constructor with
	MWT_OPTIONS, 0.55 by 0.65, the array at 0x50 (NiTArray( 1, 1 ), its
	constructor 0x50FC00 and vtable 0x88EF44), the vtable 0x88EC9C (84
	slots) and 0x68 cleared. The inline members are the top window's
	unit's copies: the scalar deleting destructor 0x50C7E0, the destructor
	0x50C810, GetObjType 0x50C730 (0x8114), IsTypeOf 0x50C740, the class
	strings 0x50C7A0-0x50C7C0 ("OptionsWindow", "_p_OptionsWindow",
	"OptionsWindowPtr") and GetClassHandlers 0x50C7D0 (0x9877A8). The
	class's own unit (0x50EE30-0x50F5F0) overrides slots 26 (GotFocus),
	29 (LostFocus), 44 (SetVisible), 45 (SetNotVisible) and 82
	(Initialize); SetHasFocusObject keeps the object's ID at 0x68. The
	SWIG wrappers OptionsWindow_* name the rest.

===============================================================================
*/

#ifndef __OPTIONSWINDOW_H__
#define __OPTIONSWINDOW_H__

#include "MainWindow.h"
#include "NiTArray.h"

class TGEvent;
class TGIntEvent;
class TGKeyboardEvent;
class TGObject;
class TGStringEvent;
class TGUIObject;

// The events the main menu's buttons send the options window (the App constants).
#define ET_NEW_GAME					0x800005
#define ET_LOAD_GAME				0x800006
#define ET_SAVE_GAME				0x800007
#define ET_NEW_MULTIPLAYER_GAME		0x8000C6

// GetObjType's value for an OptionsWindow.
#define OPTIONSWINDOW_TYPE		0x8114

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x0050c7e0 ??_GOptionsWindow@@UAEPAXI@Z
// FUNCTION: STBC 0x0050c810 ??1OptionsWindow@@UAE@XZ
class OptionsWindow : public MainWindow {
public:
	// The stream loader's: the stream fills the members (InterfaceModule's creator).
	// FUNCTION: STBC 0x005584a0 ??0OptionsWindow@@QAE@J@Z @game/InterfaceModule
					OptionsWindow( long id ) : MainWindow( id ) {}
	// The class's handler table, for the module that links the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

	// FUNCTION: STBC 0x00622300 ??0OptionsWindow@@QAE@XZ @game/AppWrap
					OptionsWindow( void ) : MainWindow( MWT_OPTIONS, 0.55f, 0.65f ) { hasFocusObjectID = 0; }

	// TGObject
	// FUNCTION: STBC 0x0050c730 ?GetObjType@OptionsWindow@@UAEHXZ
	virtual int		GetObjType( void ) { return OPTIONSWINDOW_TYPE; }
	virtual bool	IsTypeOf( int type );
	// FUNCTION: STBC 0x0050c7a0 ?GetClassName@OptionsWindow@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "OptionsWindow"; }
	// FUNCTION: STBC 0x0050c7b0 ?GetSwigTypeName@OptionsWindow@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_OptionsWindow"; }
	// FUNCTION: STBC 0x0050c7c0 ?GetPythonClassName@OptionsWindow@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "OptionsWindowPtr"; }

	// TGEventHandlerObject
	// FUNCTION: STBC 0x0050c7d0 ?GetClassHandlers@OptionsWindow@@UAEPAVTGEventHandlerTable@@XZ
	virtual TGEventHandlerTable *	GetClassHandlers( void ) { return &classHandlers; }

	// TGUIObject
	virtual void	GotFocus( bool layout = true );
	virtual void	LostFocus( bool layout = true );
	virtual void	SetVisible( bool layout = true );
	virtual void	SetNotVisible( bool layout = true );

	// MainWindow
	virtual void	Initialize( void );

	static void		RegisterHandlerNames( void );		// invented name
	static void		RegisterHandlers( void );			// invented name

	static OptionsWindow *	Cast( TGObject * );
	void	AddToPreviouslyVisible( TGUIObject * );
	void	RemoveFromPreviouslyVisible( TGUIObject * );
	void	SetHasFocusObject( TGUIObject * );
	void	RemoveAllPreviouslyVisible( void );

	// The handlers, by their registered names. The top window registers them (0x50C8B0, 0x50CA50).
	void			KeyboardHandler( TGKeyboardEvent *event );
	// FUNCTION: STBC 0x0050ca30 ?LoadGameHandler@OptionsWindow@@QAEXPAVTGEvent@@@Z @game/TopWindow
	void			LoadGameHandler( TGEvent *event ) {}
	// FUNCTION: STBC 0x0050ca40 ?SaveGameHandler@OptionsWindow@@QAEXPAVTGEvent@@@Z @game/TopWindow
	void			SaveGameHandler( TGEvent *event ) {}
	void			NewGameHandler( TGStringEvent *event );
	void			NewMultiplayerGameHandler( TGEvent *event );
	void			QuitHandler( TGIntEvent *event );

	static TGEventHandlerTable	classHandlers;		// invented name

protected:
	// TODO identity: the element type. The array's vtable (0x88EF44) and its SetAt copy (0x50FCA0) are the unit's
	// own, unlike those of NiTArray<int>, <long>, <unsigned int> and <unsigned long>; the elements are the panes' IDs
	// (compared with the pane's ID at 0x4), taken here as the pointer type they stand for, as NiNodeLoadRecord's are.
	NiTArray<TGUIObject *>	previouslyVisible;	// 0x50 the IDs of the panes to show again (invented name)
	long			hasFocusObjectID;		// 0x68 SetHasFocusObject's object (invented name)
};

/*
================
OptionsWindow::IsTypeOf
================
*/
// FUNCTION: STBC 0x0050c740 @game/TopWindow
inline bool OptionsWindow::IsTypeOf( int type ) {
	if ( type == OPTIONSWINDOW_TYPE ) {
		return true;
	}
	return MainWindow::IsTypeOf( type );
}

#endif /* !__OPTIONSWINDOW_H__ */
