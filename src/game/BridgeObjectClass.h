/*
===============================================================================

	BridgeObjectClass.h

	The bridge model: an object whose model carries the bridge's lights and
	its lit textures. It dims and brightens the textures and the gamma
	node's vertex colours with the light level, flickers the LCARS panels,
	switches the red alert lights and their dimmer controllers, and makes
	the crew react to hits of the player's ship.

	0x144 bytes, allocated from the pools (Create 0x661360 allocates 0x144,
	the scalar deleting destructor 0x6617B0 frees through the pool of that
	size).

	Evidence: the constructor (0x661460) calls ObjectClass's by ID, then
	constructs, in this order, the smart pointers at 0x9C, 0xA0, 0xA4, 0xA8
	and 0xAC, the lists at 0xB0 and 0xC8 (two items per block) and the
	arrays at 0xE0, 0xF8, 0x110 and 0x128 (vtable 0x8947FC); it then stores
	the vtable 0x8946F0 (67 slots: no new virtual functions) and initialises
	the members below. The destructor (0x6619E0) undoes them in reverse.
	GetObjType returns 0x8019, the type Cast checks. The light setup
	(0x661E40) names the nodes and materials it looks up: "Gamma", "Red Off
	GlowAlpha", "Red On GlowAlpha", "Blinking GlowAlpha", "Dim Maps", "Dim
	Material", "LCARs" and "Normal". The handler names are the class's own
	strings ("BridgeObjectClass::PlayerChanged", "::CreateEffectHandler",
	"::HandleChangeAlertLevel").

===============================================================================
*/

#ifndef __BRIDGEOBJECTCLASS_H__
#define __BRIDGEOBJECTCLASS_H__

#include "ObjectClass.h"
#include "NiPointer.h"
#include "NiTArray.h"
#include "NiTList.h"
#include "NiTStringMap.h"

class NiDynamicEffect;
class NiImage;
class NiMaterialProperty;
class NiNode;
class NiRawImageData;
class NiTimeController;
class NiVertexColorProperty;
class PointParticleController;
class PositionOrientationProperty;
class TGModelProperty;
class SetClass;
class TGModelPropertySet;
class TGObject;

// GetObjType's value for a BridgeObjectClass (the type its Cast checks).
#define BRIDGEOBJECTCLASS_TYPE		0x8019

// The states of the bridge's LCARS panels (invented names): FlickerLCARs
// sets LCARS_FLICKERING and keeps the state it interrupts, TurnLCARsOff
// LCARS_OFF.
enum lcarsState_t {
	LCARS_ON,
	LCARS_FLICKERING,
	LCARS_OFF
};

/*
================
BridgeTexture

A texture the light level rewrites: a copy of its original pixels, and the
image the renderer shows, whose pixels are rebuilt from the copy (0x662820
and 0x662A40 allocate the 8 bytes and fill them). The name is invented.
================
*/
// FUNCTION: STBC 0x00661d60 ??1BridgeTexture@@QAE@XZ
struct BridgeTexture {
	NiPointer<NiRawImageData>	original;	// 0x00 the pixels as loaded
	NiPointer<NiImage>			image;		// 0x04 the image shown
};

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x006617b0 ??_GBridgeObjectClass@@UAEPAXI@Z
class BridgeObjectClass : public ObjectClass {
public:
					BridgeObjectClass( NiAVObject *model );
					BridgeObjectClass( long id );
	virtual			~BridgeObjectClass( void );

	// TGObject
	virtual int		GetObjType( void );
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	virtual char *	GetClassName( void );
	virtual char *	GetSwigTypeName( void );
	virtual char *	GetPythonClassName( void );

	// TGEventHandlerObject
	virtual TGEventHandlerTable *	GetClassHandlers( void );
	// The class's handler table, for the modules that link the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

	// BaseObjectClass
	virtual void	Update( float time );
	virtual void	AddToSet( SetClass *set );
	virtual void	RemoveFromSet( SetClass *set );

	static BridgeObjectClass *	Create( char *modelFile );
	static BridgeObjectClass *	Cast( TGObject * );
	void	DoCrewReactions( char * );
	void	FlickerLCARs( float );
	void	TurnLCARsOff( void );
	void	TurnLCARsOn( void );
	void	GoToGreenAllert( void );
	void	GoToYellowAllert( void );
	void	GoToRedAllert( void );
	static BridgeObjectClass *	GetObject( SetClass *, char * );
	static BridgeObjectClass *	GetObjectByID( SetClass *set, long id );			// invented name
	static int	GetNumClassObjects( SetClass * );
	static BridgeObjectClass **	GetClassObjects( SetClass *set, int *count );	// invented name

	// Attaches an emitter at the emitter property of the type and name:
	// builds the controller's target with the texture, takes the
	// property's emit object and attaches the effect under the bridge's
	// node. Returns a new copy of the property's name, NULL when there is
	// no such property. BridgeEffectAction's constructor (0x4DC660) calls
	// it; the SWIG wrapper BridgeObjectClass_CreateEffect calls it.
	char *			CreateEffect( PointParticleController *controller, int propertyType, char *textureName, char *propertyName );

	TGModelPropertySet *	GetPropertySet( void ) { return propertySet; }

	// The bridge's property of the type; with a name, the emitter property
	// of that name (the type is then ignored), without one a random
	// property of the type. NULL when there is none. Placeholder name.
	// The SWIG wrapper BridgeObjectClass_GetHardpointProperty calls it.
	TGModelProperty *	GetHardpointProperty( int propertyType, char *name );

	// Starts a change of the light level (0x84) to the target: stores the
	// first argument as 0x80, and in 0x7C unless the level is already within
	// 0.01 of the target. Placeholder name; GoToGreenAllert and its siblings
	// call it.
	void			Unknown6638C0( float duration, float target );

	// A new node placed and turned as the property says: its forward axis
	// along the property's forward. NULL without a property. Placeholder
	// name.
	NiNode *		Unknown663F30( PositionOrientationProperty *property );

	// Register the class's handler methods by name, and add them to its table.
	static void		RegisterHandlerNames( void );		// invented name
	static void		RegisterHandlers( void );			// invented name

	void			PlayerChanged( TGEvent *event );
	void			CreateEffectHandler( TGEvent *event );
	void			HandleChangeAlertLevel( TGEvent *event );

protected:
	// Keeps a copy of the model file's name (invented name).
	void			SetModelName( char *name );
	// Listens to the player's ship's hits and alert changes, unless the
	// player-changed handler already does (invented name).
	void			AddPlayerHandlers( void );
	// Finds the lit nodes, textures and materials of the model and adds
	// the red alert light (invented name).
	void			SetupLighting( void );
	// Attaches the light under the scene root, switched off, with a dimmer
	// that holds it off, fades it in, holds it on and fades it out again
	// (invented name).
	void			AddAlertLight( NiDynamicEffect *light, float offTime, float onTime, float fadeTime );
	// Collects the textures of the multi-texture properties at and under
	// the object, by file name (invented name).
	void			CollectTextures( NiAVObject *object, NiTStringMap<BridgeTexture *> &textures );
	// A new texture of the image with an RGB copy of its pixels; NULL when
	// the image's file cannot be read (invented name).
	void			CreateTexture( BridgeTexture *&texture, NiImage *image );
	// The same with an RGBA copy, for an image whose pixels are RGBA only;
	// NULL otherwise (invented name).
	void			CreateAlphaTexture( BridgeTexture *&texture, NiImage *image );
	// Keeps byte copies of the vertex colours at and under the object, and
	// sets the colours from them through a table (invented names).
	void			PrepareGamma( NiAVObject *object );
	void			ApplyGamma( NiAVObject *object, unsigned char *table );
	// Applies the light level to the dim material, the textures and the
	// gamma node (invented name).
	void			ApplyLightLevel( void );
	// The colour channels of the textures through the table (invented name).
	void			ApplyColorTable( NiTArray<BridgeTexture *> *textures, unsigned char *table );
	// The alpha channel of the textures through the table (invented name).
	void			ApplyAlphaTable( NiTArray<BridgeTexture *> *textures, unsigned char *table );
	// Switches the red alert lights and their dimmers on, and off (invented
	// names; the yellow alert switches them on too).
	void			AlertLightsOn( void );
	void			AlertLightsOff( void );
	// The crew's reaction to a hit of the ship, hard or not (invented name).
	void			ShakeCrew( bool hard );

	char *			modelName;				// 0x68 owned copy, referred through g_kModelManager (invented name)
	TGModelPropertySet *	propertySet;	// 0x6C owned
	bool			redAlert;				// 0x70 set by GoToRedAllert, cleared by the green and yellow alerts (invented name)
	float			alertGlow;				// 0x74 the alpha the blinking textures last took (invented name)
	float			lastHullDamageTime;		// 0x78 the game time of the last hull hit's bridge damage (invented name)
	float			unknown7C;				// 0x7C the light level's change per fade (Unknown6638C0); 0 when the level is already at the target
	float			unknown80;				// 0x80 Unknown6638C0's first argument
	float			unknown84;				// 0x84 the light level
	float			unknown88;				// 0x88 the level the fade starts from
	float			unknown8C;				// 0x8C the level the fade ends at
	int				lcarsState;				// 0x90 an lcarsState_t (invented name)
	int				flickerReturnState;		// 0x94 the state a flicker ends in (invented name)
	float			flickerTime;			// 0x98 FlickerLCARs' time (invented name)
	NiPointer<NiAVObject>	gamma;			// 0x9C the "Gamma" node, whose vertex colours follow the light level (invented name)
	NiPointer<NiMaterialProperty>	lcarsMaterial;	// 0xA0 the "LCARs" material, whose emittance the LCARS state sets (invented name)
	NiPointer<NiMaterialProperty>	dimMaterial;	// 0xA4 the "Dim Material", whose emittance follows the light level (invented name)
	// TODO identity: nothing in the class's unit sets 0xA8 or 0xAC; the red
	// alert lights set the int at 0xA8's 0x18 (1 on, 0 off), which fits a
	// vertex colour property's source mode, and give 0xAC a white emittance
	NiPointer<NiVertexColorProperty>	unknownA8;	// 0xA8
	NiPointer<NiMaterialProperty>	unknownAC;	// 0xAC
	NiTList<NiPointer<NiDynamicEffect> >	alertLights;	// 0xB0 the red alert lights (invented name)
	NiTList<NiPointer<NiTimeController> >	alertDimmers;	// 0xC8 their dimmer controllers (invented name)
	NiTArray<BridgeTexture *>	normalTextures;		// 0xE0 the "Normal" node's (invented name)
	NiTArray<BridgeTexture *>	dimMapTextures;		// 0xF8 the "Dim Maps" node's (invented name)
	NiTArray<BridgeTexture *>	redOffTextures;		// 0x110 the "Red Off GlowAlpha" node's (invented name)
	NiTArray<BridgeTexture *>	blinkingTextures;	// 0x128 the "Blinking GlowAlpha" node's (invented name)
	bool			playerHandlersAdded;	// 0x140 set once PlayerChanged has added the player's handlers (invented name)

	static TGEventHandlerTable	classHandlers;

	// Declared for the SWIG wrappers that call them.
public:
};

#endif /* !__BRIDGEOBJECTCLASS_H__ */
