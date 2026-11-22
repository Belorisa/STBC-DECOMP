/*
===============================================================================

	SpecularPass.h

	NetImmerse nodes that draw a model a second time. A pass node holds no
	geometry of its own: it watches a model, and for each triangle shape
	in it keeps a shape of its own with the same vertices, which its
	Display fills in and draws on top of the model's.

	Evidence for the names: the NiRTTI at 0x986628 is named "SpecularPass"
	(0x8E1040), with NiNode's as its base. Nothing names the other classes:
	ModelPass, MotionBlurPass and the three shape records are invented
	names. The third pass tests LODModelManager::motionBlurEnabled before
	it does anything, as SpecularPass tests specularMapsEnabled.

	The unit's code is 0x4EC9E0-0x4EFA20:

	- ModelPass (vtable 0x88D8A8: NiNode's 44 slots and two new ones;
	  destructor 0x4ECA30), with NiNode's GetRTTI. Its constructor is
	  inline: both subclasses expand it;
	- SpecularPass (vtable 0x88D968; constructor 0x4ED020), 0x108 bytes;
	- MotionBlurPass (vtable 0x88DA2C; constructor 0x4EE540), 0x100 bytes,
	  with NiNode's GetRTTI;
	- PassShape (vtable 0x88D960, destructor 0x4ECF10), a polymorphic
	  record per watched shape with only a virtual destructor, and its two
	  subclasses (0x88D964, destructor 0x4ECE40; 0x88DA28, destructor
	  0x4EE450), 0x14 bytes each;
	- the map from a watched shape to its record (vtable 0x88D894), whose
	  COMDAT copies follow the unit's code among its header statics
	  (0x4EFDC0-0x4F0040), with NiTList<void *>'s (0x4F0240, 0x4F0280).

	LODModel's loader (0x451190, 0x4511DC and 0x451220, not written yet)
	is the only caller of the two constructors.

===============================================================================
*/

#ifndef __SPECULARPASS_H__
#define __SPECULARPASS_H__

#include "NiNode.h"
#include "NiTriShape.h"
#include "NiTMap.h"
#include "NiDynamicEffectState.h"

/*
===============================================================================

	PassShape

	What a pass keeps for one shape of its model (invented name).

===============================================================================
*/
// FUNCTION: STBC 0x004ecf40 ??_GPassShape@@UAEPAXI@Z
class PassShape {
public:
							PassShape( NiTriShape *shape_ );
	// FUNCTION: STBC 0x004ecf10 ??1PassShape@@UAE@XZ
	virtual					~PassShape() {}

	NiPointer<NiTriShape>	shape;			// 0x04 the model's shape
	unsigned int			revisionID;		// 0x08 the shape data's revision when the pass last copied it
	bool					inModel;		// 0x0C still found in the model at the last search
};

inline PassShape::PassShape( NiTriShape *shape_ ) : shape( shape_ ), revisionID( 0 ), inModel( true ) {
	if ( shape != NULL && shape->GetShapeData() != NULL ) {
		revisionID = shape->GetShapeData()->GetRevisionID();
	}
}

/*
===============================================================================

	SpecularPassShape, MotionBlurPassShape

	A pass's record with the shape the pass draws (invented names). The
	destructor takes that shape out of the pass.

===============================================================================
*/
// FUNCTION: STBC 0x004ecf60 ??_GSpecularPassShape@@UAEPAXI@Z
class SpecularPassShape : public PassShape {
public:
							SpecularPassShape( NiTriShape *shape_ ) : PassShape( shape_ ) {}
	virtual					~SpecularPassShape();

	NiPointer<NiTriShape>	passShape;		// 0x10
};

// FUNCTION: STBC 0x004ee520 ??_GMotionBlurPassShape@@UAEPAXI@Z
class MotionBlurPassShape : public PassShape {
public:
							MotionBlurPassShape( NiTriShape *shape_ ) : PassShape( shape_ ) {}
	virtual					~MotionBlurPassShape();

	NiPointer<NiTriShape>	passShape;		// 0x10
};

typedef NiTMap<NiTriShape *, PassShape *>	PassShapeMap;

/*
===============================================================================

	ModelPass

	The base of the passes (invented name).

===============================================================================
*/
// FUNCTION: STBC 0x004ecbb0 ??_GModelPass@@UAEPAXI@Z
class ModelPass : public NiNode {
public:
							ModelPass( NiAVObject *model_ ) : shapes( 37 ), model( model_ ) {}
	virtual					~ModelPass();

	virtual NiObject *		CreateClone();

	// the record for a shape found in the model, or NULL to leave it out
	// (invented name)
	virtual PassShape *		CreatePassShape( NiTriShape *shape );
	// called when a watched shape's data has changed (invented name)
	virtual void			UpdatePassShape( PassShape *passShape );

protected:
	void					CopyMembers( ModelPass *dest );

	// marks the record of every triangle shape under object, making the
	// records that are missing (invented name)
	void					FindShapes( NiAVObject *object );

	PassShapeMap			shapes;			// 0xE8
	NiPointer<NiAVObject>	model;			// 0xF8
};

/*
===============================================================================

	SpecularPass

===============================================================================
*/
// FUNCTION: STBC 0x004ed350 ??_GSpecularPass@@UAEPAXI@Z
// FUNCTION: STBC 0x004ed370 ??1SpecularPass@@UAE@XZ
class SpecularPass : public ModelPass {
public:
							SpecularPass( NiAVObject *model_, float ks, float power );

	// FUNCTION: STBC 0x004ed340 ?GetRTTI@SpecularPass@@UBEPBVNiRTTI@@XZ
	virtual const NiRTTI *	GetRTTI() const { return &rtti; }

	virtual NiObject *		CreateClone();

	virtual void			UpdateDownwardPass( float time );
	virtual void			UpdateEffectsDownward( NiDynamicEffectState *parentState );
	virtual NiPointer<NiDynamicEffectState>	GetEffectState();
	virtual void			Display( NiCamera *camera );

	virtual PassShape *		CreatePassShape( NiTriShape *shape );
	virtual void			UpdatePassShape( PassShape *passShape );

	// Sets the specular Ks of every SpecularPass in the tree (invented name).
	static void				SetSpecularKs( NiAVObject *object, float ks );

	static const NiRTTI		rtti;

protected:
	void					CopyMembers( SpecularPass *dest );

	// a shape with copies of the shape's vertices, texture coordinates and
	// triangles, vertex colours of its own and the shape's specular
	// texture; NULL when the shape has none (invented name)
	NiTriShape *			CreateShape( NiTriShape *shape );

	// brings the records up to date with the model (invented name)
	void					UpdateShapes();

	NiPointer<NiDynamicEffectState>	effectState;	// 0xFC
	float					specularKs;		// 0x100 (invented name)
	float					specularPower;	// 0x104 (invented name)
};

/*
===============================================================================

	MotionBlurPass

===============================================================================
*/
// FUNCTION: STBC 0x004ee610 ??_GMotionBlurPass@@UAEPAXI@Z
// FUNCTION: STBC 0x004ee630 ??1MotionBlurPass@@UAE@XZ
class MotionBlurPass : public ModelPass {
public:
							MotionBlurPass( NiAVObject *model_ );

	virtual NiObject *		CreateClone();

	virtual void			UpdateDownwardPass( float time_ );
	virtual void			Display( NiCamera *camera );

	virtual PassShape *		CreatePassShape( NiTriShape *shape );
	virtual void			UpdatePassShape( PassShape *passShape );

protected:
	void					CopyMembers( MotionBlurPass *dest );

	NiTriShape *			CreateShape( NiTriShape *shape );
	void					UpdateShapes();

	float					time;			// 0xFC the time of the last update
};

#endif /* !__SPECULARPASS_H__ */
