/*
===============================================================================

	ReticleManagerWindow.h

	The pane over the tactical view that holds the target reticles. There
	is one (reticleManagerWindow).

	Evidence: the constructor (0x513E20) calls TGPane's (1.0 by 1.0),
	stores the vtable 0x88B334, sets reticleManagerWindow (0x987CAC) and
	adds the broadcast handler "ReticleManagerWindow::ShipDestroyedHandler";
	the class strings are "ReticleManagerWindow", "_p_ReticleManagerWindow"
	and "ReticleManagerWindowPtr" (slots 9-11). Game's destructor empties it
	(KillChildren) and the game's start adds the player's reticle to it.
	No SWIG wrappers: only what the game's code uses is declared.

===============================================================================
*/

#ifndef __RETICLEMANAGERWINDOW_H__
#define __RETICLEMANAGERWINDOW_H__

#include "TGPane.h"
#include "NiCamera.h"
#include "NiRect.h"

class ObjectClass;
class ReticleWindow;
class TGEvent;
class TGEventHandlerTable;
class TGStream;

// GetObjType's value for the reticle manager (App's CT_RETICLE_MANAGER_WINDOW).
#define RETICLEMANAGERWINDOW_TYPE		0x810A

// The type and name slots and the destructor are inline: retail keeps the
// copies of the game's start-up unit (0x4426A0-0x442780).
// FUNCTION: STBC 0x00442750 ??_GReticleManagerWindow@@UAEPAXI@Z
// FUNCTION: STBC 0x00442780 ??1ReticleManagerWindow@@UAE@XZ
class ReticleManagerWindow : public TGPane {
public:
	// FUNCTION: STBC 0x00442670 ??0ReticleManagerWindow@@QAE@J@Z @game/UtopiaModule
					ReticleManagerWindow( long id ) : TGPane( id ) {}	// the loader's: the stream fills the members
					ReticleManagerWindow( void );

	// TGObject
	// FUNCTION: STBC 0x004426a0 ?GetObjType@ReticleManagerWindow@@UAEHXZ
	virtual int		GetObjType( void ) { return RETICLEMANAGERWINDOW_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	// FUNCTION: STBC 0x00442710 ?GetClassName@ReticleManagerWindow@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "ReticleManagerWindow"; }
	// FUNCTION: STBC 0x00442720 ?GetSwigTypeName@ReticleManagerWindow@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_ReticleManagerWindow"; }
	// FUNCTION: STBC 0x00442730 ?GetPythonClassName@ReticleManagerWindow@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "ReticleManagerWindowPtr"; }

	// TGEventHandlerObject
	// FUNCTION: STBC 0x00442740 ?GetClassHandlers@ReticleManagerWindow@@UAEPAVTGEventHandlerTable@@XZ
	virtual TGEventHandlerTable *	GetClassHandlers( void ) { return &classHandlers; }
	// The class's handler table, for the modules that link the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

	// The one reticle manager; NULL before the tactical view is built
	// (invented name).
	static ReticleManagerWindow *	GetReticleManagerWindow( void ) { return reticleManagerWindow; }

	// Shown unless the top window runs a cutscene, shows the options
	// menu or has the cinematic window in focus (invented name).
	void			UpdateVisibility( void );

	// Passes the time on to each reticle (the manager's slot 82; invented name).
	virtual void	VirtualSlot82( float time );

	// Register the handler method by name; the class adds no handlers.
	static void		RegisterHandlerNames( void );		// invented name
	static void		RegisterHandlers( void );			// invented name

	// A ship was destroyed: its reticles go.
	void			ShipDestroyedHandler( TGEvent *event );

	// The reticle following the object, the player's excepted; NULL for
	// none (invented name).
	ReticleWindow *	FindReticle( ObjectClass *object );
	// Deletes the reticles following the object, the player's excepted
	// (invented name).
	void			RemoveReticles( long id );
	// Deletes every reticle but the player's (invented name).
	void			RemoveAllReticles( void );

	// TODO identity: Unknown5152A0, VirtualSlot84 and Unknown515F70 on each reticle
	void			Unknown513F30( void );
	void			Unknown514310( void );
	// Gives the icon manager back the camera and screen size it had before
	// the view screen zoomed the reticles in (the view screen's draw,
	// 0x678A80, calls it; invented name).
	void			RestoreZoom( void );
	// Lays the reticles out for the view screen's camera, with the
	// camera's frustum and port (the view screen's draw, 0x678A80, calls
	// it; invented name).
	void			SetZoom( NiCamera *sourceCamera, NiCamera *camera, float left, float right, float top, float bottom, const NiRect<float> &port );
	void			Unknown514350( void );

	// The map window zoomed the reticles in (invented name).
	bool			IsZoomed( void ) { return zoomed; }
	// The zoomed camera's frustum and port (invented names).
	float			GetFrustumLeft( void ) { return frustumLeft; }
	float			GetFrustumRight( void ) { return frustumRight; }
	float			GetFrustumTop( void ) { return frustumTop; }
	float			GetFrustumBottom( void ) { return frustumBottom; }
	NiRect<float>	GetViewport( void ) { return viewport; }

protected:
	static ReticleManagerWindow *	reticleManagerWindow;	// set by the constructor; invented name
	static TGEventHandlerTable	classHandlers;		// invented name

	int				unknown4C;				// 0x4C
	NiPointer<NiCamera>	savedCamera;		// 0x50 the icon manager's camera while zoomed (invented name)
	float			savedScreenWidth;		// 0x54 the icon manager's while zoomed (invented name)
	float			savedScreenHeight;		// 0x58
	float			frustumLeft;			// 0x5C the zoomed camera's frustum (invented names)
	float			frustumRight;			// 0x60
	float			frustumTop;				// 0x64
	float			frustumBottom;			// 0x68
	bool			zoomed;					// 0x6C (invented name)
	NiRect<float>	viewport;				// 0x70 the zoomed camera's viewport (invented name)
	float			viewportWidth;			// 0x80 1.0 unless zoomed (invented name)
	float			viewportHeight;			// 0x84
};

/*
================
ReticleManagerWindow::IsTypeOf
================
*/
// FUNCTION: STBC 0x004426b0 ?IsTypeOf@ReticleManagerWindow@@UAE_NH@Z
inline bool ReticleManagerWindow::IsTypeOf( int type ) {
	if ( type == RETICLEMANAGERWINDOW_TYPE ) {
		return true;
	}
	return TGPane::IsTypeOf( type );
}

#endif /* !__RETICLEMANAGERWINDOW_H__ */
