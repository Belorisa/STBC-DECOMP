/*
===============================================================================

	NiLines.h

	A line geometry object: an NiGeometry drawn from NiLinesData.

	Evidence: the NiRTTI (0x9A2130) is named "NiLines" with NiGeometry's as
	its base; the vtable at 0x899D84 has NiGeometry's 46 slots and adds none
	(the load record's follows at 0x899E3C); the constructors (0x7FEAE0,
	0x7FEB70, 0x7FEB90) call NiGeometry's and add no members, the first
	creating the data from five arguments; CreateClone (0x7FEC10) allocates
	0xE0 bytes. Slot 31 (0x7FEBB0) hands the renderer at the argument's
	0x188 the lines through its slot 50, the slot NiLinesData's flags are
	read by.

===============================================================================
*/

#ifndef __NILINES_H__
#define __NILINES_H__

#include "NiGeometry.h"
#include "NiLinesData.h"
#include "NiStreamMacros.h"

/*
================
NiLinesLoadRecord

The record CreateObject hands the stream. The class name is invented.

Evidence: CreateObject (0x7FEC90) allocates 0x2C bytes and stores the vptr
0x899E3C after the link IDs, the property link ID array at 0x10 and the
model data's link ID at 0x28.
================
*/
// FUNCTION: STBC 0x7FED70 ??_GNiLinesLoadRecord@@UAEPAXI@Z
// FUNCTION: STBC 0x7FED90 ??1NiLinesLoadRecord@@UAE@XZ
class NiLinesLoadRecord : public NiGeometryLoadRecord {
};

// FUNCTION: STBC 0x007FF0A0 ??_GNiLines@@UAEPAXI@Z
class NiLines : public NiGeometry {
public:
	// FUNCTION: STBC 0x7FF090 ?GetRTTI@NiLines@@UBEPBVNiRTTI@@XZ
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

	// TODO identity: slot 31 draws the lines through the renderer at the
	// argument's 0x188
	virtual void			Display( NiCamera *camera );

							NiLines( unsigned short vertexCount_, NiPoint3 *vertices_, NiColorA *colors_, NiPoint2 *textures_, bool *flags_ );
							NiLines( NiLinesData *data );
							NiLines();

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

	NiLinesData *			GetLinesData() const { return (NiLinesData *)(NiGeometryData *)modelData; }

protected:
	void					CopyMembers( NiLines *dest );
};

NiRegisterStream( NiLines );

#endif /* !__NILINES_H__ */
