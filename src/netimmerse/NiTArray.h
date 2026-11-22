/*
===============================================================================

	NiTArray.h

	NetImmerse's growable array template.

	Evidence: NiStream's constructor (0x816C60) builds two arrays in place
	(1024 elements growing by 1024, and 1 growing by 1): vtable 0x89A75C,
	a heap block of maxSize elements, the maximum size, the grow-by step,
	then zero used size and element count; NiD3DRender's constructor
	builds one with the defaults. The vtable has a single slot,
	the scalar deleting destructor (0x818DB0), shared by every instantiation
	because the linker folded the identical copies.

===============================================================================
*/

#ifndef __NITARRAY_H__
#define __NITARRAY_H__

#include <assert.h>

template <class T> class NiTArray {
public:
					// FUNCTION: STBC 0x46F350 ??_F?$NiTArray@PAVTimeSliceProcess@@@@QAEXXZ @game/TimeSliceProcess
					NiTArray( unsigned int maxSize_ = 1, unsigned int growBy_ = 1 );
	virtual			~NiTArray();

	unsigned int	GetSize() const { return size; }
	unsigned int	GetEffectiveSize() const { return eSize; }	// the non-null elements (Gamebryo lineage name)
	void			SetGrowBy( unsigned int growBy_ ) { growBy = growBy_; }
	// FUNCTION: STBC 0x803310 ?GetAt@?$NiTArray@PAVMap@NiMultiTextureProperty@@@@QBEPAVMap@NiMultiTextureProperty@@I@Z @netimmerse/NiMultiTextureProperty
	// FUNCTION: STBC 0x7A4DC0 ?GetAt@?$NiTArray@V?$NiPointer@VNiKeyframeController@@@@@@QBE?AV?$NiPointer@VNiKeyframeController@@@@I@Z
	// FUNCTION: STBC 0x5961C0 ?GetAt@?$NiTArray@V?$NiPointer@VOBBModel@@@@@@QBE?AV?$NiPointer@VOBBModel@@@@I@Z @game/ShipClass
	// FUNCTION: STBC 0x416F50 ?GetAt@?$NiTArray@V?$NiPointer@VNiAVObject@@@@@@QBE?AV?$NiPointer@VNiAVObject@@@@I@Z @game/CharacterClass
	// FUNCTION: STBC 0x4D7290 ?GetAt@?$NiTArray@V?$NiPointer@VNiTextureProperty@@@@@@QBE?AV?$NiPointer@VNiTextureProperty@@@@I@Z @game/ViewScreenNode
	// FUNCTION: STBC 0x4B20F0 ?GetAt@?$NiTArray@V?$NiPointer@VMetaVolume@@@@@@QBE?AV?$NiPointer@VMetaVolume@@@@I@Z @game/VisibleDamage
	// FUNCTION: STBC 0x4540B0 ?GetAt@?$NiTArray@V?$NiPointer@VNiImage@@@@@@QBE?AV?$NiPointer@VNiImage@@@@I@Z @game/LODModel
	T				GetAt( unsigned int index ) const { return base[index]; }
	T *				GetBase() const { return base; }	// the element block itself (Gamebryo lineage name)

	void			SetSize( unsigned int maxSize_ );
	void			SetAt( unsigned int index, T element );
	unsigned int	SetAtGrow( unsigned int index, T element );
	void			Add( T element );
	void			AddFirstEmpty( T element );
	T				RemoveAt( unsigned int index );
	T				RemoveEnd();
	void			RemoveAll();
	void			Compact();

protected:
	T *				base;		// heap block of maxSize elements
	unsigned int	maxSize;	// allocated elements
	unsigned int	size;		// one past the highest used index
	unsigned int	eSize;		// non-null elements
	unsigned int	growBy;		// elements added when full
};

/*
================
NiTArray::NiTArray
================
*/
// FUNCTION: STBC 0x7A5090 ??0?$NiTArray@J@@QAE@II@Z @netimmerse/NiKeyframeManager
// FUNCTION: STBC 0x7A50F0 ??0?$NiTArray@PAVNiTextKeyExtraData@@@@QAE@II@Z @netimmerse/NiKeyframeManager
// FUNCTION: STBC 0x7A5150 ??0?$NiTArray@PAVNiKeyframeController@@@@QAE@II@Z @netimmerse/NiKeyframeManager
// FUNCTION: STBC 0x7B34A0 ??0?$NiTArray@PAVNiForce@@@@QAE@II@Z
// FUNCTION: STBC 0x7E5D10 ??0?$NiTArray@I@@QAE@II@Z @netimmerse/NiBSPNode
// FUNCTION: STBC 0x453F90 ??0?$NiTArray@PAD@@QAE@II@Z @game/LODModel
// FUNCTION: STBC 0x453FD0 ??0?$NiTArray@V?$NiPointer@VNiImage@@@@@@QAE@II@Z @game/LODModel
// FUNCTION: STBC 0x735960 ??0?$NiTArray@V?$NiPointer@VNiTextureProperty@@@@@@QAE@II@Z @game/TGFontManager
// FUNCTION: STBC 0x454150 ??0?$NiTArray@PAVLODModelPart@@@@QAE@II@Z @game/LODModel
// FUNCTION: STBC 0x50FC00 ??0?$NiTArray@PAVTGUIObject@@@@QAE@II@Z @game/TopWindow
// FUNCTION: STBC 0x7E5DA0 ??0?$NiTArray@PAVNiDynamicEffect@@@@QAE@II@Z @netimmerse/NiNode
// FUNCTION: STBC 0x807350 ??0?$NiTArray@H@@QAE@II@Z @netimmerse/NiSpotLight
// FUNCTION: STBC 0x831ED0 ??0?$NiTArray@PAVNiAVObject@@@@QAE@II@Z @netimmerse/NiBezierMesh
template <class T> inline NiTArray<T>::NiTArray( unsigned int maxSize_, unsigned int growBy_ ) {
	maxSize = maxSize_;
	growBy = growBy_;
	size = 0;
	eSize = 0;
	assert( maxSize_ > 0 );
	base = new T[maxSize];
	assert( base );
}

/*
================
NiTArray::~NiTArray
================
*/
// FUNCTION: STBC 0x8032F0 ??1?$NiTArray@PAVMap@NiMultiTextureProperty@@@@UAE@XZ @netimmerse/NiMultiTextureProperty
// FUNCTION: STBC 0x7A4D90 ??1?$NiTArray@V?$NiPointer@VNiKeyframeController@@@@@@UAE@XZ @netimmerse/NiKeyframeManager
// FUNCTION: STBC 0x6FDAA0 ??1?$NiTArray@PAUTGPhonemeSequenceEntry@@@@UAE@XZ @game/TGActionModule
// FUNCTION: STBC 0x7A50D0 ??1?$NiTArray@J@@UAE@XZ @netimmerse/NiKeyframeManager
// FUNCTION: STBC 0x7A5130 ??1?$NiTArray@PAVNiTextKeyExtraData@@@@UAE@XZ @netimmerse/NiKeyframeManager
// FUNCTION: STBC 0x4C3BC0 ??1?$NiTArray@V?$NiPointer@VMetaVolume@@@@@@UAE@XZ @game/DamagePolygonizer
// FUNCTION: STBC 0x4C4400 ??_G?$NiTArray@V?$NiPointer@VMetaVolume@@@@@@UAEPAXI@Z @game/DamagePolygonizer
// FUNCTION: STBC 0x7E6450 ??_G?$NiTArray@PAVNiAVObject@@@@UAEPAXI@Z @netimmerse/NiNode
// FUNCTION: STBC 0x7E6480 ??_G?$NiTArray@PAVNiDynamicEffect@@@@UAEPAXI@Z @netimmerse/NiNode
// FUNCTION: STBC 0x800450 ??_G?$NiTArray@PAVRange@NiLODNode@@@@UAEPAXI@Z @netimmerse/NiLODNode
// FUNCTION: STBC 0x4546A0 ??_G?$NiTArray@V?$NiPointer@VNiImage@@@@@@UAEPAXI@Z @game/LODModel
// FUNCTION: STBC 0x6FDAC0 ??_G?$NiTArray@PAUTGPhonemeSequenceEntry@@@@UAEPAXI@Z @game/TGActionModule
// FUNCTION: STBC 0x794490 ??_G?$NiTArray@PAVNiImage@@@@UAEPAXI@Z @netimmerse/NiFlipController
// FUNCTION: STBC 0x7C4910 ??1?$NiTArray@PAVNiD3DFrameBuffer@@@@UAE@XZ
// FUNCTION: STBC 0x48AF10 ??1?$NiTArray@PAJ@@UAE@XZ @game/OptimizedSelectTarget
// FUNCTION: STBC 0x812130 ??1?$NiTArray@PAVRecord@NiPick@@@@UAE@XZ @netimmerse/NiPick
// FUNCTION: STBC 0x812190 ??_G?$NiTArray@PAVRecord@NiPick@@@@UAEPAXI@Z @netimmerse/NiPick
// FUNCTION: STBC 0x7E5D80 ??1?$NiTArray@PAVNiAVObject@@@@UAE@XZ @netimmerse/NiNode
// FUNCTION: STBC 0x46FBF0 ??1?$NiTArray@PAVTimeSliceProcess@@@@UAE@XZ @game/TimeSliceProcess
// FUNCTION: STBC 0x46FCB0 ??_G?$NiTArray@PAVTimeSliceProcess@@@@UAEPAXI@Z @game/TimeSliceProcess
// FUNCTION: STBC 0x5376F0 ??1?$NiTArray@PAVShipSubsystem@@@@UAE@XZ @game/STSubsystemMenu
// FUNCTION: STBC 0x537710 ??_G?$NiTArray@PAVShipSubsystem@@@@UAEPAXI@Z @game/STSubsystemMenu
// FUNCTION: STBC 0x4B3490 ??1?$NiTArray@PAUDamageVertexBlock@@@@UAE@XZ @game/DamageMesh
// FUNCTION: STBC 0x4B0F50 ??1?$NiTArray@PAUOBBTriangleList@@@@UAE@XZ @game/Voxelizer
// FUNCTION: STBC 0x4B0F70 ??_G?$NiTArray@PAUOBBTriangleList@@@@UAEPAXI@Z @game/Voxelizer
// FUNCTION: STBC 0x41BA90 ??_G?$NiTArray@PAUAsteroidTileEntry@@@@UAEPAXI@Z @game/AsteroidField
// FUNCTION: STBC 0x41BAC0 ??_G?$NiTArray@PAUAsteroidTileSlot@@@@UAEPAXI@Z @game/AsteroidField
// FUNCTION: STBC 0x4B34B0 ??_G?$NiTArray@PAUDamageVertexBlock@@@@UAEPAXI@Z @game/DamageMesh
// FUNCTION: STBC 0x4B34E0 ??_G?$NiTArray@PAG@@UAEPAXI@Z @game/DamageMesh
// FUNCTION: STBC 0x7EAE50 ??1?$NiTArray@V?$NiPointer@VNiScreenPolygon@@@@@@UAE@XZ
// FUNCTION: STBC 0x7EAF10 ??1?$NiTArray@PAVNiPlane@@@@UAE@XZ
// FUNCTION: STBC 0x818D60 ??1?$NiTArray@PAVNiObject@@@@UAE@XZ @netimmerse/NiStream
// FUNCTION: STBC 0x4732B0 ??1?$NiTArray@PAUAIManagerEntry@@@@UAE@XZ @game/AIManager
// FUNCTION: STBC 0x48B240 ??_G?$NiTArray@PAJ@@UAEPAXI@Z @game/OptimizedSelectTarget
// FUNCTION: STBC 0x4733D0 ??_G?$NiTArray@PAUAIManagerEntry@@@@UAEPAXI@Z @game/AIManager
// FUNCTION: STBC 0x7C4B10 ??_G?$NiTArray@PAVNiD3DFrameBuffer@@@@UAEPAXI@Z
// FUNCTION: STBC 0x818DB0 ??_G?$NiTArray@PAVNiObject@@@@UAEPAXI@Z
// FUNCTION: STBC 0x7EAF50 ??_G?$NiTArray@K@@UAEPAXI@Z
// FUNCTION: STBC 0x456D40 ??_G?$NiTArray@I@@UAEPAXI@Z @game/TGFuzzyTriShape
// FUNCTION: STBC 0x7A5420 ??_G?$NiTArray@V?$NiPointer@VNiKeyframeController@@@@@@UAEPAXI@Z @netimmerse/NiKeyframeManager
// FUNCTION: STBC 0x7A54F0 ??_G?$NiTArray@J@@UAEPAXI@Z @netimmerse/NiKeyframeManager
// FUNCTION: STBC 0x7A5520 ??_G?$NiTArray@PAVNiTextKeyExtraData@@@@UAEPAXI@Z @netimmerse/NiKeyframeManager
// FUNCTION: STBC 0x7A5550 ??_G?$NiTArray@PAVNiKeyframeController@@@@UAEPAXI@Z @netimmerse/NiKeyframeManager
// FUNCTION: STBC 0x803470 ??_G?$NiTArray@PAVMap@NiMultiTextureProperty@@@@UAEPAXI@Z @netimmerse/NiMultiTextureProperty
// FUNCTION: STBC 0x7EAF80 ??_G?$NiTArray@V?$NiPointer@VNiScreenPolygon@@@@@@UAEPAXI@Z
// FUNCTION: STBC 0x7EAFD0 ??_G?$NiTArray@PAVNiPlane@@@@UAEPAXI@Z
// FUNCTION: STBC 0x7D7C70 ??_G?$NiTArray@PBVNiDDPixelDesc@@@@UAEPAXI@Z @netimmerse/renderer/NiD3DPixelFormat
// FUNCTION: STBC 0x674A30 ??1?$NiTArray@PAVBoneStateController@@@@UAE@XZ @game/RotateBonesController
// FUNCTION: STBC 0x674B40 ??_G?$NiTArray@PAVBoneStateController@@@@UAEPAXI@Z @game/RotateBonesController
// FUNCTION: STBC 0x7B3480 ??1?$NiTArray@H@@UAE@XZ
// FUNCTION: STBC 0x7B34E0 ??1?$NiTArray@PAVNiForce@@@@UAE@XZ
// FUNCTION: STBC 0x446AD0 ??1?$NiTArray@PAD@@UAE@XZ @game/UtopiaModule
// FUNCTION: STBC 0x454700 ??_G?$NiTArray@PAVLODModelPart@@@@UAEPAXI@Z @game/LODModel
// FUNCTION: STBC 0x446DE0 ??_G?$NiTArray@PAD@@UAEPAXI@Z @game/UtopiaModule
// FUNCTION: STBC 0x446E10 ??_G?$NiTArray@PAVObjectClass@@@@UAEPAXI@Z @game/UtopiaModule
// FUNCTION: STBC 0x7B3560 ??_G?$NiTArray@H@@UAEPAXI@Z
// FUNCTION: STBC 0x7B3590 ??_G?$NiTArray@PAVNiForce@@@@UAEPAXI@Z
// FUNCTION: STBC 0x4F4120 ??1?$NiTArray@PAVTextureAnimTrack@@@@UAE@XZ @game/TextureAnimManager
// FUNCTION: STBC 0x4F4290 ??_G?$NiTArray@PAVTextureAnimTrack@@@@UAEPAXI@Z @game/TextureAnimManager
// FUNCTION: STBC 0x587850 ??_G?$NiTArray@PAVNiPoint3@@@@UAEPAXI@Z @game/CollisionEvent
// FUNCTION: STBC 0x65F6D0 ??_G?$NiTArray@PAVBoneKeyList@@@@UAEPAXI@Z @game/BoneState
// FUNCTION: STBC 0x46C630 ??_G?$NiTArray@PAVNiTriShape@@@@UAEPAXI@Z @game/OBBRoot
// FUNCTION: STBC 0x508D90 ??1?$NiTArray@PAVTGParagraph@@@@UAE@XZ @game/SubtitleWindow
// FUNCTION: STBC 0x508F30 ??_G?$NiTArray@PAVTGParagraph@@@@UAEPAXI@Z @game/SubtitleWindow
// FUNCTION: STBC 0x50FC40 ??_G?$NiTArray@PAVTGUIObject@@@@UAEPAXI@Z @game/TopWindow
// FUNCTION: STBC 0x7E64B0 ??_G?$NiTArray@V?$NiPointer@VNiAVObject@@@@@@UAEPAXI@Z @netimmerse/NiNode
// FUNCTION: STBC 0x58CA90 ??_G?$NiTArray@PAVBackdrop@@@@UAEPAXI@Z @game/Backdrop
// FUNCTION: STBC 0x665520 ??1?$NiTArray@PAUBridgeTexture@@@@UAE@XZ @game/BridgeObjectClass
// FUNCTION: STBC 0x665850 ??_G?$NiTArray@PAUBridgeTexture@@@@UAEPAXI@Z @game/BridgeObjectClass
// FUNCTION: STBC 0x66F2F0 ??1?$NiTArray@PAURandomAnimation@@@@UAE@XZ @game/CharacterClass
// FUNCTION: STBC 0x66F5C0 ??_G?$NiTArray@PAURandomAnimation@@@@UAEPAXI@Z @game/CharacterClass
// FUNCTION: STBC 0x6DA4E0 ??_G?$NiTArray@PAVTGEventHandlerTable@@@@UAEPAXI@Z @game/TGEventManager
// FUNCTION: STBC 0x6E24B0 ??1?$NiTArray@PAVTGBroadcastHandler@@@@UAE@XZ @game/TGBroadcastHandlerList
// FUNCTION: STBC 0x6E2540 ??_G?$NiTArray@PAVTGBroadcastHandler@@@@UAEPAXI@Z @game/TGBroadcastHandlerList
// FUNCTION: STBC 0x454080 ??1?$NiTArray@V?$NiPointer@VNiImage@@@@@@UAE@XZ @game/LODModel
// FUNCTION: STBC 0x596190 ??1?$NiTArray@V?$NiPointer@VOBBModel@@@@@@UAE@XZ @game/DamageableObject
// FUNCTION: STBC 0x5961F0 ??1?$NiTArray@PAVVisibleDamage@@@@UAE@XZ @game/DamageableObject
// FUNCTION: STBC 0x726BD0 ??1?$NiTArray@PAVTGIconPoly@@@@UAE@XZ @game/TGUIModule
// FUNCTION: STBC 0x726BF0 ??_G?$NiTArray@PAVTGIconPoly@@@@UAEPAXI@Z @game/TGUIModule
// FUNCTION: STBC 0x72AD70 ??1?$NiTArray@PAVTGIconGroupRegistration@@@@UAE@XZ @game/TGIconManager
// FUNCTION: STBC 0x72CEA0 ??1?$NiTArray@V?$NiPointer@VNiTextureProperty@@@@@@UAE@XZ @game/TGIconGroup
// FUNCTION: STBC 0x5962F0 ??_G?$NiTArray@V?$NiPointer@VOBBModel@@@@@@UAEPAXI@Z @game/DamageableObject
// FUNCTION: STBC 0x596340 ??_G?$NiTArray@PAVVisibleDamage@@@@UAEPAXI@Z @game/DamageableObject
// FUNCTION: STBC 0x72AEE0 ??_G?$NiTArray@PAVTGIconGroupRegistration@@@@UAEPAXI@Z @game/TGIconManager
// FUNCTION: STBC 0x72AF90 ??_G?$NiTArray@V?$NiPointer@VNiTextureProperty@@@@@@UAEPAXI@Z @game/TGIconManager
template <class T> inline NiTArray<T>::~NiTArray() {
	delete [] base;
}

/*
================
NiTArray::SetSize

Reallocates the block for maxSize_ elements; elements past the new end are
dropped. The new maximum is stored before the old block is saved, the other
way round from Gamebryo 1.1.2's text: the store and the saved-block copy tie
in the scheduler, and only this order gives retail's store before the push
(OTREventHandlerModule::VirtualSlot00, TGUIModule::RegisterClasses,
TGIconGroup::TGIconGroup). Not declared inline: the /Ob1 game units call
their instances out of line (NiTArray<AIManagerEntry *>'s at 0x473750),
while the /Ob2 NetImmerse units still expand it (NiBSPNode::CreateClone,
0x7F8380).
================
*/
// FUNCTION: STBC 0x8034A0 ?SetSize@?$NiTArray@PAVNiImage@@@@QAEXI@Z @netimmerse/NiMultiTextureProperty
// FUNCTION: STBC 0x4BB9F0 ?SetSize@?$NiTArray@PAUDamageVertexBlock@@@@QAEXI@Z @game/VisibleDamageBuilder
// FUNCTION: STBC 0x4BBAE0 ?SetSize@?$NiTArray@PAG@@QAEXI@Z @game/VisibleDamageBuilder
// FUNCTION: STBC 0x41BB70 ?SetSize@?$NiTArray@PAUAsteroidTileEntry@@@@QAEXI@Z @game/AsteroidField
// FUNCTION: STBC 0x41BC60 ?SetSize@?$NiTArray@PAUAsteroidTileSlot@@@@QAEXI@Z @game/AsteroidField
// FUNCTION: STBC 0x473750 ?SetSize@?$NiTArray@PAUAIManagerEntry@@@@QAEXI@Z @game/AIManager
// FUNCTION: STBC 0x454C90 ?SetSize@?$NiTArray@PAD@@QAEXI@Z
// FUNCTION: STBC 0x803590 ?SetSize@?$NiTArray@PAVMap@NiMultiTextureProperty@@@@QAEXI@Z @netimmerse/NiMultiTextureProperty
// FUNCTION: STBC 0x537770 ?SetSize@?$NiTArray@PAVShipSubsystem@@@@QAEXI@Z @game/STSubsystemMenu
// FUNCTION: STBC 0x7EB010 ?SetSize@?$NiTArray@PAVNiPlane@@@@QAEXI@Z
// FUNCTION: STBC 0x7C4BC0 ?SetSize@?$NiTArray@PAVNiD3DFrameBuffer@@@@QAEXI@Z
// FUNCTION: STBC 0x73C840 ?SetSize@?$NiTArray@PAVTGIconPoly@@@@QAEXI@Z @game/TGTextButton
// FUNCTION: STBC 0x7A5840 ?SetSize@?$NiTArray@J@@QAEXI@Z
// FUNCTION: STBC 0x7A5950 ?SetSize@?$NiTArray@PAVNiTextKeyExtraData@@@@QAEXI@Z
// FUNCTION: STBC 0x46C810 ?SetSize@?$NiTArray@PAVNiTriShape@@@@QAEXI@Z @game/OBBRoot
// FUNCTION: STBC 0x46C900 ?SetSize@?$NiTArray@PAVRecord@NiPick@@@@QAEXI@Z @game/OBBNode
// FUNCTION: STBC 0x46C980 ?SetSize@?$NiTArray@PAUOBBTriangleList@@@@QAEXI@Z @game/OBBNode
// FUNCTION: STBC 0x508DB0 ?SetSize@?$NiTArray@PAVTGParagraph@@@@QAEXI@Z @game/SubtitleWindow
// FUNCTION: STBC 0x7F89B0 ?SetSize@?$NiTArray@V?$NiPointer@VNiAVObject@@@@@@QAEXI@Z @netimmerse/NiBSPNode
// FUNCTION: STBC 0x6E25A0 ?SetSize@?$NiTArray@PAVTGBroadcastHandler@@@@QAEXI@Z @game/TGBroadcastHandlerList
// FUNCTION: STBC 0x447340 ?SetSize@?$NiTArray@PAVTGEventHandlerTable@@@@QAEXI@Z @game/UtopiaModule
// FUNCTION: STBC 0x674C60 ?SetSize@?$NiTArray@PAVBoneStateController@@@@QAEXI@Z @game/RotateBonesController
// FUNCTION: STBC 0x7E6680 ?SetSize@?$NiTArray@PAVNiAVObject@@@@QAEXI@Z @netimmerse/NiNode
// FUNCTION: STBC 0x7DDF10 ?SetSize@?$NiTArray@I@@QAEXI@Z @netimmerse/NiAVObject
// FUNCTION: STBC 0x7B3850 ?SetSize@?$NiTArray@PAVNiForce@@@@QAEXI@Z @netimmerse/NiParticleSystemController
template <class T> void NiTArray<T>::SetSize( unsigned int maxSize_ ) {
	// RULE-EXCEPTION: R35, the early return has no braces, unlike Gamebryo
	// 1.1.2's text: the braces cost two tuples of inline cost, and only the
	// cheaper body expands the third Add's SetAt in
	// NiTransparentProperty::GetViewerStrings while keeping SetSize out of
	// line in LODModel::AddGlowPassTexture's second Add (no other function
	// moves); the same saving in SetAtGrow loses four matches
	// NiTArray.inl's asserts here ("uiMaxSize > 0", "m_pBase") are left
	// out: either one costs NiTransparentProperty::GetViewerStrings
	if ( maxSize_ == maxSize )
		return;

	unsigned int i;
	if ( maxSize_ < size ) {
		for ( i = maxSize_; i < size; i++ ) {
			if ( base[i] != T( 0 ) ) {
				base[i] = T( 0 );
				eSize--;
			}
		}
		size = maxSize_;
	}

	maxSize = maxSize_;
	T *pSaveBase = base;
	base = new T[maxSize];
	for ( i = 0; i < size; i++ ) {
		base[i] = pSaveBase[i];
	}
	delete [] pSaveBase;
}

/*
================
NiTArray::SetAt

Stores an element inside the allocated block, keeping the counts of used
and non-null elements. Gamebryo 1.2.2: NiTArray::SetAt tests against T( 0 ),
which for a smart pointer element is a compare with a null NiPointer.
================
*/
// FUNCTION: STBC 0x800480 ?SetAt@?$NiTArray@PAVRange@NiLODNode@@@@QAEXIPAVRange@NiLODNode@@@Z @netimmerse/NiLODNode
// FUNCTION: STBC 0x46C580 ?SetAt@?$NiTArray@PAVRecord@NiPick@@@@QAEXIPAVRecord@NiPick@@@Z @game/OBBNode
// FUNCTION: STBC 0x803520 ?SetAt@?$NiTArray@PAVNiImage@@@@QAEXIPAVNiImage@@@Z @netimmerse/NiMultiTextureProperty
// FUNCTION: STBC 0x7A56A0 ?SetAt@?$NiTArray@V?$NiPointer@VNiKeyframeController@@@@@@QAEXIV?$NiPointer@VNiKeyframeController@@@@@Z @netimmerse/NiKeyframeManager
// FUNCTION: STBC 0x454D10 ?SetAt@?$NiTArray@PAD@@QAEXIPAD@Z
// FUNCTION: STBC 0x41BBF0 ?SetAt@?$NiTArray@PAUAsteroidTileEntry@@@@QAEXIPAUAsteroidTileEntry@@@Z @game/AsteroidField
// FUNCTION: STBC 0x7E5E20 ?SetAt@?$NiTArray@V?$NiPointer@VNiAVObject@@@@@@QAEXIV?$NiPointer@VNiAVObject@@@@@Z
// FUNCTION: STBC 0x5377F0 ?SetAt@?$NiTArray@PAVShipSubsystem@@@@QAEXIPAVShipSubsystem@@@Z @game/STSubsystemMenu
// FUNCTION: STBC 0x7EB090 ?SetAt@?$NiTArray@PAVNiPlane@@@@QAEXIPAVNiPlane@@@Z
// FUNCTION: STBC 0x7EB120 ?SetAt@?$NiTArray@K@@QAEXIK@Z @netimmerse/NiCamera
// FUNCTION: STBC 0x7C4930 ?SetAt@?$NiTArray@PAVNiD3DFrameBuffer@@@@QAEXIPAVNiD3DFrameBuffer@@@Z
// FUNCTION: STBC 0x7BCCF0 ?SetAt@?$NiTArray@V?$NiPointer@VNiScreenPolygon@@@@@@QAEXIV?$NiPointer@VNiScreenPolygon@@@@@Z
// FUNCTION: STBC 0x73C8C0 ?SetAt@?$NiTArray@PAVTGIconPoly@@@@QAEXIPAVTGIconPoly@@@Z @game/TGTextButton
// FUNCTION: STBC 0x7A58E0 ?SetAt@?$NiTArray@J@@QAEXIJ@Z
// FUNCTION: STBC 0x50FCA0 ?SetAt@?$NiTArray@PAVTGUIObject@@@@QAEXIPAVTGUIObject@@@Z @game/OptionsWindow
// FUNCTION: STBC 0x7A59D0 ?SetAt@?$NiTArray@PAVNiTextKeyExtraData@@@@QAEXIPAVNiTextKeyExtraData@@@Z
// FUNCTION: STBC 0x48B420 ?SetAt@?$NiTArray@PAJ@@QAEXIPAJ@Z @game/OptimizedSelectTarget
// FUNCTION: STBC 0x46C890 ?SetAt@?$NiTArray@PAVNiTriShape@@@@QAEXIPAVNiTriShape@@@Z @game/OBBRoot
// FUNCTION: STBC 0x46CA00 ?SetAt@?$NiTArray@PAUOBBTriangleList@@@@QAEXIPAUOBBTriangleList@@@Z @game/OBBNode
// FUNCTION: STBC 0x7A5A60 ?SetAt@?$NiTArray@PAVNiKeyframeController@@@@QAEXIPAVNiKeyframeController@@@Z @netimmerse/NiKeyframeManager
// FUNCTION: STBC 0x4BBA70 ?SetAt@?$NiTArray@PAUDamageVertexBlock@@@@QAEXIPAUDamageVertexBlock@@@Z @game/VisibleDamageBuilder
// FUNCTION: STBC 0x4B2480 ?SetAt@?$NiTArray@V?$NiPointer@VMetaVolume@@@@@@QAEXIV?$NiPointer@VMetaVolume@@@@@Z @game/MetaVolume
// FUNCTION: STBC 0x453AE0 ?SetAt@?$NiTArray@PAVMap@NiMultiTextureProperty@@@@QAEXIPAVMap@NiMultiTextureProperty@@@Z @game/ModelHelpers
// FUNCTION: STBC 0x66F310 ?SetAt@?$NiTArray@PAURandomAnimation@@@@QAEXIPAURandomAnimation@@@Z @game/CharacterClass
// FUNCTION: STBC 0x6E24D0 ?SetAt@?$NiTArray@PAVTGBroadcastHandler@@@@QAEXIPAVTGBroadcastHandler@@@Z @game/TGBroadcastHandlerList
// FUNCTION: STBC 0x4473C0 ?SetAt@?$NiTArray@PAVTGEventHandlerTable@@@@QAEXIPAVTGEventHandlerTable@@@Z @game/UtopiaModule
// FUNCTION: STBC 0x674CE0 ?SetAt@?$NiTArray@PAVBoneStateController@@@@QAEXIPAVBoneStateController@@@Z @game/RotateBonesController
// FUNCTION: STBC 0x72B170 ?SetAt@?$NiTArray@PAVTGIconGroupRegistration@@@@QAEXIPAVTGIconGroupRegistration@@@Z @game/TGIconManager
// FUNCTION: STBC 0x72D1F0 ?SetAt@?$NiTArray@V?$NiPointer@VNiTextureProperty@@@@@@QAEXIV?$NiPointer@VNiTextureProperty@@@@@Z @game/TGIconGroup
// FUNCTION: STBC 0x454D80 ?SetAt@?$NiTArray@V?$NiPointer@VNiImage@@@@@@QAEXIV?$NiPointer@VNiImage@@@@@Z @game/LODModel
// FUNCTION: STBC 0x454E90 ?SetAt@?$NiTArray@V?$NiPointer@VOBBModel@@@@@@QAEXIV?$NiPointer@VOBBModel@@@@@Z @game/LODModel
// FUNCTION: STBC 0x7E6700 ?SetAt@?$NiTArray@PAVNiAVObject@@@@QAEXIPAVNiAVObject@@@Z @netimmerse/NiNode
// FUNCTION: STBC 0x7B38D0 ?SetAt@?$NiTArray@PAVNiForce@@@@QAEXIPAVNiForce@@@Z @netimmerse/NiParticleSystemController
template <class T> inline void NiTArray<T>::SetAt( unsigned int index, T element ) {
	// NiTArray.inl's "uiIndex < m_uiMaxSize" assert is left out: it costs
	// NiMaterialProperty's and NiTransparentProperty's GetViewerStrings
	if ( index >= maxSize ) {
		return;
	}

	if ( index >= size ) {
		size = index + 1;
		if ( element != T( 0 ) ) {
			eSize++;
		}
	} else if ( element != T( 0 ) ) {
		if ( base[index] == T( 0 ) ) {
			eSize++;
		}
	} else if ( base[index] != T( 0 ) ) {
		eSize--;
	}
	base[index] = element;
}

/*
================
NiTArray::SetAtGrow

Stores an element, growing the block first when the index is past its end.
================
*/
// FUNCTION: STBC 0x7B35C0 ?SetAtGrow@?$NiTArray@H@@QAEIIH@Z @netimmerse/NiParticleSystemController
// FUNCTION: STBC 0x7B36B0 ?SetAtGrow@?$NiTArray@PAE@@QAEIIPAE@Z @netimmerse/NiParticleSystemController
// FUNCTION: STBC 0x7E5F40 ?SetAtGrow@?$NiTArray@V?$NiPointer@VNiAVObject@@@@@@QAEIIV?$NiPointer@VNiAVObject@@@@@Z
// FUNCTION: STBC 0x596470 ?SetAtGrow@?$NiTArray@PAVNiPoint3@@@@QAEIIPAVNiPoint3@@@Z @game/DamageableObject
// FUNCTION: STBC 0x454850 ?SetAtGrow@?$NiTArray@V?$NiPointer@VNiImage@@@@@@QAEIIV?$NiPointer@VNiImage@@@@@Z @game/LODModel
// FUNCTION: STBC 0x454A00 ?SetAtGrow@?$NiTArray@V?$NiPointer@VOBBModel@@@@@@QAEIIV?$NiPointer@VOBBModel@@@@@Z @game/LODModel
// FUNCTION: STBC 0x454B70 ?SetAtGrow@?$NiTArray@PAVVisibleDamage@@@@QAEIIPAVVisibleDamage@@@Z @game/LODModel
// FUNCTION: STBC 0x7BCB80 ?SetAtGrow@?$NiTArray@V?$NiPointer@VNiScreenPolygon@@@@@@QAEIIV?$NiPointer@VNiScreenPolygon@@@@@Z
// FUNCTION: STBC 0x4B2270 ?SetAtGrow@?$NiTArray@V?$NiPointer@VMetaVolume@@@@@@QAEIIV?$NiPointer@VMetaVolume@@@@@Z @game/MetaVolume
// FUNCTION: STBC 0x7C7420 ?SetAtGrow@?$NiTArray@PBVNiDDPixelDesc@@@@QAEIIPBVNiDDPixelDesc@@@Z @netimmerse/renderer/NiDDImage
// FUNCTION: STBC 0x7E6560 ?SetAtGrow@?$NiTArray@PAVRecord@NiPick@@@@QAEIIPAVRecord@NiPick@@@Z @netimmerse/NiNode
// FUNCTION: STBC 0x7A4DF0 ?SetAtGrow@?$NiTArray@V?$NiPointer@VNiKeyframeController@@@@@@QAEIIV?$NiPointer@VNiKeyframeController@@@@@Z @netimmerse/NiKeyframeManager
// FUNCTION: STBC 0x803320 ?SetAtGrow@?$NiTArray@PAVMap@NiMultiTextureProperty@@@@QAEIIPAVMap@NiMultiTextureProperty@@@Z @netimmerse/NiMultiTextureProperty
// FUNCTION: STBC 0x7DDDF0 ?SetAtGrow@?$NiTArray@PAD@@QAEIIPAD@Z @netimmerse/NiAVObject
// FUNCTION: STBC 0x818DF0 ?SetAtGrow@?$NiTArray@PAVNiObject@@@@QAEIIPAVNiObject@@@Z @netimmerse/NiStream
// FUNCTION: STBC 0x46C6F0 ?SetAtGrow@?$NiTArray@PAUOBBTriangleList@@@@QAEIIPAUOBBTriangleList@@@Z @game/OBBRoot
// FUNCTION: STBC 0x6659A0 ?SetAtGrow@?$NiTArray@PAUBridgeTexture@@@@QAEIIPAUBridgeTexture@@@Z @game/BridgeObjectClass
// FUNCTION: STBC 0x4470C0 ?SetAtGrow@?$NiTArray@PAVTGEventHandlerTable@@@@QAEIIPAVTGEventHandlerTable@@@Z @game/UtopiaModule
// FUNCTION: STBC 0x72D050 ?SetAtGrow@?$NiTArray@V?$NiPointer@VNiTextureProperty@@@@@@QAEIIV?$NiPointer@VNiTextureProperty@@@@@Z @game/TGIconGroup
// FUNCTION: STBC 0x4C48B0 ?SetAtGrow@?$NiTArray@PAUDamageVertexBlock@@@@QAEIIPAUDamageVertexBlock@@@Z @game/DamagePolygonizer
// FUNCTION: STBC 0x4C49A0 ?SetAtGrow@?$NiTArray@PAG@@QAEIIPAG@Z @game/DamagePolygonizer
template <class T> inline unsigned int NiTArray<T>::SetAtGrow( unsigned int index, T element ) {
	if ( index >= maxSize ) {
		SetSize( index + growBy );
	}
	SetAt( index, element );
	return index;
}

/*
================
NiTArray::Add

Appends an element; NiObject::LoadBinary (0x7D8820) keeps the by-value
element in a register across the reallocation.
================
*/
// FUNCTION: STBC 0x7E60C0 ?Add@?$NiTArray@V?$NiPointer@VNiAVObject@@@@@@QAEXV?$NiPointer@VNiAVObject@@@@@Z
// FUNCTION: STBC 0x4540E0 ?Add@?$NiTArray@V?$NiPointer@VNiImage@@@@@@QAEXV?$NiPointer@VNiImage@@@@@Z @game/LODModel
// FUNCTION: STBC 0x454610 ?Add@?$NiTArray@V?$NiPointer@VOBBModel@@@@@@QAEXV?$NiPointer@VOBBModel@@@@@Z @game/LODModel
// FUNCTION: STBC 0x4B2120 ?Add@?$NiTArray@V?$NiPointer@VMetaVolume@@@@@@QAEXV?$NiPointer@VMetaVolume@@@@@Z @game/MetaVolume
// FUNCTION: STBC 0x72CED0 ?Add@?$NiTArray@V?$NiPointer@VNiTextureProperty@@@@@@QAEXV?$NiPointer@VNiTextureProperty@@@@@Z @game/TGIconGroup
// FUNCTION: STBC 0x7EAE90 ?Add@?$NiTArray@V?$NiPointer@VNiScreenPolygon@@@@@@QAEXV?$NiPointer@VNiScreenPolygon@@@@@Z @netimmerse/NiCamera
template <class T> inline void NiTArray<T>::Add( T element ) {
	SetAtGrow( size, element );
}

/*
================
NiTArray::AddFirstEmpty

Stores a non-null element in the first empty slot, or appends it.
================
*/
// FUNCTION: STBC 0x7E6130 ?AddFirstEmpty@?$NiTArray@V?$NiPointer@VNiAVObject@@@@@@QAEXV?$NiPointer@VNiAVObject@@@@@Z
// FUNCTION: STBC 0x7BCA80 ?AddFirstEmpty@?$NiTArray@V?$NiPointer@VNiScreenPolygon@@@@@@QAEXV?$NiPointer@VNiScreenPolygon@@@@@Z
// FUNCTION: STBC 0x7F0FF0 ?AddFirstEmpty@?$NiTArray@PAVRecord@NiPick@@@@QAEXPAVRecord@NiPick@@@Z @netimmerse/NiTriBasedGeom
template <class T> inline void NiTArray<T>::AddFirstEmpty( T element ) {
	if ( element == T( 0 ) ) {
		return;
	}

	for ( unsigned int i = 0; i < size; i++ ) {
		if ( base[i] == T( 0 ) ) {
			base[i] = element;
			eSize++;
			return;
		}
	}
	SetAtGrow( size, element );
}

/*
================
NiTArray::RemoveAt

Empties a slot and returns its element; the used size shrinks only when
the last slot is emptied.
================
*/
// FUNCTION: STBC 0x794380 ?RemoveAt@?$NiTArray@V?$NiPointer@VNiImage@@@@@@QAE?AV?$NiPointer@VNiImage@@@@I@Z @netimmerse/NiFlipController
// FUNCTION: STBC 0x7E6230 ?RemoveAt@?$NiTArray@V?$NiPointer@VNiAVObject@@@@@@QAE?AV?$NiPointer@VNiAVObject@@@@I@Z
// FUNCTION: STBC 0x7A4F60 ?RemoveAt@?$NiTArray@V?$NiPointer@VNiKeyframeController@@@@@@QAE?AV?$NiPointer@VNiKeyframeController@@@@I@Z @netimmerse/NiKeyframeManager
// FUNCTION: STBC 0x72CF40 ?RemoveAt@?$NiTArray@V?$NiPointer@VNiTextureProperty@@@@@@QAE?AV?$NiPointer@VNiTextureProperty@@@@I@Z @game/TGIconGroup
// FUNCTION: STBC 0x812150 ?RemoveAt@?$NiTArray@PAVRecord@NiPick@@@@QAEPAVRecord@NiPick@@I@Z @netimmerse/NiPick
template <class T> inline T NiTArray<T>::RemoveAt( unsigned int index ) {
	if ( index >= size ) {
		return T( 0 );
	}

	T element = base[index];
	base[index] = T( 0 );
	if ( element != T( 0 ) ) {
		eSize--;
	}
	if ( index == size - 1 ) {
		size--;
	}
	return element;
}

/*
================
NiTArray::RemoveEnd

Empties the last used slot and returns its element (Gamebryo lineage
name). ~AIManager (0x472120) empties its array through it.
================
*/
template <class T> inline T NiTArray<T>::RemoveEnd() {
	if ( size == 0 ) {
		return T( 0 );
	}

	size--;
	T element = base[size];
	base[size] = T( 0 );
	if ( element != T( 0 ) ) {
		eSize--;
	}
	return element;
}

/*
================
NiTArray::RemoveAll

Clears every element; the block keeps its size. An array of smart
pointers assigns a null smart pointer (NiCamera's destructor, 0x7E7DB0).
================
*/
// FUNCTION: STBC 0x818D80 ?RemoveAll@?$NiTArray@PAVNiObject@@@@QAEXXZ @netimmerse/NiStream
// FUNCTION: STBC 0x4C3BF0 ?RemoveAll@?$NiTArray@V?$NiPointer@VMetaVolume@@@@@@QAEXXZ @game/DamagePolygonizer
// FUNCTION: STBC 0x589890 ?RemoveAll@?$NiTArray@V?$NiPointer@VNiAVObject@@@@@@QAEXXZ @game/CollisionTestDisplay
template <class T> inline void NiTArray<T>::RemoveAll() {
	for ( unsigned int i = 0; i < size; i++ ) {
		base[i] = T( 0 );
	}
	size = 0;
	eSize = 0;
}

/*
================
NiTArray::Compact

Moves the non-null elements to the front and reallocates the block to
exactly them.

Written as the SDK's debug NetImmerse 3.1 compiles it (AnimOnly.exe
0x4D9B50, NiTArray.inl): Gamebryo's shape with the locals j and pSaveBase
(the name decides the frame slot at /Od: saveBase moves it), reading base[i] in place, and no zero-size branch around the reallocation
(BC's NetImmerse is older than 1.1.2). Its inline cost leaves
TGIconManager::DeleteIconGroup (0x72A210) too little budget to expand
NiTList::RemoveTail (accepted, user's ruling 2026-11-20).
================
*/
template <class T> inline void NiTArray<T>::Compact() {
	if ( eSize == size ) {
		return;
	}

	// move elements to contiguous memory at beginning of array
	if ( eSize ) {
		for ( unsigned int i = 0, j = 0; i < size; i++ ) {
			if ( base[i] != T( 0 ) ) {
				if ( base[j] != base[i] ) {
					base[j] = base[i];
				}
				j++;
			}
		}
	}

	// downsize storage
	T *pSaveBase = base;
	size = eSize;
	maxSize = size;
	base = new T[maxSize];
	assert( base );

	// copy old array to new array
	for ( unsigned int i = 0; i < size; i++ ) {
		base[i] = pSaveBase[i];
	}

	// delete old array
	delete [] pSaveBase;
}

#endif /* !__NITARRAY_H__ */
