/*
===============================================================================

	NiMatrix3.h

	A 3x3 matrix, stored by rows; rotations act on column vectors
	(NiMatrix3 * NiPoint3).

	Evidence: NiMatrix3.cpp's viewer string prints nine floats as three
	triples ("%s = ((%f,%f,%f),(%f,%f,%f),(%f,%f,%f))"); the streaming
	functions read and write 36 bytes; the static initializers build a zero
	matrix and an identity matrix.

===============================================================================
*/

#ifndef __NIMATRIX3_H__
#define __NIMATRIX3_H__

#include <assert.h>

#include "NiPoint3.h"

class NiStream;

class NiMatrix3 {
public:
					NiMatrix3() {}
					NiMatrix3( const NiPoint3 &col0, const NiPoint3 &col1, const NiPoint3 &col2 );
					NiMatrix3( const NiMatrix3 &mat );

	NiMatrix3 &		operator=( const NiMatrix3 &mat );

	void			GetRow( unsigned int row, NiPoint3 &pt ) const;
	void			SetRow( unsigned int row, const NiPoint3 &pt );
	void			GetCol( unsigned int col, NiPoint3 &pt ) const;
	void			SetCol( unsigned int col, const NiPoint3 &pt );
	void			GetCol( unsigned int col, float &f0, float &f1, float &f2 ) const;
	void			SetCol( unsigned int col, float f0, float f1, float f2 );
	float			GetEntry( unsigned int row, unsigned int col ) const;
	void			SetEntry( unsigned int row, unsigned int col, float value );

	void			MakeZero();
	void			MakeIdentity();
	void			MakeDiagonal( float d0, float d1, float d2 );
	void			MakeXRotation( float angle );
	void			MakeYRotation( float angle );
	void			MakeZRotation( float angle );
	void			MakeRotation( float angle, float x, float y, float z );
	void			MakeRotation( float angle, const NiPoint3 &axis );

	bool			operator==( const NiMatrix3 &mat ) const;
	// AnimOnly.exe 0x417170
	bool			operator!=( const NiMatrix3 &mat ) const { return !( *this == mat ); }
	NiMatrix3		operator+( const NiMatrix3 &mat ) const;
	NiMatrix3		operator-( const NiMatrix3 &mat ) const;
	NiMatrix3		operator*( const NiMatrix3 &mat ) const;
	NiMatrix3		operator*( float scalar ) const;
	NiPoint3		operator*( const NiPoint3 &pt ) const;
	friend NiPoint3	operator*( const NiPoint3 &pt, const NiMatrix3 &mat );

	bool			Inverse( NiMatrix3 &inv ) const;
	NiMatrix3		Inverse() const;
	NiMatrix3		Transpose() const;
	NiMatrix3		TransposeTimes( const NiMatrix3 &mat ) const;
	// rot * this * rot transposed (Gamebryo lineage name)
	NiMatrix3		Congruence( const NiMatrix3 &rot ) const;

	void			ExtractAngleAndAxis( float &angle, float &x, float &y, float &z ) const;
	bool			Reorthogonalize();

	// the eigenvalues and unit eigenvectors of a symmetric matrix
	// (Gamebryo lineage name)
	void			EigenSolveSymmetric( float eigenvalue[3], NiPoint3 eigenvector[3] );
	// the two steps of EigenSolveSymmetric on a copy of the matrix: the
	// Householder reduction to the diagonal and subdiagonal, leaving the
	// transform in the matrix, then the QL iteration (at most 32 passes per
	// eigenvalue; 0 when one does not converge) that leaves the
	// eigenvalues in diag and the eigenvectors in the columns (Gamebryo
	// lineage names)
	void			Tridiagonal( float diag[3], float subd[3] );
	int				QLAlgorithm( float diag[3], float subd[3] );

	bool			ToEulerAnglesXYZ( float &xAngle, float &yAngle, float &zAngle ) const;
	bool			ToEulerAnglesXZY( float &xAngle, float &zAngle, float &yAngle ) const;
	bool			ToEulerAnglesYXZ( float &yAngle, float &xAngle, float &zAngle ) const;
	bool			ToEulerAnglesYZX( float &yAngle, float &zAngle, float &xAngle ) const;
	bool			ToEulerAnglesZXY( float &zAngle, float &xAngle, float &yAngle ) const;
	bool			ToEulerAnglesZYX( float &zAngle, float &yAngle, float &xAngle ) const;
	void			FromEulerAnglesXYZ( float xAngle, float yAngle, float zAngle );
	void			FromEulerAnglesXZY( float xAngle, float zAngle, float yAngle );
	void			FromEulerAnglesYXZ( float yAngle, float xAngle, float zAngle );
	void			FromEulerAnglesYZX( float yAngle, float zAngle, float xAngle );
	void			FromEulerAnglesZXY( float zAngle, float xAngle, float yAngle );
	void			FromEulerAnglesZYX( float zAngle, float yAngle, float xAngle );

	static void		TransformVertices( const NiMatrix3 &rot, const NiPoint3 &trans, unsigned int count, const NiPoint3 *in, NiPoint3 *out );
	static void		TransformNormals( const NiMatrix3 &rot, unsigned int count, const NiPoint3 *in, NiPoint3 *out );
	static void		TransformVerticesAndNormals( const NiMatrix3 &rot, const NiPoint3 &trans, unsigned int count,
												 const NiPoint3 *inVertex, NiPoint3 *outVertex, const NiPoint3 *inNormal,
												 NiPoint3 *outNormal );

	// streaming
	void			LoadBinary( NiStream &stream );
	void			SaveBinary( NiStream &stream );
	char *			GetViewerString( const char *prefix ) const;

	static const NiMatrix3	ZERO;
	static const NiMatrix3	IDENTITY;

	float			entry[3][3];	// [row][column]
};

/*
================
NiMatrix3 inlines

Defined after the class, as Gamebryo 1.1.2's NiMatrix3.inl.
================
*/
inline void NiMatrix3::GetRow( unsigned int row, NiPoint3 &pt ) const {
	assert( row <= 2 );
	pt.x = entry[row][0];
	pt.y = entry[row][1];
	pt.z = entry[row][2];
}

inline void NiMatrix3::SetRow( unsigned int row, const NiPoint3 &pt ) {
	assert( row <= 2 );
	entry[row][0] = pt.x;
	entry[row][1] = pt.y;
	entry[row][2] = pt.z;
}

// FUNCTION: STBC 0x7AE740 ?GetCol@NiMatrix3@@QBEXIAAVNiPoint3@@@Z @netimmerse/NiPathController
inline void NiMatrix3::GetCol( unsigned int col, NiPoint3 &pt ) const {
	assert( col <= 2 );
	pt.x = entry[0][col];
	pt.y = entry[1][col];
	pt.z = entry[2][col];
}

// FUNCTION: STBC 0x7A8110 ?SetCol@NiMatrix3@@QAEXIABVNiPoint3@@@Z @netimmerse/NiLookAtController
inline void NiMatrix3::SetCol( unsigned int col, const NiPoint3 &pt ) {
	assert( col <= 2 );
	entry[0][col] = pt.x;
	entry[1][col] = pt.y;
	entry[2][col] = pt.z;
}

// through GetEntry, where Gamebryo 1.1.2 reads the entries directly: with the direct
// reads NiCamera::UpdateWorldData (0x7E8780) calls its operator*( float, NiPoint3 )
// where retail expands it
inline void NiMatrix3::GetCol( unsigned int col, float &f0, float &f1, float &f2 ) const {
	assert( col <= 2 );
	f0 = GetEntry( 0, col );
	f1 = GetEntry( 1, col );
	f2 = GetEntry( 2, col );
}

inline void NiMatrix3::SetCol( unsigned int col, float f0, float f1, float f2 ) {
	assert( col <= 2 );
	entry[0][col] = f0;
	entry[1][col] = f1;
	entry[2][col] = f2;
}

inline float NiMatrix3::GetEntry( unsigned int row, unsigned int col ) const {
	assert( row <= 2 && col <= 2 );
	return entry[row][col];
}

inline void NiMatrix3::SetEntry( unsigned int row, unsigned int col, float value ) {
	assert( row <= 2 && col <= 2 );
	entry[row][col] = value;
}

#endif /* !__NIMATRIX3_H__ */
