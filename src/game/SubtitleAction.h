/*
===============================================================================

	SubtitleAction.h

	A timed action that shows a subtitle: Play adds a line to the subtitle
	window, either a paragraph of the text or a paragraph made by the
	caller, and scales the duration; Completed and Skip take the line out.

	0x44 bytes, allocated from the pools.

	Evidence: the class strings "SubtitleAction", "_p_SubtitleAction" and
	"SubtitleActionPtr", the SWIG wrappers SubtitleAction_Create (a
	localization database, a key and the factor, default 1.25) and
	SubtitleAction_CreateC (the text and the factor), and the stream logs
	"Saving SubtitleAction" and "Loading SubtitleAction". The constructors
	(0x55C690, 0x55C720, 0x55C790) call TGTimedAction's, construct the
	string at 0x2C and store the vtable 0x88B568 (TGAction's 24 slots).
	GetObjType returns 0x811C, the type the game's class registration files
	the creator 0x442DC0 under.

===============================================================================
*/

#ifndef __SUBTITLEACTION_H__
#define __SUBTITLEACTION_H__

#include "TGTimedAction.h"
#include "TGString.h"

class TGLocalizationDatabase;
class TGParagraph;

// GetObjType's value for a SubtitleAction.
#define SUBTITLEACTION_TYPE		0x811C

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x00442ed0 ??_GSubtitleAction@@UAEPAXI@Z @game/UtopiaModule
class SubtitleAction : public TGTimedAction {
public:
	// FUNCTION: STBC 0x00442df0 ??0SubtitleAction@@QAE@J@Z @game/UtopiaModule
					SubtitleAction( long id = 0 ) : TGTimedAction( id ), text( "" ) {}
					SubtitleAction( TGLocalizationDatabase *database, char *key, float durationScale = 1.25f );
					SubtitleAction( TGParagraph *paragraph, float durationScale = 1.25f );
					SubtitleAction( char *text, float durationScale = 1.25f );
	// FUNCTION: STBC 0x00442f00 ??1SubtitleAction@@UAE@XZ @game/UtopiaModule
	virtual			~SubtitleAction( void ) {}

	// The SWIG wrappers SubtitleAction_Create and SubtitleAction_CreateC
	// expand them.
	static SubtitleAction *	Create( TGLocalizationDatabase *database, char *key, float durationScale = 1.25f ) {
						return new SubtitleAction( database, key, durationScale );
					}
	static SubtitleAction *	CreateC( char *text, float durationScale = 1.25f ) { return new SubtitleAction( text, durationScale ); }

	// TGObject
	// FUNCTION: STBC 0x00442e50 ?GetObjType@SubtitleAction@@UAEHXZ @game/UtopiaModule
	virtual int		GetObjType( void ) { return SUBTITLEACTION_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	virtual bool	FixUpPointers( void );
	virtual void	PostFixUp( void );
	// FUNCTION: STBC 0x00442ea0 ?GetClassName@SubtitleAction@@UAEPADXZ @game/UtopiaModule
	virtual char *	GetClassName( void ) { return "SubtitleAction"; }
	// FUNCTION: STBC 0x00442eb0 ?GetSwigTypeName@SubtitleAction@@UAEPADXZ @game/UtopiaModule
	virtual char *	GetSwigTypeName( void ) { return "_p_SubtitleAction"; }
	// FUNCTION: STBC 0x00442ec0 ?GetPythonClassName@SubtitleAction@@UAEPADXZ @game/UtopiaModule
	virtual char *	GetPythonClassName( void ) { return "SubtitleActionPtr"; }

	// TGAction
	virtual void	Play( void );
	virtual void	Skip( void );
	virtual void	Completed( void );

protected:
	long			lineID;					// 0x28 the subtitle window's line; 0 before Play
	TGString		text;					// 0x2C shown when there is no paragraph
	TGParagraph *	paragraph;				// 0x3C shown instead of the text; its ID between ReadFromStream and PostFixUp once shown
	float			durationScale;			// 0x40 Play multiplies a positive duration by it
};

/*
================
SubtitleAction::IsTypeOf
================
*/
// FUNCTION: STBC 0x00442e60 ?IsTypeOf@SubtitleAction@@UAE_NH@Z @game/UtopiaModule
inline bool SubtitleAction::IsTypeOf( int type ) {
	if ( type == SUBTITLEACTION_TYPE ) {
		return true;
	}
	return TGTimedAction::IsTypeOf( type );
}

#endif /* !__SUBTITLEACTION_H__ */
