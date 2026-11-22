/*
===============================================================================

	LODModel.cpp

===============================================================================
*/

#include <iostream>
#include <string.h>

#include "LODModel.h"
#include "LODModelManager.h"
#include "Glow.h"
#include "DamageableObject.h"
#include "ImageManagerClass.h"
#include "TGModelManager.h"
#include "TGStream.h"
#include "TGNetwork.h"
#include "UtopiaModule.h"
#include "OBBModel.h"
#include "ModelHelpers.h"
#include "ModelMerge.h"
#include "SpecularPass.h"
#include "TGIconGroup.h"
#include "TGIconManager.h"
#include "VisibleDamage.h"
#include "Voxelizer.h"
#include "NiAVObject.h"
#include "NiAccumulator.h"
#include "NiBinaryVoxelData.h"
#include "NiBinaryVoxelExtraData.h"
#include "NiDynamicEffect.h"
#include "NiExtraData.h"
#include "NiFile.h"
#include "NiFlipController.h"
#include "NiLODNode.h"
#include "NiObject.h"
#include "NiMaterialProperty.h"
#include "NiMultiTextureProperty.h"
#include "NiTextureProperty.h"
#include "NiTriShape.h"
#include "NiVertexColorProperty.h"
#include "NiNode.h"
#include "NiObjectNET.h"
#include "NiProperty.h"
#include "NiStream.h"
#include "renderer/NiImage.h"
#include "renderer/NiRender.h"
#include "NiTimeController.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x00454c60 _$E512
// FUNCTION: STBC 0x00454c80 _$E511

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x0044f380 _$E32
// FUNCTION: STBC 0x0044f390 _$E29
// FUNCTION: STBC 0x0044f3a0 _$E31
// FUNCTION: STBC 0x0044f3b0 _$E30
// FUNCTION: STBC 0x0044f3c0 _$E38
// FUNCTION: STBC 0x0044f3d0 _$E35
// FUNCTION: STBC 0x0044f3e0 _$E37
// FUNCTION: STBC 0x0044f3f0 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0044f340 _$E327
// FUNCTION: STBC 0x0044f350 _$E326
// FUNCTION: STBC 0x0044f360 _$E333
// FUNCTION: STBC 0x0044f370 _$E332
// FUNCTION: STBC 0x0044f400 _$E340
// FUNCTION: STBC 0x0044f410 _$E339
// FUNCTION: STBC 0x0044f420 _$E47
// FUNCTION: STBC 0x0044f430 _$E46
// FUNCTION: STBC 0x0044f440 _$E51
// FUNCTION: STBC 0x0044f450 _$E50
// FUNCTION: STBC 0x00450560 _$E62
// FUNCTION: STBC 0x00450570 _$E61
// FUNCTION: STBC 0x00451330 _$E66
// FUNCTION: STBC 0x00451340 _$E65
// FUNCTION: STBC 0x00451350 _$E70
// FUNCTION: STBC 0x00451360 _$E69
// FUNCTION: STBC 0x00451370 _$E74
// FUNCTION: STBC 0x00451380 _$E73
// FUNCTION: STBC 0x00451480 _$E198
// FUNCTION: STBC 0x00451490 _$E197
// FUNCTION: STBC 0x004536f0 _$E140
// FUNCTION: STBC 0x00453700 _$E139
// FUNCTION: STBC 0x00453710 _$E153
// FUNCTION: STBC 0x00453720 _$E152
// FUNCTION: STBC 0x00453730 _$E237
// FUNCTION: STBC 0x00453740 _$E236
// FUNCTION: STBC 0x00453750 _$E242
// FUNCTION: STBC 0x00453760 _$E241
// FUNCTION: STBC 0x00453770 _$E281
// FUNCTION: STBC 0x00453780 _$E280
// FUNCTION: STBC 0x00453790 _$E300
// FUNCTION: STBC 0x004537a0 _$E299
// FUNCTION: STBC 0x004537b0 _$E307
// FUNCTION: STBC 0x004537c0 _$E306
// FUNCTION: STBC 0x004537d0 _$E358
// FUNCTION: STBC 0x004537e0 _$E357
// FUNCTION: STBC 0x004537f0 _$E362
// FUNCTION: STBC 0x00453800 _$E361
// FUNCTION: STBC 0x00453810 _$E98
// FUNCTION: STBC 0x00453820 _$E97
// FUNCTION: STBC 0x00453830 _$E207
// FUNCTION: STBC 0x00453840 _$E206
// FUNCTION: STBC 0x00453890 _$E214
// FUNCTION: STBC 0x004538a0 _$E213
// FUNCTION: STBC 0x004538b0 _$E218
// FUNCTION: STBC 0x004538c0 _$E217
// FUNCTION: STBC 0x004538d0 _$E222
// FUNCTION: STBC 0x004538e0 _$E221
// FUNCTION: STBC 0x004538f0 _$E226
// FUNCTION: STBC 0x00453900 _$E225
// FUNCTION: STBC 0x00453910 _$E232
// FUNCTION: STBC 0x00453920 _$E231
// FUNCTION: STBC 0x00453930 _$E261
// FUNCTION: STBC 0x00453940 _$E260
// FUNCTION: STBC 0x00453950 _$E366
// FUNCTION: STBC 0x00453960 _$E365
// FUNCTION: STBC 0x00453970 _$E370
// FUNCTION: STBC 0x00453980 _$E369
// FUNCTION: STBC 0x00453990 _$E374
// FUNCTION: STBC 0x004539a0 _$E373
// FUNCTION: STBC 0x004539b0 _$E378
// FUNCTION: STBC 0x004539c0 _$E377
// FUNCTION: STBC 0x004539d0 _$E382
// FUNCTION: STBC 0x004539e0 _$E381
// FUNCTION: STBC 0x004539f0 _$E386
// FUNCTION: STBC 0x00453a00 _$E385
// FUNCTION: STBC 0x00453a10 _$E390
// FUNCTION: STBC 0x00453a20 _$E389
// FUNCTION: STBC 0x00453a30 _$E394
// FUNCTION: STBC 0x00453a40 _$E393
// FUNCTION: STBC 0x00453ac0 _$E398
// FUNCTION: STBC 0x00453ad0 _$E397
// FUNCTION: STBC 0x00453b50 _$E103
// FUNCTION: STBC 0x00453b60 _$E102
// FUNCTION: STBC 0x00453ba0 _$E402
// FUNCTION: STBC 0x00453bb0 _$E401
// FUNCTION: STBC 0x00453bc0 _$E108
// FUNCTION: STBC 0x00453bd0 _$E107
// FUNCTION: STBC 0x00453bf0 _$E406
// FUNCTION: STBC 0x00453c00 _$E405
// FUNCTION: STBC 0x00453c40 _$E410
// FUNCTION: STBC 0x00453c50 _$E409
// FUNCTION: STBC 0x00453c60 _$E344
// FUNCTION: STBC 0x00453c70 _$E343
// FUNCTION: STBC 0x00453c80 _$E414
// FUNCTION: STBC 0x00453c90 _$E413
// FUNCTION: STBC 0x00453ca0 _$E117
// FUNCTION: STBC 0x00453cb0 _$E116
// FUNCTION: STBC 0x00453cc0 _$E418
// FUNCTION: STBC 0x00453cd0 _$E417
// FUNCTION: STBC 0x00453ce0 _$E422
// FUNCTION: STBC 0x00453cf0 _$E421
// FUNCTION: STBC 0x00453d00 _$E348
// FUNCTION: STBC 0x00453d10 _$E347
// FUNCTION: STBC 0x00453d20 _$E123
// FUNCTION: STBC 0x00453d30 _$E122
// FUNCTION: STBC 0x00453d40 _$E128
// FUNCTION: STBC 0x00453d50 _$E127
// FUNCTION: STBC 0x00453d60 _$E158
// FUNCTION: STBC 0x00453d70 _$E157
// FUNCTION: STBC 0x00453e10 _$E426
// FUNCTION: STBC 0x00453e20 _$E425
// FUNCTION: STBC 0x00453e30 _$E163
// FUNCTION: STBC 0x00453e40 _$E162
// FUNCTION: STBC 0x00453e50 _$E430
// FUNCTION: STBC 0x00453e60 _$E429
// FUNCTION: STBC 0x00453e70 _$E434
// FUNCTION: STBC 0x00453e80 _$E433
// FUNCTION: STBC 0x00453e90 _$E168
// FUNCTION: STBC 0x00453ea0 _$E167
// FUNCTION: STBC 0x00453eb0 _$E438
// FUNCTION: STBC 0x00453ec0 _$E437
// FUNCTION: STBC 0x00453ed0 _$E133
// FUNCTION: STBC 0x00453ee0 _$E132
// FUNCTION: STBC 0x00453ef0 _$E442
// FUNCTION: STBC 0x00453f00 _$E441
// FUNCTION: STBC 0x00453f10 _$E446
// FUNCTION: STBC 0x00453f20 _$E445
// FUNCTION: STBC 0x00453f30 _$E173
// FUNCTION: STBC 0x00453f40 _$E172
// FUNCTION: STBC 0x00454060 _$E178
// FUNCTION: STBC 0x00454070 _$E177
// FUNCTION: STBC 0x004541b0 _$E450
// FUNCTION: STBC 0x004541c0 _$E449
// FUNCTION: STBC 0x004541d0 _$E454
// FUNCTION: STBC 0x004541e0 _$E453
// FUNCTION: STBC 0x00454280 _$E458
// FUNCTION: STBC 0x00454290 _$E457
// FUNCTION: STBC 0x004542a0 _$E462
// FUNCTION: STBC 0x004542b0 _$E461
// FUNCTION: STBC 0x004542c0 _$E466
// FUNCTION: STBC 0x004542d0 _$E465
// FUNCTION: STBC 0x00454470 _$E470
// FUNCTION: STBC 0x00454480 _$E469
// FUNCTION: STBC 0x00454550 _$E474
// FUNCTION: STBC 0x00454560 _$E473
// FUNCTION: STBC 0x00454570 _$E478
// FUNCTION: STBC 0x00454580 _$E477
// FUNCTION: STBC 0x00454590 _$E183
// FUNCTION: STBC 0x004545a0 _$E182
// FUNCTION: STBC 0x004545b0 _$E482
// FUNCTION: STBC 0x004545c0 _$E481
// FUNCTION: STBC 0x004545d0 _$E188
// FUNCTION: STBC 0x004545e0 _$E187
// FUNCTION: STBC 0x004545f0 _$E193
// FUNCTION: STBC 0x00454600 _$E192
// FUNCTION: STBC 0x00454fa0 _$E486
// FUNCTION: STBC 0x00454fb0 _$E485
// FUNCTION: STBC 0x00454fc0 _$E491
// FUNCTION: STBC 0x00454fd0 _$E490
// FUNCTION: STBC 0x00454fe0 _$E495
// FUNCTION: STBC 0x00454ff0 _$E494

// The one model manager.
// FUNCTION: STBC 0x0044f460 _$E500
// FUNCTION: STBC 0x0044f470 _$E497
// FUNCTION: STBC 0x0044f480 _$E499
// FUNCTION: STBC 0x0044f490 _$E498
// DATA: STBC 0x00980798
LODModelManager g_kLODModelManager;

// DATA: STBC 0x008e103c
bool LODModelManager::specularMapsEnabled = true;

// DATA: STBC 0x008e103d
bool LODModelManager::motionBlurEnabled = true;

// The image details' texture subdirectories and the default shared texture
// path (.rdata beside the unit's vtables, 0x88B6FC to 0x88B714; the names
// are invented).
static const char LOW_DETAIL_DIRECTORY[] = "/Low";
static const char MEDIUM_DETAIL_DIRECTORY[] = "/Medium";
static const char HIGH_DETAIL_DIRECTORY[] = "/High";
static const char SHARED_TEXTURE_PATH[] = "data/Models/SharedTextures";

/*
================
BuildTexturePath

The texture search path of a model file: the file's directory with the
image detail's subdirectory, then the shared texture path (the shared
models' textures by default) with the same subdirectory, separated by a
semicolon. A new heap string. The name is invented.
================
*/
// FUNCTION: STBC 0x0044f4a0
char *BuildTexturePath( char *file, char *sharePath ) {
	const char *detail;	// BUG: left unset for an image detail past HIGH_DETAIL
	switch ( g_kImageManager.GetImageDetail() ) {
		case ImageManagerClass::LOW_DETAIL:
			detail = LOW_DETAIL_DIRECTORY;
			break;
		case ImageManagerClass::MEDIUM_DETAIL:
			detail = MEDIUM_DETAIL_DIRECTORY;
			break;
		case ImageManagerClass::HIGH_DETAIL:
			detail = HIGH_DETAIL_DIRECTORY;
			break;
	}

	char *slash = strrchr( file, '/' );
	unsigned int length = strlen( file ) - strlen( slash ) + strlen( detail ) + 1;
	if ( sharePath == NULL ) {
		sharePath = (char *)SHARED_TEXTURE_PATH;
	}
	if ( sharePath != NULL ) {	// BUG: redundant: sharePath was given a default above
		length += strlen( sharePath ) + strlen( detail ) + 1;
	}
	char *path = new char[length];

	*slash = '\0';
	strcpy( path, file );
	*slash = '/';
	strcat( path, detail );
	if ( sharePath != NULL ) {	// BUG: redundant: as above
		strcat( path, ";" );
		strcat( path, sharePath );
		strcat( path, detail );
	}
	return path;
}

/*
================
BuildDetailFilePath

The path of a file under the image detail's subdirectory: in the directory
of the model file when asked, else in the given directory. A new heap
string. Nothing in the game calls it. The name is invented.
================
*/
// FUNCTION: STBC 0x0044f620
char *BuildDetailFilePath( char *directory, char *name, bool modelDirectory ) {
	const char *detail;	// BUG: left unset for an image detail past HIGH_DETAIL
	switch ( g_kImageManager.GetImageDetail() ) {
		case ImageManagerClass::LOW_DETAIL:
			detail = LOW_DETAIL_DIRECTORY;
			break;
		case ImageManagerClass::MEDIUM_DETAIL:
			detail = MEDIUM_DETAIL_DIRECTORY;
			break;
		case ImageManagerClass::HIGH_DETAIL:
			detail = HIGH_DETAIL_DIRECTORY;
			break;
	}

	if ( modelDirectory ) {
		char *slash = strrchr( directory, '/' );
		char *path = new char[strlen( directory ) - strlen( slash ) + strlen( detail ) + strlen( name ) + 2];
		*slash = '\0';
		strcpy( path, directory );
		*slash = '/';
		strcat( path, detail );
		strcat( path, "/" );
		strcat( path, name );
		return path;
	}
	char *path = new char[strlen( directory ) + strlen( detail ) + strlen( name ) + 2];
	strcpy( path, directory );
	strcat( path, detail );
	strcat( path, "/" );
	strcat( path, name );
	return path;
}

/*
================
LODModelPart::LODModelPart

Copies the strings; a missing damage texture, damage map or damage
texture directory takes its alternative, else the default.
================
*/
// FUNCTION: STBC 0x0044f800 ??0LODModelPart@@QAE@PADHMMMMM0000000000@Z
LODModelPart::LODModelPart( char *file_, int unknown4C_, float distance_, float unknown38_, float unknown3C_,
							float unknown40_, float unknown44_, char *glowSuffix_, char *unknown0C_,
							char *specularSuffix_, char *damageTexture_, char *unknown18_, char *damageMap_,
							char *unknown24_, char *damageTextures_, char *unknown2C_, char *unknown30_ ) {
	loaded = false;
	unknown38 = unknown38_;
	unknown3C = unknown3C_;
	unknown40 = unknown40_;
	file = NULL;
	voxelFile = NULL;
	glowSuffix = NULL;
	unknown0C = NULL;
	specularSuffix = NULL;
	damageTexture = NULL;
	unknown18 = NULL;
	damageMap = NULL;
	unknown24 = NULL;
	damageTextures = NULL;
	unknown2C = NULL;
	unknown30 = NULL;
	textureSharePath = NULL;
	unknown44 = unknown44_;
	distance = distance_;
	unknown4C = unknown4C_;

	if ( file_ == NULL ) {
		return;
	}

	file = new char[strlen( file_ ) + 1];
	strcpy( file, file_ );
	voxelFile = GetVoxelFileName();

	if ( glowSuffix_ != NULL ) {
		glowSuffix = new char[strlen( glowSuffix_ ) + 1];
		strcpy( glowSuffix, glowSuffix_ );
	}
	if ( unknown0C_ != NULL ) {
		unknown0C = new char[strlen( unknown0C_ ) + 1];
		strcpy( unknown0C, unknown0C_ );
	}
	if ( specularSuffix_ != NULL ) {
		specularSuffix = new char[strlen( specularSuffix_ ) + 1];
		strcpy( specularSuffix, specularSuffix_ );
	}

	if ( damageTexture_ != NULL ) {
		damageTexture = new char[strlen( damageTexture_ ) + 1];
		strcpy( damageTexture, damageTexture_ );
	} else if ( unknown18_ != NULL ) {
		unknown18 = new char[strlen( unknown18_ ) + 1];
		strcpy( unknown18, unknown18_ );
	} else {
		damageTexture = new char[strlen( "data/Textures/Effects/Damage.tga" ) + 1];
		strcpy( damageTexture, "data/Textures/Effects/Damage.tga" );
	}

	if ( damageMap_ != NULL ) {
		damageMap = new char[strlen( damageMap_ ) + 1];
		strcpy( damageMap, damageMap_ );
	} else if ( unknown24_ != NULL ) {
		unknown24 = new char[strlen( unknown24_ ) + 1];
		strcpy( damageTexture, unknown24_ );	// BUG: copies into damageTexture, not the new unknown24
	} else {
		damageMap = new char[strlen( "data/Damage1.tga" ) + 1];
		strcpy( damageMap, "data/Damage1.tga" );
	}

	if ( damageTextures_ != NULL ) {
		damageTextures = new char[strlen( damageTextures_ ) + 1];
		strcpy( damageTextures, damageTextures_ );
	} else if ( unknown2C_ != NULL ) {
		unknown2C = new char[strlen( unknown2C_ ) + 1];
		strcpy( unknown2C, unknown2C_ );
	} else {
		unknown2C = new char[strlen( "DamageTextures" ) + 1];
		strcpy( unknown2C, "DamageTextures" );
	}

	if ( unknown30_ != NULL ) {
		unknown30 = new char[strlen( unknown30_ ) + 1];
		strcpy( unknown30, unknown30_ );
	}
}

/*
================
LODModelPart::LODModelPart

An empty level, for ReadFromStream to fill.
================
*/
// FUNCTION: STBC 0x0044fb70 ??0LODModelPart@@QAE@XZ
LODModelPart::LODModelPart() {
	loaded = false;
	textureSharePath = NULL;
	file = NULL;
	voxelFile = NULL;
	glowSuffix = NULL;
	unknown0C = NULL;
	specularSuffix = NULL;
	damageTexture = NULL;
	unknown18 = NULL;
	damageMap = NULL;
	unknown24 = NULL;
	damageTextures = NULL;
	unknown2C = NULL;
	unknown30 = NULL;
}

/*
================
LODModelPart::~LODModelPart

Gives the loaded files back to the model manager and frees the strings.
================
*/
// FUNCTION: STBC 0x0044fba0
LODModelPart::~LODModelPart() {
	if ( loaded ) {
		g_kModelManager.Unrefer( file );
		if ( voxelFile != NULL && g_kModelManager.IsModelLoaded( voxelFile ) ) {
			g_kModelManager.Unrefer( voxelFile );
		}
	}
	delete file;
	delete voxelFile;
	delete glowSuffix;
	delete unknown0C;
	delete specularSuffix;
	delete unknown18;
	delete unknown24;
	delete damageTexture;
	delete unknown18;		// BUG: redundant: unknown18 was deleted above
	delete damageMap;
	delete unknown24;		// BUG: redundant: unknown24 was deleted above
	delete damageTextures;
	delete unknown2C;
	delete unknown30;
}

/*
================
LODModelPart::Load
================
*/
// FUNCTION: STBC 0x0044fc60
void LODModelPart::Load() {
	if ( loaded ) {
		return;
	}
	loaded = true;

	char *texturePath = BuildTexturePath( file, textureSharePath );
	g_kModelManager.LoadModel( file, NULL, texturePath );
	delete texturePath;

	if ( unknown3C > 0.0f ) {
		NiFile voxels( voxelFile, NiFile::READ_ONLY, 0 );
		if ( voxels ) {
			g_kModelManager.LoadModel( voxelFile, NULL, NULL );
		}
	}
}

/*
================
LODModelPart::LoadIncremental

Load's queued form: the files go to the model manager's incremental load
queue, once.
================
*/
// FUNCTION: STBC 0x0044fd30
void LODModelPart::LoadIncremental() {
	if ( loaded ) {
		return;
	}
	loaded = true;

	char *texturePath = BuildTexturePath( file, textureSharePath );
	g_kModelManager.LoadModelIncremental( file, NULL, texturePath );
	delete texturePath;

	if ( unknown3C > 0.0f ) {
		NiFile voxels( voxelFile, NiFile::READ_ONLY, 0 );
		if ( voxels ) {
			g_kModelManager.LoadModelIncremental( voxelFile, NULL, NULL );
		}
	}
}

/*
================
LODModelPart::GetVoxelFileName
================
*/
// FUNCTION: STBC 0x0044fe00
char *LODModelPart::GetVoxelFileName() {
	char suffix[] = "_vox";
	char *name = new char[strlen( file ) + strlen( suffix ) + 1];
	strcpy( name, file );
	char *dot = strrchr( name, '.' );
	char *extension = new char[strlen( dot ) + 1];
	strcpy( extension, dot );
	*dot = '\0';
	strcat( name, suffix );
	strcat( name, extension );
	delete extension;
	return name;
}

/*
================
AddGlowPasses

Gives every triangle shape in the tree whose texture's image file name has
the suffix in it a GlowPass: no vertex colours, a black emissive material,
no texture and the image as the glow alpha map. The name is invented.
================
*/
// FUNCTION: STBC 0x0044ff10
void AddGlowPasses( NiAVObject *object, const char *suffix, NiImage *image ) {
	NiTriShape *shape = NiDynamicCast( NiTriShape, object );
	if ( shape != NULL ) {
		NiTextureProperty *texture = NiDynamicCast( NiTextureProperty, shape->GetProperty( NiTextureProperty::TEXTURE ) );
		if ( texture && texture->GetImage() && texture->GetImage()->GetURL()
			 && strstr( texture->GetImage()->GetURL(), suffix ) ) {
			GlowPass *pass = new GlowPass;
			pass->unknown14 = false;

			NiPointer<NiVertexColorProperty> vertexColor = new NiVertexColorProperty;
			vertexColor->SetSourceMode( NiVertexColorProperty::SOURCE_IGNORE );
			vertexColor->SetLightingMode( NiVertexColorProperty::LIGHTING_E );
			pass->properties.AddHead( (NiVertexColorProperty *)vertexColor );

			NiPointer<NiMaterialProperty> material = new NiMaterialProperty;
			material->SetEmittance( NiColor::BLACK );
			pass->properties.AddHead( (NiMaterialProperty *)material );

			NiPointer<NiTextureProperty> noTexture = new NiTextureProperty;
			pass->properties.AddHead( (NiTextureProperty *)noTexture );

			NiPointer<NiMultiTextureProperty> multiTexture = new NiMultiTextureProperty;
			multiTexture->SetMap( NiMultiTextureProperty::GLOW_ALPHA_INDEX,
				new NiMultiTextureProperty::Map( image, 0, NiTextureModeProperty::WRAP_S_WRAP_T,
												 NiTextureModeProperty::FILTER_BILERP ) );
			pass->properties.AddHead( (NiMultiTextureProperty *)multiTexture );

			shape->AddExtraData( pass );
		}
	} else {
		NiNode *node = NiDynamicCast( NiNode, object );
		if ( node != NULL ) {
			for ( unsigned int i = 0; i < node->GetArrayCount(); i++ ) {
				if ( node->GetAt( i ) != NULL ) {
					AddGlowPasses( node->GetAt( i ), suffix, image );
				}
			}
		}
	}
}

void EnableSpecularMaps( NiAVObject *object, const char *filter, const char *specularSuffix, const char *glowSuffix );

/*
================
LODModelPart::Unknown450580

Builds the level: its collision model when 0x4C is positive (the model's
first child merged into one shape first, unless the model already has
OBB roots), its voxel data when 0x3C is positive (from the voxel file, or
voxelized from the model and saved to a new voxel file when there is
none), and its visible damage with the damage textures; then a clone of
the model with a specular and a motion blur pass. The float is not used.
================
*/
// FUNCTION: STBC 0x00450580
// DRAFT: calls and branches as retail but for the inline budget: retail calls NiObject::DecRefCount out of line once more than this build (eight against seven, with the single-statement bodies unbraced) in the merge block's smart pointer destructors (DetachChild's result, merged, child); child and merged share one slot in retail; ~NiPointer<NiAVObject> resolves to no address (retail takes LoadEpisodeAction's 0x401EA0)
void LODModelPart::Unknown450580( NiPointer<NiAVObject> &model, NiPointer<OBBModel> &collisionModel, VisibleDamage *&damage,
								  float unknown, NiTArray<char *> &glowNames, NiTArray< NiPointer<NiImage> > &glowImages ) {
	model = NULL;
	collisionModel = NULL;
	damage = NULL;
	NiPointer<OBBModel> obbModel;

	bool hasCollision = unknown4C > 0;
	TGAnimNode *source = NULL;
	if ( hasCollision ) {
		source = g_kModelManager.GetModel( file );
		if ( !OBBModel::HasOBBRoots( source ) ) {
			{
				NiPointer<NiNode> child = (NiNode *)source->GetAt( 0 );
				NiPointer<NiAVObject> merged = ModelMerge_Merge( child );
				source->DetachChild( child );
				source->AttachChild( merged, true );
			}
			EnableSpecularMaps( source, unknown0C, specularSuffix, glowSuffix );
			obbModel = new OBBModel( source, unknown4C );
			if ( glowSuffix != NULL && glowNames.GetSize() != 0 ) {
				for ( unsigned int i = 0; i < glowNames.GetSize(); i++ )
					AddGlowPasses( source, glowNames.GetAt( i ), glowImages.GetAt( i ) );
			}
		}
	}

	bool hasVoxels = unknown3C > 0.0f;
	if ( hasVoxels ) {
		if ( source == NULL )
			source = g_kModelManager.GetModel( file );
		NiBinaryVoxelExtraData *voxels = NULL;
		NiExtraData *extra = source->GetExtraData();
		if ( extra != NULL ) {
			while ( true ) {
				if ( NiIsExactKindOf( NiBinaryVoxelExtraData, extra ) ) {
					voxels = (NiBinaryVoxelExtraData *)extra;
					break;
				}
				extra = extra->GetNext();
				if ( extra == NULL )
					break;
			}
		}
		if ( voxels == NULL ) {
			NiFile voxelStream( voxelFile, NiFile::READ_ONLY, 0 );
			if ( voxelStream ) {
				// the voxel file's data, moved onto the model
				TGAnimNode *voxelModel = g_kModelManager.GetModel( voxelFile );
				extra = voxelModel->GetAt( 0 )->GetExtraData();
				if ( extra != NULL ) {
					while ( true ) {
						if ( NiIsExactKindOf( NiBinaryVoxelExtraData, extra ) ) {
							voxels = (NiBinaryVoxelExtraData *)extra;
							break;
						}
						extra = extra->GetNext();
						if ( extra == NULL )
							break;
					}
				}
				source->AddExtraData( voxels );
			} else {
				// voxelized now, and saved for the next time
				if ( obbModel == NULL )
					obbModel = new OBBModel( source, unknown4C );
				BinaryVoxelizer *voxelizer = new BinaryVoxelizer;
				voxelizer->SetOBBModel( obbModel );
				voxelizer->SetModel( source );
				voxelizer->SetCellSize( unknown3C );
				voxelizer->SetFileName( file );
				float progress = 0.0f;
				while ( voxelizer->Step( &progress ) ) {
				}

				NiBinaryVoxelExtraData *newVoxels = new NiBinaryVoxelExtraData;
				newVoxels->SetVoxelData( voxelizer->GetVoxelData() );
				NiPointer<NiBinaryVoxelExtraData> keep = newVoxels;
				{
					NiPointer<NiNode> root = new NiNode;
					root->SetName( "Scene Root" );
					root->AddExtraData( newVoxels );
					NiStream stream;
					stream.InsertObject( root );
					voxelFile = GetVoxelFileName();
					stream.Save( voxelFile );
					root->RemoveExtraData( newVoxels );
				}
				source->AddExtraData( newVoxels );
				delete voxelizer;
			}
		}
	}

	TGAnimNode *clone = g_kModelManager.CloneModel( file );
	if ( clone == NULL )
		return;
	model = clone;
	model->Update( 0.0f );
	if ( g_kLODModelManager.AreGlowMapsEnabled() && glowSuffix != NULL )
		EnableGlowAlphaMaps( clone, glowSuffix );
	if ( hasCollision )
		collisionModel = new OBBModel( clone, unknown4C );

	NiBinaryVoxelExtraData *cloneVoxels = NULL;
	if ( hasVoxels ) {
		NiExtraData *extra = clone->GetExtraData();
		if ( extra != NULL ) {
			while ( true ) {
				if ( NiIsExactKindOf( NiBinaryVoxelExtraData, extra ) ) {
					cloneVoxels = (NiBinaryVoxelExtraData *)extra;
					break;
				}
				extra = extra->GetNext();
				if ( extra == NULL )
					break;
			}
		}
	}

	if ( unknown38 <= 0.0f ) {
		damage = new VisibleDamage;
		damage->SetBuilderUnknown10( unknown38 );
		damage->SetBuilderUnknown0C( unknown40 );
		damage->SetBuilderUnknown08( unknown44 );
		damage->SetUnknown00( unknown40 * 0.75f );
	} else {
		damage = new VisibleDamage;
		damage->SetBuilderUnknown10( unknown38 );
		damage->SetBuilderUnknown0C( unknown40 );
		damage->SetBuilderUnknown08( unknown44 );
		damage->SetUnknown00( unknown40 * 0.75f );
		damage->SetBuilderUnknown19( !DamageableObject::IsVolumeDamageGeometryEnabled() );
		if ( cloneVoxels != NULL )
			damage->SetVoxelData( cloneVoxels );
		if ( damageTexture != NULL ) {
			NiPointer<NiTextureProperty> texture = new NiTextureProperty;
			texture->SetImage( g_kImageManager.GetImage( damageTexture, true, true ) );
			damage->SetUnknown14( texture );
		}
		if ( damageMap != NULL ) {
			NiPointer<NiTextureProperty> texture = new NiTextureProperty;
			texture->SetImage( g_kImageManager.GetImage( damageMap, true, true ) );
			damage->SetUnknown18( texture );
		}
		if ( damageTextures != NULL ) {
			NiPointer<NiTextureProperty> texture = new NiTextureProperty;
			texture->SetImage( g_kImageManager.GetImage( damageTextures, true, true ) );
			damage->SetUnknown10( texture );
		} else if ( unknown2C != NULL ) {
			// the icon group's textures flipped through, five a second
			NiTextureProperty *texture = new NiTextureProperty;
			damage->SetUnknown10( texture );
			NiFlipController *controller = new NiFlipController;
			TGIconGroup *group = g_kIconManager.GetIconGroup( unknown2C );
			unsigned int count = group->GetTextures()->GetSize();
			for ( unsigned int i = 0; i < count; i++ )
				controller->SetImage( group->GetIconTexture( i )->GetImage(), i );
			controller->SetAnimType( NiTimeController::APP_INIT );
			controller->SetSecondsPerFrame( 0.2f );
			controller->SetCycleType( NiTimeController::LOOP );
			controller->SetTarget( texture );
			controller->SetStartTime( 0.0f );
			controller->Start( 0.0f );
			controller->SetBeginKeyTime( 0.0f );
			controller->SetEndKeyTime( controller->GetImageCount() * controller->GetSecondsPerFrame() );
		}
		damage->SetOBBModel( collisionModel );
		damage->SetModel( NiDynamicCast( NiNode, clone->GetAt( 0 ) ) );
		collisionModel->callback = damage;
	}

	clone->AttachChild( new SpecularPass( clone->GetAt( 0 ), 1.0f, 4.0f ), false );
	clone->AttachChild( new MotionBlurPass( clone->GetAt( 0 ) ), false );
	model->UpdateProperties();
}

/*
================
LODModelPart::WriteToStream
================
*/
// FUNCTION: STBC 0x00451390
bool LODModelPart::WriteToStream( TGStream *stream ) {
	stream->VirtualSlot11( file );
	stream->VirtualSlot11( glowSuffix );
	stream->VirtualSlot11( unknown0C );
	stream->VirtualSlot11( specularSuffix );
	stream->VirtualSlot11( damageTexture );
	stream->VirtualSlot11( unknown18 );
	stream->VirtualSlot11( damageMap );
	stream->VirtualSlot11( unknown24 );
	stream->VirtualSlot11( damageTextures );
	stream->VirtualSlot11( unknown2C );
	stream->VirtualSlot11( unknown30 );
	stream->WriteFloat( unknown38 );
	stream->WriteFloat( unknown3C );
	stream->WriteFloat( unknown40 );
	stream->WriteFloat( unknown44 );
	stream->WriteFloat( distance );
	stream->WriteLong( unknown4C );
	return true;
}

/*
================
LODModelPart::ReadFromStream
================
*/
// FUNCTION: STBC 0x004514a0
bool LODModelPart::ReadFromStream( TGStream *stream ) {
	file = stream->VirtualSlot10( -1 );
	voxelFile = GetVoxelFileName();
	glowSuffix = stream->VirtualSlot10( -1 );
	unknown0C = stream->VirtualSlot10( -1 );
	specularSuffix = stream->VirtualSlot10( -1 );
	damageTexture = stream->VirtualSlot10( -1 );
	unknown18 = stream->VirtualSlot10( -1 );
	damageMap = stream->VirtualSlot10( -1 );
	unknown24 = stream->VirtualSlot10( -1 );
	damageTextures = stream->VirtualSlot10( -1 );
	unknown2C = stream->VirtualSlot10( -1 );
	unknown30 = stream->VirtualSlot10( -1 );
	unknown38 = stream->ReadFloat();
	unknown3C = stream->ReadFloat();
	unknown40 = stream->ReadFloat();
	unknown44 = stream->ReadFloat();
	distance = stream->ReadFloat();
	unknown4C = stream->ReadLong();
	return true;
}

/*
================
LODModel::~LODModel
================
*/
// FUNCTION: STBC 0x00451580
LODModel::~LODModel() {
	unsigned int i;
	for ( i = 0; i < parts.GetSize(); i++ ) {
		delete parts.GetAt( i );
	}
	for ( i = 0; i < glowNames.GetSize(); i++ ) {
		delete glowNames.GetAt( i );
	}
	for ( i = 0; i < glowFiles.GetSize(); i++ ) {
		delete glowFiles.GetAt( i );
	}
	if ( textureSharePath != NULL ) {
		delete textureSharePath;
	}
}

/*
================
LODModel::SetTextureSharePath

Keeps a copy of the path, and gives it to every level of the model.
================
*/
// FUNCTION: STBC 0x004516a0
void LODModel::SetTextureSharePath( char *path ) {
	delete textureSharePath;
	if ( path != NULL ) {
		textureSharePath = new char[strlen( path ) + 1];
		strcpy( textureSharePath, path );
	} else {
		textureSharePath = NULL;
	}
	for ( unsigned int i = 0; i < parts.GetSize(); i++ ) {
		parts.GetAt( i )->textureSharePath = textureSharePath;
	}
}

/*
================
LODModel::AddLOD
================
*/
// FUNCTION: STBC 0x00451720
void LODModel::AddLOD( char *file, int unknown4C, float distance, float unknown38, float unknown3C, float unknown40,
					   float unknown44, char *glowSuffix, char *unknown0C, char *specularSuffix, char *damageTexture,
					   char *unknown18, char *damageMap, char *unknown24, char *damageTextures, char *unknown2C,
					   char *unknown30 ) {
	LODModelPart *part = new LODModelPart( file, unknown4C, distance, unknown38, unknown3C, unknown40, unknown44,
										   glowSuffix, unknown0C, specularSuffix, damageTexture, unknown18, damageMap,
										   unknown24, damageTextures, unknown2C, unknown30 );
	part->textureSharePath = textureSharePath;
	parts.Add( part );
}

/*
================
LODModel::Load

Loads the levels the dropped LOD level leaves, at least the last one.
================
*/
// FUNCTION: STBC 0x004518a0
void LODModel::Load( void ) {
	int level = g_kLODModelManager.GetDropLODLevel();
	if ( level > maxDropLODLevel ) {
		level = maxDropLODLevel;
	}
	int count = parts.GetSize();
	if ( level >= count ) {
		level = count - 1;
	}
	for ( int i = level; i < count; i++ ) {
		parts.GetAt( i )->Load();
	}
}

/*
================
LODModel::LoadIncremental

Load's queued form, for the same levels.
================
*/
// FUNCTION: STBC 0x004518e0
void LODModel::LoadIncremental( void ) {
	int level = g_kLODModelManager.GetDropLODLevel();
	if ( level > maxDropLODLevel ) {
		level = maxDropLODLevel;
	}
	int count = parts.GetSize();
	if ( level >= count ) {
		level = count - 1;
	}
	for ( int i = level; i < count; i++ ) {
		parts.GetAt( i )->LoadIncremental();
	}
}

/*
================
LODModel::AddGlowPassTexture

Files the image under the geometry's name, when the image loads.
================
*/
// FUNCTION: STBC 0x00451920
void LODModel::AddGlowPassTexture( char *name, char *file ) {
	NiPointer<NiImage> image = g_kImageManager.GetImage( file, true, true );
	if ( image != NULL ) {
		char *nameCopy = new char[strlen( name ) + 1];
		strcpy( nameCopy, name );
		glowNames.Add( nameCopy );
		char *fileCopy = new char[strlen( file ) + 1];
		strcpy( fileCopy, file );
		glowFiles.Add( fileCopy );
		glowImages.Add( image );
	}
}

/*
================
LODModel::Unknown451AC0

Builds an NiLODNode of the levels, skipping the manager's dropped levels
(at most maxDropLODLevel, and never every level), each level's model
under it with its collision model and visible damage appended to the
arrays, sized to the levels first. The switch distances are the levels'
distances over the model's scale. unknown1B0 receives unknown58 or the
last level, unknown1B4 unknown54.
================
*/
// FUNCTION: STBC 0x00451ac0
// DRAFT: inline budget: the level model's release in the level loop expands DecRefCount (share 71) where retail calls it (needs under 68); retail's NiPointer<OBBModel> destructor call goes to 0x436D40 (an identity-TODO copy); the loader 0x450580 is a claim
void LODModel::Unknown451AC0( NiPointer<NiAVObject> &model, NiTArray< NiPointer<OBBModel> > &collisionModels,
							  NiTArray<VisibleDamage *> &damages, int &unknown1B0, int &unknown1B4, float unknown ) {
	Refer();
	if ( parts.GetSize() == 0 ) {
		return;
	}

	NiLODNode *lod = new NiLODNode;
	model = lod;

	int drop = g_kLODModelManager.GetDropLODLevel();
	if ( drop >= (int)parts.GetSize() ) {
		drop = parts.GetSize() - 1;
	}
	if ( drop > maxDropLODLevel ) {
		drop = maxDropLODLevel;
	}
	collisionModels.SetSize( parts.GetSize() - drop );
	damages.SetSize( parts.GetSize() - drop );

	for ( int i = drop; i < (int)parts.GetSize(); i++ ) {
		NiPointer<NiAVObject> levelModel;
		NiPointer<OBBModel> collisionModel;
		VisibleDamage *damage;
		parts.GetAt( i )->Unknown450580( levelModel, collisionModel, damage, unknown, glowNames, glowImages );
		if ( g_kUtopiaModule.GetNetwork() != NULL ) {
			g_kUtopiaModule.GetNetwork()->Update();
		}
		lod->AttachChild( levelModel, false );
		collisionModels.Add( collisionModel );
		damages.Add( damage );
	}

	// BUG: the ranges are read from the first levels, not from the levels
	// kept past the dropped ones
	unsigned int level = 0;
	float nearDistance = 0.0f;
	for ( ; (int)level < (int)GetNumLevels() - 1 - drop; level++ ) {
		float scale = 1.0f / unknown04;
		float farDistance = GetLevel( level )->distance;
		lod->SetRange( scale * nearDistance, scale * farDistance, level );
		nearDistance = farDistance;
	}
	float scale = 1.0f / unknown04;
	lod->SetRange( scale * nearDistance, GetLevel( GetNumLevels() - 1 )->distance * scale, level );

	lod->SetScale( unknown04 > 0.0f ? unknown04 : -unknown04 );
	lod->Update( 0.0f );
	lod->UpdateProperties();
	lod->UpdateEffects();

	int levels = GetNumLevels() - drop;
	if ( unknown58 < levels ) {
		unknown1B0 = unknown58;
	} else {
		unknown1B0 = levels - 1;
	}
	unknown1B4 = unknown54;
}

/*
================
LODModel::Unrefer

Takes a user off the model.
================
*/
// FUNCTION: STBC 0x00451ee0
void LODModel::Unrefer( void ) {
	users--;
}

/*
================
LODModel::ClearUsers
================
*/
// FUNCTION: STBC 0x00451ef0
void LODModel::ClearUsers( void ) {
	users = 0;
}

/*
================
LODModel::Refer
================
*/
// FUNCTION: STBC 0x00451f00
void LODModel::Refer( void ) {
	users++;
}

/*
================
LODModel::WriteToStream
================
*/
// FUNCTION: STBC 0x00451f10
bool LODModel::WriteToStream( TGStream *stream ) {
	stream->WriteFloat( unknown04 );
	stream->WriteInt( unknown54 );
	stream->WriteLong( unknown58 );
	stream->WriteLong( maxDropLODLevel );
	stream->VirtualSlot11( textureSharePath );
	stream->WriteLong( parts.GetSize() );
	for ( unsigned int i = 0; i < parts.GetSize(); i++ ) {
		parts.GetAt( i )->WriteToStream( stream );
	}
	return true;
}

/*
================
LODModel::ReadFromStream

Reads the settings and the levels, then loads the levels.
================
*/
// FUNCTION: STBC 0x00451f80
bool LODModel::ReadFromStream( TGStream *stream ) {
	unknown04 = stream->ReadFloat();
	unknown54 = stream->ReadInt();
	unknown58 = stream->ReadLong();
	maxDropLODLevel = stream->ReadLong();
	textureSharePath = stream->VirtualSlot10( -1 );
	int count = stream->ReadLong();
	if ( count > 0 ) {
		parts.SetSize( count );
		for ( int i = 0; i < count; i++ ) {
			LODModelPart *part = new LODModelPart;
			part->ReadFromStream( stream );
			part->textureSharePath = textureSharePath;
			parts.SetAt( i, part );
		}
	}
	Load();
	return true;
}

/*
================
LODModelManager::LODModelManager
================
*/
// FUNCTION: STBC 0x004520e0
LODModelManager::LODModelManager() {
	glowMapsEnabled = true;
	dropLODLevel = 0;
}

/*
================
LODModelManager::~LODModelManager
================
*/
// FUNCTION: STBC 0x004521a0
LODModelManager::~LODModelManager() {
	ForgetAll();
}

/*
================
LODModelManager::Purge

Deletes the loaded models nothing uses, then the model manager's.
================
*/
// FUNCTION: STBC 0x00452320 ?Purge@LODModelManager@@QAEXXZ
void LODModelManager::Purge( void ) {
	NiTMapIterator pos = unusedModels.GetFirstPos();
	while ( pos ) {
		const char *name;
		LODModel *model;
		unusedModels.GetNext( pos, name, model );
		delete model;
	}
	unusedModels.RemoveAll();
	g_kModelManager.Purge();
}

/*
================
LODModelManager::ForgetAll

Deletes every model, the used ones too.
================
*/
// FUNCTION: STBC 0x00452400 ?ForgetAll@LODModelManager@@QAEXXZ
void LODModelManager::ForgetAll( void ) {
	NiTMapIterator pos = models.GetFirstPos();
	while ( pos ) {
		const char *name;
		LODModel *model;
		models.GetNext( pos, name, model );
		delete model;
	}
	models.RemoveAll();
	Purge();
}

/*
================
LODModelManager::Unrefer

Takes a user off the model; one nothing uses any more moves to the unused
models, where Purge deletes it.
================
*/
// FUNCTION: STBC 0x004524e0 ?Unrefer@LODModelManager@@QAEXPAD@Z
void LODModelManager::Unrefer( char *name ) {
	LODModel *model;
	if ( models.GetAt( name, model ) ) {
		model->Unrefer();
		if ( model->users < 1 ) {
			unusedModels.SetAt( name, model );
			models.RemoveAt( name );
		}
	}
}

/*
================
LODModelManager::Contains
================
*/
// FUNCTION: STBC 0x00452650 ?Contains@LODModelManager@@QAE_NPAD@Z
bool LODModelManager::Contains( char *name ) {
	LODModel *model;
	if ( models.GetAt( name, model ) ) {
		return true;
	}
	if ( unusedModels.GetAt( name, model ) ) {
		return true;
	}
	return false;
}

/*
================
LODModelManager::GetModel
================
*/
// FUNCTION: STBC 0x004526d0
LODModel *LODModelManager::GetModel( char *name ) {
	LODModel *model = NULL;
	if ( !models.GetAt( name, model ) && unusedModels.GetAt( name, model ) ) {
		models.SetAt( name, model );
		unusedModels.RemoveAt( name );
	}
	return model;
}

/*
================
LODModelManager::Create
================
*/
// FUNCTION: STBC 0x00452880
LODModel *LODModelManager::Create( char *name, int unknown54, int maxDropLODLevel, int unknown58, float unknown04,
								   char *unused ) {
	LODModel *model = new LODModel( unknown04, unknown54, maxDropLODLevel, unknown58 );
	models.SetAt( name, model );
	return model;
}


/*
================
LODModelManager::AreGlowMapsEnabled
================
*/
// FUNCTION: STBC 0x00452a40
bool LODModelManager::AreGlowMapsEnabled( void ) {
	return glowMapsEnabled;
}

/*
================
LODModelManager::SetGlowMapsEnabled
================
*/
// FUNCTION: STBC 0x00452a50
void LODModelManager::SetGlowMapsEnabled( bool enabled ) {
	if ( glowMapsEnabled != enabled ) {
		glowMapsEnabled = enabled;
		UpdateGlowMaps( enabled );
	}
}

/*
================
LODModelManager::AreSpecularMapsEnabled
================
*/
// FUNCTION: STBC 0x00452a70
bool LODModelManager::AreSpecularMapsEnabled( void ) {
	return specularMapsEnabled;
}

/*
================
LODModelManager::SetSpecularMapsEnabled
================
*/
// FUNCTION: STBC 0x00452a80
void LODModelManager::SetSpecularMapsEnabled( bool enabled ) {
	specularMapsEnabled = enabled;
}

/*
================
LODModelManager::IsMotionBlurEnabled
================
*/
// FUNCTION: STBC 0x00452a90
bool LODModelManager::IsMotionBlurEnabled( void ) {
	return motionBlurEnabled;
}

/*
================
LODModelManager::SetMotionBlurEnabled
================
*/
// FUNCTION: STBC 0x00452aa0
void LODModelManager::SetMotionBlurEnabled( bool enabled ) {
	motionBlurEnabled = enabled;
}

/*
================
LODModelManager::SetDropLODLevel
================
*/
// FUNCTION: STBC 0x00452ab0
void LODModelManager::SetDropLODLevel( int level ) {
	dropLODLevel = level;
}

/*
================
LODModelManager::GetDropLODLevel
================
*/
// FUNCTION: STBC 0x00452ac0
int LODModelManager::GetDropLODLevel( void ) {
	return dropLODLevel;
}

/*
================
LODModelManager::UpdateGlowMaps

Turns the glow alpha maps of every damageable object on or off.
================
*/
// FUNCTION: STBC 0x00452ad0
void LODModelManager::UpdateGlowMaps( bool enabled ) {
	NiTMap<long, TGObject *> *objects = TGObject::objectTable;
	NiTMapIterator pos = objects->GetFirstPos();
	while ( pos ) {
		long id;
		TGObject *object;
		objects->GetNext( pos, id, object );
		if ( object != NULL && object->IsTypeOf( DAMAGEABLEOBJECT_TYPE ) ) {
			if ( enabled ) {
				( (DamageableObject *)object )->EnableGlowAlphaMaps();
			} else {
				( (DamageableObject *)object )->DisableGlowAlphaMaps();
			}
		}
	}
}

/*
================
LODModelManager::WriteToStream
================
*/
// FUNCTION: STBC 0x00452b70
bool LODModelManager::WriteToStream( TGStream *stream ) {
	const char *name;
	LODModel *model;

	NiTMapIterator pos = models.GetFirstPos();
	while ( pos != NULL ) {
		stream->WriteBool( true );
		models.GetNext( pos, name, model );
		stream->VirtualSlot11( (char *)name );
		model->WriteToStream( stream );
	}
	stream->WriteBool( false );

	pos = unusedModels.GetFirstPos();
	while ( pos != NULL ) {
		stream->WriteBool( true );
		unusedModels.GetNext( pos, name, model );
		stream->VirtualSlot11( (char *)name );
		model->WriteToStream( stream );
	}
	stream->WriteBool( false );
	return true;
}

/*
================
LODModelManager::ReadFromStream
================
*/
// FUNCTION: STBC 0x00452cb0
bool LODModelManager::ReadFromStream( TGStream *stream ) {
	while ( stream->ReadBool() == true ) {
		char *name = stream->VirtualSlot10( -1 );
		LODModel *model = new LODModel;
		model->ReadFromStream( stream );
		models.SetAt( name, model );
	}
	while ( stream->ReadBool() == true ) {
		char *name = stream->VirtualSlot10( -1 );
		LODModel *model = new LODModel;
		model->ReadFromStream( stream );
		unusedModels.SetAt( name, model );
	}
	return true;
}

/*
================
EnableSpecularMaps

Gives every textured object in the tree whose image file name has the
filter in it (every one for NULL) a gloss (specular) map, in a multi-texture
property it gets when it has none. The map's image is the texture's file
with the specular suffix before the extension; failing that, the file with
the glow suffix replaced by the specular suffix; failing both, the texture's
own image. The LOD loader (0x450580) passes its part's 0x0C, specular and
glow suffixes. The name is invented, after DisableSpecularMaps.
================
*/
// FUNCTION: STBC 0x00452f50
void EnableSpecularMaps( NiAVObject *object, const char *filter, const char *specularSuffix, const char *glowSuffix ) {
	if ( object == NULL ) {
		return;
	}

	NiTextureProperty *texture = (NiTextureProperty *)(NiProperty *)object->GetProperty( NiTextureProperty::TEXTURE );
	if ( texture != NULL && texture->GetImage() != NULL && texture->GetImage()->GetURL() != NULL
		 && ( filter == NULL || strstr( texture->GetImage()->GetURL(), filter ) != NULL ) ) {
		NiMultiTextureProperty *multiTexture = NiDynamicCast( NiMultiTextureProperty, object->GetProperty( NiMultiTextureProperty::MULTI_TEXTURE ) );
		if ( multiTexture == NULL ) {
			multiTexture = new NiMultiTextureProperty;
			object->AttachProperty( multiTexture );
		}

		NiPointer<NiImage> image;
		if ( g_kLODModelManager.AreSpecularMapsEnabled() && specularSuffix != NULL ) {
			char extension[16];
			char name[MAX_PATH];
			strcpy( name, texture->GetImage()->GetURL() );
			char *dot = strrchr( name, '.' );
			if ( dot != NULL ) {
				strcpy( extension, dot );
				*dot = '\0';
			}
			strcat( name, specularSuffix );
			if ( dot != NULL ) {
				strcat( name, extension );
			}
			image = g_kImageManager.GetImage( name, true, true );

			if ( image == NULL && glowSuffix != NULL ) {
				strcpy( name, texture->GetImage()->GetURL() );
				char *glow = strstr( name, glowSuffix );
				if ( glow != NULL ) {
					dot = strrchr( name, '.' );
					if ( dot != NULL ) {
						strcpy( extension, dot );
					}
					*glow = '\0';
					strcat( name, specularSuffix );
					if ( dot != NULL ) {
						strcat( name, extension );
					}
				}
				image = g_kImageManager.GetImage( name, true, true );
			}
		}
		if ( image == NULL ) {
			image = texture->GetImage();
		}

		multiTexture->SetMap( NiMultiTextureProperty::GLOSS_INDEX,
			new NiMultiTextureProperty::Map( image, 1, NiTextureModeProperty::WRAP_S_WRAP_T, NiTextureModeProperty::FILTER_BILERP ) );
	}

	NiNode *node = NiDynamicCast( NiNode, object );
	if ( node != NULL ) {
		for ( unsigned int i = 0; i < node->GetArrayCount(); i++ ) {
			EnableSpecularMaps( node->GetAt( i ), filter, specularSuffix, glowSuffix );
		}
	}
}

/*
================
DisableSpecularMaps

Drops the gloss (specular) map of every multi-texture property in the tree;
the counterpart of the specular map set-up (0x452F50). Nothing calls it. The
name is invented.
================
*/
// FUNCTION: STBC 0x00453530
void DisableSpecularMaps( NiAVObject *object ) {
	if ( object == NULL ) {
		return;
	}

	NiMultiTextureProperty *property = NiDynamicCast( NiMultiTextureProperty, object->GetProperty( NiMultiTextureProperty::MULTI_TEXTURE ) );
	if ( property != NULL ) {
		property->RemoveMap( NiMultiTextureProperty::GLOSS_INDEX );
	}

	NiNode *node = NiDynamicCast( NiNode, object );
	if ( node != NULL ) {
		for ( unsigned int i = 0; i < node->GetArrayCount(); i++ ) {
			DisableSpecularMaps( node->GetAt( i ) );
		}
	}
}
