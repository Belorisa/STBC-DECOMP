/*
===============================================================================

	LODModelManager.h

	The members of LODModelManager that the SWIG wrappers LODModelManager_* call.
	g_kLODModelManager (0x980798) is the one instance. The accessors (0x452A40
	to 0x452AC0) give the glow-map flag at 0x00, the dropped LOD level at 0x04
	and the specular-map and motion-blur flags, which are statics of the
	class (0x8E103C, 0x8E103D, both set at start). Nothing past 0x08 is
	modelled.

===============================================================================
*/

#ifndef __LODMODELMANAGER_H__
#define __LODMODELMANAGER_H__

#include "NiTStringMap.h"

class LODModel;
class TGStream;

class LODModelManager {
public:
			LODModelManager();
			~LODModelManager();

	bool	Contains( char * );
	// The model of the name; an unused one moves back to the models with
	// users. NULL when there is none (invented name).
	LODModel *	GetModel( char *name );
	void	Unrefer( char * );
	void	ForgetAll( void );
	void	Purge( void );
	bool	AreGlowMapsEnabled( void );
	void	SetGlowMapsEnabled( bool );
	bool	AreSpecularMapsEnabled( void );
	void	SetSpecularMapsEnabled( bool );
	bool	IsMotionBlurEnabled( void );
	void	SetMotionBlurEnabled( bool );
	void	SetDropLODLevel( int );

	// The models and their names, for the game's saves: each with a true
	// flag, then a false flag after each map (names invented).
	bool	WriteToStream( TGStream *stream );
	bool	ReadFromStream( TGStream *stream );
	int	GetDropLODLevel( void );

private:
	// Turns every damageable object's glow alpha maps on or off (0x452AD0;
	// invented name).
	void	UpdateGlowMaps( bool enabled );

	bool	glowMapsEnabled;			// 0x00
	int		dropLODLevel;				// 0x04 (SetDropLODLevel, GetDropLODLevel)
	NiTStringMap<LODModel *>	models;			// 0x08 the models with users, by name (invented name)
	NiTStringMap<LODModel *>	unusedModels;	// 0x18 the loaded models nothing uses, by name (invented name)

public:
	// public: the pass nodes read them directly (SpecularPass 0x4EDCAD,
	// MotionBlurPass 0x4EE65B)
	static bool	specularMapsEnabled;
	static bool	motionBlurEnabled;

	// Declared for the SWIG wrappers that call them.
	// Makes a model and files it under the name (the SWIG wrapper's
	// defaults: 0, 10000, 10000, 0.01, NULL); the last argument is unused.
	LODModel *	Create( char *name, int unknown54, int maxDropLODLevel, int unknown58, float unknown04, char *unused );
};

extern LODModelManager	g_kLODModelManager;

#endif /* !__LODMODELMANAGER_H__ */
