/*
===============================================================================

	NiKeyframeData.h

	The rotation, position and scale keys of a transform animation, shared
	by the NiKeyframeControllers that play them.

	Evidence: the constructor (0x791F90) stores the vtable at 0x898168
	(NiObject's 12 slots, none added) and clears the nine words from 8 to
	0x28; NiKeyframeController (0x792FF0) reads each group as a count, an
	array of keys and a key type, and tests the rotation type against 4
	(Euler angles, whose first key holds three float-key groups). Its
	CreateObject (0x792260) allocates 0x2C bytes for it and hands the stream
	a record under the vtable 0x898198. The three Replace functions
	(0x792080, 0x7920F0, 0x792170) are called by NiKeyframeController and
	game code. The NiRTTI is named "NiKeyframeData"; the viewer strings
	name the counts m_uiNumRotKeys, m_uiNumPosKeys and m_uiNumScaleKeys.

===============================================================================
*/

#ifndef __NIKEYFRAMEDATA_H__
#define __NIKEYFRAMEDATA_H__

#include "NiObject.h"
#include "NiStream.h"
#include "NiPosKey.h"
#include "NiFloatKey.h"
#include "NiRotKey.h"
#include "NiStreamMacros.h"

/*
================
NiKeyframeDataLoadRecord

The record CreateObject hands the stream: NiObject's link ID under a vtable
of its own (0x898198). The class name is invented.
================
*/
// FUNCTION: STBC 0x007922F0 ??_GNiKeyframeDataLoadRecord@@UAEPAXI@Z
class NiKeyframeDataLoadRecord : public NiObjectLoadRecord {
};

// FUNCTION: STBC 0x00792AA0 ??_GNiKeyframeData@@UAEPAXI@Z
class NiKeyframeData : public NiObject {
public:
	// FUNCTION: STBC 0x00792A90 ?GetRTTI@NiKeyframeData@@UBEPBVNiRTTI@@XZ
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

	virtual					~NiKeyframeData();

							NiKeyframeData();

	// the keys of each group, their count and type (Gamebryo lineage names)
	NiRotKey **				GetRotAnim( unsigned int &numKeys, NiRotKey::KeyType &keyType ) const {
								numKeys = numRotKeys;
								keyType = rotKeyType;
								return rotKeys;
							}
	NiPosKey **				GetPosAnim( unsigned int &numKeys, NiPosKey::KeyType &keyType ) const {
								numKeys = numPosKeys;
								keyType = posKeyType;
								return posKeys;
							}
	NiFloatKey **			GetScaleAnim( unsigned int &numKeys, NiFloatKey::KeyType &keyType ) const {
								numKeys = numScaleKeys;
								keyType = scaleKeyType;
								return scaleKeys;
							}

	// take ownership of the keys, deleting the old ones; without keys the
	// group is left empty and the old keys are kept allocated (Gamebryo
	// lineage names)
	// the key counts (Gamebryo lineage names)
	unsigned int			GetNumRotKeys() const { return numRotKeys; }
	unsigned int			GetNumPosKeys() const { return numPosKeys; }
	unsigned int			GetNumScaleKeys() const { return numScaleKeys; }

	void					ReplaceRotAnim( NiRotKey **keys, unsigned int numKeys, NiRotKey::KeyType keyType );
	void					ReplacePosAnim( NiPosKey **keys, unsigned int numKeys, NiPosKey::KeyType keyType );
	void					ReplaceScaleAnim( NiFloatKey **keys, unsigned int numKeys, NiFloatKey::KeyType keyType );

	// frees rotation keys, with the float-key groups of Euler keys (name
	// invented)
	static void				DeleteRotKeys( NiRotKey **keys, unsigned int numKeys, NiRotKey::KeyType keyType );

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

protected:
	unsigned int			numRotKeys;
	NiRotKey **				rotKeys;
	NiRotKey::KeyType		rotKeyType;
	unsigned int			numPosKeys;
	NiPosKey **				posKeys;
	NiPosKey::KeyType		posKeyType;
	unsigned int			numScaleKeys;
	NiFloatKey **			scaleKeys;
	NiFloatKey::KeyType		scaleKeyType;
};

NiRegisterStream( NiKeyframeData );

#endif /* !__NIKEYFRAMEDATA_H__ */
