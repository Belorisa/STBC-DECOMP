/*
===============================================================================

	NiParticlesData.h

	Vertex data drawn as particles: one billboard per active vertex.

	Evidence: the NiRTTI (0x9A1C80, initialised at 0x7F4560) is named
	"NiParticlesData" with NiTriBasedGeomData's as its base; CreateObject
	(0x7F4830) allocates 0x44 bytes, runs NiTriBasedGeomData's constructor,
	stores the vtable 0x89958C (18 slots; the load record's follows), sets
	the float at 0x38 to 1 and clears the word at 0x3C and the pointer at
	0x40. Only slot 14 is overridden past the streaming slots. The
	renderer's slot 49 reads the word at 0x3C and the float at 0x38. Member
	names follow the Gamebryo lineage (m_fRadius, m_usActiveVertices,
	m_pfSizes): the constructor (0x7F46F0) fills a float per vertex with 1,
	and the viewer strings name the members ("m_fRadius", "m_usActive",
	"m_pSizes").

===============================================================================
*/

#ifndef __NIPARTICLESDATA_H__
#define __NIPARTICLESDATA_H__

#include "NiTriBasedGeomData.h"
#include "NiStream.h"
#include "NiStreamMacros.h"

/*
================
NiParticlesDataLoadRecord

The record CreateObject hands the stream: NiObject's link ID under a
vtable of its own (0x8995D4). The class name is invented.
================
*/
// FUNCTION: STBC 0x007F48D0 ??_GNiParticlesDataLoadRecord@@UAEPAXI@Z
class NiParticlesDataLoadRecord : public NiObjectLoadRecord {
};

class NiParticlesData : public NiTriBasedGeomData {
public:
	// FUNCTION: STBC 0x7F4D70 ?GetRTTI@NiParticlesData@@UBEPBVNiRTTI@@XZ
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

	virtual					~NiParticlesData();

	virtual void			CalculateNormals();

	float					GetRadius() const { return radius; }
	void					SetRadius( float radius_ ) { radius = radius_; }	// name invented
	unsigned short			GetActiveParticleCount() const { return activeVertexCount; }	// name invented: the base's GetActiveVertexCount is not overridden
	float *					GetSizes() const { return sizes; }
	// shows the first count particles, at most all of them (name invented;
	// SpaceCamera, 0x42EE30)
	void					SetActiveParticleCount( unsigned short count ) {
								if ( count <= vertexCount ) {
									activeVertexCount = count;
								} else {
									activeVertexCount = vertexCount;
								}
							}

	// one particle to each vertex, all active and of size 1
							NiParticlesData( unsigned short vertexCount_, NiPoint3 *vertices_, NiPoint3 *normals_, NiColorA *colors_ );
							NiParticlesData();

	// replaces the sizes, freeing the old ones
	void					Replace( float *sizes_ );
	// takes the sizes without freeing the old ones (name invented)
	void					SetData( float *sizes_ );

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

protected:
	// NiParticleSystemController (0x7AF450, 0x7AFE10) sets the active
	// count and reads the sizes directly
	friend class NiParticleSystemController;

	void					CopyMembers( NiParticlesData *dest );

	float					radius;					// 1 when created
	unsigned short			activeVertexCount;		// the particles drawn
	float *					sizes;					// per vertex, owned
};

NiRegisterStream( NiParticlesData );

#endif /* !__NIPARTICLESDATA_H__ */
