/*
===============================================================================

	NiKeyframeManager.h

	A controller that keeps named animation sequences and plays, blends and
	morphs them on its target.

	Evidence: the NiRTTI at 0x9A03E0 is named "NiKeyframeManager" with
	NiTimeController's as parent; Register (0x7A14A0) registers CreateObject
	(0x7A3F80). The vtable at 0x898490 has NiTimeController's 20 slots and
	three more (0x7A5B10, 0x7A5B40, 0x7A5C00); CreateClone (0x7A3EB0)
	allocates 0x40 bytes and builds a string map of 37 buckets at 0x30
	(NiTMap vptr 0x898500, NiTStringMap vptr 0x8984EC). The destructor
	(0x7A3200) destroys and frees every sequence of the map. The log strings
	"Attempting to blend a sequence that's already animating", "Cannot
	activate a blending sequence" and the morphing ones name the sequence
	operations.

===============================================================================
*/

#ifndef __NIKEYFRAMEMANAGER_H__
#define __NIKEYFRAMEMANAGER_H__

#include "NiTimeController.h"
#include "NiKeyframeController.h"
#include "NiAVObject.h"
#include "NiTextKeyExtraData.h"
#include "NiTStringMap.h"
#include "NiStreamMacros.h"

/*
================
NiKeyframeManagerLoadRecord

What NiKeyframeManager::LoadBinary reads for LinkObject: per sequence its
name, the 0x40 value and the text keys' link ID, and per controller of the
sequence the node name and the controller's link ID. The record's vtable is
0x898514. TODO identity: the element types. Each array's vtable has its own
scalar deleting destructor (0x7A54F0, 0x7A5520, 0x7A5550): the sequence
values and the controller counts share the first, the text key link IDs
have the second and the controller link IDs the third, so none is the
unsigned int, unsigned long or int of the other records. The class name is
invented.
================
*/
// FUNCTION: STBC 0x7A4110 ??_GNiKeyframeManagerLoadRecord@@UAEPAXI@Z
// FUNCTION: STBC 0x7A4130 ??1NiKeyframeManagerLoadRecord@@UAE@XZ
class NiKeyframeManagerLoadRecord : public NiTimeControllerLoadRecord {
public:
	NiTArray<char *>			sequenceNames;		// 0x10 owned strings
	NiTArray<long>				sequenceValues;		// 0x28 the 0x40 value of each sequence
	NiTArray<NiTextKeyExtraData *>	textKeyLinkIDs;	// 0x40 the sequences' 0x3C link IDs
	NiTArray<long>				controllerCounts;	// 0x58 controllers per sequence
	NiTArray<char *>			controllerNames;	// 0x70 owned strings, the controllers in sequence order
	NiTArray<NiKeyframeController *>	controllerLinkIDs;	// 0x88 the controllers' link IDs
};

// FUNCTION: STBC 0x7A5B50 ??_GNiKeyframeManager@@UAEPAXI@Z
class NiKeyframeManager : public NiTimeController {
public:
	/*
	================
	NiKeyframeManager::Sequence

	One named animation of the manager. The constructor (0x7A15F0) copies
	the name into a new block at 0x00 and builds the two arrays at 0x0C and
	0x24 with the size and growth it is given; the destructor (0x7A1890)
	frees the name and the block at 0x04, every element of the first array,
	and releases the second array's objects and the object at 0x3C. Update
	(0x7A3E10) picks the per-frame step by the state at 0x48. TODO identity:
	the arrays' element types and the members without name evidence.
	================
	*/
	class Sequence {
	public:
		// the states Update steps; the names are invented from the log
		// strings ("Attempting to blend a sequence that's already
		// animating", "Cannot activate a blending sequence", the morphing
		// ones)
		enum sequenceState_t {
			INACTIVE			= 0,
			ANIMATING			= 1,
			BLENDING			= 2,
			BLEND_DESTINATION	= 3,	// the sequence a blend goes to (name invented)
			MORPHING			= 4,
			MORPH_DESTINATION	= 5		// the sequence a morph goes to (name invented)
		};

								Sequence( const char *name_, unsigned int maxSize, unsigned int growBy );
								~Sequence();

		// a new sequence from a sequence file (name invented)
		static Sequence *		Load( const char *filename_, const char *name_, unsigned int index );
		// a new sequence with the source's name, file, text keys and clones
		// of its controllers (name invented; nothing in retail calls it)
		static Sequence *		Copy( Sequence *source );
		void					SetFilename( const char *filename_ );

		// the file the sequence was loaded from (AnimOnly.exe 0x492EE0; name
		// invented)
		const char *			GetFilename() const { return filename; }
		float					GetOffset() const { return offset; }
		void					SetOffset( float offset_ ) { offset = offset_; }
		NiTextKeyExtraData *	GetTextKeys() const { return textKeys; }
		// the state, the other sequence of a blend or morph and the time it
		// started or ends (names invented): AnimOnly.exe's debug copies
		// 0x492F40, 0x492F60, 0x492F80, 0x492FA0, 0x493010 and 0x493030
		// (NiKeyframeManager.cpp), which the manager's blends and morphs call
		sequenceState_t			GetState() const { return state; }
		void					SetState( sequenceState_t state_ ) { state = state_; }
		Sequence *				GetPartner() const { return unknown50; }
		void					SetPartner( Sequence *partner ) { unknown50 = partner; }
		float					GetUnknown4C() const { return unknown4C; }
		void					SetUnknown4C( float time ) { unknown4C = time; }
		// the text keys the sequence matches (Gamebryo lineage name)
		void					SetTextKeys( NiTextKeyExtraData *keys ) { textKeys = keys; }
		unsigned int			GetUnknown08() const { return unknown08; }
		void					SetUnknown08( unsigned int value ) { unknown08 = value; }
		unsigned int			GetUnknown40() const { return unknown40; }
		void					SetUnknown40( unsigned int value ) { unknown40 = value; }
		// the sequence's name and the names of the nodes its controllers
		// play on (names invented): AnimOnly.exe's debug copies 0x492C60,
		// 0x492C70, 0x492D50, 0x492F20 and 0x492E50 (NiKeyframeManager.cpp),
		// which Copy calls
		const char *			GetName() const { return name; }
		unsigned int			GetNodeCount() const { return unknown0C.GetSize(); }
		char *					GetNodeName( unsigned int index ) const { return unknown0C.GetAt( index ); }

		// FUNCTION: STBC 0x7A5BC0 ?GetUnknown40Controller@Sequence@NiKeyframeManager@@QAEPAVNiKeyframeController@@XZ
		NiKeyframeController *	GetUnknown40Controller() { return controllers.GetAt( unknown40 ); }

		// FUNCTION: STBC 0x7A5B90 ?GetController@Sequence@NiKeyframeManager@@QAE?AV?$NiPointer@VNiKeyframeController@@@@I@Z
		NiPointer<NiKeyframeController>	GetController( unsigned int index ) { return controllers.GetAt( index ); }

		// the per-frame steps for each state
		void					Unknown7A2A80( float time );	// ANIMATING: updates at time plus the offset
		void					Unknown7A2AA0( float time );	// BLENDING: blends toward the sequence at 0x50
		void					Unknown7A2B40( float time );	// MORPHING

		// TODO identity: binds the controllers to the target's nodes named
		// for them, from the first (0x7A2170), and lets them go from the last
		// (0x7A21F0); false when a node is missing, after undoing the ones
		// done
		bool					Unknown7A2170();
		bool					Unknown7A21F0();
		// the walks behind them: the object and then its children in order,
		// each node named unknown0C[index] taking controllers[index]
		// (0x7A2270); the children last first and then the object (0x7A24B0)
		bool					Unknown7A2270( NiAVObject *object, unsigned int &index );
		bool					Unknown7A24B0( NiAVObject *object, unsigned int &index );
		// TODO identity: a new sequence merging the controllers of two
		// (0x7A1DB0), and the walk that clones them over the target (0x7A2690)
		static Sequence *		Unknown7A1DB0( Sequence *first, Sequence *second, const char *name_ );
		// TODO identity: the time until the sequence next reaches the text
		// key of that name (0x7A1FE0)
		float					Unknown7A1FE0( const char *name_, float time );
		bool					Unknown7A2690( NiAVObject *object, unsigned int &firstIndex, unsigned int &secondIndex, Sequence *first, Sequence *second );
		// a manager takes a sequence only while it has none
		bool					SetManager( NiKeyframeManager *manager_ ) {
									if ( manager ) {
										return false;
									}
									manager = manager_;
									return true;
								}

		float					Unknown7A2BF0( Sequence *destination, float time );	// this sequence's time matching the destination's time
		void					Unknown7A2E30( float time );	// updates the sequence's controllers at time
		unsigned int			Unknown7A1EB0( const char *nodeName, NiKeyframeController *controller );	// adds the controller under a copy of the node name
		void					Unknown7A2EC0( float time, float weight );	// the same, weighted against what is already there

		char *					name;				// 0x00 own copy
		char *					filename;			// 0x04 the file the sequence came from, standardized
		unsigned int			unknown08;			// 0x08
		NiTArray<char *>		unknown0C;			// 0x0C elements freed by the destructor
		NiTArray<NiPointer<NiKeyframeController> >	controllers;	// 0x24 the controllers the sequence plays
		NiPointer<NiTextKeyExtraData>	textKeys;	// 0x3C the "morph:" keys Unknown7A2BF0 matches
		unsigned int			unknown40;			// 0x40
		NiKeyframeManager *		manager;			// 0x44 the manager that owns the sequence, once added (name invented)
		sequenceState_t			state;				// 0x48
		float					unknown4C;			// 0x4C a time the blend interpolates from
		Sequence *				unknown50;			// 0x50 the sequence a blend goes to
		float					offset;				// 0x54 added to the time (Gamebryo lineage name)
	};

	// FUNCTION: STBC 0x7A5AF0 ?GetRTTI@NiKeyframeManager@@UBEPBVNiRTTI@@XZ
	virtual const NiRTTI *	GetRTTI() const { return &rtti; }

	// cloning
	virtual NiObject *		CreateClone();
	virtual void			ProcessClone( NiObject *clone );

	// streaming
	virtual void			LoadBinary( NiStream &stream );
	virtual void			LinkObject( NiStream &stream );
	virtual bool			RegisterStreamables( NiStream &stream );
	virtual void			SaveBinary( NiStream &stream );
	virtual bool			IsEqual( NiObject *object );

	virtual					~NiKeyframeManager();

	// FUNCTION: STBC 0x7A5B00 ?Start@NiKeyframeManager@@UAEXM@Z
	virtual void			Start( float time ) {}
	// FUNCTION: STBC 0x7A5B20 ?Stop@NiKeyframeManager@@UAEXXZ
	virtual void			Stop() {}
	virtual void			Update( float time );
	// FUNCTION: STBC 0x7A5B30 ?VirtualSlot15@NiKeyframeManager@@UAEXXZ
	virtual void			VirtualSlot15() {}
	// FUNCTION: STBC 0x7A5C10 ?TargetIsRequiredType@NiKeyframeManager@@UBE_NXZ
	// the target must be an NiAVObject
	virtual bool			TargetIsRequiredType() const {
								return NiIsKindOf( NiAVObject, target );
							}

	// FUNCTION: STBC 0x7A5B10 ?VirtualSlot20@NiKeyframeManager@@UAEXXZ
	virtual void			VirtualSlot20() {}
	// FUNCTION: STBC 0x7A5B40 ?VirtualSlot21@NiKeyframeManager@@UAEXXZ
	virtual void			VirtualSlot21() {}
	// FUNCTION: STBC 0x7A5C00 ?VirtualSlot22@NiKeyframeManager@@UBEPBVNiRTTI@@XZ
	// returns NiAVObject's NiRTTI, the type TargetIsRequiredType asks for
	virtual const NiRTTI *	VirtualSlot22() const { return &NiAVObject::rtti; }

							NiKeyframeManager() {}
	// a manager put at the head of the target's controllers (nothing in
	// retail calls it)
							NiKeyframeManager( NiAVObject *target_ );

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

protected:
	void					CopyMembers( NiKeyframeManager *dest );

public:
	// adds the sequence under its name unless a manager owns it (public:
	// Sequence::Unknown7A1DB0 calls it)
	void					Unknown7A3390( Sequence *sequence );
	// TODO identity: the sequence operations nothing in retail calls. Each
	// takes a sequence by name or by itself: drops it from the map (0x7A3430,
	// 0x7A3510) or drops them all (0x7A35C0), and starts one animating
	// (0x7A36E0, 0x7A3740)
	bool					Unknown7A3430( const char *name );
	bool					Unknown7A3510( Sequence *sequence );
	void					Unknown7A35C0();
	bool					Unknown7A36E0( const char *name );
	bool					Unknown7A3740( Sequence *sequence );
	// blends (0x7A3770, 0x7A3830) and morphs (0x7A3970, 0x7A3A30) from the
	// first sequence to the second, from start to end
	bool					Unknown7A3770( const char *name, const char *destinationName, const char *textKey, float start, float end );
	bool					Unknown7A3830( Sequence *sequence, Sequence *destination, const char *textKey, float start, float end );
	bool					Unknown7A3970( const char *name, const char *destinationName, float start, float end );
	bool					Unknown7A3A30( Sequence *sequence, Sequence *destination, float start, float end );
	// stops one (0x7A3AF0, 0x7A3B50), a blend pair (0x7A3B80, 0x7A3C30, "Cannot
	// deactivate a blending sequence"), a morph pair (0x7A3C80, 0x7A3D30,
	// "Cannot deactivate a morphing sequence") or all of them (0x7A3D80)
	bool					Unknown7A3AF0( const char *name );
	bool					Unknown7A3B50( Sequence *sequence );
	bool					Unknown7A3B80( const char *name, const char *destinationName );
	bool					Unknown7A3C30( Sequence *sequence, Sequence *destination );
	bool					Unknown7A3C80( const char *name, const char *destinationName );
	bool					Unknown7A3D30( Sequence *sequence, Sequence *destination );
	bool					Unknown7A3D80();
	void					Unknown7A3360( const char *file, const char *name, unsigned int index );	// loads the sequence at index of the file under name and adds it
	// the sequences' count and map (names invented; AnimOnly.exe 0x493060,
	// 0x493080)
	unsigned int			GetSequenceCount() const { return sequences.GetCount(); }
	NiTStringMap<Sequence *> *	GetSequences() { return &sequences; }

	NiTStringMap<Sequence *>	sequences;		// the sequences by name
};

NiRegisterStream( NiKeyframeManager );

#endif /* !__NIKEYFRAMEMANAGER_H__ */
