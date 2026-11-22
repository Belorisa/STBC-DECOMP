/*
===============================================================================

	TGUIThemeManager.h

	The themes of the UI and the current one, whose frames the
	"FromTheme" constructors take. There is one, the global
	g_kUIThemeManager (the SWIG variable).

	0x20 bytes, with a vtable of its own (0x897E8C, three slots).

	Evidence: the constructor (0x746B50, run by the static initializer
	0x746B20) builds the list at 0x04 (two items per block) and stores the
	vtable; it leaves the current theme to the global's zeroed storage.
	The destructor (0x746BA0) deletes every theme, then the list.
	AddTheme (0x746C30) appends a copy of the theme; FindTheme (0x746D70)
	searches the list by name; SetCurrentTheme (0x746D50) makes the theme
	of that name current. The SWIG interface declared the inline
	GetCurrentTheme (offset 0x1C), which TGFrameWindow::CreateFromTheme,
	TGDialogWindow::CreateFromTheme and TGStringDialog's (0x737F50) read
	through the global. The two other slots write and read the themes
	(0x746DE0, 0x746E50): a theme's address stands in for its ID.

===============================================================================
*/

#ifndef __TGUITHEMEMANAGER_H__
#define __TGUITHEMEMANAGER_H__

#include "NiTList.h"

class TGUITheme;
class TGStream;

// FUNCTION: STBC 0x00746b80 ??_GTGUIThemeManager@@UAEPAXI@Z
class TGUIThemeManager {
public:
					TGUIThemeManager( void );
	virtual			~TGUIThemeManager( void );

	virtual bool	WriteToStream( TGStream *stream );		// invented name, as TGObject's
	virtual bool	ReadFromStream( TGStream *stream );		// invented name, as TGObject's

	TGUITheme *		FindTheme( char *name );
	bool			SetCurrentTheme( char *name );
	void			AddTheme( TGUITheme *theme );

	TGUITheme *		GetCurrentTheme( void ) { return currentTheme; }

protected:
	NiTList<TGUITheme *>	themes;			// 0x04 invented name; copies, owned
	TGUITheme *		currentTheme;			// 0x1C one of the themes, or NULL
};

// The theme manager (the SWIG variable g_kUIThemeManager).
extern TGUIThemeManager	g_kUIThemeManager;

#endif /* !__TGUITHEMEMANAGER_H__ */
