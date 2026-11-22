/*
===============================================================================

	STMissionLog.h

	The mission log: a pane over the bridge holding the last lines the
	mission printed. There is one (missionLog, STMissionLog_GetMissionLog),
	made by the game.

	0x58 bytes, allocated from the pools. Evidence: the constructor
	(0x5286D0) calls TGPane's with the ID, stores the vtable 0x890404,
	reads the stored-line count (0x4C, 100 by default) from the
	"General Options" "Lines in Mission Log" configuration value, builds
	the stylized window with its close button when the ID is 0, and sets
	missionLog (0x989414); Game's constructor (0x405C10) allocates 0x58
	bytes for it. The SWIG wrappers name SetNumStoredLines, AddLine and
	ClearLines.

===============================================================================
*/

#ifndef __STMISSIONLOG_H__
#define __STMISSIONLOG_H__

#include "TGPane.h"

class STSubPane;
class TGParagraph;
class TGString;
class TGEventHandlerTable;
class TGEvent;
class TGMouseEvent;
class TGKeyboardEvent;
class TGStream;
class STStylizedWindow;

// GetObjType's value for an STMissionLog.
#define STMISSIONLOG_TYPE		0x80F8

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x00528ad0 ??_GSTMissionLog@@UAEPAXI@Z
class STMissionLog : public TGPane {
public:
	// The class's handler table, for the module that links the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

					STMissionLog( long id = 0 );
	virtual			~STMissionLog( void );

	// TGObject
	// FUNCTION: STBC 0x00528a20 ?GetObjType@STMissionLog@@UAEHXZ
	virtual int		GetObjType( void ) { return STMISSIONLOG_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	// FUNCTION: STBC 0x00528a90 ?GetClassName@STMissionLog@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "STMissionLog"; }
	// FUNCTION: STBC 0x00528aa0 ?GetSwigTypeName@STMissionLog@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_STMissionLog"; }
	// FUNCTION: STBC 0x00528ab0 ?GetPythonClassName@STMissionLog@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "STMissionLogPtr"; }

	// TGEventHandlerObject
	// FUNCTION: STBC 0x00528ac0 ?GetClassHandlers@STMissionLog@@UAEPAVTGEventHandlerTable@@XZ
	virtual TGEventHandlerTable *	GetClassHandlers( void ) { return &classHandlers; }

	// TGUIObject: showing the log hides the subtitles and pauses the game.
	virtual void	SetVisible( bool layout = true );
	virtual void	SetNotVisible( bool layout = true );
	virtual void	Layout( void );

	static void		RegisterHandlerNames( void );		// invented name
	static void		RegisterHandlers( void );			// invented name

	void			MouseHandler( TGMouseEvent *event );
	void			KeyboardHandler( TGKeyboardEvent *event );
	void			Close( TGEvent *event );

	void	SetNumStoredLines( int );
	void	AddLine( TGString * );
	void	ClearLines( void );

	// The one log (STMissionLog_GetMissionLog); NULL without a game.
	static STMissionLog *	GetMissionLog( void ) { return missionLog; }
	// Adds a ready paragraph as a line (invented name; CharacterAction::Play
	// logs a spoken line with it).
	void			AddLineParagraph( TGParagraph *paragraph ) { Unknown528CB0( paragraph ); }

protected:
	STSubPane *		GetLinesPane( void );							// invented name
	void			Unknown528CB0( TGParagraph *paragraph );		// AddLine's second half (adds the paragraph, drops the oldest lines)

	int				numStoredLines;			// 0x4C 100 unless configured
	int				numLines;				// 0x50 the lines the log holds now (invented name)
	bool			unknown54;				// 0x54 (invented name) set means the game was paused for the log: the destructor resumes it

	static TGEventHandlerTable	classHandlers;	// 0x989338; invented name

	static STMissionLog *	missionLog;		// set by the constructor; invented name
};

/*
================
STMissionLog::IsTypeOf
================
*/
// FUNCTION: STBC 0x00528a30 ?IsTypeOf@STMissionLog@@UAE_NH@Z
inline bool STMissionLog::IsTypeOf( int type ) {
	if ( type == STMISSIONLOG_TYPE ) {
		return true;
	}
	return TGPane::IsTypeOf( type );
}

#endif /* !__STMISSIONLOG_H__ */
