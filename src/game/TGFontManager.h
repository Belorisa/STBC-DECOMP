/*
===============================================================================

	TGFontManager.h

	The manager of the fonts: for each font family it registers the sizes
	the scripts can create on demand and holds the font groups loaded for
	it, and it names the default font that a paragraph falls back on. There
	is one, the global g_kFontManager (the SWIG variable).

	0x24 bytes, with a vtable of its own (0x897E5C, two slots).

	Evidence: the constructor (0x734810, run by the static initializer
	0x7347E0) builds the list at 0x04 (two items per block), stores the
	vtable and clears the default font (0x1C, 0x20). The destructor
	(0x734840) deletes every family, then the default font's name, then
	the list. RegisterFont (0x734A30) searches the list for the family by
	name and appends a new one (0x34 bytes) when there is none; the first
	font registered becomes the default. GetFontGroup (0x734F70) searches
	the list by name and asks the family for the size. SetDefaultFont
	(0x734F10) replaces the name with a copy and stores the size.
	TGParagraph's Init falls back on GetFontGroup of the default font.
	The SWIG interface declared TGFontManager_GetDefaultFont, whose wrapper
	the linker dropped. The two virtual functions write and read the
	families and the default font to a stream (0x735410, 0x735500).

===============================================================================
*/

#ifndef __TGFONTMANAGER_H__
#define __TGFONTMANAGER_H__

#include "NiTList.h"

class TGFontGroup;
class TGFontRegistration;		// TGIconManager.h
class TGStream;

/*
===============================================================================

	TGFontFamily

	The fonts of one name: the sizes registered for it and the font groups
	loaded for it. 0x34 bytes, not polymorphic. The name is invented; no
	string names the class.

	Evidence: the constructor (0x7348F0) builds the lists at 0x04 and 0x1C
	(two items per block) and copies the name (NULL stays NULL); the
	destructor (0x7349B0) deletes the name and frees both lists' blocks.
	TGFontManager::RegisterFont hands the size and the script module and
	function to 0x734C20, which appends a registration to the list at
	0x04 and registers the icon group "<name><size>" ("%s%.2f") with
	g_kIconManager; GetFontGroup hands the size to 0x734FE0, which searches
	the groups at 0x1C (0x735120) and creates one from the nearest
	registered size when none is loaded.

===============================================================================
*/

class TGFontFamily {
public:
					TGFontFamily( char *name );
					~TGFontFamily( void );

	void			RegisterSize( float size, char *module, char *function );	// invented name
	TGFontGroup *	GetFontGroup( float size );		// invented name
	TGFontGroup *	FindFontGroup( float size );	// invented name; NULL when none of that size is loaded
	void			AddFontGroup( TGFontGroup *group );		// invented name
	void			RemoveGroup( TGFontGroup *group );		// invented name
	void			Purge( void );					// invented name; deletes the groups nothing holds
	// The name, then each registration and each group (invented names; the
	// manager's stream functions call them).
	bool			WriteToStream( TGStream *stream );
	bool			ReadFromStream( TGStream *stream );

	char *			name;					// 0x00 a heap copy, or NULL
	NiTList<TGFontRegistration *>	registrations;	// 0x04 the sizes the scripts can create
	NiTList<TGFontGroup *>	fontGroups;		// 0x1C the groups loaded
};

class TGFontManager {
public:
					TGFontManager( void );
					~TGFontManager( void );

	virtual bool	WriteToStream( TGStream *stream );		// invented name, as TGObject's
	virtual bool	ReadFromStream( TGStream *stream );		// invented name, as TGObject's

	void	RegisterFont( char *name, float size, char *module, char *function );
	TGFontGroup *	CreateFontGroup( char *name, float size );
	void	AddFontGroup( TGFontGroup *group );
	void	SetDefaultFont( char *name, float size );
	TGFontGroup *	GetFontGroup( char *name, float size );
	void	Purge( void );
	void	RemoveGroup( TGFontGroup *group );
	// Forgets the fonts without deleting them, before a saved game is loaded
	// (invented name, after TGEventManager::ForgetAll).
	void	ForgetAll( void );

	TGFontGroup *	GetDefaultFont( void ) { return GetFontGroup( defaultFontName, defaultFontSize ); }

protected:
	NiTList<TGFontFamily *>	families;		// 0x04 invented name; one per registered font name
	char *			defaultFontName;		// 0x1C a heap copy, or NULL
	float			defaultFontSize;		// 0x20
};

// The font manager (the SWIG variable g_kFontManager).
extern TGFontManager	g_kFontManager;

#endif /* !__TGFONTMANAGER_H__ */
