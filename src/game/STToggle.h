/*
===============================================================================

	STToggle.h

	A menu button that steps through a list of states: its name is the
	primary caption followed by the current state's text, and each state
	may send its own event. Activating the toggle (or the right arrow)
	moves to the next state that has text; Home and End jump to the first
	and the last state.

	0x178 bytes (its scalar deleting destructor frees 0x178).

	Evidence: GetObjType (0x53B740) returns 0x80EF; the class name is
	"STToggle" and the vtable 0x891744 has STButton's 91 slots. SetState
	(0x53BE20) stores the state (0x174), names the button with the primary
	caption (0x114) followed by the state's text (0x124, 16 bytes apart)
	and sets the state's event (0x164, 4 bytes apart) as the activation
	event. The SWIG wrappers STToggle_* name SetState, GetState,
	SetStateValue(W), SetStateValues(W), SetPrimaryCaption(W) and
	GetPrimaryCaption; the inline ones are expanded in the wrappers and in
	the constructors. The handler name is "STToggle::KeyboardHandler".

===============================================================================
*/

#ifndef __STTOGGLE_H__
#define __STTOGGLE_H__

#include "STButton.h"
#include "TGString.h"

class TGEvent;
class TGKeyboardEvent;
class TGObject;

// GetObjType's value for an STToggle.
#define STTOGGLE_TYPE			0x80EF

// The states a toggle holds.
#define STTOGGLE_MAX_STATES		4
#define STTOGGLE_LAST_STATE		( STTOGGLE_MAX_STATES - 1 )

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x0053b800 ??_GSTToggle@@UAEPAXI@Z
class STToggle : public STButton {
public:
	// The stream loader's: the stream fills the members (InterfaceModule's creator).
	// FUNCTION: STBC 0x00557a30 ??0STToggle@@QAE@J@Z @game/InterfaceModule
					STToggle( long id ) : STButton( id ) {}
					STToggle( char *caption, int state, char *value0, TGEvent *event0, char *value1, TGEvent *event1,
							  char *value2, TGEvent *event2, char *value3, TGEvent *event3 );
					STToggle( TGString *caption, int state, TGString *value0, TGEvent *event0, TGString *value1, TGEvent *event1,
							  TGString *value2, TGEvent *event2, TGString *value3, TGEvent *event3 );
					STToggle( TGString *caption, int state, unsigned int buttonFlags, TGString *value0, TGEvent *event0,
							  TGString *value1, TGEvent *event1, TGString *value2, TGEvent *event2, TGString *value3,
							  TGEvent *event3 );
	virtual			~STToggle( void );

	// The SWIG wrappers STToggle_Create, CreateW and CreateUnsizedW expand these.
	static STToggle *	Create( char *caption, int state, char *value0, TGEvent *event0, char *value1, TGEvent *event1,
								char *value2, TGEvent *event2, char *value3, TGEvent *event3 ) {
		return new STToggle( caption, state, value0, event0, value1, event1, value2, event2, value3, event3 );
	}
	static STToggle *	CreateW( TGString *caption, int state, TGString *value0, TGEvent *event0, TGString *value1, TGEvent *event1,
								 TGString *value2, TGEvent *event2, TGString *value3, TGEvent *event3 ) {
		return new STToggle( caption, state, value0, event0, value1, event1, value2, event2, value3, event3 );
	}
	static STToggle *	CreateUnsizedW( TGString *caption, int state, TGString *value0, TGEvent *event0, TGString *value1, TGEvent *event1,
										TGString *value2, TGEvent *event2, TGString *value3, TGEvent *event3 ) {
		return new STToggle( caption, state, 0, value0, event0, value1, event1, value2, event2, value3, event3 );
	}

	// TGObject
	// FUNCTION: STBC 0x0053b740 ?GetObjType@STToggle@@UAEHXZ
	virtual int		GetObjType( void ) { return STTOGGLE_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	virtual bool	FixUpPointers( void );
	virtual void	PostFixUp( void );
	// FUNCTION: STBC 0x0053b7c0 ?GetClassName@STToggle@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "STToggle"; }
	// FUNCTION: STBC 0x0053b7d0 ?GetSwigTypeName@STToggle@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_STToggle"; }
	// FUNCTION: STBC 0x0053b7e0 ?GetPythonClassName@STToggle@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "STTogglePtr"; }

	// TGEventHandlerObject
	// FUNCTION: STBC 0x0053b7f0 ?GetClassHandlers@STToggle@@UAEPAVTGEventHandlerTable@@XZ
	virtual TGEventHandlerTable *	GetClassHandlers( void ) { return &classHandlers; }

	// TGUIObject
	virtual void	GetDesiredSize( NiPoint2 *size );

	// TGButtonBase: activation moves to the next state first.
	virtual void	SendActivationEvent( void );

	static STToggle *	Cast( TGObject *object );

	// The caption before the state's text, with the space that separates them.
	void			SetPrimaryCaption( char *caption ) {
		primaryCaption.AssignC( caption );
		primaryCaption.Unknown6F54A0( primaryCaption.GetLength(), ' ' );
	}
	// FUNCTION: STBC 0x0062bc20 ?SetPrimaryCaptionW@STToggle@@QAEXPAVTGString@@@Z @game/AppWrap
	void			SetPrimaryCaptionW( TGString *caption ) {
		TGString empty( "" );
		if ( caption == NULL ) {
			primaryCaption = empty;
		} else {
			primaryCaption = *caption;
		}
		primaryCaption.Unknown6F54A0( primaryCaption.GetLength(), ' ' );
	}
	// FUNCTION: STBC 0x00533a00 ?GetPrimaryCaption@STToggle@@QAE?AVTGString@@XZ
	TGString		GetPrimaryCaption( void ) { return primaryCaption; }

	// The text and the event of one state; the button shows the state again.
	// FUNCTION: STBC 0x0054a080 ?SetStateValue@STToggle@@QAEXHPADPAVTGEvent@@@Z
	void			SetStateValue( int index, char *value, TGEvent *event ) {
		TGString text( "" );
		if ( value != NULL ) {
			text.AssignC( value );
		}
		SetStateValueW( index, &text, event );
	}
	void			SetStateValueW( int index, TGString *value, TGEvent *event );
	// The caption, the state and the four states' texts and events.
	void			SetStateValues( char *caption, int state, char *value0, TGEvent *event0, char *value1, TGEvent *event1,
									char *value2, TGEvent *event2, char *value3, TGEvent *event3 ) {
		char *values[STTOGGLE_MAX_STATES] = { value0, value1, value2, value3 };
		TGEvent *events[STTOGGLE_MAX_STATES] = { event0, event1, event2, event3 };
		SetPrimaryCaption( caption );
		this->state = state;
		for ( int i = 0; i < STTOGGLE_MAX_STATES; i++ ) {
			SetStateValue( i, values[i], events[i] );
		}
	}
	void			SetStateValuesW( TGString *caption, int state, TGString *value0, TGEvent *event0, TGString *value1,
									 TGEvent *event1, TGString *value2, TGEvent *event2, TGString *value3, TGEvent *event3 ) {
		TGString *values[STTOGGLE_MAX_STATES] = { value0, value1, value2, value3 };
		TGEvent *events[STTOGGLE_MAX_STATES] = { event0, event1, event2, event3 };
		SetPrimaryCaptionW( caption );
		this->state = state;
		for ( int i = 0; i < STTOGGLE_MAX_STATES; i++ ) {
			SetStateValueW( i, values[i], events[i] );
		}
	}

	void			SetState( int state, bool layout = true );
	int				GetState( void ) { return state; }
	// The states with neither text nor event (invented name).
	int				GetNumUnusedStates( void );
	// Moves to the next state with text, back to the first after the last
	// (invented name).
	void			NextState( void );
	// Moves to the state before, to the last from the first (invented name).
	void			PreviousState( void );

	static void		RegisterHandlerNames( void );		// invented name
	static void		RegisterHandlers( void );			// invented name

	void			KeyboardHandler( TGKeyboardEvent *event );

	// The class's handler table, for the modules that link the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

protected:
	TGString		primaryCaption;						// 0x114 the name before the state's text
	TGString		stateValues[STTOGGLE_MAX_STATES];	// 0x124 the text of each state
	TGEvent *		stateEvents[STTOGGLE_MAX_STATES];	// 0x164 the activation event of each state, a reference held; an ID between ReadFromStream and PostFixUp
	int				state;								// 0x174

	static TGEventHandlerTable	classHandlers;		// invented name
};

/*
================
STToggle::IsTypeOf
================
*/
// FUNCTION: STBC 0x0053b750
inline bool STToggle::IsTypeOf( int type ) {
	if ( type == STTOGGLE_TYPE ) {
		return true;
	}
	return STButton::IsTypeOf( type );
}

#endif /* !__STTOGGLE_H__ */
