/*
===============================================================================

	BackgroundEditor.cpp

===============================================================================
*/

#include <iostream>
#include <stdio.h>
#include <string.h>

#include "BackgroundEditor.h"
#include "BackdropSphere.h"
#include "NiCamera.h"
#include "SetClass.h"
#include "SetManager.h"
#include "TGEventHandlerTable.h"
#include "TGEventManager.h"
#include "TGIntEvent.h"
#include "TGStringDialog.h"
#include "TGSystemWrapperClass.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x004966a0 _$E473
// FUNCTION: STBC 0x004966c0 _$E472

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00496040 _$E32
// FUNCTION: STBC 0x00496050 _$E29
// FUNCTION: STBC 0x00496060 _$E31
// FUNCTION: STBC 0x00496070 _$E30
// FUNCTION: STBC 0x00496080 _$E38
// FUNCTION: STBC 0x00496090 _$E35
// FUNCTION: STBC 0x004960a0 _$E37
// FUNCTION: STBC 0x004960b0 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x004960c0 _$E57
// FUNCTION: STBC 0x004960d0 _$E56
// FUNCTION: STBC 0x004960e0 _$E61
// FUNCTION: STBC 0x004960f0 _$E60
// FUNCTION: STBC 0x00496100 _$E72
// FUNCTION: STBC 0x00496110 _$E71
// FUNCTION: STBC 0x00496120 _$E76
// FUNCTION: STBC 0x00496130 _$E75
// FUNCTION: STBC 0x00496140 _$E80
// FUNCTION: STBC 0x00496150 _$E79
// FUNCTION: STBC 0x00496160 _$E84
// FUNCTION: STBC 0x00496170 _$E83
// FUNCTION: STBC 0x00496180 _$E208
// FUNCTION: STBC 0x00496190 _$E207
// FUNCTION: STBC 0x004961a0 _$E150
// FUNCTION: STBC 0x004961b0 _$E149
// FUNCTION: STBC 0x004961c0 _$E163
// FUNCTION: STBC 0x004961d0 _$E162
// FUNCTION: STBC 0x004961e0 _$E213
// FUNCTION: STBC 0x004961f0 _$E212
// FUNCTION: STBC 0x00496200 _$E218
// FUNCTION: STBC 0x00496210 _$E217
// FUNCTION: STBC 0x00496220 _$E222
// FUNCTION: STBC 0x00496230 _$E221
// FUNCTION: STBC 0x00496240 _$E226
// FUNCTION: STBC 0x00496250 _$E225
// FUNCTION: STBC 0x00496260 _$E233
// FUNCTION: STBC 0x00496270 _$E232
// FUNCTION: STBC 0x00496280 _$E292
// FUNCTION: STBC 0x00496290 _$E291
// FUNCTION: STBC 0x004962a0 _$E296
// FUNCTION: STBC 0x004962b0 _$E295
// FUNCTION: STBC 0x004962c0 _$E108
// FUNCTION: STBC 0x004962d0 _$E107
// FUNCTION: STBC 0x004962e0 _$E278
// FUNCTION: STBC 0x004962f0 _$E277
// FUNCTION: STBC 0x00496300 _$E285
// FUNCTION: STBC 0x00496310 _$E284
// FUNCTION: STBC 0x00496320 _$E308
// FUNCTION: STBC 0x00496330 _$E307
// FUNCTION: STBC 0x00496340 _$E312
// FUNCTION: STBC 0x00496350 _$E311
// FUNCTION: STBC 0x00496360 _$E352
// FUNCTION: STBC 0x00496370 _$E351
// FUNCTION: STBC 0x00496380 _$E358
// FUNCTION: STBC 0x00496390 _$E357
// FUNCTION: STBC 0x004963a0 _$E300
// FUNCTION: STBC 0x004963b0 _$E299
// FUNCTION: STBC 0x004963c0 _$E304
// FUNCTION: STBC 0x004963d0 _$E303
// FUNCTION: STBC 0x004963e0 _$E316
// FUNCTION: STBC 0x004963f0 _$E315
// FUNCTION: STBC 0x00496400 _$E320
// FUNCTION: STBC 0x00496410 _$E319
// FUNCTION: STBC 0x00496420 _$E324
// FUNCTION: STBC 0x00496430 _$E323
// FUNCTION: STBC 0x00496440 _$E328
// FUNCTION: STBC 0x00496450 _$E327
// FUNCTION: STBC 0x00496460 _$E332
// FUNCTION: STBC 0x00496470 _$E331
// FUNCTION: STBC 0x00496480 _$E336
// FUNCTION: STBC 0x00496490 _$E335
// FUNCTION: STBC 0x004964a0 _$E340
// FUNCTION: STBC 0x004964b0 _$E339
// FUNCTION: STBC 0x004964c0 _$E344
// FUNCTION: STBC 0x004964d0 _$E343
// FUNCTION: STBC 0x004964e0 _$E113
// FUNCTION: STBC 0x004964f0 _$E112
// FUNCTION: STBC 0x00496500 _$E348
// FUNCTION: STBC 0x00496510 _$E347
// FUNCTION: STBC 0x00496520 _$E118
// FUNCTION: STBC 0x00496530 _$E117
// FUNCTION: STBC 0x00496540 _$E362
// FUNCTION: STBC 0x00496550 _$E361
// FUNCTION: STBC 0x00496560 _$E366
// FUNCTION: STBC 0x00496570 _$E365
// FUNCTION: STBC 0x00496580 _$E370
// FUNCTION: STBC 0x00496590 _$E369
// FUNCTION: STBC 0x004965a0 _$E374
// FUNCTION: STBC 0x004965b0 _$E373
// FUNCTION: STBC 0x004965c0 _$E127
// FUNCTION: STBC 0x004965d0 _$E126
// FUNCTION: STBC 0x004965e0 _$E378
// FUNCTION: STBC 0x004965f0 _$E377
// FUNCTION: STBC 0x00496600 _$E382
// FUNCTION: STBC 0x00496610 _$E381
// FUNCTION: STBC 0x00496620 _$E386
// FUNCTION: STBC 0x00496630 _$E385
// FUNCTION: STBC 0x00496640 _$E133
// FUNCTION: STBC 0x00496650 _$E132
// FUNCTION: STBC 0x00496660 _$E138
// FUNCTION: STBC 0x00496670 _$E137
// FUNCTION: STBC 0x00496680 _$E168
// FUNCTION: STBC 0x00496690 _$E167
// FUNCTION: STBC 0x004966d0 _$E390
// FUNCTION: STBC 0x004966e0 _$E389
// FUNCTION: STBC 0x004966f0 _$E173
// FUNCTION: STBC 0x00496700 _$E172
// FUNCTION: STBC 0x00496710 _$E394
// FUNCTION: STBC 0x00496720 _$E393
// FUNCTION: STBC 0x00496730 _$E398
// FUNCTION: STBC 0x00496740 _$E397
// FUNCTION: STBC 0x00496750 _$E178
// FUNCTION: STBC 0x00496760 _$E177
// FUNCTION: STBC 0x00496770 _$E402
// FUNCTION: STBC 0x00496780 _$E401
// FUNCTION: STBC 0x00496790 _$E143
// FUNCTION: STBC 0x004967a0 _$E142
// FUNCTION: STBC 0x004967b0 _$E406
// FUNCTION: STBC 0x004967c0 _$E405
// FUNCTION: STBC 0x004967d0 _$E410
// FUNCTION: STBC 0x004967e0 _$E409
// FUNCTION: STBC 0x004967f0 _$E183
// FUNCTION: STBC 0x00496800 _$E182
// FUNCTION: STBC 0x00496810 _$E188
// FUNCTION: STBC 0x00496820 _$E187
// FUNCTION: STBC 0x00496830 _$E414
// FUNCTION: STBC 0x00496840 _$E413
// FUNCTION: STBC 0x00496850 _$E418
// FUNCTION: STBC 0x00496860 _$E417
// FUNCTION: STBC 0x00496870 _$E422
// FUNCTION: STBC 0x00496880 _$E421
// FUNCTION: STBC 0x00496890 _$E426
// FUNCTION: STBC 0x004968a0 _$E425
// FUNCTION: STBC 0x004968b0 _$E430
// FUNCTION: STBC 0x004968c0 _$E429
// FUNCTION: STBC 0x004968d0 _$E434
// FUNCTION: STBC 0x004968e0 _$E433
// FUNCTION: STBC 0x004968f0 _$E438
// FUNCTION: STBC 0x00496900 _$E437
// FUNCTION: STBC 0x00496910 _$E442
// FUNCTION: STBC 0x00496920 _$E441
// FUNCTION: STBC 0x00496930 _$E193
// FUNCTION: STBC 0x00496940 _$E192
// FUNCTION: STBC 0x00496950 _$E446
// FUNCTION: STBC 0x00496960 _$E445
// FUNCTION: STBC 0x00496970 _$E198
// FUNCTION: STBC 0x00496980 _$E197
// FUNCTION: STBC 0x00496990 _$E203
// FUNCTION: STBC 0x004969a0 _$E202
// FUNCTION: STBC 0x004969b0 _$E450
// FUNCTION: STBC 0x004969c0 _$E449
// FUNCTION: STBC 0x004969d0 _$E455
// FUNCTION: STBC 0x004969e0 _$E454
// FUNCTION: STBC 0x004969f0 _$E459
// FUNCTION: STBC 0x00496a00 _$E458

// The handler table's dynamic initialiser and exit-time destructor.
// FUNCTION: STBC 0x004954a0 _$E465
// FUNCTION: STBC 0x004954b0 _$E461
// FUNCTION: STBC 0x004954c0 _$E464
// FUNCTION: STBC 0x004954d0 _$E462
// DATA: STBC 0x00982fc0
TGEventHandlerTable BackgroundEditor::classHandlers;

/*
================
BackgroundEditor::BackgroundEditor

Builds the save and create dialogs, hidden.
================
*/
// FUNCTION: STBC 0x004954f0
BackgroundEditor::BackgroundEditor( void ) {
	saveDialog = TGStringDialog::CreateFromTheme( 0.3f, 0.3f, "Save Backdrop Objects", "Enter file name:", "backdrops.py", NULL, NULL );
	g_kEventManager.AddBroadcastMethodHandler( ET_DIALOG_RESULT, this, "BackgroundEditor::SaveDialogHandler", saveDialog );
	saveDialog->SetNotVisible( false );
	AddChild( saveDialog, 0.5f, 0.5f );

	createDialog = TGStringDialog::CreateFromTheme( 0.3f, 0.3f, "Create Background Object", "Enter texture name:", "data/phaser.tga", NULL, NULL );
	g_kEventManager.AddBroadcastMethodHandler( ET_DIALOG_RESULT, this, "BackgroundEditor::CreateDialogHandler", createDialog );
	createDialog->SetNotVisible( false );
	AddChild( createDialog, 0.5f, 0.5f );
}

/*
================
BackgroundEditor::Create
================
*/
// FUNCTION: STBC 0x00495620
BackgroundEditor *BackgroundEditor::Create( void ) {
	BackgroundEditor *editor = new BackgroundEditor;
	editor->CreateMenu( "Background Editor" );
	editor->CreateModes();
	return editor;
}

/*
================
BackgroundEditor::RefreshDisplay

The background editor shows nothing of its own.
================
*/
// FUNCTION: STBC 0x00495670
void BackgroundEditor::RefreshDisplay( void ) {
}

/*
================
BackgroundEditor::RebuildSphere
================
*/
// FUNCTION: STBC 0x00495680
void BackgroundEditor::RebuildSphere( BackdropSphere *sphere ) {
	sphere->Rebuild();
	sphere->SetLastUpdateTime( 0.0f );
	sphere->Update( 0.0f );
	sphere->UpdateNodeOnly();
}

/*
================
BackgroundEditor::RegisterHandlerNames
================
*/
// FUNCTION: STBC 0x004956b0
void BackgroundEditor::RegisterHandlerNames( void ) {
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&BackgroundEditor::MenuHandler, "BackgroundEditor::MenuHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&BackgroundEditor::CreateDialogHandler, "BackgroundEditor::CreateDialogHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&BackgroundEditor::SaveDialogHandler, "BackgroundEditor::SaveDialogHandler" );
}

/*
================
BackgroundEditor::RegisterHandlers

The dialog handlers are broadcast handlers added with the dialogs.
================
*/
// FUNCTION: STBC 0x004956f0
void BackgroundEditor::RegisterHandlers( void ) {
	AddMethodHandlerForClass( &classHandlers, ET_BACKGROUND_EDITOR_MENU, "BackgroundEditor::MenuHandler" );
}

/*
================
BackgroundEditor::MenuHandler

Runs the menu entry the event names; the sphere entries work on the
selected backdrop sphere, and with none selected the event goes no
further.
================
*/
// FUNCTION: STBC 0x00495710
// PARKED: block layout: retail lays out the save and create cases as the switch's fall-through with the selected-sphere default after them, this build the default first; nested switch, early returns and both case orders tried
void BackgroundEditor::MenuHandler( TGEvent *event ) {
	switch ( ( (TGIntEvent *)event )->GetInt() ) {
	case BEM_SAVE:
		ShowSaveDialog();
		break;
	case BEM_CREATE_BACKDROP:
		ShowCreateDialog();
		break;
	default: {
		BackdropSphere *sphere = (BackdropSphere *)GetSelectedObject();
		if ( sphere == NULL ) {
			return;
		}
		switch ( ( (TGIntEvent *)event )->GetInt() ) {
		case BEM_SET_ORIENTATION:
			SetOrientation( sphere );
			break;
		case BEM_MORE_POLYS:
			AddPolys( sphere );
			break;
		case BEM_FEWER_POLYS:
			RemovePolys( sphere );
			break;
		case BEM_WIDER_HORIZONTAL_SPAN:
			WidenHorizontalSpan( sphere );
			break;
		case BEM_NARROWER_HORIZONTAL_SPAN:
			NarrowHorizontalSpan( sphere );
			break;
		case BEM_WIDER_VERTICAL_SPAN:
			WidenVerticalSpan( sphere );
			break;
		case BEM_NARROWER_VERTICAL_SPAN:
			NarrowVerticalSpan( sphere );
			break;
		case BEM_MORE_TILING:
			AddTiling( sphere );
			break;
		case BEM_LESS_TILING:
			RemoveTiling( sphere );
			break;
		case BEM_MOVE_FORWARD:
			MoveForward( sphere );
			break;
		case BEM_MOVE_BACK:
			MoveBack( sphere );
			break;
		}
		break;
	}
	}
	CallNextHandler( event );
}

/*
================
BackgroundEditor::SaveDialogHandler

Writes every backdrop's script to the file the dialog names.
================
*/
// FUNCTION: STBC 0x00495890
void BackgroundEditor::SaveDialogHandler( TGEvent *event ) {
	if ( ( (TGIntEvent *)event )->GetInt() == TGDialogWindow::DFT_OK ) {
		char *fileName = ( (TGStringDialog *)event->GetSource() )->GetCString();
		FILE *file = g_kSystemWrapper.FileOpen( fileName, "w" );
		if ( file == NULL || !Backdrop::SaveAll( file ) ) {
			ShowError( "Unable to save backgrounds." );
		}
		delete [] fileName;
		if ( file != NULL ) {
			g_kSystemWrapper.FileClose( file );
		}
	}
	saveDialog->SetNotVisible( true );
	CallNextHandler( event );
}

/*
================
BackgroundEditor::CreateDialogHandler

Makes a backdrop sphere of the texture the dialog names, with the
trailing blanks cut off, and selects it.
================
*/
// FUNCTION: STBC 0x00495910
void BackgroundEditor::CreateDialogHandler( TGEvent *event ) {
	if ( ( (TGIntEvent *)event )->GetInt() == TGDialogWindow::DFT_OK ) {
		char *textureName = ( (TGStringDialog *)event->GetSource() )->GetCString();
		for ( int i = strlen( textureName ) - 1; i >= 0 && strchr( "\n\r\t ", textureName[i] ) != NULL; i-- ) {
			textureName[i] = '\0';
		}
		BackdropSphere *sphere = CreateBackdrop( textureName );
		delete [] textureName;
		if ( sphere == NULL ) {
			ShowError( "Unable to create a backdrop with that file." );
		} else {
			SelectObject( sphere );
		}
	}
	createDialog->SetNotVisible( true );
	CallNextHandler( event );
}

/*
================
BackgroundEditor::SetOrientation

Faces the sphere the way the rendered set's camera looks.
================
*/
// FUNCTION: STBC 0x004959b0
void BackgroundEditor::SetOrientation( BackdropSphere *sphere ) {
	NiCamera *camera = g_kSetManager.GetRenderedSet()->GetActiveNiCamera();
	sphere->AlignToVectors( camera->GetWorldDirection(), camera->GetWorldUpVector() );
	sphere->SetLastUpdateTime( 0.0f );
	sphere->Update( 0.0f );
}

/*
================
BackgroundEditor::AddPolys
================
*/
// FUNCTION: STBC 0x004959f0
void BackgroundEditor::AddPolys( BackdropSphere *sphere ) {
	int count = sphere->GetTargetPolyCount();
	sphere->SetTargetPolyCount( count + count / 10 );
	RebuildSphere( sphere );
}

/*
================
BackgroundEditor::RemovePolys
================
*/
// FUNCTION: STBC 0x00495a30
void BackgroundEditor::RemovePolys( BackdropSphere *sphere ) {
	int count = sphere->GetTargetPolyCount();
	sphere->SetTargetPolyCount( count - count / 10 );
	RebuildSphere( sphere );
}

/*
================
BackgroundEditor::WidenHorizontalSpan
================
*/
// FUNCTION: STBC 0x00495a70
void BackgroundEditor::WidenHorizontalSpan( BackdropSphere *sphere ) {
	sphere->SetHorizontalSpan( sphere->GetHorizontalSpan() * 1.1f );
	RebuildSphere( sphere );
}

/*
================
BackgroundEditor::NarrowHorizontalSpan
================
*/
// FUNCTION: STBC 0x00495aa0
void BackgroundEditor::NarrowHorizontalSpan( BackdropSphere *sphere ) {
	sphere->SetHorizontalSpan( sphere->GetHorizontalSpan() * 0.9f );
	RebuildSphere( sphere );
}

/*
================
BackgroundEditor::WidenVerticalSpan
================
*/
// FUNCTION: STBC 0x00495ad0
void BackgroundEditor::WidenVerticalSpan( BackdropSphere *sphere ) {
	sphere->SetVerticalSpan( sphere->GetVerticalSpan() * 1.1f );
	RebuildSphere( sphere );
}

/*
================
BackgroundEditor::NarrowVerticalSpan
================
*/
// FUNCTION: STBC 0x00495b00
void BackgroundEditor::NarrowVerticalSpan( BackdropSphere *sphere ) {
	sphere->SetVerticalSpan( sphere->GetVerticalSpan() * 0.9f );
	RebuildSphere( sphere );
}

/*
================
BackgroundEditor::AddTiling

One more tile of the texture each way.
================
*/
// FUNCTION: STBC 0x00495b30
void BackgroundEditor::AddTiling( BackdropSphere *sphere ) {
	float tile = sphere->GetTextureHTile() + 1.0f;
	sphere->SetTextureHTile( tile );
	sphere->SetTextureVTile( tile );
	RebuildSphere( sphere );
}

/*
================
BackgroundEditor::RemoveTiling

One tile fewer each way, down to one.
================
*/
// FUNCTION: STBC 0x00495b70
void BackgroundEditor::RemoveTiling( BackdropSphere *sphere ) {
	float tile = sphere->GetTextureHTile();
	if ( tile > 1.0f ) {
		tile -= 1.0f;
		sphere->SetTextureHTile( tile );
		sphere->SetTextureVTile( tile );
		RebuildSphere( sphere );
	}
}

/*
================
BackgroundEditor::MoveForward
================
*/
// FUNCTION: STBC 0x00495bc0
void BackgroundEditor::MoveForward( Backdrop *backdrop ) {
	backdrop->MoveForward();
}

/*
================
BackgroundEditor::MoveBack
================
*/
// FUNCTION: STBC 0x00495bd0
void BackgroundEditor::MoveBack( Backdrop *backdrop ) {
	backdrop->MoveBack();
}

/*
================
BackgroundEditor::ShowSaveDialog
================
*/
// FUNCTION: STBC 0x00495be0
void BackgroundEditor::ShowSaveDialog( void ) {
	SetFocus( saveDialog, false );
	saveDialog->SetVisible( true );
}

/*
================
BackgroundEditor::ShowCreateDialog
================
*/
// FUNCTION: STBC 0x00495c10
void BackgroundEditor::ShowCreateDialog( void ) {
	SetFocus( createDialog, false );
	createDialog->SetVisible( true );
}

/*
================
BackgroundEditor::CreateMenu

The editor's own entries after every editor's.
================
*/
// FUNCTION: STBC 0x00495c40
void BackgroundEditor::CreateMenu( char *title ) {
	Editor::CreateMenu( title );
	AddMenuButton( "Create Backdrop", ET_BACKGROUND_EDITOR_MENU, BEM_CREATE_BACKDROP, 1.0f );
	AddMenuButton( "Set Orientation", ET_BACKGROUND_EDITOR_MENU, BEM_SET_ORIENTATION, 1.0f );
	AddMenuButton( "+ Polys", ET_BACKGROUND_EDITOR_MENU, BEM_MORE_POLYS, 0.5f );
	AddMenuButton( "- Polys", ET_BACKGROUND_EDITOR_MENU, BEM_FEWER_POLYS, 0.5f );
	AddMenuButton( "+ Horiz Span", ET_BACKGROUND_EDITOR_MENU, BEM_WIDER_HORIZONTAL_SPAN, 0.5f );
	AddMenuButton( "- Horiz Span", ET_BACKGROUND_EDITOR_MENU, BEM_NARROWER_HORIZONTAL_SPAN, 0.5f );
	AddMenuButton( "+ Vert Span", ET_BACKGROUND_EDITOR_MENU, BEM_WIDER_VERTICAL_SPAN, 0.5f );
	AddMenuButton( "- Vert Span", ET_BACKGROUND_EDITOR_MENU, BEM_NARROWER_VERTICAL_SPAN, 0.5f );
	AddMenuButton( "+ Tiling", ET_BACKGROUND_EDITOR_MENU, BEM_MORE_TILING, 0.5f );
	AddMenuButton( "- Tiling", ET_BACKGROUND_EDITOR_MENU, BEM_LESS_TILING, 0.5f );
	AddMenuButton( "Move Back", ET_BACKGROUND_EDITOR_MENU, BEM_MOVE_BACK, 0.5f );
	AddMenuButton( "Move Frwd", ET_BACKGROUND_EDITOR_MENU, BEM_MOVE_FORWARD, 0.5f );
}

/*
================
BackgroundEditor::CreateModes
================
*/
// FUNCTION: STBC 0x00495d80
void BackgroundEditor::CreateModes( void ) {
	SetMode( new BackgroundEditorMode( this ) );
}

/*
================
BackgroundEditor::CreateBackdrop

Names the sphere "Backdrop <texture>" after the texture's file name
without its directories and extension, numbering it from 2 when the name
is taken.
================
*/
// FUNCTION: STBC 0x00495e90
BackdropSphere *BackgroundEditor::CreateBackdrop( char *textureName ) {
	FILE *file = g_kSystemWrapper.FileOpen( textureName, "r" );
	if ( file == NULL ) {
		return NULL;
	}
	g_kSystemWrapper.FileClose( file );

	char *baseName = new char[strlen( textureName ) + 1];
	char *slash = strrchr( textureName, '/' );
	if ( slash == NULL ) {
		strcpy( baseName, textureName );
	} else {
		strcpy( baseName, slash + 1 );
	}
	char *dot = strrchr( baseName, '.' );
	if ( dot != NULL ) {
		*dot = '\0';
	}

	char name[64];
	sprintf( name, "Backdrop %s", baseName );
	if ( Backdrop::GetByName( name ) != NULL ) {
		int number = 2;
		do {
			sprintf( name, "Backdrop %s %d", baseName, number );
			number++;
		} while ( Backdrop::GetByName( name ) != NULL );
	}

	BackdropSphere *sphere = new BackdropSphere;
	sphere->SetName( name );
	sphere->SetTextureFileName( textureName );
	sphere->Rebuild();
	SetClass *set = g_kSetManager.GetRenderedSet();
	NiCamera *camera = set->GetActiveNiCamera();
	sphere->AlignToVectors( camera->GetWorldDirection(), camera->GetWorldUpVector() );
	set->AddBackdropToSet( sphere, name );
	sphere->SetLastUpdateTime( 0.0f );
	sphere->Update( 0.0f );
	delete [] baseName;
	return sphere;
}

/*
================
BackgroundEditorMode::CanSelect

Only backdrop spheres.
================
*/
// FUNCTION: STBC 0x00496020
bool BackgroundEditorMode::CanSelect( ObjectClass *object ) {
	return object->IsTypeOf( BACKDROPSPHERE_TYPE );
}
