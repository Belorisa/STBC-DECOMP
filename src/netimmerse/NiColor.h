/*
===============================================================================

	NiColor.h

	RGB and RGBA colours with float channels, nominally in [0, 1].

	Evidence: NiColor.cpp's viewer strings print three channels
	("%s = (%5.3f,%5.3f,%5.3f)") and four ("%s = (%5.3f,%5.3f,%5.3f,%5.3f)");
	the streaming functions read and write 12 and 16 bytes.

===============================================================================
*/

#ifndef __NICOLOR_H__
#define __NICOLOR_H__

class NiStream;

class NiColor {
public:
	float			r;
	float			g;
	float			b;

					NiColor( float r_ = 0.0f, float g_ = 0.0f, float b_ = 0.0f );

	// FUNCTION: STBC 0x40DB40 ??4NiColor@@QAEAAV0@ABV0@@Z @game/SetClass
	// FUNCTION: STBC 0x4342A0 ??4NiColor@@QAEAAV0@ABV0@@Z @game/LightObjectClass
	NiColor &		operator=( const NiColor &c ) { r = c.r; g = c.g; b = c.b; return *this; }
	bool			operator==( const NiColor &c ) const;
	bool			operator!=( const NiColor &c ) const { return !( *this == c ); }
	NiColor			operator*( float scalar ) const;
	NiColor &		operator*=( float scalar );

	void			Clamp();
	void			Scale();

	// streaming
	void			LoadBinary( NiStream &stream );
	void			SaveBinary( NiStream &stream );
	char *			GetViewerString( const char *prefix ) const;

	static const NiColor	WHITE;
	static const NiColor	BLACK;
};

// The default constructor closure `new NiColorA[n]` passes to the vector
// constructor iterator.
// FUNCTION: STBC 0x006930a0 ??_FNiColorA@@QAEXXZ @game/SubsystemProperty
class NiColorA {
public:
	float			r;
	float			g;
	float			b;
	float			a;

					NiColorA( float r_ = 0.0f, float g_ = 0.0f, float b_ = 0.0f, float a_ = 0.0f );
					// declared: a by-value NiColorA argument then gets an EH-tracked home
					// FUNCTION: STBC 0x4E7B10 ??0NiColorA@@QAE@ABV0@@Z @game/PhaserGraphic
					NiColorA( const NiColorA &c ) { r = c.r; g = c.g; b = c.b; a = c.a; }

	// FUNCTION: STBC 0x494C10 ??4NiColorA@@QAEAAV0@ABV0@@Z @game/EffectController
	NiColorA &		operator=( const NiColorA &c ) { r = c.r; g = c.g; b = c.b; a = c.a; return *this; }
	bool			operator==( const NiColorA &c ) const;
	bool			operator!=( const NiColorA &c ) const { return !( *this == c ); }
	NiColorA		operator+( const NiColorA &c ) const;
	NiColorA		operator-( const NiColorA &c ) const;
	NiColorA		operator*( float scalar ) const;
	NiColorA		operator/( float scalar ) const;
	friend NiColorA	operator*( float scalar, const NiColorA &c );
	NiColorA &		operator+=( const NiColorA &c );

	void			Clamp();
	void			Scale();

	// streaming
	void			LoadBinary( NiStream &stream );
	void			SaveBinary( NiStream &stream );
	char *			GetViewerString( const char *prefix ) const;

	static const NiColorA	WHITE;
	static const NiColorA	BLACK;
};

/*
================
NiColor and NiColorA inlines

Defined after the classes, as Gamebryo 1.1.2's NiColor.inl. The arithmetic
builds its result in a copy of this colour and returns it by value
(NiBezierPatch's colour evaluators copy the result out).
================
*/
// FUNCTION: STBC 0x0040db20 ??0NiColor@@QAE@MMM@Z @game/SetClass
inline NiColor::NiColor( float r_, float g_, float b_ ) {
	r = r_;
	g = g_;
	b = b_;
}

inline bool NiColor::operator==( const NiColor &c ) const {
	return r == c.r && g == c.g && b == c.b;
}

inline NiColor NiColor::operator*( float scalar ) const {
	NiColor result = *this;
	result.r *= scalar;
	result.g *= scalar;
	result.b *= scalar;
	return result;
}

inline NiColor &NiColor::operator*=( float scalar ) {
	r *= scalar;
	g *= scalar;
	b *= scalar;
	return *this;
}

// FUNCTION: STBC 0x46C280 ??0NiColorA@@QAE@MMMM@Z @game/OBBNode
inline NiColorA::NiColorA( float r_, float g_, float b_, float a_ ) {
	r = r_;
	g = g_;
	b = b_;
	a = a_;
}

inline bool NiColorA::operator==( const NiColorA &c ) const {
	return r == c.r && g == c.g && b == c.b && a == c.a;
}

// FUNCTION: STBC 0x8342A0 ??HNiColorA@@QBE?AV0@ABV0@@Z @netimmerse/NiBezierPatch
inline NiColorA NiColorA::operator+( const NiColorA &c ) const {
	NiColorA result = *this;
	result.r += c.r;
	result.g += c.g;
	result.b += c.b;
	result.a += c.a;
	return result;
}

// PhaserBeam's constructor (0x4E40A0) expands both + and -
inline NiColorA NiColorA::operator-( const NiColorA &c ) const {
	NiColorA result = *this;
	result.r -= c.r;
	result.g -= c.g;
	result.b -= c.b;
	result.a -= c.a;
	return result;
}

// FUNCTION: STBC 0x7F1190 ??DNiColorA@@QBE?AV0@M@Z @netimmerse/NiTriBasedGeom
inline NiColorA NiColorA::operator*( float scalar ) const {
	NiColorA result = *this;
	result.r *= scalar;
	result.g *= scalar;
	result.b *= scalar;
	result.a *= scalar;
	return result;
}

inline NiColorA NiColorA::operator/( float scalar ) const {
	NiColorA result = *this;
	result.r /= scalar;
	result.g /= scalar;
	result.b /= scalar;
	result.a /= scalar;
	return result;
}

// FUNCTION: STBC 0x8510A0 ??D@YA?AVNiColorA@@MABV0@@Z @netimmerse/NiBezierCylinder
inline NiColorA operator*( float scalar, const NiColorA &c ) {
	return c * scalar;
}

inline NiColorA &NiColorA::operator+=( const NiColorA &c ) {
	r += c.r;
	g += c.g;
	b += c.b;
	a += c.a;
	return *this;
}

#endif /* !__NICOLOR_H__ */
