/*
===============================================================================

	PlainAI.h

	An AI run by a script: an instance of the class named like its module
	in "AI.PlainAI.<module>". The script's Update returns the AI's status
	and GetNextUpdateTime the delay to the next update; Activate,
	Deactivate, GotFocus, LostFocus, Reset and GetStatusInfo are called
	when the script has them.

	0x40 bytes: the create function (0x48D800) allocates that much.

	Evidence: the constructor (0x48CC40) and the destructor (0x48CCD0)
	store the vtable 0x88C0D8 (21 slots, ArtificialIntelligence's); the
	log strings "PlainAI(%s)::Update", "::IsDormant, (false)",
	"::SetActive", "::SetInactive", "::GotFocus" and "::LostFocus" name
	the overrides. The script method names are the pointers 0x8DBA58 to
	0x8DBA78. SetScriptModule (the SWIG wrapper PlainAI_SetScriptModule)
	makes the instance; the Python methods "SetScriptInstance" and
	"GetScriptInstance" of the shadow class set and get it.

===============================================================================
*/

#ifndef __PLAINAI_H__
#define __PLAINAI_H__

#include "ArtificialIntelligence.h"

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
class PlainAI : public ArtificialIntelligence {
public:
					PlainAI( PhysicsObjectClass *object, char *name_ );
					PlainAI( TGStream *stream, bool *ok );
	virtual			~PlainAI( void );

	// ArtificialIntelligence
	virtual void	Reset( void );
	virtual void	GetAllAIsInTree( NiTList<ArtificialIntelligence *> &ais );
	virtual void	GetFocusAIs( NiTList<ArtificialIntelligence *> &ais );
	// Calls the script method named by the registered function's "Name".
	virtual bool	CallExternalFunction( char *functionName, PyObject *arguments );
	virtual void	SetActive( void );
	virtual void	SetInactive( void );
	virtual void	GotFocus( void );
	virtual void	LostFocus( void );
	virtual int		Update( double time );
	virtual bool	IsDormant( void );
	virtual PyObject *	VirtualSlot15( void );
	virtual bool	WriteToStream( TGStream *stream );
	virtual int		GetAIType( void );
	virtual bool	VirtualSlot20( int type );

	// Makes the script instance of the module's class (invented name for
	// the module argument).
	void			SetScriptModule( char *module );
	void			SetScriptInstance( PyObject *instance );
	PyObject *		GetScriptInstance( void ) { return scriptInstance; }
	// The script's Activate is no longer called (the SWIG wrapper
	// PlainAI_StopCallingActivate, inline).
	void			StopCallingActivate( void ) { hasActivate = false; }

	// The stream constructor, for CreateFromStream.
	static ArtificialIntelligence *	Create( TGStream *stream );
	// The script-side constructor (the SWIG wrapper PlainAI_Create calls it).
	// FUNCTION: STBC 0x006043c0 ?Create@PlainAI@@SAPAV1@PAVPhysicsObjectClass@@PAD@Z
	static PlainAI *	Create( PhysicsObjectClass *object, char *name ) { return new PlainAI( object, name ); }

protected:
	// Notes which of the optional methods the script instance has.
	void			FindScriptMethods( void );		// invented name
	// The script's Update; US_DONE when it fails or there is no script.
	int				CallScriptUpdate( void );		// invented name
	// The script's GetNextUpdateTime; 0 when it fails.
	float			GetNextUpdateTime( void );

	PyObject *		scriptInstance;		// 0x28 a reference held
	char *			scriptModule;		// 0x2C "AI.PlainAI.<module>", a new[] copy
	int				status;				// 0x30 the last update's status
	bool			hasActivate;		// 0x34 the script has the method
	bool			hasDeactivate;		// 0x35
	bool			hasGotFocus;		// 0x36
	bool			hasLostFocus;		// 0x37
	bool			hasReset;			// 0x38
	bool			hasGetStatusInfo;	// 0x39
	float			nextUpdateTime;		// 0x3C game time of the next update

	// The script's method names.
	static char *	updateMethod;
	static char *	getNextUpdateTimeMethod;
	static char *	activateMethod;
	static char *	deactivateMethod;
	static char *	resetMethod;
	static char *	gotFocusMethod;
	static char *	lostFocusMethod;
	static char *	registerExternalFunctionsMethod;
	static char *	getStatusInfoMethod;
};

// The Python methods of the shadow class.
PyObject *	PlainAI_SetScriptInstance( PyObject *self, PyObject *args );
PyObject *	PlainAI_GetScriptInstance( PyObject *self, PyObject *args );

#endif /* !__PLAINAI_H__ */
