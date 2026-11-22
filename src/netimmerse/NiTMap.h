/*
===============================================================================

	NiTMap.h

	NetImmerse's chained hash map template. The class names NiTMap and
	NiTMapItem are Gamebryo lineage: the structure and behaviour match
	Gamebryo's NiTMap (KeyToHashIndex, IsKeysEqual, SetValue and ClearValue
	as virtual hooks, a default hash size of 37), but no string in the
	binary names them.

	Evidence: the stream's loader registry (0x9A2B98) is built in place by
	every class's Register function (for example NiObjectNET's, 0x7DAB30):
	a 0x10-byte object whose base vtable has five slots (the scalar deleting
	destructor, then the four hooks), a count at +4, a hash size of 37 at +8
	and a table of 37 chain heads at +0xC. Each 12-byte item holds the key,
	the value and the next item in that order. TGObject's ID map (0x6F0A70)
	is a plain NiTMap of 2039 buckets; its unit inlines SetAt, GetAt and
	RemoveAt. The event handler table (0x6D7C30) walks a map with
	GetFirstPos and GetNext inlined: a bucket scan, then the next link or a
	rehash of the last key to resume the scan.

	<new> declares operator delete as not throwing: the destructors of the
	maps (0x455720, 0x4557C0) have no unwind frames, which a delete that
	might throw would give them.

===============================================================================
*/

#ifndef __NITMAP_H__
#define __NITMAP_H__

#include <assert.h>
#include <new>
#include <string.h>

// a position while walking a map: the next item to return, or NULL at the end
typedef void *			NiTMapIterator;

// FUNCTION: STBC 0x4586F0 ??1?$NiTMapItem@PBDV?$NiPointer@VTGOverlayImage@@@@@@QAE@XZ @game/TGOverlayController
// FUNCTION: STBC 0x6E7A90 ??1?$NiTMapItem@HVTGString@@@@QAE@XZ @game/TGInputManager
template <class TKEY, class TVAL> class NiTMapItem {
public:
	TKEY				key;
	TVAL				val;
	NiTMapItem *		next;		// the next item in the same bucket
};

template <class TKEY, class TVAL> class NiTMap {
public:
						NiTMap( unsigned int hashSize_ = 37 );
	virtual				~NiTMap();

	unsigned int		GetCount() const { return count; }

	void				SetAt( TKEY key, TVAL val );
	bool				RemoveAt( TKEY key );
	void				RemoveAll();
	bool				GetAt( TKEY key, TVAL &val ) const;	// const, as DamageTool's NetImmerse decorates it (QBE)

	// walking the map, bucket by bucket
	NiTMapIterator		GetFirstPos();
	void				GetNext( NiTMapIterator &pos, TKEY &key, TVAL &val );

protected:
	virtual unsigned int	KeyToHashIndex( TKEY key ) const;
	virtual bool		IsKeysEqual( TKEY key1, TKEY key2 ) const;
	virtual void		SetValue( NiTMapItem<TKEY, TVAL> *item, TKEY key, TVAL val );
	virtual void		ClearValue( NiTMapItem<TKEY, TVAL> *item );

	unsigned int		count;		// items in the map
	unsigned int		hashSize;	// buckets
	NiTMapItem<TKEY, TVAL> **	hashTable;	// heap block of hashSize chain heads
};

/*
================
NiTMap::NiTMap

Allocates the buckets, all empty.
================
*/
// FUNCTION: STBC 0x4BB140 ??0?$NiTMap@_JG@@QAE@I@Z @game/VisibleDamageBuilder
// FUNCTION: STBC 0x42E510 ??0?$NiTMap@PBDPAVCameraMode@@@@QAE@I@Z @game/CameraObjectClass
// FUNCTION: STBC 0x4EFDC0 ??0?$NiTMap@PAVNiTriShape@@PAVPassShape@@@@QAE@I@Z @game/SpecularPass
// FUNCTION: STBC 0x42E2E0 ??0?$NiTMap@PBDPAD@@QAE@I@Z @game/CameraObjectClass
// FUNCTION: STBC 0x417AE0 ??0?$NiTMap@PBDJ@@QAE@I@Z @game/SetClass
// FUNCTION: STBC 0x4471B0 ??0?$NiTMap@PBDM@@QAE@I@Z @game/UtopiaModule
// FUNCTION: STBC 0x447200 ??0?$NiTMap@PBDVNiPoint3@@@@QAE@I@Z @game/UtopiaModule
// FUNCTION: STBC 0x665A90 ??0?$NiTMap@PBDPAUBridgeTexture@@@@QAE@I@Z @game/BridgeObjectClass
// FUNCTION: STBC 0x6E7470 ??0?$NiTMap@HH@@QAE@I@Z @game/TGInputManager
template <class TKEY, class TVAL> inline NiTMap<TKEY, TVAL>::NiTMap( unsigned int hashSize_ ) {
	hashSize = hashSize_;
	count = 0;
	hashTable = new NiTMapItem<TKEY, TVAL> *[hashSize];
	assert( hashTable );
	memset( hashTable, 0, hashSize * sizeof( NiTMapItem<TKEY, TVAL> * ) );
}

/*
================
NiTMap::~NiTMap
================
*/
// FUNCTION: STBC 0x4BB390 ??_G?$NiTMap@_JG@@UAEPAXI@Z @game/VisibleDamageBuilder
// FUNCTION: STBC 0x4C3CB0 ??1?$NiTMap@_JUDamageVertexIndices@@@@UAE@XZ @game/DamagePolygonizer
// FUNCTION: STBC 0x4BB190 ??1?$NiTMap@_JG@@UAE@XZ @game/VisibleDamageBuilder
// FUNCTION: STBC 0x455720 ??_G?$NiTMap@PBDP6AXAAVNiStream@@AAPAVNiObject@@@Z@@UAEPAXI@Z
// FUNCTION: STBC 0x4EFE10 ??1?$NiTMap@PAVNiTriShape@@PAVPassShape@@@@UAE@XZ @game/SpecularPass
// FUNCTION: STBC 0x711DE0 ??1?$NiTMap@PAVNiProvider_Info@@PAUTGSamplePool@@@@UAE@XZ @game/TGSoundManager
// FUNCTION: STBC 0x4F0040 ??_G?$NiTMap@PAVNiTriShape@@PAVPassShape@@@@UAEPAXI@Z @game/SpecularPass
// FUNCTION: STBC 0x42E220 ??_G?$NiTMap@PBDPAVCameraMode@@@@UAEPAXI@Z @game/CameraObjectClass
// FUNCTION: STBC 0x58CAC0 ??_G?$NiTMap@PBDPAVBackdrop@@@@UAEPAXI@Z @game/Backdrop
// FUNCTION: STBC 0x454730 ??_G?$NiTMap@PBDPAVLODModel@@@@UAEPAXI@Z @game/LODModel
// FUNCTION: STBC 0x42E120 ??_G?$NiTMap@PBDPAD@@UAEPAXI@Z @game/TGAnimationManagerClass
// FUNCTION: STBC 0x48B270 ??_G?$NiTMap@JM@@UAEPAXI@Z @game/OptimizedSelectTarget
// FUNCTION: STBC 0x7C49A0 ??1?$NiTMap@IPAVNiD3DGeometryData@@@@UAE@XZ
// FUNCTION: STBC 0x48AF90 ??1?$NiTMap@JM@@UAE@XZ @game/OptimizedSelectTarget
// FUNCTION: STBC 0x7C4B40 ??_G?$NiTMap@IPAVNiD3DGeometryData@@@@UAEPAXI@Z
// FUNCTION: STBC 0x429170 ??_G?$NiTMap@PBDVNiPoint3@@@@UAEPAXI@Z @game/CameraMode
// FUNCTION: STBC 0x4290D0 ??_G?$NiTMap@PBDM@@UAEPAXI@Z @game/CameraMode
// FUNCTION: STBC 0x429210 ??_G?$NiTMap@PBDP6APAVCameraMode@@PAVCameraObjectClass@@@Z@@UAEPAXI@Z @game/CameraMode
// FUNCTION: STBC 0x4178F0 ??_G?$NiTMap@PBDJ@@UAEPAXI@Z @game/CameraMode
// FUNCTION: STBC 0x417270 ??1?$NiTMap@PBDJ@@UAE@XZ @game/SetClass
// FUNCTION: STBC 0x5A36F0 ??1?$NiTMap@IPAVUnknownClass6A8C50@@@@UAE@XZ @game/PhysicsObjectClass
// FUNCTION: STBC 0x5A3780 ?KeyToHashIndex@?$NiTMap@IPAVUnknownClass6A8C50@@@@MBEII@Z @game/PhysicsObjectClass
// FUNCTION: STBC 0x5A3790 ?IsKeysEqual@?$NiTMap@IPAVUnknownClass6A8C50@@@@MBE_NII@Z @game/PhysicsObjectClass
// FUNCTION: STBC 0x5A37B0 ?SetValue@?$NiTMap@IPAVUnknownClass6A8C50@@@@MAEXPAV?$NiTMapItem@IPAVUnknownClass6A8C50@@@@IPAVUnknownClass6A8C50@@@Z @game/PhysicsObjectClass
// FUNCTION: STBC 0x5A37D0 ?ClearValue@?$NiTMap@IPAVUnknownClass6A8C50@@@@MAEXPAV?$NiTMapItem@IPAVUnknownClass6A8C50@@@@@Z @game/PhysicsObjectClass
// FUNCTION: STBC 0x5A3800 ??1?$NiTMap@JJ@@UAE@XZ @game/PhysicsObjectClass
// FUNCTION: STBC 0x5A38B0 ?KeyToHashIndex@?$NiTMap@JJ@@MBEIJ@Z @game/PhysicsObjectClass
// FUNCTION: STBC 0x5A38C0 ?IsKeysEqual@?$NiTMap@JJ@@MBE_NJJ@Z @game/PhysicsObjectClass
// FUNCTION: STBC 0x5A38E0 ?SetValue@?$NiTMap@JJ@@MAEXPAV?$NiTMapItem@JJ@@JJ@Z @game/PhysicsObjectClass
// FUNCTION: STBC 0x5A3900 ?ClearValue@?$NiTMap@JJ@@MAEXPAV?$NiTMapItem@JJ@@@Z @game/PhysicsObjectClass
// FUNCTION: STBC 0x4BB340 ?ClearValue@?$NiTMap@_JG@@MAEXPAV?$NiTMapItem@_JG@@@Z @game/VisibleDamageBuilder
// FUNCTION: STBC 0x5A3970 ??_G?$NiTMap@IPAVUnknownClass6A8C50@@@@UAEPAXI@Z @game/PhysicsObjectClass
// FUNCTION: STBC 0x5A39F0 ??_G?$NiTMap@JJ@@UAEPAXI@Z @game/PhysicsObjectClass
// FUNCTION: STBC 0x5A6E20 ??1?$NiTMap@J_N@@UAE@XZ @game/ProximityCheck
// FUNCTION: STBC 0x55B8E0 ??1?$NiTMap@HPAV?$NiTList@PAUKeyBind@@@@@@UAE@XZ @game/KeyboardBinding
// FUNCTION: STBC 0x55BD40 ??_G?$NiTMap@HPAV?$NiTList@PAUKeyBind@@@@@@UAEPAXI@Z @game/KeyboardBinding
// FUNCTION: STBC 0x55BAA0 ?KeyToHashIndex@?$NiTMap@HPAV?$NiTList@PAUKeyBind@@@@@@MBEIH@Z @game/KeyboardBinding
// FUNCTION: STBC 0x55BAB0 ?IsKeysEqual@?$NiTMap@HPAV?$NiTList@PAUKeyBind@@@@@@MBE_NHH@Z @game/KeyboardBinding
// FUNCTION: STBC 0x55BAD0 ?SetValue@?$NiTMap@HPAV?$NiTList@PAUKeyBind@@@@@@MAEXPAV?$NiTMapItem@HPAV?$NiTList@PAUKeyBind@@@@@@HPAV?$NiTList@PAUKeyBind@@@@@Z @game/KeyboardBinding
// FUNCTION: STBC 0x55BAF0 ?ClearValue@?$NiTMap@HPAV?$NiTList@PAUKeyBind@@@@@@MAEXPAV?$NiTMapItem@HPAV?$NiTList@PAUKeyBind@@@@@@@Z @game/KeyboardBinding
// FUNCTION: STBC 0x6F1890 ?KeyToHashIndex@?$NiTMap@HP6APAVTGObject@@J@Z@@MBEIH@Z @game/TGObject
// FUNCTION: STBC 0x6F18A0 ?IsKeysEqual@?$NiTMap@HP6APAVTGObject@@J@Z@@MBE_NHH@Z @game/TGObject
// FUNCTION: STBC 0x6F18C0 ?SetValue@?$NiTMap@HP6APAVTGObject@@J@Z@@MAEXPAV?$NiTMapItem@HP6APAVTGObject@@J@Z@@HP6APAVTGObject@@J@Z@Z @game/TGObject
// FUNCTION: STBC 0x6F18E0 ?ClearValue@?$NiTMap@HP6APAVTGObject@@J@Z@@MAEXPAV?$NiTMapItem@HP6APAVTGObject@@J@Z@@@Z @game/TGObject
// FUNCTION: STBC 0x6F19C0 ??_G?$NiTMap@HP6APAVTGObject@@J@Z@@UAEPAXI@Z @game/TGObject
// FUNCTION: STBC 0x6F18F0 ?KeyToHashIndex@?$NiTMap@HUTGLoadSizes@@@@MBEIH@Z @game/TGObject
// FUNCTION: STBC 0x6F1900 ?IsKeysEqual@?$NiTMap@HUTGLoadSizes@@@@MBE_NHH@Z @game/TGObject
// FUNCTION: STBC 0x6F1920 ?SetValue@?$NiTMap@HUTGLoadSizes@@@@MAEXPAV?$NiTMapItem@HUTGLoadSizes@@@@HUTGLoadSizes@@@Z @game/TGObject
// FUNCTION: STBC 0x6F1950 ?ClearValue@?$NiTMap@HUTGLoadSizes@@@@MAEXPAV?$NiTMapItem@HUTGLoadSizes@@@@@Z @game/TGObject
// FUNCTION: STBC 0x6F1A40 ??_G?$NiTMap@HUTGLoadSizes@@@@UAEPAXI@Z @game/TGObject
// FUNCTION: STBC 0x6F1960 ?KeyToHashIndex@?$NiTMap@JPAVTGObject@@@@MBEIJ@Z @game/TGObject
// FUNCTION: STBC 0x6F1970 ?IsKeysEqual@?$NiTMap@JPAVTGObject@@@@MBE_NJJ@Z @game/TGObject
// FUNCTION: STBC 0x6F1990 ?SetValue@?$NiTMap@JPAVTGObject@@@@MAEXPAV?$NiTMapItem@JPAVTGObject@@@@JPAVTGObject@@@Z @game/TGObject
// FUNCTION: STBC 0x6F19B0 ?ClearValue@?$NiTMap@JPAVTGObject@@@@MAEXPAV?$NiTMapItem@JPAVTGObject@@@@@Z @game/TGObject
// FUNCTION: STBC 0x6F1AC0 ??_G?$NiTMap@JPAVTGObject@@@@UAEPAXI@Z @game/TGObject
// FUNCTION: STBC 0x707520 ??_G?$NiTMap@PBDPAUTGOverlayEntry@@@@UAEPAXI@Z @game/TGOverlayManager
// FUNCTION: STBC 0x707260 ?KeyToHashIndex@?$NiTMap@PBDPAUTGOverlayEntry@@@@MBEIPBD@Z @game/TGOverlayManager
// FUNCTION: STBC 0x707270 ?IsKeysEqual@?$NiTMap@PBDPAUTGOverlayEntry@@@@MBE_NPBD0@Z @game/TGOverlayManager
// FUNCTION: STBC 0x707290 ?SetValue@?$NiTMap@PBDPAUTGOverlayEntry@@@@MAEXPAV?$NiTMapItem@PBDPAUTGOverlayEntry@@@@PBDPAUTGOverlayEntry@@@Z @game/TGOverlayManager
// FUNCTION: STBC 0x7072B0 ?ClearValue@?$NiTMap@PBDPAUTGOverlayEntry@@@@MAEXPAV?$NiTMapItem@PBDPAUTGOverlayEntry@@@@@Z @game/TGOverlayManager
// FUNCTION: STBC 0x7075C0 ??_G?$NiTMap@JPAVTGPhoneme@@@@UAEPAXI@Z @game/TGOverlayManager
// FUNCTION: STBC 0x707450 ??1?$NiTMap@JPAVTGPhoneme@@@@UAE@XZ @game/TGOverlayManager
// FUNCTION: STBC 0x7074C0 ?KeyToHashIndex@?$NiTMap@JPAVTGPhoneme@@@@MBEIJ@Z @game/TGOverlayManager
// FUNCTION: STBC 0x7074D0 ?IsKeysEqual@?$NiTMap@JPAVTGPhoneme@@@@MBE_NJJ@Z @game/TGOverlayManager
// FUNCTION: STBC 0x7074F0 ?SetValue@?$NiTMap@JPAVTGPhoneme@@@@MAEXPAV?$NiTMapItem@JPAVTGPhoneme@@@@JPAVTGPhoneme@@@Z @game/TGOverlayManager
// FUNCTION: STBC 0x707510 ?ClearValue@?$NiTMap@JPAVTGPhoneme@@@@MAEXPAV?$NiTMapItem@JPAVTGPhoneme@@@@@Z @game/TGOverlayManager
// FUNCTION: STBC 0x7D1CE0 ??1?$NiTMap@IPAVNiD3DLightEntry@@@@UAE@XZ
// FUNCTION: STBC 0x7D1E50 ??_G?$NiTMap@IPAVNiD3DLightEntry@@@@UAEPAXI@Z
// FUNCTION: STBC 0x7D8260 ??1?$NiTMap@IPAVNiD3DMaterialEntry@@@@UAE@XZ
// FUNCTION: STBC 0x7D8330 ??_G?$NiTMap@IPAVNiD3DMaterialEntry@@@@UAEPAXI@Z
// FUNCTION: STBC 0x44EFA0 ??_G?$NiTMap@PBDV?$NiPointer@VNiImage@@@@@@UAEPAXI@Z
// FUNCTION: STBC 0x7D7930 ??_G?$NiTMap@PAUIDirectDraw7@@PAVNiD3DTextureManager@@@@UAEPAXI@Z
// FUNCTION: STBC 0x7CBC70 ??1?$NiTMap@IPAVNiD3DVertexBufferEntry@@@@UAE@XZ
// FUNCTION: STBC 0x7CBD40 ??_G?$NiTMap@IPAVNiD3DVertexBufferEntry@@@@UAEPAXI@Z
// FUNCTION: STBC 0x6F6B10 ?KeyToHashIndex@?$NiTMap@HM@@MBEIH@Z @game/TGAttrTable
// FUNCTION: STBC 0x6F6B20 ?IsKeysEqual@?$NiTMap@HM@@MBE_NHH@Z @game/TGAttrTable
// FUNCTION: STBC 0x6F6B40 ?SetValue@?$NiTMap@HM@@MAEXPAV?$NiTMapItem@HM@@HM@Z @game/TGAttrTable
// FUNCTION: STBC 0x6F6B60 ?ClearValue@?$NiTMap@HM@@MAEXPAV?$NiTMapItem@HM@@@Z @game/TGAttrTable
// FUNCTION: STBC 0x6F6B70 ??_G?$NiTMap@HM@@UAEPAXI@Z @game/TGAttrTable
// FUNCTION: STBC 0x446AF0 ??1?$NiTMap@JH@@UAE@XZ @game/UtopiaModule
// FUNCTION: STBC 0x446B60 ?KeyToHashIndex@?$NiTMap@JH@@MBEIJ@Z @game/UtopiaModule
// FUNCTION: STBC 0x446B70 ?IsKeysEqual@?$NiTMap@JH@@MBE_NJJ@Z @game/UtopiaModule
// FUNCTION: STBC 0x446B90 ?SetValue@?$NiTMap@JH@@MAEXPAV?$NiTMapItem@JH@@JH@Z @game/UtopiaModule
// FUNCTION: STBC 0x446BB0 ?ClearValue@?$NiTMap@JH@@MAEXPAV?$NiTMapItem@JH@@@Z @game/UtopiaModule
// FUNCTION: STBC 0x446E40 ??_G?$NiTMap@JH@@UAEPAXI@Z @game/UtopiaModule
// FUNCTION: STBC 0x446BC0 ?KeyToHashIndex@?$NiTMap@J_N@@MBEIJ@Z @game/UtopiaModule
// FUNCTION: STBC 0x446BD0 ?IsKeysEqual@?$NiTMap@J_N@@MBE_NJJ@Z @game/UtopiaModule
// FUNCTION: STBC 0x446BF0 ?SetValue@?$NiTMap@J_N@@MAEXPAV?$NiTMapItem@J_N@@J_N@Z @game/UtopiaModule
// FUNCTION: STBC 0x446C10 ?ClearValue@?$NiTMap@J_N@@MAEXPAV?$NiTMapItem@J_N@@@Z @game/UtopiaModule
// FUNCTION: STBC 0x446EC0 ??_G?$NiTMap@J_N@@UAEPAXI@Z @game/UtopiaModule
// FUNCTION: STBC 0x446980 ?KeyToHashIndex@?$NiTMap@PBDPAU_object@@@@MBEIPBD@Z @game/UtopiaModule
// FUNCTION: STBC 0x446990 ?IsKeysEqual@?$NiTMap@PBDPAU_object@@@@MBE_NPBD0@Z @game/UtopiaModule
// FUNCTION: STBC 0x4469B0 ?SetValue@?$NiTMap@PBDPAU_object@@@@MAEXPAV?$NiTMapItem@PBDPAU_object@@@@PBDPAU_object@@@Z @game/UtopiaModule
// FUNCTION: STBC 0x4469D0 ?ClearValue@?$NiTMap@PBDPAU_object@@@@MAEXPAV?$NiTMapItem@PBDPAU_object@@@@@Z @game/UtopiaModule
// FUNCTION: STBC 0x446CA0 ??_G?$NiTMap@PBDPAU_object@@@@UAEPAXI@Z @game/UtopiaModule
// FUNCTION: STBC 0x6EFC60 ??1?$NiTMap@JPAX@@UAE@XZ @game/TGObjectModule
// FUNCTION: STBC 0x6EFD30 ??_G?$NiTMap@JPAX@@UAEPAXI@Z @game/TGObjectModule
// FUNCTION: STBC 0x70A840 ??_G?$NiTMap@PBDPAVTGSound@@@@UAEPAXI@Z @game/TGSoundModule
// FUNCTION: STBC 0x70EAA0 ??_G?$NiTMap@PBDPAVTGSoundRegion@@@@UAEPAXI@Z @game/TGSoundRegion
// FUNCTION: STBC 0x70A8E0 ??_G?$NiTMap@PAVNiProvider_Info@@PAUTGSamplePool@@@@UAEPAXI@Z @game/TGSoundModule
// FUNCTION: STBC 0x478260 ??_G?$NiTMap@PBDPAUBuilderAIBlock@@@@UAEPAXI@Z @game/BuilderAI
// FUNCTION: STBC 0x47A870 ??_G?$NiTMap@PBDPAV?$NiTList@PAU_object@@@@@@UAEPAXI@Z @game/ConditionalAI
// FUNCTION: STBC 0x471CB0 ??_G?$NiTMap@JPAVArtificialIntelligence@@@@UAEPAXI@Z @game/ArtificialIntelligence
// FUNCTION: STBC 0x7A5580 ??_G?$NiTMap@PBDPAVSequence@NiKeyframeManager@@@@UAEPAXI@Z @netimmerse/NiKeyframeManager
// FUNCTION: STBC 0x4F42C0 ??_G?$NiTMap@PBDPAVTextureAnimContainer@@@@UAEPAXI@Z @game/TextureAnimManager
// FUNCTION: STBC 0x6A6DE0 ??1?$NiTMap@HPAVTGMessage@@@@UAE@XZ @game/NetFile
// FUNCTION: STBC 0x6A7250 ??_G?$NiTMap@HPAVTGMessage@@@@UAEPAXI@Z @game/NetFile
// FUNCTION: STBC 0x6A6F30 ??1?$NiTMap@HPAV?$NiTList@PAVTGMessage@@@@@@UAE@XZ @game/NetFile
// FUNCTION: STBC 0x6A72D0 ??_G?$NiTMap@HPAV?$NiTList@PAVTGMessage@@@@@@UAEPAXI@Z @game/NetFile
// FUNCTION: STBC 0x6A7080 ??1?$NiTMap@HPAVNetFileNameList@@@@UAE@XZ @game/NetFile
// FUNCTION: STBC 0x6A7350 ??_G?$NiTMap@HPAVNetFileNameList@@@@UAEPAXI@Z @game/NetFile
// FUNCTION: STBC 0x6A8FB0 ??1?$NiTMap@IPAUUnknownClass6A8C50Item@@@@UAE@XZ @game/UnknownClass6A8C50
// FUNCTION: STBC 0x6A9100 ??_G?$NiTMap@IPAUUnknownClass6A8C50Item@@@@UAEPAXI@Z @game/UnknownClass6A8C50
// FUNCTION: STBC 0x6D8520 ??_G?$NiTMap@HPAVTGHandler@@@@UAEPAXI@Z @game/TGEventHandlerTable
// FUNCTION: STBC 0x6DB850 ??_G?$NiTMap@HPAVTGBroadcastHandlerList@@@@UAEPAXI@Z @game/TGBroadcastHandlerTable
// FUNCTION: STBC 0x6E7980 ??_G?$NiTMap@HH@@UAEPAXI@Z @game/TGInputManager
// FUNCTION: STBC 0x6E7A00 ??_G?$NiTMap@HVTGString@@@@UAEPAXI@Z @game/TGInputManager
// FUNCTION: STBC 0x6E7680 ??1?$NiTMap@HVTGString@@@@UAE@XZ @game/TGInputManager
// FUNCTION: STBC 0x719DA0 ??_G?$NiTMap@JPAUTGTiming@@@@UAEPAXI@Z @game/TGProfilingInfo
// FUNCTION: STBC 0x4C4450 ??_G?$NiTMap@_JUDamageVertexIndices@@@@UAEPAXI@Z @game/DamagePolygonizer
// FUNCTION: STBC 0x4C4550 ??_G?$NiTMap@_JPAUDamageIndexBlock@@@@UAEPAXI@Z @game/DamagePolygonizer
// FUNCTION: STBC 0x4C44D0 ??_G?$NiTMap@_JVVertexInfo@MetaVolumePolygonizer@@@@UAEPAXI@Z @game/DamagePolygonizer
// FUNCTION: STBC 0x54F3D0 ??1?$NiTMap@PAVSTNumericBar@@J@@UAE@XZ @game/EngPowerCtrl
// FUNCTION: STBC 0x54F6A0 ??_G?$NiTMap@PAVSTNumericBar@@J@@UAEPAXI@Z @game/EngPowerCtrl
// FUNCTION: STBC 0x4C3F20 ??1?$NiTMap@_JPAUDamageIndexBlock@@@@UAE@XZ @game/DamagePolygonizer
// FUNCTION: STBC 0x4C3DA0 ??1?$NiTMap@_JVVertexInfo@MetaVolumePolygonizer@@@@UAE@XZ @game/DamagePolygonizer
// FUNCTION: STBC 0x665540 ??1?$NiTMap@PBDPAUBridgeTexture@@@@UAE@XZ @game/BridgeObjectClass
// FUNCTION: STBC 0x6658A0 ??_G?$NiTMap@PBDPAUBridgeTexture@@@@UAEPAXI@Z @game/BridgeObjectClass
// FUNCTION: STBC 0x66F220 ??1?$NiTMap@HPAVTGParagraph@@@@UAE@XZ @game/CharacterClass
// FUNCTION: STBC 0x66F3E0 ??1?$NiTMap@HPAUPhonemeList@@@@UAE@XZ @game/CharacterClass
// FUNCTION: STBC 0x66F540 ??_G?$NiTMap@HPAVTGParagraph@@@@UAEPAXI@Z @game/CharacterClass
// FUNCTION: STBC 0x66F5F0 ??_G?$NiTMap@HPAUPhonemeList@@@@UAEPAXI@Z @game/CharacterClass
// FUNCTION: STBC 0x6D7000 ??_G?$NiTMap@PAVTGObject@@PAV?$NiTList@PAVTGEvent@@@@@@UAEPAXI@Z @game/TGEvent
// FUNCTION: STBC 0x6D7080 ??_G?$NiTMap@JPAVTGEvent@@@@UAEPAXI@Z @game/TGEvent
// FUNCTION: STBC 0x6DD7C0 ??_G?$NiTMap@JPAVTGTimer@@@@UAEPAXI@Z @game/TGTimer
// FUNCTION: STBC 0x6D8450 ??1?$NiTMap@HPAVTGHandler@@@@UAE@XZ @game/TGEventHandlerTable
// FUNCTION: STBC 0x4756C0 ??_G?$NiTMap@PBDPAUAILogScope@@@@UAEPAXI@Z @game/AILog
// FUNCTION: STBC 0x6DB780 ??1?$NiTMap@HPAVTGBroadcastHandlerList@@@@UAE@XZ @game/TGBroadcastHandlerTable
// FUNCTION: STBC 0x458520 ??_G?$NiTMap@PBDV?$NiPointer@VTGOverlayImage@@@@@@UAEPAXI@Z @game/TGOverlayController
// FUNCTION: STBC 0x723760 ??_G?$NiTMap@PBD_N@@UAEPAXI@Z @game/TGTraceStream
// FUNCTION: STBC 0x4A3C90 ??_G?$NiTMap@PBDPAV?$SortedArray@VPlacementObject@@J@@@@UAEPAXI@Z @game/PlacementObject
// FUNCTION: STBC 0x71E140 ??_G?$NiTMap@PBDPAVTGConfigValue@@@@UAEPAXI@Z @game/TGConfigMapping
// FUNCTION: STBC 0x70ACA0 ??_G?$NiTMap@PBDPAUTGMusicPeriod@@@@UAEPAXI@Z @game/TGMusic
// FUNCTION: STBC 0x70D370 ??_G?$NiTMap@PBDPAVTGSoundFile@@@@UAEPAXI@Z @game/TGSound
// FUNCTION: STBC 0x72AF10 ??_G?$NiTMap@HPAVTGIconLocation@@@@UAEPAXI@Z @game/TGIconManager
// FUNCTION: STBC 0x4867C0 ??_G?$NiTMap@JPAUFireSubsystemRating@@@@UAEPAXI@Z @game/OptimizedFireScript
// FUNCTION: STBC 0x6E74C0 ??1?$NiTMap@HH@@UAE@XZ @game/TGInputManager
// FUNCTION: STBC 0x72AE10 ??1?$NiTMap@HPAVTGIconLocation@@@@UAE@XZ @game/TGIconManager
// FUNCTION: STBC 0x4864B0 ??1?$NiTMap@JPAUFireSubsystemRating@@@@UAE@XZ @game/OptimizedFireScript
template <class TKEY, class TVAL> inline NiTMap<TKEY, TVAL>::~NiTMap() {
	RemoveAll();
	delete [] hashTable;
}

/*
================
NiTMap::KeyToHashIndex

The key's value modulo the hash size.
================
*/
// FUNCTION: STBC 0x4555B0 ?KeyToHashIndex@?$NiTMap@PBDP6AXAAVNiStream@@AAPAVNiObject@@@Z@@MBEIPBD@Z
// FUNCTION: STBC 0x4EFEC0 ?KeyToHashIndex@?$NiTMap@PAVNiTriShape@@PAVPassShape@@@@MBEIPAVNiTriShape@@@Z @game/SpecularPass
// FUNCTION: STBC 0x42DDF0 ?KeyToHashIndex@?$NiTMap@PBDPAVCameraMode@@@@MBEIPBD@Z @game/CameraObjectClass
// FUNCTION: STBC 0x58C700 ?KeyToHashIndex@?$NiTMap@PBDPAVBackdrop@@@@MBEIPBD@Z @game/Backdrop
// FUNCTION: STBC 0x4542E0 ?KeyToHashIndex@?$NiTMap@PBDPAVLODModel@@@@MBEIPBD@Z @game/LODModel
// FUNCTION: STBC 0x42D9D0 ?KeyToHashIndex@?$NiTMap@PBDPAD@@MBEIPBD@Z @game/TGAnimationManagerClass
// FUNCTION: STBC 0x71DD60 ?KeyToHashIndex@?$NiTMap@PBDPAVTGConfigValue@@@@MBEIPBD@Z @game/TGConfigMapping
// FUNCTION: STBC 0x7C4A70 ?KeyToHashIndex@?$NiTMap@IPAVNiD3DGeometryData@@@@MBEII@Z
// FUNCTION: STBC 0x7D1D50 ?KeyToHashIndex@?$NiTMap@IPAVNiD3DLightEntry@@@@MBEII@Z
// FUNCTION: STBC 0x7D82D0 ?KeyToHashIndex@?$NiTMap@IPAVNiD3DMaterialEntry@@@@MBEII@Z
// FUNCTION: STBC 0x44ECE0 ?KeyToHashIndex@?$NiTMap@PBDV?$NiPointer@VNiImage@@@@@@MBEIPBD@Z
// FUNCTION: STBC 0x7D7880 ?KeyToHashIndex@?$NiTMap@PAUIDirectDraw7@@PAVNiD3DTextureManager@@@@MBEIPAUIDirectDraw7@@@Z
// FUNCTION: STBC 0x7CBCE0 ?KeyToHashIndex@?$NiTMap@IPAVNiD3DVertexBufferEntry@@@@MBEII@Z
// FUNCTION: STBC 0x6EFCD0 ?KeyToHashIndex@?$NiTMap@JPAX@@MBEIJ@Z @game/TGObjectModule
// FUNCTION: STBC 0x70A570 ?KeyToHashIndex@?$NiTMap@PBDPAVTGSound@@@@MBEIPBD@Z @game/TGSoundModule
// FUNCTION: STBC 0x70E8B0 ?KeyToHashIndex@?$NiTMap@PBDPAVTGSoundRegion@@@@MBEIPBD@Z @game/TGSoundRegion
// FUNCTION: STBC 0x70A7E0 ?KeyToHashIndex@?$NiTMap@PAVNiProvider_Info@@PAUTGSamplePool@@@@MBEIPAVNiProvider_Info@@@Z @game/TGSoundModule
// FUNCTION: STBC 0x477F10 ?KeyToHashIndex@?$NiTMap@PBDPAUBuilderAIBlock@@@@MBEIPBD@Z @game/BuilderAI
// FUNCTION: STBC 0x47A4C0 ?KeyToHashIndex@?$NiTMap@PBDPAV?$NiTList@PAU_object@@@@@@MBEIPBD@Z @game/ConditionalAI
// FUNCTION: STBC 0x471A70 ?KeyToHashIndex@?$NiTMap@JPAVArtificialIntelligence@@@@MBEIJ@Z @game/ArtificialIntelligence
// FUNCTION: STBC 0x7A51D0 ?KeyToHashIndex@?$NiTMap@PBDPAVSequence@NiKeyframeManager@@@@MBEIPBD@Z @netimmerse/NiKeyframeManager
// FUNCTION: STBC 0x4F4140 ?KeyToHashIndex@?$NiTMap@PBDPAVTextureAnimContainer@@@@MBEIPBD@Z @game/TextureAnimManager
// FUNCTION: STBC 0x48B020 ?KeyToHashIndex@?$NiTMap@JM@@MBEIJ@Z @game/OptimizedSelectTarget
// FUNCTION: STBC 0x6A6ED0 ?KeyToHashIndex@?$NiTMap@HPAVTGMessage@@@@MBEIH@Z @game/NetFile
// FUNCTION: STBC 0x6A7020 ?KeyToHashIndex@?$NiTMap@HPAV?$NiTList@PAVTGMessage@@@@@@MBEIH@Z @game/NetFile
// FUNCTION: STBC 0x6A7150 ?KeyToHashIndex@?$NiTMap@HPAVNetFileNameList@@@@MBEIH@Z @game/NetFile
// FUNCTION: STBC 0x6A9020 ?KeyToHashIndex@?$NiTMap@IPAUUnknownClass6A8C50Item@@@@MBEII@Z @game/UnknownClass6A8C50
// FUNCTION: STBC 0x428C80 ?KeyToHashIndex@?$NiTMap@PBDVNiPoint3@@@@MBEIPBD@Z @game/CameraMode
// FUNCTION: STBC 0x428980 ?KeyToHashIndex@?$NiTMap@PBDM@@MBEIPBD@Z @game/CameraMode
// FUNCTION: STBC 0x428EE0 ?KeyToHashIndex@?$NiTMap@PBDP6APAVCameraMode@@PAVCameraObjectClass@@@Z@@MBEIPBD@Z @game/CameraMode
// FUNCTION: STBC 0x4172E0 ?KeyToHashIndex@?$NiTMap@PBDJ@@MBEIPBD@Z @game/CameraMode
// FUNCTION: STBC 0x4C40E0 ?KeyToHashIndex@?$NiTMap@_JPAUDamageIndexBlock@@@@MBEI_J@Z @game/DamagePolygonizer
// FUNCTION: STBC 0x4C3EB0 ?KeyToHashIndex@?$NiTMap@_JVVertexInfo@MetaVolumePolygonizer@@@@MBEI_J@Z @game/DamagePolygonizer
// FUNCTION: STBC 0x4C3D20 ?KeyToHashIndex@?$NiTMap@_JUDamageVertexIndices@@@@MBEI_J@Z @game/DamagePolygonizer
// FUNCTION: STBC 0x6D84C0 ?KeyToHashIndex@?$NiTMap@HPAVTGHandler@@@@MBEIH@Z @game/TGEventHandlerTable
// FUNCTION: STBC 0x6DB7F0 ?KeyToHashIndex@?$NiTMap@HPAVTGBroadcastHandlerList@@@@MBEIH@Z @game/TGBroadcastHandlerTable
// FUNCTION: STBC 0x6E75C0 ?KeyToHashIndex@?$NiTMap@HH@@MBEIH@Z @game/TGInputManager
// FUNCTION: STBC 0x6E78E0 ?KeyToHashIndex@?$NiTMap@HVTGString@@@@MBEIH@Z @game/TGInputManager
// FUNCTION: STBC 0x719BA0 ?KeyToHashIndex@?$NiTMap@JPAUTGTiming@@@@MBEIJ@Z @game/TGProfilingInfo
// FUNCTION: STBC 0x665620 ?KeyToHashIndex@?$NiTMap@PBDPAUBridgeTexture@@@@MBEIPBD@Z @game/BridgeObjectClass
// FUNCTION: STBC 0x66F290 ?KeyToHashIndex@?$NiTMap@HPAVTGParagraph@@@@MBEIH@Z @game/CharacterClass
// FUNCTION: STBC 0x66F450 ?KeyToHashIndex@?$NiTMap@HPAUPhonemeList@@@@MBEIH@Z @game/CharacterClass
// FUNCTION: STBC 0x6D6F20 ?KeyToHashIndex@?$NiTMap@PAVTGObject@@PAV?$NiTList@PAVTGEvent@@@@@@MBEIPAVTGObject@@@Z @game/TGEvent
// FUNCTION: STBC 0x4BB2B0 ?KeyToHashIndex@?$NiTMap@_JG@@MBEI_J@Z @game/VisibleDamageBuilder
// FUNCTION: STBC 0x6D6FA0 ?KeyToHashIndex@?$NiTMap@JPAVTGEvent@@@@MBEIJ@Z @game/TGEvent
// FUNCTION: STBC 0x6DD760 ?KeyToHashIndex@?$NiTMap@JPAVTGTimer@@@@MBEIJ@Z @game/TGTimer
// FUNCTION: STBC 0x475430 ?KeyToHashIndex@?$NiTMap@PBDPAUAILogScope@@@@MBEIPBD@Z @game/AILog
// FUNCTION: STBC 0x4581E0 ?KeyToHashIndex@?$NiTMap@PBDV?$NiPointer@VTGOverlayImage@@@@@@MBEIPBD@Z @game/TGOverlayController
// FUNCTION: STBC 0x54F5E0 ?KeyToHashIndex@?$NiTMap@PAVSTNumericBar@@J@@MBEIPAVSTNumericBar@@@Z @game/EngPowerCtrl
// FUNCTION: STBC 0x723480 ?KeyToHashIndex@?$NiTMap@PBD_N@@MBEIPBD@Z @game/TGTraceStream
// FUNCTION: STBC 0x4A3870 ?KeyToHashIndex@?$NiTMap@PBDPAV?$SortedArray@VPlacementObject@@J@@@@MBEIPBD@Z @game/PlacementObject
// FUNCTION: STBC 0x70D040 ?KeyToHashIndex@?$NiTMap@PBDPAVTGSoundFile@@@@MBEIPBD@Z @game/TGSound
// FUNCTION: STBC 0x7199B0 ?KeyToHashIndex@?$NiTMap@PBDPAUTGTimingSamples@@@@MBEIPBD@Z @game/TGProfilingInfo
// FUNCTION: STBC 0x72AE80 ?KeyToHashIndex@?$NiTMap@HPAVTGIconLocation@@@@MBEIH@Z @game/TGIconManager
// FUNCTION: STBC 0x486670 ?KeyToHashIndex@?$NiTMap@JPAUFireSubsystemRating@@@@MBEIJ@Z @game/OptimizedFireScript
template <class TKEY, class TVAL> inline unsigned int NiTMap<TKEY, TVAL>::KeyToHashIndex( TKEY key ) const {
	return (unsigned int)key % hashSize;
}

/*
================
NiTMap::IsKeysEqual
================
*/
// FUNCTION: STBC 0x4555C0 ?IsKeysEqual@?$NiTMap@PBDP6AXAAVNiStream@@AAPAVNiObject@@@Z@@MBE_NPBD0@Z
// FUNCTION: STBC 0x4EFED0 ?IsKeysEqual@?$NiTMap@PAVNiTriShape@@PAVPassShape@@@@MBE_NPAVNiTriShape@@0@Z @game/SpecularPass
// FUNCTION: STBC 0x42DE00 ?IsKeysEqual@?$NiTMap@PBDPAVCameraMode@@@@MBE_NPBD0@Z @game/CameraObjectClass
// FUNCTION: STBC 0x58C710 ?IsKeysEqual@?$NiTMap@PBDPAVBackdrop@@@@MBE_NPBD0@Z @game/Backdrop
// FUNCTION: STBC 0x4542F0 ?IsKeysEqual@?$NiTMap@PBDPAVLODModel@@@@MBE_NPBD0@Z @game/LODModel
// FUNCTION: STBC 0x42D9E0 ?IsKeysEqual@?$NiTMap@PBDPAD@@MBE_NPBD0@Z @game/TGAnimationManagerClass
// FUNCTION: STBC 0x71DD70 ?IsKeysEqual@?$NiTMap@PBDPAVTGConfigValue@@@@MBE_NPBD0@Z @game/TGConfigMapping
// FUNCTION: STBC 0x7C4A80 ?IsKeysEqual@?$NiTMap@IPAVNiD3DGeometryData@@@@MBE_NII@Z
// FUNCTION: STBC 0x7D1D60 ?IsKeysEqual@?$NiTMap@IPAVNiD3DLightEntry@@@@MBE_NII@Z
// FUNCTION: STBC 0x7D82E0 ?IsKeysEqual@?$NiTMap@IPAVNiD3DMaterialEntry@@@@MBE_NII@Z
// FUNCTION: STBC 0x44ECF0 ?IsKeysEqual@?$NiTMap@PBDV?$NiPointer@VNiImage@@@@@@MBE_NPBD0@Z
// FUNCTION: STBC 0x7D7890 ?IsKeysEqual@?$NiTMap@PAUIDirectDraw7@@PAVNiD3DTextureManager@@@@MBE_NPAUIDirectDraw7@@0@Z
// FUNCTION: STBC 0x7CBCF0 ?IsKeysEqual@?$NiTMap@IPAVNiD3DVertexBufferEntry@@@@MBE_NII@Z
// FUNCTION: STBC 0x6EFCE0 ?IsKeysEqual@?$NiTMap@JPAX@@MBE_NJJ@Z @game/TGObjectModule
// FUNCTION: STBC 0x70A580 ?IsKeysEqual@?$NiTMap@PBDPAVTGSound@@@@MBE_NPBD0@Z @game/TGSoundModule
// FUNCTION: STBC 0x70E8C0 ?IsKeysEqual@?$NiTMap@PBDPAVTGSoundRegion@@@@MBE_NPBD0@Z @game/TGSoundRegion
// FUNCTION: STBC 0x70A7F0 ?IsKeysEqual@?$NiTMap@PAVNiProvider_Info@@PAUTGSamplePool@@@@MBE_NPAVNiProvider_Info@@0@Z @game/TGSoundModule
// FUNCTION: STBC 0x477F40 ?IsKeysEqual@?$NiTMap@PBDPAUBuilderAIBlock@@@@MBE_NPBD0@Z @game/BuilderAI
// FUNCTION: STBC 0x47A4D0 ?IsKeysEqual@?$NiTMap@PBDPAV?$NiTList@PAU_object@@@@@@MBE_NPBD0@Z @game/ConditionalAI
// FUNCTION: STBC 0x471A80 ?IsKeysEqual@?$NiTMap@JPAVArtificialIntelligence@@@@MBE_NJJ@Z @game/ArtificialIntelligence
// FUNCTION: STBC 0x7A51E0 ?IsKeysEqual@?$NiTMap@PBDPAVSequence@NiKeyframeManager@@@@MBE_NPBD0@Z @netimmerse/NiKeyframeManager
// FUNCTION: STBC 0x4F4150 ?IsKeysEqual@?$NiTMap@PBDPAVTextureAnimContainer@@@@MBE_NPBD0@Z @game/TextureAnimManager
// FUNCTION: STBC 0x48B030 ?IsKeysEqual@?$NiTMap@JM@@MBE_NJJ@Z @game/OptimizedSelectTarget
// FUNCTION: STBC 0x6A6EE0 ?IsKeysEqual@?$NiTMap@HPAVTGMessage@@@@MBE_NHH@Z @game/NetFile
// FUNCTION: STBC 0x6A7030 ?IsKeysEqual@?$NiTMap@HPAV?$NiTList@PAVTGMessage@@@@@@MBE_NHH@Z @game/NetFile
// FUNCTION: STBC 0x6A7160 ?IsKeysEqual@?$NiTMap@HPAVNetFileNameList@@@@MBE_NHH@Z @game/NetFile
// FUNCTION: STBC 0x6A9050 ?IsKeysEqual@?$NiTMap@IPAUUnknownClass6A8C50Item@@@@MBE_NII@Z @game/UnknownClass6A8C50
// FUNCTION: STBC 0x428C90 ?IsKeysEqual@?$NiTMap@PBDVNiPoint3@@@@MBE_NPBD0@Z @game/CameraMode
// FUNCTION: STBC 0x428990 ?IsKeysEqual@?$NiTMap@PBDM@@MBE_NPBD0@Z @game/CameraMode
// FUNCTION: STBC 0x428EF0 ?IsKeysEqual@?$NiTMap@PBDP6APAVCameraMode@@PAVCameraObjectClass@@@Z@@MBE_NPBD0@Z @game/CameraMode
// FUNCTION: STBC 0x4172F0 ?IsKeysEqual@?$NiTMap@PBDJ@@MBE_NPBD0@Z @game/CameraMode
// FUNCTION: STBC 0x4C40F0 ?IsKeysEqual@?$NiTMap@_JPAUDamageIndexBlock@@@@MBE_N_J0@Z @game/DamagePolygonizer
// FUNCTION: STBC 0x4C3EC0 ?IsKeysEqual@?$NiTMap@_JVVertexInfo@MetaVolumePolygonizer@@@@MBE_N_J0@Z @game/DamagePolygonizer
// FUNCTION: STBC 0x4C3D30 ?IsKeysEqual@?$NiTMap@_JUDamageVertexIndices@@@@MBE_N_J0@Z @game/DamagePolygonizer
// FUNCTION: STBC 0x6D84D0 ?IsKeysEqual@?$NiTMap@HPAVTGHandler@@@@MBE_NHH@Z @game/TGEventHandlerTable
// FUNCTION: STBC 0x6DB800 ?IsKeysEqual@?$NiTMap@HPAVTGBroadcastHandlerList@@@@MBE_NHH@Z @game/TGBroadcastHandlerTable
// FUNCTION: STBC 0x6E75D0 ?IsKeysEqual@?$NiTMap@HH@@MBE_NHH@Z @game/TGInputManager
// FUNCTION: STBC 0x6E78F0 ?IsKeysEqual@?$NiTMap@HVTGString@@@@MBE_NHH@Z @game/TGInputManager
// FUNCTION: STBC 0x719BB0 ?IsKeysEqual@?$NiTMap@JPAUTGTiming@@@@MBE_NJJ@Z @game/TGProfilingInfo
// FUNCTION: STBC 0x665630 ?IsKeysEqual@?$NiTMap@PBDPAUBridgeTexture@@@@MBE_NPBD0@Z @game/BridgeObjectClass
// FUNCTION: STBC 0x66F2A0 ?IsKeysEqual@?$NiTMap@HPAVTGParagraph@@@@MBE_NHH@Z @game/CharacterClass
// FUNCTION: STBC 0x66F460 ?IsKeysEqual@?$NiTMap@HPAUPhonemeList@@@@MBE_NHH@Z @game/CharacterClass
// FUNCTION: STBC 0x6D6F30 ?IsKeysEqual@?$NiTMap@PAVTGObject@@PAV?$NiTList@PAVTGEvent@@@@@@MBE_NPAVTGObject@@0@Z @game/TGEvent
// FUNCTION: STBC 0x4BB2C0 ?IsKeysEqual@?$NiTMap@_JG@@MBE_N_J0@Z @game/VisibleDamageBuilder
// FUNCTION: STBC 0x6D6FB0 ?IsKeysEqual@?$NiTMap@JPAVTGEvent@@@@MBE_NJJ@Z @game/TGEvent
// FUNCTION: STBC 0x6DD770 ?IsKeysEqual@?$NiTMap@JPAVTGTimer@@@@MBE_NJJ@Z @game/TGTimer
// FUNCTION: STBC 0x475440 ?IsKeysEqual@?$NiTMap@PBDPAUAILogScope@@@@MBE_NPBD0@Z @game/AILog
// FUNCTION: STBC 0x4581F0 ?IsKeysEqual@?$NiTMap@PBDV?$NiPointer@VTGOverlayImage@@@@@@MBE_NPBD0@Z @game/TGOverlayController
// FUNCTION: STBC 0x54F5F0 ?IsKeysEqual@?$NiTMap@PAVSTNumericBar@@J@@MBE_NPAVSTNumericBar@@0@Z @game/EngPowerCtrl
// FUNCTION: STBC 0x723490 ?IsKeysEqual@?$NiTMap@PBD_N@@MBE_NPBD0@Z @game/TGTraceStream
// FUNCTION: STBC 0x4A3880 ?IsKeysEqual@?$NiTMap@PBDPAV?$SortedArray@VPlacementObject@@J@@@@MBE_NPBD0@Z @game/PlacementObject
// FUNCTION: STBC 0x70D050 ?IsKeysEqual@?$NiTMap@PBDPAVTGSoundFile@@@@MBE_NPBD0@Z @game/TGSound
// FUNCTION: STBC 0x7199C0 ?IsKeysEqual@?$NiTMap@PBDPAUTGTimingSamples@@@@MBE_NPBD0@Z @game/TGProfilingInfo
// FUNCTION: STBC 0x72AE90 ?IsKeysEqual@?$NiTMap@HPAVTGIconLocation@@@@MBE_NHH@Z @game/TGIconManager
// FUNCTION: STBC 0x486680 ?IsKeysEqual@?$NiTMap@JPAUFireSubsystemRating@@@@MBE_NJJ@Z @game/OptimizedFireScript
template <class TKEY, class TVAL> inline bool NiTMap<TKEY, TVAL>::IsKeysEqual( TKEY key1, TKEY key2 ) const {
	return key1 == key2;
}

/*
================
NiTMap::SetValue

Fills a new or reused item.
================
*/
// FUNCTION: STBC 0x4555E0 ?SetValue@?$NiTMap@PBDP6AXAAVNiStream@@AAPAVNiObject@@@Z@@MAEXPAV?$NiTMapItem@PBDP6AXAAVNiStream@@AAPAVNiObject@@@Z@@PBDP6AXAAVNiStream@@AAPAVNiObject@@@Z@Z
// FUNCTION: STBC 0x4EFEF0 ?SetValue@?$NiTMap@PAVNiTriShape@@PAVPassShape@@@@MAEXPAV?$NiTMapItem@PAVNiTriShape@@PAVPassShape@@@@PAVNiTriShape@@PAVPassShape@@@Z @game/SpecularPass
// FUNCTION: STBC 0x42DE20 ?SetValue@?$NiTMap@PBDPAVCameraMode@@@@MAEXPAV?$NiTMapItem@PBDPAVCameraMode@@@@PBDPAVCameraMode@@@Z @game/CameraObjectClass
// FUNCTION: STBC 0x58C730 ?SetValue@?$NiTMap@PBDPAVBackdrop@@@@MAEXPAV?$NiTMapItem@PBDPAVBackdrop@@@@PBDPAVBackdrop@@@Z @game/Backdrop
// FUNCTION: STBC 0x454310 ?SetValue@?$NiTMap@PBDPAVLODModel@@@@MAEXPAV?$NiTMapItem@PBDPAVLODModel@@@@PBDPAVLODModel@@@Z @game/LODModel
// FUNCTION: STBC 0x42DA00 ?SetValue@?$NiTMap@PBDPAD@@MAEXPAV?$NiTMapItem@PBDPAD@@PBDPAD@Z @game/TGAnimationManagerClass
// FUNCTION: STBC 0x71DD90 ?SetValue@?$NiTMap@PBDPAVTGConfigValue@@@@MAEXPAV?$NiTMapItem@PBDPAVTGConfigValue@@@@PBDPAVTGConfigValue@@@Z @game/TGConfigMapping
// FUNCTION: STBC 0x7C4AA0 ?SetValue@?$NiTMap@IPAVNiD3DGeometryData@@@@MAEXPAV?$NiTMapItem@IPAVNiD3DGeometryData@@@@IPAVNiD3DGeometryData@@@Z
// FUNCTION: STBC 0x7D1D80 ?SetValue@?$NiTMap@IPAVNiD3DLightEntry@@@@MAEXPAV?$NiTMapItem@IPAVNiD3DLightEntry@@@@IPAVNiD3DLightEntry@@@Z
// FUNCTION: STBC 0x7D8300 ?SetValue@?$NiTMap@IPAVNiD3DMaterialEntry@@@@MAEXPAV?$NiTMapItem@IPAVNiD3DMaterialEntry@@@@IPAVNiD3DMaterialEntry@@@Z
// FUNCTION: STBC 0x44ED10 ?SetValue@?$NiTMap@PBDV?$NiPointer@VNiImage@@@@@@MAEXPAV?$NiTMapItem@PBDV?$NiPointer@VNiImage@@@@@@PBDV?$NiPointer@VNiImage@@@@@Z
// FUNCTION: STBC 0x7D78B0 ?SetValue@?$NiTMap@PAUIDirectDraw7@@PAVNiD3DTextureManager@@@@MAEXPAV?$NiTMapItem@PAUIDirectDraw7@@PAVNiD3DTextureManager@@@@PAUIDirectDraw7@@PAVNiD3DTextureManager@@@Z
// FUNCTION: STBC 0x7CBD10 ?SetValue@?$NiTMap@IPAVNiD3DVertexBufferEntry@@@@MAEXPAV?$NiTMapItem@IPAVNiD3DVertexBufferEntry@@@@IPAVNiD3DVertexBufferEntry@@@Z
// FUNCTION: STBC 0x6EFD00 ?SetValue@?$NiTMap@JPAX@@MAEXPAV?$NiTMapItem@JPAX@@JPAX@Z @game/TGObjectModule
// FUNCTION: STBC 0x70A5A0 ?SetValue@?$NiTMap@PBDPAVTGSound@@@@MAEXPAV?$NiTMapItem@PBDPAVTGSound@@@@PBDPAVTGSound@@@Z @game/TGSoundModule
// FUNCTION: STBC 0x70E8E0 ?SetValue@?$NiTMap@PBDPAVTGSoundRegion@@@@MAEXPAV?$NiTMapItem@PBDPAVTGSoundRegion@@@@PBDPAVTGSoundRegion@@@Z @game/TGSoundRegion
// FUNCTION: STBC 0x70A810 ?SetValue@?$NiTMap@PAVNiProvider_Info@@PAUTGSamplePool@@@@MAEXPAV?$NiTMapItem@PAVNiProvider_Info@@PAUTGSamplePool@@@@PAVNiProvider_Info@@PAUTGSamplePool@@@Z @game/TGSoundModule
// FUNCTION: STBC 0x477F60 ?SetValue@?$NiTMap@PBDPAUBuilderAIBlock@@@@MAEXPAV?$NiTMapItem@PBDPAUBuilderAIBlock@@@@PBDPAUBuilderAIBlock@@@Z @game/BuilderAI
// FUNCTION: STBC 0x47A4F0 ?SetValue@?$NiTMap@PBDPAV?$NiTList@PAU_object@@@@@@MAEXPAV?$NiTMapItem@PBDPAV?$NiTList@PAU_object@@@@@@PBDPAV?$NiTList@PAU_object@@@@@Z @game/ConditionalAI
// FUNCTION: STBC 0x471AA0 ?SetValue@?$NiTMap@JPAVArtificialIntelligence@@@@MAEXPAV?$NiTMapItem@JPAVArtificialIntelligence@@@@JPAVArtificialIntelligence@@@Z @game/ArtificialIntelligence
// FUNCTION: STBC 0x7A5200 ?SetValue@?$NiTMap@PBDPAVSequence@NiKeyframeManager@@@@MAEXPAV?$NiTMapItem@PBDPAVSequence@NiKeyframeManager@@@@PBDPAVSequence@NiKeyframeManager@@@Z @netimmerse/NiKeyframeManager
// FUNCTION: STBC 0x4F4170 ?SetValue@?$NiTMap@PBDPAVTextureAnimContainer@@@@MAEXPAV?$NiTMapItem@PBDPAVTextureAnimContainer@@@@PBDPAVTextureAnimContainer@@@Z @game/TextureAnimManager
// FUNCTION: STBC 0x48B050 ?SetValue@?$NiTMap@JM@@MAEXPAV?$NiTMapItem@JM@@JM@Z @game/OptimizedSelectTarget
// FUNCTION: STBC 0x6A6F00 ?SetValue@?$NiTMap@HPAVTGMessage@@@@MAEXPAV?$NiTMapItem@HPAVTGMessage@@@@HPAVTGMessage@@@Z @game/NetFile
// FUNCTION: STBC 0x6A7050 ?SetValue@?$NiTMap@HPAV?$NiTList@PAVTGMessage@@@@@@MAEXPAV?$NiTMapItem@HPAV?$NiTList@PAVTGMessage@@@@@@HPAV?$NiTList@PAVTGMessage@@@@@Z @game/NetFile
// FUNCTION: STBC 0x6A7180 ?SetValue@?$NiTMap@HPAVNetFileNameList@@@@MAEXPAV?$NiTMapItem@HPAVNetFileNameList@@@@HPAVNetFileNameList@@@Z @game/NetFile
// FUNCTION: STBC 0x6A9070 ?SetValue@?$NiTMap@IPAUUnknownClass6A8C50Item@@@@MAEXPAV?$NiTMapItem@IPAUUnknownClass6A8C50Item@@@@IPAUUnknownClass6A8C50Item@@@Z @game/UnknownClass6A8C50
// FUNCTION: STBC 0x428CB0 ?SetValue@?$NiTMap@PBDVNiPoint3@@@@MAEXPAV?$NiTMapItem@PBDVNiPoint3@@@@PBDVNiPoint3@@@Z @game/CameraMode
// FUNCTION: STBC 0x4289B0 ?SetValue@?$NiTMap@PBDM@@MAEXPAV?$NiTMapItem@PBDM@@PBDM@Z @game/CameraMode
// FUNCTION: STBC 0x428F10 ?SetValue@?$NiTMap@PBDP6APAVCameraMode@@PAVCameraObjectClass@@@Z@@MAEXPAV?$NiTMapItem@PBDP6APAVCameraMode@@PAVCameraObjectClass@@@Z@@PBDP6APAVCameraMode@@PAVCameraObjectClass@@@Z@Z @game/CameraMode
// FUNCTION: STBC 0x417310 ?SetValue@?$NiTMap@PBDJ@@MAEXPAV?$NiTMapItem@PBDJ@@PBDJ@Z @game/CameraMode
// FUNCTION: STBC 0x4C4120 ?SetValue@?$NiTMap@_JPAUDamageIndexBlock@@@@MAEXPAV?$NiTMapItem@_JPAUDamageIndexBlock@@@@_JPAUDamageIndexBlock@@@Z @game/DamagePolygonizer
// FUNCTION: STBC 0x4C3EF0 ?SetValue@?$NiTMap@_JVVertexInfo@MetaVolumePolygonizer@@@@MAEXPAV?$NiTMapItem@_JVVertexInfo@MetaVolumePolygonizer@@@@_JVVertexInfo@MetaVolumePolygonizer@@@Z @game/DamagePolygonizer
// FUNCTION: STBC 0x6D84F0 ?SetValue@?$NiTMap@HPAVTGHandler@@@@MAEXPAV?$NiTMapItem@HPAVTGHandler@@@@HPAVTGHandler@@@Z @game/TGEventHandlerTable
// FUNCTION: STBC 0x6DB820 ?SetValue@?$NiTMap@HPAVTGBroadcastHandlerList@@@@MAEXPAV?$NiTMapItem@HPAVTGBroadcastHandlerList@@@@HPAVTGBroadcastHandlerList@@@Z @game/TGBroadcastHandlerTable
// FUNCTION: STBC 0x6E75F0 ?SetValue@?$NiTMap@HH@@MAEXPAV?$NiTMapItem@HH@@HH@Z @game/TGInputManager
// FUNCTION: STBC 0x6E7910 ?SetValue@?$NiTMap@HVTGString@@@@MAEXPAV?$NiTMapItem@HVTGString@@@@HVTGString@@@Z @game/TGInputManager
// FUNCTION: STBC 0x719BD0 ?SetValue@?$NiTMap@JPAUTGTiming@@@@MAEXPAV?$NiTMapItem@JPAUTGTiming@@@@JPAUTGTiming@@@Z @game/TGProfilingInfo
// FUNCTION: STBC 0x665650 ?SetValue@?$NiTMap@PBDPAUBridgeTexture@@@@MAEXPAV?$NiTMapItem@PBDPAUBridgeTexture@@@@PBDPAUBridgeTexture@@@Z @game/BridgeObjectClass
// FUNCTION: STBC 0x66F2C0 ?SetValue@?$NiTMap@HPAVTGParagraph@@@@MAEXPAV?$NiTMapItem@HPAVTGParagraph@@@@HPAVTGParagraph@@@Z @game/CharacterClass
// FUNCTION: STBC 0x66F480 ?SetValue@?$NiTMap@HPAUPhonemeList@@@@MAEXPAV?$NiTMapItem@HPAUPhonemeList@@@@HPAUPhonemeList@@@Z @game/CharacterClass
// FUNCTION: STBC 0x6D6F50 ?SetValue@?$NiTMap@PAVTGObject@@PAV?$NiTList@PAVTGEvent@@@@@@MAEXPAV?$NiTMapItem@PAVTGObject@@PAV?$NiTList@PAVTGEvent@@@@@@PAVTGObject@@PAV?$NiTList@PAVTGEvent@@@@@Z @game/TGEvent
// FUNCTION: STBC 0x4BB320 ?SetValue@?$NiTMap@_JG@@MAEXPAV?$NiTMapItem@_JG@@_JG@Z @game/VisibleDamageBuilder
// FUNCTION: STBC 0x4C3D60 ?SetValue@?$NiTMap@_JUDamageVertexIndices@@@@MAEXPAV?$NiTMapItem@_JUDamageVertexIndices@@@@_JUDamageVertexIndices@@@Z @game/DamagePolygonizer
// FUNCTION: STBC 0x6D6FD0 ?SetValue@?$NiTMap@JPAVTGEvent@@@@MAEXPAV?$NiTMapItem@JPAVTGEvent@@@@JPAVTGEvent@@@Z @game/TGEvent
// FUNCTION: STBC 0x6DD790 ?SetValue@?$NiTMap@JPAVTGTimer@@@@MAEXPAV?$NiTMapItem@JPAVTGTimer@@@@JPAVTGTimer@@@Z @game/TGTimer
// FUNCTION: STBC 0x475460 ?SetValue@?$NiTMap@PBDPAUAILogScope@@@@MAEXPAV?$NiTMapItem@PBDPAUAILogScope@@@@PBDPAUAILogScope@@@Z @game/AILog
// FUNCTION: STBC 0x458210 ?SetValue@?$NiTMap@PBDV?$NiPointer@VTGOverlayImage@@@@@@MAEXPAV?$NiTMapItem@PBDV?$NiPointer@VTGOverlayImage@@@@@@PBDV?$NiPointer@VTGOverlayImage@@@@@Z @game/TGOverlayController
// FUNCTION: STBC 0x54F610 ?SetValue@?$NiTMap@PAVSTNumericBar@@J@@MAEXPAV?$NiTMapItem@PAVSTNumericBar@@J@@PAVSTNumericBar@@J@Z @game/EngPowerCtrl
// FUNCTION: STBC 0x7234B0 ?SetValue@?$NiTMap@PBD_N@@MAEXPAV?$NiTMapItem@PBD_N@@PBD_N@Z @game/TGTraceStream
// FUNCTION: STBC 0x4A38A0 ?SetValue@?$NiTMap@PBDPAV?$SortedArray@VPlacementObject@@J@@@@MAEXPAV?$NiTMapItem@PBDPAV?$SortedArray@VPlacementObject@@J@@@@PBDPAV?$SortedArray@VPlacementObject@@J@@@Z @game/PlacementObject
// FUNCTION: STBC 0x70D070 ?SetValue@?$NiTMap@PBDPAVTGSoundFile@@@@MAEXPAV?$NiTMapItem@PBDPAVTGSoundFile@@@@PBDPAVTGSoundFile@@@Z @game/TGSound
// FUNCTION: STBC 0x7199E0 ?SetValue@?$NiTMap@PBDPAUTGTimingSamples@@@@MAEXPAV?$NiTMapItem@PBDPAUTGTimingSamples@@@@PBDPAUTGTimingSamples@@@Z @game/TGProfilingInfo
// FUNCTION: STBC 0x72AEB0 ?SetValue@?$NiTMap@HPAVTGIconLocation@@@@MAEXPAV?$NiTMapItem@HPAVTGIconLocation@@@@HPAVTGIconLocation@@@Z @game/TGIconManager
// FUNCTION: STBC 0x4866A0 ?SetValue@?$NiTMap@JPAUFireSubsystemRating@@@@MAEXPAV?$NiTMapItem@JPAUFireSubsystemRating@@@@JPAUFireSubsystemRating@@@Z @game/OptimizedFireScript
template <class TKEY, class TVAL> inline void NiTMap<TKEY, TVAL>::SetValue( NiTMapItem<TKEY, TVAL> *item, TKEY key, TVAL val ) {
	item->key = key;
	item->val = val;
}

/*
================
NiTMap::ClearValue

Releases what SetValue took for an item; nothing for plain keys.
================
*/
// FUNCTION: STBC 0x455600 ?ClearValue@?$NiTMap@PBDP6AXAAVNiStream@@AAPAVNiObject@@@Z@@MAEXPAV?$NiTMapItem@PBDP6AXAAVNiStream@@AAPAVNiObject@@@Z@@@Z
// FUNCTION: STBC 0x4EFF30 ?ClearValue@?$NiTMap@PAVNiTriShape@@PAVPassShape@@@@MAEXPAV?$NiTMapItem@PAVNiTriShape@@PAVPassShape@@@@@Z @game/SpecularPass
// FUNCTION: STBC 0x42DE40 ?ClearValue@?$NiTMap@PBDPAVCameraMode@@@@MAEXPAV?$NiTMapItem@PBDPAVCameraMode@@@@@Z @game/CameraObjectClass
// FUNCTION: STBC 0x58C750 ?ClearValue@?$NiTMap@PBDPAVBackdrop@@@@MAEXPAV?$NiTMapItem@PBDPAVBackdrop@@@@@Z @game/Backdrop
// FUNCTION: STBC 0x42DA20 ?ClearValue@?$NiTMap@PBDPAD@@MAEXPAV?$NiTMapItem@PBDPAD@@@Z @game/TGAnimationManagerClass
// FUNCTION: STBC 0x71DDB0 ?ClearValue@?$NiTMap@PBDPAVTGConfigValue@@@@MAEXPAV?$NiTMapItem@PBDPAVTGConfigValue@@@@@Z @game/TGConfigMapping
// FUNCTION: STBC 0x7C4AC0 ?ClearValue@?$NiTMap@IPAVNiD3DGeometryData@@@@MAEXPAV?$NiTMapItem@IPAVNiD3DGeometryData@@@@@Z
// FUNCTION: STBC 0x7D1DA0 ?ClearValue@?$NiTMap@IPAVNiD3DLightEntry@@@@MAEXPAV?$NiTMapItem@IPAVNiD3DLightEntry@@@@@Z
// FUNCTION: STBC 0x7D8320 ?ClearValue@?$NiTMap@IPAVNiD3DMaterialEntry@@@@MAEXPAV?$NiTMapItem@IPAVNiD3DMaterialEntry@@@@@Z
// FUNCTION: STBC 0x44EDA0 ?ClearValue@?$NiTMap@PBDV?$NiPointer@VNiImage@@@@@@MAEXPAV?$NiTMapItem@PBDV?$NiPointer@VNiImage@@@@@@@Z
// FUNCTION: STBC 0x7D78D0 ?ClearValue@?$NiTMap@PAUIDirectDraw7@@PAVNiD3DTextureManager@@@@MAEXPAV?$NiTMapItem@PAUIDirectDraw7@@PAVNiD3DTextureManager@@@@@Z
// FUNCTION: STBC 0x7CBD30 ?ClearValue@?$NiTMap@IPAVNiD3DVertexBufferEntry@@@@MAEXPAV?$NiTMapItem@IPAVNiD3DVertexBufferEntry@@@@@Z
// FUNCTION: STBC 0x6EFD20 ?ClearValue@?$NiTMap@JPAX@@MAEXPAV?$NiTMapItem@JPAX@@@Z @game/TGObjectModule
// FUNCTION: STBC 0x70A5C0 ?ClearValue@?$NiTMap@PBDPAVTGSound@@@@MAEXPAV?$NiTMapItem@PBDPAVTGSound@@@@@Z @game/TGSoundModule
// FUNCTION: STBC 0x70E900 ?ClearValue@?$NiTMap@PBDPAVTGSoundRegion@@@@MAEXPAV?$NiTMapItem@PBDPAVTGSoundRegion@@@@@Z @game/TGSoundRegion
// FUNCTION: STBC 0x70A830 ?ClearValue@?$NiTMap@PAVNiProvider_Info@@PAUTGSamplePool@@@@MAEXPAV?$NiTMapItem@PAVNiProvider_Info@@PAUTGSamplePool@@@@@Z @game/TGSoundModule
// FUNCTION: STBC 0x477F80 ?ClearValue@?$NiTMap@PBDPAUBuilderAIBlock@@@@MAEXPAV?$NiTMapItem@PBDPAUBuilderAIBlock@@@@@Z @game/BuilderAI
// FUNCTION: STBC 0x47A530 ?ClearValue@?$NiTMap@PBDPAV?$NiTList@PAU_object@@@@@@MAEXPAV?$NiTMapItem@PBDPAV?$NiTList@PAU_object@@@@@@@Z @game/ConditionalAI
// FUNCTION: STBC 0x471AE0 ?ClearValue@?$NiTMap@JPAVArtificialIntelligence@@@@MAEXPAV?$NiTMapItem@JPAVArtificialIntelligence@@@@@Z @game/ArtificialIntelligence
// FUNCTION: STBC 0x7A5220 ?ClearValue@?$NiTMap@PBDPAVSequence@NiKeyframeManager@@@@MAEXPAV?$NiTMapItem@PBDPAVSequence@NiKeyframeManager@@@@@Z @netimmerse/NiKeyframeManager
// FUNCTION: STBC 0x4F4190 ?ClearValue@?$NiTMap@PBDPAVTextureAnimContainer@@@@MAEXPAV?$NiTMapItem@PBDPAVTextureAnimContainer@@@@@Z @game/TextureAnimManager
// FUNCTION: STBC 0x48B070 ?ClearValue@?$NiTMap@JM@@MAEXPAV?$NiTMapItem@JM@@@Z @game/OptimizedSelectTarget
// FUNCTION: STBC 0x6A6F20 ?ClearValue@?$NiTMap@HPAVTGMessage@@@@MAEXPAV?$NiTMapItem@HPAVTGMessage@@@@@Z @game/NetFile
// FUNCTION: STBC 0x6A7070 ?ClearValue@?$NiTMap@HPAV?$NiTList@PAVTGMessage@@@@@@MAEXPAV?$NiTMapItem@HPAV?$NiTList@PAVTGMessage@@@@@@@Z @game/NetFile
// FUNCTION: STBC 0x6A71A0 ?ClearValue@?$NiTMap@HPAVNetFileNameList@@@@MAEXPAV?$NiTMapItem@HPAVNetFileNameList@@@@@Z @game/NetFile
// FUNCTION: STBC 0x6A9090 ?ClearValue@?$NiTMap@IPAUUnknownClass6A8C50Item@@@@MAEXPAV?$NiTMapItem@IPAUUnknownClass6A8C50Item@@@@@Z @game/UnknownClass6A8C50
// FUNCTION: STBC 0x428CE0 ?ClearValue@?$NiTMap@PBDVNiPoint3@@@@MAEXPAV?$NiTMapItem@PBDVNiPoint3@@@@@Z @game/CameraMode
// FUNCTION: STBC 0x4289D0 ?ClearValue@?$NiTMap@PBDM@@MAEXPAV?$NiTMapItem@PBDM@@@Z @game/CameraMode
// FUNCTION: STBC 0x428F30 ?ClearValue@?$NiTMap@PBDP6APAVCameraMode@@PAVCameraObjectClass@@@Z@@MAEXPAV?$NiTMapItem@PBDP6APAVCameraMode@@PAVCameraObjectClass@@@Z@@@Z @game/CameraMode
// FUNCTION: STBC 0x417330 ?ClearValue@?$NiTMap@PBDJ@@MAEXPAV?$NiTMapItem@PBDJ@@@Z @game/CameraMode
// FUNCTION: STBC 0x4C4140 ?ClearValue@?$NiTMap@_JPAUDamageIndexBlock@@@@MAEXPAV?$NiTMapItem@_JPAUDamageIndexBlock@@@@@Z @game/DamagePolygonizer
// FUNCTION: STBC 0x4C3F10 ?ClearValue@?$NiTMap@_JVVertexInfo@MetaVolumePolygonizer@@@@MAEXPAV?$NiTMapItem@_JVVertexInfo@MetaVolumePolygonizer@@@@@Z @game/DamagePolygonizer
// FUNCTION: STBC 0x4C3D90 ?ClearValue@?$NiTMap@_JUDamageVertexIndices@@@@MAEXPAV?$NiTMapItem@_JUDamageVertexIndices@@@@@Z @game/DamagePolygonizer
// FUNCTION: STBC 0x6D8510 ?ClearValue@?$NiTMap@HPAVTGHandler@@@@MAEXPAV?$NiTMapItem@HPAVTGHandler@@@@@Z @game/TGEventHandlerTable
// FUNCTION: STBC 0x6DB840 ?ClearValue@?$NiTMap@HPAVTGBroadcastHandlerList@@@@MAEXPAV?$NiTMapItem@HPAVTGBroadcastHandlerList@@@@@Z @game/TGBroadcastHandlerTable
// FUNCTION: STBC 0x6E7610 ?ClearValue@?$NiTMap@HH@@MAEXPAV?$NiTMapItem@HH@@@Z @game/TGInputManager
// FUNCTION: STBC 0x6E7970 ?ClearValue@?$NiTMap@HVTGString@@@@MAEXPAV?$NiTMapItem@HVTGString@@@@@Z @game/TGInputManager
// FUNCTION: STBC 0x719BF0 ?ClearValue@?$NiTMap@JPAUTGTiming@@@@MAEXPAV?$NiTMapItem@JPAUTGTiming@@@@@Z @game/TGProfilingInfo
// FUNCTION: STBC 0x665670 ?ClearValue@?$NiTMap@PBDPAUBridgeTexture@@@@MAEXPAV?$NiTMapItem@PBDPAUBridgeTexture@@@@@Z @game/BridgeObjectClass
// FUNCTION: STBC 0x66F2E0 ?ClearValue@?$NiTMap@HPAVTGParagraph@@@@MAEXPAV?$NiTMapItem@HPAVTGParagraph@@@@@Z @game/CharacterClass
// FUNCTION: STBC 0x66F4A0 ?ClearValue@?$NiTMap@HPAUPhonemeList@@@@MAEXPAV?$NiTMapItem@HPAUPhonemeList@@@@@Z @game/CharacterClass
// FUNCTION: STBC 0x6D6F70 ?ClearValue@?$NiTMap@PAVTGObject@@PAV?$NiTList@PAVTGEvent@@@@@@MAEXPAV?$NiTMapItem@PAVTGObject@@PAV?$NiTList@PAVTGEvent@@@@@@@Z @game/TGEvent
// FUNCTION: STBC 0x6D6FF0 ?ClearValue@?$NiTMap@JPAVTGEvent@@@@MAEXPAV?$NiTMapItem@JPAVTGEvent@@@@@Z @game/TGEvent
// FUNCTION: STBC 0x6DD7B0 ?ClearValue@?$NiTMap@JPAVTGTimer@@@@MAEXPAV?$NiTMapItem@JPAVTGTimer@@@@@Z @game/TGTimer
// FUNCTION: STBC 0x475480 ?ClearValue@?$NiTMap@PBDPAUAILogScope@@@@MAEXPAV?$NiTMapItem@PBDPAUAILogScope@@@@@Z @game/AILog
// FUNCTION: STBC 0x4582A0 ?ClearValue@?$NiTMap@PBDV?$NiPointer@VTGOverlayImage@@@@@@MAEXPAV?$NiTMapItem@PBDV?$NiPointer@VTGOverlayImage@@@@@@@Z @game/TGOverlayController
// FUNCTION: STBC 0x54F630 ?ClearValue@?$NiTMap@PAVSTNumericBar@@J@@MAEXPAV?$NiTMapItem@PAVSTNumericBar@@J@@@Z @game/EngPowerCtrl
// FUNCTION: STBC 0x7234D0 ?ClearValue@?$NiTMap@PBD_N@@MAEXPAV?$NiTMapItem@PBD_N@@@Z @game/TGTraceStream
// FUNCTION: STBC 0x4A38C0 ?ClearValue@?$NiTMap@PBDPAV?$SortedArray@VPlacementObject@@J@@@@MAEXPAV?$NiTMapItem@PBDPAV?$SortedArray@VPlacementObject@@J@@@@@Z @game/PlacementObject
// FUNCTION: STBC 0x719D00 ??_G?$NiTMap@PBDPAUTGTimingSamples@@@@UAEPAXI@Z @game/TGProfilingInfo
// FUNCTION: STBC 0x70AB50 ?KeyToHashIndex@?$NiTMap@PBDPAUTGMusicPeriod@@@@MBEIPBD@Z @game/TGMusic
// FUNCTION: STBC 0x70AB60 ?IsKeysEqual@?$NiTMap@PBDPAUTGMusicPeriod@@@@MBE_NPBD0@Z @game/TGMusic
// FUNCTION: STBC 0x70AB80 ?SetValue@?$NiTMap@PBDPAUTGMusicPeriod@@@@MAEXPAV?$NiTMapItem@PBDPAUTGMusicPeriod@@@@PBDPAUTGMusicPeriod@@@Z @game/TGMusic
// FUNCTION: STBC 0x70ABA0 ?ClearValue@?$NiTMap@PBDPAUTGMusicPeriod@@@@MAEXPAV?$NiTMapItem@PBDPAUTGMusicPeriod@@@@@Z @game/TGMusic
// FUNCTION: STBC 0x70D090 ?ClearValue@?$NiTMap@PBDPAVTGSoundFile@@@@MAEXPAV?$NiTMapItem@PBDPAVTGSoundFile@@@@@Z @game/TGSound
// FUNCTION: STBC 0x719A00 ?ClearValue@?$NiTMap@PBDPAUTGTimingSamples@@@@MAEXPAV?$NiTMapItem@PBDPAUTGTimingSamples@@@@@Z @game/TGProfilingInfo
// FUNCTION: STBC 0x72AED0 ?ClearValue@?$NiTMap@HPAVTGIconLocation@@@@MAEXPAV?$NiTMapItem@HPAVTGIconLocation@@@@@Z @game/TGIconManager
// FUNCTION: STBC 0x4866C0 ?ClearValue@?$NiTMap@JPAUFireSubsystemRating@@@@MAEXPAV?$NiTMapItem@JPAUFireSubsystemRating@@@@@Z @game/OptimizedFireScript
template <class TKEY, class TVAL> inline void NiTMap<TKEY, TVAL>::ClearValue( NiTMapItem<TKEY, TVAL> *item ) {
}

/*
================
NiTMap::SetAt

Replaces the value of an existing key, or adds a new item at the head of
its bucket.
================
*/
// FUNCTION: STBC 0x4BB200 ?SetAt@?$NiTMap@_JG@@QAEX_JG@Z @game/VisibleDamageBuilder
// FUNCTION: STBC 0x4541F0 ?SetAt@?$NiTMap@PBDPAVLODModel@@@@QAEXPBDPAVLODModel@@@Z @game/LODModel
// FUNCTION: STBC 0x42DCA0 ?SetAt@?$NiTMap@PBDPAVCameraMode@@@@QAEXPBDPAVCameraMode@@@Z @game/CameraObjectClass
// FUNCTION: STBC 0x4288A0 ?SetAt@?$NiTMap@PBDM@@QAEXPBDM@Z @game/CameraMode
// FUNCTION: STBC 0x428BC0 ?SetAt@?$NiTMap@PBDVNiPoint3@@@@QAEXPBDVNiPoint3@@@Z @game/CameraMode
// FUNCTION: STBC 0x4C3E10 ?SetAt@?$NiTMap@_JVVertexInfo@MetaVolumePolygonizer@@@@QAEX_JVVertexInfo@MetaVolumePolygonizer@@@Z @game/DamagePolygonizer
// FUNCTION: STBC 0x477E40 ?SetAt@?$NiTMap@PBDPAU_object@@@@QAEXPBDPAU_object@@@Z @game/BuilderAI
// FUNCTION: STBC 0x417040 ?SetAt@?$NiTMap@JPAX@@QAEXJPAX@Z @game/SetClass
// FUNCTION: STBC 0x4468F0 ?SetAt@?$NiTMap@HP6APAVTGObject@@J@Z@@QAEXHP6APAVTGObject@@J@Z@Z @game/UtopiaModule
// FUNCTION: STBC 0x4580D0 ?SetAt@?$NiTMap@PBDV?$NiPointer@VTGOverlayImage@@@@@@QAEXPBDV?$NiPointer@VTGOverlayImage@@@@@Z @game/TGOverlayController
// FUNCTION: STBC 0x44EBD0 ?SetAt@?$NiTMap@PBDV?$NiPointer@VNiImage@@@@@@QAEXPBDV?$NiPointer@VNiImage@@@@@Z @game/ImageManagerClass
// FUNCTION: STBC 0x54F440 ?SetAt@?$NiTMap@PAVSTNumericBar@@J@@QAEXPAVSTNumericBar@@J@Z @game/EngPowerCtrl
// FUNCTION: STBC 0x6E7530 ?SetAt@?$NiTMap@HH@@QAEXHH@Z @game/TGInputManager
// FUNCTION: STBC 0x6E76F0 ?SetAt@?$NiTMap@HVTGString@@@@QAEXHVTGString@@@Z @game/TGInputManager
// FUNCTION: STBC 0x55B950 ?SetAt@?$NiTMap@HPAV?$NiTList@PAUKeyBind@@@@@@QAEXHPAV?$NiTList@PAUKeyBind@@@@@Z @game/KeyboardBinding
// FUNCTION: STBC 0x486520 ?SetAt@?$NiTMap@JPAUFireSubsystemRating@@@@QAEXJPAUFireSubsystemRating@@@Z @game/OptimizedFireScript
template <class TKEY, class TVAL> inline void NiTMap<TKEY, TVAL>::SetAt( TKEY key, TVAL val ) {
	unsigned int index = KeyToHashIndex( key );
	NiTMapItem<TKEY, TVAL> *item = hashTable[index];
	while ( item ) {
		if ( IsKeysEqual( key, item->key ) ) {
			ClearValue( item );
			SetValue( item, key, val );
			return;
		}
		item = item->next;
	}

	item = new NiTMapItem<TKEY, TVAL>;
	assert( item );
	SetValue( item, key, val );
	item->next = hashTable[index];
	hashTable[index] = item;
	count++;
}

/*
================
NiTMap::RemoveAt

False when the key is not in the map.
================
*/
// FUNCTION: STBC 0x55B9E0 ?RemoveAt@?$NiTMap@HPAV?$NiTList@PAUKeyBind@@@@@@QAE_NH@Z @game/KeyboardBinding
// FUNCTION: STBC 0x4C3FB0 ?RemoveAt@?$NiTMap@_JPAUDamageIndexBlock@@@@QAE_N_J@Z @game/DamagePolygonizer
template <class TKEY, class TVAL> inline bool NiTMap<TKEY, TVAL>::RemoveAt( TKEY key ) {
	unsigned int index = KeyToHashIndex( key );
	NiTMapItem<TKEY, TVAL> *item = hashTable[index];
	if ( item ) {
		if ( IsKeysEqual( key, item->key ) ) {
			hashTable[index] = item->next;
			ClearValue( item );
			delete item;
			count--;
			return true;
		}

		NiTMapItem<TKEY, TVAL> *prev = item;
		item = prev->next;
		while ( item && !IsKeysEqual( key, item->key ) ) {
			prev = item;
			item = item->next;
		}
		if ( item ) {
			prev->next = item->next;
			ClearValue( item );
			delete item;
			count--;
			return true;
		}
	}
	return false;
}

/*
================
NiTMap::RemoveAll
================
*/
// FUNCTION: STBC 0x42D8B0 ?RemoveAll@?$NiTMap@PBDPAD@@QAEXXZ @game/CameraObjectClass
// FUNCTION: STBC 0x429550 ?RemoveAll@?$NiTMap@PBDVNiPoint3@@@@QAEXXZ @game/CameraMode
// FUNCTION: STBC 0x4294F0 ?RemoveAll@?$NiTMap@PBDJ@@QAEXXZ @game/CameraMode
// FUNCTION: STBC 0x707640 ?RemoveAll@?$NiTMap@PBDPAUTGOverlayEntry@@@@QAEXXZ @game/TGOverlayManager
// FUNCTION: STBC 0x7076A0 ?RemoveAll@?$NiTMap@JPAVTGPhoneme@@@@QAEXXZ @game/TGOverlayManager
// FUNCTION: STBC 0x429490 ?RemoveAll@?$NiTMap@PBDM@@QAEXXZ @game/CameraMode
// FUNCTION: STBC 0x7C4A10 ?RemoveAll@?$NiTMap@IPAVNiD3DGeometryData@@@@QAEXXZ
// FUNCTION: STBC 0x6A6E50 ?RemoveAll@?$NiTMap@HPAVTGMessage@@@@QAEXXZ @game/NetFile
// FUNCTION: STBC 0x6A6FC0 ?RemoveAll@?$NiTMap@HPAV?$NiTList@PAVTGMessage@@@@@@QAEXXZ @game/NetFile
// FUNCTION: STBC 0x6A70F0 ?RemoveAll@?$NiTMap@HPAVNetFileNameList@@@@QAEXXZ @game/NetFile
// FUNCTION: STBC 0x4C4720 ?RemoveAll@?$NiTMap@_JPAUDamageIndexBlock@@@@QAEXXZ @game/DamagePolygonizer
// FUNCTION: STBC 0x4BB930 ?RemoveAll@?$NiTMap@_JG@@QAEXXZ @game/VisibleDamageBuilder
// FUNCTION: STBC 0x4C46C0 ?RemoveAll@?$NiTMap@_JVVertexInfo@MetaVolumePolygonizer@@@@QAEXXZ @game/DamagePolygonizer
// FUNCTION: STBC 0x4C4660 ?RemoveAll@?$NiTMap@_JUDamageVertexIndices@@@@QAEXXZ @game/DamagePolygonizer
// FUNCTION: STBC 0x66F730 ?RemoveAll@?$NiTMap@HPAUPhonemeList@@@@QAEXXZ @game/CharacterClass
// FUNCTION: STBC 0x458650 ?RemoveAll@?$NiTMap@PBDV?$NiPointer@VTGOverlayImage@@@@@@QAEXXZ @game/TGOverlayController
// FUNCTION: STBC 0x54F4F0 ?RemoveAll@?$NiTMap@PAVSTNumericBar@@J@@QAEXXZ @game/EngPowerCtrl
// FUNCTION: STBC 0x72B0D0 ?RemoveAll@?$NiTMap@PBDV?$NiPointer@VNiImage@@@@@@QAEXXZ @game/TGIconManager
template <class TKEY, class TVAL> inline void NiTMap<TKEY, TVAL>::RemoveAll() {
	for ( unsigned int i = 0; i < hashSize; i++ ) {
		while ( hashTable[i] ) {
			NiTMapItem<TKEY, TVAL> *item = hashTable[i];
			hashTable[i] = hashTable[i]->next;
			ClearValue( item );
			delete item;
		}
	}
	count = 0;
}

/*
================
NiTMap::GetAt

Copies the key's value into val; false when the key is not in the map.
================
*/
// FUNCTION: STBC 0x4C7F60 ?GetAt@?$NiTMap@PBDPAVTextureAnimContainer@@@@QBE_NPBDAAPAVTextureAnimContainer@@@Z @game/AnimTSParticleController
// FUNCTION: STBC 0x428B70 ?GetAt@?$NiTMap@PBDJ@@QBE_NPBDAAJ@Z @game/CameraMode
// FUNCTION: STBC 0x428930 ?GetAt@?$NiTMap@PBDM@@QBE_NPBDAAM@Z @game/CameraMode
// FUNCTION: STBC 0x4C4080 ?GetAt@?$NiTMap@_JPAUDamageIndexBlock@@@@QBE_N_JAAPAUDamageIndexBlock@@@Z @game/DamagePolygonizer
// FUNCTION: STBC 0x4865B0 ?GetAt@?$NiTMap@JPAUFireSubsystemRating@@@@QBE_NJAAPAUFireSubsystemRating@@@Z @game/OptimizedFireScript
template <class TKEY, class TVAL> inline bool NiTMap<TKEY, TVAL>::GetAt( TKEY key, TVAL &val ) const {
	unsigned int index = KeyToHashIndex( key );
	NiTMapItem<TKEY, TVAL> *item = hashTable[index];
	while ( item ) {
		if ( IsKeysEqual( key, item->key ) ) {
			val = item->val;
			return true;
		}
		item = item->next;
	}
	return false;
}

/*
================
NiTMap::GetFirstPos

The first item of the first non-empty bucket; NULL for an empty map.
================
*/
template <class TKEY, class TVAL> inline NiTMapIterator NiTMap<TKEY, TVAL>::GetFirstPos() {
	for ( unsigned int i = 0; i < hashSize; i++ ) {
		if ( hashTable[i] ) {
			return hashTable[i];
		}
	}
	return 0;
}

/*
================
NiTMap::GetNext

Returns the item at pos and advances pos to the next item in its bucket,
else to the head of the next non-empty bucket, else to NULL.
================
*/
// FUNCTION: STBC 0x6655B0 ?GetNext@?$NiTMap@PBDPAUBridgeTexture@@@@QAEXAAPAXAAPBDAAPAUBridgeTexture@@@Z @game/BridgeObjectClass
// FUNCTION: STBC 0x54F550 ?GetNext@?$NiTMap@PAVSTNumericBar@@J@@QAEXAAPAXAAPAVSTNumericBar@@AAJ@Z @game/EngPowerCtrl
// FUNCTION: STBC 0x486600 ?GetNext@?$NiTMap@JPAUFireSubsystemRating@@@@QAEXAAPAXAAJAAPAUFireSubsystemRating@@@Z @game/OptimizedFireScript
template <class TKEY, class TVAL> inline void NiTMap<TKEY, TVAL>::GetNext( NiTMapIterator &pos, TKEY &key, TVAL &val ) {
	NiTMapItem<TKEY, TVAL> *item = (NiTMapItem<TKEY, TVAL> *)pos;
	key = item->key;
	val = item->val;

	if ( item->next ) {
		pos = item->next;
		return;
	}

	unsigned int i = KeyToHashIndex( item->key );
	for ( ++i; i < hashSize; i++ ) {
		item = hashTable[i];
		if ( item ) {
			pos = item;
			return;
		}
	}
	pos = 0;
}

#endif /* !__NITMAP_H__ */
