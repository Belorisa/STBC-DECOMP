/*
===============================================================================

	TGFrameWindow.cpp

===============================================================================
*/

#include <iostream>
#include "TGFrameWindow.h"
#include "TGFrame.h"
#include "TGEventHandlerTable.h"
#include "TGUITheme.h"
#include "TGUIThemeManager.h"
#include "NiMain.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00744690 _$E32
// FUNCTION: STBC 0x007446a0 _$E29
// FUNCTION: STBC 0x007446b0 _$E31
// FUNCTION: STBC 0x007446c0 _$E30
// FUNCTION: STBC 0x007446d0 _$E38
// FUNCTION: STBC 0x007446e0 _$E35
// FUNCTION: STBC 0x007446f0 _$E37
// FUNCTION: STBC 0x00744700 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00744710 _$E57
// FUNCTION: STBC 0x00744720 _$E56
// FUNCTION: STBC 0x00744730 _$E61
// FUNCTION: STBC 0x00744740 _$E60
// FUNCTION: STBC 0x00744750 _$E72
// FUNCTION: STBC 0x00744760 _$E71
// FUNCTION: STBC 0x00744770 _$E76
// FUNCTION: STBC 0x00744780 _$E75
// FUNCTION: STBC 0x00744790 _$E80
// FUNCTION: STBC 0x007447a0 _$E79
// FUNCTION: STBC 0x007447b0 _$E84
// FUNCTION: STBC 0x007447c0 _$E83
// FUNCTION: STBC 0x007447d0 _$E208
// FUNCTION: STBC 0x007447e0 _$E207
// FUNCTION: STBC 0x007447f0 _$E150
// FUNCTION: STBC 0x00744800 _$E149
// FUNCTION: STBC 0x00744810 _$E163
// FUNCTION: STBC 0x00744820 _$E162
// FUNCTION: STBC 0x00744830 _$E213
// FUNCTION: STBC 0x00744840 _$E212
// FUNCTION: STBC 0x00744850 _$E218
// FUNCTION: STBC 0x00744860 _$E217
// FUNCTION: STBC 0x00744870 _$E222
// FUNCTION: STBC 0x00744880 _$E221
// FUNCTION: STBC 0x00744890 _$E226
// FUNCTION: STBC 0x007448a0 _$E225
// FUNCTION: STBC 0x007448b0 _$E233
// FUNCTION: STBC 0x007448c0 _$E232
// FUNCTION: STBC 0x007448d0 _$E260
// FUNCTION: STBC 0x007448e0 _$E259
// FUNCTION: STBC 0x007448f0 _$E264
// FUNCTION: STBC 0x00744900 _$E263
// FUNCTION: STBC 0x00744910 _$E108
// FUNCTION: STBC 0x00744920 _$E107
// FUNCTION: STBC 0x00744930 _$E246
// FUNCTION: STBC 0x00744940 _$E245
// FUNCTION: STBC 0x00744950 _$E253
// FUNCTION: STBC 0x00744960 _$E252
// FUNCTION: STBC 0x00744970 _$E276
// FUNCTION: STBC 0x00744980 _$E275
// FUNCTION: STBC 0x00744990 _$E280
// FUNCTION: STBC 0x007449a0 _$E279
// FUNCTION: STBC 0x007449b0 _$E320
// FUNCTION: STBC 0x007449c0 _$E319
// FUNCTION: STBC 0x007449d0 _$E326
// FUNCTION: STBC 0x007449e0 _$E325
// FUNCTION: STBC 0x007449f0 _$E268
// FUNCTION: STBC 0x00744a00 _$E267
// FUNCTION: STBC 0x00744a10 _$E272
// FUNCTION: STBC 0x00744a20 _$E271
// FUNCTION: STBC 0x00744a30 _$E284
// FUNCTION: STBC 0x00744a40 _$E283
// FUNCTION: STBC 0x00744a50 _$E288
// FUNCTION: STBC 0x00744a60 _$E287
// FUNCTION: STBC 0x00744a70 _$E292
// FUNCTION: STBC 0x00744a80 _$E291
// FUNCTION: STBC 0x00744a90 _$E296
// FUNCTION: STBC 0x00744aa0 _$E295
// FUNCTION: STBC 0x00744ab0 _$E300
// FUNCTION: STBC 0x00744ac0 _$E299
// FUNCTION: STBC 0x00744ad0 _$E304
// FUNCTION: STBC 0x00744ae0 _$E303
// FUNCTION: STBC 0x00744af0 _$E308
// FUNCTION: STBC 0x00744b00 _$E307
// FUNCTION: STBC 0x00744b10 _$E312
// FUNCTION: STBC 0x00744b20 _$E311
// FUNCTION: STBC 0x00744b30 _$E113
// FUNCTION: STBC 0x00744b40 _$E112
// FUNCTION: STBC 0x00744b50 _$E316
// FUNCTION: STBC 0x00744b60 _$E315
// FUNCTION: STBC 0x00744b70 _$E118
// FUNCTION: STBC 0x00744b80 _$E117
// FUNCTION: STBC 0x00744b90 _$E330
// FUNCTION: STBC 0x00744ba0 _$E329
// FUNCTION: STBC 0x00744bb0 _$E334
// FUNCTION: STBC 0x00744bc0 _$E333
// FUNCTION: STBC 0x00744bd0 _$E338
// FUNCTION: STBC 0x00744be0 _$E337
// FUNCTION: STBC 0x00744bf0 _$E342
// FUNCTION: STBC 0x00744c00 _$E341
// FUNCTION: STBC 0x00744c10 _$E127
// FUNCTION: STBC 0x00744c20 _$E126
// FUNCTION: STBC 0x00744c30 _$E346
// FUNCTION: STBC 0x00744c40 _$E345
// FUNCTION: STBC 0x00744c50 _$E350
// FUNCTION: STBC 0x00744c60 _$E349
// FUNCTION: STBC 0x00744c70 _$E354
// FUNCTION: STBC 0x00744c80 _$E353
// FUNCTION: STBC 0x00744c90 _$E133
// FUNCTION: STBC 0x00744ca0 _$E132
// FUNCTION: STBC 0x00744cb0 _$E138
// FUNCTION: STBC 0x00744cc0 _$E137
// FUNCTION: STBC 0x00744cd0 _$E168
// FUNCTION: STBC 0x00744ce0 _$E167
// FUNCTION: STBC 0x00744cf0 _$E358
// FUNCTION: STBC 0x00744d00 _$E357
// FUNCTION: STBC 0x00744d10 _$E173
// FUNCTION: STBC 0x00744d20 _$E172
// FUNCTION: STBC 0x00744d30 _$E362
// FUNCTION: STBC 0x00744d40 _$E361
// FUNCTION: STBC 0x00744d50 _$E366
// FUNCTION: STBC 0x00744d60 _$E365
// FUNCTION: STBC 0x00744d70 _$E178
// FUNCTION: STBC 0x00744d80 _$E177
// FUNCTION: STBC 0x00744d90 _$E370
// FUNCTION: STBC 0x00744da0 _$E369
// FUNCTION: STBC 0x00744db0 _$E143
// FUNCTION: STBC 0x00744dc0 _$E142
// FUNCTION: STBC 0x00744dd0 _$E374
// FUNCTION: STBC 0x00744de0 _$E373
// FUNCTION: STBC 0x00744df0 _$E378
// FUNCTION: STBC 0x00744e00 _$E377
// FUNCTION: STBC 0x00744e10 _$E183
// FUNCTION: STBC 0x00744e20 _$E182
// FUNCTION: STBC 0x00744e30 _$E188
// FUNCTION: STBC 0x00744e40 _$E187
// FUNCTION: STBC 0x00744e50 _$E382
// FUNCTION: STBC 0x00744e60 _$E381
// FUNCTION: STBC 0x00744e70 _$E386
// FUNCTION: STBC 0x00744e80 _$E385
// FUNCTION: STBC 0x00744e90 _$E390
// FUNCTION: STBC 0x00744ea0 _$E389
// FUNCTION: STBC 0x00744eb0 _$E394
// FUNCTION: STBC 0x00744ec0 _$E393
// FUNCTION: STBC 0x00744ed0 _$E398
// FUNCTION: STBC 0x00744ee0 _$E397
// FUNCTION: STBC 0x00744ef0 _$E402
// FUNCTION: STBC 0x00744f00 _$E401
// FUNCTION: STBC 0x00744f10 _$E406
// FUNCTION: STBC 0x00744f20 _$E405
// FUNCTION: STBC 0x00744f30 _$E410
// FUNCTION: STBC 0x00744f40 _$E409
// FUNCTION: STBC 0x00744f50 _$E193
// FUNCTION: STBC 0x00744f60 _$E192
// FUNCTION: STBC 0x00744f70 _$E414
// FUNCTION: STBC 0x00744f80 _$E413
// FUNCTION: STBC 0x00744f90 _$E198
// FUNCTION: STBC 0x00744fa0 _$E197
// FUNCTION: STBC 0x00744fb0 _$E203
// FUNCTION: STBC 0x00744fc0 _$E202

// The class handler table's initialiser and exit-time destructor.
// FUNCTION: STBC 0x00744fd0 _$E420
// FUNCTION: STBC 0x00744fe0 _$E416
// FUNCTION: STBC 0x00744ff0 _$E419
// FUNCTION: STBC 0x00745000 _$E417
// DATA: STBC 0x0099e8f0
TGEventHandlerTable TGFrameWindow::classHandlers;

/*
================
TGFrameWindow::TGFrameWindow

A window of the given size with a frame drawn from the icon group around it;
the sub pane is fitted to the frame's middle.
================
*/
// FUNCTION: STBC 0x00745020
TGFrameWindow::TGFrameWindow( float width, float height, char *frameName, int firstIconNum, float frameSize,
		int stretchType, TGPane *subPane ) : TGWindow( width, height, 0.0f, 0.0f, subPane ) {
	TGRect bounds( 0.0f, 0.0f, width, height );
	TGFrame *frame = new TGFrame( frameName, firstIconNum, &bounds, frameSize, stretchType );
	TGPane::InsertChild( 0x7FFFFFFF, frame, 0.0f, 0.0f, true );
	frame->Layout();
	TGRect inner = frame->GetInnerRect();
	TGUIObject *pane = TGPane::GetFirstChild();
	pane->Resize( inner.width, inner.height, true );
	pane->Move( inner.left, inner.top, true );
}

/*
================
TGFrameWindow::Create
================
*/
// FUNCTION: STBC 0x00745150
TGFrameWindow *TGFrameWindow::Create( float width, float height, char *frameName, int arg4, float frameSize, int arg6,
		TGPane *subPane ) {
	return new TGFrameWindow( width, height, frameName, arg4, frameSize, arg6, subPane );
}

/*
================
TGFrameWindow::~TGFrameWindow
================
*/
// FUNCTION: STBC 0x007451a0
TGFrameWindow::~TGFrameWindow( void ) {
}

/*
================
TGFrameWindow::GetFramePane

The frame is the window's own last child, outside the sub pane.
================
*/
// FUNCTION: STBC 0x007451b0
TGFrame *TGFrameWindow::GetFramePane( void ) {
	TGUIObject *child = TGPane::GetLastChild();
	// The type check survives in the release build with its result unused,
	// as an assertion that keeps its expression would.
	child->IsTypeOf( TGFRAME_TYPE );
	return (TGFrame *)child;
}

/*
================
TGFrameWindow::Resize

Resizes the frame by as much as the window, then the window.
================
*/
// FUNCTION: STBC 0x007451d0
void TGFrameWindow::Resize( float width, float height, bool layout ) {
	TGRect windowBounds;
	TGRect frameBounds;
	TGFrame *frame = GetFramePane();
	GetBounds( &windowBounds );
	frame->GetBounds( &frameBounds );
	float dWidth = windowBounds.width - width;
	float dHeight = windowBounds.height - height;
	frame->Resize( frameBounds.width - dWidth, frameBounds.height - dHeight, true );
	TGWindow::Resize( width, height, layout );
}

/*
================
TGFrameWindow::CreateFromTheme

A frame window in the current theme's window frame; NULL when no theme is
current.

The new window reaches the caller only through EAX: the source has no
return statement after the call. Retail passes the frame size through the
FPU, which VC6 does for an inline float accessor only when the call's
result is unused; a returned call copies it with integer moves.
================
*/
// FUNCTION: STBC 0x007452d0
TGFrameWindow *TGFrameWindow::CreateFromTheme( float width, float height, TGPane *subPane ) {
	TGUITheme *theme = g_kUIThemeManager.GetCurrentTheme();
	if ( theme == NULL ) {
		return NULL;
	}
	// BUG: the result is not returned; Create's value is left in EAX
	Create( width, height, theme->GetWindowFrameName(), 0, theme->GetWindowFrameScale(), 0, subPane );
}
