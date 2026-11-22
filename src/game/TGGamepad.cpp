/*
===============================================================================

	TGGamepad.cpp

===============================================================================
*/

#include <iostream>
#include <string.h>

#include "TGGamepad.h"
#include "TGDirectInput7.h"
#include "TGSystemWrapperClass.h"
#include "NiApplication.h"
#include "NiAppWindow.h"
#include "NiMain.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x00722410 _$E415
// FUNCTION: STBC 0x00722430 _$E414

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x007222b0 _$E32
// FUNCTION: STBC 0x007222c0 _$E29
// FUNCTION: STBC 0x007222d0 _$E31
// FUNCTION: STBC 0x007222e0 _$E30
// FUNCTION: STBC 0x007222f0 _$E38
// FUNCTION: STBC 0x00722300 _$E35
// FUNCTION: STBC 0x00722310 _$E37
// FUNCTION: STBC 0x00722320 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00722330 _$E51
// FUNCTION: STBC 0x00722340 _$E50
// FUNCTION: STBC 0x00722350 _$E207
// FUNCTION: STBC 0x00722360 _$E206
// FUNCTION: STBC 0x00722370 _$E62
// FUNCTION: STBC 0x00722380 _$E61
// FUNCTION: STBC 0x00722390 _$E66
// FUNCTION: STBC 0x007223a0 _$E65
// FUNCTION: STBC 0x007223b0 _$E70
// FUNCTION: STBC 0x007223c0 _$E69
// FUNCTION: STBC 0x007223d0 _$E74
// FUNCTION: STBC 0x007223e0 _$E73
// FUNCTION: STBC 0x007223f0 _$E194
// FUNCTION: STBC 0x00722400 _$E193
// FUNCTION: STBC 0x00722440 _$E136
// FUNCTION: STBC 0x00722450 _$E135
// FUNCTION: STBC 0x00722460 _$E149
// FUNCTION: STBC 0x00722470 _$E148
// FUNCTION: STBC 0x00722480 _$E199
// FUNCTION: STBC 0x00722490 _$E198
// FUNCTION: STBC 0x007224a0 _$E212
// FUNCTION: STBC 0x007224b0 _$E211
// FUNCTION: STBC 0x007224c0 _$E203
// FUNCTION: STBC 0x007224d0 _$E202
// FUNCTION: STBC 0x007224e0 _$E216
// FUNCTION: STBC 0x007224f0 _$E215
// FUNCTION: STBC 0x00722500 _$E223
// FUNCTION: STBC 0x00722510 _$E222
// FUNCTION: STBC 0x00722520 _$E253
// FUNCTION: STBC 0x00722530 _$E252
// FUNCTION: STBC 0x00722540 _$E257
// FUNCTION: STBC 0x00722550 _$E256
// FUNCTION: STBC 0x00722560 _$E98
// FUNCTION: STBC 0x00722570 _$E97
// FUNCTION: STBC 0x00722580 _$E239
// FUNCTION: STBC 0x00722590 _$E238
// FUNCTION: STBC 0x007225a0 _$E246
// FUNCTION: STBC 0x007225b0 _$E245
// FUNCTION: STBC 0x007225c0 _$E269
// FUNCTION: STBC 0x007225d0 _$E268
// FUNCTION: STBC 0x007225e0 _$E273
// FUNCTION: STBC 0x007225f0 _$E272
// FUNCTION: STBC 0x00722600 _$E313
// FUNCTION: STBC 0x00722610 _$E312
// FUNCTION: STBC 0x00722620 _$E319
// FUNCTION: STBC 0x00722630 _$E318
// FUNCTION: STBC 0x00722640 _$E261
// FUNCTION: STBC 0x00722650 _$E260
// FUNCTION: STBC 0x00722660 _$E265
// FUNCTION: STBC 0x00722670 _$E264
// FUNCTION: STBC 0x00722680 _$E277
// FUNCTION: STBC 0x00722690 _$E276
// FUNCTION: STBC 0x007226a0 _$E281
// FUNCTION: STBC 0x007226b0 _$E280
// FUNCTION: STBC 0x007226c0 _$E285
// FUNCTION: STBC 0x007226d0 _$E284
// FUNCTION: STBC 0x007226e0 _$E289
// FUNCTION: STBC 0x007226f0 _$E288
// FUNCTION: STBC 0x00722700 _$E293
// FUNCTION: STBC 0x00722710 _$E292
// FUNCTION: STBC 0x00722720 _$E297
// FUNCTION: STBC 0x00722730 _$E296
// FUNCTION: STBC 0x00722740 _$E301
// FUNCTION: STBC 0x00722750 _$E300
// FUNCTION: STBC 0x00722760 _$E305
// FUNCTION: STBC 0x00722770 _$E304
// FUNCTION: STBC 0x00722780 _$E103
// FUNCTION: STBC 0x00722790 _$E102
// FUNCTION: STBC 0x007227a0 _$E309
// FUNCTION: STBC 0x007227b0 _$E308
// FUNCTION: STBC 0x007227c0 _$E108
// FUNCTION: STBC 0x007227d0 _$E107
// FUNCTION: STBC 0x007227e0 _$E323
// FUNCTION: STBC 0x007227f0 _$E322
// FUNCTION: STBC 0x00722800 _$E327
// FUNCTION: STBC 0x00722810 _$E326
// FUNCTION: STBC 0x00722820 _$E331
// FUNCTION: STBC 0x00722830 _$E330
// FUNCTION: STBC 0x00722840 _$E335
// FUNCTION: STBC 0x00722850 _$E334
// FUNCTION: STBC 0x00722860 _$E113
// FUNCTION: STBC 0x00722870 _$E112
// FUNCTION: STBC 0x00722880 _$E339
// FUNCTION: STBC 0x00722890 _$E338
// FUNCTION: STBC 0x007228a0 _$E343
// FUNCTION: STBC 0x007228b0 _$E342
// FUNCTION: STBC 0x007228c0 _$E347
// FUNCTION: STBC 0x007228d0 _$E346
// FUNCTION: STBC 0x007228e0 _$E119
// FUNCTION: STBC 0x007228f0 _$E118
// FUNCTION: STBC 0x00722900 _$E124
// FUNCTION: STBC 0x00722910 _$E123
// FUNCTION: STBC 0x00722920 _$E154
// FUNCTION: STBC 0x00722930 _$E153
// FUNCTION: STBC 0x00722940 _$E351
// FUNCTION: STBC 0x00722950 _$E350
// FUNCTION: STBC 0x00722960 _$E159
// FUNCTION: STBC 0x00722970 _$E158
// FUNCTION: STBC 0x00722980 _$E355
// FUNCTION: STBC 0x00722990 _$E354
// FUNCTION: STBC 0x007229a0 _$E359
// FUNCTION: STBC 0x007229b0 _$E358
// FUNCTION: STBC 0x007229c0 _$E164
// FUNCTION: STBC 0x007229d0 _$E163
// FUNCTION: STBC 0x007229e0 _$E363
// FUNCTION: STBC 0x007229f0 _$E362
// FUNCTION: STBC 0x00722a00 _$E129
// FUNCTION: STBC 0x00722a10 _$E128
// FUNCTION: STBC 0x00722a20 _$E367
// FUNCTION: STBC 0x00722a30 _$E366
// FUNCTION: STBC 0x00722a40 _$E371
// FUNCTION: STBC 0x00722a50 _$E370
// FUNCTION: STBC 0x00722a60 _$E169
// FUNCTION: STBC 0x00722a70 _$E168
// FUNCTION: STBC 0x00722a80 _$E174
// FUNCTION: STBC 0x00722a90 _$E173
// FUNCTION: STBC 0x00722aa0 _$E375
// FUNCTION: STBC 0x00722ab0 _$E374
// FUNCTION: STBC 0x00722ac0 _$E379
// FUNCTION: STBC 0x00722ad0 _$E378
// FUNCTION: STBC 0x00722ae0 _$E383
// FUNCTION: STBC 0x00722af0 _$E382
// FUNCTION: STBC 0x00722b00 _$E387
// FUNCTION: STBC 0x00722b10 _$E386
// FUNCTION: STBC 0x00722b20 _$E391
// FUNCTION: STBC 0x00722b30 _$E390
// FUNCTION: STBC 0x00722b40 _$E395
// FUNCTION: STBC 0x00722b50 _$E394
// FUNCTION: STBC 0x00722b60 _$E399
// FUNCTION: STBC 0x00722b70 _$E398
// FUNCTION: STBC 0x00722b80 _$E403
// FUNCTION: STBC 0x00722b90 _$E402
// FUNCTION: STBC 0x00722ba0 _$E179
// FUNCTION: STBC 0x00722bb0 _$E178
// FUNCTION: STBC 0x00722bc0 _$E407
// FUNCTION: STBC 0x00722bd0 _$E406
// FUNCTION: STBC 0x00722be0 _$E184
// FUNCTION: STBC 0x00722bf0 _$E183
// FUNCTION: STBC 0x00722c00 _$E189
// FUNCTION: STBC 0x00722c10 _$E188

// DATA: STBC 0x0099cbf0
TGGamepad::JoystickList TGGamepad::joysticks;

// DATA: STBC 0x0099cc3c
int TGGamepad::refCount;

/*
================
TGGamepad::TGGamepad

Takes the index's joystick, if there is one: each axis it has is scaled to
GAMEPAD_AXIS_MINIMUM to GAMEPAD_AXIS_MAXIMUM, and the device is acquired.
================
*/
// FUNCTION: STBC 0x00721d50
TGGamepad::TGGamepad( NiApplication *application, int index ) {
	if ( refCount == 0 ) {
		EnumerateDevices( application );
	}
	refCount++;
	memset( &state, 0, sizeof( state ) );

	IDirectInputDevice2A *joystick = joysticks.devices[index];
	device = joystick;
	if ( joystick == NULL ) {
		return;
	}

	DIPROPRANGE range;
	range.diph.dwSize = sizeof( DIPROPRANGE );
	range.diph.dwHeaderSize = sizeof( DIPROPHEADER );
	range.diph.dwObj = DIJOFS_X;
	range.diph.dwHow = DIPH_BYOFFSET;
	if ( SUCCEEDED( joystick->GetProperty( DIPROP_RANGE, &range.diph ) ) ) {
		state.axes |= GAMEPAD_AXIS_BIT( GAMEPAD_AXIS_X );
		state.minimums[GAMEPAD_AXIS_X] = range.lMin = GAMEPAD_AXIS_MINIMUM;
		state.maximums[GAMEPAD_AXIS_X] = range.lMax = GAMEPAD_AXIS_MAXIMUM;
		joystick->SetProperty( DIPROP_RANGE, &range.diph );
	}
	range.diph.dwObj = DIJOFS_Y;
	if ( SUCCEEDED( joystick->GetProperty( DIPROP_RANGE, &range.diph ) ) ) {
		state.axes |= GAMEPAD_AXIS_BIT( GAMEPAD_AXIS_Y );
		state.minimums[GAMEPAD_AXIS_Y] = range.lMin = GAMEPAD_AXIS_MINIMUM;
		state.maximums[GAMEPAD_AXIS_Y] = range.lMax = GAMEPAD_AXIS_MAXIMUM;
		joystick->SetProperty( DIPROP_RANGE, &range.diph );
	}
	range.diph.dwObj = DIJOFS_Z;
	if ( SUCCEEDED( joystick->GetProperty( DIPROP_RANGE, &range.diph ) ) ) {
		state.axes |= GAMEPAD_AXIS_BIT( GAMEPAD_AXIS_Z );
		state.minimums[GAMEPAD_AXIS_Z] = range.lMin = GAMEPAD_AXIS_MINIMUM;
		state.maximums[GAMEPAD_AXIS_Z] = range.lMax = GAMEPAD_AXIS_MAXIMUM;
		joystick->SetProperty( DIPROP_RANGE, &range.diph );
	}
	range.diph.dwObj = DIJOFS_RX;
	if ( SUCCEEDED( joystick->GetProperty( DIPROP_RANGE, &range.diph ) ) ) {
		state.axes |= GAMEPAD_AXIS_BIT( GAMEPAD_AXIS_RX );
		state.minimums[GAMEPAD_AXIS_RX] = range.lMin = GAMEPAD_AXIS_MINIMUM;
		state.maximums[GAMEPAD_AXIS_RX] = range.lMax = GAMEPAD_AXIS_MAXIMUM;
		joystick->SetProperty( DIPROP_RANGE, &range.diph );
	}
	range.diph.dwObj = DIJOFS_RY;
	if ( SUCCEEDED( joystick->GetProperty( DIPROP_RANGE, &range.diph ) ) ) {
		state.axes |= GAMEPAD_AXIS_BIT( GAMEPAD_AXIS_RY );
		state.minimums[GAMEPAD_AXIS_RY] = range.lMin = GAMEPAD_AXIS_MINIMUM;
		state.maximums[GAMEPAD_AXIS_RY] = range.lMax = GAMEPAD_AXIS_MAXIMUM;
		joystick->SetProperty( DIPROP_RANGE, &range.diph );
	}
	range.diph.dwObj = DIJOFS_RZ;
	if ( SUCCEEDED( joystick->GetProperty( DIPROP_RANGE, &range.diph ) ) ) {
		state.axes |= GAMEPAD_AXIS_BIT( GAMEPAD_AXIS_RZ );
		state.minimums[GAMEPAD_AXIS_RZ] = range.lMin = GAMEPAD_AXIS_MINIMUM;
		state.maximums[GAMEPAD_AXIS_RZ] = range.lMax = GAMEPAD_AXIS_MAXIMUM;
		joystick->SetProperty( DIPROP_RANGE, &range.diph );
	}
	range.diph.dwObj = DIJOFS_SLIDER( 0 );
	if ( SUCCEEDED( joystick->GetProperty( DIPROP_RANGE, &range.diph ) ) ) {
		state.axes |= GAMEPAD_AXIS_BIT( GAMEPAD_AXIS_SLIDER0 );
		state.minimums[GAMEPAD_AXIS_SLIDER0] = range.lMin = GAMEPAD_AXIS_MINIMUM;
		state.maximums[GAMEPAD_AXIS_SLIDER0] = range.lMax = GAMEPAD_AXIS_MAXIMUM;
		joystick->SetProperty( DIPROP_RANGE, &range.diph );
	}
	range.diph.dwObj = DIJOFS_SLIDER( 1 );
	if ( SUCCEEDED( joystick->GetProperty( DIPROP_RANGE, &range.diph ) ) ) {
		state.axes |= GAMEPAD_AXIS_BIT( GAMEPAD_AXIS_SLIDER1 );
		state.minimums[GAMEPAD_AXIS_SLIDER1] = range.lMin = GAMEPAD_AXIS_MINIMUM;
		state.maximums[GAMEPAD_AXIS_SLIDER1] = range.lMax = GAMEPAD_AXIS_MAXIMUM;
		joystick->SetProperty( DIPROP_RANGE, &range.diph );
	}

	DIDEVICEOBJECTINSTANCE pov;
	pov.dwSize = sizeof( DIDEVICEOBJECTINSTANCE );
	if ( SUCCEEDED( joystick->GetObjectInfo( &pov, DIJOFS_POV( 0 ), DIPH_BYOFFSET ) ) ) {
		state.axes |= GAMEPAD_AXIS_BIT( GAMEPAD_AXIS_POV );
		state.minimums[GAMEPAD_AXIS_POV] = 0;
		state.maximums[GAMEPAD_AXIS_POV] = GAMEPAD_POV_MAXIMUM;
	} else {
		state.minimums[GAMEPAD_AXIS_POV] = 0;
		state.maximums[GAMEPAD_AXIS_POV] = 0;
	}
	joystick->Acquire();
}

/*
================
TGGamepad::~TGGamepad
================
*/
// FUNCTION: STBC 0x00722020
TGGamepad::~TGGamepad( void ) {
	if ( refCount == 1 ) {
		ReleaseDevices();
	}
	refCount--;
}

/*
================
TGGamepad::EnumerateDevices
================
*/
// FUNCTION: STBC 0x00722040
void TGGamepad::EnumerateDevices( NiApplication *application ) {
	joysticks.numDevices = 0;
	IDirectInput7A *input = g_kSystemWrapper.GetDirectInput();
	if ( input != NULL ) {
		input->EnumDevices( DIDEVTYPE_JOYSTICK, EnumerateDevice, application, DIEDFL_ATTACHEDONLY );
	}
}

/*
================
TGGamepad::ReleaseDevices
================
*/
// FUNCTION: STBC 0x00722070
void TGGamepad::ReleaseDevices( void ) {
	for ( int i = 0; i < TGGAMEPAD_MAX_DEVICES; i++ ) {
		if ( joysticks.devices[i] != NULL ) {
			joysticks.devices[i]->Unacquire();
			joysticks.devices[i]->Release();
			joysticks.devices[i] = NULL;
		}
	}
	joysticks.numDevices = 0;
}

/*
================
TGGamepad::Poll
================
*/
// FUNCTION: STBC 0x007220d0
bool TGGamepad::Poll( void ) {
	IDirectInputDevice2A *joystick = device;
	if ( joystick == NULL ) {
		return false;
	}
	joystick->Poll();

	DIJOYSTATE joyState;
	HRESULT result = joystick->GetDeviceState( sizeof( DIJOYSTATE ), &joyState );
	if ( result != DI_OK ) {
		if ( result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED ) {
			joystick->Acquire();
		}
		return false;
	}

	if ( state.axes & GAMEPAD_AXIS_BIT( GAMEPAD_AXIS_X ) ) {
		state.values[GAMEPAD_AXIS_X] = joyState.lX;
	} else {
		state.values[GAMEPAD_AXIS_X] = 0;
	}
	if ( state.axes & GAMEPAD_AXIS_BIT( GAMEPAD_AXIS_Y ) ) {
		state.values[GAMEPAD_AXIS_Y] = joyState.lY;
	} else {
		// BUG: clears the X axis, not the Y axis.
		state.values[GAMEPAD_AXIS_X] = 0;
	}
	if ( state.axes & GAMEPAD_AXIS_BIT( GAMEPAD_AXIS_Z ) ) {
		state.values[GAMEPAD_AXIS_Z] = joyState.lZ;
	} else {
		state.values[GAMEPAD_AXIS_Z] = 0;
	}
	if ( state.axes & GAMEPAD_AXIS_BIT( GAMEPAD_AXIS_RZ ) ) {
		state.values[GAMEPAD_AXIS_RZ] = joyState.lRz;
	} else {
		state.values[GAMEPAD_AXIS_RZ] = 0;
	}
	if ( state.axes & GAMEPAD_AXIS_BIT( GAMEPAD_AXIS_RX ) ) {
		state.values[GAMEPAD_AXIS_RX] = joyState.lRx;
	} else {
		state.values[GAMEPAD_AXIS_RX] = 0;
	}
	if ( state.axes & GAMEPAD_AXIS_BIT( GAMEPAD_AXIS_RY ) ) {
		state.values[GAMEPAD_AXIS_RY] = joyState.lRy;
	} else {
		state.values[GAMEPAD_AXIS_RY] = 0;
	}
	if ( state.axes & GAMEPAD_AXIS_BIT( GAMEPAD_AXIS_SLIDER0 ) ) {
		state.values[GAMEPAD_AXIS_SLIDER0] = joyState.rglSlider[0];
	} else {
		state.values[GAMEPAD_AXIS_SLIDER0] = 0;
	}
	if ( state.axes & GAMEPAD_AXIS_BIT( GAMEPAD_AXIS_SLIDER1 ) ) {
		state.values[GAMEPAD_AXIS_SLIDER1] = joyState.rglSlider[1];
	} else {
		state.values[GAMEPAD_AXIS_SLIDER1] = 0;
	}
	if ( state.axes & GAMEPAD_AXIS_BIT( GAMEPAD_AXIS_POV ) ) {
		state.values[GAMEPAD_AXIS_POV] = joyState.rgdwPOV[0];
	}

	for ( int i = 0; i < TGGAMEPAD_MAX_BUTTONS; i++ ) {
		if ( joyState.rgbButtons[i] & 0x80 ) {
			state.buttons |= 1 << i;
			state.buttonStates[i] = 0xFF;
		} else {
			state.buttons &= ~( 1 << i );
			state.buttonStates[i] = 0;
		}
	}
	return true;
}

/*
================
TGGamepad::EnumerateDevice

Keeps each attached joystick, up to TGGAMEPAD_MAX_DEVICES, set up to read
joystick states in the foreground without exclusive access.
================
*/
// FUNCTION: STBC 0x007221f0
int __stdcall TGGamepad::EnumerateDevice( const DIDEVICEINSTANCEA *instance, void *application ) {
	IDirectInputDevice2A *joystick;
	IDirectInput7A *input = g_kSystemWrapper.GetDirectInput();
	if ( input != NULL && input->CreateDeviceEx( instance->guidInstance, IID_IDirectInputDevice7A, (void **)&joystick, NULL ) == DI_OK &&
		joysticks.numDevices < TGGAMEPAD_MAX_DEVICES ) {
		// BUG: a joystick past the last slot is created and never released.
		if ( joystick->SetDataFormat( &c_dfDIJoystick ) != DI_OK ) {
			joystick->Release();
			return DIENUM_CONTINUE;
		}
		if ( joystick->SetCooperativeLevel( ( (NiApplication *)application )->GetAppWindow()->GetWindow(), DISCL_NONEXCLUSIVE | DISCL_FOREGROUND ) != DI_OK ) {
			joystick->Release();
			return DIENUM_CONTINUE;
		}
		joysticks.devices[joysticks.numDevices] = joystick;
		joysticks.numDevices++;
	}
	return DIENUM_CONTINUE;
}
