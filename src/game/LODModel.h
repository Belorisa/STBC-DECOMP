/*
===============================================================================

	LODModel.h

	A ship's or station's model at several levels of detail: the scripts
	make one through LODModelManager::Create, add a level per model file
	(AddLOD), the glow textures (AddGlowPassTexture), and load the files
	(Load, LoadIncremental). The SWIG wrappers LODModel_* call the methods.

	0x78 bytes (LODModelManager::Create, 0x452880, allocates it and inlines
	the constructor): the users, a float and three ints of Create's
	arguments, the shared texture path, the glow textures' geometry names
	and image files (two NiTArray<char *>), their images (an
	NiTArray<NiPointer<NiImage>>) and the levels (an NiTArray of
	LODModelPart pointers). The destructor (0x451580) deletes the levels,
	the strings and the path.

===============================================================================
*/

#ifndef __LODMODEL_H__
#define __LODMODEL_H__

#include "NiPointer.h"
#include "NiTArray.h"

class NiAVObject;
class NiImage;
class OBBModel;
class TGStream;
class VisibleDamage;

/*
================
LODModelPart

One level of detail: the model file, the voxel file made from its name
(the file name with "_vox" before the extension), the texture suffixes
and damage textures AddLOD gives, the shared texture path and five floats
and an int of AddLOD's arguments. 0x54 bytes (AddLOD allocates it). The
class name is invented, and so are the member names unless noted.
================
*/
class LODModelPart {
public:
							LODModelPart();
							LODModelPart( char *file_, int unknown4C_, float distance_, float unknown38_, float unknown3C_,
										  float unknown40_, float unknown44_, char *glowSuffix_, char *unknown0C_,
										  char *specularSuffix_, char *damageTexture_, char *unknown18_,
										  char *damageMap_, char *unknown24_, char *damageTextures_,
										  char *unknown2C_, char *unknown30_ );
							~LODModelPart();

	// reads the model file (and the voxel file, when unknown3C is
	// positive) through the model manager, once
	void					Load();
	void					LoadIncremental();

	// the voxel file of the model file (a new heap string)
	char *					GetVoxelFileName();

	// Builds the level's model (a clone of the model file's, with its glow
	// passes, specular maps, a specular and a motion blur pass), its
	// collision model when 0x4C is positive and its visible damage, making
	// and saving the voxel file first when there is none. LODModel's
	// Unknown451AC0 calls it per level with the model's glow names and
	// images; the float is its last argument. The body (0x450580) waits on
	// BinaryVoxelizer's steps; the name is a placeholder.
	void					Unknown450580( NiPointer<NiAVObject> &model, NiPointer<OBBModel> &collisionModel,
										   VisibleDamage *&damage, float unknown, NiTArray<char *> &glowNames,
										   NiTArray< NiPointer<NiImage> > &glowImages );

	bool					ReadFromStream( TGStream *stream );
	bool					WriteToStream( TGStream *stream );

	char *					file;				// 0x00 the model file
	char *					voxelFile;			// 0x04
	char *					glowSuffix;			// 0x08 "_glow" in the scripts
	char *					unknown0C;			// 0x0C
	char *					specularSuffix;		// 0x10 "_specular" in the scripts
	char *					damageTexture;		// 0x14 "data/Textures/Effects/Damage.tga" by default
	char *					unknown18;			// 0x18
	char *					unknown1C;			// 0x1C never set
	char *					damageMap;			// 0x20 "data/Damage1.tga" by default
	char *					unknown24;			// 0x24
	char *					damageTextures;		// 0x28 "DamageTextures" by default
	char *					unknown2C;			// 0x2C
	char *					unknown30;			// 0x30
	char *					textureSharePath;	// 0x34 the model's (not owned)
	float					unknown38;			// 0x38
	float					unknown3C;			// 0x3C positive: the level has a voxel file
	float					unknown40;			// 0x40
	float					unknown44;			// 0x44
	float					distance;			// 0x48 the level's switch distance
	int						unknown4C;			// 0x4C
	bool					loaded;				// 0x50
};

class LODModel {
public:
	// LODModelManager::Create's arguments (Create inlines it)
			LODModel( float unknown04_, int unknown54_, int maxDropLODLevel_, int unknown58_ ) :
				users( 0 ), unknown04( unknown04_ ), textureSharePath( NULL ), unknown54( unknown54_ ),
				unknown58( unknown58_ ), maxDropLODLevel( maxDropLODLevel_ ) {}
	// a model for ReadFromStream to fill (LODModelManager::ReadFromStream
	// inlines it)
			LODModel() : users( 0 ), unknown04( 1.0f ), textureSharePath( NULL ), unknown54( 0 ), unknown58( 0 ),
				maxDropLODLevel( 0 ) {}
	// The destructor is not inline; the model manager deletes models.
	~LODModel();

	// Takes a user off the model (name invented; 0x451EE0).
	void	Unrefer( void );
	// Adds a user (name invented; Unknown451AC0 calls it).
	void	Refer( void );
	// Forgets the users (name invented; no caller in retail).
	void	ClearUsers( void );

	// The model's settings and levels, for LODModelManager's saves; reading
	// loads the levels (names invented).
	bool	WriteToStream( TGStream *stream );
	bool	ReadFromStream( TGStream *stream );

	int		users;						// 0x00 (invented name)

	void	SetTextureSharePath( char *path );
	void	AddGlowPassTexture( char *name, char *file );
	void	Load( void );
	void	LoadIncremental( void );

	// Builds the model's NiLODNode of its levels (past the dropped LOD
	// level) into model, sizes the collision models and the visible
	// damages to the levels, and gives the two LOD indices
	// DamageableObject::SetupModel keeps (0x1B0, 0x1B4). The last argument
	// is a float, 0 there. Unidentified.
	void	Unknown451AC0( NiPointer<NiAVObject> &model, NiTArray< NiPointer<OBBModel> > &collisionModels, NiTArray<VisibleDamage *> &damages, int &unknown1B0, int &unknown1B4, float );

	void	AddLOD( char *file, int unknown4C, float distance, float unknown38, float unknown3C, float unknown40,
					float unknown44, char *glowSuffix, char *unknown0C, char *specularSuffix, char *damageTexture,
					char *unknown18, char *damageMap, char *unknown24, char *damageTextures, char *unknown2C,
					char *unknown30 );

	// The levels, the most detailed first (invented names).
	unsigned int			GetNumLevels( void ) { return parts.GetSize(); }
	LODModelPart *			GetLevel( unsigned int index ) { return parts.GetAt( index ); }

protected:
	float						unknown04;			// 0x04 Create's float (0.01 by default)
	char *						textureSharePath;	// 0x08 a heap copy of the path other models' textures are shared from (invented name)
	NiTArray<char *>			glowNames;			// 0x0C AddGlowPassTexture's names, heap copies (invented name)
	NiTArray<char *>			glowFiles;			// 0x24 AddGlowPassTexture's image files, heap copies (invented name)
	NiTArray< NiPointer<NiImage> >	glowImages;		// 0x3C AddGlowPassTexture's images (invented name)
	int							unknown54;			// 0x54 Create's first int (0 by default)
	int							unknown58;			// 0x58 Create's third int (10000 by default)
	int							maxDropLODLevel;	// 0x5C Create's second int: the most levels Load drops (invented name)
	NiTArray<LODModelPart *>	parts;				// 0x60 the levels (invented name)
};

#endif /* !__LODMODEL_H__ */
