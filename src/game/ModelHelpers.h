/*
===============================================================================

	ModelHelpers.h

	Game functions on NetImmerse model trees that walk a node and its
	children (ModelHelpers.cpp; the unit and the file name are invented).

===============================================================================
*/

#ifndef __MODELHELPERS_H__
#define __MODELHELPERS_H__

class NiAVObject;

// Drops the glow alpha map of every multi-texture property in the tree
// (invented name, after DamageableObject::DisableGlowAlphaMaps, which calls
// it on the model and on the cloned model).
void	DisableGlowAlphaMaps( NiAVObject *object );

// Counts a change on every texture image in the tree: the texture's and
// the first five multi-texture maps' (invented name; the model manager
// passes it to its walks).
void	TouchTextureImages( NiAVObject *object );

// Drops the gloss map of every multi-texture property in the tree (invented
// name, after DisableGlowAlphaMaps; nothing calls it).
void	DisableGlossMaps( NiAVObject *object );

// Gives every textured object in the tree whose image file name has the
// suffix in it a gloss map of that image, or of the file named with
// glossSuffix (invented name, after DisableGlossMaps; nothing calls it).
void	EnableGlossMaps( NiAVObject *object, const char *suffix, const char *glossSuffix );

class NiObject;

// Gives every triangle shape in the tree whose data is shared its own copy
// of the data; with always set, the object's own shape is copied whatever
// (invented name: CopyModel calls it with false on its copies, the
// damageable object's clone with true).
void	UnshareTriShapeData( NiObject *object, bool always );

// Sets the revision ID of the data of every geometry in the tree to 0,
// which tells the renderer the data never changes (invented name).
void	ClearRevisionIDs( NiAVObject *object );

class NiImage;

// Gives the image to every texture and multi-texture map in the tree whose
// image has the name in its file name (invented name; ObjectClass's
// ReplaceTexture and the character's body and head replacement call it).
void	ReplaceTextureImages( NiAVObject *object, NiImage *image, const char *name );

class NiProperty;

// The first object named so at or under the object, depth first; NULL
// when there is none (invented name; BridgeObjectClass's light setup
// looks its nodes up with it).
NiAVObject *	FindNamedObject( NiAVObject *object, const char *name );

// The property of the type of the first object named so at or under the
// object, depth first; NULL when there is none (invented name;
// BridgeObjectClass's light setup looks its materials up with it).
NiProperty *	FindNamedProperty( NiAVObject *object, const char *name, int type );

// Takes the property of the type off the object and off every object below it
// (invented name; BaseObjectClass::RemovePropertyFromTree is the same walk
// that uses the object before testing it).
void	RemovePropertyFromTree( NiAVObject *object, int type );

// Gives every textured object in the tree whose image file name has the
// suffix in it (every one for NULL) a glow alpha map of that image
// (invented name, after DisableGlowAlphaMaps; LOD models pass their parts'
// glow suffix, BridgeObjectClass's light setup NULL).
void	EnableGlowAlphaMaps( NiAVObject *object, const char *suffix );

#endif /* !__MODELHELPERS_H__ */
