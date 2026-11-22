/*
===============================================================================

	NiRect.h

	A rectangle by its four edges. NiCamera's view port is an
	NiRect<float> in normalised screen units.

	Evidence: NiCamera holds its port at 0x15C (left, right, top, bottom);
	its constructor (0x7E7AF0) sets { 0, 1, 1, 0 }, the whole screen, and
	its viewer strings (0x7EAAB0) format "m_port" as
	"%s = (L=%f,R=%f,T=%f,B=%f)". NiD3DRender's creation (0x7C3480) passes
	the same { 0, 1, 1, 0 } to the renderer's slot 41. The constructor's
	edges default to zero: NiCamera's constructor zeroes the port before it
	sets it.

===============================================================================
*/

#ifndef __NIRECT_H__
#define __NIRECT_H__

#include <stdio.h>
#include <string.h>

template <class T> class NiRect {
public:
							NiRect( T left_ = 0, T right_ = 0, T top_ = 0, T bottom_ = 0 ) {
								left = left_; right = right_; top = top_; bottom = bottom_;
							}

	char *					GetViewerString( const char *prefix ) const;

	T						left;
	T						right;
	T						top;
	T						bottom;
};

/*
================
NiRect::GetViewerString

Returns "<prefix> = (L=left,R=right,T=top,B=bottom)" in a new string that
the caller deletes.
================
*/
template <class T> char *NiRect<T>::GetViewerString( const char *prefix ) const {
	char *string = new char[strlen( prefix ) + 64];
	sprintf( string, "%s = (L=%f,R=%f,T=%f,B=%f)", prefix, left, right, top, bottom );
	return string;
}

#endif /* !__NIRECT_H__ */
