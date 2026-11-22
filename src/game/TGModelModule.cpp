/*
===============================================================================

	TGModelModule.cpp

	The model module: the model manager's purge and the managers' cleanup.
	The unit's name is invented, after its invented class: retail's unit
	runs from the module's static quad at 0x6C2F40 through the module and
	the locale-id pair to its <iostream> quads (0x6C3020) and NetImmerse
	registrations (0x6C30A0-0x6C31FF), whose stores (0x997638-0x99767C)
	surround the module (0x997648) and the unit's <iostream> bytes; the
	model manager's unit follows at 0x6C3200.

===============================================================================
*/

#include <iostream>

#include "TGModelModule.h"
#include "TGModelManager.h"
#include "TGAnimationManagerClass.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x006c30a0 _$E47
// FUNCTION: STBC 0x006c30b0 _$E46
// FUNCTION: STBC 0x006c30c0 _$E51
// FUNCTION: STBC 0x006c30d0 _$E50
// FUNCTION: STBC 0x006c30e0 _$E140
// FUNCTION: STBC 0x006c30f0 _$E139
// FUNCTION: STBC 0x006c3100 _$E153
// FUNCTION: STBC 0x006c3110 _$E152
// FUNCTION: STBC 0x006c3120 _$E62
// FUNCTION: STBC 0x006c3130 _$E61
// FUNCTION: STBC 0x006c3140 _$E66
// FUNCTION: STBC 0x006c3150 _$E65
// FUNCTION: STBC 0x006c3160 _$E70
// FUNCTION: STBC 0x006c3170 _$E69
// FUNCTION: STBC 0x006c3180 _$E74
// FUNCTION: STBC 0x006c3190 _$E73
// FUNCTION: STBC 0x006c31a0 _$E198
// FUNCTION: STBC 0x006c31b0 _$E197
// FUNCTION: STBC 0x006c31c0 _$E203
// FUNCTION: STBC 0x006c31d0 _$E202
// FUNCTION: STBC 0x006c31e0 _$E208
// FUNCTION: STBC 0x006c31f0 _$E207

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x006c2ff0 _$E269
// FUNCTION: STBC 0x006c3010 _$E268

// The standard streams' initialisers and exit-time destructors (<iostream>):
// retail lays them out after the module's code.
// FUNCTION: STBC 0x006c3020 _$E32
// FUNCTION: STBC 0x006c3030 _$E29
// FUNCTION: STBC 0x006c3040 _$E31
// FUNCTION: STBC 0x006c3050 _$E30
// FUNCTION: STBC 0x006c3060 _$E38
// FUNCTION: STBC 0x006c3070 _$E35
// FUNCTION: STBC 0x006c3080 _$E37
// FUNCTION: STBC 0x006c3090 _$E36

// Its construction and exit-time destruction.
// FUNCTION: STBC 0x006c2f40 _$E262
// FUNCTION: STBC 0x006c2f50 _$E259
// FUNCTION: STBC 0x006c2f60 _$E261
// FUNCTION: STBC 0x006c2f70 _$E260
// DATA: STBC 0x00997648
TGModelModule modelModule;		// invented name

/*
================
TGModelModule::VirtualSlot00
================
*/
// FUNCTION: STBC 0x006c2fb0
void TGModelModule::VirtualSlot00( void ) {
}

/*
================
TGModelModule::VirtualSlot04

The models no one uses any more are freed.
================
*/
// FUNCTION: STBC 0x006c2fc0
void TGModelModule::VirtualSlot04( void ) {
	g_kModelManager.Purge();
}

/*
================
TGModelModule::VirtualSlot05

The animations and every model go.
================
*/
// FUNCTION: STBC 0x006c2fd0
void TGModelModule::VirtualSlot05( void ) {
	g_kAnimationManager.ClearAnimations();
	g_kModelManager.FreeAllModels();
}
