/*
===============================================================================

	NiTextureProperty.h

	The texture image of a surface. How it is applied is the business of
	NiTextureModeProperty.

	Evidence: the vtable at 0x89A288 (NiProperty's 14 slots, none added;
	Type, 0x80A8D0, returns 6); CreateClone (0x80A270) allocates 0x1C
	bytes, clears the flags and the smart pointer at 0x18, assigns it null
	again in the constructor's body and then takes the source's image; the
	viewer strings name "m_spImage"; the destructor (0x80A1D0) releases
	the image; LoadBinary (0x80A480) skips two
	values that files older than "3.04" store after the image's link ID.
	The NiRTTI is 0x9A24D8, named "NiTextureProperty".

===============================================================================
*/

#ifndef __NITEXTUREPROPERTY_H__
#define __NITEXTUREPROPERTY_H__

#include "NiProperty.h"
#include "renderer/NiImage.h"
#include "NiStreamMacros.h"

/*
================
NiTexturePropertyLoadRecord

The record CreateObject hands the stream: NiObjectNET's link IDs and the
image's, under a vtable of its own (0x89A2C0). The class name is
invented.
================
*/
// FUNCTION: STBC 0x0080a450 ??_GNiTexturePropertyLoadRecord@@UAEPAXI@Z @netimmerse/NiTextureProperty
class NiTexturePropertyLoadRecord : public NiObjectNETLoadRecord {
public:
	unsigned int			imageLinkID;	// not cleared: LoadBinary always reads it
};

// FUNCTION: STBC 0x0080a8a0 ??_GNiTextureProperty@@UAEPAXI@Z @netimmerse/NiTextureProperty
class NiTextureProperty : public NiProperty {
public:
	// FUNCTION: STBC 0x80A890 ?GetRTTI@NiTextureProperty@@UBEPBVNiRTTI@@XZ
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

	// FUNCTION: STBC 0x80A8D0 ?Type@NiTextureProperty@@UAEHXZ
	virtual int				Type() { return TEXTURE; }
	static int				GetType() { return TEXTURE; }

	// the property's type number (Type, and GetType for the class)
	enum {
		TEXTURE				= 6
	};

							NiTextureProperty();
	// the image of the file, mipmapped in the default format
							NiTextureProperty( const char *fileName );
							NiTextureProperty( NiPointer<NiRawImageData> rawImage );
	virtual					~NiTextureProperty();

	NiImage *				GetImage() const { return image; }
	void					SetImage( NiImage *image_ ) { image = image_; }	// NiFlipController::Update (0x793DC0)

	// the texture property of objects without one
	// FUNCTION: STBC 0x820C80 ?GetDefault@NiTextureProperty@@SA?AV?$NiPointer@VNiTextureProperty@@@@XZ
	static NiPointer<NiTextureProperty>	GetDefault() { return defaultProperty; }

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

	// the default property, which the file-scope init object creates
	static NiPointer<NiTextureProperty>	defaultProperty;	// name invented

protected:
	void					CopyMembers( NiTextureProperty *dest );

	NiPointer<NiImage>		image;
};

NiRegisterStream( NiTextureProperty );

#endif /* !__NITEXTUREPROPERTY_H__ */
