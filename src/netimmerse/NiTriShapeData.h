/*
===============================================================================

	NiTriShapeData.h

	Triangle list geometry data: three vertex indices per triangle, and for
	each vertex the other vertices that share its normal.

	Evidence: the vtable at 0x899328 (18 slots; slot 14 is CalculateNormals,
	0x821440); the constructor (0x7F1400) passes six arguments on to
	NiTriBasedGeomData's, sets the list length at 0x38 to three times the
	triangle count and takes the list at 0x3C; the destructor (0x7F1480)
	frees the list and the shared normal array at 0x40 (8-byte elements,
	count at 0x44); the NiRTTI (0x9A1B40) is named "NiTriShapeData" with
	NiTriBasedGeomData's as its base; the viewer strings name the members
	("m_uiTriListLength", "m_pTriList"). LoadBinary reads the shared normals
	only from files newer than "3.05". The names follow the Gamebryo lineage
	(m_uiTriListLength, m_pusTriList, SharedNormalArray, the shared normal
	array and its size).

===============================================================================
*/

#ifndef __NITRISHAPEDATA_H__
#define __NITRISHAPEDATA_H__

#include "NiStream.h"
#include "NiTriBasedGeomData.h"
#include "NiStreamMacros.h"

/*
================
NiTriShapeDataLoadRecord

The record CreateObject hands the stream: NiObject's link ID under a
vtable of its own (0x899370). The class name is invented.

Evidence: CreateObject (0x7F1860) clears the link ID before it stores the
vptr, the order a derived class's inline constructor leaves.
================
*/
// FUNCTION: STBC 0x007F1900 ??_GNiTriShapeDataLoadRecord@@UAEPAXI@Z
class NiTriShapeDataLoadRecord : public NiObjectLoadRecord {
};

// FUNCTION: STBC 0x007F1E60 ??_GNiTriShapeData@@UAEPAXI@Z
class NiTriShapeData : public NiTriBasedGeomData {
public:
	// the vertices that share one vertex's normal
	// FUNCTION: STBC 0x7F1500 ??_ESharedNormalArray@NiTriShapeData@@QAEPAXI@Z
	class SharedNormalArray {
	public:
							SharedNormalArray();
							// FUNCTION: STBC 0x7F1EA0 ??1SharedNormalArray@NiTriShapeData@@QAE@XZ
							~SharedNormalArray() { delete [] indices; }

		unsigned short		GetCount() const;
		unsigned short *	GetArray() const;

		// takes the array; no indices or no array clears both
		void				SetArray( unsigned short count_, unsigned short *indices_ );

	protected:
		unsigned short		count;
		unsigned short *	indices;	// heap array of count vertex indices, owned
	};

	// FUNCTION: STBC 0x007F1E50 ?GetRTTI@NiTriShapeData@@UBEPBVNiRTTI@@XZ
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

	virtual					~NiTriShapeData();

	virtual void			CalculateNormals();

							NiTriShapeData( unsigned short vertexCount_, NiPoint3 *vertices_, NiPoint3 *normals_, NiColorA *colors_, NiPoint2 *textures_, unsigned short triangleCount_, unsigned short *triList_ );
							NiTriShapeData();

	unsigned int			GetTriListLength() const;
	unsigned short *		GetTriList() const;

	// replaces the triangle list of the current triangle count, freeing the
	// old one
	void					Replace( unsigned short *triList_ );
	// takes the list without freeing the old one (name invented)
	void					SetData( unsigned short *triList_ );
	// makes the two vertices share their normal, with every vertex either
	// already shares it with (Gamebryo lineage name; nothing in retail calls it)
	void					LetVerticesShareNormal( unsigned short index0, unsigned short index1 );

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

protected:
	void					CopyMembers( NiTriShapeData *dest );

	unsigned int			triListLength;		// three indices per triangle
	unsigned short *		triList;			// heap array of triListLength vertex indices, owned
	SharedNormalArray *		sharedNormals;		// heap array of sharedNormalsArraySize, one per vertex, owned
	unsigned short			sharedNormalsArraySize;
};

NiRegisterStream( NiTriShapeData );

/*
================
NiTriShapeData::GetTriList
================
*/
inline unsigned short *NiTriShapeData::GetTriList() const {
	return triList;
}

/*
================
NiTriShapeData::GetTriListLength
================
*/
inline unsigned int NiTriShapeData::GetTriListLength() const {
	return triListLength;
}

/*
================
NiTriShapeData::SharedNormalArray::SharedNormalArray
================
*/
// FUNCTION: STBC 0x7F1E90 ??0SharedNormalArray@NiTriShapeData@@QAE@XZ
inline NiTriShapeData::SharedNormalArray::SharedNormalArray() {
	count = 0;
	indices = 0;
}

/*
================
NiTriShapeData::SharedNormalArray::GetCount
================
*/
inline unsigned short NiTriShapeData::SharedNormalArray::GetCount() const {
	return count;
}

/*
================
NiTriShapeData::SharedNormalArray::GetArray
================
*/
inline unsigned short *NiTriShapeData::SharedNormalArray::GetArray() const {
	return indices;
}

/*
================
NiTriShapeData::SharedNormalArray::SetArray
================
*/
inline void NiTriShapeData::SharedNormalArray::SetArray( unsigned short count_, unsigned short *indices_ ) {
	if ( !count_ || !indices_ ) {
		count = 0;
		indices = 0;
	} else {
		count = count_;
		indices = indices_;
	}
}

#endif /* !__NITRISHAPEDATA_H__ */
