/*
===============================================================================

	OptimizedManagePower.cpp

===============================================================================
*/

#include <iostream>
#include "OptimizedManagePower.h"
#include "CloakingSubsystem.h"
#include "PowerSubsystem.h"
#include "PoweredSubsystem.h"
#include "PyEmbed.h"
#include "SensorSubsystem.h"
#include "ShipClass.h"
#include "TGStream.h"
#include "UtopiaModule.h"
#include "WarpEngineSubsystem.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x00487fe0 _$E509
// FUNCTION: STBC 0x00488000 _$E508

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00486df0 _$E32
// FUNCTION: STBC 0x00486e00 _$E29
// FUNCTION: STBC 0x00486e10 _$E31
// FUNCTION: STBC 0x00486e20 _$E30
// FUNCTION: STBC 0x00486e30 _$E38
// FUNCTION: STBC 0x00486e40 _$E35
// FUNCTION: STBC 0x00486e50 _$E37
// FUNCTION: STBC 0x00486e60 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00487740 _$E102
// FUNCTION: STBC 0x00487750 _$E101
// FUNCTION: STBC 0x00487760 _$E316
// FUNCTION: STBC 0x00487770 _$E315
// FUNCTION: STBC 0x00487780 _$E113
// FUNCTION: STBC 0x00487790 _$E112
// FUNCTION: STBC 0x004877a0 _$E117
// FUNCTION: STBC 0x004877b0 _$E116
// FUNCTION: STBC 0x004877c0 _$E121
// FUNCTION: STBC 0x004877d0 _$E120
// FUNCTION: STBC 0x004877e0 _$E125
// FUNCTION: STBC 0x004877f0 _$E124
// FUNCTION: STBC 0x00487800 _$E240
// FUNCTION: STBC 0x00487810 _$E239
// FUNCTION: STBC 0x00487820 _$E182
// FUNCTION: STBC 0x00487830 _$E181
// FUNCTION: STBC 0x00487840 _$E195
// FUNCTION: STBC 0x00487850 _$E194
// FUNCTION: STBC 0x00487860 _$E245
// FUNCTION: STBC 0x00487870 _$E244
// FUNCTION: STBC 0x00487880 _$E321
// FUNCTION: STBC 0x00487890 _$E320
// FUNCTION: STBC 0x004878a0 _$E264
// FUNCTION: STBC 0x004878b0 _$E263
// FUNCTION: STBC 0x004878c0 _$E325
// FUNCTION: STBC 0x004878d0 _$E324
// FUNCTION: STBC 0x004878e0 _$E332
// FUNCTION: STBC 0x004878f0 _$E331
// FUNCTION: STBC 0x00487900 _$E353
// FUNCTION: STBC 0x00487910 _$E352
// FUNCTION: STBC 0x00487920 _$E357
// FUNCTION: STBC 0x00487930 _$E356
// FUNCTION: STBC 0x00487940 _$E144
// FUNCTION: STBC 0x00487950 _$E143
// FUNCTION: STBC 0x00487960 _$E280
// FUNCTION: STBC 0x00487970 _$E279
// FUNCTION: STBC 0x00487980 _$E287
// FUNCTION: STBC 0x00487990 _$E286
// FUNCTION: STBC 0x004879a0 _$E291
// FUNCTION: STBC 0x004879b0 _$E290
// FUNCTION: STBC 0x004879c0 _$E295
// FUNCTION: STBC 0x004879d0 _$E294
// FUNCTION: STBC 0x004879e0 _$E299
// FUNCTION: STBC 0x004879f0 _$E298
// FUNCTION: STBC 0x00487a00 _$E305
// FUNCTION: STBC 0x00487a10 _$E304
// FUNCTION: STBC 0x00487a20 _$E249
// FUNCTION: STBC 0x00487a30 _$E248
// FUNCTION: STBC 0x00487a40 _$E253
// FUNCTION: STBC 0x00487a50 _$E252
// FUNCTION: STBC 0x00487a60 _$E361
// FUNCTION: STBC 0x00487a70 _$E360
// FUNCTION: STBC 0x00487a80 _$E365
// FUNCTION: STBC 0x00487a90 _$E364
// FUNCTION: STBC 0x00487aa0 _$E369
// FUNCTION: STBC 0x00487ab0 _$E368
// FUNCTION: STBC 0x00487ac0 _$E373
// FUNCTION: STBC 0x00487ad0 _$E372
// FUNCTION: STBC 0x00487ae0 _$E377
// FUNCTION: STBC 0x00487af0 _$E376
// FUNCTION: STBC 0x00487b00 _$E381
// FUNCTION: STBC 0x00487b10 _$E380
// FUNCTION: STBC 0x00487b20 _$E385
// FUNCTION: STBC 0x00487b30 _$E384
// FUNCTION: STBC 0x00487b40 _$E389
// FUNCTION: STBC 0x00487b50 _$E388
// FUNCTION: STBC 0x00487b60 _$E149
// FUNCTION: STBC 0x00487b70 _$E148
// FUNCTION: STBC 0x00487b80 _$E393
// FUNCTION: STBC 0x00487b90 _$E392
// FUNCTION: STBC 0x00487ba0 _$E154
// FUNCTION: STBC 0x00487bb0 _$E153
// FUNCTION: STBC 0x00487bc0 _$E397
// FUNCTION: STBC 0x00487bd0 _$E396
// FUNCTION: STBC 0x00487be0 _$E401
// FUNCTION: STBC 0x00487bf0 _$E400
// FUNCTION: STBC 0x00487c00 _$E405
// FUNCTION: STBC 0x00487c10 _$E404
// FUNCTION: STBC 0x00487c20 _$E409
// FUNCTION: STBC 0x00487c30 _$E408
// FUNCTION: STBC 0x00487c40 _$E159
// FUNCTION: STBC 0x00487c50 _$E158
// FUNCTION: STBC 0x00487c60 _$E413
// FUNCTION: STBC 0x00487c70 _$E412
// FUNCTION: STBC 0x00487cc0 _$E417
// FUNCTION: STBC 0x00487cd0 _$E416
// FUNCTION: STBC 0x00487ce0 _$E421
// FUNCTION: STBC 0x00487cf0 _$E420
// FUNCTION: STBC 0x00487d00 _$E165
// FUNCTION: STBC 0x00487d10 _$E164
// FUNCTION: STBC 0x00487d20 _$E170
// FUNCTION: STBC 0x00487d30 _$E169
// FUNCTION: STBC 0x00487d40 _$E200
// FUNCTION: STBC 0x00487d50 _$E199
// FUNCTION: STBC 0x00487d60 _$E425
// FUNCTION: STBC 0x00487d70 _$E424
// FUNCTION: STBC 0x00487d80 _$E205
// FUNCTION: STBC 0x00487d90 _$E204
// FUNCTION: STBC 0x00487da0 _$E429
// FUNCTION: STBC 0x00487db0 _$E428
// FUNCTION: STBC 0x00487dc0 _$E433
// FUNCTION: STBC 0x00487dd0 _$E432
// FUNCTION: STBC 0x00487de0 _$E210
// FUNCTION: STBC 0x00487df0 _$E209
// FUNCTION: STBC 0x00487e00 _$E437
// FUNCTION: STBC 0x00487e10 _$E436
// FUNCTION: STBC 0x00487e20 _$E175
// FUNCTION: STBC 0x00487e30 _$E174
// FUNCTION: STBC 0x00487e40 _$E441
// FUNCTION: STBC 0x00487e50 _$E440
// FUNCTION: STBC 0x00487e60 _$E445
// FUNCTION: STBC 0x00487e70 _$E444
// FUNCTION: STBC 0x00487e80 _$E215
// FUNCTION: STBC 0x00487e90 _$E214
// FUNCTION: STBC 0x00487ea0 _$E220
// FUNCTION: STBC 0x00487eb0 _$E219
// FUNCTION: STBC 0x00488010 _$E449
// FUNCTION: STBC 0x00488020 _$E448
// FUNCTION: STBC 0x00488030 _$E453
// FUNCTION: STBC 0x00488040 _$E452
// FUNCTION: STBC 0x00488050 _$E457
// FUNCTION: STBC 0x00488060 _$E456
// FUNCTION: STBC 0x00488070 _$E461
// FUNCTION: STBC 0x00488080 _$E460
// FUNCTION: STBC 0x00488090 _$E465
// FUNCTION: STBC 0x004880a0 _$E464
// FUNCTION: STBC 0x004880b0 _$E469
// FUNCTION: STBC 0x004880c0 _$E468
// FUNCTION: STBC 0x004880d0 _$E473
// FUNCTION: STBC 0x004880e0 _$E472
// FUNCTION: STBC 0x004880f0 _$E477
// FUNCTION: STBC 0x00488100 _$E476
// FUNCTION: STBC 0x00488110 _$E225
// FUNCTION: STBC 0x00488120 _$E224
// FUNCTION: STBC 0x00488130 _$E481
// FUNCTION: STBC 0x00488140 _$E480
// FUNCTION: STBC 0x00488150 _$E230
// FUNCTION: STBC 0x00488160 _$E229
// FUNCTION: STBC 0x00488170 _$E235
// FUNCTION: STBC 0x00488180 _$E234
// FUNCTION: STBC 0x00488190 _$E485
// FUNCTION: STBC 0x004881a0 _$E484
// FUNCTION: STBC 0x004881b0 _$E490
// FUNCTION: STBC 0x004881c0 _$E489
// FUNCTION: STBC 0x004881d0 _$E494
// FUNCTION: STBC 0x004881e0 _$E493

// The seconds between two sharings of the power (the script's
// GetNextUpdateTime).
static const float UPDATE_INTERVAL = 3.0f;

// The most a subsystem is asked for, as a fraction of its normal power.
static const float MAX_POWER_PERCENTAGE = 1.25f;

// FUNCTION: STBC 0x00486e70 _$E497
// FUNCTION: STBC 0x00486e80 _$E496
static AICreateFunctionRegistration managePowerRegistration( AI_TYPE_MANAGE_POWER, OptimizedManagePower::Create );

// FUNCTION: STBC 0x00486ed0 _$E500
// FUNCTION: STBC 0x00486ee0 _$E499
static OptimizedVersionRegistration managePowerVersion( "ManagePower", OptimizedManagePower::CreateFromScript );

/*
================
OptimizedManagePower::OptimizedManagePower
================
*/
// FUNCTION: STBC 0x00486fa0 ??0OptimizedManagePower@@QAE@PAVPreprocessingAI@@@Z
OptimizedManagePower::OptimizedManagePower( PreprocessingAI *ai ) : PreprocessingAI( ai->GetObject(), ai->name ) {
	containedAI = ai->containedAI;
	ai->containedAI = NULL;
	containedAI->SetParentID( GetID() );
	interruptable = ai->interruptable;
	PyEmbed::GetBoolAttribute( ai->preprocessingInstance, "bConservePower", &conservePower );
	PyErr_Print();
}

/*
================
OptimizedManagePower::GetAIType
================
*/
// FUNCTION: STBC 0x00487030
int OptimizedManagePower::GetAIType( void ) {
	return AI_TYPE_MANAGE_POWER;
}

/*
================
OptimizedManagePower::VirtualSlot20
================
*/
// FUNCTION: STBC 0x00487040
bool OptimizedManagePower::VirtualSlot20( int type ) {
	if ( type == AI_TYPE_MANAGE_POWER ) {
		return true;
	}
	return PreprocessingAI::VirtualSlot20( type );
}

/*
================
OptimizedManagePower::~OptimizedManagePower
================
*/
// FUNCTION: STBC 0x00487080
// FUNCTION: STBC 0x00487060 ??_GOptimizedManagePower@@UAEPAXI@Z
OptimizedManagePower::~OptimizedManagePower( void ) {
}

/*
================
OptimizedManagePower::CreateFromScript
================
*/
// FUNCTION: STBC 0x00487090
ArtificialIntelligence *OptimizedManagePower::CreateFromScript( PreprocessingAI *ai ) {
	ArtificialIntelligence *optimized = new OptimizedManagePower( ai );
	delete ai;
	return optimized;
}

/*
================
OptimizedManagePower::CallPreprocessingMethod

Shares the power when the time has come; the contained AI always runs.
================
*/
// FUNCTION: STBC 0x00487100
int OptimizedManagePower::CallPreprocessingMethod( double time ) {
	if ( !forceUpdate && g_kUtopiaModule.GetGameTime() < nextUpdateTime ) {
		return preprocessStatus;
	}
	ShipClass *ship = GetShip();
	if ( ship == NULL ) {
		return PS_DONE;
	}
	PowerSubsystem *power = ship->GetPowerSubsystem();
	if ( power == NULL ) {
		return PS_NORMAL;
	}
	nextUpdateTime = g_kUtopiaModule.GetGameTime() + UPDATE_INTERVAL;
	float level = GetBatteryLevel( power );
	SetPowerLevel( power, level );
	return PS_NORMAL;
}

/*
================
OptimizedManagePower::GetBatteryLevel
================
*/
// FUNCTION: STBC 0x00487180
float OptimizedManagePower::GetBatteryLevel( PowerSubsystem *power ) {
	float batteryPower = power->GetMainBatteryPower();
	float limit = power->GetMainBatteryLimit();
	float base = 0.0f;
	float threshold = limit * 0.9f;
	if ( conservePower ) {
		threshold = limit * 1.0625f;
	}
	float fraction;
	if ( batteryPower < threshold ) {
		fraction = 1.0f - ( threshold - batteryPower ) / threshold;
	} else {
		fraction = ( limit - batteryPower ) / ( limit - threshold );
		base = 1.0f - fraction;
	}
	return fraction * 0.5f + base;
}

/*
================
OptimizedManagePower::SetPowerLevel
================
*/
// FUNCTION: STBC 0x00487220
void OptimizedManagePower::SetPowerLevel( PowerSubsystem *power, float level ) {
	float mainPower = power->GetMainBatteryPower();
	if ( power->GetMainConduitCapacity() < mainPower ) {
		mainPower = power->GetMainConduitCapacity();
	}
	float backupPower = power->GetBackupBatteryPower();
	if ( power->GetBackupConduitCapacity() < backupPower ) {
		backupPower = power->GetBackupConduitCapacity();
	}
	float batteryPower = backupPower + mainPower;
	float shortfall = 0.0f;
	float batteryFraction = 0.0f;
	float outputFraction;
	if ( level < 0.5f ) {
		shortfall = ( 0.5f - level ) * 2.0f;
		outputFraction = 1.0f - shortfall;
	} else {
		outputFraction = ( 1.0f - level ) * 2.0f;
		batteryFraction = 1.0f - outputFraction;
	}
	float available = power->GetPowerOutput() * outputFraction
		+ power->GetPowerOutput() * shortfall * 0.1f
		+ batteryFraction * batteryPower;
	DistributePower( power, available );
}

/*
================
OptimizedManagePower::DistributePower
================
*/
// FUNCTION: STBC 0x00487330
void OptimizedManagePower::DistributePower( PowerSubsystem *power, float available ) {
	NiTList<PowerShare *> shares( 2 );
	float totalWeight = 0.0f;

	NiTList<PoweredSubsystem *> &subsystems = power->GetPoweredSubsystems();
	NiTListIterator pos = subsystems.GetHeadPos();
	while ( pos != NULL ) {
		PoweredSubsystem *subsystem = subsystems.GetNext( pos );
		if ( !subsystem->IsTypeOf( CLOAKINGSUBSYSTEM_TYPE ) ) {
			PowerShare *share = new PowerShare;
			share->weight = GetWeight( subsystem );
			share->subsystem = subsystem;
			totalWeight += share->weight;
			shares.AddTail( share );
		}
	}

	bool capped;
	do {
		capped = false;
		pos = shares.GetHeadPos();
		while ( pos != NULL ) {
			NiTListIterator sharePos = pos;
			PowerShare *share = shares.GetNext( pos );
			float wanted = share->weight / totalWeight * available;
			float percentage = 0.0f;
			float normalPower = share->subsystem->GetNormalPowerWanted();
			if ( normalPower != 0.0f ) {
				percentage = wanted / normalPower;
			}
			if ( percentage > MAX_POWER_PERCENTAGE ) {
				available -= MAX_POWER_PERCENTAGE * normalPower;
				totalWeight -= share->weight;
				share->subsystem->SetPowerPercentageWanted( MAX_POWER_PERCENTAGE );
				shares.RemovePos( sharePos );
				delete share;
				capped = true;
			} else {
				share->subsystem->SetPowerPercentageWanted( percentage );
			}
		}
	} while ( capped );

	while ( shares.GetSize() != 0 ) {
		delete shares.RemoveHead();
	}
}

/*
================
OptimizedManagePower::GetWeight
================
*/
// FUNCTION: STBC 0x004875d0
float OptimizedManagePower::GetWeight( PoweredSubsystem *subsystem ) {
	if ( subsystem->IsTypeOf( SENSORSUBSYSTEM_TYPE ) ) {
		return 0.25f;
	}
	if ( subsystem->IsTypeOf( WARPENGINESUBSYSTEM_TYPE ) ) {
		return 0.05f;
	}
	return 1.0f;
}

/*
================
OptimizedManagePower::WriteToStream
================
*/
// FUNCTION: STBC 0x00487620
bool OptimizedManagePower::WriteToStream( TGStream *stream ) {
	if ( !PreprocessingAI::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteBool( conservePower );
	return true;
}

/*
================
OptimizedManagePower::OptimizedManagePower
================
*/
// FUNCTION: STBC 0x00487650 ??0OptimizedManagePower@@QAE@PAVTGStream@@PA_N@Z
OptimizedManagePower::OptimizedManagePower( TGStream *stream, bool *ok ) : PreprocessingAI( stream, ok ) {
	if ( *ok ) {
		conservePower = stream->ReadBool();
	}
}

/*
================
OptimizedManagePower::Create
================
*/
// FUNCTION: STBC 0x004876c0
ArtificialIntelligence *OptimizedManagePower::Create( TGStream *stream ) {
	bool ok = true;
	OptimizedManagePower *ai = new OptimizedManagePower( stream, &ok );
	if ( !ok ) {
		delete ai;
		ai = NULL;
	}
	return ai;
}
