/*
===============================================================================

	NiGeometry.h

	A scene graph leaf drawn from shareable model data: it holds the model
	data, the property and effect states gathered for it, and world-space
	copies of the model vertices and normals made on demand.

	Evidence: the vtable at 0x899164 (46 slots: NiAVObject's 39, then 7
	new); the constructors (0x7EDCC0, 0x7EDD10) call NiAVObject's and
	clear the property state (0xC8), the effect state (0xCC), the world
	vertices (0xD4) and normals (0xD8) and their two update flags (0xDC,
	0xDD), the first taking the model data (0xD0); the destructor
	(0x7EDD50) releases the model data, frees both world copies, then
	destroys the three smart pointers; the NiRTTI (0x9A1A98) is named
	"NiGeometry" with NiAVObject's as its base; LoadBinary (0x7EE6C0) reads
	the model data's link ID into the load record at 0x28. Slots 39-42
	return the model data's arrays, 43 replaces the model data, 44 returns
	it. The member and method names follow the Gamebryo lineage
	(m_spModelData, m_spPropertyState, m_spEffectState, SetModelData and
	the like), which keeps this layout; the two update flags have invented
	names.

===============================================================================
*/

#ifndef __NIGEOMETRY_H__
#define __NIGEOMETRY_H__

#include "NiAVObject.h"
#include "NiGeometryData.h"
#include "NiStreamMacros.h"

/*
================
NiGeometryLoadRecord

The link ID of the model data, read by LoadBinary and resolved by
LinkObject. The class name is invented.
================
*/
class NiGeometryLoadRecord : public NiAVObjectLoadRecord {
public:
							NiGeometryLoadRecord() { modelDataLinkID = 0; }

	unsigned int			modelDataLinkID;
};

// FUNCTION: STBC 0x007EEAB0 ??_GNiGeometry@@UAEPAXI@Z
class NiGeometry : public NiAVObject {
public:
	// FUNCTION: STBC 0x007EEAA0 ?GetRTTI@NiGeometry@@UBEPBVNiRTTI@@XZ
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
	virtual void			GetViewerStrings( NiTArray<char *> *strings );

	virtual					~NiGeometry();

	virtual void			ApplyTransform( const NiMatrix3 &rotate, const NiPoint3 &translate, bool onLeft );
	virtual void			CreateWorldVertices();
	virtual void			UpdateWorldVertices();
	virtual void			DestroyWorldVertices();
	virtual void			CreateWorldNormals();
	virtual void			UpdateWorldNormals();
	virtual void			DestroyWorldNormals();
	virtual void			UpdatePropertiesDownward( NiPropertyState *parentState );
	virtual void			UpdateEffectsDownward( NiDynamicEffectState *parentState );
	virtual NiPointer<NiPropertyState>	GetPropertyState();
	virtual NiPointer<NiDynamicEffectState>	GetEffectState();
	// TODO identity: slot 35 takes the object whose 0x188 holds the
	// renderer (a camera?); this override hands the renderer the property
	// and effect states
	virtual void			VirtualSlot35( NiCamera *camera );
	virtual void			UpdateWorldData();
	virtual void			UpdateWorldBound();

	// the model data's arrays, virtual (DamageTool.exe's symbols name them
	// GetPacked*)
	// FUNCTION: STBC 0x007ee8d0 ?GetPackedVertices@NiGeometry@@UBEPAVNiPoint3@@XZ
	virtual NiPoint3 *		GetPackedVertices() const { return modelData->GetVertices(); }
	// FUNCTION: STBC 0x007ee8e0 ?GetPackedNormals@NiGeometry@@UBEPAVNiPoint3@@XZ
	virtual NiPoint3 *		GetPackedNormals() const { return modelData->GetNormals(); }
	// FUNCTION: STBC 0x007ee900 ?GetPackedColors@NiGeometry@@UBEPAVNiColorA@@XZ
	virtual NiColorA *		GetPackedColors() const { return modelData->GetColors(); }
	// FUNCTION: STBC 0x007ee8f0 ?GetPackedTextures@NiGeometry@@UBEPAVNiPoint2@@XZ
	virtual NiPoint2 *		GetPackedTextures() const { return modelData->GetTextures(); }

	// the model data: a new one of another vertex count rebuilds the world
	// copies
	virtual void			SetModelData( NiGeometryData *data );
	virtual NiGeometryData *	GetModelData() const;
	// the model data member as a pointer, called directly (name invented;
	// AnimOnly.exe 0x498040, from NiMorphController::Update and
	// NiMorpherController::VirtualSlot15)
	NiGeometryData *		GetModelDataPointer() const { return modelData; }
	// the model data's bound (Gamebryo lineage names; AnimOnly.exe 0x452290)
	void					SetModelBound( const NiBound &bound );
	NiBound &				GetModelBound() const;

	virtual void			CalculateNormals();

	// the model data's vertex count and arrays (Gamebryo lineage names;
	// AnimOnly.exe 0x432490, 0x4324B0, 0x4358D0)
	unsigned short			GetVertexCount() const;
	NiPoint3 *				GetVertices() const;
	NiPoint3 *				GetNormals() const;
	// (Gamebryo lineage names; AnimOnly.exe 0x4358F0, 0x435910, 0x435950)
	NiColorA *				GetColors() const;
	unsigned short			GetTextureSets() const;
	NiPoint2 *				GetTextures() const;
	// the world-space copies of the vertices: NULL until CreateWorldVertices,
	// current after UpdateWorldVertices (Gamebryo lineage name)
	NiPoint3 *				GetWorldVertices() const { return worldVertices; }
	NiPoint3 *				GetWorldNormals() const { return worldNormals; }		// ModelMerge_Merge (0x46DF8D) reads it beside the vertices

							NiGeometry( NiGeometryData *data );
							NiGeometry();

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

	// what the Display functions have drawn (names invented from their use)
	static unsigned int		shapesDrawn;
	static unsigned int		trianglesDrawn;
	static unsigned int		verticesDrawn;
	// Clears the counts, and reads them (invented names; UtopiaModule's
	// frame calls them around the rendered set's draw).
	static void				ResetDrawCounts();
	static void				GetDrawCounts( unsigned int *shapes, unsigned int *triangles, unsigned int *vertices );

protected:
	// TODO identity: BaseObjectClass::MarkGeometryChanged (0x431910) reads
	// the model data member directly, not through the virtual GetModelData
	friend class BaseObjectClass;
	// TODO identity: so does NiTriShapeSkinController (0x80CB00, 0x80CBC0,
	// 0x80CC60)
	friend class NiTriShapeSkinController;
	// TODO identity: and BridgeObjectClass's gamma functions (0x662640,
	// 0x663A40)
	friend class BridgeObjectClass;
	// TODO identity: and NiMorpherController (0x7AB0E0, 0x7AB190)
	friend class NiMorpherController;
	// TODO identity: and NiParticleSystemController (0x7AF450, 0x7AF9C0)
	friend class NiParticleSystemController;
	// TODO identity: and TGIconPoly (0x740A30, 0x740AE0, 0x740C30), which edits a UI batch's quads in place
	friend class TGIconPoly;
	// TODO identity: and the game's ClearRevisionIDs (0x45D2A0)
	friend void ClearRevisionIDs( NiAVObject *object );
	// TODO identity: and AsteroidFieldPlacement's ring builder (0x493DD0)
	friend class AsteroidFieldPlacement;
	// TODO identity: and DamageCollisionTest's vertex test (0x588280)
	friend class DamageCollisionTest;
	// TODO identity: and ObjectClass's vertex search (0x435C50)
	friend bool FindVertex( int *index, NiAVObject *object, NiGeometry **geometry, NiPoint3 **vertex, NiPoint3 **normal );
	// TODO identity: and PlacementObject::SetColors (0x4A2190), which recolours its model's data in place
	friend class PlacementObject;

	void					CopyMembers( NiGeometry *dest );

	NiPointer<NiPropertyState>	propertyState;
	NiPointer<NiDynamicEffectState>	effectState;
	NiPointer<NiGeometryData>	modelData;
	NiPoint3 *				worldVertices;			// heap arrays of the model's vertex count, or NULL
	NiPoint3 *				worldNormals;
	bool					worldVerticesChanged;	// the world transform moved since the last update
	bool					worldNormalsChanged;
};

NiRegisterStream( NiGeometry );

/*
================
NiGeometry::GetVertexCount
================
*/
inline unsigned short NiGeometry::GetVertexCount() const {
	return modelData->GetVertexCount();
}

/*
================
NiGeometry::GetVertices
================
*/
inline NiPoint3 *NiGeometry::GetVertices() const {
	return modelData->GetVertices();
}

/*
================
NiGeometry::GetNormals
================
*/
inline NiPoint3 *NiGeometry::GetNormals() const {
	return modelData->GetNormals();
}

/*
================
NiGeometry::GetColors
================
*/
inline NiColorA *NiGeometry::GetColors() const {
	return modelData->GetColors();
}

/*
================
NiGeometry::GetTextureSets
================
*/
inline unsigned short NiGeometry::GetTextureSets() const {
	return modelData->GetTextureSets();
}

/*
================
NiGeometry::GetTextures
================
*/
inline NiPoint2 *NiGeometry::GetTextures() const {
	return modelData->GetTextures();
}

/*
================
NiGeometry::SetModelBound
================
*/
inline void NiGeometry::SetModelBound( const NiBound &bound ) {
	modelData->SetBound( bound );
}

/*
================
NiGeometry::GetModelBound
================
*/
inline NiBound &NiGeometry::GetModelBound() const {
	return modelData->GetBound();
}

#endif /* !__NIGEOMETRY_H__ */
