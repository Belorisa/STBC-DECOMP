/*
===============================================================================

	PhaserProperty.h

	The property of a phaser bank: its firing arcs, its orientation, and
	the shape, colours and texture of the beam it draws.

	0x148 bytes: the SWIG wrapper new_PhaserProperty allocates that much.

	Evidence: the SWIG wrappers PhaserProperty_* read and write the arcs
	(0x94-0xA0), the phaser texture start and end (0xC8, 0xCC), the phaser
	width (0xD0), the number of sides (0x114), the beam's radii, scale,
	taper and tiling (0x118-0x138), the width and length (0x140, 0x144)
	inline, read the texture name (0x13C), and copy the orientation
	(forward 0xA4, up 0xB0; SetOrientation also sets 0xBC) and the four colours (0xD4, 0xE4, 0xF4,
	0x104). The virtual functions are not declared yet.

===============================================================================
*/

#ifndef __PHASERPROPERTY_H__
#define __PHASERPROPERTY_H__

#include "EnergyWeaponProperty.h"
#include "NiColor.h"
#include "TGPoint3.h"

class TGModelProperty;

// The marker that ends a PhaserProperty's stream record (invented name).
#define PHASERPROPERTY_STREAM_END		45

// The type PhaserProperty::Cast (0x685800) checks.
#define PHASERPROPERTY_TYPE		0x8132

// FUNCTION: STBC 0x00685cb0 ??_GPhaserProperty@@UAEPAXI@Z
// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
class PhaserProperty : public EnergyWeaponProperty {
public:
					PhaserProperty( long id = 0 );
	virtual			~PhaserProperty( void );

	// TGObject
	// FUNCTION: STBC 0x00685b20 ?GetObjType@PhaserProperty@@UAEHXZ
	virtual int		GetObjType( void ) { return PHASERPROPERTY_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	virtual bool	FixUpPointers( void );
	virtual void	PostFixUp( void );
	// FUNCTION: STBC 0x00685b80 ?GetClassName@PhaserProperty@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "PhaserProperty"; }
	// FUNCTION: STBC 0x00685b90 ?GetSwigTypeName@PhaserProperty@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_PhaserProperty"; }
	// FUNCTION: STBC 0x00685ba0 ?GetPythonClassName@PhaserProperty@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "PhaserPropertyPtr"; }

	// TGModelProperty
	virtual void	WriteScript( TGStream *stream );
	virtual void	WriteInit( TGStream *stream, char *variableName );
	virtual void	VirtualSlot15( bool show, ModelPropertyViewer *viewer, NiNode *parent );
	virtual bool	Copy( TGModelProperty *property );

	static PhaserProperty *	Cast( TGModelProperty * );
	static PhaserProperty *	Create( char * );
	void	SetTextureName( char * );

	// Sets the forward and up axes; right is their cross product.
	void			SetOrientation( const NiPoint3 &forward, const NiPoint3 &up ) { orientationForward = forward; orientationUp = up; orientationRight = orientationForward.Cross( orientationUp ); }
	void			SetOrientation( const TGPoint3 &forward, const TGPoint3 &up ) { SetOrientation( (NiPoint3)forward, (NiPoint3)up ); }
	void			SetArcWidthAngles( float min, float max ) { arcWidthAngleMin = min; arcWidthAngleMax = max; }
	void			SetArcHeightAngles( float min, float max ) { arcHeightAngleMin = min; arcHeightAngleMax = max; }
	void			GetArcWidthAngles( float *min, float *max ) { *min = arcWidthAngleMin; *max = arcWidthAngleMax; }
	void			GetArcHeightAngles( float *min, float *max ) { *min = arcHeightAngleMin; *max = arcHeightAngleMax; }
	NiColorA		GetOuterShellColor( void ) { return outerShellColor; }
	NiColorA		GetInnerShellColor( void ) { return innerShellColor; }
	NiColorA		GetOuterCoreColor( void ) { return outerCoreColor; }
	NiColorA		GetInnerCoreColor( void ) { return innerCoreColor; }
	// The copies kept out of line are the SWIG wrappers' (Copy calls them).
	// FUNCTION: STBC 0x0063d620 ?SetOuterShellColor@PhaserProperty@@QAEXVNiColorA@@@Z
	void			SetOuterShellColor( NiColorA color ) { outerShellColor = color; }
	// FUNCTION: STBC 0x0063d710 ?SetInnerShellColor@PhaserProperty@@QAEXVNiColorA@@@Z
	void			SetInnerShellColor( NiColorA color ) { innerShellColor = color; }
	// FUNCTION: STBC 0x0063d800 ?SetOuterCoreColor@PhaserProperty@@QAEXVNiColorA@@@Z
	void			SetOuterCoreColor( NiColorA color ) { outerCoreColor = color; }
	// FUNCTION: STBC 0x0063d8f0 ?SetInnerCoreColor@PhaserProperty@@QAEXVNiColorA@@@Z
	void			SetInnerCoreColor( NiColorA color ) { innerCoreColor = color; }
	void			SetWidth( float width_ ) { width = width_; }
	void			SetLength( float length_ ) { length = length_; }
	void			SetPhaserTextureStart( int start ) { phaserTextureStart = start; }
	void			SetPhaserTextureEnd( int end ) { phaserTextureEnd = end; }
	void			SetPhaserWidth( float width_ ) { phaserWidth = width_; }
	void			SetNumSides( int sides ) { numSides = sides; }
	void			SetMainRadius( float radius ) { mainRadius = radius; }
	void			SetTaperRadius( float radius ) { taperRadius = radius; }
	void			SetCoreScale( float scale ) { coreScale = scale; }
	void			SetTaperRatio( float ratio ) { taperRatio = ratio; }
	void			SetTaperMinLength( float length_ ) { taperMinLength = length_; }
	void			SetTaperMaxLength( float length_ ) { taperMaxLength = length_; }
	void			SetLengthTextureTilePerUnit( float tile ) { lengthTextureTilePerUnit = tile; }
	void			SetPerimeterTile( float tile ) { perimeterTile = tile; }
	void			SetTextureSpeed( float speed ) { textureSpeed = speed; }
	float			GetWidth( void ) { return width; }
	float			GetLength( void ) { return length; }
	int				GetPhaserTextureStart( void ) { return phaserTextureStart; }
	int				GetPhaserTextureEnd( void ) { return phaserTextureEnd; }
	float			GetPhaserWidth( void ) { return phaserWidth; }
	float			GetArcWidthAngleMin( void ) { return arcWidthAngleMin; }
	float			GetArcWidthAngleMax( void ) { return arcWidthAngleMax; }
	float			GetArcHeightAngleMin( void ) { return arcHeightAngleMin; }
	float			GetArcHeightAngleMax( void ) { return arcHeightAngleMax; }
	int				GetNumSides( void ) { return numSides; }
	float			GetMainRadius( void ) { return mainRadius; }
	float			GetTaperRadius( void ) { return taperRadius; }
	float			GetCoreScale( void ) { return coreScale; }
	float			GetTaperRatio( void ) { return taperRatio; }
	float			GetTaperMinLength( void ) { return taperMinLength; }
	float			GetTaperMaxLength( void ) { return taperMaxLength; }
	float			GetLengthTextureTilePerUnit( void ) { return lengthTextureTilePerUnit; }
	float			GetPerimeterTile( void ) { return perimeterTile; }
	float			GetTextureSpeed( void ) { return textureSpeed; }
	char *			GetTextureName( void ) { return textureName; }
	TGPoint3		GetOrientationForward( void );
	TGPoint3		GetOrientationUp( void );

protected:
	// Builds the display model's geometry under the root node (unidentified).
	void			Unknown6866F0( NiNode *root );
	// The display model's pieces and their updates (unidentified).
	NiAVObject *	Unknown686960( float radius );
	NiAVObject *	Unknown686F00( float size );
	NiAVObject *	Unknown687730( float size );
	void			Unknown686C80( float radius );
	void			Unknown687350( float size );
	void			Unknown687A60( float size );

	NiNode *		debugNode;				// 0x90 built by slot 15, as SubsystemProperty's; NULL until then; saved as its address and resolved on load (invented name)
	float			arcWidthAngleMin;		// 0x94
	float			arcWidthAngleMax;		// 0x98
	float			arcHeightAngleMin;		// 0x9C
	float			arcHeightAngleMax;		// 0xA0
	NiPoint3		orientationForward;		// 0xA4
	NiPoint3		orientationUp;			// 0xB0
	NiPoint3		orientationRight;		// 0xBC SetOrientation derives it (the name follows TractorBeamProperty_GetOrientationRight)
	int				phaserTextureStart;		// 0xC8
	int				phaserTextureEnd;		// 0xCC
	float			phaserWidth;			// 0xD0
	NiColorA		outerShellColor;		// 0xD4
	NiColorA		innerShellColor;		// 0xE4
	NiColorA		outerCoreColor;			// 0xF4
	NiColorA		innerCoreColor;			// 0x104
	int				numSides;				// 0x114
	float			mainRadius;				// 0x118
	float			taperRadius;			// 0x11C
	float			coreScale;				// 0x120
	float			taperRatio;				// 0x124
	float			taperMinLength;			// 0x128
	float			taperMaxLength;			// 0x12C
	float			lengthTextureTilePerUnit;	// 0x130
	float			perimeterTile;			// 0x134
	float			textureSpeed;			// 0x138
	char *			textureName;			// 0x13C the texture's name (SetTextureName sets it)
	float			width;					// 0x140
	float			length;					// 0x144
};

/*
================
PhaserProperty::GetOrientationForward
================
*/
// FUNCTION: STBC 0x004e74e0
inline TGPoint3 PhaserProperty::GetOrientationForward( void ) {
	return orientationForward;
}

/*
================
PhaserProperty::GetOrientationUp
================
*/
// FUNCTION: STBC 0x004e7510
inline TGPoint3 PhaserProperty::GetOrientationUp( void ) {
	return orientationUp;
}

/*
================
PhaserProperty::IsTypeOf
================
*/
// FUNCTION: STBC 0x00685b30
inline bool PhaserProperty::IsTypeOf( int type ) {
	if ( type == PHASERPROPERTY_TYPE ) {
		return true;
	}
	return EnergyWeaponProperty::IsTypeOf( type );
}

#endif /* !__PHASERPROPERTY_H__ */
