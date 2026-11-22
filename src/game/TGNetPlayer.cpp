/*
===============================================================================

	TGNetPlayer.cpp

===============================================================================
*/

#include <iostream>
#include "TGNetPlayer.h"
#include "TGAckMessage.h"
#include "TGMessage.h"
#include "TGSystemWrapperClass.h"
#include "NiMain.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x006c0f30 _$E410
// FUNCTION: STBC 0x006c0f50 _$E409

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x006c0d90 _$E32
// FUNCTION: STBC 0x006c0da0 _$E29
// FUNCTION: STBC 0x006c0db0 _$E31
// FUNCTION: STBC 0x006c0dc0 _$E30
// FUNCTION: STBC 0x006c0dd0 _$E38
// FUNCTION: STBC 0x006c0de0 _$E35
// FUNCTION: STBC 0x006c0df0 _$E37
// FUNCTION: STBC 0x006c0e00 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x006c0e10 _$E47
// FUNCTION: STBC 0x006c0e20 _$E46
// FUNCTION: STBC 0x006c0e30 _$E51
// FUNCTION: STBC 0x006c0e40 _$E50
// FUNCTION: STBC 0x006c0e50 _$E62
// FUNCTION: STBC 0x006c0e60 _$E61
// FUNCTION: STBC 0x006c0e70 _$E66
// FUNCTION: STBC 0x006c0e80 _$E65
// FUNCTION: STBC 0x006c0e90 _$E70
// FUNCTION: STBC 0x006c0ea0 _$E69
// FUNCTION: STBC 0x006c0eb0 _$E74
// FUNCTION: STBC 0x006c0ec0 _$E73
// FUNCTION: STBC 0x006c0ed0 _$E198
// FUNCTION: STBC 0x006c0ee0 _$E197
// FUNCTION: STBC 0x006c0ef0 _$E140
// FUNCTION: STBC 0x006c0f00 _$E139
// FUNCTION: STBC 0x006c0f10 _$E153
// FUNCTION: STBC 0x006c0f20 _$E152
// FUNCTION: STBC 0x006c0f60 _$E203
// FUNCTION: STBC 0x006c0f70 _$E202
// FUNCTION: STBC 0x006c0f80 _$E208
// FUNCTION: STBC 0x006c0f90 _$E207
// FUNCTION: STBC 0x006c0fa0 _$E212
// FUNCTION: STBC 0x006c0fb0 _$E211
// FUNCTION: STBC 0x006c0fc0 _$E216
// FUNCTION: STBC 0x006c0fd0 _$E215
// FUNCTION: STBC 0x006c0fe0 _$E223
// FUNCTION: STBC 0x006c0ff0 _$E222
// FUNCTION: STBC 0x006c1000 _$E248
// FUNCTION: STBC 0x006c1010 _$E247
// FUNCTION: STBC 0x006c1020 _$E252
// FUNCTION: STBC 0x006c1030 _$E251
// FUNCTION: STBC 0x006c1040 _$E98
// FUNCTION: STBC 0x006c1050 _$E97
// FUNCTION: STBC 0x006c1060 _$E234
// FUNCTION: STBC 0x006c1070 _$E233
// FUNCTION: STBC 0x006c1080 _$E241
// FUNCTION: STBC 0x006c1090 _$E240
// FUNCTION: STBC 0x006c10a0 _$E264
// FUNCTION: STBC 0x006c10b0 _$E263
// FUNCTION: STBC 0x006c10c0 _$E268
// FUNCTION: STBC 0x006c10d0 _$E267
// FUNCTION: STBC 0x006c10e0 _$E308
// FUNCTION: STBC 0x006c10f0 _$E307
// FUNCTION: STBC 0x006c1100 _$E314
// FUNCTION: STBC 0x006c1110 _$E313
// FUNCTION: STBC 0x006c1120 _$E256
// FUNCTION: STBC 0x006c1130 _$E255
// FUNCTION: STBC 0x006c1140 _$E260
// FUNCTION: STBC 0x006c1150 _$E259
// FUNCTION: STBC 0x006c1160 _$E272
// FUNCTION: STBC 0x006c1170 _$E271
// FUNCTION: STBC 0x006c1180 _$E276
// FUNCTION: STBC 0x006c1190 _$E275
// FUNCTION: STBC 0x006c11a0 _$E280
// FUNCTION: STBC 0x006c11b0 _$E279
// FUNCTION: STBC 0x006c11c0 _$E284
// FUNCTION: STBC 0x006c11d0 _$E283
// FUNCTION: STBC 0x006c11e0 _$E288
// FUNCTION: STBC 0x006c11f0 _$E287
// FUNCTION: STBC 0x006c1200 _$E292
// FUNCTION: STBC 0x006c1210 _$E291
// FUNCTION: STBC 0x006c1220 _$E296
// FUNCTION: STBC 0x006c1230 _$E295
// FUNCTION: STBC 0x006c1240 _$E300
// FUNCTION: STBC 0x006c1250 _$E299
// FUNCTION: STBC 0x006c1260 _$E103
// FUNCTION: STBC 0x006c1270 _$E102
// FUNCTION: STBC 0x006c1280 _$E304
// FUNCTION: STBC 0x006c1290 _$E303
// FUNCTION: STBC 0x006c12a0 _$E108
// FUNCTION: STBC 0x006c12b0 _$E107
// FUNCTION: STBC 0x006c12c0 _$E318
// FUNCTION: STBC 0x006c12d0 _$E317
// FUNCTION: STBC 0x006c12e0 _$E322
// FUNCTION: STBC 0x006c12f0 _$E321
// FUNCTION: STBC 0x006c1300 _$E326
// FUNCTION: STBC 0x006c1310 _$E325
// FUNCTION: STBC 0x006c1320 _$E330
// FUNCTION: STBC 0x006c1330 _$E329
// FUNCTION: STBC 0x006c1340 _$E117
// FUNCTION: STBC 0x006c1350 _$E116
// FUNCTION: STBC 0x006c1360 _$E334
// FUNCTION: STBC 0x006c1370 _$E333
// FUNCTION: STBC 0x006c1380 _$E338
// FUNCTION: STBC 0x006c1390 _$E337
// FUNCTION: STBC 0x006c13a0 _$E342
// FUNCTION: STBC 0x006c13b0 _$E341
// FUNCTION: STBC 0x006c13c0 _$E123
// FUNCTION: STBC 0x006c13d0 _$E122
// FUNCTION: STBC 0x006c13e0 _$E128
// FUNCTION: STBC 0x006c13f0 _$E127
// FUNCTION: STBC 0x006c1400 _$E158
// FUNCTION: STBC 0x006c1410 _$E157
// FUNCTION: STBC 0x006c1420 _$E346
// FUNCTION: STBC 0x006c1430 _$E345
// FUNCTION: STBC 0x006c1440 _$E163
// FUNCTION: STBC 0x006c1450 _$E162
// FUNCTION: STBC 0x006c1460 _$E350
// FUNCTION: STBC 0x006c1470 _$E349
// FUNCTION: STBC 0x006c1480 _$E354
// FUNCTION: STBC 0x006c1490 _$E353
// FUNCTION: STBC 0x006c14a0 _$E168
// FUNCTION: STBC 0x006c14b0 _$E167
// FUNCTION: STBC 0x006c14c0 _$E358
// FUNCTION: STBC 0x006c14d0 _$E357
// FUNCTION: STBC 0x006c14e0 _$E133
// FUNCTION: STBC 0x006c14f0 _$E132
// FUNCTION: STBC 0x006c1500 _$E362
// FUNCTION: STBC 0x006c1510 _$E361
// FUNCTION: STBC 0x006c1520 _$E366
// FUNCTION: STBC 0x006c1530 _$E365
// FUNCTION: STBC 0x006c1540 _$E173
// FUNCTION: STBC 0x006c1550 _$E172
// FUNCTION: STBC 0x006c1560 _$E178
// FUNCTION: STBC 0x006c1570 _$E177
// FUNCTION: STBC 0x006c1580 _$E370
// FUNCTION: STBC 0x006c1590 _$E369
// FUNCTION: STBC 0x006c15a0 _$E374
// FUNCTION: STBC 0x006c15b0 _$E373
// FUNCTION: STBC 0x006c15c0 _$E378
// FUNCTION: STBC 0x006c15d0 _$E377
// FUNCTION: STBC 0x006c15e0 _$E382
// FUNCTION: STBC 0x006c15f0 _$E381
// FUNCTION: STBC 0x006c1600 _$E386
// FUNCTION: STBC 0x006c1610 _$E385
// FUNCTION: STBC 0x006c1620 _$E390
// FUNCTION: STBC 0x006c1630 _$E389
// FUNCTION: STBC 0x006c1640 _$E394
// FUNCTION: STBC 0x006c1650 _$E393
// FUNCTION: STBC 0x006c1660 _$E398
// FUNCTION: STBC 0x006c1670 _$E397
// FUNCTION: STBC 0x006c1680 _$E183
// FUNCTION: STBC 0x006c1690 _$E182
// FUNCTION: STBC 0x006c16a0 _$E402
// FUNCTION: STBC 0x006c16b0 _$E401
// FUNCTION: STBC 0x006c16c0 _$E188
// FUNCTION: STBC 0x006c16d0 _$E187
// FUNCTION: STBC 0x006c16e0 _$E193
// FUNCTION: STBC 0x006c16f0 _$E192

/*
================
TGNetPlayer::TGNetPlayer
================
*/
// FUNCTION: STBC 0x006c08d0 ??0TGNetPlayer@@QAE@XZ
TGNetPlayer::TGNetPlayer( void ) {
	name.AssignC( "Unknown" );
	netID = -1;
	address = 0;
	connectUID = 0;
	connectAddress = 0;
	unknown28 = 0;
	unknown2A = 0;
	unknown24 = 0;
	unknown26 = 0;
	unknown2C = g_kSystemWrapper.GetTimeInSeconds();
	unknown30 = g_kSystemWrapper.GetTimeInSeconds();
	bytesReceived = 0;
	sampleBytesReceived = 0;
	bytesReceivedWithHeaders = 0;
	sampleBytesReceivedWithHeaders = 0;
	bytesSent = 0;
	sampleBytesSent = 0;
	bytesSentWithHeaders = 0;
	numSamples = 0;
	sampleBytesSentWithHeaders = 0;
	bytesPerSecondTo = 0;
	bytesPerSecondFrom = 0;
	lastSampleTime = 0;
	disconnected = false;
	disconnectTime = 0.0f;
}

/*
================
TGNetPlayer::~TGNetPlayer
================
*/
// FUNCTION: STBC 0x006c0a10
// FUNCTION: STBC 0x006c09f0 ??_GTGNetPlayer@@UAEPAXI@Z
TGNetPlayer::~TGNetPlayer( void ) {
}

/*
================
TGNetPlayer::SetName
================
*/
// FUNCTION: STBC 0x006c0b50
void TGNetPlayer::SetName( TGString *newName ) {
	name = *newName;
}

/*
================
TGNetPlayer::TGNetPlayer

The copy: the members' own constructors run, and nothing of the player is
taken over.
================
*/
// FUNCTION: STBC 0x006c0b60 ??0TGNetPlayer@@QAE@ABV0@@Z
// BUG: copies none of the player's state: the copy has an empty name and none of its counts
TGNetPlayer::TGNetPlayer( const TGNetPlayer &player ) {
}

/*
================
TGNetPlayer::UpdateProfile
================
*/
// FUNCTION: STBC 0x006c0be0
// PARKED: x87 schedule; retail computes sampled after loading the first count and keeps it in memory, this build keeps it on the FPU stack
void TGNetPlayer::UpdateProfile( float time ) {
	while ( true ) {
		float elapsed = time - lastSampleTime;
		if ( elapsed < 1.0f ) {
			break;
		}
		float later = ( elapsed - 1.0f ) / elapsed;
		float sampled = 1.0f - later;

		bytesReceived += (int)( sampleBytesReceived * sampled );
		sampleBytesReceived = (int)( sampleBytesReceived * later );

		bytesPerSecondFrom = (int)( sampleBytesReceivedWithHeaders * sampled );
		bytesReceivedWithHeaders += bytesPerSecondFrom;
		sampleBytesReceivedWithHeaders = (int)( sampleBytesReceivedWithHeaders * later );

		bytesSent += (int)( sampleBytesSent * sampled );
		sampleBytesSent = (int)( sampleBytesSent * later );

		bytesPerSecondTo = (int)( sampleBytesSentWithHeaders * sampled );
		bytesSentWithHeaders += bytesPerSecondTo;
		sampleBytesSentWithHeaders = (int)( sampleBytesSentWithHeaders * later );

		lastSampleTime += 1.0f;
		numSamples++;
	}
}

/*
================
TGNetPlayer::WriteProfile
================
*/
// FUNCTION: STBC 0x006c0cc0
void TGNetPlayer::WriteProfile( FILE *file ) {
	if ( numSamples != 0 ) {
		char *playerName = name.Unknown6F56F0();
		fprintf( file, "--------------------------------------------\n" );
		fprintf( file, "Network info for player: %s\n", playerName );
		fprintf( file, "--------------------------------------------\n" );
		fprintf( file, "Sent BPS: %d\t\tSent Plus Header BPS: %d\n", bytesSent / numSamples, bytesSentWithHeaders / numSamples );
		fprintf( file, "Received BPS: %d\t\tReceived Plus Header BPS: %d\n", bytesReceived / numSamples, bytesReceivedWithHeaders / numSamples );
		fprintf( file, "Num samples %d\n", numSamples );
		fprintf( file, "--------------------------------------------\n" );
	}
}

/*
================
TGNetPlayer::GetBytesPerSecondTo
================
*/
// FUNCTION: STBC 0x006c0d70
int TGNetPlayer::GetBytesPerSecondTo( void ) {
	return bytesPerSecondTo;
}

/*
================
TGNetPlayer::GetBytesPerSecondFrom
================
*/
// FUNCTION: STBC 0x006c0d80
int TGNetPlayer::GetBytesPerSecondFrom( void ) {
	return bytesPerSecondFrom;
}

/*
================
TGNetPlayer::Copy
================
*/
// FUNCTION: STBC 0x006c1700
TGNetPlayer *TGNetPlayer::Copy( void ) {
	return new TGNetPlayer( *this );
}
