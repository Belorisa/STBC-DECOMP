/*
===============================================================================

	NiMaterialColorController.h

	Animates one colour of its target, an NiMaterialProperty, by the keys
	of an NiPosData (the colour channels as x, y and z).

	Evidence: the vtable at 0x8986E4 (NiTimeController's 20 slots, none
	added); the constructor (0x7AC1C0) takes the data into the smart
	pointer at 0x34, clears the field in bits 4-6 of the flags and the key
	index at 0x30; Update (0x7AC3B0) clamps the colour to [0, 1] and stores
	it at the material's ambient, diffuse, specular or emissive colour by
	the field. The NiRTTI is 0x9A06D0, named "NiMaterialColorController".

===============================================================================
*/

#ifndef __NIMATERIALCOLORCONTROLLER_H__
#define __NIMATERIALCOLORCONTROLLER_H__

#include "NiTimeController.h"
#include "NiPosData.h"
#include "NiMaterialProperty.h"
#include "NiStreamMacros.h"

/*
================
NiMaterialColorControllerLoadRecord

The record CreateObject hands the stream: NiTimeController's link IDs and
the data's, under a vtable of its own (0x898734). The class name is
invented.
================
*/
// FUNCTION: STBC 0x007acaa0 ??_GNiMaterialColorControllerLoadRecord@@UAEPAXI@Z @netimmerse/NiMaterialColorController
class NiMaterialColorControllerLoadRecord : public NiTimeControllerLoadRecord {
public:
							NiMaterialColorControllerLoadRecord() { dataLinkID = 0; }

	unsigned int			dataLinkID;
};

// FUNCTION: STBC 0x007aca80 ??_GNiMaterialColorController@@UAEPAXI@Z @netimmerse/NiMaterialColorController
class NiMaterialColorController : public NiTimeController {
public:
	// FUNCTION: STBC 0x7ACA70 ?GetRTTI@NiMaterialColorController@@UBEPBVNiRTTI@@XZ
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

	virtual					~NiMaterialColorController();

	virtual void			Update( float time );
	// FUNCTION: STBC 0x7ACAF0 ?TargetIsRequiredType@NiMaterialColorController@@UBE_NXZ
	virtual bool			TargetIsRequiredType() const {
								return NiIsKindOf( NiMaterialProperty, target );
							}

							NiMaterialColorController( NiPosData *data_ = 0 );

	// the colour keys' owner (Gamebryo lineage names; AnimOnly.exe 0x49B620)
	void					SetPosData( NiPosData *data_ ) { data = data_; }
	NiPosData *				GetPosData() { return data; }

	// the material colour the controller animates (Gamebryo lineage names)
	enum Field {
		AMB,
		DIFF,
		SPEC,
		SELF_ILLUM
	};

	// the field's place in NiTimeController's flags
	enum {
		FIELD_MASK			= 0x0070,
		FIELD_POS			= 4
	};

	void					SetField( Field field ) { flags = ( flags & ~FIELD_MASK ) | ( field << FIELD_POS ); }
	Field					GetField() const { return (Field)( ( flags & FIELD_MASK ) >> FIELD_POS ); }

	// the data's keys, their count and type; none without data (Gamebryo
	// lineage name)
	NiPosKey **				GetAnim( unsigned int &numKeys, NiPosKey::KeyType &keyType ) const {
								if ( data ) {
									return data->GetAnim( numKeys, keyType );
								}
								numKeys = 0;
								keyType = NiPosKey::NOINTERP;
								return 0;
							}

	// gives the data new keys, creating the data first; the key times
	// become those of the first and last keys (Gamebryo lineage name; the
	// count comes first, as in NiFloatController::ReplaceAnim)
	void					ReplaceAnim( unsigned int numKeys, NiPosKey **keys, NiPosKey::KeyType keyType );

	// the first NiMaterialColorController of the target's list (name
	// invented)
	static NiMaterialColorController *	FindController( NiObjectNET *target );

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

protected:
	void					CopyMembers( NiMaterialColorController *dest );

	unsigned int			lastIndex;	// the key the last update found
	NiPointer<NiPosData>	data;
};

NiRegisterStream( NiMaterialColorController );

#endif /* !__NIMATERIALCOLORCONTROLLER_H__ */
