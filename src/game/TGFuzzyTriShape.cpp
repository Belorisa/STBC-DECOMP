/*
===============================================================================

	TGFuzzyTriShape.cpp

	A triangle shape whose vertices fade with the angle between their
	normal and the camera's view: each vertex's alpha is the cosine of
	that angle times the vertex's fuzziness, raised to the fourth power.

===============================================================================
*/

#include "TGFuzzyTriShape.h"
#include "NiCamera.h"
#include "NiMatrix3.h"
#include "NiNode.h"	// the unit registers NiNode (its stub at 0x457070, among the unit's own)
#include <iostream>

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00456da0 _$E21
// FUNCTION: STBC 0x00456db0 _$E20
// FUNCTION: STBC 0x00456dc0 _$E32
// FUNCTION: STBC 0x00456dd0 _$E31
// FUNCTION: STBC 0x00456de0 _$E36
// FUNCTION: STBC 0x00456df0 _$E35
// FUNCTION: STBC 0x00456e30 _$E40
// FUNCTION: STBC 0x00456e40 _$E39
// FUNCTION: STBC 0x00456e50 _$E44
// FUNCTION: STBC 0x00456e60 _$E43
// FUNCTION: STBC 0x00456e70 _$E168
// FUNCTION: STBC 0x00456e80 _$E167
// FUNCTION: STBC 0x00456ea0 _$E176
// FUNCTION: STBC 0x00456eb0 _$E175
// FUNCTION: STBC 0x00456ec0 _$E183
// FUNCTION: STBC 0x00456ed0 _$E182
// FUNCTION: STBC 0x00456ee0 _$E187
// FUNCTION: STBC 0x00456ef0 _$E186
// FUNCTION: STBC 0x00456f00 _$E191
// FUNCTION: STBC 0x00456f10 _$E190
// FUNCTION: STBC 0x00456f20 _$E195
// FUNCTION: STBC 0x00456f30 _$E194
// FUNCTION: STBC 0x00456f40 _$E201
// FUNCTION: STBC 0x00456f50 _$E200
// FUNCTION: STBC 0x00456fd0 _$E206
// FUNCTION: STBC 0x00456fe0 _$E205
// FUNCTION: STBC 0x00456ff0 _$E110
// FUNCTION: STBC 0x00457000 _$E109
// FUNCTION: STBC 0x00457010 _$E123
// FUNCTION: STBC 0x00457020 _$E122
// FUNCTION: STBC 0x00457050 _$E216
// FUNCTION: STBC 0x00457060 _$E215
// FUNCTION: STBC 0x00457070 _$E220
// FUNCTION: STBC 0x00457080 _$E219
// FUNCTION: STBC 0x00457090 _$E224
// FUNCTION: STBC 0x004570a0 _$E223
// FUNCTION: STBC 0x004570b0 _$E231
// FUNCTION: STBC 0x004570c0 _$E230

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00456cc0 _$E249
// FUNCTION: STBC 0x00456cd0 _$E246
// FUNCTION: STBC 0x00456ce0 _$E248
// FUNCTION: STBC 0x00456cf0 _$E247
// FUNCTION: STBC 0x00456d00 _$E255
// FUNCTION: STBC 0x00456d10 _$E252
// FUNCTION: STBC 0x00456d20 _$E254
// FUNCTION: STBC 0x00456d30 _$E253

// std::ctype<unsigned short>::id (<string>, through <iostream>).
// FUNCTION: STBC 0x00456d70 _$E268
// FUNCTION: STBC 0x00456d90 _$E267

// The run-time type's dynamic initialiser and exit-time destructor.
// FUNCTION: STBC 0x004566e0 _$E261
// FUNCTION: STBC 0x004566f0 _$E257
// FUNCTION: STBC 0x00456710 _$E260
// FUNCTION: STBC 0x00456720 _$E258

// DATA: STBC 0x00980ae8 ?rtti@TGFuzzyTriShape@@2VNiRTTI@@B
const NiRTTI TGFuzzyTriShape::rtti( "TGFuzzyTriShape", &NiTriShape::rtti );

/*
================
TGFuzzyTriShape::Register
================
*/
// FUNCTION: STBC 0x00456740 ?Register@TGFuzzyTriShape@@SAHXZ
NiImplementRegister( TGFuzzyTriShape )

/*
================
TGFuzzyTriShape::TGFuzzyTriShape

A shape over new data made of the arrays.
================
*/
// FUNCTION: STBC 0x00456890 ??0TGFuzzyTriShape@@QAE@GPAVNiPoint3@@0PAVNiColorA@@PAVNiPoint2@@GPAG@Z
TGFuzzyTriShape::TGFuzzyTriShape( unsigned short vertexCount_, NiPoint3 *vertices_, NiPoint3 *normals_, NiColorA *colors_,
								  NiPoint2 *textures_, unsigned short triangleCount_, unsigned short *triList_ ) :
	NiTriShape( vertexCount_, vertices_, normals_, colors_, textures_, triangleCount_, triList_ ),
	fuzziness( NULL ) {
}

/*
================
TGFuzzyTriShape::TGFuzzyTriShape
================
*/
// FUNCTION: STBC 0x004568e0 ??0TGFuzzyTriShape@@QAE@PAVNiTriShapeData@@@Z
TGFuzzyTriShape::TGFuzzyTriShape( NiTriShapeData *data ) : NiTriShape( data ), fuzziness( NULL ) {
}

/*
================
TGFuzzyTriShape::CreateClone
================
*/
// FUNCTION: STBC 0x00456910
NiObject *TGFuzzyTriShape::CreateClone() {
	TGFuzzyTriShape *clone = new TGFuzzyTriShape( NULL );
	CopyMembers( clone );
	return clone;
}

/*
================
TGFuzzyTriShape::CreateObject

Creates the shape and loads it.
================
*/
// FUNCTION: STBC 0x00456980
void TGFuzzyTriShape::CreateObject( NiStream &stream, NiObject *&object ) {
	TGFuzzyTriShape *shape = new TGFuzzyTriShape( NULL );
	stream.SetLoadRecord( new NiTriShapeLoadRecord );
	shape->LoadBinary( stream );
	object = shape;
}

/*
================
TGFuzzyTriShape::CopyMembers

The clone gets no fuzziness: it allocates its own when it is first drawn.
================
*/
// FUNCTION: STBC 0x00456aa0
void TGFuzzyTriShape::CopyMembers( TGFuzzyTriShape *dest ) {
	NiTriShape::CopyMembers( dest );
}

/*
================
TGFuzzyTriShape::LoadBinary
================
*/
// FUNCTION: STBC 0x00456ab0
void TGFuzzyTriShape::LoadBinary( NiStream &stream ) {
	NiTriShape::LoadBinary( stream );
}

/*
================
TGFuzzyTriShape::LinkObject
================
*/
// FUNCTION: STBC 0x00456ac0
void TGFuzzyTriShape::LinkObject( NiStream &stream ) {
	NiTriShape::LinkObject( stream );
}

/*
================
TGFuzzyTriShape::RegisterStreamables
================
*/
// FUNCTION: STBC 0x00456ad0
bool TGFuzzyTriShape::RegisterStreamables( NiStream &stream ) {
	return NiTriShape::RegisterStreamables( stream );
}

/*
================
TGFuzzyTriShape::SaveBinary
================
*/
// FUNCTION: STBC 0x00456ae0
void TGFuzzyTriShape::SaveBinary( NiStream &stream ) {
	NiTriShape::SaveBinary( stream );
}

/*
================
TGFuzzyTriShape::IsEqual
================
*/
// FUNCTION: STBC 0x00456af0
bool TGFuzzyTriShape::IsEqual( NiObject *object ) {
	return NiTriShape::IsEqual( object );
}

/*
================
TGFuzzyTriShape::AllocateFuzziness

Frees the values and allocates one per vertex, each 1.
================
*/
// FUNCTION: STBC 0x00456b00
void TGFuzzyTriShape::AllocateFuzziness() {
	delete [] fuzziness;
	fuzziness = NULL;
	unsigned short count = GetVertexCount();
	if ( count > 0 ) {
		fuzziness = new float[count];
		for ( unsigned short i = 0; i < count; i++ ) {
			fuzziness[i] = 1.0f;
		}
	}
}

/*
================
TGFuzzyTriShape::VirtualSlot35

Sets each vertex's alpha from the angle between its normal and the
camera's view, before the shape is drawn. A shape without normals or
colours is drawn as it is.
================
*/
// FUNCTION: STBC 0x00456b70
// PARKED: x87: retail stores the alpha to its local (fst) before the colour (fstp) and reloads it for the first test; every chain order, a named local and a function-scope alpha keep it on the FPU or copy it through eax
void TGFuzzyTriShape::VirtualSlot35( NiCamera *camera ) {
	NiPoint3 *normals = GetShapeData()->GetNormals();
	NiColorA *colors = GetShapeData()->GetColors();
	if ( normals != NULL && colors != NULL ) {
		if ( fuzziness == NULL ) {
			AllocateFuzziness();
		}
		// the direction towards the camera, in model space
		NiPoint3 toCamera = -camera->GetWorldDirection();
		toCamera = toCamera * GetWorldRotate();
		int count = GetVertexCount();
		for ( int i = 0; i < count; i++ ) {
			float alpha;
			alpha = colors[i].a = ( toCamera * normals[i] ) * fuzziness[i];
			if ( alpha > 0.0f ) {
				if ( alpha < 1.0f ) {
					alpha = alpha * alpha;
					colors[i].a = alpha * alpha;
				} else {
					colors[i].a = 1.0f;
				}
			} else {
				colors[i].a = 0.0f;
			}
		}
		GetShapeData()->IncRevisionID();
	}
	NiTriShape::VirtualSlot35( camera );
}

/*
================
TGFuzzyTriShape::GetRTTI
================
*/
// FUNCTION: STBC 0x00456f70
const NiRTTI *TGFuzzyTriShape::GetRTTI() const {
	return &rtti;
}
