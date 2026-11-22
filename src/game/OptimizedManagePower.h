/*
===============================================================================

	OptimizedManagePower.h

	The optimized version of the script preprocessor ManagePower
	(AI.Preprocessors): every UPDATE_INTERVAL seconds it shares the power
	plant's output and the batteries among the ship's powered subsystems,
	by their weights, and asks each for its share as a percentage of its
	normal power. With bConservePower the batteries are spared sooner.

	Evidence: the static initialiser 0x486EE0 files the script class name
	"ManagePower" with 0x487090 in PreprocessingAI's list of optimized
	versions, and 0x486E80 files the stream create function 0x4876C0 for
	the AI type 11. 0x487090 allocates 0x50 bytes and calls the
	constructor 0x486FA0 with the scripted AI, which it then deletes; the
	constructor and the stream constructor (0x487650) store the vtable
	0x88BEC4 (PreprocessingAI's 23 slots: the destructor 0x487060,
	WriteToStream 0x487620, GetAIType 0x487030, VirtualSlot20 0x487040 and
	the preprocessing call 0x487100 are overridden). The constructor reads
	the script attribute "bConservePower" into 0x4C. The class has no name
	string: it is named after OptimizedFireScript and OptimizedSelectTarget,
	the optimized versions the SWIG interface names. Member names are
	invented.

===============================================================================
*/

#ifndef __OPTIMIZEDMANAGEPOWER_H__
#define __OPTIMIZEDMANAGEPOWER_H__

#include "PreprocessingAI.h"

class PowerSubsystem;
class PoweredSubsystem;

// The AI type of the class (slot 19).
#define AI_TYPE_MANAGE_POWER	11

// A powered subsystem's claim on the power being shared (invented name).
struct PowerShare {
	float				weight;		// the subsystem's weight (GetWeight)
	PoweredSubsystem *	subsystem;
};

class OptimizedManagePower : public PreprocessingAI {
public:
	// Takes over the scripted AI's contained AI and its bConservePower.
						OptimizedManagePower( PreprocessingAI *ai );
						OptimizedManagePower( TGStream *stream, bool *ok );
	virtual				~OptimizedManagePower( void );

	// ArtificialIntelligence
	virtual bool		WriteToStream( TGStream *stream );
	virtual int			GetAIType( void );
	virtual bool		VirtualSlot20( int type );

	// PreprocessingAI
	virtual int			CallPreprocessingMethod( double time );

	// The optimized version of the scripted AI, which is deleted (invented
	// name).
	static ArtificialIntelligence *	CreateFromScript( PreprocessingAI *ai );
	// The stream constructor, for CreateFromStream.
	static ArtificialIntelligence *	Create( TGStream *stream );

protected:
	// How far the batteries are from where they should be: 0.5 at the
	// threshold, toward 0 below it, toward 1 above it (invented name).
	float				GetBatteryLevel( PowerSubsystem *power );
	// Turns the battery level into the power to share out (invented name).
	void				SetPowerLevel( PowerSubsystem *power, float level );
	// Shares the power among the powered subsystems other than the cloak
	// by their weights; a subsystem whose share passes
	// MAX_POWER_PERCENTAGE gets that much and the rest is shared again
	// (invented name).
	void				DistributePower( PowerSubsystem *power, float available );
	// The subsystem's weight in the sharing (invented name).
	float				GetWeight( PoweredSubsystem *subsystem );

	bool				conservePower;		// 0x4C the script's bConservePower
};

#endif /* !__OPTIMIZEDMANAGEPOWER_H__ */
