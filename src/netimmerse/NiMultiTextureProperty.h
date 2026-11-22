/*
===============================================================================

	NiMultiTextureProperty.h

	The extra texture maps of a surface, each with its own image, clamp and
	filter modes and texture coordinate set: a dark map, a detail map, a
	gloss map, a glow map, a glow alpha map and any number of decal maps.

	Evidence: the vtable at 0x899F38 (NiProperty's 14 slots, none added;
	Type, 0x803750, returns 13); CreateClone (0x8022A0) allocates 0x34
	bytes, clears the flags, builds an array of map pointers at 0x18
	(vtable 0x899F70, one element growing by one), empties its first six
	elements, clears 0x30, then copies 0x30 and each map (0x18 bytes); the
	destructor is 0x8021F0. RegisterStreamables (0x802D10) and SaveBinary
	(0x802DE0) skip the glow alpha map. The viewer strings (0x802F40) name each map by
	its index ("MultiTexture Dark Map %d" for 0, then Detail, Gloss, Glow,
	Glow Alpha, and "MultiTexture Decal Map %d" from 5 on) and format a
	map's "m_spImage " (0x00), "m_eClamp " (0x04) and "m_eFilter " (0x08)
	with NiTextureModeProperty's mode names, "m_uiTexCoord " (0x0C), "m_sL"
	(0x10) and "m_sK" (0x12). The NiRTTI is 0x9A2268, named
	"NiMultiTextureProperty".

===============================================================================
*/

#ifndef __NIMULTITEXTUREPROPERTY_H__
#define __NIMULTITEXTUREPROPERTY_H__

#include "NiTextureModeProperty.h"
#include "NiStencilProperty.h"
#include "NiTArray.h"
#include "renderer/NiImage.h"
#include "NiStreamMacros.h"

/*
================
NiMultiTexturePropertyLoadRecord

The link IDs of the maps' images, by map index, read by LoadBinary
(0x8029D0) and resolved by LinkObject (0x802C60). The class name is
invented.

Evidence: CreateObject (0x802630) allocates 0x28 bytes: NiObjectNET's link
IDs, then an array at 0x10 (vtable 0x89824C, NiFlipControllerLoadRecord's
image array) of the default sizes. Between NiObjectNET's record and this
one CreateObject stores the vtable 0x899F78, the one NiStencilProperty's
record has, and this unit emits that class's scalar deleting destructor
(0x8027F0) beside this record's (0x8027A0): it is the base. TODO identity:
both names are invented; the shared base is likely a record of NiProperty's
own that NiStencilProperty uses unchanged.
================
*/
// FUNCTION: STBC 0x8027A0 ??_GNiMultiTexturePropertyLoadRecord@@UAEPAXI@Z
// FUNCTION: STBC 0x8027D0 ??1NiMultiTexturePropertyLoadRecord@@UAE@XZ
class NiMultiTexturePropertyLoadRecord : public NiStencilPropertyLoadRecord {
public:
	NiTArray<NiImage *>		imageLinkIDs;
};

// FUNCTION: STBC 0x803720 ??_GNiMultiTextureProperty@@UAEPAXI@Z
class NiMultiTextureProperty : public NiProperty {
public:
	// one texture map (the class name is invented)
	class Map {
	public:
		NiPointer<NiImage>		image;
		NiTextureModeProperty::ClampMode	clamp;
		NiTextureModeProperty::FilterMode	filter;
		unsigned int			texCoord;		// the texture coordinate set
		short					l;				// as NiTextureModeProperty's
		short					k;
		unsigned char			unknown14;
		unsigned char			unknown15;

		// an image's map; its out-of-line copy (0x453D80) comes from a game
		// unit (Gamebryo lineage order: image, texture set, clamp, filter)
		// FUNCTION: STBC 0x453D80 ??0Map@NiMultiTextureProperty@@QAE@V?$NiPointer@VNiImage@@@@IW4ClampMode@NiTextureModeProperty@@W4FilterMode@4@@Z @game/LODModel
								Map( NiPointer<NiImage> image_, unsigned int texCoord_, NiTextureModeProperty::ClampMode clamp_, NiTextureModeProperty::FilterMode filter_ ) {
									image = image_;
									clamp = clamp_;
									filter = filter_;
									texCoord = texCoord_;
									l = 0;
									k = -75;
									unknown14 = 0;
									unknown15 = 0;
								}

		// the members, as the SDK's debug NetImmerse 3.1 reads them
		// (AnimOnly.exe 0x4472C0, 0x447330, 0x447350, 0x4473B0, 0x447370,
		// 0x447390; names after Gamebryo's NiTexturingProperty::Map,
		// invented where it has none)
		NiImage *				GetImage() const { return image; }
		NiTextureModeProperty::ClampMode	GetClampMode() const { return clamp; }
		NiTextureModeProperty::FilterMode	GetFilterMode() const { return filter; }
		unsigned int			GetTextureIndex() const { return texCoord; }
		short					GetL() const { return l; }
		short					GetK() const { return k; }

		// an empty map for LoadBinary to fill
								Map() {
									image = NULL;
									clamp = NiTextureModeProperty::WRAP_S_WRAP_T;
									filter = NiTextureModeProperty::FILTER_NEAREST;
									texCoord = 0;
									l = 0;
									k = -75;
									unknown14 = 0;
									unknown15 = 0;
								}

		// a copy of a map, sharing its image (CopyMembers expands it)
								Map( const Map &map ) {
									image = map.image;
									clamp = map.clamp;
									filter = map.filter;
									texCoord = map.texCoord;
									l = map.l;
									k = map.k;
									unknown14 = map.unknown14;
									unknown15 = map.unknown15;
								}

		// the image as its link ID, then the members (names invented); the
		// load records the link ID under the map's index for LinkObject
		void					LoadBinary( NiStream &stream, unsigned int index );
		void					SaveBinary( NiStream &stream );
		// the images are equal when both or neither are set
		bool					IsEqual( Map *map );
		// takes the image (name invented). The by-value smart pointer gives it
		// an unwind frame, so every caller calls it out of line; retail keeps
		// ModelHelpers' copy, the first unit that calls it
		// FUNCTION: STBC 0x45DFF0 ?SetImage@Map@NiMultiTextureProperty@@QAEXV?$NiPointer@VNiImage@@@@@Z @game/ModelHelpers
		void					SetImage( NiPointer<NiImage> image_ ) { image = image_; }
	};

	// the maps' indices; decal maps follow from DECAL_INDEX on. The glow
	// alpha map is not saved.
	enum {
		DARK_INDEX,
		DETAIL_INDEX,
		GLOSS_INDEX,
		GLOW_INDEX,
		GLOW_ALPHA_INDEX,
		DECAL_INDEX
	};

	// FUNCTION: STBC 0x803710 ?GetRTTI@NiMultiTextureProperty@@UBEPBVNiRTTI@@XZ
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

	// FUNCTION: STBC 0x803750 ?Type@NiMultiTextureProperty@@UAEHXZ
	virtual int				Type() { return MULTI_TEXTURE; }
	static int				GetType() { return MULTI_TEXTURE; }

	// the property's type number (Type, and GetType for the class)
	enum {
		MULTI_TEXTURE		= 13
	};

							NiMultiTextureProperty();
							~NiMultiTextureProperty();

	// the maps, indexed as above; null where absent (names invented)
	unsigned int			GetMapCount() const { return maps.GetSize(); }
	Map *					GetMap( unsigned int index ) const { return maps.GetAt( index ); }
	// each map by name, and the decal maps by their own index
	// (NiMultiTextureProperty.inl, AnimOnly.exe 0x52BA00-0x52BB50; names
	// after the indices)
	Map *					GetDarkMap() const { return maps.GetAt( DARK_INDEX ); }
	Map *					GetDetailMap() const { return maps.GetAt( DETAIL_INDEX ); }
	Map *					GetGlossMap() const { return maps.GetAt( GLOSS_INDEX ); }
	Map *					GetGlowMap() const { return maps.GetAt( GLOW_INDEX ); }
	Map *					GetGlowAlphaMap() const { return maps.GetAt( GLOW_ALPHA_INDEX ); }
	unsigned int			GetDecalMapCount() const { return maps.GetSize() - DECAL_INDEX; }
	Map *					GetDecalMap( unsigned int index ) const {
								assert( ( index + DECAL_INDEX ) < maps.GetSize() );
								return maps.GetAt( index + DECAL_INDEX );
							}
	// puts a map in the slot at index, deleting the one there (name
	// invented); BackdropSphere's mesh builder (0x58DB00) expands it
	void					SetMap( unsigned int index, Map *map ) {
		Map *old = maps.GetAt( index );
		if ( map != old ) {
			delete old;
			maps.SetAt( index, map );
		}
	}
	// deletes the map at index, if there is one, and empties its slot (name
	// invented); the game's DisableGlowAlphaMaps (0x45C540) expands it
	void					RemoveMap( unsigned int index ) {
		Map *map = maps.GetAt( index );
		if ( map != NULL ) {
			delete map;
			maps.SetAt( index, NULL );
		}
	}

	// the multi-texture property of objects without one
	// FUNCTION: STBC 0x7D0EB0 ?GetDefault@NiMultiTextureProperty@@SA?AV?$NiPointer@VNiMultiTextureProperty@@@@XZ
	static NiPointer<NiMultiTextureProperty>	GetDefault() { return defaultProperty; }

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

	// the default property, which the file-scope init object creates
	static NiPointer<NiMultiTextureProperty>	defaultProperty;	// name invented

protected:
	void					CopyMembers( NiMultiTextureProperty *dest );

	NiTArray<Map *>			maps;			// indexed as above; null where absent
	unsigned int			unknown30;		// cleared by the constructor, copied by CreateClone
};

NiRegisterStream( NiMultiTextureProperty );

#endif /* !__NIMULTITEXTUREPROPERTY_H__ */
