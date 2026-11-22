/*
===============================================================================

	LensFlare.h

	A set's lens flare: images placed along the line from a source object
	through the middle of the view. The SWIG wrappers LensFlare_* call
	Create, SetSource, AddFlare and Build.

	0x28 bytes, from operator new (Create, 0x4E1CD0). Evidence: the
	constructor (0x4E2B10) builds the flare list with 2 items a block,
	clears the node at 0x24, keeps the set at 0x18 and adds the lens flare
	to a static list of them (an NiTList at 0x9861E0, "Fixing up LensFlares
	statics"); the destructor (0x4E2C30) takes it out of that list, deletes
	the flares, detaches the node from its parent and releases it.
	SetSource (0x4E2D50) keeps the source's ID at 0x20 and its second
	argument at 0x1C and copies both into the node (0x134 and 0x130).
	AddFlare (0x4E3050) appends a 0x18-byte flare; the stream functions
	(0x4E2D80, 0x4E2E30) save the set's ID, the source, the count and each
	flare, and the fix-up (0x4E3030) finds the set and calls Build.

	The statics: the list's dynamic initialiser and its destructor
	(0x4E1B20-0x4E1B60), DeleteAll (0x4E1BB0, which Mission::Terminate
	calls), and the save, load and fix-up of every lens flare (0x4E1BE0,
	0x4E1C30, 0x4E1CA0), which UtopiaModule's save (0x443AC0) and load
	(0x444840) call; the load deletes the old flares and makes each new one
	with the inline constructor before reading it.

	The node at 0x24 is a LensFlareNode (below). Build (0x4E31B0) also
	makes a fan for each image and, for an image that wobbles, a
	LensFlareController (below) on it.

===============================================================================
*/

#ifndef __LENSFLARE_H__
#define __LENSFLARE_H__

#include "NiTList.h"
#include "NiPointer.h"
#include "NiNode.h"
#include "NiPick.h"
#include "NiMaterialProperty.h"
#include "NiTimeController.h"
#include "NiMath.h"

class NiCamera;
class NiImage;
class ObjectClass;
class SetClass;
class TGStream;

// One image of a lens flare, as AddFlare gives it (invented name; the
// meanings of the numbers are not recovered yet).
struct LensFlareImage {
	NiPointer<NiImage>	image;			// 0x00 from g_kImageManager by name
	int					unknown04;		// 0x04 AddFlare's first argument
	float				unknown08;		// 0x08
	float				unknown0C;		// 0x0C
	float				unknown10;		// 0x10
	float				unknown14;		// 0x14
};

/*
================
LensFlareController

Wobbles a lens flare image's fan: each rim vertex sits at a radius of
1 + amplitude * sin( rate * time + its angle ). 0x3C bytes, made by Build
with the image's rate and amplitude. The vtable 0x88D3C0 keeps
NiTimeController's 20 slots and its RTTI, with the destructor (0x4E3420),
Update (0x4E1D30) and TargetIsRequiredType (0x4E3410, always true)
replaced. Nothing names the class or its members: the names are
invented.
================
*/
// FUNCTION: STBC 0x004e3420 ??_GLensFlareController@@UAEPAXI@Z @game/LensFlare
// FUNCTION: STBC 0x004e3440 ??1LensFlareController@@UAE@XZ @game/LensFlare
class LensFlareController : public NiTimeController {
public:
	virtual void	Update( float time );
	// FUNCTION: STBC 0x004e3410 ?TargetIsRequiredType@LensFlareController@@UBE_NXZ @game/LensFlare
	virtual bool	TargetIsRequiredType() const { return true; }

					LensFlareController( float rate_, float amplitude_ ) {
						lastUpdate = -NI_INFINITY;
						rate = rate_;
						amplitude = amplitude_;
					}

protected:
	float			lastUpdate;		// 0x30 the time of the last update
	float			rate;			// 0x34 radians a second
	float			amplitude;		// 0x38 of the rim's radius
};

/*
================
LensFlareNode

The node a lens flare draws its images through (invented name: the class
has no RTTI or strings of its own). 0x140 bytes: Build allocates it and
calls the constructor (0x4E1EA0) with the lens flare's set. The
constructor runs NiNode's, stores the vtable 0x88D310 (NiNode's 44 slots:
the destructor, UpdateDownwardPass 0x4E1E40, slot 35 0x4E22F0 and
UpdateWorldBound 0x4E1E70 overridden; NiNode's RTTI is kept), makes a
material at 0xE8 and four more properties, sets up the pick at 0xF0 and
sets 0x128 to 1.0.
================
*/
// FUNCTION: STBC 0x004e2230 ??_GLensFlareNode@@UAEPAXI@Z @game/LensFlare
class LensFlareNode : public NiNode {
public:
					LensFlareNode( SetClass *set );
	virtual			~LensFlareNode();

	// NiAVObject
	virtual void	UpdateDownwardPass( float time );
	virtual void	VirtualSlot35( NiCamera *camera );
	virtual void	UpdateWorldBound();

	// LensFlare::SetSource passes its source on (invented name).
	void			SetSource( long sourceID_, int unknown130_ ) { sourceID = sourceID_; unknown130 = unknown130_; }

protected:
	// LensFlare::Build fills the per-flare values
	friend class LensFlare;

	NiPointer<NiMaterialProperty>	material;	// 0xE8 attached to the node (invented name)
	SetClass *		set;					// 0xEC the constructor's argument (invented name)
	NiPick			pick;					// 0xF0 (invented name)
	float			unknown128;				// 0x128 1.0 after construction
	float			unknown12C;				// 0x12C the time of the last UpdateDownwardPass
	int				unknown130;				// 0x130 LensFlare::SetSource's second argument
	long			sourceID;				// 0x134 LensFlare::SetSource's object (invented name)
	float *			unknown138;				// 0x138 owned: each image's AddFlare third argument
	float *			unknown13C;				// 0x13C owned: each image's AddFlare fourth argument
};

class LensFlare {
public:
					LensFlare( void ) : images( 2 ) {}	// the load makes each flare with it before reading it
					LensFlare( SetClass *set_ );
					~LensFlare( void );

	static LensFlare *	Create( SetClass * );
	void	SetSource( ObjectClass *, int );
	void	AddFlare( int, char *, float, float, float, float );
	void	Build( void );

	bool			WriteToStream( TGStream *stream );		// invented name
	bool			ReadFromStream( TGStream *stream );		// invented name
	bool			FixUp( void );							// invented name

	// Every lens flare (invented names).
	static void		DeleteAll( void );
	static bool		SaveStatics( TGStream *stream );
	static bool		LoadStatics( TGStream *stream );
	static bool		FixUpStatics( void );

protected:
	NiTList<LensFlareImage *>	images;	// 0x00 owned, in AddFlare's order (invented name)
	SetClass *			set;			// 0x18 an ID between the stream read and the fix-up (invented name)
	int					unknown1C;		// 0x1C SetSource's second argument
	long				sourceID;		// 0x20 SetSource's object (invented name)
	NiPointer<LensFlareNode>	node;	// 0x24 the node Build makes (invented name)
};

#endif /* !__LENSFLARE_H__ */
