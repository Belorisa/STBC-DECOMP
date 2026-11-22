/*
===============================================================================

	TorpedoAmmoType.cpp

===============================================================================
*/

#include "TorpedoAmmoType.h"
#include "PyEmbed.h"

#include <string.h>

/*
================
TorpedoAmmoType::TorpedoAmmoType
================
*/
// FUNCTION: STBC 0x00694200
TorpedoAmmoType::TorpedoAmmoType( void ) {
	maxTorpedoes = 400;
	torpedoScript = NULL;
	launchSound = NULL;
	launchSpeed = 1.0f;
}

/*
================
TorpedoAmmoType::~TorpedoAmmoType
================
*/
// FUNCTION: STBC 0x00694220
TorpedoAmmoType::~TorpedoAmmoType( void ) {
	SetTorpedoScript( NULL );
	launchSound = NULL;
}

/*
================
TorpedoAmmoType::SetTorpedoScript

Replaces the script with a copy, and asks it for the launch speed and
sound.
================
*/
// FUNCTION: STBC 0x00694240
void TorpedoAmmoType::SetTorpedoScript( char *script ) {
	if ( torpedoScript != NULL ) {
		delete torpedoScript;
		torpedoScript = NULL;
	}
	launchSpeed = 1.0f;
	launchSound = NULL;
	if ( script != NULL ) {
		torpedoScript = new char[strlen( script ) + 1];
		strcpy( torpedoScript, script );
		PyEmbed::CallFunction( torpedoScript, "GetLaunchSpeed", "f", &launchSpeed, "" );
		PyEmbed::CallFunction( torpedoScript, "GetLaunchSound", "z", &launchSound, "" );
	}
}

/*
================
TorpedoAmmoType::GetPowerCost

The script's GetPowerCost; 0 without a script.
================
*/
// FUNCTION: STBC 0x006942f0
float TorpedoAmmoType::GetPowerCost( void ) {
	float powerCost = 0.0f;
	if ( torpedoScript != NULL ) {
		PyEmbed::CallFunction( torpedoScript, "GetPowerCost", "f", &powerCost, "" );
	}
	return powerCost;
}

/*
================
TorpedoAmmoType::GetAmmoName

The script's GetName; NULL without a script.
================
*/
// FUNCTION: STBC 0x00694330
char *TorpedoAmmoType::GetAmmoName( void ) {
	char *name = NULL;
	if ( torpedoScript != NULL ) {
		PyEmbed::CallFunction( torpedoScript, "GetName", "z", &name, "" );
	}
	return name;
}
