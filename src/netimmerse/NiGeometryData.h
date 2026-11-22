/*
===============================================================================

	NiGeometryData.h

	The shareable vertex data of a geometry object: the positions, normals,
	colours and texture coordinates, their model bound, and an ID the
	renderer keys its cached copies by.

	Evidence: the vtable at 0x899124 (16 slots, slot 14 _purecall; NiGeometry's
	follows at 0x899164); the
	constructor (0x7ECB40) takes the vertex count and the four arrays,
	computes the bound from the positions, and takes the ID at 0x30 from
	the static counter at 0x978738; the NiRTTI (0x9A1A60) is named
	"NiGeometryData" with NiObject's as its base. The destructor (0x7ECC80)
	first tells every renderer (0x7E2F20, NiRender's slot 54) that the data
	is going, then frees the four arrays. The member and method names follow
	the Gamebryo lineage (m_usVertices, m_kBound, m_pkVertex, m_pkNormal,
	m_pkColor, m_pkTexture, CreateVertices and the like), which keeps this
	layout.

===============================================================================
*/

#ifndef __NIGEOMETRYDATA_H__
#define __NIGEOMETRYDATA_H__

#include "NiBound.h"
#include "NiColor.h"
#include "NiObject.h"
#include "NiPoint2.h"
#include "NiPoint3.h"
#include "NiStreamMacros.h"

// FUNCTION: STBC 0x007eda40 ??_GNiGeometryData@@UAEPAXI@Z @netimmerse/NiGeometryData
class NiGeometryData : public NiObject {
public:
	// FUNCTION: STBC 0x007EDA30 ?GetRTTI@NiGeometryData@@UBEPBVNiRTTI@@XZ
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

	virtual					~NiGeometryData();

	// the vertices in use (Gamebryo lineage names; no class here keeps a
	// separate active count)
	// FUNCTION: STBC 0x007EDA70 ?SetActiveVertexCount@NiGeometryData@@UAEXG@Z
	virtual void			SetActiveVertexCount( unsigned short count ) {}
	// FUNCTION: STBC 0x007EDA80 ?GetActiveVertexCount@NiGeometryData@@UBEGXZ
	virtual unsigned short	GetActiveVertexCount() const { return vertexCount; }
	virtual void			CalculateNormals() = 0;	// Gamebryo lineage name: NiTriShapeData's (0x821440) builds the normals from the triangles
	// FUNCTION: STBC 0x4FB450 ?VirtualSlot15@NiGeometryData@@UAEXXZ
	virtual void			VirtualSlot15() {}

							NiGeometryData( unsigned short vertexCount_, NiPoint3 *vertices_, NiPoint3 *normals_, NiColorA *colors_, NiPoint2 *textures_ );
							NiGeometryData();

	unsigned short			GetVertexCount() const;
	unsigned short			GetTextureSets() const;
	void					SetTextureSets( unsigned short sets ) { textureSets = sets; }
	unsigned short			GetID() const;
	unsigned short			GetRevisionID() const;
	void					SetRevisionID( unsigned short revisionID_ ) { revisionID = revisionID_; }	// Gamebryo lineage name
	// whether the data never changes: revision 0 (name invented, after
	// Gamebryo's STATIC consistency; AnimOnly.exe 0x513FC0, which
	// NiDirect3D.cpp's vertex buffer cache calls)
	bool					IsStatic() const { return revisionID == 0; }
	// marks the data changed: the next revision, skipping 0, which means
	// the data never changes (name invented, after the Gamebryo lineage's
	// revision ID)
	// FUNCTION: STBC 0x004d9050 ?IncRevisionID@NiGeometryData@@QAEXXZ @game/AnimatedTriShapeParticles
	void					IncRevisionID() { if ( ++revisionID == 0 ) { revisionID = 1; } }
	void					SetBound( const NiBound &bound_ );	// Gamebryo lineage name (AnimOnly.exe 0x4522C0)
	const NiBound &			GetBound() const;
	NiBound &				GetBound();
	NiPoint3 *				GetVertices() const;
	NiPoint3 *				GetNormals() const;
	NiColorA *				GetColors() const;
	NiPoint2 *				GetTextures() const;
	NiPoint2 *				GetTextureSet( unsigned short set );
	const NiPoint2 *		GetTextureSet( unsigned short set ) const;
	void					AppendTextureSet( NiPoint2 *set );	// name invented

	// allocates the array when there is none; initialize clears it
	void					CreateVertices( bool initialize );
	void					CreateNormals( bool initialize );
	void					CreateColors( bool initialize );
	void					CreateTextures( bool initialize, unsigned short sets );

	// replaces the arrays, freeing the old ones
	void					Replace( unsigned short vertexCount_, NiPoint3 *vertices_, NiPoint3 *normals_, NiColorA *colors_, NiPoint2 *textures_ );
	// takes the arrays without freeing the old ones (name invented)
	void					SetData( unsigned short vertexCount_, NiPoint3 *vertices_, NiPoint3 *normals_, NiColorA *colors_, NiPoint2 *textures_ );

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

protected:
	// NiParticleSystemController (0x7AF450, 0x7AF9C0) reads the counts,
	// the bound and the arrays directly
	friend class NiParticleSystemController;
	// TODO identity: the DisruptorGraphic constructor (0x4DD250) sets the
	// bound's flag directly after building the shape
	friend class DisruptorGraphic;

	void					CopyMembers( NiGeometryData *dest );

	unsigned short			vertexCount;
	unsigned short			textureSets;	// texture coordinate sets: vertexCount coordinates each
	NiBound					bound;			// model bound of the positions
	NiPoint3 *				vertices;		// heap arrays of vertexCount elements, owned
	NiPoint3 *				normals;
	NiColorA *				colors;
	NiPoint2 *				textures;		// the sets one after another
	unsigned short			id;				// unique, from nextID
	unsigned short			revisionID;		// 0 (after construction): the data never changes; the renderer optimises only such data (Gamebryo lineage name)

	static unsigned short	nextID;
};

NiRegisterStream( NiGeometryData );

/*
================
NiGeometryData::GetVertexCount
================
*/
inline unsigned short NiGeometryData::GetVertexCount() const {
	return vertexCount;
}

/*
================
NiGeometryData::GetVertices
================
*/
inline NiPoint3 *NiGeometryData::GetVertices() const {
	return vertices;
}

/*
================
NiGeometryData::GetNormals
================
*/
inline NiPoint3 *NiGeometryData::GetNormals() const {
	return normals;
}

/*
================
NiGeometryData::SetBound
================
*/
inline void NiGeometryData::SetBound( const NiBound &bound_ ) {
	bound = bound_;
}

/*
================
NiGeometryData::GetBound
================
*/
inline NiBound &NiGeometryData::GetBound() {
	return bound;
}

/*
================
NiGeometryData::GetBound
================
*/
inline const NiBound &NiGeometryData::GetBound() const {
	return bound;
}

/*
================
NiGeometryData::GetColors
================
*/
inline NiColorA *NiGeometryData::GetColors() const {
	return colors;
}

/*
================
NiGeometryData::GetTextureSets
================
*/
inline unsigned short NiGeometryData::GetTextureSets() const {
	return textureSets;
}

/*
================
NiGeometryData::GetTextures
================
*/
inline NiPoint2 *NiGeometryData::GetTextures() const {
	return textures;
}

/*
================
NiGeometryData::GetRevisionID
================
*/
inline unsigned short NiGeometryData::GetRevisionID() const {
	return revisionID;
}

/*
================
NiGeometryData::GetID
================
*/
inline unsigned short NiGeometryData::GetID() const {
	return id;
}

#endif /* !__NIGEOMETRYDATA_H__ */
