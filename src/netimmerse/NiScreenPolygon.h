/*
===============================================================================

	NiScreenPolygon.h

	A polygon drawn in screen space over a camera's view: its vertices with
	optional texture coordinates and colours, and the property state it is
	drawn with.

	Evidence: the static initializer at 0x7E68B0 registers "NiScreenPolygon"
	with NiObject's NiRTTI (0x9A18D0); the vtable at 0x898FF4 has NiObject's
	12 slots and adds none; CreateClone (0x7E6D60) allocates 0x1C bytes,
	passes the count at 0x0C and the arrays at 0x10, 0x14 and 0x18 to the
	constructor (0x7E6A40) and copies the property state at 0x08; IsEqual
	(0x7E7430) compares the count as an unsigned short, the vertices as
	12-byte points, the texture coordinates as 8-byte points and then the
	colours. The viewer strings name "m_usVertices", "m_pVertex",
	"m_pTexture" and "m_pColor". NiCamera keeps its screen polygons in an
	array and draws them after the scene.

	CreateObject (0x7E6ED0) hands the stream a load record of its own
	(vtable 0x899024) with an array of link IDs; LinkObject (0x7E7260) puts
	each linked property into the property state by its type. The two
	draw functions (0x7E6C60, 0x7E6CF0) hand the renderer the property
	state and the arrays (NiRender's slot 51).

===============================================================================
*/

#ifndef __NISCREENPOLYGON_H__
#define __NISCREENPOLYGON_H__

#include "NiObject.h"
#include "NiPointer.h"
#include "NiPropertyState.h"
#include "NiStream.h"
#include "NiTArray.h"
#include "NiStreamMacros.h"

class NiColorA;
class NiRender;
class NiPoint2;
class NiPoint3;

/*
================
NiScreenPolygonLoadRecord

The link IDs of the properties, read by LoadBinary (0x7E7000) and resolved
by LinkObject (0x7E7260). The class name is invented.
================
*/
// FUNCTION: STBC 0x7E6FC0 ??_GNiScreenPolygonLoadRecord@@UAEPAXI@Z
// FUNCTION: STBC 0x7E6FE0 ??1NiScreenPolygonLoadRecord@@UAE@XZ
class NiScreenPolygonLoadRecord : public NiObjectLoadRecord {
public:
	NiTArray<unsigned int>	propertyLinkIDs;
};

// FUNCTION: STBC 0x007e7930 ??_GNiScreenPolygon@@UAEPAXI@Z @netimmerse/NiScreenPolygon
class NiScreenPolygon : public NiObject {
public:
	// FUNCTION: STBC 0x7E7920 ?GetRTTI@NiScreenPolygon@@UBEPBVNiRTTI@@XZ
	virtual const NiRTTI *	GetRTTI() const { return &rtti; }

	// cloning
	virtual NiObject *		CreateClone();
	void					CopyMembers( NiScreenPolygon *dest );

	// streaming
	virtual void			LoadBinary( NiStream &stream );
	virtual void			LinkObject( NiStream &stream );
	virtual bool			RegisterStreamables( NiStream &stream );
	virtual void			SaveBinary( NiStream &stream );
	virtual bool			IsEqual( NiObject *object );
	virtual void			GetViewerStrings( NiTArray<char *> *strings );

	virtual					~NiScreenPolygon();

							NiScreenPolygon( unsigned short vertices_, const NiPoint3 *vertex_, const NiPoint2 *texture_, const NiColorA *color_ );

	// the polygon's own rendering properties (name invented)
	NiPropertyState *		GetPropertyState() const { return propertyState; }
	// puts a property into the state by its type (Gamebryo lineage name;
	// AnimOnly.exe 0x4255C0)
	void					SetProperty( NiProperty *property ) { propertyState->SetProperty( property ); }

	// hand the renderer the property state and the polygon: Draw on its
	// own, DrawBatched among NiCamera's polygons (names invented)
	void					Draw( NiRender *renderer );
	void					DrawBatched( NiRender *renderer );

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

protected:
							NiScreenPolygon() { vertices = 0; vertex = 0; texture = 0; color = 0; }

	NiPointer<NiPropertyState>	propertyState;
	unsigned short			vertices;		// the number of vertices
	NiPoint3 *				vertex;			// the screen positions
	NiPoint2 *				texture;		// the texture coordinates, or NULL
	NiColorA *				color;			// the colours, or NULL
};

NiRegisterStream( NiScreenPolygon );

#endif /* !__NISCREENPOLYGON_H__ */
