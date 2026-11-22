/*
===============================================================================

	NiPointer.h

	The reference-counting smart pointer that holds NetImmerse objects.

	Evidence: every pointer member that the destructors release follows the
	same inlined pattern: when the pointer changes, decrement the old
	object's count and call its scalar deleting destructor (vtable slot 10)
	at zero, then increment the new object's count.

===============================================================================
*/

#ifndef __NIPOINTER_H__
#define __NIPOINTER_H__

// The vector deleting destructors of the smart pointer arrays that NiTArray
// deletes (the node children's and the camera's screen polygons').
// FUNCTION: STBC 0x7A5470 ??_E?$NiPointer@VNiKeyframeController@@@@QAEPAXI@Z @netimmerse/NiKeyframeManager
// FUNCTION: STBC 0x401E30 ??_E?$NiPointer@VNiAVObject@@@@QAEPAXI@Z @game/ChangeRenderedSetAction
// FUNCTION: STBC 0x401EC0 ??_E?$NiPointer@VNiScreenPolygon@@@@QAEPAXI@Z @game/ChangeRenderedSetAction
// FUNCTION: STBC 0x7AF040 ??_E?$NiPointer@VNiAVObject@@@@QAEPAXI@Z @netimmerse/NiParticleSystemController
// FUNCTION: STBC 0x4547D0 ??_E?$NiPointer@VOBBModel@@@@QAEPAXI@Z @game/LODModel
// FUNCTION: STBC 0x407D60 ??_E?$NiPointer@VMetaVolume@@@@QAEPAXI@Z @game/Game
// FUNCTION: STBC 0x407CD0 ??_E?$NiPointer@VNiImage@@@@QAEPAXI@Z @game/Game
// FUNCTION: STBC 0x401FB0 ??_E?$NiPointer@VNiTextureProperty@@@@QAEPAXI@Z @game/ChangeRenderedSetAction
template <class T> class NiPointer {
public:
					// FUNCTION: STBC 0x7A54E0 ??_F?$NiPointer@VNiKeyframeController@@@@QAEXXZ @netimmerse/NiKeyframeManager
					// FUNCTION: STBC 0x7A4D50 ??0?$NiPointer@VNiKeyframeController@@@@QAE@PAVNiKeyframeController@@@Z @netimmerse/NiKeyframeManager
					// FUNCTION: STBC 0x7CCA30 ??_F?$NiPointer@VNiAlphaProperty@@@@QAEXXZ
					// FUNCTION: STBC 0x4E1750 ??_F?$NiPointer@VNiTriShape@@@@QAEXXZ @game/Glow
					// FUNCTION: STBC 0x4B23E0 ??_F?$NiPointer@VMetaVolume@@@@QAEXXZ @game/MetaVolume
					// FUNCTION: STBC 0x4546F0 ??_F?$NiPointer@VNiImage@@@@QAEXXZ @game/LODModel
					// FUNCTION: STBC 0x454840 ??_F?$NiPointer@VOBBModel@@@@QAEXXZ @game/LODModel
					// FUNCTION: STBC 0x46C4C0 ??0?$NiPointer@VNiAVObject@@@@QAE@PAVNiAVObject@@@Z @game/OBBNode
					// FUNCTION: STBC 0x4B21B0 ??0?$NiPointer@VNiBinaryVoxelData@@@@QAE@PAVNiBinaryVoxelData@@@Z @game/VisibleDamage
					// FUNCTION: STBC 0x4E1380 ??0?$NiPointer@VNiD3DRender@@@@QAE@PAVNiD3DRender@@@Z @game/Glow
					// FUNCTION: STBC 0x46C440 ??0?$NiPointer@VNiTriShape@@@@QAE@PAVNiTriShape@@@Z @game/OBBRoot
					// FUNCTION: STBC 0x4E1270 ??0?$NiPointer@VGlowAccumulator@@@@QAE@PAVGlowAccumulator@@@Z @game/Glow
					// FUNCTION: STBC 0x7C59D0 ??_F?$NiPointer@VNiRawImageData@@@@QAEXXZ
					// FUNCTION: STBC 0x7BC8A0 ??0?$NiPointer@VNiRawImageData@@@@QAE@PAVNiRawImageData@@@Z @netimmerse/NiFrameRate
					// FUNCTION: STBC 0x42FD00 ??0?$NiPointer@VNiProperty@@@@QAE@PAVNiProperty@@@Z @game/PointParticleController
					// FUNCTION: STBC 0x453AA0 ??0?$NiPointer@VNiImage@@@@QAE@PAVNiImage@@@Z @game/LODModel
					// FUNCTION: STBC 0x4544F0 ??0?$NiPointer@VOBBModel@@@@QAE@PAVOBBModel@@@Z @game/LODModel
					// FUNCTION: STBC 0x46C560 ??0?$NiPointer@VNiNode@@@@QAE@PAVNiNode@@@Z @game/OBBNode
					// FUNCTION: STBC 0x596170 ??0?$NiPointer@VMetaVolume@@@@QAE@PAVMetaVolume@@@Z @game/DamageableObject
					NiPointer( T *object_ = 0 ) { object = object_; if ( object ) object->IncRefCount(); }
					// FUNCTION: STBC 0x4E1100 ??0?$NiPointer@VNiProperty@@@@QAE@ABV0@@Z @game/Glow
					// FUNCTION: STBC 0x458070 ??0?$NiPointer@VNiRawImageData@@@@QAE@ABV0@@Z @game/TGOverlayController
					// FUNCTION: STBC 0x4E8FA0 ??0?$NiPointer@VNiNode@@@@QAE@ABV0@@Z @game/PulsingLight
					// FUNCTION: STBC 0x453A80 ??0?$NiPointer@VNiImage@@@@QAE@ABV0@@Z @game/LODModel
					NiPointer( const NiPointer &ptr ) { object = ptr.object; if ( object ) object->IncRefCount(); }
					// FUNCTION: STBC 0x7A4D70 ??1?$NiPointer@VNiKeyframeController@@@@QAE@XZ @netimmerse/NiKeyframeManager
					// FUNCTION: STBC 0x7A5070 ??1?$NiPointer@VNiTextKeyExtraData@@@@QAE@XZ @netimmerse/NiKeyframeManager
					// FUNCTION: STBC 0x78E8B0 ??1?$NiPointer@VNiListener@@@@QAE@XZ @netimmerse/NiSoundSystem
					// FUNCTION: STBC 0x78EAB0 ??1?$NiPointer@VNiProvider_Info@@@@QAE@XZ @netimmerse/NiSoundSystem
					// FUNCTION: STBC 0x7B3500 ??1?$NiPointer@VNiColorData@@@@QAE@XZ @netimmerse/NiParticleSystemController
					// FUNCTION: STBC 0x70CF30 ??1?$NiPointer@VNiSource@@@@QAE@XZ @game/TGSound
					// FUNCTION: STBC 0x416EC0 ??1?$NiPointer@VNiProperty@@@@QAE@XZ @game/SetClass
					// FUNCTION: STBC 0x417020 ??1?$NiPointer@VNiWireframeProperty@@@@QAE@XZ @game/SetClass
					// FUNCTION: STBC 0x416FE0 ??1?$NiPointer@VNiRender@@@@QAE@XZ @game/SetClass
					// FUNCTION: STBC 0x417000 ??1?$NiPointer@VNiFogProperty@@@@QAE@XZ @game/SetClass
					// FUNCTION: STBC 0x4177D0 ??1?$NiPointer@VNiDDBufferImage@@@@QAE@XZ @game/SetClass
					// FUNCTION: STBC 0x834B40 ??1?$NiPointer@VNiTriShapeData@@@@QAE@XZ @netimmerse/NiBezierPatch
					// FUNCTION: STBC 0x407D40 ??1?$NiPointer@VNiImage@@@@QAE@XZ @game/BackdropSphere
					// FUNCTION: STBC 0x401EA0 ??1?$NiPointer@VNiAVObject@@@@QAE@XZ @game/ChangeRenderedSetAction
					// FUNCTION: STBC 0x416F80 ??1?$NiPointer@VNiAVObject@@@@QAE@XZ @netimmerse/NiParticleSystemController
					// FUNCTION: STBC 0x401F30 ??1?$NiPointer@VNiScreenPolygon@@@@QAE@XZ @game/ChangeRenderedSetAction
					// FUNCTION: STBC 0x6CCCC0 ??1?$NiPointer@VNiKeyframeData@@@@QAE@XZ @game/TGNodeAnimation
					// FUNCTION: STBC 0x407DD0 ??1?$NiPointer@VMetaVolume@@@@QAE@XZ @game/Game
					// FUNCTION: STBC 0x58FE20 ??1?$NiPointer@VNiLines@@@@QAE@XZ @game/WaypointPathDisplay
					// FUNCTION: STBC 0x66F3C0 ??1?$NiPointer@VTGOverlayController@@@@QAE@XZ @game/CharacterClass
					// FUNCTION: STBC 0x4E1210 ??1?$NiPointer@VNiDynamicEffectState@@@@QAE@XZ @game/Glow
					// FUNCTION: STBC 0x458090 ??1?$NiPointer@VNiRawImageData@@@@QAE@XZ @game/TGOverlayController
					// FUNCTION: STBC 0x4580B0 ??1?$NiPointer@VTGOverlayImage@@@@QAE@XZ @game/TGOverlayController
					// FUNCTION: STBC 0x4E12F0 ??1?$NiPointer@VNiPropertyState@@@@QAE@XZ @game/Glow
					// FUNCTION: STBC 0x8209C0 ??1?$NiPointer@VNiCorrectionProperty@@@@QAE@XZ @netimmerse/NiPropertyState
					// FUNCTION: STBC 0x8209E0 ??1?$NiPointer@VNiShadeProperty@@@@QAE@XZ @netimmerse/NiPropertyState
					// FUNCTION: STBC 0x820A00 ??1?$NiPointer@VNiTransparentProperty@@@@QAE@XZ @netimmerse/NiPropertyState
					// FUNCTION: STBC 0x820A20 ??1?$NiPointer@VNiDitherProperty@@@@QAE@XZ @netimmerse/NiPropertyState
					// FUNCTION: STBC 0x820A40 ??1?$NiPointer@VNiSpecularProperty@@@@QAE@XZ @netimmerse/NiPropertyState
					// FUNCTION: STBC 0x820A60 ??1?$NiPointer@VNiStencilProperty@@@@QAE@XZ @netimmerse/NiPropertyState
					// FUNCTION: STBC 0x831F30 ??1?$NiPointer@VNiBezierPatch@@@@QAE@XZ @netimmerse/NiBezierMesh
					// FUNCTION: STBC 0x831F60 ??1?$NiPointer@VNiBezierRectangle@@@@QAE@XZ @netimmerse/NiBezierMesh
					// FUNCTION: STBC 0x831F80 ??1?$NiPointer@VNiBezierTriangle@@@@QAE@XZ @netimmerse/NiBezierMesh
					// FUNCTION: STBC 0x596150 ??1?$NiPointer@VTGAnimNode@@@@QAE@XZ @game/DamageableObject
					~NiPointer() { if ( object ) object->DecRefCount(); }

					// FUNCTION: STBC 0x4E1400 ??B?$NiPointer@VNiFogProperty@@@@QBEPAVNiFogProperty@@XZ @game/Glow
					// FUNCTION: STBC 0x453C30 ??B?$NiPointer@VNiVertexColorProperty@@@@QBEPAVNiVertexColorProperty@@XZ @game/LODModel
					// FUNCTION: STBC 0x453A70 ??B?$NiPointer@VNiMaterialProperty@@@@QBEPAVNiMaterialProperty@@XZ @game/LODModel
					// FUNCTION: STBC 0x453BE0 ??B?$NiPointer@VNiTextureProperty@@@@QBEPAVNiTextureProperty@@XZ @game/LODModel
					// FUNCTION: STBC 0x453B90 ??B?$NiPointer@VNiMultiTextureProperty@@@@QBEPAVNiMultiTextureProperty@@XZ @game/LODModel
					operator T *() const { return object; }
	T &				operator*() const { return *object; }
	T *				operator->() const { return object; }

	bool			operator==( T *object_ ) const { return object == object_; }
	bool			operator!=( T *object_ ) const { return object != object_; }
	bool			operator==( const NiPointer &ptr ) const { return object == ptr.object; }
	bool			operator!=( const NiPointer &ptr ) const { return object != ptr.object; }

	NiPointer &		operator=( T *object_ );
	NiPointer &		operator=( const NiPointer &ptr );

protected:
	T *				object;
};

/*
================
NiPointer::operator=

Releases the old object and takes a reference to the new one. The
reference is taken through the argument, not the member: the cheaper
spelling lets NiD3DDirectDraw::Create (0x7C7F80) inline as much as retail
does.
================
*/
// FUNCTION: STBC 0x7BCE00 ??4?$NiPointer@VNiScreenPolygon@@@@QAEAAV0@ABV0@@Z @netimmerse/NiFrameRate
// FUNCTION: STBC 0x7DB910 ??4?$NiPointer@VNiExtraData@@@@QAEAAV0@PAVNiExtraData@@@Z
// FUNCTION: STBC 0x7AE690 ??4?$NiPointer@VNiPosData@@@@QAEAAV0@PAVNiPosData@@@Z @netimmerse/NiPathController
// FUNCTION: STBC 0x7AE6D0 ??4?$NiPointer@VNiFloatData@@@@QAEAAV0@PAVNiFloatData@@@Z @netimmerse/NiPathController
// FUNCTION: STBC 0x7DB950 ??4?$NiPointer@VNiTimeController@@@@QAEAAV0@PAVNiTimeController@@@Z
// FUNCTION: STBC 0x7BC780 ??4?$NiPointer@VNiAlphaProperty@@@@QAEAAV0@PAVNiAlphaProperty@@@Z @netimmerse/NiFrameRate
// FUNCTION: STBC 0x7BC7C0 ??4?$NiPointer@VNiTextureModeProperty@@@@QAEAAV0@PAVNiTextureModeProperty@@@Z @netimmerse/NiFrameRate
// FUNCTION: STBC 0x7BC800 ??4?$NiPointer@VNiVertexColorProperty@@@@QAEAAV0@PAVNiVertexColorProperty@@@Z @netimmerse/NiFrameRate
// FUNCTION: STBC 0x453850 ??4?$NiPointer@VNiProperty@@@@QAEAAV0@PAVNiProperty@@@Z @netimmerse/NiPropertyState
// FUNCTION: STBC 0x45D9A0 ??4?$NiPointer@VNiImage@@@@QAEAAV0@PAVNiImage@@@Z @netimmerse/NiFrameRate
// FUNCTION: STBC 0x7B3520 ??4?$NiPointer@VNiColorData@@@@QAEAAV0@PAVNiColorData@@@Z @netimmerse/NiParticleSystemController
// FUNCTION: STBC 0x454510 ??4?$NiPointer@VOBBModel@@@@QAEAAV0@PAVOBBModel@@@Z @game/LODModel
// FUNCTION: STBC 0x453F50 ??4?$NiPointer@VNiBinaryVoxelData@@@@QAEAAV0@PAVNiBinaryVoxelData@@@Z @game/LODModel
// FUNCTION: STBC 0x6CC6A0 ??4?$NiPointer@VNiKeyframeData@@@@QAEAAV0@PAVNiKeyframeData@@@Z @game/TGAnimationEntry
// FUNCTION: STBC 0x416FA0 ??4?$NiPointer@VNiNode@@@@QAEAAV0@PAVNiNode@@@Z @game/SetClass
// FUNCTION: STBC 0x42D5B0 ??4?$NiPointer@VNiRender@@@@QAEAAV0@PAVNiRender@@@Z @game/CameraObjectClass
// The copy NiOBBLeaf::FindIntersections calls lies among the OBB game units (0x46C4E0): OBBLeaf's
// ray tests build their pick records through it.
// FUNCTION: STBC 0x46C4E0 ??4?$NiPointer@VNiAVObject@@@@QAEAAV0@PAVNiAVObject@@@Z @game/OBBNode
// FUNCTION: STBC 0x46C480 ??4?$NiPointer@VNiTriShape@@@@QAEAAV0@PAVNiTriShape@@@Z @game/OBBNode
// FUNCTION: STBC 0x4E1230 ??4?$NiPointer@VNiDynamicEffectState@@@@QAEAAV0@PAVNiDynamicEffectState@@@Z @game/Glow
// FUNCTION: STBC 0x4E12B0 ??4?$NiPointer@VNiAccumulator@@@@QAEAAV0@PAVNiAccumulator@@@Z @game/Glow
// FUNCTION: STBC 0x4E1320 ??4?$NiPointer@VNiPropertyState@@@@QAEAAV0@PAVNiPropertyState@@@Z @game/Glow
template <class T> inline NiPointer<T> &NiPointer<T>::operator=( T *object_ ) {
	if ( object != object_ ) {
		if ( object ) {
			object->DecRefCount();
		}
		object = object_;
		if ( object_ ) {
			object_->IncRefCount();
		}
	}
	return *this;
}

/*
================
NiPointer::operator=

Reads ptr.object at each use: NiD3DFrameBuffer's constructor (0x7C9CE0)
reloads it after the old object's release.
================
*/
// FUNCTION: STBC 0x7A5660 ??4?$NiPointer@VNiKeyframeController@@@@QAEAAV0@ABV0@@Z @netimmerse/NiKeyframeManager
// FUNCTION: STBC 0x7E6520 ??4?$NiPointer@VNiAVObject@@@@QAEAAV0@ABV0@@Z @netimmerse/NiNode
// FUNCTION: STBC 0x4B2590 ??4?$NiPointer@VMetaVolume@@@@QAEAAV0@ABV0@@Z @game/MetaVolume
// FUNCTION: STBC 0x72CE60 ??4?$NiPointer@VNiTextureProperty@@@@QAEAAV0@ABV0@@Z @game/TGIconGroup
// FUNCTION: STBC 0x44F130 ??4?$NiPointer@VNiImage@@@@QAEAAV0@ABV0@@Z @game/LODModel
// FUNCTION: STBC 0x4E1120 ??4?$NiPointer@VNiProperty@@@@QAEAAV0@ABV0@@Z @game/Glow
// FUNCTION: STBC 0x4549C0 ??4?$NiPointer@VOBBModel@@@@QAEAAV0@ABV0@@Z @game/LODModel
template <class T> inline NiPointer<T> &NiPointer<T>::operator=( const NiPointer &ptr ) {
	if ( object != ptr.object ) {
		if ( object ) {
			object->DecRefCount();
		}
		object = ptr.object;
		if ( object ) {
			object->IncRefCount();
		}
	}
	return *this;
}

// The object a smart pointer holds, as one of its derived classes (Gamebryo
// lineage; AnimOnly.exe's asserts quote it: "usTriangle <
// NiSmartPointerCast(NiTrianglesData, m_spModelData) ->GetTriangleCount()" at 0x45097B)
#define NiSmartPointerCast( type, smartptr ) ( (type *)(void *)( smartptr ) )

#endif /* !__NIPOINTER_H__ */
