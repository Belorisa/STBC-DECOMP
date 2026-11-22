/*
===============================================================================

	NiLightColorController.h

	Animates the colour of its target, an NiLight, by the keys of an
	NiPosData (the colour channels as x, y and z): either its ambient
	colour, or its diffuse and specular colours.

	Evidence: the vtable at 0x898558 (NiTimeController's 20 slots, none
	added); the constructor (0x7A6690) takes the data into the smart
	pointer at 0x34, clears bit 4 of the flags and the key index at 0x30;
	Update (0x7A6880) clamps the colour to [0, 1] and, by bit 4, sets the
	light's ambient colour (the others black) or its diffuse and specular
	colours (the ambient black). The NiRTTI is 0x9A0478, named
	"NiLightColorController".

===============================================================================
*/

#ifndef __NILIGHTCOLORCONTROLLER_H__
#define __NILIGHTCOLORCONTROLLER_H__

#include "NiTimeController.h"
#include "NiPosData.h"
#include "NiLight.h"
#include "NiStreamMacros.h"

/*
================
NiLightColorControllerLoadRecord

The record CreateObject hands the stream: NiTimeController's link IDs and
the data's, under a vtable of its own (0x8985A8). The class name is
invented.
================
*/
// FUNCTION: STBC 0x007a7070 ??_GNiLightColorControllerLoadRecord@@UAEPAXI@Z @netimmerse/NiLightColorController
class NiLightColorControllerLoadRecord : public NiTimeControllerLoadRecord {
public:
							NiLightColorControllerLoadRecord() { dataLinkID = 0; }

	unsigned int			dataLinkID;
};

// FUNCTION: STBC 0x007a7050 ??_GNiLightColorController@@UAEPAXI@Z @netimmerse/NiLightColorController
class NiLightColorController : public NiTimeController {
public:
	// FUNCTION: STBC 0x7A7040 ?GetRTTI@NiLightColorController@@UBEPBVNiRTTI@@XZ
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

	virtual					~NiLightColorController();

	virtual void			Update( float time );
	// FUNCTION: STBC 0x7A70C0 ?TargetIsRequiredType@NiLightColorController@@UBE_NXZ
	virtual bool			TargetIsRequiredType() const {
								return NiIsKindOf( NiLight, target );
							}

							NiLightColorController( NiPosData *data_ = 0 );

	// the colour keys' owner (Gamebryo lineage names; AnimOnly.exe 0x494640)
	void					SetPosData( NiPosData *data_ ) { data = data_; }
	NiPosData *				GetPosData() { return data; }

	// whether the ambient colour is animated, in NiTimeController's flags
	enum {
		AMBIENT_MASK		= 0x0010,
		AMBIENT_POS			= 4
	};

	// Gamebryo lineage names
	void					SetAmbient( bool ambient ) { if ( ambient ) { flags |= AMBIENT_MASK; } else { flags &= ~AMBIENT_MASK; } }
	bool					GetAmbient() const { return ( flags >> AMBIENT_POS ) & 1; }

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

	// the first NiLightColorController of the target's list (name
	// invented)
	static NiLightColorController *	FindController( NiObjectNET *target );

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

protected:
	void					CopyMembers( NiLightColorController *dest );

	unsigned int			lastIndex;	// the key the last update found
	NiPointer<NiPosData>	data;
};

NiRegisterStream( NiLightColorController );

#endif /* !__NILIGHTCOLORCONTROLLER_H__ */
