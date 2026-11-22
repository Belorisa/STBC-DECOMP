/*
===============================================================================

	TimeSliceProcess.cpp

===============================================================================
*/

// <iostream>'s statics: the ios_base::Init and _Winit quads, then the locale
// id pair at the unit's end.
// FUNCTION: STBC 0x0046f9f0 _$E32
// FUNCTION: STBC 0x0046fa00 _$E29
// FUNCTION: STBC 0x0046fa10 _$E31
// FUNCTION: STBC 0x0046fa20 _$E30
// FUNCTION: STBC 0x0046fa30 _$E38
// FUNCTION: STBC 0x0046fa40 _$E35
// FUNCTION: STBC 0x0046fa50 _$E37
// FUNCTION: STBC 0x0046fa60 _$E36
// FUNCTION: STBC 0x0046fec0 _$E462
// FUNCTION: STBC 0x0046fee0 _$E461
#include <iostream>

#include "TimeSliceProcess.h"
#include "PythonMethodProcess.h"
#include "NiMain.h"
#include "PyEmbed.h"
#include "TGSystemWrapperClass.h"
#include "UtopiaModule.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0046fa70 _$E49
// FUNCTION: STBC 0x0046fa80 _$E48
// FUNCTION: STBC 0x0046fa90 _$E53
// FUNCTION: STBC 0x0046faa0 _$E52
// FUNCTION: STBC 0x0046fab0 _$E64
// FUNCTION: STBC 0x0046fac0 _$E63
// FUNCTION: STBC 0x0046fad0 _$E68
// FUNCTION: STBC 0x0046fae0 _$E67
// FUNCTION: STBC 0x0046faf0 _$E72
// FUNCTION: STBC 0x0046fb00 _$E71
// FUNCTION: STBC 0x0046fb10 _$E76
// FUNCTION: STBC 0x0046fb20 _$E75
// FUNCTION: STBC 0x0046fb30 _$E200
// FUNCTION: STBC 0x0046fb40 _$E199
// FUNCTION: STBC 0x0046fb50 _$E142
// FUNCTION: STBC 0x0046fb60 _$E141
// FUNCTION: STBC 0x0046fb70 _$E155
// FUNCTION: STBC 0x0046fb80 _$E154
// FUNCTION: STBC 0x0046fb90 _$E205
// FUNCTION: STBC 0x0046fba0 _$E204
// FUNCTION: STBC 0x0046fbb0 _$E210
// FUNCTION: STBC 0x0046fbc0 _$E209
// FUNCTION: STBC 0x0046fbd0 _$E214
// FUNCTION: STBC 0x0046fbe0 _$E213
// FUNCTION: STBC 0x0046fc10 _$E218
// FUNCTION: STBC 0x0046fc20 _$E217
// FUNCTION: STBC 0x0046fc30 _$E225
// FUNCTION: STBC 0x0046fc40 _$E224
// FUNCTION: STBC 0x0046fc50 _$E250
// FUNCTION: STBC 0x0046fc60 _$E249
// FUNCTION: STBC 0x0046fc70 _$E254
// FUNCTION: STBC 0x0046fc80 _$E253
// FUNCTION: STBC 0x0046fc90 _$E100
// FUNCTION: STBC 0x0046fca0 _$E99
// FUNCTION: STBC 0x0046fce0 _$E236
// FUNCTION: STBC 0x0046fcf0 _$E235
// FUNCTION: STBC 0x0046fd00 _$E243
// FUNCTION: STBC 0x0046fd10 _$E242
// FUNCTION: STBC 0x0046fd20 _$E266
// FUNCTION: STBC 0x0046fd30 _$E265
// FUNCTION: STBC 0x0046fd40 _$E270
// FUNCTION: STBC 0x0046fd50 _$E269
// FUNCTION: STBC 0x0046fd60 _$E310
// FUNCTION: STBC 0x0046fd70 _$E309
// FUNCTION: STBC 0x0046fd80 _$E316
// FUNCTION: STBC 0x0046fd90 _$E315
// FUNCTION: STBC 0x0046fda0 _$E258
// FUNCTION: STBC 0x0046fdb0 _$E257
// FUNCTION: STBC 0x0046fdc0 _$E262
// FUNCTION: STBC 0x0046fdd0 _$E261
// FUNCTION: STBC 0x0046fde0 _$E274
// FUNCTION: STBC 0x0046fdf0 _$E273
// FUNCTION: STBC 0x0046fe00 _$E278
// FUNCTION: STBC 0x0046fe10 _$E277
// FUNCTION: STBC 0x0046fe20 _$E282
// FUNCTION: STBC 0x0046fe30 _$E281
// FUNCTION: STBC 0x0046fe40 _$E286
// FUNCTION: STBC 0x0046fe50 _$E285
// FUNCTION: STBC 0x0046fe60 _$E290
// FUNCTION: STBC 0x0046fe70 _$E289
// FUNCTION: STBC 0x0046fe80 _$E294
// FUNCTION: STBC 0x0046fe90 _$E293
// FUNCTION: STBC 0x0046fea0 _$E298
// FUNCTION: STBC 0x0046feb0 _$E297
// FUNCTION: STBC 0x0046fef0 _$E302
// FUNCTION: STBC 0x0046ff00 _$E301
// FUNCTION: STBC 0x0046ff10 _$E105
// FUNCTION: STBC 0x0046ff20 _$E104
// FUNCTION: STBC 0x0046ff30 _$E306
// FUNCTION: STBC 0x0046ff40 _$E305
// FUNCTION: STBC 0x0046ff50 _$E110
// FUNCTION: STBC 0x0046ff60 _$E109
// FUNCTION: STBC 0x0046ff70 _$E320
// FUNCTION: STBC 0x0046ff80 _$E319
// FUNCTION: STBC 0x0046ff90 _$E324
// FUNCTION: STBC 0x0046ffa0 _$E323
// FUNCTION: STBC 0x0046ffb0 _$E328
// FUNCTION: STBC 0x0046ffc0 _$E327
// FUNCTION: STBC 0x0046ffd0 _$E332
// FUNCTION: STBC 0x0046ffe0 _$E331
// FUNCTION: STBC 0x0046fff0 _$E119
// FUNCTION: STBC 0x00470000 _$E118
// FUNCTION: STBC 0x00470010 _$E336
// FUNCTION: STBC 0x00470020 _$E335
// FUNCTION: STBC 0x00470030 _$E340
// FUNCTION: STBC 0x00470040 _$E339
// FUNCTION: STBC 0x00470050 _$E344
// FUNCTION: STBC 0x00470060 _$E343
// FUNCTION: STBC 0x00470070 _$E125
// FUNCTION: STBC 0x00470080 _$E124
// FUNCTION: STBC 0x00470090 _$E130
// FUNCTION: STBC 0x004700a0 _$E129
// FUNCTION: STBC 0x004700b0 _$E160
// FUNCTION: STBC 0x004700c0 _$E159
// FUNCTION: STBC 0x004700d0 _$E348
// FUNCTION: STBC 0x004700e0 _$E347
// FUNCTION: STBC 0x004700f0 _$E165
// FUNCTION: STBC 0x00470100 _$E164
// FUNCTION: STBC 0x00470110 _$E352
// FUNCTION: STBC 0x00470120 _$E351
// FUNCTION: STBC 0x00470130 _$E356
// FUNCTION: STBC 0x00470140 _$E355
// FUNCTION: STBC 0x00470150 _$E170
// FUNCTION: STBC 0x00470160 _$E169
// FUNCTION: STBC 0x00470170 _$E360
// FUNCTION: STBC 0x00470180 _$E359
// FUNCTION: STBC 0x00470190 _$E135
// FUNCTION: STBC 0x004701a0 _$E134
// FUNCTION: STBC 0x004701b0 _$E364
// FUNCTION: STBC 0x004701c0 _$E363
// FUNCTION: STBC 0x004701d0 _$E368
// FUNCTION: STBC 0x004701e0 _$E367
// FUNCTION: STBC 0x004701f0 _$E175
// FUNCTION: STBC 0x00470200 _$E174
// FUNCTION: STBC 0x00470210 _$E180
// FUNCTION: STBC 0x00470220 _$E179
// FUNCTION: STBC 0x00470230 _$E372
// FUNCTION: STBC 0x00470240 _$E371
// FUNCTION: STBC 0x00470250 _$E376
// FUNCTION: STBC 0x00470260 _$E375
// FUNCTION: STBC 0x00470270 _$E380
// FUNCTION: STBC 0x00470280 _$E379
// FUNCTION: STBC 0x00470290 _$E384
// FUNCTION: STBC 0x004702a0 _$E383
// FUNCTION: STBC 0x004702b0 _$E388
// FUNCTION: STBC 0x004702c0 _$E387
// FUNCTION: STBC 0x004702d0 _$E392
// FUNCTION: STBC 0x004702e0 _$E391
// FUNCTION: STBC 0x004702f0 _$E396
// FUNCTION: STBC 0x00470300 _$E395
// FUNCTION: STBC 0x00470310 _$E400
// FUNCTION: STBC 0x00470320 _$E399
// FUNCTION: STBC 0x00470330 _$E185
// FUNCTION: STBC 0x00470340 _$E184
// FUNCTION: STBC 0x00470350 _$E404
// FUNCTION: STBC 0x00470360 _$E403
// FUNCTION: STBC 0x00470370 _$E190
// FUNCTION: STBC 0x00470380 _$E189
// FUNCTION: STBC 0x00470390 _$E195
// FUNCTION: STBC 0x004703a0 _$E194
// FUNCTION: STBC 0x004703b0 _$E439
// FUNCTION: STBC 0x004703c0 _$E438
// FUNCTION: STBC 0x004703d0 _$E444
// FUNCTION: STBC 0x004703e0 _$E443
// FUNCTION: STBC 0x004703f0 _$E448
// FUNCTION: STBC 0x00470400 _$E447

// The least time a frame leaves the processes.
#define MIN_FRAME_TIME			0.01f

// The arrays' dynamic initialiser and exit-time destructor.
// FUNCTION: STBC 0x0046f2e0 _$E454
// FUNCTION: STBC 0x0046f2f0 _$E450
// FUNCTION: STBC 0x0046f310 _$E453
// FUNCTION: STBC 0x0046f320 _$E451
// DATA: STBC 0x00981490
NiTArray<TimeSliceProcess *>	TimeSliceProcess::processes[NUM_PRIORITIES];
// DATA: STBC 0x00981560
double	TimeSliceProcess::frameTimes[TSP_NUM_FRAME_TIMES];
// DATA: STBC 0x009815e0
int		TimeSliceProcess::frameTimeIndex;
// DATA: STBC 0x009815e4
int		TimeSliceProcess::frameCount;

/*
================
TimeSliceProcess::TimeSliceProcess
================
*/
// FUNCTION: STBC 0x0046f380
TimeSliceProcess::TimeSliceProcess( void ) {
	priority = NORMAL;
	delay = 0.0;
	updated = false;
	delayUsesGameTime = true;
	lastUpdateTime = -1.0e20;
	createdUpdate = g_kSystemWrapper.GetUpdateNumber();
	AddToArray( this );
}

/*
================
TimeSliceProcess::~TimeSliceProcess
================
*/
// FUNCTION: STBC 0x0046f3e0
TimeSliceProcess::~TimeSliceProcess() {
	RemoveFromArray( this );
}

/*
================
TimeSliceProcess::Update

The process has had its turn: notes when, on the clock its delay uses.
================
*/
// FUNCTION: STBC 0x0046f3f0
void TimeSliceProcess::Update( double time ) {
	updated = true;
	if ( delayUsesGameTime ) {
		lastUpdateTime = g_kUtopiaModule.GetGameTime();
	} else {
		lastUpdateTime = g_kSystemWrapper.GetTimeInSeconds();
	}
}

/*
================
TimeSliceProcess::UpdateProcesses

The time the processes get is the average of the last frames' update
starts (the shortest and the longest left out) less this frame's, but
never less than MIN_FRAME_TIME. The unstoppable processes that are due all
run; of the other priorities the frame count picks the one that goes
first, and the rest follow while time is left.
================
*/
// FUNCTION: STBC 0x0046f420
void TimeSliceProcess::UpdateProcesses( void ) {
	double now = g_kSystemWrapper.GetTimeSinceFrameStart();
	frameTimes[frameTimeIndex] = now;
	frameTimeIndex = ( frameTimeIndex + 1 ) % TSP_NUM_FRAME_TIMES;

	double total = 0.0;
	double shortest = frameTimes[0];
	double longest = frameTimes[0];
	for ( int i = 0; i < TSP_NUM_FRAME_TIMES; i++ ) {
		total += frameTimes[i];
		if ( frameTimes[i] < shortest ) {
			shortest = frameTimes[i];
		}
		if ( longest < frameTimes[i] ) {
			longest = frameTimes[i];
		}
	}
	double endTime = ( total - shortest - longest ) * ( 1.0 / ( TSP_NUM_FRAME_TIMES - 2 ) );
	double timeLeft = endTime - now;
	if ( timeLeft < MIN_FRAME_TIME ) {
		timeLeft = MIN_FRAME_TIME;
	}

	for ( i = 0; i < (int)processes[UNSTOPPABLE].GetSize(); i++ ) {
		TimeSliceProcess *process = processes[UNSTOPPABLE].GetAt( i );
		if ( process != NULL && process->IsReady() ) {
			process->Update( g_kSystemWrapper.GetTimeSinceFrameStart() + timeLeft );
		}
	}

	timeLeft = endTime - g_kSystemWrapper.GetTimeSinceFrameStart();
	if ( timeLeft < 0.0 ) {
		timeLeft = 0.0;
	}

	// the priority whose bit the count has just set
	int lastCount = frameCount++;
	int changed = frameCount ^ lastCount;
	for ( int first = CRITICAL; first < NUM_PRIORITIES; first++ ) {
		if ( changed & ( frameCount & ( 1 << ( first - 1 ) ) ) ) {
			break;
		}
	}
	if ( first == NUM_PRIORITIES ) {
		frameCount++;
		first = CRITICAL;
	}

	UpdatePriority( first, &timeLeft, endTime );
	for ( i = CRITICAL; timeLeft > 0.0 && i < NUM_PRIORITIES; i++ ) {
		if ( i != first ) {
			UpdatePriority( i, &timeLeft, endTime );
		}
	}
}

/*
================
TimeSliceProcess::UpdatePriority
================
*/
// FUNCTION: STBC 0x0046f610
// DRAFT: register allocation: retail keeps the scaled priority in ebx and the loop index in edi, this build the other way round (a reference to the array gives the registers but addresses through it; a second index variable and a continue changed nothing)
void TimeSliceProcess::UpdatePriority( int priority_, double *timeLeft, double endTime ) {
	if ( processes[priority_].GetEffectiveSize() < 1 ) {
		return;
	}

	int numUpdated = 0;
	for ( ;; ) {
		bool anyReady = false;
		for ( int i = 0; i < (int)processes[priority_].GetSize(); i++ ) {
			TimeSliceProcess *process = processes[priority_].GetAt( i );
			if ( process != NULL && process->IsReady() ) {
				anyReady = true;
				if ( !process->updated ) {
					process->Update( g_kSystemWrapper.GetTimeSinceFrameStart() + *timeLeft );
					numUpdated++;
					*timeLeft = endTime - g_kSystemWrapper.GetTimeSinceFrameStart();
					if ( *timeLeft < 0.0 ) {
						break;
					}
				}
			}
		}
		if ( numUpdated != 0 || !anyReady ) {
			return;
		}

		// every process that was due had its turn: start the round again
		for ( i = 0; i < (int)processes[priority_].GetSize(); i++ ) {
			TimeSliceProcess *process = processes[priority_].GetAt( i );
			if ( process != NULL ) {
				process->updated = false;
			}
		}
	}
}

/*
================
TimeSliceProcess::SetPriority
================
*/
// FUNCTION: STBC 0x0046f720
void TimeSliceProcess::SetPriority( int priority_ ) {
	RemoveFromArray( this );
	priority = priority_;
	AddToArray( this );
}

/*
================
TimeSliceProcess::IsReady
================
*/
// FUNCTION: STBC 0x0046f740
bool TimeSliceProcess::IsReady( void ) {
	if ( createdUpdate == g_kSystemWrapper.GetUpdateNumber() ) {
		return false;
	}
	float time;
	if ( delayUsesGameTime ) {
		time = g_kUtopiaModule.GetGameTime();
	} else {
		time = g_kSystemWrapper.GetTimeInSeconds();
	}
	return time - lastUpdateTime >= delay;
}

/*
================
TimeSliceProcess::RemoveAllProcesses
================
*/
// FUNCTION: STBC 0x0046f790
void TimeSliceProcess::RemoveAllProcesses( void ) {
	for ( int i = 0; i < NUM_PRIORITIES; i++ ) {
		processes[i].RemoveAll();
	}
	frameTimeIndex = 0;
	frameCount = 0;
}

/*
================
TimeSliceProcess::AddToArray
================
*/
// FUNCTION: STBC 0x0046f7d0
void TimeSliceProcess::AddToArray( TimeSliceProcess *process ) {
	processes[process->priority].AddFirstEmpty( this );
}

/*
================
TimeSliceProcess::RemoveFromArray
================
*/
// FUNCTION: STBC 0x0046f8d0
void TimeSliceProcess::RemoveFromArray( TimeSliceProcess *process ) {
	NiTArray<TimeSliceProcess *> &array = processes[process->priority];
	for ( int i = 0; i < (int)array.GetSize(); i++ ) {
		if ( array.GetAt( i ) == this ) {
			array.RemoveAt( i );
			return;
		}
	}
}

/*
================
PythonMethodProcess::Update

Calls the instance's method with the time.
================
*/
// FUNCTION: STBC 0x0046f930
void PythonMethodProcess::Update( double time ) {
	TimeSliceProcess::Update( time );
	PyEmbed::CallMethod( instance, function, "", NULL, "(f)", time );
	PyErr_Print();
}

/*
================
PythonMethodProcess_SetInstance

The Python method SetInstance of PythonMethodProcess, installed by hand:
it takes the raw Python object. The instance is not counted (the
function's name is invented after the method).
================
*/
// FUNCTION: STBC 0x0046f970
PyObject *PythonMethodProcess_SetInstance( PyObject *self, PyObject *args ) {
	PyObject *processObject = NULL;
	PyObject *instance = NULL;
	if ( PyArg_ParseTuple( args, "OO", &processObject, &instance ) != -1 ) {
		PythonMethodProcess *process = NULL;
		PyEmbed::Unknown6F98E0( processObject, (void **)&process, "PythonMethodProcess" );
		if ( process != NULL ) {
			process->SetInstance( instance );
			Py_INCREF( Py_None );
			return Py_None;
		}
	}
	return NULL;
}
