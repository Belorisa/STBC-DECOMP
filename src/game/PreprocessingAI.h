/*
===============================================================================

	PreprocessingAI.h

	An AI that runs a script's preprocessing method before the AI it
	contains: the method's status lets the contained AI update, skips it
	(active or dormant) or ends. The script may also have Activate,
	Deactivate, GetNextUpdateTime, Reset, GotFocus, LostFocus and
	GetStatusInfo, which are called when present. Some preprocessing
	classes have optimized C++ versions: the AI manager files the version
	VirtualSlot13 makes instead of the scripted one.

	0x4C bytes: the create function (0x48F1D0) allocates that much.

	Evidence: the constructor (0x48E2B0) and the destructor (0x48E340)
	store the vtable 0x88C12C (23 slots: ArtificialIntelligence's 21,
	GetPreprocessingInstance and the preprocessing call); the log strings
	"PreprocessingAI(%s)::SetActive", "::SetInactive", "::GotFocus",
	"::LostFocus", "::Update" and "::IsDormant (%s)" name the overrides.
	The SWIG wrappers PreprocessingAI_Cast (0x48E3E0) and SetContainedAI
	(0x48E570) are direct, GetPreprocessingInstance calls slot 21, and
	ForceUpdate (0x3C), ForceDormantStatus (0x40), ForceStatusChange
	(0x44) and GetContainedAI (0x48) are inline. The statuses are the SWIG
	constants PS_* and FDS_*. The Python methods "SetPreprocessingMethod"
	and "GetPreprocessingInstance" of the shadow class are 0x48EE90 and
	0x48EF10. The optimized versions are filed by class name in the list
	at 0x982A1C ("AvoidObstacles" by 0x47F1B0, three more alike).

===============================================================================
*/

#ifndef __PREPROCESSINGAI_H__
#define __PREPROCESSINGAI_H__

#include <string.h>

#include "ArtificialIntelligence.h"
#include "PyEmbed.h"

class PreprocessingAI;

// Makes the optimized version of a scripted preprocessing AI.
typedef ArtificialIntelligence *( *optimizedVersionFunction_t )( PreprocessingAI *ai );

// An optimized version filed under the script class it replaces. The
// name is invented.
struct OptimizedVersion {
	char *						className;	// a new[] copy
	optimizedVersionFunction_t	create;
	OptimizedVersion *			next;
};

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
class PreprocessingAI : public ArtificialIntelligence {
public:
	// What the preprocessing method returns (the SWIG constants PS_*).
	enum preprocessStatus_t {
		PS_NORMAL,						// update the contained AI
		PS_SKIP_ACTIVE,					// the AI is active, the contained AI is not updated
		PS_SKIP_DORMANT,				// the AI is dormant, the contained AI is not updated
		PS_DONE,
		PS_INVALID,
		PS_NUM_STATUSES
	};

	// Overrides of IsDormant (the SWIG constants FDS_*).
	enum forcedDormantStatus_t {
		FDS_NORMAL,
		FDS_TRUE,
		FDS_FALSE
	};

					PreprocessingAI( PhysicsObjectClass *object, char *name_ );
					PreprocessingAI( TGStream *stream, bool *ok );
	virtual			~PreprocessingAI( void );

	// ArtificialIntelligence
	virtual bool	IsInterruptable( void );
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
	// The optimized version of the script's class, if one is filed.
	virtual ArtificialIntelligence *	VirtualSlot13( void );
	virtual bool	IsDormant( void );
	virtual PyObject *	VirtualSlot15( void );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	FixUpPointers( void );
	virtual void	PostFixUp( void );
	virtual int		GetAIType( void );
	virtual bool	VirtualSlot20( int type );

	virtual PyObject *	GetPreprocessingInstance( void );
	// Calls the preprocessing method when its time has come; returns the
	// preprocess status, the last one in between (invented name).
	virtual int		CallPreprocessingMethod( double time );

	// The AI, if it is a PreprocessingAI; NULL otherwise.
	static PreprocessingAI *	Cast( ArtificialIntelligence *ai );
	// The AI it contains, which it owns; the contained AI's parent becomes
	// this one.
	void			SetContainedAI( ArtificialIntelligence *ai );
	// The script instance and the name of its preprocessing method, which
	// is called with the update's time.
	void			SetPreprocessingMethod( PyObject *instance, char *method );

	ArtificialIntelligence *	GetContainedAI( void ) { return containedAI; }
	void			ForceUpdate( void ) { forceUpdate = true; }
	void			ForceDormantStatus( int forcedDormantStatus_ ) { forcedDormantStatus = forcedDormantStatus_; }
	void			ForceStatusChange( int preprocessStatus_ ) { preprocessStatus = preprocessStatus_; }

	// Files the optimized version for the script class, unless the class
	// has one (invented name).
	static void		RegisterOptimizedVersion( char *className, optimizedVersionFunction_t create );

	// The stream constructor, for CreateFromStream.
	static ArtificialIntelligence *	Create( TGStream *stream );
	// The script-side constructor (the SWIG wrapper PreprocessingAI_Create calls it).
	// FUNCTION: STBC 0x006050c0 ?Create@PreprocessingAI@@SAPAV1@PAVPhysicsObjectClass@@PAD@Z
	static PreprocessingAI *	Create( PhysicsObjectClass *object, char *name ) { return new PreprocessingAI( object, name ); }

protected:
	// The optimized versions take over the scripted AI's state.
	friend class OptimizedAvoidObstacles;
	friend class OptimizedFireScript;
	friend class OptimizedManagePower;
	friend class OptimizedSelectTarget;

	PyObject *		preprocessingInstance;	// 0x28 a reference held
	char *			preprocessingMethod;	// 0x2C a new[] copy
	bool			hasActivate;			// 0x30 the script has the method
	bool			hasDeactivate;			// 0x31
	bool			hasGetNextUpdateTime;	// 0x32
	bool			hasReset;				// 0x33
	bool			hasGotFocus;			// 0x34
	bool			hasLostFocus;			// 0x35
	bool			hasGetStatusInfo;		// 0x36
	float			nextUpdateTime;			// 0x38 game time of the next preprocessing call
	bool			forceUpdate;			// 0x3C the next update calls the method (ForceUpdate)
	int				forcedDormantStatus;	// 0x40 a forcedDormantStatus_t (ForceDormantStatus)
	int				preprocessStatus;		// 0x44 the last preprocess status (ForceStatusChange)
	ArtificialIntelligence *	containedAI;	// 0x48 owned (GetContainedAI)

	static OptimizedVersion *	optimizedVersions;	// newest first (invented name)
};

/*
================
PreprocessingAI::VirtualSlot15
================
*/
// FUNCTION: STBC 0x0047f2d0
inline PyObject *PreprocessingAI::VirtualSlot15( void ) {
	return PyEmbed::Unknown6F9920( this, "PreprocessingAI" );
}

/*
================
PreprocessingAI::VirtualSlot20

BuilderAI's (0x476120) inlines it.
================
*/
// FUNCTION: STBC 0x0048e310 @game/PreprocessingAI
inline bool PreprocessingAI::VirtualSlot20( int type ) {
	return type == AI_TYPE_PREPROCESSING ? true : false;
}

/*
================
PreprocessingAI::GetPreprocessingInstance
================
*/
// FUNCTION: STBC 0x004760f0
inline PyObject *PreprocessingAI::GetPreprocessingInstance( void ) {
	return preprocessingInstance;
}

/*
================
PreprocessingAI::RegisterOptimizedVersion

Evidence: each optimized class's unit runs this body inline at startup
with the script class name and its function.
================
*/
inline void PreprocessingAI::RegisterOptimizedVersion( char *className, optimizedVersionFunction_t create ) {
	OptimizedVersion *entry;
	for ( entry = optimizedVersions; entry != NULL; entry = entry->next ) {
		if ( strcmp( entry->className, className ) == 0 ) {
			if ( entry->create != NULL ) {
				return;
			}
			break;
		}
	}
	entry = new OptimizedVersion;
	entry->className = new char[strlen( className ) + 1];
	strcpy( entry->className, className );
	entry->create = create;
	entry->next = optimizedVersions;
	optimizedVersions = entry;
}

// Files an optimized version when the program starts (invented name).
class OptimizedVersionRegistration {
public:
					OptimizedVersionRegistration( char *className, optimizedVersionFunction_t create ) {
						PreprocessingAI::RegisterOptimizedVersion( className, create );
					}
};

// The Python methods of the shadow class.
PyObject *	PreprocessingAI_SetPreprocessingMethod( PyObject *self, PyObject *args );
PyObject *	PreprocessingAI_GetPreprocessingInstance( PyObject *self, PyObject *args );

#endif /* !__PREPROCESSINGAI_H__ */
