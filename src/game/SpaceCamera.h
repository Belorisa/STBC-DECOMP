/*
===============================================================================

	SpaceCamera.h

	The members of SpaceCamera that the SWIG wrappers SpaceCamera_* call. Only what the
	wrappers prove is declared: no virtual functions yet.

	Evidence: SetSpaceDustForCamera (0x42F7F0) compares and stores its
	argument at 0x12C, just past CameraObjectClass (0x12C bytes), and
	tests the flag 0x8D96F4 SetSpaceDustInGame sets; the SWIG wrapper
	SpaceCamera_IsSpaceDustEnabled returns both together.

===============================================================================
*/

#ifndef __SPACECAMERA_H__
#define __SPACECAMERA_H__

#include "CameraObjectClass.h"
#include "NiParticles.h"
#include "NiParticlesData.h"

class SetClass;
class TGStream;

// GetObjType's value for a SpaceCamera (the type its object lists use).
#define SPACECAMERA_TYPE		0x80BF

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x0042eaf0 ??_GSpaceCamera@@UAEPAXI@Z @game/SpaceCamera
class SpaceCamera : public CameraObjectClass {
public:
					SpaceCamera( long id );	// the loader's: the stream fills the members
					SpaceCamera( char *name_ );
	// The script-side constructor (the SWIG wrapper SpaceCamera_Create expands it).
	static SpaceCamera *	Create( char *name ) { return new SpaceCamera( name ); }
	virtual			~SpaceCamera( void );

	// TGObject
	virtual int		GetObjType( void );
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	virtual void	PostFixUp( void );
	virtual char *	GetClassName( void );
	virtual char *	GetSwigTypeName( void );
	virtual char *	GetPythonClassName( void );

	// TGEventHandlerObject
	virtual TGEventHandlerTable *	GetClassHandlers( void );
	// The class's handler table, for the modules that link the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

	// BaseObjectClass
	virtual void	Update( float time );
	virtual void	SetContainingSet( SetClass *set );
	virtual void	AddToSet( SetClass *set );
	virtual void	RemoveFromSet( SetClass *set );

	void	SetSpaceDustForCamera( bool );
	static SpaceCamera *	GetObject( SetClass *, char * );
	static int	GetNumClassObjects( SetClass * );
	// the set's space cameras and their number (invented name, as
	// CameraObjectClass::GetClassObjects)
	static SpaceCamera **	GetClassObjects( SetClass *set, int *count );
	// the space camera with the ID in the set, or among every object when
	// the set is NULL (invented name, as CameraObjectClass::GetObjectByID)
	static SpaceCamera *	GetObjectByID( SetClass *set, long id );

	// Declared for the SWIG wrappers that call them.
public:
	static void	SetSpaceDustInGame( bool );	// the SWIG wrapper SpaceCamera_SetSpaceDustInGame calls it
	static bool	IsSpaceDustEnabledInGame( void ) { return spaceDustInGame; }
	bool	IsSpaceDustEnabled( void ) { return spaceDustForCamera && spaceDustInGame; }

protected:
	// The dust: DUST_COUNT particles kept within DUST_MAX_DISTANCE of the
	// camera and inside its view; a particle that leaves either goes back
	// in front of the camera (invented names).
	void	UpdateSpaceDust( float time );
	void	PlaceDustParticle( int index );
	void	CreateSpaceDust( int count );
	void	DeleteSpaceDust( void );
	// CreateSpaceDust for DUST_COUNT particles, all of them shown (invented name).
	void	StartSpaceDust( void ) {
				CreateSpaceDust( DUST_COUNT );
				dust->GetParticlesData()->SetActiveParticleCount( DUST_COUNT );
			}
	// Whether the camera is the one the rendered set draws through, or the
	// one its bridge's view screen shows (invented name).
	bool	IsRenderedCamera( void );
	// Sets up a new camera: the NetImmerse camera with the name, its
	// keyframe controller and node, and the frustum (invented name).
	void	Initialize( char *name_ );
	// The frustum of the dust camera: DUST_HALF_WIDTH to each side at unit
	// distance, as high as the renderer's screen proportions make it, out
	// to DUST_FAR_DISTANCE (invented name).
	void	SetDustFrustum( NiCamera *niCamera );
	// Every set's space cameras in one new array, and their number; NULL
	// when there are none (invented name).
	static SpaceCamera **	GetAllSpaceCameras( int *count );

	enum {
		DUST_COUNT = 60
	};

	bool			spaceDustForCamera;		// 0x12C
	NiPointer<NiParticles>	dust;			// 0x130 the space dust while it is shown (invented name)

	static const float	DUST_MAX_DISTANCE;
	static const float	DUST_MIN_DISTANCE;
	static const float	DUST_SIZE;
	static const float	DUST_HALF_WIDTH;

	static bool		spaceDustInGame;		// true at first
	static TGEventHandlerTable	classHandlers;		// invented name
};

#endif /* !__SPACECAMERA_H__ */
