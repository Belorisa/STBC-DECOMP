/*
===============================================================================

	NiTStringMap.h

	An NiTMap keyed by strings: the map keeps its own copy of each key. The
	class name is Gamebryo lineage (Gamebryo's NiTStringMap derives from
	NiTMap<const char *, TVAL> and overrides the same four hooks); no string
	in the binary names it.

	Evidence: the stream's loader registry (0x9A2B98) stores the derived
	vtable 0x88B7C4 after the base constructor's 0x88B7D8. The hooks hash
	with hash * 33 + c (0x455610), compare with strcmp (0x455640), copy the
	key into a new block (0x4556A0) and delete that copy (0x4556F0). The
	destructor (0x4557C0) deletes the key copies itself, then leaves the
	items to NiTMap's destructor.

	The keys stay const char * as in NiTMap; the map deletes its own copies
	through a char * cast, since VC6 refuses to delete through a pointer to
	const.

===============================================================================
*/

#ifndef __NITSTRINGMAP_H__
#define __NITSTRINGMAP_H__

#include "NiTMap.h"

template <class TVAL> class NiTStringMap : public NiTMap<const char *, TVAL> {
public:
	// FUNCTION: STBC 0x70E860 ??0?$NiTStringMap@PAVTGSoundRegion@@@@QAE@I@Z @game/TGSoundRegion
	// FUNCTION: STBC 0x58C6B0 ??0?$NiTStringMap@PAVBackdrop@@@@QAE@I@Z @game/Backdrop
	// FUNCTION: STBC 0x4753E0 ??0?$NiTStringMap@PAUAILogScope@@@@QAE@I@Z @game/AILog
// FUNCTION: STBC 0x428E90 ??0?$NiTStringMap@P6APAVCameraMode@@PAVCameraObjectClass@@@Z@@QAE@I@Z @game/CameraMode
// FUNCTION: STBC 0x70CFF0 ??0?$NiTStringMap@PAVTGSoundFile@@@@QAE@I@Z @game/TGSound
// FUNCTION: STBC 0x719960 ??0?$NiTStringMap@PAUTGTimingSamples@@@@QAE@I@Z @game/TGProfilingInfo
	// FUNCTION: STBC 0x723430 ??0?$NiTStringMap@_N@@QAE@I@Z @game/TGTraceStream
	// FUNCTION: STBC 0x4A3820 ??0?$NiTStringMap@PAV?$SortedArray@VPlacementObject@@J@@@@QAE@I@Z @game/PlacementObject
						NiTStringMap( unsigned int hashSize_ = 37 ) : NiTMap<const char *, TVAL>( hashSize_ ) {}
	virtual				~NiTStringMap();

protected:
	virtual unsigned int	KeyToHashIndex( const char *key ) const;
	virtual bool		IsKeysEqual( const char *key1, const char *key2 ) const;
	virtual void		SetValue( NiTMapItem<const char *, TVAL> *item, const char *key, TVAL val );
	virtual void		ClearValue( NiTMapItem<const char *, TVAL> *item );
};

/*
================
NiTStringMap::~NiTStringMap

Deletes the key copies; NiTMap's destructor then frees the items, calling
its own ClearValue, which no longer reaches this class's.
================
*/
// FUNCTION: STBC 0x4557A0 ??_G?$NiTStringMap@P6AXAAVNiStream@@AAPAVNiObject@@@Z@@UAEPAXI@Z
// FUNCTION: STBC 0x42E2A0 ??_G?$NiTStringMap@PAVCameraMode@@@@UAEPAXI@Z @game/CameraObjectClass
// FUNCTION: STBC 0x7075A0 ??_G?$NiTStringMap@PAUTGOverlayEntry@@@@UAEPAXI@Z @game/TGOverlayManager
// FUNCTION: STBC 0x58CB40 ??_G?$NiTStringMap@PAVBackdrop@@@@UAEPAXI@Z @game/Backdrop
// FUNCTION: STBC 0x4547B0 ??_G?$NiTStringMap@PAVLODModel@@@@UAEPAXI@Z @game/LODModel
// FUNCTION: STBC 0x42E1C0 ??_G?$NiTStringMap@PAD@@UAEPAXI@Z @game/TGAnimationManagerClass
// FUNCTION: STBC 0x4557C0 ??1?$NiTStringMap@P6AXAAVNiStream@@AAPAVNiObject@@@Z@@UAE@XZ
// FUNCTION: STBC 0x42DD50 ??1?$NiTStringMap@PAVCameraMode@@@@UAE@XZ @game/CameraObjectClass
// FUNCTION: STBC 0x70AD40 ??1?$NiTStringMap@PAUTGMusicPeriod@@@@UAE@XZ @game/TGMusic
// FUNCTION: STBC 0x719A10 ??1?$NiTStringMap@PAUTGTimingSamples@@@@UAE@XZ @game/TGProfilingInfo
// FUNCTION: STBC 0x7072C0 ??1?$NiTStringMap@PAUTGOverlayEntry@@@@UAE@XZ @game/TGOverlayManager
// FUNCTION: STBC 0x58C780 ??1?$NiTStringMap@PAVBackdrop@@@@UAE@XZ @game/Backdrop
// FUNCTION: STBC 0x454340 ??1?$NiTStringMap@PAVLODModel@@@@UAE@XZ @game/LODModel
// FUNCTION: STBC 0x428F40 ??1?$NiTStringMap@P6APAVCameraMode@@PAVCameraObjectClass@@@Z@@UAE@XZ @game/CameraMode
// FUNCTION: STBC 0x428CF0 ??1?$NiTStringMap@VNiPoint3@@@@UAE@XZ @game/CameraMode
// FUNCTION: STBC 0x4289E0 ??1?$NiTStringMap@M@@UAE@XZ @game/CameraMode
// FUNCTION: STBC 0x42D930 ??1?$NiTStringMap@PAD@@UAE@XZ @game/TGAnimationManagerClass
// FUNCTION: STBC 0x71DDC0 ??1?$NiTStringMap@PAVTGConfigValue@@@@UAE@XZ @game/TGConfigMapping
// FUNCTION: STBC 0x44F030 ??_G?$NiTStringMap@V?$NiPointer@VNiImage@@@@@@UAEPAXI@Z
// FUNCTION: STBC 0x44F050 ??1?$NiTStringMap@V?$NiPointer@VNiImage@@@@@@UAE@XZ
// FUNCTION: STBC 0x70A5D0 ??1?$NiTStringMap@PAVTGSound@@@@UAE@XZ @game/TGSoundModule
// FUNCTION: STBC 0x70E910 ??1?$NiTStringMap@PAVTGSoundRegion@@@@UAE@XZ @game/TGSoundRegion
// FUNCTION: STBC 0x70A8C0 ??_G?$NiTStringMap@PAVTGSound@@@@UAEPAXI@Z @game/TGSoundModule
// FUNCTION: STBC 0x70EB20 ??_G?$NiTStringMap@PAVTGSoundRegion@@@@UAEPAXI@Z @game/TGSoundRegion
// FUNCTION: STBC 0x477F90 ??1?$NiTStringMap@PAUBuilderAIBlock@@@@UAE@XZ @game/BuilderAI
// FUNCTION: STBC 0x446D20 ??_G?$NiTStringMap@PAU_object@@@@UAEPAXI@Z @game/UtopiaModule
// FUNCTION: STBC 0x446D40 ??1?$NiTStringMap@PAU_object@@@@UAE@XZ @game/UtopiaModule
// FUNCTION: STBC 0x4469E0 ?KeyToHashIndex@?$NiTStringMap@PAU_object@@@@MBEIPBD@Z @game/UtopiaModule
// FUNCTION: STBC 0x446A10 ?IsKeysEqual@?$NiTStringMap@PAU_object@@@@MBE_NPBD0@Z @game/UtopiaModule
// FUNCTION: STBC 0x446A70 ?SetValue@?$NiTStringMap@PAU_object@@@@MAEXPAV?$NiTMapItem@PBDPAU_object@@@@PBDPAU_object@@@Z @game/UtopiaModule
// FUNCTION: STBC 0x446AC0 ?ClearValue@?$NiTStringMap@PAU_object@@@@MAEXPAV?$NiTMapItem@PBDPAU_object@@@@@Z @game/UtopiaModule
// FUNCTION: STBC 0x454330 ?ClearValue@?$NiTMap@PBDPAVLODModel@@@@MAEXPAV?$NiTMapItem@PBDPAVLODModel@@@@@Z @game/LODModel
// FUNCTION: STBC 0x70D220 ?ClearValue@?$NiTStringMap@PAVTGSoundFile@@@@MAEXPAV?$NiTMapItem@PBDPAVTGSoundFile@@@@@Z @game/TGSound
// FUNCTION: STBC 0x70D3F0 ??_G?$NiTStringMap@PAVTGSoundFile@@@@UAEPAXI@Z @game/TGSound
// FUNCTION: STBC 0x707360 ?KeyToHashIndex@?$NiTStringMap@PAUTGOverlayEntry@@@@MBEIPBD@Z @game/TGOverlayManager
// FUNCTION: STBC 0x707390 ?IsKeysEqual@?$NiTStringMap@PAUTGOverlayEntry@@@@MBE_NPBD0@Z @game/TGOverlayManager
// FUNCTION: STBC 0x7073F0 ?SetValue@?$NiTStringMap@PAUTGOverlayEntry@@@@MAEXPAV?$NiTMapItem@PBDPAUTGOverlayEntry@@@@PBDPAUTGOverlayEntry@@@Z @game/TGOverlayManager
// FUNCTION: STBC 0x707440 ?ClearValue@?$NiTStringMap@PAUTGOverlayEntry@@@@MAEXPAV?$NiTMapItem@PBDPAUTGOverlayEntry@@@@@Z @game/TGOverlayManager
// FUNCTION: STBC 0x4782E0 ??_G?$NiTStringMap@PAUBuilderAIBlock@@@@UAEPAXI@Z @game/BuilderAI
// FUNCTION: STBC 0x4291F0 ??_G?$NiTStringMap@VNiPoint3@@@@UAEPAXI@Z @game/CameraMode
// FUNCTION: STBC 0x429150 ??_G?$NiTStringMap@M@@UAEPAXI@Z @game/CameraMode
// FUNCTION: STBC 0x429290 ??_G?$NiTStringMap@P6APAVCameraMode@@PAVCameraObjectClass@@@Z@@UAEPAXI@Z @game/CameraMode
// FUNCTION: STBC 0x417970 ??_G?$NiTStringMap@J@@UAEPAXI@Z @game/CameraMode
// FUNCTION: STBC 0x417340 ??1?$NiTStringMap@J@@UAE@XZ @game/SetClass
// FUNCTION: STBC 0x47A540 ??1?$NiTStringMap@PAV?$NiTList@PAU_object@@@@@@UAE@XZ @game/ConditionalAI
// FUNCTION: STBC 0x47A8F0 ??_G?$NiTStringMap@PAV?$NiTList@PAU_object@@@@@@UAEPAXI@Z @game/ConditionalAI
// FUNCTION: STBC 0x7A5600 ??_G?$NiTStringMap@PAVSequence@NiKeyframeManager@@@@UAEPAXI@Z @netimmerse/NiKeyframeManager
// FUNCTION: STBC 0x7A5230 ??1?$NiTStringMap@PAVSequence@NiKeyframeManager@@@@UAE@XZ @netimmerse/NiKeyframeManager
// FUNCTION: STBC 0x4F4340 ??_G?$NiTStringMap@PAVTextureAnimContainer@@@@UAEPAXI@Z @game/TextureAnimManager
// FUNCTION: STBC 0x4F4360 ??1?$NiTStringMap@PAVTextureAnimContainer@@@@UAE@XZ @game/TextureAnimManager
// FUNCTION: STBC 0x719D80 ??_G?$NiTStringMap@PAUTGTimingSamples@@@@UAEPAXI@Z @game/TGProfilingInfo
// FUNCTION: STBC 0x665680 ??1?$NiTStringMap@PAUBridgeTexture@@@@UAE@XZ @game/BridgeObjectClass
// FUNCTION: STBC 0x665920 ??_G?$NiTStringMap@PAUBridgeTexture@@@@UAEPAXI@Z @game/BridgeObjectClass
// FUNCTION: STBC 0x475490 ??1?$NiTStringMap@PAUAILogScope@@@@UAE@XZ @game/AILog
// FUNCTION: STBC 0x475740 ??_G?$NiTStringMap@PAUAILogScope@@@@UAEPAXI@Z @game/AILog
// FUNCTION: STBC 0x4582B0 ??1?$NiTStringMap@V?$NiPointer@VTGOverlayImage@@@@@@UAE@XZ @game/TGOverlayController
// FUNCTION: STBC 0x4585B0 ??_G?$NiTStringMap@V?$NiPointer@VTGOverlayImage@@@@@@UAEPAXI@Z @game/TGOverlayController
// FUNCTION: STBC 0x7237E0 ??_G?$NiTStringMap@_N@@UAEPAXI@Z @game/TGTraceStream
// FUNCTION: STBC 0x4A3D10 ??_G?$NiTStringMap@PAV?$SortedArray@VPlacementObject@@J@@@@UAEPAXI@Z @game/PlacementObject
// FUNCTION: STBC 0x71E1C0 ??_G?$NiTStringMap@PAVTGConfigValue@@@@UAEPAXI@Z @game/TGConfigMapping
// FUNCTION: STBC 0x70AD20 ??_G?$NiTStringMap@PAUTGMusicPeriod@@@@UAEPAXI@Z @game/TGMusic
// FUNCTION: STBC 0x7234E0 ??1?$NiTStringMap@_N@@UAE@XZ @game/TGTraceStream
// FUNCTION: STBC 0x4A38D0 ??1?$NiTStringMap@PAV?$SortedArray@VPlacementObject@@J@@@@UAE@XZ @game/PlacementObject
// FUNCTION: STBC 0x70D0A0 ??1?$NiTStringMap@PAVTGSoundFile@@@@UAE@XZ @game/TGSound
template <class TVAL> inline NiTStringMap<TVAL>::~NiTStringMap() {
	for ( unsigned int i = 0; i < hashSize; i++ ) {
		NiTMapItem<const char *, TVAL> *item = hashTable[i];
		while ( item ) {
			NiTMapItem<const char *, TVAL> *save = item;
			item = item->next;
			delete [] (char *)save->key;
		}
	}
}

/*
================
NiTStringMap::KeyToHashIndex
================
*/
// FUNCTION: STBC 0x455610 ?KeyToHashIndex@?$NiTStringMap@P6AXAAVNiStream@@AAPAVNiObject@@@Z@@MBEIPBD@Z
// FUNCTION: STBC 0x42DE50 ?KeyToHashIndex@?$NiTStringMap@PAVCameraMode@@@@MBEIPBD@Z @game/CameraObjectClass
// FUNCTION: STBC 0x58C820 ?KeyToHashIndex@?$NiTStringMap@PAVBackdrop@@@@MBEIPBD@Z @game/Backdrop
// FUNCTION: STBC 0x4543E0 ?KeyToHashIndex@?$NiTStringMap@PAVLODModel@@@@MBEIPBD@Z @game/LODModel
// FUNCTION: STBC 0x42DA30 ?KeyToHashIndex@?$NiTStringMap@PAD@@MBEIPBD@Z @game/TGAnimationManagerClass
// FUNCTION: STBC 0x71DE60 ?KeyToHashIndex@?$NiTStringMap@PAVTGConfigValue@@@@MBEIPBD@Z @game/TGConfigMapping
// FUNCTION: STBC 0x44EDB0 ?KeyToHashIndex@?$NiTStringMap@V?$NiPointer@VNiImage@@@@@@MBEIPBD@Z
// FUNCTION: STBC 0x70A670 ?KeyToHashIndex@?$NiTStringMap@PAVTGSound@@@@MBEIPBD@Z @game/TGSoundModule
// FUNCTION: STBC 0x70E9B0 ?KeyToHashIndex@?$NiTStringMap@PAVTGSoundRegion@@@@MBEIPBD@Z @game/TGSoundRegion
// FUNCTION: STBC 0x478030 ?KeyToHashIndex@?$NiTStringMap@PAUBuilderAIBlock@@@@MBEIPBD@Z @game/BuilderAI
// FUNCTION: STBC 0x47A5E0 ?KeyToHashIndex@?$NiTStringMap@PAV?$NiTList@PAU_object@@@@@@MBEIPBD@Z @game/ConditionalAI
// FUNCTION: STBC 0x7A52D0 ?KeyToHashIndex@?$NiTStringMap@PAVSequence@NiKeyframeManager@@@@MBEIPBD@Z @netimmerse/NiKeyframeManager
// FUNCTION: STBC 0x4F41A0 ?KeyToHashIndex@?$NiTStringMap@PAVTextureAnimContainer@@@@MBEIPBD@Z @game/TextureAnimManager
// FUNCTION: STBC 0x428D90 ?KeyToHashIndex@?$NiTStringMap@VNiPoint3@@@@MBEIPBD@Z @game/CameraMode
// FUNCTION: STBC 0x428A80 ?KeyToHashIndex@?$NiTStringMap@M@@MBEIPBD@Z @game/CameraMode
// FUNCTION: STBC 0x428FE0 ?KeyToHashIndex@?$NiTStringMap@P6APAVCameraMode@@PAVCameraObjectClass@@@Z@@MBEIPBD@Z @game/CameraMode
// FUNCTION: STBC 0x4173E0 ?KeyToHashIndex@?$NiTStringMap@J@@MBEIPBD@Z @game/CameraMode
// FUNCTION: STBC 0x719AB0 ?KeyToHashIndex@?$NiTStringMap@PAUTGTimingSamples@@@@MBEIPBD@Z @game/TGProfilingInfo
// FUNCTION: STBC 0x665720 ?KeyToHashIndex@?$NiTStringMap@PAUBridgeTexture@@@@MBEIPBD@Z @game/BridgeObjectClass
// FUNCTION: STBC 0x475530 ?KeyToHashIndex@?$NiTStringMap@PAUAILogScope@@@@MBEIPBD@Z @game/AILog
// FUNCTION: STBC 0x458370 ?KeyToHashIndex@?$NiTStringMap@V?$NiPointer@VTGOverlayImage@@@@@@MBEIPBD@Z @game/TGOverlayController
// FUNCTION: STBC 0x723580 ?KeyToHashIndex@?$NiTStringMap@_N@@MBEIPBD@Z @game/TGTraceStream
// FUNCTION: STBC 0x4A3990 ?KeyToHashIndex@?$NiTStringMap@PAV?$SortedArray@VPlacementObject@@J@@@@MBEIPBD@Z @game/PlacementObject
// FUNCTION: STBC 0x70ABB0 ?KeyToHashIndex@?$NiTStringMap@PAUTGMusicPeriod@@@@MBEIPBD@Z @game/TGMusic
// FUNCTION: STBC 0x70D140 ?KeyToHashIndex@?$NiTStringMap@PAVTGSoundFile@@@@MBEIPBD@Z @game/TGSound
template <class TVAL> inline unsigned int NiTStringMap<TVAL>::KeyToHashIndex( const char *key ) const {
	unsigned int hash = 0;
	while ( *key ) {
		hash = ( hash << 5 ) + hash + *key++;
	}
	return hash % hashSize;
}

/*
================
NiTStringMap::IsKeysEqual
================
*/
// FUNCTION: STBC 0x455640 ?IsKeysEqual@?$NiTStringMap@P6AXAAVNiStream@@AAPAVNiObject@@@Z@@MBE_NPBD0@Z
// FUNCTION: STBC 0x42DE80 ?IsKeysEqual@?$NiTStringMap@PAVCameraMode@@@@MBE_NPBD0@Z @game/CameraObjectClass
// FUNCTION: STBC 0x58C850 ?IsKeysEqual@?$NiTStringMap@PAVBackdrop@@@@MBE_NPBD0@Z @game/Backdrop
// FUNCTION: STBC 0x454410 ?IsKeysEqual@?$NiTStringMap@PAVLODModel@@@@MBE_NPBD0@Z @game/LODModel
// FUNCTION: STBC 0x42DA80 ?IsKeysEqual@?$NiTStringMap@PAD@@MBE_NPBD0@Z @game/TGAnimationManagerClass
// FUNCTION: STBC 0x71DE90 ?IsKeysEqual@?$NiTStringMap@PAVTGConfigValue@@@@MBE_NPBD0@Z @game/TGConfigMapping
// FUNCTION: STBC 0x44EDE0 ?IsKeysEqual@?$NiTStringMap@V?$NiPointer@VNiImage@@@@@@MBE_NPBD0@Z
// FUNCTION: STBC 0x70A6A0 ?IsKeysEqual@?$NiTStringMap@PAVTGSound@@@@MBE_NPBD0@Z @game/TGSoundModule
// FUNCTION: STBC 0x70E9E0 ?IsKeysEqual@?$NiTStringMap@PAVTGSoundRegion@@@@MBE_NPBD0@Z @game/TGSoundRegion
// FUNCTION: STBC 0x478080 ?IsKeysEqual@?$NiTStringMap@PAUBuilderAIBlock@@@@MBE_NPBD0@Z @game/BuilderAI
// FUNCTION: STBC 0x47A610 ?IsKeysEqual@?$NiTStringMap@PAV?$NiTList@PAU_object@@@@@@MBE_NPBD0@Z @game/ConditionalAI
// FUNCTION: STBC 0x7A5300 ?IsKeysEqual@?$NiTStringMap@PAVSequence@NiKeyframeManager@@@@MBE_NPBD0@Z @netimmerse/NiKeyframeManager
// FUNCTION: STBC 0x4F41D0 ?IsKeysEqual@?$NiTStringMap@PAVTextureAnimContainer@@@@MBE_NPBD0@Z @game/TextureAnimManager
// FUNCTION: STBC 0x428DC0 ?IsKeysEqual@?$NiTStringMap@VNiPoint3@@@@MBE_NPBD0@Z @game/CameraMode
// FUNCTION: STBC 0x428AB0 ?IsKeysEqual@?$NiTStringMap@M@@MBE_NPBD0@Z @game/CameraMode
// FUNCTION: STBC 0x429010 ?IsKeysEqual@?$NiTStringMap@P6APAVCameraMode@@PAVCameraObjectClass@@@Z@@MBE_NPBD0@Z @game/CameraMode
// FUNCTION: STBC 0x417410 ?IsKeysEqual@?$NiTStringMap@J@@MBE_NPBD0@Z @game/CameraMode
// FUNCTION: STBC 0x719AE0 ?IsKeysEqual@?$NiTStringMap@PAUTGTimingSamples@@@@MBE_NPBD0@Z @game/TGProfilingInfo
// FUNCTION: STBC 0x665750 ?IsKeysEqual@?$NiTStringMap@PAUBridgeTexture@@@@MBE_NPBD0@Z @game/BridgeObjectClass
// FUNCTION: STBC 0x475560 ?IsKeysEqual@?$NiTStringMap@PAUAILogScope@@@@MBE_NPBD0@Z @game/AILog
// FUNCTION: STBC 0x4583A0 ?IsKeysEqual@?$NiTStringMap@V?$NiPointer@VTGOverlayImage@@@@@@MBE_NPBD0@Z @game/TGOverlayController
// FUNCTION: STBC 0x7235B0 ?IsKeysEqual@?$NiTStringMap@_N@@MBE_NPBD0@Z @game/TGTraceStream
// FUNCTION: STBC 0x4A39C0 ?IsKeysEqual@?$NiTStringMap@PAV?$SortedArray@VPlacementObject@@J@@@@MBE_NPBD0@Z @game/PlacementObject
// FUNCTION: STBC 0x70ABE0 ?IsKeysEqual@?$NiTStringMap@PAUTGMusicPeriod@@@@MBE_NPBD0@Z @game/TGMusic
// FUNCTION: STBC 0x70D170 ?IsKeysEqual@?$NiTStringMap@PAVTGSoundFile@@@@MBE_NPBD0@Z @game/TGSound
template <class TVAL> inline bool NiTStringMap<TVAL>::IsKeysEqual( const char *key1, const char *key2 ) const {
	return strcmp( key1, key2 ) == 0;
}

/*
================
NiTStringMap::SetValue

Stores a copy of the key.
================
*/
// FUNCTION: STBC 0x4556A0 ?SetValue@?$NiTStringMap@P6AXAAVNiStream@@AAPAVNiObject@@@Z@@MAEXPAV?$NiTMapItem@PBDP6AXAAVNiStream@@AAPAVNiObject@@@Z@@PBDP6AXAAVNiStream@@AAPAVNiObject@@@Z@Z
// FUNCTION: STBC 0x42DEE0 ?SetValue@?$NiTStringMap@PAVCameraMode@@@@MAEXPAV?$NiTMapItem@PBDPAVCameraMode@@@@PBDPAVCameraMode@@@Z @game/CameraObjectClass
// FUNCTION: STBC 0x58C8B0 ?SetValue@?$NiTStringMap@PAVBackdrop@@@@MAEXPAV?$NiTMapItem@PBDPAVBackdrop@@@@PBDPAVBackdrop@@@Z @game/Backdrop
// FUNCTION: STBC 0x454490 ?SetValue@?$NiTStringMap@PAVLODModel@@@@MAEXPAV?$NiTMapItem@PBDPAVLODModel@@@@PBDPAVLODModel@@@Z @game/LODModel
// FUNCTION: STBC 0x42DAE0 ?SetValue@?$NiTStringMap@PAD@@MAEXPAV?$NiTMapItem@PBDPAD@@PBDPAD@Z @game/TGAnimationManagerClass
// FUNCTION: STBC 0x71DEF0 ?SetValue@?$NiTStringMap@PAVTGConfigValue@@@@MAEXPAV?$NiTMapItem@PBDPAVTGConfigValue@@@@PBDPAVTGConfigValue@@@Z @game/TGConfigMapping
// FUNCTION: STBC 0x44EE40 ?SetValue@?$NiTStringMap@V?$NiPointer@VNiImage@@@@@@MAEXPAV?$NiTMapItem@PBDV?$NiPointer@VNiImage@@@@@@PBDV?$NiPointer@VNiImage@@@@@Z
// FUNCTION: STBC 0x70A700 ?SetValue@?$NiTStringMap@PAVTGSound@@@@MAEXPAV?$NiTMapItem@PBDPAVTGSound@@@@PBDPAVTGSound@@@Z @game/TGSoundModule
// FUNCTION: STBC 0x70EA40 ?SetValue@?$NiTStringMap@PAVTGSoundRegion@@@@MAEXPAV?$NiTMapItem@PBDPAVTGSoundRegion@@@@PBDPAVTGSoundRegion@@@Z @game/TGSoundRegion
// FUNCTION: STBC 0x4780E0 ?SetValue@?$NiTStringMap@PAUBuilderAIBlock@@@@MAEXPAV?$NiTMapItem@PBDPAUBuilderAIBlock@@@@PBDPAUBuilderAIBlock@@@Z @game/BuilderAI
// FUNCTION: STBC 0x47A670 ?SetValue@?$NiTStringMap@PAV?$NiTList@PAU_object@@@@@@MAEXPAV?$NiTMapItem@PBDPAV?$NiTList@PAU_object@@@@@@PBDPAV?$NiTList@PAU_object@@@@@Z @game/ConditionalAI
// FUNCTION: STBC 0x7A5360 ?SetValue@?$NiTStringMap@PAVSequence@NiKeyframeManager@@@@MAEXPAV?$NiTMapItem@PBDPAVSequence@NiKeyframeManager@@@@PBDPAVSequence@NiKeyframeManager@@@Z @netimmerse/NiKeyframeManager
// FUNCTION: STBC 0x4F4230 ?SetValue@?$NiTStringMap@PAVTextureAnimContainer@@@@MAEXPAV?$NiTMapItem@PBDPAVTextureAnimContainer@@@@PBDPAVTextureAnimContainer@@@Z @game/TextureAnimManager
// FUNCTION: STBC 0x428E20 ?SetValue@?$NiTStringMap@VNiPoint3@@@@MAEXPAV?$NiTMapItem@PBDVNiPoint3@@@@PBDVNiPoint3@@@Z @game/CameraMode
// FUNCTION: STBC 0x428B10 ?SetValue@?$NiTStringMap@M@@MAEXPAV?$NiTMapItem@PBDM@@PBDM@Z @game/CameraMode
// FUNCTION: STBC 0x429070 ?SetValue@?$NiTStringMap@P6APAVCameraMode@@PAVCameraObjectClass@@@Z@@MAEXPAV?$NiTMapItem@PBDP6APAVCameraMode@@PAVCameraObjectClass@@@Z@@PBDP6APAVCameraMode@@PAVCameraObjectClass@@@Z@Z @game/CameraMode
// FUNCTION: STBC 0x417470 ?SetValue@?$NiTStringMap@J@@MAEXPAV?$NiTMapItem@PBDJ@@PBDJ@Z @game/CameraMode
// FUNCTION: STBC 0x719B40 ?SetValue@?$NiTStringMap@PAUTGTimingSamples@@@@MAEXPAV?$NiTMapItem@PBDPAUTGTimingSamples@@@@PBDPAUTGTimingSamples@@@Z @game/TGProfilingInfo
// FUNCTION: STBC 0x6657B0 ?SetValue@?$NiTStringMap@PAUBridgeTexture@@@@MAEXPAV?$NiTMapItem@PBDPAUBridgeTexture@@@@PBDPAUBridgeTexture@@@Z @game/BridgeObjectClass
// FUNCTION: STBC 0x4755C0 ?SetValue@?$NiTStringMap@PAUAILogScope@@@@MAEXPAV?$NiTMapItem@PBDPAUAILogScope@@@@PBDPAUAILogScope@@@Z @game/AILog
// FUNCTION: STBC 0x458400 ?SetValue@?$NiTStringMap@V?$NiPointer@VTGOverlayImage@@@@@@MAEXPAV?$NiTMapItem@PBDV?$NiPointer@VTGOverlayImage@@@@@@PBDV?$NiPointer@VTGOverlayImage@@@@@Z @game/TGOverlayController
// FUNCTION: STBC 0x723610 ?SetValue@?$NiTStringMap@_N@@MAEXPAV?$NiTMapItem@PBD_N@@PBD_N@Z @game/TGTraceStream
// FUNCTION: STBC 0x4A3A20 ?SetValue@?$NiTStringMap@PAV?$SortedArray@VPlacementObject@@J@@@@MAEXPAV?$NiTMapItem@PBDPAV?$SortedArray@VPlacementObject@@J@@@@PBDPAV?$SortedArray@VPlacementObject@@J@@@Z @game/PlacementObject
// FUNCTION: STBC 0x70AC40 ?SetValue@?$NiTStringMap@PAUTGMusicPeriod@@@@MAEXPAV?$NiTMapItem@PBDPAUTGMusicPeriod@@@@PBDPAUTGMusicPeriod@@@Z @game/TGMusic
// FUNCTION: STBC 0x70D1D0 ?SetValue@?$NiTStringMap@PAVTGSoundFile@@@@MAEXPAV?$NiTMapItem@PBDPAVTGSoundFile@@@@PBDPAVTGSoundFile@@@Z @game/TGSound
template <class TVAL> inline void NiTStringMap<TVAL>::SetValue( NiTMapItem<const char *, TVAL> *item, const char *key, TVAL val ) {
	item->key = new char[strlen( key ) + 1];
	assert( item->key );
	strcpy( (char *)item->key, key );
	item->val = val;
}

/*
================
NiTStringMap::ClearValue

Deletes the key copy.
================
*/
// FUNCTION: STBC 0x4556F0 ?ClearValue@?$NiTStringMap@P6AXAAVNiStream@@AAPAVNiObject@@@Z@@MAEXPAV?$NiTMapItem@PBDP6AXAAVNiStream@@AAPAVNiObject@@@Z@@@Z
// FUNCTION: STBC 0x42DF50 ?ClearValue@?$NiTStringMap@PAVCameraMode@@@@MAEXPAV?$NiTMapItem@PBDPAVCameraMode@@@@@Z @game/CameraObjectClass
// FUNCTION: STBC 0x58C900 ?ClearValue@?$NiTStringMap@PAVBackdrop@@@@MAEXPAV?$NiTMapItem@PBDPAVBackdrop@@@@@Z @game/Backdrop
// FUNCTION: STBC 0x4544E0 ?ClearValue@?$NiTStringMap@PAVLODModel@@@@MAEXPAV?$NiTMapItem@PBDPAVLODModel@@@@@Z @game/LODModel
// FUNCTION: STBC 0x42DB30 ?ClearValue@?$NiTStringMap@PAD@@MAEXPAV?$NiTMapItem@PBDPAD@@@Z @game/TGAnimationManagerClass
// FUNCTION: STBC 0x44EF10 ?ClearValue@?$NiTStringMap@V?$NiPointer@VNiImage@@@@@@MAEXPAV?$NiTMapItem@PBDV?$NiPointer@VNiImage@@@@@@@Z
// FUNCTION: STBC 0x70A750 ?ClearValue@?$NiTStringMap@PAVTGSound@@@@MAEXPAV?$NiTMapItem@PBDPAVTGSound@@@@@Z @game/TGSoundModule
// FUNCTION: STBC 0x70EA90 ?ClearValue@?$NiTStringMap@PAVTGSoundRegion@@@@MAEXPAV?$NiTMapItem@PBDPAVTGSoundRegion@@@@@Z @game/TGSoundRegion
// FUNCTION: STBC 0x478130 ?ClearValue@?$NiTStringMap@PAUBuilderAIBlock@@@@MAEXPAV?$NiTMapItem@PBDPAUBuilderAIBlock@@@@@Z @game/BuilderAI
// FUNCTION: STBC 0x47A6C0 ?ClearValue@?$NiTStringMap@PAV?$NiTList@PAU_object@@@@@@MAEXPAV?$NiTMapItem@PBDPAV?$NiTList@PAU_object@@@@@@@Z @game/ConditionalAI
// FUNCTION: STBC 0x7A53B0 ?ClearValue@?$NiTStringMap@PAVSequence@NiKeyframeManager@@@@MAEXPAV?$NiTMapItem@PBDPAVSequence@NiKeyframeManager@@@@@Z @netimmerse/NiKeyframeManager
// FUNCTION: STBC 0x4F4280 ?ClearValue@?$NiTStringMap@PAVTextureAnimContainer@@@@MAEXPAV?$NiTMapItem@PBDPAVTextureAnimContainer@@@@@Z @game/TextureAnimManager
// FUNCTION: STBC 0x428E80 ?ClearValue@?$NiTStringMap@VNiPoint3@@@@MAEXPAV?$NiTMapItem@PBDVNiPoint3@@@@@Z @game/CameraMode
// FUNCTION: STBC 0x428B60 ?ClearValue@?$NiTStringMap@M@@MAEXPAV?$NiTMapItem@PBDM@@@Z @game/CameraMode
// FUNCTION: STBC 0x4290C0 ?ClearValue@?$NiTStringMap@P6APAVCameraMode@@PAVCameraObjectClass@@@Z@@MAEXPAV?$NiTMapItem@PBDP6APAVCameraMode@@PAVCameraObjectClass@@@Z@@@Z @game/CameraMode
// FUNCTION: STBC 0x4174C0 ?ClearValue@?$NiTStringMap@J@@MAEXPAV?$NiTMapItem@PBDJ@@@Z @game/CameraMode
// FUNCTION: STBC 0x719B90 ?ClearValue@?$NiTStringMap@PAUTGTimingSamples@@@@MAEXPAV?$NiTMapItem@PBDPAUTGTimingSamples@@@@@Z @game/TGProfilingInfo
// FUNCTION: STBC 0x665800 ?ClearValue@?$NiTStringMap@PAUBridgeTexture@@@@MAEXPAV?$NiTMapItem@PBDPAUBridgeTexture@@@@@Z @game/BridgeObjectClass
// FUNCTION: STBC 0x475610 ?ClearValue@?$NiTStringMap@PAUAILogScope@@@@MAEXPAV?$NiTMapItem@PBDPAUAILogScope@@@@@Z @game/AILog
// FUNCTION: STBC 0x4584D0 ?ClearValue@?$NiTStringMap@V?$NiPointer@VTGOverlayImage@@@@@@MAEXPAV?$NiTMapItem@PBDV?$NiPointer@VTGOverlayImage@@@@@@@Z @game/TGOverlayController
// FUNCTION: STBC 0x723660 ?ClearValue@?$NiTStringMap@_N@@MAEXPAV?$NiTMapItem@PBD_N@@@Z @game/TGTraceStream
// FUNCTION: STBC 0x4A3A70 ?ClearValue@?$NiTStringMap@PAV?$SortedArray@VPlacementObject@@J@@@@MAEXPAV?$NiTMapItem@PBDPAV?$SortedArray@VPlacementObject@@J@@@@@Z @game/PlacementObject
// FUNCTION: STBC 0x71DF40 ?ClearValue@?$NiTStringMap@PAVTGConfigValue@@@@MAEXPAV?$NiTMapItem@PBDPAVTGConfigValue@@@@@Z @game/TGConfigMapping
// FUNCTION: STBC 0x70AC90 ?ClearValue@?$NiTStringMap@PAUTGMusicPeriod@@@@MAEXPAV?$NiTMapItem@PBDPAUTGMusicPeriod@@@@@Z @game/TGMusic
template <class TVAL> inline void NiTStringMap<TVAL>::ClearValue( NiTMapItem<const char *, TVAL> *item ) {
	delete [] (char *)item->key;
}

#endif /* !__NITSTRINGMAP_H__ */
