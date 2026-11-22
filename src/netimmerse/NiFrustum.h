/*
===============================================================================

	NiFrustum.h

	A camera's view frustum: the side planes at unit distance from the eye,
	and the near and far distances.

	Evidence: NiCamera holds one at 0x144 (left, right, top, bottom, near,
	far); its constructor (0x7E7AF0) sets { -0.5, 0.5, 0.5, -0.5, 1, 2 }
	and its viewer strings (0x7EAAB0) format "m_frustum" as
	"%s = (N=%f,F=%f,L=%f,R=%f,T=%f,B=%f)" from 0x154, 0x158, 0x144, 0x148,
	0x14C and 0x150. NiD3DRender's creation (0x7C3480) passes
	{ -1, 1, 1, -1, 0.1, 1 } to the renderer's slot 41. The member names
	follow the viewer string's letters.

===============================================================================
*/

#ifndef __NIFRUSTUM_H__
#define __NIFRUSTUM_H__

class NiFrustum {
public:
							NiFrustum() {}
							NiFrustum( float left_, float right_, float top_, float bottom_, float near_, float far_ ) {
								left = left_; right = right_; top = top_; bottom = bottom_;
								nearDistance = near_; farDistance = far_;
							}

	float					left;
	float					right;
	float					top;
	float					bottom;
	float					nearDistance;
	float					farDistance;

	// defined with NiCamera, the only user
	char *					GetViewerString( const char *prefix ) const;
};

#endif /* !__NIFRUSTUM_H__ */
