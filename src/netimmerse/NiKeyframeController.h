/*
===============================================================================

	NiKeyframeController.h

	Animates its target's transform by the rotation, position and scale keys
	of an NiKeyframeData.

	Evidence: the vtable at 0x89819C (NiTimeController's 20 slots, none
	added); the constructor (0x792CE0) takes the data into the smart
	pointer at 0x3C and clears the three key indices at 0x30, 0x34 and
	0x38; CreateClone (0x793210) allocates 0x40 bytes; slot 19 (0x793840)
	tests the target against NiAVObject's NiRTTI. The NiRTTI is 0x9A0058,
	named "NiKeyframeController".

===============================================================================
*/

#ifndef __NIKEYFRAMECONTROLLER_H__
#define __NIKEYFRAMECONTROLLER_H__

#include "NiTimeController.h"
#include "NiKeyframeData.h"
#include "NiStreamMacros.h"

/*
================
NiKeyframeControllerLoadRecord

The record CreateObject hands the stream: NiTimeController's link IDs and
the data's, under a vtable of its own (0x8981F4). The class name is
invented.
================
*/
// FUNCTION: STBC 0x007937f0 ??_GNiKeyframeControllerLoadRecord@@UAEPAXI@Z @netimmerse/NiKeyframeController
class NiKeyframeControllerLoadRecord : public NiTimeControllerLoadRecord {
public:
							NiKeyframeControllerLoadRecord() { dataLinkID = 0; }

	unsigned int			dataLinkID;
};

// FUNCTION: STBC 0x007937d0 ??_GNiKeyframeController@@UAEPAXI@Z @netimmerse/NiKeyframeController
class NiKeyframeController : public NiTimeController {
public:
	// FUNCTION: STBC 0x7937C0 ?GetRTTI@NiKeyframeController@@UBEPBVNiRTTI@@XZ
	virtual const NiRTTI *	GetRTTI() const { return &rtti; }

	// cloning
	virtual NiObject *		CreateClone();

	// streaming
	virtual void			LoadBinary( NiStream &stream );
	virtual void			LinkObject( NiStream &stream );
	virtual bool			RegisterStreamables( NiStream &stream );
	virtual void			SaveBinary( NiStream &stream );
	virtual bool			IsEqual( NiObject *object );
	virtual void			GetViewerStrings( NiTArray<char *> *strings );

	virtual					~NiKeyframeController();

	virtual void			Update( float time );
	virtual bool			TargetIsRequiredType() const;

							NiKeyframeController( NiKeyframeData *data_ = 0 );

	// shares the data, restarting the key search; the key times bound the
	// controller's time, or are cleared without data (Gamebryo lineage name)
	void					SetData( NiKeyframeData *data_ ) {
								data = data_;
								lastRotIndex = 0;
								lastPosIndex = 0;
								lastScaleIndex = 0;
								if ( !data_ ) {
									loKeyTime = 0.0f;
									hiKeyTime = 0.0f;
								} else {
									ResetTimeExtrema();
								}
							}

	NiKeyframeData *		GetData() const { return data; }	// Gamebryo lineage name

	// the data's keys of each group, their count and type; none without
	// data (Gamebryo lineage names)
	NiRotKey **              GetRotAnim( unsigned int &numKeys, NiRotKey::KeyType &keyType ) const {
								if ( data ) {
									return data->GetRotAnim( numKeys, keyType );
								}
								numKeys = 0;
								keyType = NiRotKey::NOINTERP;
								return 0;
							}
	NiPosKey **              GetPosAnim( unsigned int &numKeys, NiPosKey::KeyType &keyType ) const {
								if ( data ) {
									return data->GetPosAnim( numKeys, keyType );
								}
								numKeys = 0;
								keyType = NiPosKey::NOINTERP;
								return 0;
							}
	NiFloatKey **            GetScaleAnim( unsigned int &numKeys, NiFloatKey::KeyType &keyType ) const {
								if ( data ) {
									return data->GetScaleAnim( numKeys, keyType );
								}
								numKeys = 0;
								keyType = NiFloatKey::NOINTERP;
								return 0;
							}

	// the key times bound the controller's time (Gamebryo lineage name)
	void					ResetTimeExtrema();

	// drops the keys and restarts the key searches (name invented; the
	// game's animation code expands it)
	void					ClearData() {
								data = 0;
								lastRotIndex = 0;
								lastPosIndex = 0;
								lastScaleIndex = 0;
								loKeyTime = 0.0f;
								hiKeyTime = 0.0f;
							}

	// the first keyframe controller of the object's list, or NULL (name
	// invented; 0x792C90)
	static NiKeyframeController *	FindKeyframeController( NiObjectNET *object );

	// hand the keys to the data, creating it if there is none, and restart
	// that group's key search; resetTimes bounds the controller's time by
	// the new keys (Gamebryo lineage names)
	void					ReplaceRotAnim( NiRotKey **keys, unsigned int numKeys, NiRotKey::KeyType keyType, bool resetTimes );
	void					ReplacePosAnim( NiPosKey **keys, unsigned int numKeys, NiPosKey::KeyType keyType, bool resetTimes );
	void					ReplaceScaleAnim( NiFloatKey **keys, unsigned int numKeys, NiFloatKey::KeyType keyType, bool resetTimes );

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

protected:
	void					CopyMembers( NiKeyframeController *dest );

	unsigned int			lastRotIndex;	// the keys the last update found
	unsigned int			lastPosIndex;
	unsigned int			lastScaleIndex;
	NiPointer<NiKeyframeData>	data;
};

NiRegisterStream( NiKeyframeController );

#endif /* !__NIKEYFRAMECONTROLLER_H__ */
