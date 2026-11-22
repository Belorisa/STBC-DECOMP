/*
===============================================================================

	STFillGauge.h

	A bar that shows how full a value is: two icons side by side, the fill
	in the fill colour (or the alternate colour) sized to the value's share
	of its maximum, and the empty part in the empty colour. The value comes
	either from member functions of a game object or from functions of a
	Python module, called with the object; every gauge is updated from the
	list of gauges.

	0xB0 bytes (its scalar deleting destructor frees 0xB0).

	Evidence: the constructor (0x522160, called by STFillGauge_Create with
	an icon number, an icon group name and a flag) builds the pane,
	constructs the three colours, stores the vtable 0x88FFA8 (TGPane's 82
	slots; of the UI slots only Layout is overridden), sets up the members
	(0x522410) and adds the fill (colour 0x78) and the empty icon (colour
	0x88). GetObjType returns 0x80F0. The SWIG wrappers STFillGauge_* name
	the members: SetObject stores the object's ID (0x4C), GetObject looks
	it up, SetFillColor, SetEmptyColor and SetAlternateColor write 0x78,
	0x88 and 0x98, SetPythonMethods copies four strings (0x5C to 0x68),
	SetFillDirection writes 0xA8 (the SWIG constants STFillGauge_*_TO_*).
	EngPowerDisplay::CreateBatteryGauge stores the value and maximum
	functions (0x50, 0x54) and clears the alternate function (0x58);
	InterfaceModule walks the list of gauges (0x988FA0) to update them.

===============================================================================
*/

#ifndef __STFILLGAUGE_H__
#define __STFILLGAUGE_H__

#include "TGPane.h"
#include "NiColor.h"
#include "TGColorA.h"
#include "NiTList.h"

class TGObject;

// GetObjType's value for an STFillGauge.
#define STFILLGAUGE_TYPE		0x80F0

// The functions a gauge reads its object through (invented names): the
// value and its maximum, and whether to draw the fill in the alternate
// colour.
typedef float ( TGObject::*GaugeValueFunction )( void );
typedef bool ( TGObject::*GaugeFlagFunction )( void );

// The functions a saved gauge can name, by their place in the table.
#define STFILLGAUGE_NUM_FUNCTIONS	12

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x00522350 ??_GSTFillGauge@@UAEPAXI@Z
class STFillGauge : public TGPane {
public:
	// The class's handler table, for the module that links the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

	// The way the fill grows (the SWIG constants STFillGauge_*).
	enum fillDirection_t {
		LEFT_TO_RIGHT,
		RIGHT_TO_LEFT,
		BOTTOM_TO_TOP,
		TOP_TO_BOTTOM
	};

					STFillGauge( long id );
					STFillGauge( int iconNum, char *iconGroupName, bool tileIcons );
	static STFillGauge *	Create( int iconNum, char *iconGroupName, bool tileIcons ) { return new STFillGauge( iconNum, iconGroupName, tileIcons ); }	// the SWIG wrapper STFillGauge_Create expands it
	virtual			~STFillGauge( void );

	// TGObject
	// FUNCTION: STBC 0x005222a0 ?GetObjType@STFillGauge@@UAEHXZ
	virtual int		GetObjType( void ) { return STFILLGAUGE_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	virtual void	PostFixUp( void );
	// FUNCTION: STBC 0x00522310 ?GetClassName@STFillGauge@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "STFillGauge"; }
	// FUNCTION: STBC 0x00522320 ?GetSwigTypeName@STFillGauge@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_STFillGauge"; }
	// FUNCTION: STBC 0x00522330 ?GetPythonClassName@STFillGauge@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "STFillGaugePtr"; }

	// TGEventHandlerObject
	// FUNCTION: STBC 0x00522340 ?GetClassHandlers@STFillGauge@@UAEPAVTGEventHandlerTable@@XZ
	virtual TGEventHandlerTable *	GetClassHandlers( void ) { return &classHandlers; }

	// TGUIObject
	virtual void	Layout( void );

	static STFillGauge *	Cast( TGObject * );

	// Sizes the fill to the value's share of the maximum and colours it.
	void			UpdateGauge( void );
	void			SetFillColor( NiColorA color );
	void			SetEmptyColor( NiColorA color );
	void			SetAlternateColor( NiColorA color );
	// The same for the scripts (the SWIG wrappers STFillGauge_SetFillColor and the rest).
	// FUNCTION: STBC 0x0062cab0 ?SetFillColor@STFillGauge@@QAEXVTGColorA@@@Z @game/AppWrap
	void			SetFillColor( TGColorA color ) { SetFillColor( (NiColorA)color ); }
	// FUNCTION: STBC 0x0062cbd0 ?SetEmptyColor@STFillGauge@@QAEXVTGColorA@@@Z @game/AppWrap
	void			SetEmptyColor( TGColorA color ) { SetEmptyColor( (NiColorA)color ); }
	// FUNCTION: STBC 0x0062ccf0 ?SetAlternateColor@STFillGauge@@QAEXVTGColorA@@@Z @game/AppWrap
	void			SetAlternateColor( TGColorA color ) { SetAlternateColor( (NiColorA)color ); }
	// Names the Python module and its functions that give the value, the
	// maximum and the alternate flag; each is called with the object.
	void			SetPythonMethods( char *module, char *valueFunction, char *maxFunction, char *alternateFunction );
	void			SetFillDirection( int direction ) { fillDirection = direction; }
	void			SetObject( TGObject *object ) {
						if ( object != NULL ) {
							objectID = object->GetObjID();
						} else {
							objectID = 0;
						}
					}

	// Every gauge, in the order they were made (invented name).
	static NiTList<STFillGauge *>	gauges;

protected:
	void			Init( void );								// invented name
	void			ClearSource( void );						// invented name
	// A texture coordinate rounded to whole pixels across (axis 0) or down
	// (axis 1) the screen (invented name).
	float			RoundToPixels( float coord, int axis );
	// A function's place in the table, for the stream (invented names).
	int				GetFunctionIndex( GaugeValueFunction function );
	int				GetFunctionIndex( GaugeFlagFunction function );
	GaugeValueFunction	GetValueFunction( int index );
	GaugeFlagFunction	GetFlagFunction( int index );

	long			objectID;					// 0x4C the object the functions are called on

public:
	// The object functions; the menus that make gauges set them directly.
	GaugeValueFunction	valueFunction;			// 0x50 invented name
	GaugeValueFunction	maxFunction;			// 0x54 invented name; NULL: the maximum is 1
	GaugeFlagFunction	alternateFunction;		// 0x58 invented name; true draws the fill in the alternate colour

protected:
	char *			pythonModule;				// 0x5C owned copies, or NULL
	char *			pythonValueFunction;		// 0x60
	char *			pythonMaxFunction;			// 0x64
	char *			pythonAlternateFunction;	// 0x68
	int				savedValueFunction;			// 0x6C the table places, between loading and fixing up
	int				savedMaxFunction;			// 0x70
	int				savedAlternateFunction;		// 0x74
	NiColorA		fillColor;					// 0x78
	NiColorA		emptyColor;					// 0x88
	NiColorA		alternateColor;				// 0x98
	int				fillDirection;				// 0xA8 fillDirection_t
	bool			tileIcons;					// 0xAC invented name; repeat the icons' art instead of stretching it

	static GaugeValueFunction	functions[STFILLGAUGE_NUM_FUNCTIONS];	// invented name
	static TGEventHandlerTable	classHandlers;		// invented name

	// Declared for the SWIG wrappers that call them.
public:
	static TGObject *	GetObject( STFillGauge *gauge ) { return TGObject::GetTGObjectPtr( gauge->objectID ); }	// the SWIG wrapper STFillGauge_GetObject expands it
};

/*
================
STFillGauge::IsTypeOf
================
*/
// FUNCTION: STBC 0x005222b0
inline bool STFillGauge::IsTypeOf( int type ) {
	if ( type == STFILLGAUGE_TYPE ) {
		return true;
	}
	return TGPane::IsTypeOf( type );
}

#endif /* !__STFILLGAUGE_H__ */
