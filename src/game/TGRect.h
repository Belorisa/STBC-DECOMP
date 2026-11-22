/*
===============================================================================

	TGRect.h

	A screen rectangle of the user interface: a position and a size, in
	the fractions of the screen the UI objects are laid out in.

	0x10 bytes, no vtable. Evidence: the constructor (0x739DB0) clears the
	four floats; IsWithin (0x739E90) tests a point against the left edge
	(0x00), the top (0x04), left + width (0x08) and top + height (0x0C), and
	finds nothing within an empty rectangle. The SWIG wrappers TGRect_*
	name the accessors (GetLeft, GetTop, GetWidth, GetHeight, GetRight,
	GetBottom) and IsEmpty, whose wrapper (0x5CC320) inlines the width or
	height test that IsWithin and Unknown739F00 begin with. The functions
	that move a rectangle by an NiPoint2 take the point by value and have
	the bodies of +, +=, - and -=; nothing names them.

===============================================================================
*/

#ifndef __TGRECT_H__
#define __TGRECT_H__

#include "NiPoint2.h"

class TGStream;

// What Unknown739F00 finds of a rectangle within this one.
#define RECT_OUTSIDE			0		// no corner within
#define RECT_INSIDE				1		// every corner within
#define RECT_PARTIAL			2		// some corners within

class TGRect {
public:
					TGRect( void );
					TGRect( const TGRect &rect );
					TGRect( float left, float top, float width, float height );
					~TGRect( void );

	TGRect &		Unknown739DE0( const TGRect &rect );	// a copy; nothing calls it

	void			Set( float left, float top, float width, float height ) { this->left = left; this->top = top; this->width = width; this->height = height; }
	float			GetLeft( void ) const { return left; }
	float			GetTop( void ) const { return top; }
	float			GetWidth( void ) const { return width; }
	float			GetHeight( void ) const { return height; }
	float			GetRight( void ) const { return left + width; }
	float			GetBottom( void ) const { return top + height; }
	bool			IsEmpty( void ) const { return width == 0.0f || height == 0.0f; }
	void	SetLeft( float left_ ) { left = left_; }
	void	SetWidth( float width_ ) { width = width_; }
	void	SetTop( float top_ ) { top = top_; }
	void	SetHeight( float height_ ) { height = height_; }
	void	SetRight( float right ) { width = right - left; }
	void	SetBottom( float bottom ) { height = bottom - top; }
	void	GetPosition( NiPoint2 *position ) { position->x = left; position->y = top; }
	void	SetPosition( NiPoint2 *position ) { left = position->x; top = position->y; }
	bool	IsClear( void ) { return top == 0.0f && left == 0.0f && width == 0.0f && height == 0.0f; }

	bool			DoesIntersect( TGRect *rect );
	bool			IsWithin( float x, float y );
	int				Unknown739F00( TGRect *rect );			// RECT_*: how much of rect is within this one
	void			ClipTo( TGRect *rect );
	void			Clear( void );

	TGRect			operator-( NiPoint2 offset );
	void			operator-=( NiPoint2 offset );
	TGRect			operator+( NiPoint2 offset );
	void			operator+=( NiPoint2 offset );

	bool			WriteToStream( TGStream *stream );		// invented name
	bool			ReadFromStream( TGStream *stream );		// invented name

	float			left;					// 0x00
	float			top;					// 0x04
	float			width;					// 0x08
	float			height;					// 0x0C
};

#endif /* !__TGRECT_H__ */
