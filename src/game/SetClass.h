/*
===============================================================================

	SetClass.h

	A region of space and what is in it: the objects by ID, name and class
	type, the cameras, the lights and the backdrops, and the scene graph
	they render through.

	The vtable (0x888A7C) has 31 slots: TGEventHandlerObject's 21 and ten
	of SetClass's own. The SWIG wrappers name AddObjectToSet (slot 21); the
	wrappers' RemoveObjectFromSet and DeleteObjectFromSet by name call
	slots 22 and 23 with the object's ID; slot 24 steps the objects, the
	cameras and the lights by the time (Update); slot 26 calls slot 25 with
	the active camera's renderer camera. Slots 27 and 28 compare a name
	with the set's (strcmp), and a set by its name.

	Evidence: the constructor (0x40D150) calls TGEventHandlerObject's and
	constructs the object table at 0x14 (0x40AF40); the classes' AddToSet,
	RemoveFromSet, GetObjectByID and GetNumClassObjects reach it through
	the set (BaseObjectClass 0x4306A0-0x430730). The members from 0x2C on
	are those the constructor proves: the ID-sorted object array at 0x30
	(GetNextObject 0x40FE80 searches it), the name at 0x74, the map from
	object names to IDs at 0x80 (an NiTStringMap of 37 buckets, vtable
	0x888AF8; GetClassObject by name 0x40FE00 reads it), the smart node
	pointers at 0x98 and 0x9C and the nodes at 0xA0-0xAC that the
	constructor creates and attaches under 0x98. The objects' nodes go
	under 0xA0 (BaseObjectClass::SetContainingSet 0x430E20). The SWIG
	wrappers name the members they read: GetDisplayName 0x64, GetName 0x74,
	GetRegionModule 0x78, GetSceneRoot 0xA0, GetEffectRoot 0xA4,
	GetBackdropRoot 0xA8, GetForegroundRoot 0xAC, IsGlowEnabled the static
	0x97E998, GetNebula 0xEC, GetProximityManager 0xF4, SetGlowIntensity
	0x104, IsRendered 0x10D, Get/SetSplitLargeZRenders the static
	0x8D8B30. The size, 0x13C, is the pool block SetClass_Create allocates.
	The arrays at 0x3C-0x5C are SortedArrays like the one at 0x30: the
	objects added while the set steps its objects (0x40E530 moves them
	into 0x30), the cameras (AddCameraToSet 0x4128B0 inserts by ID) and
	the lights. The star sphere at 0xE8 is the backdrop AddBackdropToSet
	finds with StarSphere::Cast. The other members from 0xB0 on are
	placeholders.

===============================================================================
*/

#ifndef __SETCLASS_H__
#define __SETCLASS_H__

#include "TGEventHandlerObject.h"
#include "DistanceModifier.h"
#include "SetObjectTable.h"
#include "SortedArray.h"
#include "TGString.h"
#include "TGPoint3.h"
#include "NiNode.h"
#include "NiPointer.h"
#include "NiTStringMap.h"
#include "NiWireframeProperty.h"
#include "NiFogProperty.h"
#include "NiColor.h"
#include "NiPick.h"

class Backdrop;
class BaseObjectClass;
class CameraObjectClass;
class GlowRenderer;
class LightObjectClass;
class NiCamera;
class NiRender;
class Nebula;
class ProximityManager;
class StarSphere;
class ObjectClass;
class ShipClass;
class TGEventHandlerTable;
class TGObject;
class TGStream;

// GetObjType's value for a SetClass.
#define SETCLASS_TYPE		0x80D1

// Event types, as the SWIG constants of the App module give them.
#define ET_TORPEDO_ENTERED_SET	0x80005C	// a torpedo entered a set
#define ET_ENTERED_SET		0x80005D		// an object entered a set
#define ET_TORPEDO_EXITED_SET	0x80005E	// a torpedo left a set
#define ET_EXITED_SET		0x80005F		// an object left a set

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x0040dbd0 ??_GSetClass@@UAEPAXI@Z
class SetClass : public TGEventHandlerObject {
public:
	// A sound the set plays for one of the objects nearest the active
	// camera; 0x413CB0 stops them (invented names).
	struct ObjectSound {
		long			objectID;		// 0x00 the ID of the object the sound follows
		long			soundID;		// 0x04 the sound manager's ID
	};

	virtual			~SetClass( void );

	// TGObject
	virtual int		GetObjType( void );
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	virtual bool	FixUpPointers( void );
	virtual void	PostFixUp( void );
	virtual char *	GetClassName( void );
	virtual char *	GetSwigTypeName( void );
	virtual char *	GetPythonClassName( void );

	// TGEventHandlerObject
	virtual TGEventHandlerTable *	GetClassHandlers( void );
	// The class's handler table, for the modules that link the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

	virtual bool	AddObjectToSet( ObjectClass *object, char *name_ );
	virtual ObjectClass *	RemoveObjectFromSet( long id );
	virtual void	DeleteObjectFromSet( long id );
	virtual void	Update( float time );
	virtual void	VirtualSlot25( NiCamera *camera, bool, bool, bool );	// renders the set through the camera
	virtual void	VirtualSlot26( void );		// renders the set through its active camera
	// The order of the sets by name (SortedArray's CompareKey and Compare).
	virtual int		CompareKey( char *name_ );			// invented name
	virtual int		Compare( SetClass *other );			// invented name
	virtual bool	VirtualSlot29( TGStream *stream );	// reads the list of strings at 0x134
	virtual void	VirtualSlot30( void );		// sets the active camera named at 0x7C, re-attaches the objects

	static SetClass *	Cast( TGObject *object );
	static bool	CanGlowBeEnabled( void );
	void	SetProximityManagerActive( bool active );
	void	SetRegionModule( char *module );
	void	CalcIsRendered( void );
	bool	AddBackdropToSet( Backdrop *backdrop, char *name_ );
	bool	RemoveBackdropFromSet( Backdrop *backdrop );
	// TODO identity: the backdrop's node changes places with the one before
	// or after it among the backdrop root's children (0x40E6A0, 0x40E810);
	// Backdrop::MoveForward and MoveBack call them
	void	Unknown40E6A0( Backdrop *backdrop );
	void	Unknown40E810( Backdrop *backdrop );
	bool	SetBackgroundModel( char *file, float x, float y, float z );
	bool	RemoveBackgroundModel( void );
	ObjectClass *	RemoveObjectFromSet( char * );
	void	DeleteObjectFromSet( char * );
	ObjectClass *	GetObject( char *name_ );
	ObjectClass *	GetObjectByID( long id );
	// TODO identity: turns the active camera to the object, from the
	// object's world translation and bound (CharacterClass's menu handler
	// calls it for the character's look-at target)
	void			Unknown414270( ObjectClass *object );
	// TODO identity: plays a sound for each of the four objects nearest
	// the active camera (the proximity manager's objects within 35 of it,
	// those with a 0x2CC member), and stops them when the set is not the
	// one at 0x97E9C4 (0x413CB0); the destructor and Update call it
	void			Unknown413CB0( void );
	// TODO identity: moves an object's entry in the name map from oldName to
	// newName (0x4145E0; BaseObjectClass::SetName calls it)
	void			Unknown4145E0( char *oldName, char *newName );
	// TODO identity: gives the set and everything in it a last update time
	// shortly before the time (0x410460; SetManager::Unknown418630 passes
	// the time to every set)
	// Centres the backdrops and the foreground on the camera (no caller
	// in retail; 0x410390).
	void			Unknown410390( NiCamera *camera );
	void			Unknown410460( float time );
	// TODO identity: clears 0x25 of every object, camera and light before
	// Update steps them (0x413C60)
	void			Unknown413C60( void );
	// whether nothing that collides lies within the radius of the location
	// (name invented, after the SWIG wrapper's IsLocationEmptyTG)
	bool	IsLocationEmpty( NiPoint3 *location, float radius, bool ignoreNebulas );
	// the scripts' form
	bool	IsLocationEmptyTG( TGPoint3 *location, float radius, bool ignoreNebulas ) { return IsLocationEmpty( location, radius, ignoreNebulas ); }
	void	SetName( char *name_ );
	ObjectClass *	GetFirstObject( void );
	// A new array of the set's targetable objects, and their count:
	// with targeter, those it can target; with aliveOnly, none that is
	// dead or dying (invented name; STTargetMenu::UpdateTargets calls it).
	ObjectClass **	GetTargetableObjects( int *count, ShipClass *targeter, bool aliveOnly );
	ObjectClass *	GetNextObject( long id );
	ObjectClass *	GetPreviousObject( long id );
	bool	CreateCamera( float x, float y, float z, float unknown4, float unknown5, float unknown6, float unknown7, char *name_ );
	bool	AddCameraToSet( CameraObjectClass *camera, char *name_ );
	CameraObjectClass *	GetCamera( char *name_ );
	void	RemoveCameraFromSet( char *name_ );
	void	DeleteCameraFromSet( char * );
	bool	SetActiveCamera( char * );
	CameraObjectClass *	GetActiveCamera( void );
	// The NetImmerse camera of the active camera; NULL when there is none
	// (0x413010; invented name).
	NiCamera *		GetActiveNiCamera( void );
	bool	CreateAmbientLight( float red, float green, float blue, float dimmer, char *name_ );
	bool	CreateDirectionalLight( float red, float green, float blue, float dimmer, float directionX, float directionY, float directionZ,
								char *name_ );
	// A point light and a spot light at the location, like the two above;
	// nothing in retail calls them (0x413200 and 0x4132A0; invented names).
	bool	CreatePointLight( float x, float y, float z, float red, float green, float blue, float dimmer, float linearAttenuation,
							  char *name_ );
	bool	CreateSpotLight( float x, float y, float z, float red, float green, float blue, float dimmer, float directionX,
							 float directionY, float directionZ, float angle, float unknown12, char *name_ );
	// Names the light and adds it to the set (0x413030; invented name).
	bool	AddLightToSet( LightObjectClass *light, char *name_ );
	LightObjectClass *	GetLight( char *name_ );
	LightObjectClass *	GetLightByID( long id );		// invented name
	void	DeleteLightFromSet( char *name_ );
	// DeleteLightFromSet for the light with the ID (0x413710; invented name)
	void	DeleteLightByID( long id );
	void	StartPick( float, float, bool );
	ObjectClass *	GetPickedObject( void );
	// The pick along the ray, and the next object it hit with its record
	// (0x4138E0, 0x413B80; the overloads StartPick( float, float, bool )
	// and GetPickedObject( void ) call them).
	void	StartPick( NiPoint3 *origin, NiPoint3 *direction, bool boundsOnly );
	ObjectClass *	GetPickedObject( NiPick::Record **record );
	float	GetDotProductToObject( ObjectClass * );
	float	GetDotProductToVector( TGPoint3 vector );
	// The distance between the points, as the set's distance modifiers
	// change it (0x410570; invented name).
	float	GetDistance( NiPoint3 *from, NiPoint3 *to );
	// Adds a modifier to those GetDistance applies (0x4106C0; invented name).
	void	AddDistanceModifier( DistanceModifier *modifier );

	// The wireframe and fog properties of the scene (invented names).
	void			SetWireframe( bool wireframe_ );
	bool			GetWireframe( void );
	void			SetFog( bool fog_ );
	// The colour of the fog, kept in the set and passed to the fog property (0x415C80; invented name)
	void			SetFogColor( NiColor *color );

	// While the objects are locked (Update steps them), the objects added
	// wait in pendingObjects; unlocking moves them in (invented names).
	void			LockObjects( void );
	void			UnlockObjects( void );

	// The set's object of the class type with the ID or name; NULL when there is none.
	BaseObjectClass *	GetClassObject( int type, long id );		// invented name
	BaseObjectClass *	GetClassObject( int type, char *name_ );	// invented name

	NiNode *		GetSceneRoot( void ) { return sceneRoot; }
	NiNode *		GetEffectRoot( void ) { return effectRoot; }
	NiNode *		GetBackdropRoot( void ) { return backdropRoot; }
	NiNode *		GetForegroundRoot( void ) { return foregroundRoot; }
	char *			GetName( void ) { return name; }
	void			GetDisplayName( TGString *string ) { *string = displayName; }
	char *			GetRegionModule( void ) { return regionModule; }
	static bool		IsGlowEnabled( void ) { return glowEnabled; }
	static void		SetGlowEnabled( bool enabled );
	// Releases the glow renderer and forgets whether the card can glow;
	// given a renderer, glow that is enabled is made again for it
	// (invented name; called with each new renderer, 0x42C210).
	static void		ResetGlow( NiRender *renderer );
	// ResetGlow( NULL ), at shutdown (invented name).
	static void		ShutdownGlow( void );
	static SetClass *	GetNull( void ) { return NULL; }
	static TGString	MakeDisplayName( char *name_ );		// the display name for a set name
	void			SetGlowIntensity( float intensity ) { glowIntensity = intensity; }
	static void		SetSplitLargeZRenders( bool split ) { splitLargeZRenders = split; }
	static bool		GetSplitLargeZRenders( void ) { return splitLargeZRenders; }
	bool			IsRendered( void ) { return rendered; }
	Nebula *		GetNebula( void ) { return nebula; }
	ProximityManager *	GetProximityManager( void ) { return proximityManager; }
	// Marks the scene graph changed (invented name; ExplodeParticleController::
	// AttachEffect calls it after attaching an effect to the effect root).
	void			SetSceneChanged( bool changed ) { sceneChanged = changed; }

	SetObjectTable	objectTable;		// 0x14 the objects by class type and ID
	bool			objectsLocked;		// 0x2C objects added meanwhile wait in pendingObjects (invented name)
	SortedArray<ObjectClass, long>	objects;	// 0x30 the objects by ID
	SortedArray<ObjectClass, long>	pendingObjects;	// 0x3C added while objectsLocked (invented name)
	SortedArray<CameraObjectClass, long>	cameras;	// 0x48 by ID
	SortedArray<LightObjectClass, long>	lights;		// 0x54 by ID
	bool			unknown60;			// 0x60 true at first: VirtualSlot26 updates the scene root to the game time and clears it
	TGString		displayName;		// 0x64
	char *			name;				// 0x74 owned (SetName 0x40DF80)
	char *			regionModule;		// 0x78
	char *			activeCameraName;	// 0x7C owned; the camera VirtualSlot30 makes active (invented name)
	NiTStringMap<long>	objectIDs;		// 0x80 the IDs of the objects by name (invented name)
	float			unknown90;			// 0x90 0.001 at first
	float			unknown94;			// 0x94 a random fraction at first
	NiPointer<NiNode>	unknown98;		// 0x98 the nodes at 0xA0-0xAC are attached under it
	NiPointer<NiNode>	unknown9C;		// 0x9C
	NiNode *		sceneRoot;			// 0xA0 the objects' nodes are attached under it
	NiNode *		effectRoot;			// 0xA4
	NiNode *		backdropRoot;		// 0xA8
	NiNode *		foregroundRoot;		// 0xAC
	NiPointer<NiWireframeProperty>	wireframe;	// 0xB0 attached to the scene root (invented name)
	NiPointer<NiFogProperty>	fog;		// 0xB4 attached to the scene root (invented name)
	NiPick *		pick;				// 0xB8 (invented name)
	NiPoint3		pickOrigin;			// 0xBC the ray StartPick was given (invented name)
	NiPoint3		pickDirection;		// 0xC8 (invented name)
	int				pickIndex;			// 0xD4 the next result GetPickedObject returns (invented name)
	bool			unknownD8;			// 0xD8 (identity unproven: GetDotProductToVector clears it as a byte; StartPick sets it)
	NiColor			fogColor;			// 0xDC copied to the fog property (invented name)
	StarSphere *	starSphere;			// 0xE8 the star sphere among the backdrops (invented name)
	Nebula *		nebula;				// 0xEC
	bool			sceneChanged;		// 0xF0 set when the scene graph changes: a backdrop or light added or removed, an effect attached (invented name)
	bool			unknownF1;			// 0xF1 Update steps the objects only when set
	ProximityManager *	proximityManager;	// 0xF4
	int				numDistanceModifiers;	// 0xF8 (invented name)
	DistanceModifier **	distanceModifiers;	// 0xFC heap block of numDistanceModifiers (invented name)
	char *			backgroundModel;	// 0x100 owned; the model file SetBackgroundModel loaded (invented name)
	float			glowIntensity;		// 0x104
	float			unknown108;			// 0x108 200 at first
	bool			unknown10C;			// 0x10C true at first
	bool			rendered;			// 0x10D
	int				numSounds;			// 0x110 the entries of sounds in use (invented name)
	ObjectSound		sounds[4];			// 0x114 the sounds playing for the nearest objects (invented name)
	int				unknown134;			// 0x134 the number of strings at 0x138
	char **			unknown138;			// 0x138 strings VirtualSlot29 reads

	static TGEventHandlerTable	classHandlers;		// invented name
	static bool		glowChecked;		// CanGlowBeEnabled has run since the last ResetGlow
	static bool		glowSupported;		// CanGlowBeEnabled's answer, once glowChecked
	static bool		glowEnabled;
	static bool		splitLargeZRenders;

	// Declared for the SWIG wrappers that call them.
public:
					SetClass( long id = 0 );
	static SetClass *	Create( void ) { return new SetClass; }	// the SWIG wrapper SetClass_Create expands it
};

/*
================
SetClass::IsTypeOf
================
*/
// FUNCTION: STBC 0x0040d980
inline bool SetClass::IsTypeOf( int type ) {
	if ( type == SETCLASS_TYPE ) {
		return true;
	}
	return TGEventHandlerObject::IsTypeOf( type );
}

// The Python methods SetClass.GetTargetableObjects, GetClassObjectList and
// GetNavPoints (UtopiaModule::VirtualSlot00 adds them to the class).
PyObject *	SetClass_GetTargetableObjects( PyObject *self, PyObject *args );
PyObject *	SetClass_GetClassObjectList( PyObject *self, PyObject *args );
PyObject *	SetClass_GetNavPoints( PyObject *self, PyObject *args );

#endif /* !__SETCLASS_H__ */
