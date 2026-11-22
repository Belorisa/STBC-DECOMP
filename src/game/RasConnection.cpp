/*
===============================================================================

	RasConnection.cpp

	The dial-up check behind UtopiaModule::DetermineModemConnection. The
	unit's name is invented; the function is the only code of its unit
	before the static initialisers of the headers it includes.

===============================================================================
*/

#include <iostream>
#include <windows.h>
#include <ras.h>

#include "UtopiaModule.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x006a8600 _$E432
// FUNCTION: STBC 0x006a8620 _$E431

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x006a8280 _$E32
// FUNCTION: STBC 0x006a8290 _$E29
// FUNCTION: STBC 0x006a82a0 _$E31
// FUNCTION: STBC 0x006a82b0 _$E30
// FUNCTION: STBC 0x006a82c0 _$E38
// FUNCTION: STBC 0x006a82d0 _$E35
// FUNCTION: STBC 0x006a82e0 _$E37
// FUNCTION: STBC 0x006a82f0 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x006a8420 _$E200
// FUNCTION: STBC 0x006a8430 _$E199
// FUNCTION: STBC 0x006a84c0 _$E257
// FUNCTION: STBC 0x006a84d0 _$E256
// FUNCTION: STBC 0x006a84e0 _$E261
// FUNCTION: STBC 0x006a84f0 _$E260
// FUNCTION: STBC 0x006a8520 _$E243
// FUNCTION: STBC 0x006a8530 _$E242
// FUNCTION: STBC 0x006a8540 _$E250
// FUNCTION: STBC 0x006a8550 _$E249
// FUNCTION: STBC 0x006a8560 _$E273
// FUNCTION: STBC 0x006a8570 _$E272
// FUNCTION: STBC 0x006a8580 _$E277
// FUNCTION: STBC 0x006a8590 _$E276
// FUNCTION: STBC 0x006a85a0 _$E317
// FUNCTION: STBC 0x006a85b0 _$E316
// FUNCTION: STBC 0x006a85c0 _$E323
// FUNCTION: STBC 0x006a85d0 _$E322
// FUNCTION: STBC 0x006a85e0 _$E265
// FUNCTION: STBC 0x006a85f0 _$E264
// FUNCTION: STBC 0x006a8630 _$E269
// FUNCTION: STBC 0x006a8640 _$E268
// FUNCTION: STBC 0x006a8650 _$E281
// FUNCTION: STBC 0x006a8660 _$E280
// FUNCTION: STBC 0x006a8670 _$E285
// FUNCTION: STBC 0x006a8680 _$E284
// FUNCTION: STBC 0x006a8690 _$E289
// FUNCTION: STBC 0x006a86a0 _$E288
// FUNCTION: STBC 0x006a86b0 _$E293
// FUNCTION: STBC 0x006a86c0 _$E292
// FUNCTION: STBC 0x006a86d0 _$E297
// FUNCTION: STBC 0x006a86e0 _$E296
// FUNCTION: STBC 0x006a86f0 _$E301
// FUNCTION: STBC 0x006a8700 _$E300
// FUNCTION: STBC 0x006a8710 _$E305
// FUNCTION: STBC 0x006a8720 _$E304
// FUNCTION: STBC 0x006a8730 _$E309
// FUNCTION: STBC 0x006a8740 _$E308
// FUNCTION: STBC 0x006a8770 _$E313
// FUNCTION: STBC 0x006a8780 _$E312
// FUNCTION: STBC 0x006a87b0 _$E327
// FUNCTION: STBC 0x006a87c0 _$E326
// FUNCTION: STBC 0x006a87d0 _$E331
// FUNCTION: STBC 0x006a87e0 _$E330
// FUNCTION: STBC 0x006a87f0 _$E335
// FUNCTION: STBC 0x006a8800 _$E334
// FUNCTION: STBC 0x006a8810 _$E339
// FUNCTION: STBC 0x006a8820 _$E338
// FUNCTION: STBC 0x006a8850 _$E343
// FUNCTION: STBC 0x006a8860 _$E342
// FUNCTION: STBC 0x006a8870 _$E347
// FUNCTION: STBC 0x006a8880 _$E346
// FUNCTION: STBC 0x006a8890 _$E351
// FUNCTION: STBC 0x006a88a0 _$E350
// FUNCTION: STBC 0x006a8910 _$E355
// FUNCTION: STBC 0x006a8920 _$E354
// FUNCTION: STBC 0x006a8950 _$E359
// FUNCTION: STBC 0x006a8960 _$E358
// FUNCTION: STBC 0x006a8970 _$E363
// FUNCTION: STBC 0x006a8980 _$E362
// FUNCTION: STBC 0x006a89b0 _$E367
// FUNCTION: STBC 0x006a89c0 _$E366
// FUNCTION: STBC 0x006a89f0 _$E371
// FUNCTION: STBC 0x006a8a00 _$E370
// FUNCTION: STBC 0x006a8a10 _$E375
// FUNCTION: STBC 0x006a8a20 _$E374
// FUNCTION: STBC 0x006a8a70 _$E379
// FUNCTION: STBC 0x006a8a80 _$E378
// FUNCTION: STBC 0x006a8a90 _$E383
// FUNCTION: STBC 0x006a8aa0 _$E382
// FUNCTION: STBC 0x006a8ab0 _$E387
// FUNCTION: STBC 0x006a8ac0 _$E386
// FUNCTION: STBC 0x006a8ad0 _$E391
// FUNCTION: STBC 0x006a8ae0 _$E390
// FUNCTION: STBC 0x006a8af0 _$E395
// FUNCTION: STBC 0x006a8b00 _$E394
// FUNCTION: STBC 0x006a8b10 _$E399
// FUNCTION: STBC 0x006a8b20 _$E398
// FUNCTION: STBC 0x006a8b30 _$E403
// FUNCTION: STBC 0x006a8b40 _$E402
// FUNCTION: STBC 0x006a8b50 _$E407
// FUNCTION: STBC 0x006a8b60 _$E406
// FUNCTION: STBC 0x006a8b90 _$E411
// FUNCTION: STBC 0x006a8ba0 _$E410
// FUNCTION: STBC 0x006a8300 _$E52
// FUNCTION: STBC 0x006a8310 _$E51
// FUNCTION: STBC 0x006a8320 _$E208
// FUNCTION: STBC 0x006a8330 _$E207
// FUNCTION: STBC 0x006a8340 _$E63
// FUNCTION: STBC 0x006a8350 _$E62
// FUNCTION: STBC 0x006a8360 _$E67
// FUNCTION: STBC 0x006a8370 _$E66
// FUNCTION: STBC 0x006a8380 _$E71
// FUNCTION: STBC 0x006a8390 _$E70
// FUNCTION: STBC 0x006a83a0 _$E75
// FUNCTION: STBC 0x006a83b0 _$E74
// FUNCTION: STBC 0x006a83c0 _$E195
// FUNCTION: STBC 0x006a83d0 _$E194
// FUNCTION: STBC 0x006a83e0 _$E137
// FUNCTION: STBC 0x006a83f0 _$E136
// FUNCTION: STBC 0x006a8400 _$E150
// FUNCTION: STBC 0x006a8410 _$E149
// FUNCTION: STBC 0x006a8440 _$E213
// FUNCTION: STBC 0x006a8450 _$E212
// FUNCTION: STBC 0x006a8460 _$E204
// FUNCTION: STBC 0x006a8470 _$E203
// FUNCTION: STBC 0x006a8480 _$E217
// FUNCTION: STBC 0x006a8490 _$E216
// FUNCTION: STBC 0x006a84a0 _$E224
// FUNCTION: STBC 0x006a84b0 _$E223
// FUNCTION: STBC 0x006a8500 _$E99
// FUNCTION: STBC 0x006a8510 _$E98
// FUNCTION: STBC 0x006a8750 _$E104
// FUNCTION: STBC 0x006a8760 _$E103
// FUNCTION: STBC 0x006a8790 _$E109
// FUNCTION: STBC 0x006a87a0 _$E108
// FUNCTION: STBC 0x006a8830 _$E114
// FUNCTION: STBC 0x006a8840 _$E113
// FUNCTION: STBC 0x006a88b0 _$E120
// FUNCTION: STBC 0x006a88c0 _$E119
// FUNCTION: STBC 0x006a88d0 _$E125
// FUNCTION: STBC 0x006a88e0 _$E124
// FUNCTION: STBC 0x006a88f0 _$E155
// FUNCTION: STBC 0x006a8900 _$E154
// FUNCTION: STBC 0x006a8930 _$E160
// FUNCTION: STBC 0x006a8940 _$E159
// FUNCTION: STBC 0x006a8990 _$E165
// FUNCTION: STBC 0x006a89a0 _$E164
// FUNCTION: STBC 0x006a89d0 _$E130
// FUNCTION: STBC 0x006a89e0 _$E129
// FUNCTION: STBC 0x006a8a30 _$E170
// FUNCTION: STBC 0x006a8a40 _$E169
// FUNCTION: STBC 0x006a8a50 _$E175
// FUNCTION: STBC 0x006a8a60 _$E174
// FUNCTION: STBC 0x006a8b70 _$E180
// FUNCTION: STBC 0x006a8b80 _$E179
// FUNCTION: STBC 0x006a8bb0 _$E185
// FUNCTION: STBC 0x006a8bc0 _$E184
// FUNCTION: STBC 0x006a8bd0 _$E190
// FUNCTION: STBC 0x006a8be0 _$E189
// FUNCTION: STBC 0x006a8bf0 _$E415
// FUNCTION: STBC 0x006a8c00 _$E414
// FUNCTION: STBC 0x006a8c10 _$E420
// FUNCTION: STBC 0x006a8c20 _$E419
// FUNCTION: STBC 0x006a8c30 _$E424
// FUNCTION: STBC 0x006a8c40 _$E423

typedef DWORD ( APIENTRY *RasGetConnectStatusFunction )( HRASCONN, LPRASCONNSTATUSA );
typedef DWORD ( APIENTRY *RasEnumConnectionsFunction )( LPRASCONNA, LPDWORD, LPDWORD );

/*
================
GetRasConnectionStatus

0 when a dial-up connection is connected, 1 when none is, 2 when the RAS
library can't be loaded and 3 when the connections can't be listed.
Only the first connection fits the buffer.
================
*/
// FUNCTION: STBC 0x006a8170
int GetRasConnectionStatus( void ) {
	RasGetConnectStatusFunction getConnectStatus;
	RasEnumConnectionsFunction enumConnections;

	HINSTANCE library = LoadLibraryA( "RASAPI32.DLL" );
	if ( library != NULL ) {
		getConnectStatus = (RasGetConnectStatusFunction)GetProcAddress( library, "RasGetConnectStatusA" );
		enumConnections = (RasEnumConnectionsFunction)GetProcAddress( library, "RasEnumConnectionsA" );
	}
	// BUG: without the library both pointers are read uninitialised.
	if ( getConnectStatus == NULL || enumConnections == NULL ) {
		return 2;
	}

	DWORD count = 0;
	DWORD size = sizeof( RASCONNA );
	RASCONNA *connection = (RASCONNA *)GlobalAlloc( GPTR, sizeof( RASCONNA ) );
	connection->dwSize = size;
	RASCONNSTATUSA *status = NULL;
	int result = 1;
	if ( enumConnections( connection, &size, &count ) == 0 ) {
		status = (RASCONNSTATUSA *)GlobalAlloc( GPTR, sizeof( RASCONNSTATUSA ) );
		for ( DWORD i = 0; i < count; i++, connection++ ) {
			status->dwSize = sizeof( RASCONNSTATUSA );
			if ( getConnectStatus( connection->hrasconn, status ) == 0 && status->dwError == 0 && status->rasconnstate == RASCS_Connected ) {
				result = 0;
				break;
			}
		}
	} else {
		result = 3;
	}
	GlobalFree( status );
	// BUG: after the walk this frees an advanced pointer, not the block.
	GlobalFree( connection );
	return result;
}
