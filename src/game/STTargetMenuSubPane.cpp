/*
===============================================================================

	STTargetMenuSubPane.cpp

	The target list's sub pane. Retail keeps it in a unit of its own after
	STTargetMenu's: its handler table's initialiser (0x539A70) and code
	(0x539AC0-0x539CF0), then its <iostream> quad and the NetImmerse
	headers' class registrations (0x539D00-0x53A6C0), their statics in
	.bss 0x989E58-0x989F98. The file name is invented.

===============================================================================
*/

#include <iostream>
#include "STTargetMenuSubPane.h"
#include "TGEventHandlerTable.h"
#include "TGStream.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x0053a100 _$E443
// FUNCTION: STBC 0x0053a120 _$E442

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00539d80 _$E57
// FUNCTION: STBC 0x00539d90 _$E56
// FUNCTION: STBC 0x00539da0 _$E61
// FUNCTION: STBC 0x00539db0 _$E60
// FUNCTION: STBC 0x00539dc0 _$E72
// FUNCTION: STBC 0x00539dd0 _$E71
// FUNCTION: STBC 0x00539de0 _$E76
// FUNCTION: STBC 0x00539df0 _$E75
// FUNCTION: STBC 0x00539e00 _$E80
// FUNCTION: STBC 0x00539e10 _$E79
// FUNCTION: STBC 0x00539e20 _$E84
// FUNCTION: STBC 0x00539e30 _$E83
// FUNCTION: STBC 0x00539e40 _$E208
// FUNCTION: STBC 0x00539e50 _$E207
// FUNCTION: STBC 0x00539e60 _$E150
// FUNCTION: STBC 0x00539e70 _$E149
// FUNCTION: STBC 0x00539e80 _$E163
// FUNCTION: STBC 0x00539e90 _$E162
// FUNCTION: STBC 0x00539ea0 _$E213
// FUNCTION: STBC 0x00539eb0 _$E212
// FUNCTION: STBC 0x00539ec0 _$E218
// FUNCTION: STBC 0x00539ed0 _$E217
// FUNCTION: STBC 0x00539ee0 _$E222
// FUNCTION: STBC 0x00539ef0 _$E221
// FUNCTION: STBC 0x00539f00 _$E226
// FUNCTION: STBC 0x00539f10 _$E225
// FUNCTION: STBC 0x00539f20 _$E233
// FUNCTION: STBC 0x00539f30 _$E232
// FUNCTION: STBC 0x00539f40 _$E262
// FUNCTION: STBC 0x00539f50 _$E261
// FUNCTION: STBC 0x00539f60 _$E266
// FUNCTION: STBC 0x00539f70 _$E265
// FUNCTION: STBC 0x00539f80 _$E108
// FUNCTION: STBC 0x00539f90 _$E107
// FUNCTION: STBC 0x00539fa0 _$E248
// FUNCTION: STBC 0x00539fb0 _$E247
// FUNCTION: STBC 0x00539fc0 _$E255
// FUNCTION: STBC 0x00539fd0 _$E254
// FUNCTION: STBC 0x00539fe0 _$E278
// FUNCTION: STBC 0x00539ff0 _$E277
// FUNCTION: STBC 0x0053a000 _$E282
// FUNCTION: STBC 0x0053a010 _$E281
// FUNCTION: STBC 0x0053a020 _$E322
// FUNCTION: STBC 0x0053a030 _$E321
// FUNCTION: STBC 0x0053a040 _$E328
// FUNCTION: STBC 0x0053a050 _$E327
// FUNCTION: STBC 0x0053a060 _$E270
// FUNCTION: STBC 0x0053a070 _$E269
// FUNCTION: STBC 0x0053a080 _$E274
// FUNCTION: STBC 0x0053a090 _$E273
// FUNCTION: STBC 0x0053a0a0 _$E286
// FUNCTION: STBC 0x0053a0b0 _$E285
// FUNCTION: STBC 0x0053a0c0 _$E290
// FUNCTION: STBC 0x0053a0d0 _$E289
// FUNCTION: STBC 0x0053a0e0 _$E294
// FUNCTION: STBC 0x0053a0f0 _$E293
// FUNCTION: STBC 0x0053a130 _$E298
// FUNCTION: STBC 0x0053a140 _$E297
// FUNCTION: STBC 0x0053a150 _$E302
// FUNCTION: STBC 0x0053a160 _$E301
// FUNCTION: STBC 0x0053a170 _$E306
// FUNCTION: STBC 0x0053a180 _$E305
// FUNCTION: STBC 0x0053a190 _$E310
// FUNCTION: STBC 0x0053a1a0 _$E309
// FUNCTION: STBC 0x0053a1b0 _$E314
// FUNCTION: STBC 0x0053a1c0 _$E313
// FUNCTION: STBC 0x0053a1d0 _$E113
// FUNCTION: STBC 0x0053a1e0 _$E112
// FUNCTION: STBC 0x0053a1f0 _$E318
// FUNCTION: STBC 0x0053a200 _$E317
// FUNCTION: STBC 0x0053a210 _$E118
// FUNCTION: STBC 0x0053a220 _$E117
// FUNCTION: STBC 0x0053a230 _$E332
// FUNCTION: STBC 0x0053a240 _$E331
// FUNCTION: STBC 0x0053a250 _$E336
// FUNCTION: STBC 0x0053a260 _$E335
// FUNCTION: STBC 0x0053a270 _$E340
// FUNCTION: STBC 0x0053a280 _$E339
// FUNCTION: STBC 0x0053a290 _$E344
// FUNCTION: STBC 0x0053a2a0 _$E343
// FUNCTION: STBC 0x0053a2b0 _$E127
// FUNCTION: STBC 0x0053a2c0 _$E126
// FUNCTION: STBC 0x0053a2d0 _$E348
// FUNCTION: STBC 0x0053a2e0 _$E347
// FUNCTION: STBC 0x0053a2f0 _$E352
// FUNCTION: STBC 0x0053a300 _$E351
// FUNCTION: STBC 0x0053a310 _$E356
// FUNCTION: STBC 0x0053a320 _$E355
// FUNCTION: STBC 0x0053a330 _$E133
// FUNCTION: STBC 0x0053a340 _$E132
// FUNCTION: STBC 0x0053a350 _$E138
// FUNCTION: STBC 0x0053a360 _$E137
// FUNCTION: STBC 0x0053a370 _$E168
// FUNCTION: STBC 0x0053a380 _$E167
// FUNCTION: STBC 0x0053a390 _$E360
// FUNCTION: STBC 0x0053a3a0 _$E359
// FUNCTION: STBC 0x0053a3b0 _$E173
// FUNCTION: STBC 0x0053a3c0 _$E172
// FUNCTION: STBC 0x0053a3d0 _$E364
// FUNCTION: STBC 0x0053a3e0 _$E363
// FUNCTION: STBC 0x0053a3f0 _$E368
// FUNCTION: STBC 0x0053a400 _$E367
// FUNCTION: STBC 0x0053a410 _$E178
// FUNCTION: STBC 0x0053a420 _$E177
// FUNCTION: STBC 0x0053a430 _$E372
// FUNCTION: STBC 0x0053a440 _$E371
// FUNCTION: STBC 0x0053a450 _$E143
// FUNCTION: STBC 0x0053a460 _$E142
// FUNCTION: STBC 0x0053a470 _$E376
// FUNCTION: STBC 0x0053a480 _$E375
// FUNCTION: STBC 0x0053a490 _$E380
// FUNCTION: STBC 0x0053a4a0 _$E379
// FUNCTION: STBC 0x0053a4b0 _$E183
// FUNCTION: STBC 0x0053a4c0 _$E182
// FUNCTION: STBC 0x0053a4d0 _$E188
// FUNCTION: STBC 0x0053a4e0 _$E187
// FUNCTION: STBC 0x0053a4f0 _$E384
// FUNCTION: STBC 0x0053a500 _$E383
// FUNCTION: STBC 0x0053a510 _$E388
// FUNCTION: STBC 0x0053a520 _$E387
// FUNCTION: STBC 0x0053a530 _$E392
// FUNCTION: STBC 0x0053a540 _$E391
// FUNCTION: STBC 0x0053a550 _$E396
// FUNCTION: STBC 0x0053a560 _$E395
// FUNCTION: STBC 0x0053a570 _$E400
// FUNCTION: STBC 0x0053a580 _$E399
// FUNCTION: STBC 0x0053a590 _$E404
// FUNCTION: STBC 0x0053a5a0 _$E403
// FUNCTION: STBC 0x0053a5b0 _$E408
// FUNCTION: STBC 0x0053a5c0 _$E407
// FUNCTION: STBC 0x0053a5d0 _$E412
// FUNCTION: STBC 0x0053a5e0 _$E411
// FUNCTION: STBC 0x0053a5f0 _$E193
// FUNCTION: STBC 0x0053a600 _$E192
// FUNCTION: STBC 0x0053a610 _$E416
// FUNCTION: STBC 0x0053a620 _$E415
// FUNCTION: STBC 0x0053a630 _$E198
// FUNCTION: STBC 0x0053a640 _$E197
// FUNCTION: STBC 0x0053a650 _$E203
// FUNCTION: STBC 0x0053a660 _$E202
// FUNCTION: STBC 0x0053a670 _$E420
// FUNCTION: STBC 0x0053a680 _$E419
// FUNCTION: STBC 0x0053a690 _$E425
// FUNCTION: STBC 0x0053a6a0 _$E424
// FUNCTION: STBC 0x0053a6b0 _$E429
// FUNCTION: STBC 0x0053a6c0 _$E428

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00539d00 _$E32
// FUNCTION: STBC 0x00539d10 _$E29
// FUNCTION: STBC 0x00539d20 _$E31
// FUNCTION: STBC 0x00539d30 _$E30
// FUNCTION: STBC 0x00539d40 _$E38
// FUNCTION: STBC 0x00539d50 _$E35
// FUNCTION: STBC 0x00539d60 _$E37
// FUNCTION: STBC 0x00539d70 _$E36

// The handler table's dynamic initialiser and exit-time destructor.
// FUNCTION: STBC 0x00539a70 _$E435
// FUNCTION: STBC 0x00539a80 _$E431
// FUNCTION: STBC 0x00539a90 _$E434
// FUNCTION: STBC 0x00539aa0 _$E432

// DATA: STBC 0x00989e98
TGEventHandlerTable STTargetMenuSubPane::classHandlers;

/*
================
STTargetMenuSubPane::Cast
================
*/
// FUNCTION: STBC 0x00539ac0
STTargetMenuSubPane *STTargetMenuSubPane::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( STTARGETMENUSUBPANE_TYPE ) ) {
		return (STTargetMenuSubPane *)object;
	}
	return NULL;
}

/*
================
STTargetMenuSubPane::STTargetMenuSubPane
================
*/
// FUNCTION: STBC 0x00539af0 ??0STTargetMenuSubPane@@QAE@M_N@Z
STTargetMenuSubPane::STTargetMenuSubPane( float vertSpacing, bool expandToFillParent ) : STSubPane( vertSpacing, expandToFillParent ) {
}

/*
================
STTargetMenuSubPane::STTargetMenuSubPane

An empty pane for the load to fill.
================
*/
// FUNCTION: STBC 0x00539bf0 ??0STTargetMenuSubPane@@QAE@J_N@Z
STTargetMenuSubPane::STTargetMenuSubPane( long id, bool expandToFillParent ) : STSubPane( id, expandToFillParent ) {
}

/*
================
STTargetMenuSubPane::RegisterHandlerNames

The pane has no handlers of its own.
================
*/
// FUNCTION: STBC 0x00539c20
void STTargetMenuSubPane::RegisterHandlerNames( void ) {
}

/*
================
STTargetMenuSubPane::RegisterHandlers
================
*/
// FUNCTION: STBC 0x00539c30
void STTargetMenuSubPane::RegisterHandlers( void ) {
}

/*
================
STTargetMenuSubPane::GotFocus

With no focus and no chosen menu, the first menu takes the focus and is
highlighted.
================
*/
// FUNCTION: STBC 0x00539c40 ?GotFocus@STTargetMenuSubPane@@UAEX_N@Z
void STTargetMenuSubPane::GotFocus( bool layout ) {
	if ( GetFocus() == NULL ) {
		TGUIObject *child;
		for ( child = GetFirstChild(); child != NULL; child = GetNextChild( child ) ) {
			if ( child->IsSelected() ) {
				break;
			}
		}
		if ( child == NULL ) {
			TGUIObject *first = GetFirstChild();
			SetFocus( first, layout );
			if ( first != NULL ) {
				GetFirstChild()->SetHighlighted( true );
			}
		}
	}
	TGUIObject::GotFocus( layout );
}

/*
================
STTargetMenuSubPane::WriteToStream
================
*/
// FUNCTION: STBC 0x00539cc0 ?WriteToStream@STTargetMenuSubPane@@UAE_NPAVTGStream@@@Z
bool STTargetMenuSubPane::WriteToStream( TGStream *stream ) {
	if ( !STSubPane::WriteToStream( stream ) ) {
		return false;
	}
	return true;
}

/*
================
STTargetMenuSubPane::ReadFromStream
================
*/
// FUNCTION: STBC 0x00539ce0 ?ReadFromStream@STTargetMenuSubPane@@UAE_NPAVTGStream@@@Z
bool STTargetMenuSubPane::ReadFromStream( TGStream *stream ) {
	if ( !STSubPane::ReadFromStream( stream ) ) {
		return false;
	}
	return true;
}
