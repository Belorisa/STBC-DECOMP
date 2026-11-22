/*
===============================================================================

	PowerSubsystem.cpp

===============================================================================
*/

#include <iostream>
#include "PowerSubsystem.h"
#include "Game.h"
#include "PowerProperty.h"
#include "PoweredSubsystem.h"
#include "ShipClass.h"
#include "TGEventHandlerTable.h"
#include "TGStream.h"
#include "UtopiaModule.h"

#include <math.h>
#include <stdlib.h>
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x00564e60 _$E484
// FUNCTION: STBC 0x00564e80 _$E483

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00563380 _$E32
// FUNCTION: STBC 0x00563390 _$E29
// FUNCTION: STBC 0x005633a0 _$E31
// FUNCTION: STBC 0x005633b0 _$E30
// FUNCTION: STBC 0x005633c0 _$E38
// FUNCTION: STBC 0x005633d0 _$E35
// FUNCTION: STBC 0x005633e0 _$E37
// FUNCTION: STBC 0x005633f0 _$E36

// The handler table's dynamic initialiser and exit-time destructor.
// FUNCTION: STBC 0x00563420 _$E475
// FUNCTION: STBC 0x00563430 _$E471
// FUNCTION: STBC 0x00563440 _$E474
// FUNCTION: STBC 0x00563450 _$E472

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00563400 _$E85
// FUNCTION: STBC 0x00563410 _$E84
// FUNCTION: STBC 0x00563500 _$E286
// FUNCTION: STBC 0x00563510 _$E285
// FUNCTION: STBC 0x005645e0 _$E96
// FUNCTION: STBC 0x005645f0 _$E95
// FUNCTION: STBC 0x00564600 _$E100
// FUNCTION: STBC 0x00564610 _$E99
// FUNCTION: STBC 0x00564620 _$E104
// FUNCTION: STBC 0x00564630 _$E103
// FUNCTION: STBC 0x00564640 _$E108
// FUNCTION: STBC 0x00564650 _$E107
// FUNCTION: STBC 0x00564660 _$E225
// FUNCTION: STBC 0x00564670 _$E224
// FUNCTION: STBC 0x00564680 _$E167
// FUNCTION: STBC 0x00564690 _$E166
// FUNCTION: STBC 0x005646a0 _$E180
// FUNCTION: STBC 0x005646b0 _$E179
// FUNCTION: STBC 0x005646c0 _$E230
// FUNCTION: STBC 0x005646d0 _$E229
// FUNCTION: STBC 0x005646e0 _$E291
// FUNCTION: STBC 0x005646f0 _$E290
// FUNCTION: STBC 0x00564700 _$E234
// FUNCTION: STBC 0x00564710 _$E233
// FUNCTION: STBC 0x00564720 _$E295
// FUNCTION: STBC 0x00564730 _$E294
// FUNCTION: STBC 0x00564740 _$E302
// FUNCTION: STBC 0x00564750 _$E301
// FUNCTION: STBC 0x00564760 _$E320
// FUNCTION: STBC 0x00564770 _$E319
// FUNCTION: STBC 0x00564780 _$E324
// FUNCTION: STBC 0x00564790 _$E323
// FUNCTION: STBC 0x005647a0 _$E125
// FUNCTION: STBC 0x005647b0 _$E124
// FUNCTION: STBC 0x005647c0 _$E250
// FUNCTION: STBC 0x005647d0 _$E249
// FUNCTION: STBC 0x005647e0 _$E257
// FUNCTION: STBC 0x005647f0 _$E256
// FUNCTION: STBC 0x00564800 _$E261
// FUNCTION: STBC 0x00564810 _$E260
// FUNCTION: STBC 0x00564820 _$E265
// FUNCTION: STBC 0x00564830 _$E264
// FUNCTION: STBC 0x00564840 _$E269
// FUNCTION: STBC 0x00564850 _$E268
// FUNCTION: STBC 0x00564860 _$E275
// FUNCTION: STBC 0x00564870 _$E274
// FUNCTION: STBC 0x00564880 _$E328
// FUNCTION: STBC 0x00564890 _$E327
// FUNCTION: STBC 0x005648a0 _$E332
// FUNCTION: STBC 0x005648b0 _$E331
// FUNCTION: STBC 0x005648c0 _$E336
// FUNCTION: STBC 0x005648d0 _$E335
// FUNCTION: STBC 0x005648e0 _$E340
// FUNCTION: STBC 0x005648f0 _$E339
// FUNCTION: STBC 0x00564900 _$E344
// FUNCTION: STBC 0x00564910 _$E343
// FUNCTION: STBC 0x00564920 _$E348
// FUNCTION: STBC 0x00564930 _$E347
// FUNCTION: STBC 0x00564940 _$E352
// FUNCTION: STBC 0x00564950 _$E351
// FUNCTION: STBC 0x00564960 _$E356
// FUNCTION: STBC 0x00564970 _$E355
// FUNCTION: STBC 0x00564980 _$E360
// FUNCTION: STBC 0x00564990 _$E359
// FUNCTION: STBC 0x005649a0 _$E364
// FUNCTION: STBC 0x005649b0 _$E363
// FUNCTION: STBC 0x005649c0 _$E130
// FUNCTION: STBC 0x005649d0 _$E129
// FUNCTION: STBC 0x005649e0 _$E368
// FUNCTION: STBC 0x005649f0 _$E367
// FUNCTION: STBC 0x00564a00 _$E135
// FUNCTION: STBC 0x00564a10 _$E134
// FUNCTION: STBC 0x00564a20 _$E372
// FUNCTION: STBC 0x00564a30 _$E371
// FUNCTION: STBC 0x00564a40 _$E376
// FUNCTION: STBC 0x00564a50 _$E375
// FUNCTION: STBC 0x00564a60 _$E380
// FUNCTION: STBC 0x00564a70 _$E379
// FUNCTION: STBC 0x00564a80 _$E384
// FUNCTION: STBC 0x00564a90 _$E383
// FUNCTION: STBC 0x00564aa0 _$E144
// FUNCTION: STBC 0x00564ab0 _$E143
// FUNCTION: STBC 0x00564ac0 _$E388
// FUNCTION: STBC 0x00564ad0 _$E387
// FUNCTION: STBC 0x00564ae0 _$E392
// FUNCTION: STBC 0x00564af0 _$E391
// FUNCTION: STBC 0x00564b00 _$E396
// FUNCTION: STBC 0x00564b10 _$E395
// FUNCTION: STBC 0x00564b20 _$E150
// FUNCTION: STBC 0x00564b30 _$E149
// FUNCTION: STBC 0x00564b40 _$E155
// FUNCTION: STBC 0x00564b50 _$E154
// FUNCTION: STBC 0x00564b60 _$E185
// FUNCTION: STBC 0x00564b70 _$E184
// FUNCTION: STBC 0x00564b80 _$E400
// FUNCTION: STBC 0x00564b90 _$E399
// FUNCTION: STBC 0x00564ba0 _$E190
// FUNCTION: STBC 0x00564bb0 _$E189
// FUNCTION: STBC 0x00564bc0 _$E404
// FUNCTION: STBC 0x00564bd0 _$E403
// FUNCTION: STBC 0x00564be0 _$E408
// FUNCTION: STBC 0x00564bf0 _$E407
// FUNCTION: STBC 0x00564c00 _$E195
// FUNCTION: STBC 0x00564c10 _$E194
// FUNCTION: STBC 0x00564c20 _$E412
// FUNCTION: STBC 0x00564c30 _$E411
// FUNCTION: STBC 0x00564c40 _$E160
// FUNCTION: STBC 0x00564c50 _$E159
// FUNCTION: STBC 0x00564c60 _$E416
// FUNCTION: STBC 0x00564c70 _$E415
// FUNCTION: STBC 0x00564c80 _$E420
// FUNCTION: STBC 0x00564c90 _$E419
// FUNCTION: STBC 0x00564ca0 _$E200
// FUNCTION: STBC 0x00564cb0 _$E199
// FUNCTION: STBC 0x00564cc0 _$E205
// FUNCTION: STBC 0x00564cd0 _$E204
// FUNCTION: STBC 0x00564ce0 _$E424
// FUNCTION: STBC 0x00564cf0 _$E423
// FUNCTION: STBC 0x00564d00 _$E428
// FUNCTION: STBC 0x00564d10 _$E427
// FUNCTION: STBC 0x00564d20 _$E432
// FUNCTION: STBC 0x00564d30 _$E431
// FUNCTION: STBC 0x00564d40 _$E436
// FUNCTION: STBC 0x00564d50 _$E435
// FUNCTION: STBC 0x00564d60 _$E440
// FUNCTION: STBC 0x00564d70 _$E439
// FUNCTION: STBC 0x00564d80 _$E444
// FUNCTION: STBC 0x00564d90 _$E443
// FUNCTION: STBC 0x00564da0 _$E448
// FUNCTION: STBC 0x00564db0 _$E447
// FUNCTION: STBC 0x00564dc0 _$E452
// FUNCTION: STBC 0x00564dd0 _$E451
// FUNCTION: STBC 0x00564de0 _$E210
// FUNCTION: STBC 0x00564df0 _$E209
// FUNCTION: STBC 0x00564e00 _$E456
// FUNCTION: STBC 0x00564e10 _$E455
// FUNCTION: STBC 0x00564e20 _$E215
// FUNCTION: STBC 0x00564e30 _$E214
// FUNCTION: STBC 0x00564e40 _$E220
// FUNCTION: STBC 0x00564e50 _$E219
// FUNCTION: STBC 0x00564ee0 _$E460
// FUNCTION: STBC 0x00564ef0 _$E459
// FUNCTION: STBC 0x00564f00 _$E465
// FUNCTION: STBC 0x00564f10 _$E464
// FUNCTION: STBC 0x00564f20 _$E469
// FUNCTION: STBC 0x00564f30 _$E468

// DATA: STBC 0x00892e20
const float PowerSubsystem::POWER_UPDATE_INTERVAL = 1.0f;

// DATA: STBC 0x0098bdb0
TGEventHandlerTable PowerSubsystem::classHandlers;

/*
================
PowerSubsystem::Cast

The object as a PowerSubsystem; NULL when it is none.
================
*/
// FUNCTION: STBC 0x00563470
PowerSubsystem *PowerSubsystem::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( POWERSUBSYSTEM_TYPE ) ) {
		return (PowerSubsystem *)object;
	}
	return NULL;
}

/*
================
PowerSubsystem::GetProperty
================
*/
// FUNCTION: STBC 0x005634a0
PowerProperty *PowerSubsystem::GetProperty( void ) {
	return (PowerProperty *)property;
}

/*
================
PowerSubsystem::GetPowerOutput

The property's output, scaled by the plant's condition.
================
*/
// FUNCTION: STBC 0x005634b0
float PowerSubsystem::GetPowerOutput( void ) {
	return GetProperty()->GetPowerOutput() * conditionPercentage;
}

/*
================
PowerSubsystem::GetMainBatteryLimit
================
*/
// FUNCTION: STBC 0x005634c0
float PowerSubsystem::GetMainBatteryLimit( void ) {
	return GetProperty()->GetMainBatteryLimit();
}

/*
================
PowerSubsystem::GetBackupBatteryLimit
================
*/
// FUNCTION: STBC 0x005634d0
float PowerSubsystem::GetBackupBatteryLimit( void ) {
	return GetProperty()->GetBackupBatteryLimit();
}

/*
================
PowerSubsystem::GetMaxMainConduitCapacity

The main conduit's capacity when undamaged.
================
*/
// FUNCTION: STBC 0x005634e0
float PowerSubsystem::GetMaxMainConduitCapacity( void ) {
	return GetProperty()->GetMainConduitCapacity();
}

/*
================
PowerSubsystem::GetMainConduitCapacity

The main conduit's capacity, scaled by the plant's condition.
================
*/
// FUNCTION: STBC 0x005634f0
float PowerSubsystem::GetMainConduitCapacity( void ) {
	return GetProperty()->GetMainConduitCapacity() * conditionPercentage;
}

/*
================
PowerSubsystem::GetBackupConduitCapacity
================
*/
// FUNCTION: STBC 0x00563520
float PowerSubsystem::GetBackupConduitCapacity( void ) {
	return GetProperty()->GetBackupConduitCapacity();
}

/*
================
PowerSubsystem::PowerSubsystem

A power plant with no power and empty batteries; the first Update runs
the plant at once.
================
*/
// FUNCTION: STBC 0x00563530
PowerSubsystem::PowerSubsystem( void ) : poweredSubsystems( 2 ) {
	availablePower = 0.0f;
	mainConduitPower = 0.0f;
	backupConduitPower = 0.0f;
	mainBatteryPower = 0.0f;
	backupBatteryPower = 0.0f;
	powerDispensed = 0.0f;
	lastPowerUpdateTime = g_kUtopiaModule.GetGameTime() - POWER_UPDATE_INTERVAL;
	mainBatteryPercentage = 0.0f;
	backupBatteryPercentage = 0.0f;
	mainBatteryWatcher.SetVariable( &mainBatteryPercentage );
	backupBatteryWatcher.SetVariable( &backupBatteryPercentage );
}

/*
================
PowerSubsystem::~PowerSubsystem
================
*/
// FUNCTION: STBC 0x00563610
// FUNCTION: STBC 0x004401d0 ??_GPowerSubsystem@@UAEPAXI@Z
PowerSubsystem::~PowerSubsystem( void ) {
}

/*
================
PowerSubsystem::SetParentShip

The batteries of a power plant fitted to a ship start full.
================
*/
// FUNCTION: STBC 0x005636d0
void PowerSubsystem::SetParentShip( ShipClass *ship ) {
	ShipSubsystem::SetParentShip( ship );
	mainBatteryPower = GetMainBatteryLimit();
	backupBatteryPower = GetBackupBatteryLimit();
}

/*
================
PowerSubsystem::DrawBatteryPower

What the batteries can give over the turns: each battery gives what it
holds, at most its conduit's capacity per turn. The two shares are kept
apart; the sum is returned. The name is invented.
================
*/
// FUNCTION: STBC 0x00563700
// PARKED: FPU allocation: retail keeps the sum on the FPU stack and subtracts mainConduitPower from memory; ours spills the sum to the parameter slot (chained assignment, if/else and parameter reuse tried)
float PowerSubsystem::DrawBatteryPower( float turns ) {
	float mainLimit = GetMainConduitCapacity() * turns;
	float backupLimit = GetBackupConduitCapacity() * turns;

	mainConduitPower = mainBatteryPower >= mainLimit ? mainLimit : mainBatteryPower;
	float total = mainConduitPower + ( backupBatteryPower >= backupLimit ? backupLimit : backupBatteryPower );
	backupConduitPower = total - mainConduitPower;
	return total;
}

/*
================
PowerSubsystem::Update

Runs the power plant once for every POWER_UPDATE_INTERVAL seconds of game
time that have passed: a working plant adds its output for those turns,
and the batteries offer what their conduits carry. The battery watchers
then see the batteries' levels as fractions of their limits.
================
*/
// FUNCTION: STBC 0x00563780
void PowerSubsystem::Update( float deltaTime ) {
	ShipSubsystem::Update( deltaTime );

	float now = UtopiaApp::GetApp()->GetGameTime();
	if ( now < lastPowerUpdateTime ) {
		lastPowerUpdateTime = now;
	}
	if ( now - lastPowerUpdateTime > POWER_UPDATE_INTERVAL ) {
		powerDispensed = 0.0f;
		int turns = (int)( ( now - lastPowerUpdateTime ) / POWER_UPDATE_INTERVAL );
		if ( !IsDisabled() ) {
			AddPower( GetPowerOutput() * turns );
		}
		availablePower = DrawBatteryPower( turns );
		lastPowerUpdateTime = now - (float)fmod( now - lastPowerUpdateTime, POWER_UPDATE_INTERVAL );
	}

	mainBatteryPercentage = mainBatteryPower;
	if ( GetMainBatteryLimit() > 0.0f ) {
		mainBatteryPercentage /= GetMainBatteryLimit();
	}
	backupBatteryPercentage = backupBatteryPower;
	if ( GetBackupBatteryLimit() > 0.0f ) {
		backupBatteryPercentage /= GetBackupBatteryLimit();
	}
	mainBatteryWatcher.CheckRanges();
	backupBatteryWatcher.CheckRanges();
}

/*
================
PowerSubsystem::AddPower

Adds power to the batteries: the main battery first, then what it can't
hold goes to the backup battery, and what that can't hold goes back to
the main one. A client leaves the power to the host.
================
*/
// FUNCTION: STBC 0x005638d0
void PowerSubsystem::AddPower( float amount ) {
	if ( g_kUtopiaModule.IsMultiplayer() && !g_kUtopiaModule.IsHost() ) {
		return;
	}

	float overflow = 0.0f;
	float mainPower = GetMainBatteryPower();
	float mainRoom = GetMainBatteryLimit() - mainPower;
	if ( amount < mainRoom ) {
		mainBatteryPower += amount;
		availablePower += amount;
	} else {
		mainBatteryPower = GetMainBatteryLimit();
		availablePower += mainRoom;
		overflow = amount - mainRoom;
	}

	float rest = 0.0f;
	float backupPower = GetBackupBatteryPower();
	float backupRoom = GetBackupBatteryLimit() - backupPower;
	if ( overflow < backupRoom ) {
		backupBatteryPower += overflow;
		availablePower += overflow;
	} else {
		backupBatteryPower = GetBackupBatteryLimit();
		availablePower += backupRoom;
		rest = overflow - backupRoom;
	}

	float newMainPower = GetMainBatteryPower();
	mainRoom = GetMainBatteryLimit() - newMainPower;
	if ( mainRoom > 0.0f ) {
		if ( rest < mainRoom ) {
			mainBatteryPower += rest;
			availablePower += rest;
		} else {
			mainBatteryPower = GetMainBatteryLimit();
			availablePower += mainRoom;
		}
	}
}

/*
================
PowerSubsystem::DeductPower

Takes power from the conduits and, behind them, the batteries; returns
what was taken. A client doesn't touch the batteries, and the host
reports the full amount for a ship a remote player flies.
================
*/
// FUNCTION: STBC 0x00563a70
float PowerSubsystem::DeductPower( float amount ) {
	bool takesPower = true;
	bool reportsTaken = true;
	if ( g_kUtopiaModule.IsMultiplayer() ) {
		ShipClass *player = Game::GetCurrentPlayer();
		bool isHost = g_kUtopiaModule.IsHost();
		ShipClass *ship = GetParentShip();
		if ( isHost ) {
			if ( ship != player && ship->GetNetPlayerID() != 0 ) {
				reportsTaken = false;
			}
		} else {
			if ( ship != player ) {
				reportsTaken = false;
			}
			takesPower = false;
		}
	}

	if ( !( mainConduitPower < amount ) ) {
		mainConduitPower -= amount;
		if ( takesPower ) {
			mainBatteryPower -= amount;
			powerDispensed += amount;
			return amount;
		}
	} else {
		float fromMain = mainConduitPower;
		float rest = amount - fromMain;
		mainConduitPower = 0.0f;
		if ( takesPower ) {
			mainBatteryPower -= fromMain;
		}
		if ( !( backupConduitPower < rest ) ) {
			backupConduitPower -= rest;
			if ( takesPower ) {
				backupBatteryPower -= rest;
				powerDispensed += amount;
			}
		} else {
			backupConduitPower = 0.0f;
			if ( takesPower ) {
				// BUG: the backup conduit is zeroed before its power is taken from the
				// battery, so this subtracts nothing (retail stores the battery back unchanged)
				backupBatteryPower -= backupConduitPower;
				powerDispensed += fromMain;
			}
			if ( reportsTaken ) {
				return fromMain;
			}
		}
	}
	return amount;
}

/*
================
PowerSubsystem::StealPower

Takes power straight from the batteries, the main one first; returns
what was taken.
================
*/
// FUNCTION: STBC 0x00563bb0
float PowerSubsystem::StealPower( float amount ) {
	bool takesPower = true;
	bool reportsTaken = true;
	if ( g_kUtopiaModule.IsMultiplayer() ) {
		ShipClass *player = Game::GetCurrentPlayer();
		bool isHost = g_kUtopiaModule.IsHost();
		ShipClass *ship = GetParentShip();
		if ( isHost ) {
			if ( ship != player && ship->GetNetPlayerID() != 0 ) {
				reportsTaken = false;
			}
		} else {
			if ( ship != player ) {
				reportsTaken = false;
			}
			takesPower = false;
		}
	}

	if ( GetMainBatteryPower() >= amount ) {
		if ( takesPower ) {
			mainBatteryPower -= amount;
		}
	} else {
		float fromMain = mainBatteryPower;
		if ( takesPower ) {
			mainBatteryPower = 0.0f;
		}
		float available = fromMain + backupBatteryPower;
		if ( available >= amount ) {
			if ( takesPower ) {
				backupBatteryPower -= amount - fromMain;
			}
		} else {
			if ( takesPower ) {
				backupBatteryPower = 0.0f;
			}
			if ( reportsTaken ) {
				return available;
			}
		}
	}
	return amount;
}

/*
================
PowerSubsystem::StealPowerFromReserve

Takes power from the backup battery alone; returns what was taken.
================
*/
// FUNCTION: STBC 0x00563cb0
float PowerSubsystem::StealPowerFromReserve( float amount ) {
	bool takesPower = true;
	bool reportsTaken = true;
	if ( g_kUtopiaModule.IsMultiplayer() ) {
		ShipClass *player = Game::GetCurrentPlayer();
		bool isHost = g_kUtopiaModule.IsHost();
		ShipClass *ship = GetParentShip();
		if ( isHost ) {
			if ( ship != player && ship->GetNetPlayerID() != 0 ) {
				reportsTaken = false;
			}
		} else {
			if ( ship != player ) {
				reportsTaken = false;
			}
			takesPower = false;
		}
	}

	if ( !( backupBatteryPower < amount ) ) {
		if ( takesPower ) {
			backupBatteryPower -= amount;
		}
		return amount;
	}

	float available = backupBatteryPower;
	if ( takesPower ) {
		backupBatteryPower = 0.0f;
	}
	if ( reportsTaken ) {
		return available;
	} else {
		return amount;
	}
}

/*
================
PowerSubsystem::AddPoweredSubsystem

Adds the subsystem to those this one powers.
================
*/
// FUNCTION: STBC 0x00563d50
void PowerSubsystem::AddPoweredSubsystem( PoweredSubsystem *subsystem ) {
	poweredSubsystems.AddTail( subsystem );
}

/*
================
PowerSubsystem::RemovePoweredSubsystem

Takes the subsystem off those this one powers. Nothing calls it.
================
*/
// FUNCTION: STBC 0x00563dc0
void PowerSubsystem::RemovePoweredSubsystem( PoweredSubsystem *subsystem ) {
	poweredSubsystems.Remove( subsystem );
}

/*
================
PowerSubsystem::GetPowerWanted

The power the powered subsystems want over a number of turns.
================
*/
// FUNCTION: STBC 0x00563ed0
float PowerSubsystem::GetPowerWanted( float turns ) {
	float powerWanted = 0.0f;
	NiTListIterator pos = poweredSubsystems.GetHeadPos();
	while ( pos ) {
		PoweredSubsystem *subsystem = poweredSubsystems.GetNext( pos );
		powerWanted += subsystem->GetPowerWanted() * turns;
	}
	return powerWanted;
}

/*
================
PowerSubsystem::WriteToStream

After ShipSubsystem's state, the power figures, the IDs of the powered
subsystems ended by 0, and the two battery watchers each followed by
its variable.
================
*/
// FUNCTION: STBC 0x00563f00
bool PowerSubsystem::WriteToStream( TGStream *stream ) {
	if ( !ShipSubsystem::WriteToStream( stream ) ) {
		return false;
	}

	stream->WriteFloat( availablePower );
	stream->WriteFloat( mainConduitPower );
	stream->WriteFloat( backupConduitPower );
	stream->WriteFloat( mainBatteryPower );
	stream->WriteFloat( backupBatteryPower );

	NiTListIterator pos = poweredSubsystems.GetHeadPos();
	while ( pos ) {
		PoweredSubsystem *subsystem = poweredSubsystems.GetNext( pos );
		if ( subsystem ) {
			stream->WriteID( subsystem->GetObjID() );
		}
	}
	stream->WriteID( 0 );

	stream->WriteFloat( powerDispensed );
	stream->WriteFloat( lastPowerUpdateTime );
	if ( !mainBatteryWatcher.WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteFloat( mainBatteryPercentage );
	if ( !backupBatteryWatcher.WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteFloat( backupBatteryPercentage );
	return true;
}

/*
================
PowerSubsystem::ReadFromStream

Reads what WriteToStream writes. The powered subsystems are held as
their IDs until PostFixUp turns them back into pointers.
================
*/
// FUNCTION: STBC 0x00564000
bool PowerSubsystem::ReadFromStream( TGStream *stream ) {
	if ( !ShipSubsystem::ReadFromStream( stream ) ) {
		return false;
	}

	availablePower = stream->ReadFloat();
	mainConduitPower = stream->ReadFloat();
	backupConduitPower = stream->ReadFloat();
	mainBatteryPower = stream->ReadFloat();
	backupBatteryPower = stream->ReadFloat();

	long id = stream->ReadID();
	while ( id ) {
		poweredSubsystems.AddTail( (PoweredSubsystem *)id );
		id = stream->ReadID();
	}

	powerDispensed = stream->ReadFloat();
	lastPowerUpdateTime = stream->ReadFloat();
	mainBatteryWatcher.SetVariable( &mainBatteryPercentage );
	if ( !mainBatteryWatcher.ReadFromStream( stream ) ) {
		return false;
	}
	mainBatteryPercentage = stream->ReadFloat();
	backupBatteryWatcher.SetVariable( &backupBatteryPercentage );
	if ( !backupBatteryWatcher.ReadFromStream( stream ) ) {
		return false;
	}
	backupBatteryPercentage = stream->ReadFloat();
	return true;
}

/*
================
PowerSubsystem::FixUpPointers

Fails until every powered subsystem read by ReadFromStream exists.
================
*/
// FUNCTION: STBC 0x00564140
bool PowerSubsystem::FixUpPointers( void ) {
	if ( !ShipSubsystem::FixUpPointers() ) {
		return false;
	}

	NiTListIterator pos = poweredSubsystems.GetHeadPos();
	while ( pos ) {
		if ( !TGObject::GetTGObjectPtr( (long)poweredSubsystems.GetNext( pos ) ) ) {
			return false;
		}
	}

	if ( !mainBatteryWatcher.FixUpPointers() ) {
		return false;
	}
	if ( !backupBatteryWatcher.FixUpPointers() ) {
		return false;
	}
	return true;
}

/*
================
PowerSubsystem::PostFixUp

Turns the IDs of the powered subsystems back into pointers, keeping
their order.
================
*/
// FUNCTION: STBC 0x005641a0
void PowerSubsystem::PostFixUp( void ) {
	ShipSubsystem::PostFixUp();

	NiTList<PoweredSubsystem *> subsystems( 2 );
	NiTListIterator pos = poweredSubsystems.GetHeadPos();
	while ( pos ) {
		long id = (long)poweredSubsystems.GetNext( pos );
		subsystems.AddTail( (PoweredSubsystem *)TGObject::GetTGObjectPtr( id ) );
	}

	poweredSubsystems.RemoveAll();
	pos = subsystems.GetHeadPos();
	while ( pos ) {
		poweredSubsystems.AddTail( subsystems.GetNext( pos ) );
	}

	mainBatteryWatcher.PostFixUp();
	backupBatteryWatcher.PostFixUp();
}

/*
================
PowerSubsystem::VirtualSlot26

After ShipSubsystem's state, the charge of the main and the backup
battery, in 255ths of their limits; the host writes them full.
================
*/
// FUNCTION: STBC 0x005643a0
void PowerSubsystem::VirtualSlot26( TGStream *stream ) {
	ShipSubsystem::VirtualSlot26( stream );

	unsigned char mainCharge;
	unsigned char backupCharge;
	if ( g_kUtopiaModule.IsHost() ) {
		mainCharge = 255;
		backupCharge = 255;
	} else {
		mainCharge = (unsigned char)( mainBatteryPower / GetMainBatteryLimit() * 255.0f );
		backupCharge = (unsigned char)( backupBatteryPower / GetBackupBatteryLimit() * 255.0f );
	}
	stream->WriteChar( mainCharge );
	stream->WriteChar( backupCharge );
	stream->GetPos();
}

/*
================
PowerSubsystem::VirtualSlot27

Reads what VirtualSlot26 writes.
================
*/
// FUNCTION: STBC 0x00564420
void PowerSubsystem::VirtualSlot27( TGStream *stream ) {
	ShipSubsystem::VirtualSlot27( stream );

	unsigned char mainCharge = stream->ReadChar();
	unsigned char backupCharge = stream->ReadChar();
	mainBatteryPower = GetMainBatteryLimit() * ( mainCharge * ( 1.0f / 255.0f ) );
	backupBatteryPower = GetBackupBatteryLimit() * ( backupCharge * ( 1.0f / 255.0f ) );
	stream->GetPos();
}

/*
================
PowerSubsystem::VirtualSlot28

After ShipSubsystem's state, the charge of both batteries, as
VirtualSlot26 writes it for a client.
================
*/
// FUNCTION: STBC 0x005644b0
void PowerSubsystem::VirtualSlot28( TGStream *stream, bool flag ) {
	ShipSubsystem::VirtualSlot28( stream, flag );

	unsigned char mainCharge = (unsigned char)( mainBatteryPower / GetMainBatteryLimit() * 255.0f );
	unsigned char backupCharge = (unsigned char)( backupBatteryPower / GetBackupBatteryLimit() * 255.0f );
	stream->WriteChar( mainCharge );
	stream->WriteChar( backupCharge );
	stream->GetPos();
}

/*
================
PowerSubsystem::VirtualSlot29

Reads what VirtualSlot28 writes; the charges are taken only when the
state is newer than the last one taken.
================
*/
// FUNCTION: STBC 0x00564530
void PowerSubsystem::VirtualSlot29( TGStream *stream, float time ) {
	float lastTime = unknown84;
	ShipSubsystem::VirtualSlot29( stream, time );

	unsigned char mainCharge = stream->ReadChar();
	unsigned char backupCharge = stream->ReadChar();
	if ( time > lastTime ) {
		mainBatteryPower = GetMainBatteryLimit() * ( mainCharge * ( 1.0f / 255.0f ) );
		backupBatteryPower = GetBackupBatteryLimit() * ( backupCharge * ( 1.0f / 255.0f ) );
	}
	stream->GetPos();
}
