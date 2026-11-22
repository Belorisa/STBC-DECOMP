/*
===============================================================================

	SunEffect.h

	The model of a sun: a textured sphere that turns, its glow shells and
	the flares that leave its surface. Only the builder is a member; the
	SWIG interface wraps it as SunEffect_Create (0x6202A0), and Sun builds
	its model with it. The builder's code lies in FlareController's unit.

===============================================================================
*/

#ifndef __SUNEFFECT_H__
#define __SUNEFFECT_H__

class NiAVObject;

class SunEffect {
public:
	// builds a sun of radius from the base and flare textures (the
	// defaults when NULL), its glow shells and flares reaching out to
	// outerRadius (parameter names invented)
	static NiAVObject *	Create( char *baseTexture, char *flareTexture, float outerRadius, float radius );

	// TODO identity: a unit sphere of the texture, rings * 8 by rings * 16
	// vertices, the texture tiled uTile by vTile; a TGFuzzyTriShape with
	// white vertex colours when fuzzy, a lit NiTriShape otherwise (0x4F06D0;
	// only Create calls it: the class, name and parameters are invented)
	static NiAVObject *	CreateSphere( int rings, char *texture, bool fuzzy, float uTile, float vTile );
};

#endif /* !__SUNEFFECT_H__ */
