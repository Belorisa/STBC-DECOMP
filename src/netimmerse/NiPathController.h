/*
===============================================================================

	NiPathController.h

	Moves its target along the curve of an NiPosData, at the path
	percentages of an NiFloatData, optionally following and banking.

	Evidence: the vtable at 0x898738 (NiTimeController's 20 slots, none
	added); the constructor (0x7ACD20) takes the path data into the smart
	pointer at 0x38 and the percentage data into the one at 0x3C, sets the
	flag bits 0x10 and 0x20 and clears 0x40, 0x100, 0x200 and 0x400, the
	word at 0x58 to 1 and the float at 0x44 to -1; the destructor
	(0x7ACDB0) frees the array at 0x40, whose IsEqual (0x7AE3B0) compares
	four bytes per path key; LoadBinary (0x7AE1F0) and SaveBinary
	(0x7AE340) stream 0x58, 0x48, 0x4C and the short at 0x50; CreateClone
	allocates 0x5C bytes; slot 19 (0x7AE940) tests the target against
	NiAVObject's NiRTTI. The NiRTTI is 0x9A0720, named "NiPathController".
	The member names follow the Gamebryo lineage and are unconfirmed.

===============================================================================
*/

#ifndef __NIPATHCONTROLLER_H__
#define __NIPATHCONTROLLER_H__

#include "NiTimeController.h"
#include "NiPosData.h"
#include "NiFloatData.h"
#include "NiStreamMacros.h"

class NiMatrix3;

/*
================
NiPathControllerLoadRecord

The record CreateObject hands the stream: NiTimeController's link IDs and
those of the path and percentage data, under a vtable of its own
(0x898788). The class name is invented.
================
*/
// FUNCTION: STBC 0x007ae8f0 ??_GNiPathControllerLoadRecord@@UAEPAXI@Z @netimmerse/NiPathController
class NiPathControllerLoadRecord : public NiTimeControllerLoadRecord {
public:
							NiPathControllerLoadRecord() { pathDataLinkID = 0; pctDataLinkID = 0; }

	unsigned int			pathDataLinkID;
	unsigned int			pctDataLinkID;
};

// FUNCTION: STBC 0x007ae8d0 ??_GNiPathController@@UAEPAXI@Z @netimmerse/NiPathController
class NiPathController : public NiTimeController {
public:
	// FUNCTION: STBC 0x7AE8C0 ?GetRTTI@NiPathController@@UBEPBVNiRTTI@@XZ
	virtual const NiRTTI *	GetRTTI() const { return &rtti; }

	// cloning
	virtual NiObject *		CreateClone();

	// streaming
	virtual void			LoadBinary( NiStream &stream );
	virtual void			LinkObject( NiStream &stream );
	virtual bool			RegisterStreamables( NiStream &stream );
	virtual void			SaveBinary( NiStream &stream );
	virtual bool			IsEqual( NiObject *object );
	virtual void			GetViewerStrings( NiTArray<char *> *strings );

	virtual					~NiPathController();

	virtual void			Update( float time );
	virtual bool			TargetIsRequiredType() const;

							NiPathController( NiPosData *pathData_ = 0, NiFloatData *pctData_ = 0 );

	// the direction the target banks in (Gamebryo lineage)
	enum BankDir {
		NEGATIVE = -1,
		POSITIVE = 1
	};

	// flags beyond NiTimeController's, above its own (Gamebryo 1.2.2's
	// NiPathController masks, shifted by four)
	enum {
		CVDATAUPDATE_MASK	= 0x0010,	// the total path length is stale
		CVDATAUPDATE_POS	= 4,
		OPENCURVE_MASK		= 0x0020,
		OPENCURVE_POS		= 5,
		ALLOWFLIP_MASK		= 0x0040,
		ALLOWFLIP_POS		= 6,
		BANK_POS			= 7,
		CONSTVELOCITY_MASK	= 0x0100,	// the target moves at constant speed along the path
		CONSTVELOCITY_POS	= 8,
		FOLLOW_MASK			= 0x0200,
		FOLLOW_POS			= 9,
		FLIP_MASK			= 0x0400,
		FLIP_POS			= 10
	};

	// the path and the percentages along it (Gamebryo lineage names)
	void					SetPathData( NiPosData *pathData_ ) { pathData = pathData_; }
	void					SetPctData( NiFloatData *pctData_ ) { pctData = pctData_; }
	// take ownership of new keys (Gamebryo lineage names)
	void					ReplacePathData( NiPosKey **keys, unsigned int numKeys, NiPosKey::KeyType keyType );
	void					ReplacePctData( NiFloatKey **keys, unsigned int numKeys, NiFloatKey::KeyType keyType );

	// the flags (Gamebryo lineage names)
	void					SetAllowFlip( bool allowFlip ) { SetBit( allowFlip, ALLOWFLIP_POS ); }
	bool					GetAllowFlip() const { return GetBit( ALLOWFLIP_POS ); }
	void					SetBank( bool bank ) { SetBit( bank, BANK_POS ); }
	bool					GetBank() const { return GetBit( BANK_POS ); }
	void					SetConstVelocity( bool constVelocity ) {
								SetBit( constVelocity, CONSTVELOCITY_POS );
								if ( constVelocity ) {
									SetConstantVelocityData();
								}
							}
	bool					GetConstVelocity() const { return GetBit( CONSTVELOCITY_POS ); }
	void					SetFollow( bool follow ) { SetBit( follow, FOLLOW_POS ); }
	bool					GetFollow() const { return GetBit( FOLLOW_POS ); }
	void					SetFlip( bool flip ) { SetBit( flip, FLIP_POS ); }
	bool					GetFlip() const { return GetBit( FLIP_POS ); }
	void					SetCurveTypeOpen( bool open ) { SetBit( open, OPENCURVE_POS ); }
	bool					GetCurveTypeOpen() const { return GetBit( OPENCURVE_POS ); }

	// banking and following (Gamebryo lineage names)
	void					SetBankDir( BankDir dir ) { bankDir = dir; }
	BankDir					GetBankDir() const { return bankDir; }
	void					SetMaxBankAngle( float angle ) { maxBankAngle = angle; }
	float					GetMaxBankAngle() const { return maxBankAngle; }
	void					SetSmoothing( float smooth ) { smoothing = smooth; }
	float					GetSmoothing() const { return smoothing; }
	void					SetFollowAxis( short axis ) { followAxis = axis; }
	short					GetFollowAxis() const { return followAxis; }

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	// the first NiPathController of the target's list (name invented)
	static NiPathController *	FindController( NiObjectNET *target );

	static const NiRTTI		rtti;

protected:
	void					CopyMembers( NiPathController *dest );

	// measures the total path length when it is stale (Gamebryo lineage name)
	void					SetConstantVelocityData();
	void					SetCVDataNeedsUpdate( bool update ) { SetBit( update, CVDATAUPDATE_POS ); }
	bool					GetCVDataNeedsUpdate() const { return GetBit( CVDATAUPDATE_POS ); }
	// the length of the whole path, measuring each key segment into the
	// path lengths when it is not known (Gamebryo lineage name)
	float					GetTotalPathLength();

	// the path's keys, or none without path data (Gamebryo lineage name)
	void					GetPathData( NiPosKey **&keys, unsigned int &numKeys, NiPosKey::KeyType &keyType ) const {
								if ( pathData ) {
									keys = pathData->GetAnim( numKeys, keyType );
								} else {
									numKeys = 0;
									keyType = NiPosKey::NOINTERP;
									keys = 0;
								}
							}
	// the percentage keys, or none without percentage data (Gamebryo
	// lineage name)
	void					GetPctData( NiFloatKey **&keys, unsigned int &numKeys, NiFloatKey::KeyType &keyType ) const {
								if ( pctData ) {
									keys = pctData->GetAnim( numKeys, keyType );
								} else {
									numKeys = 0;
									keyType = NiFloatKey::NOINTERP;
									keys = 0;
								}
							}

	// the path keys around a time and the parameter between them: by the
	// keys' times, or at constant velocity by GetPercentLoc, by distance
	// along the path (Gamebryo lineage names)
	void					GetLoc( float pct, unsigned int &index0, unsigned int &index1, float &t );
	void					GetPercentLoc( float pct, unsigned int &index0, unsigned int &index1, float &t );
	// the target's rotation following the path at t between two keys
	// (Gamebryo lineage name)
	void					CalcRefFrame( unsigned int index0, unsigned int index1, float t, NiMatrix3 &rotate );
	// the position at t between two keys
	NiPoint3				GetPosition( unsigned int index0, unsigned int index1, float t ) const;
	// the speed along the path at t between two keys, and the length of
	// the path from the first key to t, by Gauss-Legendre quadrature
	// (Gamebryo lineage names)
	float					PathSpeed( unsigned int index0, unsigned int index1, float t ) const;
	float					PathLength( unsigned int index0, unsigned int index1, float t ) const;

protected:
	unsigned int			lastPathIndex;	// the keys the last update found
	unsigned int			lastPctIndex;
	NiPointer<NiPosData>	pathData;
	NiPointer<NiFloatData>	pctData;
	float *					pathLengths;	// one per path key
	float					totalLength;	// -1 until measured
	float					maxBankAngle;
	float					smoothing;
	short					followAxis;
	float					maxCurvature;	// Gamebryo lineage name, by the constructor's store order; no reader seen
	BankDir					bankDir;
};

NiRegisterStream( NiPathController );

#endif /* !__NIPATHCONTROLLER_H__ */
