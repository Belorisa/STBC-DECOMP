/*
===============================================================================

	TGMatrix3.h

	The game's 3x3 matrix: NiMatrix3 with the game's own copies of its
	methods, plus the eigen solver and Congruence. The SWIG wrappers TGMatrix3_*
	name the methods; the row and column accessors are inline (their wrappers
	call nothing). No data members of its own and no virtual functions.

===============================================================================
*/

#ifndef __TGMATRIX3_H__
#define __TGMATRIX3_H__

#include "NiMatrix3.h"
#include "TGPoint3.h"

// The SWIG interface derives TGMatrix3 from NiMatrix3. Its methods are its
// own (0x459670..), not NiMatrix3's (0x813420..).
class TGMatrix3 : public NiMatrix3 {
public:
			TGMatrix3( void ) {}
			TGMatrix3( const TGPoint3 &col0, const TGPoint3 &col1, const TGPoint3 &col2 );
			TGMatrix3( const NiMatrix3 &mat );
			~TGMatrix3( void ) {}

	void	GetRow( int row, TGPoint3 *pt ) const { pt->x = entry[row][0]; pt->y = entry[row][1]; pt->z = entry[row][2]; }
	void	SetRow( int row, TGPoint3 *pt ) { entry[row][0] = pt->x; entry[row][1] = pt->y; entry[row][2] = pt->z; }
	void	GetCol( int col, TGPoint3 *pt ) const { pt->x = entry[0][col]; pt->y = entry[1][col]; pt->z = entry[2][col]; }
	void	SetCol( int col, TGPoint3 *pt ) { entry[0][col] = pt->x; entry[1][col] = pt->y; entry[2][col] = pt->z; }
	float	GetEntry( int row, int col ) const { return entry[row][col]; }
	void	SetEntry( int row, int col, float value ) { entry[row][col] = value; }

	void	MakeZero( void );
	void	MakeIdentity( void );
	void	MakeDiagonal( float, float, float );
	void	MakeXRotation( float );
	void	MakeYRotation( float );
	void	MakeZRotation( float );
	void	MakeRotation( float angle, float x, float y, float z );
	void	MakeRotation( float, TGPoint3 * );
	int		operator==( const TGMatrix3 &matrix ) const;
	TGMatrix3	operator+( const TGMatrix3 &mat ) const;
	TGMatrix3	operator-( const TGMatrix3 &mat ) const;
	TGMatrix3	operator*( const TGMatrix3 &matrix ) const;
	TGMatrix3	operator*( float scalar ) const;
	TGPoint3	operator*( const TGPoint3 &pt ) const;
	friend TGPoint3	operator*( const TGPoint3 &pt, const TGMatrix3 &mat );
	bool	Inverse( TGMatrix3 &inv ) const;
	TGMatrix3	Inverse( void ) const;
	TGMatrix3	Transpose( void ) const;
	TGMatrix3	TransposeTimes( const TGMatrix3 &mat ) const;
	// The products with another matrix on the right and on the left (the
	// SWIG wrappers TGMatrix3_MultMatrix and TGMatrix3_MultMatrixLeft).
	// FUNCTION: STBC 0x005ec3b0 ?MultMatrix@TGMatrix3@@QBE?AV1@ABV1@@Z @game/AppWrap
	TGMatrix3	MultMatrix( const TGMatrix3 &matrix ) const { return *this * matrix; }
	// FUNCTION: STBC 0x005ec4a0 ?MultMatrixLeft@TGMatrix3@@QBE?AV1@ABV1@@Z @game/AppWrap
	TGMatrix3	MultMatrixLeft( const TGMatrix3 &matrix ) const { return matrix * *this; }
	TGMatrix3 &	operator=( const TGMatrix3 &matrix );
	void	Set( TGMatrix3 *matrix ) { *this = *matrix; }
	void	ExtractAngleAndAxis( float *, float *, float *, float * );
	bool	ToEulerAnglesXYZ( float *, float *, float * );
	bool	ToEulerAnglesXZY( float *, float *, float * );
	bool	ToEulerAnglesYXZ( float *, float *, float * );
	bool	ToEulerAnglesYZX( float *, float *, float * );
	bool	ToEulerAnglesZXY( float *, float *, float * );
	bool	ToEulerAnglesZYX( float *, float *, float * );
	void	FromEulerAnglesXYZ( float, float, float );
	void	FromEulerAnglesXZY( float, float, float );
	void	FromEulerAnglesYXZ( float, float, float );
	void	FromEulerAnglesYZX( float, float, float );
	void	FromEulerAnglesZXY( float, float, float );
	void	FromEulerAnglesZYX( float, float, float );
	bool	Reorthogonalize( void );
	static void	TransformVertices( TGMatrix3 *, TGPoint3 *, int, TGPoint3 *, TGPoint3 * );
	static void	TransformNormals( TGMatrix3 *, int, TGPoint3 *, TGPoint3 * );
	static void	TransformVerticesAndNormals( TGMatrix3 *, TGPoint3 *, int, TGPoint3 *, TGPoint3 *, TGPoint3 *, TGPoint3 * );
	TGMatrix3	Congruence( const TGMatrix3 &mat ) const;
	void	EigenSolveSymmetric( float *eigenvalues, TGPoint3 *eigenvectors );

private:
	// Lineage: the Householder reduction and the QL iteration of David
	// Eberly's Magic Software Matrix3 (EigenSolveSymmetric's helpers), the
	// same algorithms under the same names; no string in the binary names them.
	void	Tridiagonal( float *diag, float *subDiag );
	int		QLAlgorithm( float *diag, float *subDiag );
};

#endif /* !__TGMATRIX3_H__ */
