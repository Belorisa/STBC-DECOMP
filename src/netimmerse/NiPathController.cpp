// NiPathController.cpp -- path animation

#include "NiRTLib.h"
#include <math.h>
#include <string.h>

#include "NiPathController.h"
#include "NiMath.h"
#include "NiAVObject.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id pair.
// FUNCTION: STBC 0x007ae710 _$E226
// FUNCTION: STBC 0x007ae730 _$E225
// FUNCTION: STBC 0x007ae610 _$E17
// FUNCTION: STBC 0x007ae620 _$E14
// FUNCTION: STBC 0x007ae630 _$E16
// FUNCTION: STBC 0x007ae640 _$E15
// FUNCTION: STBC 0x007ae650 _$E23
// FUNCTION: STBC 0x007ae660 _$E20
// FUNCTION: STBC 0x007ae670 _$E22
// FUNCTION: STBC 0x007ae680 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x007ae760 _$E30
// FUNCTION: STBC 0x007ae770 _$E29
// FUNCTION: STBC 0x007ae780 _$E85
// FUNCTION: STBC 0x007ae790 _$E84
// FUNCTION: STBC 0x007ae7a0 _$E41
// FUNCTION: STBC 0x007ae7b0 _$E40
// FUNCTION: STBC 0x007ae7c0 _$E89
// FUNCTION: STBC 0x007ae7d0 _$E88
// FUNCTION: STBC 0x007ae7e0 _$E93
// FUNCTION: STBC 0x007ae7f0 _$E92
// FUNCTION: STBC 0x007ae800 _$E212
// FUNCTION: STBC 0x007ae810 _$E211
// FUNCTION: STBC 0x007ae820 _$E60
// FUNCTION: STBC 0x007ae830 _$E59
// FUNCTION: STBC 0x007ae840 _$E64
// FUNCTION: STBC 0x007ae850 _$E63
// FUNCTION: STBC 0x007ae880 _$E73
// FUNCTION: STBC 0x007ae890 _$E72
// FUNCTION: STBC 0x007ae8a0 _$E77
// FUNCTION: STBC 0x007ae8b0 _$E76
// FUNCTION: STBC 0x007ae920 _$E81
// FUNCTION: STBC 0x007ae930 _$E80

// FUNCTION: STBC 0x007acb20 _$E218
// FUNCTION: STBC 0x007acb30 _$E214
// FUNCTION: STBC 0x007acb50 _$E217
// FUNCTION: STBC 0x007acb60 _$E215
// DATA: STBC 0x9A0720 ?rtti@NiPathController@@2VNiRTTI@@B
const NiRTTI NiPathController::rtti( "NiPathController", &NiTimeController::rtti );

/*
================
NiPathController::Register
================
*/
// FUNCTION: STBC 0x7ACB80 ?Register@NiPathController@@SAHXZ
NiImplementRegister( NiPathController )

/*
================
NiPathController::FindController

The first NiPathController of the target's controllers.
================
*/
// FUNCTION: STBC 0x7ACCD0
NiPathController *NiPathController::FindController( NiObjectNET *target ) {
	for ( NiTimeController *controller = target->GetControllers(); controller; controller = controller->GetNext() ) {
		NiPathController *pathController = NiDynamicCast( NiPathController, controller );
		if ( pathController ) {
			return pathController;
		}
	}
	return 0;
}

/*
================
NiPathController::NiPathController
================
*/
// FUNCTION: STBC 0x7ACD20
NiPathController::NiPathController( NiPosData *pathData_, NiFloatData *pctData_ ) : pathData( pathData_ ), pctData( pctData_ ) {
	lastPathIndex = 0;
	lastPctIndex = 0;

	SetAllowFlip( false );
	SetConstVelocity( false );
	SetFollow( false );
	maxBankAngle = 0.0f;
	smoothing = 0.0f;
	bankDir = POSITIVE;
	followAxis = 0;
	SetFlip( false );
	SetCurveTypeOpen( true );
	maxCurvature = 0.0f;

	// constant velocity data
	pathLengths = 0;
	totalLength = -1.0f;
	SetCVDataNeedsUpdate( true );
}

/*
================
NiPathController::~NiPathController
================
*/
// FUNCTION: STBC 0x7ACDB0
NiPathController::~NiPathController() {
	pathData = 0;
	pctData = 0;
	delete [] pathLengths;
}

/*
================
NiPathController::ReplacePathData

Takes ownership of the path keys; the curve is open when its ends differ
(Gamebryo 1.2.2: NiPathController::ReplacePathData).
================
*/
// FUNCTION: STBC 0x7ACEA0
void NiPathController::ReplacePathData( NiPosKey **keys, unsigned int numKeys, NiPosKey::KeyType keyType ) {
	if ( !pathData ) {
		pathData = new NiPosData;
	}
	pathData->ReplaceAnim( keys, numKeys, keyType );
	lastPathIndex = 0;

	SetCurveTypeOpen( keys[0]->GetPos() != keys[numKeys - 1]->GetPos() );

	SetCVDataNeedsUpdate( true );
	if ( GetConstVelocity() ) {
		SetConstantVelocityData();
	}

	NiPosKey::CurvatureFunction curvature = NiPosKey::GetCurvatureFunction( keyType );
	maxCurvature = curvature( keys, numKeys );
}

/*
================
NiPathController::ReplacePctData

Takes ownership of the percentage keys; they set the controller's key
times (Gamebryo 1.2.2: NiPathController::ReplacePctData).
================
*/
// FUNCTION: STBC 0x7AD020
void NiPathController::ReplacePctData( NiFloatKey **keys, unsigned int numKeys, NiFloatKey::KeyType keyType ) {
	if ( !pctData ) {
		pctData = new NiFloatData;
	}
	pctData->ReplaceAnim( keys, numKeys, keyType );
	lastPctIndex = 0;

	loKeyTime = keys[0]->GetTime();
	hiKeyTime = keys[numKeys - 1]->GetTime();
}

/*
================
NiPathController::GetTotalPathLength
================
*/
// FUNCTION: STBC 0x7AD0F0
float NiPathController::GetTotalPathLength() {
	if ( totalLength < 0.0f ) {
		unsigned int uiNumPathKeys;
		NiPosKey::KeyType keyType;
		NiPosKey **keys;
		GetPathData( keys, uiNumPathKeys, keyType );
		assert( uiNumPathKeys >= 1 );
		delete [] pathLengths;
		pathLengths = new float[uiNumPathKeys];
		assert( pathLengths );

		pathLengths[0] = 0.0f;
		for ( unsigned int ui0 = 0, ui1 = 1; ui0 < uiNumPathKeys - 1; ui0++, ui1++ ) {
			float length = PathLength( ui0, ui1, 1.0f );
			pathLengths[ui1] = pathLengths[ui0] + length;
		}
		totalLength = pathLengths[uiNumPathKeys - 1];
	}
	return totalLength;
}

/*
================
NiPathController::SetConstantVelocityData
================
*/
// FUNCTION: STBC 0x7AD180
void NiPathController::SetConstantVelocityData() {
	if ( !GetCVDataNeedsUpdate() ) {
		return;
	}

	totalLength = GetTotalPathLength();
	SetCVDataNeedsUpdate( false );
}

/*
================
NiPathController::Update

Moves the target to the path position of the percentage the percentage
keys give for the time, turning it along the path when it follows it
(Gamebryo 1.2.2: NiPathController::Update). The locals keep the lineage's
Hungarian names, which give the debug copy's /Od frame order (AnimOnly.exe
0x49BD8C); ppPctKeys and ppPathKeys hold its key pointers, and pos (name
invented; fits the /Od frame order) its kPos.
================
*/
// FUNCTION: STBC 0x7AD1A0
void NiPathController::Update( float time ) {
	unsigned int uiNumPctKeys;
	NiFloatKey::KeyType ePctType;
	NiFloatKey **ppPctKeys;
	GetPctData( ppPctKeys, uiNumPctKeys, ePctType );
	if ( !ppPctKeys ) {
		return;
	}
	unsigned int uiNumPathKeys;
	NiPosKey::KeyType ePathType;
	NiPosKey **ppPathKeys;
	GetPathData( ppPathKeys, uiNumPathKeys, ePathType );
	if ( !ppPathKeys ) {
		return;
	}
	if ( DontDoUpdate( time ) ) {
		return;
	}

	float fScaledTime = ComputeScaledTime( time );
	float fPct = NiFloatKey::GenInterp( fScaledTime, ppPctKeys, ePctType, uiNumPctKeys, lastPctIndex );

	unsigned int ui1, ui2;
	float fNormTime;
	GetLoc( fPct, ui1, ui2, fNormTime );

	if ( GetFollow() ) {
		NiMatrix3 rotate;
		CalcRefFrame( ui1, ui2, fNormTime, rotate );
		( (NiAVObject *)target )->SetRotate( rotate );
	}
	NiPoint3 pos = GetPosition( ui1, ui2, fNormTime );
	( (NiAVObject *)target )->SetTranslate( pos );
}

/*
================
NiPathController::GetPercentLoc

Finds the segment holding the distance along the path, then refines the
parameter by Newton's method until the length to it is the distance
(Gamebryo 1.2.2: NiPathController::GetPercentLoc). The function's locals
keep the lineage's Hungarian names, which give the debug copy's /Od frame
order (AnimOnly.exe 0x49BEA6); ppPathKeys (name invented; fits the /Od frame
order) holds the 1.2.2 text's pkPathKeys, a key pointer array here.
================
*/
// FUNCTION: STBC 0x7AD2C0
void NiPathController::GetPercentLoc( float pct, unsigned int &index0, unsigned int &index1, float &t ) {
	unsigned int uiNumPathKeys;
	NiPosKey::KeyType keyType;
	NiPosKey **ppPathKeys;
	GetPathData( ppPathKeys, uiNumPathKeys, keyType );
	assert( uiNumPathKeys >= 2 );
	if ( pct <= 0.0f ) {
		index0 = 0;
		index1 = 1;
		t = 0.0f;
		return;
	}
	if ( pct >= 1.0f ) {
		index0 = uiNumPathKeys - 2;
		index1 = uiNumPathKeys - 1;
		t = 1.0f;
		return;
	}

	float fLength = pct * totalLength;
	float fDist;
	float fTime;
	unsigned int ui0, ui1;
	for ( ui0 = 0, ui1 = 1; ui0 < uiNumPathKeys - 1; ui0++, ui1++ ) {
		if ( fLength <= pathLengths[ui1] ) {
			fDist = fLength - pathLengths[ui0];
			fTime = fDist / ( pathLengths[ui1] - pathLengths[ui0] );
			break;
		}
	}

	const int iMaxIterations = 32;
	for ( int i = 0; i < iMaxIterations; i++ ) {
		float error = PathLength( ui0, ui1, fTime ) - fDist;
		const float tolerance = 1e-04f;
		if ( fabsf( error ) <= tolerance ) {
			break;
		}
		fTime -= error / PathSpeed( ui0, ui1, fTime );
	}

	index0 = ui0;
	index1 = ui1;
	t = fTime;
}

/*
================
NiPathController::GetLoc
================
*/
// FUNCTION: STBC 0x7AD400
void NiPathController::GetLoc( float pct, unsigned int &index0, unsigned int &index1, float &t ) {
	if ( GetConstVelocity() ) {
		GetPercentLoc( pct, index0, index1, t );
		lastPathIndex = index0;
		return;
	}

	unsigned int uiNumPathKeys;
	NiPosKey::KeyType keyType;
	NiPosKey **keys;
	GetPathData( keys, uiNumPathKeys, keyType );
	assert( uiNumPathKeys >= 1 );
	if ( pct <= keys[0]->GetTime() ) {
		index0 = 0;
		index1 = 1;
		t = 0.0f;
		return;
	}
	assert( uiNumPathKeys >= 2 );
	if ( pct >= keys[uiNumPathKeys - 1]->GetTime() ) {
		index0 = uiNumPathKeys - 2;
		index1 = uiNumPathKeys - 1;
		t = 1.0f;
		return;
	}

	// search on from the last segment, wrapping around once (Gamebryo
	// 1.2.2: NiPathController::GetLoc); a time outside every segment
	// leaves ui2 unset, which is only asserted against. The locals keep the
	// lineage's Hungarian names where the debug copy's /Od frame order needs
	// them (AnimOnly.exe 0x49C060)
	bool bFound = false;
	bool bFirstLoop = true;
	unsigned int ui1, ui2;
	for ( ui1 = lastPathIndex; ui1 < uiNumPathKeys - 1; ) {
		if ( pct == keys[ui1]->GetTime() ) {
			lastPathIndex = ui1;
			ui2 = ui1 + 1;
			bFound = true;
			break;
		} else if ( pct > keys[ui1]->GetTime() && pct < keys[ui1 + 1]->GetTime() ) {
			lastPathIndex = ui1;
			ui2 = ui1 + 1;
			bFound = true;
			break;
		}
		if ( ui1 == uiNumPathKeys - 2 && bFirstLoop ) {
			ui1 = 0;
			bFirstLoop = false;
			continue;
		}
		ui1++;
	}

	assert( bFound );
	t = ( pct - keys[ui1]->GetTime() ) / ( keys[ui2]->GetTime() - keys[ui1]->GetTime() );
	index0 = ui1;
	index1 = ui2;
}

/*
================
NiPathController::GetPosition
================
*/
// FUNCTION: STBC 0x7AD540
NiPoint3 NiPathController::GetPosition( unsigned int index0, unsigned int index1, float t ) const {
	unsigned int uiNumPathKeys;
	NiPosKey::KeyType keyType;
	NiPosKey **ppPathKeys;
	GetPathData( ppPathKeys, uiNumPathKeys, keyType );
	NiPosKey::InterpFunction interp = NiPosKey::GetInterpFunction( keyType );
	assert( interp );
	return interp( t, ppPathKeys[index0], ppPathKeys[index1] );
}

/*
================
NiPathController::CalcRefFrame

The target's rotation along the path: the tangent is its first column and
the normal and binormal, or an up vector from the Z axis when the target
may not flip, the others. Near a key the normal is blended with the one
of the next segment to smooth the jump in the second derivative.
Gamebryo 1.2.2: NiPathController::CalcRefFrame. The locals keep the
lineage's Hungarian names where the debug copy's /Od frame order needs them
(AnimOnly.exe 0x49C32B; ppPathKeys fits that order, name invented).
================
*/
// FUNCTION: STBC 0x7AD5A0
void NiPathController::CalcRefFrame( unsigned int index0, unsigned int index1, float t, NiMatrix3 &rotate ) {
	NiMatrix3 tm = NiMatrix3::IDENTITY;

	unsigned int uiNumPathKeys;
	NiPosKey::KeyType keyType;
	NiPosKey **ppPathKeys;
	GetPathData( ppPathKeys, uiNumPathKeys, keyType );

	if ( uiNumPathKeys < 2 ) {
		rotate = tm;
		return;
	}

	NiPoint3 tangent;
	NiPoint3 normal;
	NiPoint3 binormal;
	float fCurvature;

	if ( GetAllowFlip() ) {
		NiPosKey::CoordinateFrame( t, ppPathKeys[index0], ppPathKeys[index1], keyType, tangent, normal, binormal, fCurvature );

		// near a key the normal is a blend of the two segments' normals
		NiPoint3 tmpTan;
		NiPoint3 normal1;
		float fA0;
		float fA1;
		float fCurvature1;

		if ( smoothing ) {
			if ( t >= 1.0f - smoothing ) {
				unsigned int next = ( !GetCurveTypeOpen() && index1 == uiNumPathKeys - 1 ? 0 : index1 );
				if ( next < uiNumPathKeys - 1 ) {
					fA0 = ( 1.0f - t + smoothing ) / ( 2.0f * smoothing );
					fA1 = 1.0f - fA0;

					NiPosKey::CoordinateFrame( 1.0f - smoothing, ppPathKeys[index0], ppPathKeys[index1], keyType, tmpTan, normal, binormal, fCurvature );
					NiPosKey::CoordinateFrame( smoothing, ppPathKeys[next], ppPathKeys[next + 1], keyType, tmpTan, normal1, binormal, fCurvature1 );

					normal = fA0 * normal + fA1 * normal1;
					normal.Unitize();
					fCurvature = fA0 * fCurvature + fA1 * fCurvature1;
					binormal = tangent.Cross( normal );
				}
			} else if ( t <= smoothing ) {
				unsigned int previous = ( !GetCurveTypeOpen() && index0 == 0 ? uiNumPathKeys - 1 : index0 );
				if ( previous >= 1 ) {
					fA1 = ( smoothing - t ) / ( 2.0f * smoothing );
					fA0 = 1.0f - fA1;

					NiPosKey::CoordinateFrame( smoothing, ppPathKeys[index0], ppPathKeys[index1], keyType, tmpTan, normal, binormal, fCurvature );
					NiPosKey::CoordinateFrame( 1.0f - smoothing, ppPathKeys[previous - 1], ppPathKeys[previous], keyType, tmpTan, normal1, binormal, fCurvature1 );

					normal = fA0 * normal + fA1 * normal1;
					normal.Unitize();
					fCurvature = fA0 * fCurvature + fA1 * fCurvature1;
					binormal = tangent.Cross( normal );
				}
			}
		}

		tm.SetCol( 0, tangent );
		tm.SetCol( 1, -normal );
		tm.SetCol( 2, -binormal );
	} else {
		NiPosKey::CoordinateFrame( t, ppPathKeys[index0], ppPathKeys[index1], keyType, tangent, normal, binormal, fCurvature );

		tm.SetCol( 0, tangent );

		NiPoint3 up = NiPoint3::UNIT_Z.UnitCross( tangent );
		tm.SetCol( 1, up );

		NiPoint3 right = tangent.Cross( up );
		tm.SetCol( 2, right );
	}

	// swap axes if necessary
	if ( GetFlip() ) {
		NiPoint3 column;
		tm.GetCol( 0, column );
		tm.SetCol( 0, -column );
		tm.GetCol( 1, column );
		tm.SetCol( 1, -column );
	}

	NiPoint3 tmp;
	NiPoint3 tmp2;

	switch ( followAxis ) {
	case 1:
		tm.GetCol( 0, tmp );
		tm.GetCol( 1, tmp2 );
		tm.SetCol( 0, -tmp2 );
		tm.SetCol( 1, tmp );
		break;
	case 2:
		tm.GetCol( 0, tmp );
		tm.GetCol( 2, tmp2 );
		tm.SetCol( 0, -tmp2 );
		tm.SetCol( 2, tmp );
		break;
	}

	if ( GetBank() ) {
		static float fourOverPi = 4.0f / NI_PI;
		float bankAmount = float( bankDir ) * maxBankAngle;
		if ( fCurvature < maxCurvature ) {
			bankAmount *= fourOverPi * NiATan( fCurvature / maxCurvature );
		}

		NiMatrix3 bankMat;
		bankMat.MakeRotation( bankAmount, tangent );
		tm = bankMat * tm;
	}

	rotate = tm;
}

/*
================
NiPathController::PathSpeed
================
*/
// FUNCTION: STBC 0x7ADF30
float NiPathController::PathSpeed( unsigned int index0, unsigned int index1, float t ) const {
	unsigned int uiNumPathKeys;
	NiPosKey::KeyType keyType;
	NiPosKey **ppPathKeys;
	GetPathData( ppPathKeys, uiNumPathKeys, keyType );
	NiPosKey::InterpFunction interpD1 = NiPosKey::GetInterpD1Function( keyType );
	assert( interpD1 );
	NiPoint3 deriv = interpD1( t, ppPathKeys[index0], ppPathKeys[index1] );
	return deriv.Length();
}

/*
================
NiPathController::PathLength

Five point Gauss-Legendre quadrature of the speed over [0, t].
================
*/
// FUNCTION: STBC 0x7ADFA0
float NiPathController::PathLength( unsigned int index0, unsigned int index1, float t ) const {
	// the quadrature's roots and weights, mapped to [0, 1]
	const int degree = 5;
	// DATA: STBC 0x976288 _?roots@?1??PathLength@NiPathController@@IBEMIIM@Z@4PAMA
	static float roots[degree] = { 0.046910077f, 0.230765345f, 0.5f, 0.769234655f, 0.953089922f };
	// DATA: STBC 0x97629C _?coefficients@?1??PathLength@NiPathController@@IBEMIIM@Z@4PAMA
	static float coefficients[degree] = { 0.118463442f, 0.239314335f, 0.284444444f, 0.239314335f, 0.118463442f };

	float length = 0.0f;
	for ( int i = 0; i < degree; i++ ) {
		length += coefficients[i] * PathSpeed( index0, index1, t * roots[i] );
	}
	length *= t;
	return length;
}

/*
================
NiPathController::CreateClone
================
*/
// FUNCTION: STBC 0x7AE000
NiObject *NiPathController::CreateClone() {
	NiPathController *clone = new NiPathController;
	CopyMembers( clone );
	return clone;
}

/*
================
NiPathController::CopyMembers

The clone takes copies of the path and percentage data.
================
*/
// FUNCTION: STBC 0x7AE060
void NiPathController::CopyMembers( NiPathController *dest ) {
	NiTimeController::CopyMembers( dest );

	if ( pathData ) {
		dest->SetPathData( (NiPosData *)pathData->CreateClone() );
	}
	if ( pctData ) {
		dest->SetPctData( (NiFloatData *)pctData->CreateClone() );
	}

	dest->SetAllowFlip( GetAllowFlip() );
	dest->SetBank( GetBank() );
	dest->SetBankDir( GetBankDir() );
	dest->SetConstVelocity( GetConstVelocity() );
	dest->SetFollow( GetFollow() );
	dest->SetMaxBankAngle( GetMaxBankAngle() );
	dest->SetSmoothing( GetSmoothing() );
	dest->SetFollowAxis( GetFollowAxis() );
	dest->SetFlip( GetFlip() );
	dest->SetCurveTypeOpen( GetCurveTypeOpen() );
}

/*
================
NiPathController::CreateObject
================
*/
// FUNCTION: STBC 0x7AE150
void NiPathController::CreateObject( NiStream &stream, NiObject *&object ) {
	NiPathController *controller = new NiPathController;
	stream.SetLoadRecord( new NiPathControllerLoadRecord );
	controller->LoadBinary( stream );
	object = controller;
}

/*
================
NiPathController::LoadBinary

The bank direction, bank angle, smoothing and follow axis, then the link
IDs of the path and percentage data.
================
*/
// FUNCTION: STBC 0x7AE1F0
void NiPathController::LoadBinary( NiStream &stream ) {
	NiTimeController::LoadBinary( stream );
	NiStreamLoadEnum( stream, bankDir );
	NiStreamLoadBinary( stream, maxBankAngle );
	NiStreamLoadBinary( stream, smoothing );
	NiStreamLoadBinary( stream, followAxis );
	NiPathControllerLoadRecord *linkRecord = (NiPathControllerLoadRecord *)stream.GetLoadRecord();
	assert( linkRecord );
	NiStreamLoadBinary( stream, linkRecord->pathDataLinkID );
	NiStreamLoadBinary( stream, linkRecord->pctDataLinkID );
}

/*
================
NiPathController::LinkObject
================
*/
// FUNCTION: STBC 0x7AE270
void NiPathController::LinkObject( NiStream &stream ) {
	NiTimeController::LinkObject( stream );
	NiPathControllerLoadRecord *linkRecord = (NiPathControllerLoadRecord *)stream.GetLoadRecord();
	assert( linkRecord );
	pathData = (NiPosData *)stream.GetObjectFromLinkID( linkRecord->pathDataLinkID );
	pctData = (NiFloatData *)stream.GetObjectFromLinkID( linkRecord->pctDataLinkID );
}

/*
================
NiPathController::RegisterStreamables
================
*/
// FUNCTION: STBC 0x7AE300
bool NiPathController::RegisterStreamables( NiStream &stream ) {
	if ( !NiTimeController::RegisterStreamables( stream ) ) {
		return false;
	}
	if ( pathData ) {
		pathData->RegisterStreamables( stream );
	}
	if ( pctData ) {
		pctData->RegisterStreamables( stream );
	}
	return true;
}

/*
================
NiPathController::SaveBinary
================
*/
// FUNCTION: STBC 0x7AE340
void NiPathController::SaveBinary( NiStream &stream ) {
	NiTimeController::SaveBinary( stream );
	NiStreamSaveEnum( stream, bankDir );
	NiStreamSaveBinary( stream, maxBankAngle );
	NiStreamSaveBinary( stream, smoothing );
	NiStreamSaveBinary( stream, followAxis );
	NiStreamSaveBinary( stream, pathData );
	NiStreamSaveBinary( stream, pctData );
}

/*
================
NiPathController::IsEqual
================
*/
// FUNCTION: STBC 0x7AE3B0
bool NiPathController::IsEqual( NiObject *object ) {
	if ( !NiTimeController::IsEqual( object ) ) {
		return false;
	}

	NiPathController *controller = (NiPathController *)object;

	if ( !pathData->IsEqual( controller->pathData ) ) {
		return false;
	}
	if ( !pctData->IsEqual( controller->pctData ) ) {
		return false;
	}

	if ( ( pathLengths && !controller->pathLengths ) || ( !pathLengths && controller->pathLengths ) ) {
		return false;
	}
	if ( pathLengths ) {
		unsigned int uiNumPathKeys;
		NiPosKey::KeyType ePathType;
		NiPosKey **ppPathKeys;
		GetPathData( ppPathKeys, uiNumPathKeys, ePathType );

		unsigned int uiCount = uiNumPathKeys * sizeof( float );
		if ( memcmp( pathLengths, controller->pathLengths, uiCount ) != 0 ) {
			return false;
		}
	}

	if ( totalLength != controller->totalLength ||
		GetCVDataNeedsUpdate() != controller->GetCVDataNeedsUpdate() ||
		GetCurveTypeOpen() != controller->GetCurveTypeOpen() ||
		bankDir != controller->bankDir ||
		GetAllowFlip() != controller->GetAllowFlip() ||
		GetBank() != controller->GetBank() ||
		GetConstVelocity() != controller->GetConstVelocity() ||
		GetFollow() != controller->GetFollow() ||
		maxBankAngle != controller->maxBankAngle ||
		smoothing != controller->smoothing ||
		followAxis != controller->followAxis ||
		GetFlip() != controller->GetFlip() ) {
		return false;
	}
	return true;
}

/*
================
NiPathController::GetViewerStrings
================
*/
// FUNCTION: STBC 0x7AE510
void NiPathController::GetViewerStrings( NiTArray<char *> *strings ) {
	NiTimeController::GetViewerStrings( strings );
	strings->Add( NiGetViewerString( rtti.GetName() ) );
}

/*
================
NiPathController::TargetIsRequiredType

The target must be an NiAVObject.
================
*/
// FUNCTION: STBC 0x7AE940
bool NiPathController::TargetIsRequiredType() const {
	return NiIsKindOf( NiAVObject, target );
}
