/*
===============================================================================

	TGProfilingInfo.cpp

===============================================================================
*/

#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <string.h>

#include "TGProfilingInfo.h"
#include "TGSystemWrapperClass.h"
#include "AppConstants.h"
#include "NiMain.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00719c80 _$E51
// FUNCTION: STBC 0x00719c90 _$E50
// FUNCTION: STBC 0x00719ca0 _$E55
// FUNCTION: STBC 0x00719cb0 _$E54
// FUNCTION: STBC 0x00719cc0 _$E66
// FUNCTION: STBC 0x00719cd0 _$E65
// FUNCTION: STBC 0x00719ce0 _$E70
// FUNCTION: STBC 0x00719cf0 _$E69
// FUNCTION: STBC 0x00719e20 _$E74
// FUNCTION: STBC 0x00719e30 _$E73
// FUNCTION: STBC 0x00719e40 _$E78
// FUNCTION: STBC 0x00719e50 _$E77
// FUNCTION: STBC 0x00719e60 _$E202
// FUNCTION: STBC 0x00719e70 _$E201
// FUNCTION: STBC 0x0071a0d0 _$E144
// FUNCTION: STBC 0x0071a0e0 _$E143
// FUNCTION: STBC 0x0071a0f0 _$E157
// FUNCTION: STBC 0x0071a100 _$E156
// FUNCTION: STBC 0x0071a110 _$E207
// FUNCTION: STBC 0x0071a120 _$E206
// FUNCTION: STBC 0x0071a130 _$E212
// FUNCTION: STBC 0x0071a140 _$E211
// FUNCTION: STBC 0x0071a150 _$E216
// FUNCTION: STBC 0x0071a160 _$E215
// FUNCTION: STBC 0x0071a170 _$E220
// FUNCTION: STBC 0x0071a180 _$E219
// FUNCTION: STBC 0x0071a190 _$E227
// FUNCTION: STBC 0x0071a1a0 _$E226
// FUNCTION: STBC 0x0071a1b0 _$E252
// FUNCTION: STBC 0x0071a1c0 _$E251
// FUNCTION: STBC 0x0071a1d0 _$E256
// FUNCTION: STBC 0x0071a1e0 _$E255
// FUNCTION: STBC 0x0071a1f0 _$E102
// FUNCTION: STBC 0x0071a200 _$E101
// FUNCTION: STBC 0x0071a210 _$E238
// FUNCTION: STBC 0x0071a220 _$E237
// FUNCTION: STBC 0x0071a230 _$E245
// FUNCTION: STBC 0x0071a240 _$E244
// FUNCTION: STBC 0x0071a250 _$E268
// FUNCTION: STBC 0x0071a260 _$E267
// FUNCTION: STBC 0x0071a270 _$E272
// FUNCTION: STBC 0x0071a280 _$E271
// FUNCTION: STBC 0x0071a290 _$E312
// FUNCTION: STBC 0x0071a2a0 _$E311
// FUNCTION: STBC 0x0071a2b0 _$E318
// FUNCTION: STBC 0x0071a2c0 _$E317
// FUNCTION: STBC 0x0071a2d0 _$E260
// FUNCTION: STBC 0x0071a2e0 _$E259
// FUNCTION: STBC 0x0071a2f0 _$E264
// FUNCTION: STBC 0x0071a300 _$E263
// FUNCTION: STBC 0x0071a310 _$E276
// FUNCTION: STBC 0x0071a320 _$E275
// FUNCTION: STBC 0x0071a330 _$E280
// FUNCTION: STBC 0x0071a340 _$E279
// FUNCTION: STBC 0x0071a350 _$E284
// FUNCTION: STBC 0x0071a360 _$E283
// FUNCTION: STBC 0x0071a370 _$E288
// FUNCTION: STBC 0x0071a380 _$E287
// FUNCTION: STBC 0x0071a390 _$E292
// FUNCTION: STBC 0x0071a3a0 _$E291
// FUNCTION: STBC 0x0071a3b0 _$E296
// FUNCTION: STBC 0x0071a3c0 _$E295
// FUNCTION: STBC 0x0071a3d0 _$E300
// FUNCTION: STBC 0x0071a3e0 _$E299
// FUNCTION: STBC 0x0071a3f0 _$E304
// FUNCTION: STBC 0x0071a400 _$E303
// FUNCTION: STBC 0x0071a410 _$E107
// FUNCTION: STBC 0x0071a420 _$E106
// FUNCTION: STBC 0x0071a430 _$E308
// FUNCTION: STBC 0x0071a440 _$E307
// FUNCTION: STBC 0x0071a450 _$E112
// FUNCTION: STBC 0x0071a460 _$E111
// FUNCTION: STBC 0x0071a470 _$E322
// FUNCTION: STBC 0x0071a480 _$E321
// FUNCTION: STBC 0x0071a490 _$E326
// FUNCTION: STBC 0x0071a4a0 _$E325
// FUNCTION: STBC 0x0071a4b0 _$E330
// FUNCTION: STBC 0x0071a4c0 _$E329
// FUNCTION: STBC 0x0071a4d0 _$E334
// FUNCTION: STBC 0x0071a4e0 _$E333
// FUNCTION: STBC 0x0071a4f0 _$E121
// FUNCTION: STBC 0x0071a500 _$E120
// FUNCTION: STBC 0x0071a510 _$E338
// FUNCTION: STBC 0x0071a520 _$E337
// FUNCTION: STBC 0x0071a530 _$E342
// FUNCTION: STBC 0x0071a540 _$E341
// FUNCTION: STBC 0x0071a550 _$E346
// FUNCTION: STBC 0x0071a560 _$E345
// FUNCTION: STBC 0x0071a570 _$E127
// FUNCTION: STBC 0x0071a580 _$E126
// FUNCTION: STBC 0x0071a590 _$E132
// FUNCTION: STBC 0x0071a5a0 _$E131
// FUNCTION: STBC 0x0071a5b0 _$E162
// FUNCTION: STBC 0x0071a5c0 _$E161
// FUNCTION: STBC 0x0071a5d0 _$E350
// FUNCTION: STBC 0x0071a5e0 _$E349
// FUNCTION: STBC 0x0071a5f0 _$E167
// FUNCTION: STBC 0x0071a600 _$E166
// FUNCTION: STBC 0x0071a610 _$E354
// FUNCTION: STBC 0x0071a620 _$E353
// FUNCTION: STBC 0x0071a630 _$E358
// FUNCTION: STBC 0x0071a640 _$E357
// FUNCTION: STBC 0x0071a650 _$E172
// FUNCTION: STBC 0x0071a660 _$E171
// FUNCTION: STBC 0x0071a670 _$E362
// FUNCTION: STBC 0x0071a680 _$E361
// FUNCTION: STBC 0x0071a690 _$E137
// FUNCTION: STBC 0x0071a6a0 _$E136
// FUNCTION: STBC 0x0071a6b0 _$E366
// FUNCTION: STBC 0x0071a6c0 _$E365
// FUNCTION: STBC 0x0071a6d0 _$E370
// FUNCTION: STBC 0x0071a6e0 _$E369
// FUNCTION: STBC 0x0071a6f0 _$E177
// FUNCTION: STBC 0x0071a700 _$E176
// FUNCTION: STBC 0x0071a710 _$E182
// FUNCTION: STBC 0x0071a720 _$E181
// FUNCTION: STBC 0x0071a730 _$E374
// FUNCTION: STBC 0x0071a740 _$E373
// FUNCTION: STBC 0x0071a750 _$E378
// FUNCTION: STBC 0x0071a760 _$E377
// FUNCTION: STBC 0x0071a770 _$E382
// FUNCTION: STBC 0x0071a780 _$E381
// FUNCTION: STBC 0x0071a790 _$E386
// FUNCTION: STBC 0x0071a7a0 _$E385
// FUNCTION: STBC 0x0071a7b0 _$E390
// FUNCTION: STBC 0x0071a7c0 _$E389
// FUNCTION: STBC 0x0071a7d0 _$E394
// FUNCTION: STBC 0x0071a7e0 _$E393
// FUNCTION: STBC 0x0071a7f0 _$E398
// FUNCTION: STBC 0x0071a800 _$E397
// FUNCTION: STBC 0x0071a810 _$E402
// FUNCTION: STBC 0x0071a820 _$E401
// FUNCTION: STBC 0x0071a830 _$E187
// FUNCTION: STBC 0x0071a840 _$E186
// FUNCTION: STBC 0x0071a850 _$E406
// FUNCTION: STBC 0x0071a860 _$E405
// FUNCTION: STBC 0x0071a870 _$E192
// FUNCTION: STBC 0x0071a880 _$E191
// FUNCTION: STBC 0x0071a890 _$E197
// FUNCTION: STBC 0x0071a8a0 _$E196

// The samples are block lists of this many items.
#define SAMPLE_BLOCK_SIZE		512

// The standard streams' initialisers and exit-time destructors (<iostream>):
// retail lays them out after the unit's code.
// FUNCTION: STBC 0x00719c00 _$E32
// FUNCTION: STBC 0x00719c10 _$E29
// FUNCTION: STBC 0x00719c20 _$E31
// FUNCTION: STBC 0x00719c30 _$E30
// FUNCTION: STBC 0x00719c40 _$E38
// FUNCTION: STBC 0x00719c50 _$E35
// FUNCTION: STBC 0x00719c60 _$E37
// FUNCTION: STBC 0x00719c70 _$E36

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x0071a000 _$E425
// FUNCTION: STBC 0x0071a020 _$E424

// The object's initialiser and exit-time destructor.
// FUNCTION: STBC 0x00718dd0 _$E412
// FUNCTION: STBC 0x00718de0 _$E408
// FUNCTION: STBC 0x00718df0 _$E411
// FUNCTION: STBC 0x00718e00 _$E409
// DATA: STBC 0x0099c4e8
NiTStringMap<TGTimingSamples *> TGProfilingInfo::timingsByName( 307 );

// FUNCTION: STBC 0x00718e20 _$E417
// FUNCTION: STBC 0x00718e30 _$E414
// FUNCTION: STBC 0x00718e70 _$E416
// FUNCTION: STBC 0x00718e80 _$E415
// DATA: STBC 0x0099c530
NiTMap<long, TGTiming *> TGProfilingInfo::timingsByID( 47 );

// DATA: STBC 0x0095c610 ?nextTimingID@TGProfilingInfo@@1JA
long TGProfilingInfo::nextTimingID = 1;

// DATA: STBC 0x0099c5f4 ?isEnabled@TGProfilingInfo@@1_NA
bool TGProfilingInfo::isEnabled;

// DATA: STBC 0x0095c608 ?ticksPerSecond@TGProfilingInfo@@1NA
double TGProfilingInfo::ticksPerSecond = 0.0;

// DATA: STBC 0x0099c5f8 ?totalTiming@TGProfilingInfo@@1JA
long TGProfilingInfo::totalTiming;

/*
================
TGProfilingInfo::EnableProfiling

Starts profiling: reads the counter's frequency and times the whole run.
================
*/
// FUNCTION: STBC 0x00718f10 ?EnableProfiling@TGProfilingInfo@@SAXXZ
void TGProfilingInfo::EnableProfiling( void ) {
	if ( !isEnabled ) {
		LARGE_INTEGER frequency;
		if ( QueryPerformanceFrequency( &frequency ) ) {
			isEnabled = true;
			ticksPerSecond = (double)frequency.QuadPart;
		}
		totalTiming = StartTiming( "Total profiling time" );
	}
}

/*
================
TGProfilingInfo::DisableProfiling

Ends the timing of the whole run and stops profiling.
================
*/
// FUNCTION: STBC 0x00718f60 ?DisableProfiling@TGProfilingInfo@@SAXXZ
void TGProfilingInfo::DisableProfiling( void ) {
	StopTiming( totalTiming );
	totalTiming = 0;
	isEnabled = false;
}

/*
================
TGProfilingInfo::StartTiming

Starts a timing under the name and returns its ID for StopTiming; 0 while
profiling is off.
================
*/
// FUNCTION: STBC 0x00718f80
long TGProfilingInfo::StartTiming( char *name ) {
	if ( !isEnabled ) {
		return 0;
	}
	TGTimingSamples *samples;
	if ( !timingsByName.GetAt( name, samples ) ) {
		samples = new TGTimingSamples;
		samples->times = new NiTList<float>( SAMPLE_BLOCK_SIZE );
		samples->frames = new NiTList<unsigned long>( SAMPLE_BLOCK_SIZE );
		timingsByName.SetAt( name, samples );
	}
	TGTiming *timing = new TGTiming;
	timing->frame = g_kSystemWrapper.GetUpdateNumber();
	timing->samples = samples;
	timingsByID.SetAt( nextTimingID, timing );
	QueryPerformanceCounter( (LARGE_INTEGER *)&timing->start );
	return nextTimingID++;
}

/*
================
TGProfilingInfo::SetTimingData

Adds a sample of the given milliseconds under the name, for this frame.
================
*/
// FUNCTION: STBC 0x007191a0
void TGProfilingInfo::SetTimingData( char *name, float milliseconds ) {
	if ( isEnabled ) {
		TGTimingSamples *samples;
		if ( !timingsByName.GetAt( name, samples ) ) {
			samples = new TGTimingSamples;
			samples->times = new NiTList<float>( SAMPLE_BLOCK_SIZE );
			samples->frames = new NiTList<unsigned long>( SAMPLE_BLOCK_SIZE );
			timingsByName.SetAt( name, samples );
		}
		samples->times->AddTail( milliseconds );
		samples->frames->AddTail( g_kSystemWrapper.GetUpdateNumber() );
	}
}

/*
================
TGProfilingInfo::StopTiming

Ends the timing: its milliseconds and starting frame become a sample of its
name.
================
*/
// FUNCTION: STBC 0x007193f0
void TGProfilingInfo::StopTiming( long id ) {
	if ( isEnabled ) {
		LARGE_INTEGER now;
		QueryPerformanceCounter( &now );
		TGTiming *timing;
		if ( !timingsByID.GetAt( id, timing ) ) {
			return;
		}
		float milliseconds = (float)( ( now.QuadPart - timing->start ) / ticksPerSecond * 1000.0 );
		timing->samples->times->AddTail( milliseconds );
		timing->samples->frames->AddTail( timing->frame );
		timingsByID.RemoveAt( id );
		delete timing;
	}
}

/*
================
TGProfilingInfo::SaveRawData

Writes every name's samples to the file, a line per name: the name, then
frame:milliseconds pairs. An existing file is kept, unless its name starts
with "RawTiming" and ends in ".txt".
================
*/
// FUNCTION: STBC 0x007195f0
bool TGProfilingInfo::SaveRawData( char *fileName ) {
	int length = strlen( fileName );
	if ( strncmp( fileName, "RawTiming", strlen( "RawTiming" ) ) != 0 || length < 4 || strcmp( &fileName[length - 4], ".txt" ) != 0 ) {
		if ( g_kSystemWrapper.FileExists( fileName ) ) {
			return false;
		}
	}
	FILE *file = g_kSystemWrapper.FileOpen( fileName, "w" );
	if ( file == NULL ) {
		return false;
	}

	NiTMapIterator pos = timingsByName.GetFirstPos();
	while ( pos != NULL ) {
		const char *name;
		TGTimingSamples *samples;
		timingsByName.GetNext( pos, name, samples );
		fprintf( file, "%s - ", name );
		NiTListIterator timePos = samples->times->GetHeadPos();
		NiTListIterator framePos = samples->frames->GetHeadPos();
		while ( timePos != NULL && framePos != NULL ) {
			fprintf( file, "%ld:%f ", samples->frames->GetNext( framePos ), samples->times->GetNext( timePos ) );
		}
		fprintf( file, "\n" );
	}
	g_kSystemWrapper.FileClose( file );
	return true;
}

/*
================
TGProfilingInfo::Terminate

Stops profiling and frees every sample.
================
*/
// FUNCTION: STBC 0x007197c0
void TGProfilingInfo::Terminate( void ) {
	isEnabled = false;
	NiTMapIterator pos = timingsByName.GetFirstPos();
	while ( pos != NULL ) {
		const char *name;
		TGTimingSamples *samples;
		timingsByName.GetNext( pos, name, samples );
		delete samples->frames;
		delete samples->times;
		delete samples;
	}
	timingsByName.RemoveAll();
}

// The profile ID of no profile; TGSystemWrapperClass's constructor stores it.
// DATA: STBC 0x00896adc
extern const int PFID_INVALID = 0;
