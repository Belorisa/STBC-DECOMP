/*
===============================================================================

	ShieldPatchParticles.h

	The shield hit effect: patches of the shield bubble lit where weapons
	hit a ship, and the effect of a ship diving into a planet's atmosphere.

	The unit's code is 0x4E9C00-0x4EC650: the static initialiser of the
	NiRTTI of ShieldPatchParticles::PatchInstance (0x9865C8, named
	"ShieldPatchParticles::PatchInstance" at 0x8E1018, NiTriShape's NiRTTI
	as its base, built at 0x4E9C10), then:

	- ShieldPatchParticles (vtable 0x88D65C: NiNode's 44 slots with the
	  destructor 0x4E9F80 and AttachChild 0x4EA150 replaced; no NiRTTI of
	  its own). The name is taken from the NiRTTI name of its nested class;
	  ShipClass::WeaponHitHandler allocates its 0x104 bytes;
	- ShieldPatchParticles::PatchInstance (vtable 0x88D70C: NiTriShape's 51
	  slots with GetRTTI 0x4EC8D0, the destructor 0x4EA2F0 and
	  UpdateDownwardPass 0x4EA3E0 replaced), 0x108 bytes;
	- AtmosphereEffect (vtable 0x88D7D8: NiNode's 44 slots with the
	  destructor 0x4EAE70 and UpdateDownwardPass 0x4EAF40 replaced), 0xF0
	  bytes, which ShipClass::Unknown5B2BF0 builds from
	  "data/Textures/AtmosphereEffectShockWave.tga" and
	  "data/Textures/AtmosphereEffect.tga". Nothing names the class or its
	  two parts: AtmosphereEffect, AtmosphereShockWave and AtmosphereStreaks
	  are invented names after the texture files.

===============================================================================
*/

#ifndef __SHIELDPATCHPARTICLES_H__
#define __SHIELDPATCHPARTICLES_H__

#include "NiNode.h"
#include "NiTriShape.h"
#include "NiColor.h"

class NiImage;
class TGStream;

/*
===============================================================================

	ShieldPatchParticles

	The node that owns a ship's lit shield patches.

===============================================================================
*/
// FUNCTION: STBC 0x004e9f80 ??_GShieldPatchParticles@@UAEPAXI@Z
// FUNCTION: STBC 0x004e9fa0 ??1ShieldPatchParticles@@UAE@XZ
class ShieldPatchParticles : public NiNode {
public:
	/*
	================
	PatchInstance

	One lit patch: a piece of a unit sphere around the hit direction,
	scaled to the ship's box, with random vertex alphas that fade out.
	================
	*/
	// FUNCTION: STBC 0x004ea2f0 ??_GPatchInstance@ShieldPatchParticles@@UAEPAXI@Z
	class PatchInstance : public NiTriShape {
	public:
		// the patch keeps the addresses of its owner's extent and color
							PatchInstance( const NiPoint3 &extent_, const NiColorA &color_ );
		virtual				~PatchInstance();

		// FUNCTION: STBC 0x004ec8d0 ?GetRTTI@PatchInstance@ShieldPatchParticles@@UBEPBVNiRTTI@@XZ
		virtual const NiRTTI *	GetRTTI() const { return &rtti; }

		virtual void		UpdateDownwardPass( float time );

		// the names of the members below are invented
		void				Hide();
		// lights the patch at the time around the direction
		void				Show( float time, const NiPoint3 &direction_, unsigned int size_, float scale_ );
		bool				Save( TGStream *stream );
		bool				Load( TGStream *stream );

		static const NiRTTI	rtti;

		float				startTime;		// 0xE4 when the patch was lit
		NiPoint3			direction;		// 0xE8 the patch's center on the unit sphere

	protected:
		// turns the unit patch to its direction and scales it to the extent
		void				Orient();
		void				GetGridSize( int &columns, int &rows );
		// builds the unit patch for size and scale
		void				BuildGeometry();

		const NiPoint3 &	extent;			// 0xF4 the owner's
		const NiColorA &	color;			// 0xF8 the owner's
		unsigned int		size;			// 0xFC the number of triangles asked for
		float				scale;			// 0x100 the part of the sphere the patch covers
		NiPoint3 *			baseVertices;	// 0x104 the unit patch's vertices, before Orient
	};

	// The bubble of the given color around the box (extent, center),
	// textured with the image (parameter names invented).
					ShieldPatchParticles( const NiColorA &color, const NiPoint3 &extent, const NiPoint3 &center, NiImage *image );

	// Lights a patch facing the direction at the time, of the size (the
	// effect level's texels) and scale; a free patch is reused, else a
	// new one is attached (invented name).
	void			Unknown4E9FB0( float time, NiPoint3 direction, int size, float scale );
	// Clears the patches that face the direction (invented name).
	void			Unknown4EA170( const NiPoint3 &direction );

	// Nothing calls the two (invented names).
	bool			Save( TGStream *stream );
	bool			Load( TGStream *stream );

	// NiNode
	virtual void	AttachChild( NiAVObject *child, bool firstAvail );

protected:
	NiPoint3		extent;				// 0xE8 the box's half size (invented name)
	NiColorA		color;				// 0xF4 the glow color (invented name)
};

/*
===============================================================================

	AtmosphereShockWave, AtmosphereStreaks

	The two shapes of the atmosphere effect (invented names, as are their
	members'): each keeps what the effect was last given and refills its
	shape's vertices, colors and texture coordinates from it every frame.

===============================================================================
*/
class AtmosphereShockWave {
public:
							AtmosphereShockWave( float time_, float intensity_, const NiPoint3 &extent_, NiImage *image, unsigned int textureTurns_, unsigned int count );

	void					Update( float time_ );
	void					Set( float time_, float intensity_, const NiPoint3 &extent_ );

	float					time;			// 0x00
	float					intensity;		// 0x04
	NiPoint3				extent;			// 0x08
	int						columns;		// 0x14
	int						rows;			// 0x18
	unsigned int			textureTurns;	// 0x1C how often the texture goes around the cap; 1
	NiPointer<NiTriShape>	shape;			// 0x20

protected:
	float					GetScale();
	NiColorA				GetColor();
	void					Fill( float scale, float angle, const NiColorA &color, NiPoint3 *vertices, NiColorA *colors, NiPoint2 *textures );
};

class AtmosphereStreaks {
public:
							AtmosphereStreaks( float time_, float intensity_, const NiPoint3 &extent_, NiImage *image, unsigned int textureTurns_, int tailRows_, unsigned int count );

	void					Set( float time_, float intensity_, const NiPoint3 &extent_ );
	void					Update( float time_ );

	float					time;			// 0x00
	float					intensity;		// 0x04
	NiPoint3				extent;			// 0x08
	int						columns;		// 0x14
	int						rows;			// 0x18
	unsigned int			textureTurns;	// 0x1C how often the texture goes around the rings; 1
	int						tailRows;		// 0x20 the rings behind the cap, less one; 3
	NiPointer<NiTriShape>	shape;			// 0x24

protected:
	void					Fill( float scale, float angle, float length, const NiColorA &color, NiPoint3 *vertices, NiColorA *colors, NiPoint2 *textures );
	float					GetScale();
	float					GetLength();
	NiColorA				GetColor();
};

/*
===============================================================================

	AtmosphereEffect

	The glow around a ship in a planet's atmosphere (invented name).

===============================================================================
*/
// FUNCTION: STBC 0x004eae70 ??_GAtmosphereEffect@@UAEPAXI@Z
class AtmosphereEffect : public NiNode {
public:
							AtmosphereEffect( float time, float intensity, const NiPoint3 &extent, NiImage *shockWaveImage, NiImage *image, unsigned int count );
	virtual					~AtmosphereEffect();

	virtual void			UpdateDownwardPass( float time );

	// what ShipClass::Unknown5B2BF0 gives the effect on every hit after
	// the first (invented name)
	void					Set( float time, float intensity, const NiPoint3 &extent );

protected:
	AtmosphereShockWave *	shockWave;		// 0xE8
	AtmosphereStreaks *		streaks;		// 0xEC
};

#endif /* !__SHIELDPATCHPARTICLES_H__ */
