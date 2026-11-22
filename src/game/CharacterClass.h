/*
===============================================================================

	CharacterClass.h

	A bridge character: an object with a body and a head model that plays
	animations (queued, random, blinks), speaks lines through its speaking
	queue, shows phonemes and facial images through an overlay manager, and
	carries a menu and a status box in the bridge interface.

	0x1B4 bytes, allocated from the pools (Create 0x667D10 allocates 0x1B4,
	the scalar deleting destructor 0x6673F0 frees through the pool of that
	size).

	Evidence: the constructor (0x666F10) calls ObjectClass's by ID, then
	constructs, in this order, the map at 0xD8 (vtable 0x8949D0), the arrays
	at 0xE8, 0x104 and 0x120 (vtable 0x8949CC), the list at 0x160 (two items
	per block), the smart pointer at 0x180, the string map at 0x184, the
	smart pointer at 0x194 and the map at 0x198 (vtable 0x8949B8); it then
	stores the vtable 0x8948AC (67 slots: no new virtual functions) and
	initialises the members below. GetObjType returns 0x8016, the type Cast
	checks. The SWIG wrappers name the members they read and write
	(GetYesSir 0x74, Get/SetSize 0x78, Get/SetGender 0x7C, IsStateSet 0x80,
	Get/SetAudioMode 0x84, GetCharacterName 0x88, GetLocation 0x9C,
	GetTurnTowardsLocation 0xA0, Get/SetBlinkChance 0xB8, SetAsExtra and
	IsAnExtra 0xC4, IsActive 0xCC, GetStatusBox 0xD4, the random animation
	flag 0x13C and chance 0x140, GetDatabase 0x148, Get/SetMenu 0x14C,
	SetLookAtAdj 0x150, SetBlinkStages 0x178, the animated speaking flag
	0x17C, UsePhonemeGroup 0x1A8, GetLastTalkTime 0x1AC, IsMenuEnabled
	0x1B0). The constants are the App module's CharacterClass_* values.

===============================================================================
*/

#ifndef __CHARACTERCLASS_H__
#define __CHARACTERCLASS_H__

#include "ObjectClass.h"
#include "TGOverlayController.h"
#include "TGString.h"
#include "TGOverlayManager.h"
#include "NiTArray.h"
#include "NiTList.h"
#include "NiTMap.h"
#include "NiTStringMap.h"

class CharacterSpeakingQueue;
class NiAVObject;
class NiBone;
class NiSkinController;
class STTopLevelMenu;
class SetClass;
class TGAction;
class TGAnimNode;
class RotateBonesController;
class TGLocalizationDatabase;
class TGObject;
class TGPane;
class TGParagraph;
class TGPhoneme;
class TGSequence;
class TGStringToStringMap;

// GetObjType's value for a CharacterClass.
#define CHARACTERCLASS_TYPE		0x8016

// GetPositionZoom's value for a position without a zoom (invented name).
#define NO_POSITION_ZOOM		( -1.0f )

// The overlay manager's IDs of the blink phonemes: the stages that close
// the eyes from the first on, and the one that opens them (invented names).
#define FIRST_BLINK_PHONEME		6
#define OPEN_EYES_PHONEME		9

// The highest index of a status line (invented name).
#define MAX_STATUS_INDEX		5

// Event types, as the SWIG constants of the App module give them.
#define ET_CHARACTER_ANIMATION_DONE		0x80004A
#define ET_CHARACTER_SOUND_DONE			0x80004B
#define ET_CHARACTER_MENU				0x80004C

/*
================
CharacterAnimation

An animation the character plays or has queued: the action that plays it
and its category. 0x10 bytes (0x66AEF0 allocates and fills it). The name
is invented.
================
*/
struct CharacterAnimation {
					CharacterAnimation( void ) { flags = 0; name = NULL; actionID = 0; type = 0; }
					~CharacterAnimation( void ) { if ( name != NULL ) { delete name; } }

	long			actionID;		// 0x00 the TGAction's ID; 0 without one
	long			flags;			// 0x04 CharacterClass::CS_* flags of the animation (invented name)
	int				type;			// 0x08 a CharacterClass::CAT_* category
	char *			name;			// 0x0C owned copy; NULL without one
};

/*
================
RandomAnimation

An animation the character may play at random, chosen by weight
(AddRandomAnimation 0x66AB00 allocates 0x10 bytes). The name is invented.
================
*/
struct RandomAnimation {
	char *			name;			// 0x00 owned copy
	int				unknown04;		// 0x04
	int				weight;			// 0x08 the running total of the weights up to this one
	bool			unknown0C;		// 0x0C AddRandomAnimation's last argument was 1
};

/*
================
PositionZoom

The camera zoom and the look-at target for a character at a named bridge
position (AddPositionZoom 0x66C530 grows the array by 0x18 bytes). The name
is invented.
================
*/
struct PositionZoom {
	char *			position;		// 0x00 owned copy
	float			zoom;			// 0x04
	char *			lookAtName;		// 0x08 owned copy; NULL without one
	int				unknown0C;		// 0x0C
	int				unknown10;		// 0x10
	int				unknown14;		// 0x14
};

/*
================
PhonemeList

The phonemes a character added under one phoneme type: their IDs in the
overlay manager and the group each belongs to (AddPhoneme 0x669050
allocates 0x30 bytes). The name is invented.
================
*/
struct PhonemeList {
					PhonemeList( void ) : phonemeIDs( 2 ), groups( 2 ) {}

	NiTList<long>	phonemeIDs;		// 0x00
	NiTList<int>	groups;			// 0x18
};

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x006673f0 ??_GCharacterClass@@UAEPAXI@Z
class CharacterClass : public ObjectClass {
public:
	// The state flags (IsStateSet, SetFlags, ClearFlags).
	enum {
		CS_IDLE					= 0x000,
		CS_STANDING				= 0x001,
		CS_GLANCING				= 0x002,
		CS_TURNED				= 0x004,
		CS_UI_DISABLED			= 0x008,
		CS_HIDDEN				= 0x010,
		CS_INITIATIVE			= 0x020,
		CS_MIDDLE				= 0x040,
		CS_SEATED				= 0x080,
		CS_VISIBLE				= 0x100,
		CS_CLEAR_GLANCE			= 0x200,
		CS_CLEAR_TURNED			= 0x400,
		CS_UI_ENABLED			= 0x800,
		CS_STOP_INITIATIVE		= 0xFD8
	};

	// The animation categories (CharacterAnimation::type).
	enum {
		CAT_BREATHE,
		CAT_INTERRUPTABLE,
		CAT_NON_INTERRUPTABLE,
		CAT_TURN,
		CAT_TURN_BACK,
		CAT_GLANCE,
		CAT_GLANCE_BACK
	};

	// How a new animation meets one already playing or queued: it replaces
	// it, the two cancel out, it is dropped, or both play (the names are
	// invented).
	enum animationConflict_t {
		AC_REPLACE,
		AC_CANCEL,
		AC_DROP,
		AC_QUEUE
	};

	// The phoneme types.
	enum {
		CPT_DEFAULT				= -1,
		CPT_BLINK,
		CPT_SPEAK,
		CPT_EYEBROW
	};

	// The audio modes.
	enum {
		CAM_MUTE,
		CAM_EXTREMELY_VOCAL,
		CAM_VOCAL,
		CAM_REDUCED
	};

	// The genders.
	enum {
		MALE,
		FEMALE,
		MAX_GENDERS
	};

	// The sizes.
	enum {
		SMALL,
		MEDIUM,
		LARGE,
		MAX_SIZES
	};

					CharacterClass( long id = 0 );
	virtual			~CharacterClass( void );

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

	// BaseObjectClass
	virtual void	Update( float time );
	virtual const TGPoint3 &	GetWorldLocation( void );
	virtual void	AddToSet( SetClass *set );
	virtual void	RemoveFromSet( SetClass *set );

	// ObjectClass
	virtual void	VirtualSlot66( void );

	static CharacterClass *	Cast( TGObject *object );
	static bool		IsSomeoneSpeaking( void );
	void			ReplaceBodyAndHead( char *bodyTexture, char *headTexture_ );
	void			MorphBody( float, char * );
	void			SetYesSir( char * );
	float			GetHeadHeight( void );
	int				IsSpeaking( void );
	bool			IsReadyToSpeak( void );
	bool			IsAnimating( void );
	bool			IsGoingToAnimate( void );
	bool			IsAnimatingInterruptable( void );
	bool			IsAnimatingNonInterruptable( void );
	void			SetMenuEnabled( bool enabled );
	void			SetInitiative( bool initiative );
	void			AddAnimation( char *name_, char *file );
	void			ClearAnimations( void );
	void			ClearExtraAnimations( void );
	void			AddRandomAnimation( char *name_, int type, int weight, int unknown );
	void			SetBlinkAnimation( char * );
	void			SetActive( bool );
	void			ClearAnimationsOfType( int type );
	void			SetAnimationDoneEvent( TGAction *, long );
	void			SetCurrentAnimation( TGAction *action, int type, long flags_ = 0, char *name_ = NULL );
	bool			SetLocation( char * );
	void			SetLocationName( char * );
	int				IsStateSet( int flags_ );
	void			SetFlags( int flags_ );
	void			ClearFlags( int flags_ );
	void			SetStanding( int standing );
	void			AddSoundToQueue( TGAction *, int, int );
	bool			MoveTo( char *destination, TGEvent *event = NULL );
	bool			GlanceAt( char *target, TGEvent *event = NULL );
	bool			GlanceAway( TGEvent *event = NULL );
	bool			TurnTowards( char *target, TGEvent *event = NULL );
	bool			TurnBack( TGEvent *event = NULL );
	// The priority's sign picks how the animation plays (see the definitions).
	bool			PlayAnimation( char *name_, int priority = 0, TGEvent *event = NULL );
	bool			PlayAnimationFile( char *file, int priority = 0, TGEvent *event = NULL );
	// The meanings of the two int parameters the speaking action gets are open.
	bool			SpeakLine( TGLocalizationDatabase *database_, char *line, int unknown = 0, int unknown2 = 0, TGEvent *event = NULL );
	bool			SayLine( TGLocalizationDatabase *database_, char *line, char *target = NULL, int turnBack = 0, int unknown = 0,
							 int unknown2 = 0, int turnFirst = 0, TGEvent *event = NULL );
	void			Blink( void );
	// Not BaseObjectClass's virtual SetName: the wrapper calls it directly
	// (0x66C7C0), and it calls BaseObjectClass::SetName itself, so its
	// parameter type differs from the virtual's.
	void			SetName( const char *name_ );
	void			SetCharacterName( const char *characterName_ );
	void			AddPositionZoom( char *position, float zoom, char *lookAtName = NULL );
	float			GetPositionZoom( char *position );
	char *			GetPositionLookAtName( char *position );
	TGLocalizationDatabase *	SetDatabase( char * );
	void			SetStatus( TGString status, int index = 0 );
	void			ClearStatus( int index );
	TGParagraph *	GetStatus( int index );
	void			ToolTip( void );
	// Fits the status box again and lays its lines out anew (invented name).
	void			RefreshStatusBox( void );
	static CharacterClass *	GetCharacterFromMenu( STTopLevelMenu * );
	bool			MenuUp( void );
	bool			MenuDown( void );
	int				IsMenuUp( void );
	void			LookAtMe( void );
	// CharacterAction's AT_FORCE_BREATHE (invented name).
	void			ForceBreathe( void );
	// CharacterAction's AT_WATCH_ME and AT_STOP_WATCHING_ME (invented names).
	void			WatchMe( void );
	void			StopWatchingMe( void );
	// Update's step while watching: turns the camera to the head (invented name).
	void			UpdateWatching( float time );
	void			AddFacialImage( char *, char * );
	int				AddPhoneme( TGPhoneme *phoneme, int group, int type );
	static CharacterClass *	GetObject( SetClass *set, char *name_ );
	static CharacterClass *	GetObjectByID( SetClass *set, long id );			// invented name
	static int		GetNumClassObjects( SetClass *set );
	static CharacterClass **	GetClassObjects( SetClass *set, int *count );		// invented name
	static CharacterClass *	Create( char *bodyFile, char *headFile, bool unknown );
	// TODO identity: Create's set-up (0x667ED0): copies the body model
	// (g_kModelManager.CopyModel) and clones the head model, builds the
	// character's NetImmerse objects from them (0x667D60) and keeps copies
	// of the two names at 0x6C and 0x68; the int is not used
	void			Unknown667ED0( char *bodyFile, char *headFile, bool unknown );
	// TODO identity: Unknown667ED0's builder: hands the body to the object,
	// attaches the head, adds a RotateBonesController and sets up the
	// properties and the overlays
	void			Unknown667D60( TGAnimNode *body, TGAnimNode *head );
	// TODO identity: Unknown667D60's steps: fitting the head on the body
	// (0x668A00, which calls 0x668A20), replacing the body's vertex colour,
	// material, texture mode and specular properties (0x668000) and loading
	// a model file under the body (0x668250)
	void			Unknown668A00( TGAnimNode *body, TGAnimNode *head );
	void			Unknown668A20( TGAnimNode *body, NiAVObject *head );
	// The first NiBone at or under the object, depth first (invented name).
	NiBone *		FindBone( NiAVObject *object );
	// The object's first skin controller (invented name).
	NiSkinController *	FindSkinController( NiAVObject *object );
	void			Unknown668000( TGAnimNode *body, TGAnimNode *head );
	void			Unknown668250( TGAnimNode *body );

	// Register the class's handler methods by name, and add them to its table.
	static void		RegisterHandlerNames( void );		// invented name
	static void		RegisterHandlers( void );			// invented name

	static void		ShowPhonemeDebugging( bool show ) { TGOverlayManager::showPhonemeDebugging = show; }

	bool			Breathe( TGEvent *event );

	void			AnimationDoneHandler( TGEvent *event );
	void			MenuEventHandler( TGEvent *event );

	// The scripts' inline accessors (the SWIG wrappers).
	char *			GetYesSir( void ) { return yesSir; }
	int				GetSize( void ) { return size; }
	void			SetSize( int size_ ) { size = size_; }
	int				GetGender( void ) { return gender; }
	void			SetGender( int gender_ ) { gender = gender_; }
	int				GetAudioMode( void ) { return audioMode; }
	void			SetAudioMode( int audioMode_ ) { audioMode = audioMode_; }
	char *			GetCharacterName( void ) { return characterName; }
	char *			GetLocation( void ) { return location; }
	char *			GetTurnTowardsLocation( void ) { return turnTowardsLocation; }
	float			GetBlinkChance( void ) { return blinkChance; }
	void			SetBlinkChance( float blinkChance_ ) { blinkChance = blinkChance_; }
	bool			IsAnExtra( void ) { return extra; }
	void			SetAsExtra( bool extra_ ) { extra = extra_; }
	bool			IsActive( void ) { return active; }
	TGPane *		GetStatusBox( void ) { return statusBox; }
	bool			IsRandomAnimationEnabled( void ) { return randomAnimationEnabled; }
	void			SetRandomAnimationEnabled( bool enabled ) { randomAnimationEnabled = enabled; }
	float			GetRandomAnimationChance( void ) { return randomAnimationChance; }
	void			SetRandomAnimationChance( float chance ) { randomAnimationChance = chance; }
	TGLocalizationDatabase *	GetDatabase( void ) { return database; }
	TGOverlayManager *	GetOverlayManager( void ) { return overlayManager; }	// invented name
	STTopLevelMenu *	GetMenu( void ) { return menu; }
	void			SetMenu( STTopLevelMenu *menu_ ) { menu = menu_; }
	void			SetBlinkStages( int blinkStages_ ) { blinkStages = blinkStages_; }
	bool			UsesAnimatedSpeaking( void ) { return animatedSpeaking; }
	void			SetAnimatedSpeaking( bool animatedSpeaking_ ) { animatedSpeaking = animatedSpeaking_; }
	void			UsePhonemeGroup( int phonemeGroup_ ) { phonemeGroup = phonemeGroup_; }
	float			GetLastTalkTime( void ) { return lastTalkTime; }
	int				IsMenuEnabled( void ) { return menuEnabled; }	// an int: the SWIG wrapper zero-extends the result

protected:
	// Takes back what the animation's category set, as it ends (invented name).
	void			AnimationEnded( CharacterAnimation *animation );
	// Whether the animation may start now (invented name).
	bool			CanPlayAnimation( CharacterAnimation *animation );
	// Drops the current animation when its action is gone or is the given
	// one (invented name).
	void			ClearDoneAnimation( TGAction *action );
	// How the new animation meets the existing one (invented name).
	animationConflict_t	ResolveConflict( CharacterAnimation *existing, CharacterAnimation *added );
	// How the new animation meets the current one (invented name).
	animationConflict_t	ResolveConflictWithCurrent( CharacterAnimation *added );
	// The action a "module.function" script name makes (invented name).
	TGAction *		CreateScriptAction( char *function );
	// The sequence that speaks a line, with the turns around it (invented name).
	TGSequence *	CreateLineSequence( TGLocalizationDatabase *database_, char *line, char *target, int turnBack, int unknown,
										int unknown2, int turnFirst );
	// Whether the current animation carries one of the flags (invented name).
	bool			IsCurrentAnimationFlagSet( int flags_ );
	// A sequence playing one of the random animations, by weight (invented name).
	TGSequence *	CreateRandomAnimation( void );
	// The state an animation sets as it starts (invented name).
	void			StartAnimation( CharacterAnimation *animation );
	// Starts the first queued animation when nothing plays (invented name).
	void			PlayNextAnimation( void );
	// Append the glance-away and turn-back animations to their sequences
	// (invented names).
	bool			AppendGlanceAway( CharacterAnimation *animation );
	bool			AppendTurnBack( CharacterAnimation *animation );

	char *			unknown68;			// 0x68 owned model name, referred through g_kModelManager
	char *			unknown6C;			// 0x6C owned model name, referred through g_kModelManager
	char *			headTexture;		// 0x70 owned: the head's texture file, ReplaceBodyAndHead's (invented name)
	char *			yesSir;				// 0x74
	int				size;				// 0x78 SMALL, MEDIUM or LARGE
	int				gender;				// 0x7C MALE or FEMALE
	int				flags;				// 0x80 CS_* (invented name)
	int				audioMode;			// 0x84 CAM_*
	char *			characterName;		// 0x88 owned copy
	CharacterSpeakingQueue *	speakingQueue;	// 0x8C (invented name)
	TGStringToStringMap *	animations;	// 0x90 owned: the animation files by name (invented name)
	bool			watching;			// 0x94 set by WatchMe, cleared by StopWatchingMe (invented name)
	float			unknown98;			// 0x98 0.5 at first
	char *			location;			// 0x9C
	char *			turnTowardsLocation;	// 0xA0
	char *			glanceAtLocation;	// 0xA4 owned: where a glance looks; freed when a CAT_GLANCE_BACK animation ends (invented name)
	int				numPositionZooms;	// 0xA8 (invented name)
	PositionZoom *	positionZooms;		// 0xAC heap block of numPositionZooms (invented name)
	RotateBonesController *	rotateBonesController;	// 0xB0 turns the model's bones (invented name)
	float			blinkDelay;			// 0xB4 seconds to the next blink (invented name)
	float			blinkChance;		// 0xB8 0.1 at first
	char *			blinkAnimation;		// 0xBC owned copy (invented name)
	float			unknownC0;			// 0xC0 -2 once a glance or turn back starts
	bool			extra;				// 0xC4
	float			unknownC8;			// 0xC8
	bool			active;				// 0xCC
	float			unknownD0;			// 0xD0 -1 at first
	TGPane *		statusBox;			// 0xD4
	NiTMap<int, TGParagraph *>	statuses;	// 0xD8 by index (invented name)
	NiTArray<RandomAnimation *>	unknownE8;	// 0xE8 the CAT_NON_INTERRUPTABLE random animations
	int				unknown100;			// 0x100 the total of unknownE8's weights
	NiTArray<RandomAnimation *>	unknown104;	// 0x104 the CAT_INTERRUPTABLE random animations
	int				unknown11C;			// 0x11C the total of unknown104's weights
	NiTArray<RandomAnimation *>	unknown120;	// 0x120 the other random animations
	int				unknown138;			// 0x138 the total of unknown120's weights
	bool			randomAnimationEnabled;	// 0x13C
	float			randomAnimationChance;	// 0x140
	float			unknown144;			// 0x144
	TGLocalizationDatabase *	database;	// 0x148
	STTopLevelMenu *	menu;			// 0x14C
	NiPoint3		lookAtAdj;			// 0x150
	CharacterAnimation *	currentAnimation;	// 0x15C (invented name)
	NiTList<CharacterAnimation *>	queuedAnimations;	// 0x160 (invented name)
	int				blinkStages;		// 0x178 -1 at first
	bool			animatedSpeaking;	// 0x17C
	NiPointer<TGOverlayController>	overlayController;	// 0x180 the facial images (invented name)
	NiTStringMap<char *>	facialImages;	// 0x184 owned image names, by name (AddFacialImage; invented name)
	NiPointer<TGOverlayManager>	overlayManager;	// 0x194 (invented name)
	NiTMap<int, PhonemeList *>	phonemeLists;	// 0x198 owned, by phoneme type (invented name)
	int				phonemeGroup;		// 0x1A8
	float			lastTalkTime;		// 0x1AC
	bool			menuEnabled;		// 0x1B0

	static TGEventHandlerTable	classHandlers;		// invented name
	// The extra walking on to or off the bridge; NULL when none is (invented name).
	static CharacterClass *	walkingExtra;
	// The character whose tool tip is shown (the SWIG wrappers
	// Set/GetCurrentToolTipOwner; invented name).
	static CharacterClass *	currentToolTipOwner;
	// Whether the extras walk on and off the bridge, and the chance per
	// second that one starts to (Update 0x669680; saved with the
	// characters; invented names).
	static bool		extraWalkingEnabled;
	static float	extraWalkChance;
	// Whether the tool tips and the collision alert are on, and the volume
	// of each line type's speech (invented names).
	static bool		toolTipsEnabled;
	static bool		collisionAlertEnabled;
	static float	lineVolumes[3];

	// The state tests the SWIG wrappers expose (CharacterClass_IsStanding and the like).
public:
	int	IsStanding( void ) { return IsStateSet( CS_STANDING ); }
	int	IsTurned( void ) { return IsStateSet( CS_TURNED ); }
	int	IsUIDisabled( void ) { return IsStateSet( CS_UI_DISABLED ); }
	int	IsGlancing( void ) { return IsStateSet( CS_GLANCING ); }
	int	IsInitiativeOn( void ) { return IsStateSet( CS_INITIATIVE ); }

	void	SetLookAtAdj( float x, float y, float z ) { lookAtAdj = NiPoint3( x, y, z ); }
	static void		SetAllowExtras( bool allow ) { extraWalkingEnabled = allow; }
	static void		SetWalkOnChance( float chance ) { extraWalkChance = chance; }
	static float	GetWalkOnChance( void ) { return extraWalkChance; }
	static void		SetToolTipsEnabled( bool enabled ) { toolTipsEnabled = enabled; }
	static bool		AreToolTipsEnabled( void ) { return toolTipsEnabled; }
	static void		SetCollisionAlertEnabled( bool enabled ) { collisionAlertEnabled = enabled; }
	static bool		IsCollisionAlertEnabled( void ) { return collisionAlertEnabled; }
	static void		SetVolumeForLineType( int lineType, float volume ) { lineVolumes[lineType] = volume; }
	static float	GetVolumeForLineType( int lineType ) { return lineVolumes[lineType]; }
	static void		SetCurrentToolTipOwner( CharacterClass *owner ) { currentToolTipOwner = owner; }
	static CharacterClass *	GetCurrentToolTipOwner( void ) { return currentToolTipOwner; }
};

#endif /* !__CHARACTERCLASS_H__ */
