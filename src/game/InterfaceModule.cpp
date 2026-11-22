/*
===============================================================================

	InterfaceModule.cpp

===============================================================================
*/

#include <iostream>
#include "InterfaceModule.h"
#include "MainWindow.h"
#include "PyEmbed.h"
#include "EngPowerCtrl.h"
#include "STButton.h"
#include "STFillGauge.h"
#include "STMenu.h"
#include "STNumericBar.h"
#include "STMissionLog.h"
#include "STRoundedButton.h"
#include "STStylizedWindow.h"
#include "TacticalControlWindow.h"
#include "TGButton.h"
#include "TGEventManager.h"
#include "TGIcon.h"
#include "TGPane.h"
#include "TGParagraph.h"
#include "TGRootPane.h"
#include "TGSoundManager.h"
#include "TopWindow.h"
#include "UtopiaApp.h"
#include "DamageDisplay.h"
#include "DamageIcon.h"
#include "Editor.h"
#include "EngPowerDisplay.h"
#include "EngRepairPane.h"
#include "GraphicsMenu.h"
#include "KeyboardBinding.h"
#include "RadarDisplay.h"
#include "RadarScope.h"
#include "ReticleManagerWindow.h"
#include "ReticleWindow.h"
#include "STCharacterMenu.h"
#include "STComponentMenu.h"
#include "STFileDialog.h"
#include "STFileMenu.h"
#include "STLoadDialog.h"
#include "STRepairButton.h"
#include "STSaveDialog.h"
#include "STSubPane.h"
#include "STSubsystemMenu.h"
#include "STTargetMenu.h"
#include "STToggle.h"
#include "STTopLevelMenu.h"
#include "STWarpButton.h"
#include "ShieldsDisplay.h"
#include "ShipDisplay.h"
#include "SortedRegionMenu.h"
#include "TacWeaponsCtrl.h"
#include "TGBoolEvent.h"
#include "TGEventHandlerTable.h"
#include "TGSound.h"
#include "WeaponsDisplay.h"
#include "CharacterClass.h"
#include "SubtitleWindow.h"
#include "TGFontGroup.h"
#include "TGFontManager.h"
#include "TGIconManager.h"
#include "STComponentMenuItem.h"
#include "STCheckbox.h"
#include "STTiledIcon.h"
#include "BridgeWindow.h"
#include "TacticalWindow.h"
#include "ConsoleWindow.h"
#include "OptionsWindow.h"
#include "CinematicWindow.h"
#include "CDCheckWindow.h"
#include "ModalDialogWindow.h"
#include "RadarBlip.h"
#include "MultiplayerWindow.h"
#include "UtopiaModule.h"
#include "TGObject.h"
#include "TGButtonBase.h"
#include "TGWindow.h"
#include "MapWindow.h"
#include "SubtitleAction.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00555380 _$E32
// FUNCTION: STBC 0x00555390 _$E29
// FUNCTION: STBC 0x005553a0 _$E31
// FUNCTION: STBC 0x005553b0 _$E30
// FUNCTION: STBC 0x005553c0 _$E38
// FUNCTION: STBC 0x005553d0 _$E35
// FUNCTION: STBC 0x005553e0 _$E37
// FUNCTION: STBC 0x005553f0 _$E36

// The global object's dynamic initialiser and exit-time destructor.
// FUNCTION: STBC 0x00555900 _$E606
// FUNCTION: STBC 0x00555910 _$E603
// FUNCTION: STBC 0x00555920 _$E605
// FUNCTION: STBC 0x00555930 _$E604

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00555400 _$E63
// FUNCTION: STBC 0x00555410 _$E62
// FUNCTION: STBC 0x00555420 _$E67
// FUNCTION: STBC 0x00555430 _$E66
// FUNCTION: STBC 0x00555440 _$E78
// FUNCTION: STBC 0x00555450 _$E77
// FUNCTION: STBC 0x00555460 _$E82
// FUNCTION: STBC 0x00555470 _$E81
// FUNCTION: STBC 0x00555480 _$E86
// FUNCTION: STBC 0x00555490 _$E85
// FUNCTION: STBC 0x005554a0 _$E90
// FUNCTION: STBC 0x005554b0 _$E89
// FUNCTION: STBC 0x005554c0 _$E210
// FUNCTION: STBC 0x005554d0 _$E209
// FUNCTION: STBC 0x005554e0 _$E152
// FUNCTION: STBC 0x005554f0 _$E151
// FUNCTION: STBC 0x00555500 _$E165
// FUNCTION: STBC 0x00555510 _$E164
// FUNCTION: STBC 0x00555520 _$E215
// FUNCTION: STBC 0x00555530 _$E214
// FUNCTION: STBC 0x00555540 _$E220
// FUNCTION: STBC 0x00555550 _$E219
// FUNCTION: STBC 0x00555560 _$E224
// FUNCTION: STBC 0x00555570 _$E223
// FUNCTION: STBC 0x00555580 _$E228
// FUNCTION: STBC 0x00555590 _$E227
// FUNCTION: STBC 0x005555a0 _$E235
// FUNCTION: STBC 0x005555b0 _$E234
// FUNCTION: STBC 0x005555c0 _$E438
// FUNCTION: STBC 0x005555d0 _$E437
// FUNCTION: STBC 0x005555e0 _$E442
// FUNCTION: STBC 0x005555f0 _$E441
// FUNCTION: STBC 0x00555600 _$E114
// FUNCTION: STBC 0x00555610 _$E113
// FUNCTION: STBC 0x00555620 _$E424
// FUNCTION: STBC 0x00555630 _$E423
// FUNCTION: STBC 0x00555640 _$E431
// FUNCTION: STBC 0x00555650 _$E430
// FUNCTION: STBC 0x00555660 _$E454
// FUNCTION: STBC 0x00555670 _$E453
// FUNCTION: STBC 0x00555680 _$E458
// FUNCTION: STBC 0x00555690 _$E457
// FUNCTION: STBC 0x005556a0 _$E498
// FUNCTION: STBC 0x005556b0 _$E497
// FUNCTION: STBC 0x005556c0 _$E504
// FUNCTION: STBC 0x005556d0 _$E503
// FUNCTION: STBC 0x005556e0 _$E446
// FUNCTION: STBC 0x005556f0 _$E445
// FUNCTION: STBC 0x00555700 _$E450
// FUNCTION: STBC 0x00555710 _$E449
// FUNCTION: STBC 0x00555720 _$E462
// FUNCTION: STBC 0x00555730 _$E461
// FUNCTION: STBC 0x00555740 _$E466
// FUNCTION: STBC 0x00555750 _$E465
// FUNCTION: STBC 0x00555760 _$E470
// FUNCTION: STBC 0x00555770 _$E469
// FUNCTION: STBC 0x00555780 _$E474
// FUNCTION: STBC 0x00555790 _$E473
// FUNCTION: STBC 0x005557a0 _$E478
// FUNCTION: STBC 0x005557b0 _$E477
// FUNCTION: STBC 0x005557c0 _$E482
// FUNCTION: STBC 0x005557d0 _$E481
// FUNCTION: STBC 0x005557e0 _$E486
// FUNCTION: STBC 0x005557f0 _$E485
// FUNCTION: STBC 0x00555800 _$E490
// FUNCTION: STBC 0x00555810 _$E489
// FUNCTION: STBC 0x00555820 _$E119
// FUNCTION: STBC 0x00555830 _$E118
// FUNCTION: STBC 0x00555840 _$E494
// FUNCTION: STBC 0x00555850 _$E493
// FUNCTION: STBC 0x00555860 _$E124
// FUNCTION: STBC 0x00555870 _$E123
// FUNCTION: STBC 0x00555880 _$E508
// FUNCTION: STBC 0x00555890 _$E507
// FUNCTION: STBC 0x005558a0 _$E512
// FUNCTION: STBC 0x005558b0 _$E511
// FUNCTION: STBC 0x005558c0 _$E516
// FUNCTION: STBC 0x005558d0 _$E515
// FUNCTION: STBC 0x005558e0 _$E520
// FUNCTION: STBC 0x005558f0 _$E519
// FUNCTION: STBC 0x00555ac0 _$E129
// FUNCTION: STBC 0x00555ad0 _$E128
// FUNCTION: STBC 0x00555e60 _$E524
// FUNCTION: STBC 0x00555e70 _$E523
// FUNCTION: STBC 0x00555e80 _$E528
// FUNCTION: STBC 0x00555e90 _$E527
// FUNCTION: STBC 0x00556030 _$E532
// FUNCTION: STBC 0x00556040 _$E531
// FUNCTION: STBC 0x00556270 _$E135
// FUNCTION: STBC 0x00556280 _$E134
// FUNCTION: STBC 0x005565f0 _$E140
// FUNCTION: STBC 0x00556600 _$E139
// FUNCTION: STBC 0x00559110 _$E170
// FUNCTION: STBC 0x00559120 _$E169
// FUNCTION: STBC 0x005593d0 _$E536
// FUNCTION: STBC 0x005593e0 _$E535
// FUNCTION: STBC 0x005593f0 _$E175
// FUNCTION: STBC 0x00559400 _$E174
// FUNCTION: STBC 0x00559410 _$E540
// FUNCTION: STBC 0x00559420 _$E539
// FUNCTION: STBC 0x00559430 _$E544
// FUNCTION: STBC 0x00559440 _$E543
// FUNCTION: STBC 0x00559450 _$E180
// FUNCTION: STBC 0x00559460 _$E179
// FUNCTION: STBC 0x00559470 _$E548
// FUNCTION: STBC 0x00559480 _$E547
// FUNCTION: STBC 0x00559490 _$E145
// FUNCTION: STBC 0x005594a0 _$E144
// FUNCTION: STBC 0x005594b0 _$E552
// FUNCTION: STBC 0x005594c0 _$E551
// FUNCTION: STBC 0x005594d0 _$E556
// FUNCTION: STBC 0x005594e0 _$E555
// FUNCTION: STBC 0x005594f0 _$E185
// FUNCTION: STBC 0x00559500 _$E184
// FUNCTION: STBC 0x00559510 _$E190
// FUNCTION: STBC 0x00559520 _$E189
// FUNCTION: STBC 0x00559530 _$E560
// FUNCTION: STBC 0x00559540 _$E559
// FUNCTION: STBC 0x00559550 _$E564
// FUNCTION: STBC 0x00559560 _$E563
// FUNCTION: STBC 0x00559570 _$E568
// FUNCTION: STBC 0x00559580 _$E567
// FUNCTION: STBC 0x00559590 _$E572
// FUNCTION: STBC 0x005595a0 _$E571
// FUNCTION: STBC 0x005595b0 _$E576
// FUNCTION: STBC 0x005595c0 _$E575
// FUNCTION: STBC 0x005595d0 _$E580
// FUNCTION: STBC 0x005595e0 _$E579
// FUNCTION: STBC 0x005595f0 _$E584
// FUNCTION: STBC 0x00559600 _$E583
// FUNCTION: STBC 0x00559610 _$E588
// FUNCTION: STBC 0x00559620 _$E587
// FUNCTION: STBC 0x00559630 _$E195
// FUNCTION: STBC 0x00559640 _$E194
// FUNCTION: STBC 0x00559650 _$E592
// FUNCTION: STBC 0x00559660 _$E591
// FUNCTION: STBC 0x00559670 _$E200
// FUNCTION: STBC 0x00559680 _$E199
// FUNCTION: STBC 0x00559690 _$E205
// FUNCTION: STBC 0x005596a0 _$E204
// FUNCTION: STBC 0x00559790 _$E596
// FUNCTION: STBC 0x005597a0 _$E595
// FUNCTION: STBC 0x005597b0 _$E601
// FUNCTION: STBC 0x005597c0 _$E600
// FUNCTION: STBC 0x005597d0 _$E359
// FUNCTION: STBC 0x005597e0 _$E358

// The guard of std::ctype<unsigned short>::id and its exit-time thunk (<iostream>).
// FUNCTION: STBC 0x005597f0 _$E615
// FUNCTION: STBC 0x00559810 _$E614

// The module's priority among the TGModules (invented name).
#define INTERFACE_MODULE_PRIORITY	0x20000000

// DATA: STBC 0x0098b320
InterfaceModule g_kInterfaceModule;

/*
================
InterfaceModule::InterfaceModule
================
*/
// FUNCTION: STBC 0x00555950
InterfaceModule::InterfaceModule( void ) : TGModule( INTERFACE_MODULE_PRIORITY ) {
	muted = false;
	playClickSound = false;
}

/*
================
InterfaceModule::VirtualSlot00

The startup: the handler names and the classes, the LoadInterface script's
sounds, then the default key bindings and the player's over them.
================
*/
// FUNCTION: STBC 0x00555970
void InterfaceModule::VirtualSlot00( void ) {
	RegisterHandlerNames();
	RegisterClasses();
	PyEmbed::CallFunction( "LoadInterface", "LoadSounds", "", NULL, "()" );
	PyErr_Print();
	g_kKeyboardBinding.RebuildMappingFromFile( "DefaultKeyboardBinding" );
	g_kKeyboardBinding.RebuildMappingFromFile( NULL );
}

/*
================
InterfaceModule::VirtualSlot01

The class handlers, then the module's broadcast handlers, which slot 4
removes.
================
*/
// FUNCTION: STBC 0x005559c0
void InterfaceModule::VirtualSlot01( void ) {
	RegisterHandlers();
	muted = false;
	playClickSound = false;
	g_kEventManager.AddBroadcastFuncHandler( ET_UI_SWITCH_ICON_GROUPS, NULL, "InterfaceModule::HandleUISwitchIconGroups" );
	g_kEventManager.AddBroadcastFuncHandler( ET_UI_RESIZE, NULL, "InterfaceModule::HandleUIResize" );
	g_kEventManager.AddBroadcastFuncHandler( ET_UI_REPOSITION, NULL, "InterfaceModule::HandleUIReposition" );
}

/*
================
InterfaceModule::VirtualSlot04

Removes the interface's broadcast handlers.
================
*/
// FUNCTION: STBC 0x00555a40
void InterfaceModule::VirtualSlot04( void ) {
	g_kEventManager.RemoveBroadcastHandler( ET_UI_SWITCH_ICON_GROUPS, NULL, "InterfaceModule::HandleUISwitchIconGroups" );
	g_kEventManager.RemoveBroadcastHandler( ET_UI_RESIZE, NULL, "InterfaceModule::HandleUIResize" );
	g_kEventManager.RemoveBroadcastHandler( ET_UI_REPOSITION, NULL, "InterfaceModule::HandleUIReposition" );
}

/*
================
InterfaceModule::VirtualSlot05

The LoadInterface script unloads its sounds.
================
*/
// FUNCTION: STBC 0x00555ae0
void InterfaceModule::VirtualSlot05( void ) {
	PyEmbed::CallFunction( "LoadInterface", "UnloadSounds", "", NULL, "()" );
	PyErr_Print();
}

/*
================
InterfaceModule::VirtualSlot02

Updates every fill gauge, refreshes the tactical control window while it
is visible, and plays "UIBeep" once when playClickSound is set and muted is
not, then clears playClickSound.
================
*/
// FUNCTION: STBC 0x00555B10
void InterfaceModule::VirtualSlot02( void ) {
	NiTListIterator pos = STFillGauge::gauges.GetHeadPos();
	while ( pos != NULL ) {
		STFillGauge *gauge = STFillGauge::gauges.GetNext( pos );
		if ( gauge != NULL ) {
			gauge->UpdateGauge();
		}
	}
	TacticalControlWindow *tacticalControlWindow = TacticalControlWindow::GetTacticalControlWindow();
	if ( tacticalControlWindow != NULL && tacticalControlWindow->IsVisible() ) {
		tacticalControlWindow->VirtualSlot83();
	}
	if ( !muted && playClickSound ) {
		g_kSoundManager.StopSound( "UIBeep" );
		g_kSoundManager.PlaySound( "UIBeep" );
		playClickSound = false;
	}
}

/*
================
InterfaceModule::OverrideHandler

Does nothing: a handler that keeps an event from the handlers after it.
================
*/
// FUNCTION: STBC 0x00555b80
void InterfaceModule::OverrideHandler( TGEventHandlerObject *object, TGEvent *event ) {
}

/*
================
InterfaceModule::DoTheRightThing

Shows or hides the mouse cursor for what is on the screen: it shows over
the options menu, the tactical controls, a dialog, the mission log, the
editor, a cinematic window with a stylized window, a multiplayer pane
or a paused game away from the main windows, and hides on the bridge,
the map, and in a cutscene without the options menu.
================
*/
// FUNCTION: STBC 0x00555b90
// PARKED: complete; same calls and branches, retail keeps the cursor in EDI, the top window in EBX and the options window in EBP where this build rotates the three (declaration order tried)
void InterfaceModule::DoTheRightThing( void ) {
	TGIcon *cursor = g_kRootWindow.GetMouseCursor();
	if ( cursor == NULL ) {
		return;
	}
	TopWindow *topWindow = TopWindow::GetTopWindow();
	if ( topWindow == NULL ) {
		cursor->SetNotVisible();
		return;
	}
	if ( !topWindow->IsVisible() ) {
		cursor->SetNotVisible();
		return;
	}
	MainWindow *options = topWindow->FindMainWindow( MWT_OPTIONS );
	if ( options->IsVisible() ) {
		cursor->SetVisible();
		return;
	}
	TacticalControlWindow *tacticalControlWindow = TacticalControlWindow::GetTacticalControlWindow();
	if ( tacticalControlWindow != NULL && tacticalControlWindow->IsCompletelyVisible() == true ) {
		cursor->SetVisible();
		return;
	}

	MainWindow *dialog = topWindow->FindMainWindow( MWT_MODAL_DIALOG );
	if ( dialog == NULL || !dialog->IsWindowActive() ) {
		STMissionLog *missionLog = STMissionLog::GetMissionLog();
		if ( missionLog == NULL || missionLog->IsCompletelyVisible() != true ) {
			MainWindow *editor = topWindow->FindMainWindow( MWT_EDITOR );
			if ( editor != NULL && editor->IsWindowActive() == true ) {
				cursor->SetVisible();
				return;
			}
			MainWindow *cinematic = topWindow->FindMainWindow( MWT_CINEMATIC );
			if ( cinematic != NULL && cinematic->IsWindowActive() == true ) {
				if ( cinematic->GetNumChildren() > 0 ) {
					TGPane *pane = TGPane::Cast( cinematic->GetFirstChild() );
					if ( pane != NULL && STStylizedWindow::Cast( pane->GetFirstChild() ) != NULL ) {
						cursor->SetVisible();
						return;
					}
				}
				cursor->SetNotVisible();
				return;
			}
			MainWindow *multiplayer = topWindow->FindMainWindow( MWT_MULTIPLAYER );
			if ( multiplayer != NULL && multiplayer->IsVisible() ) {
				int count = multiplayer->GetNumChildren();
				for ( int i = 0; i < count; i++ ) {
					TGUIObject *child = multiplayer->GetNthChild( i );
					if ( child != NULL && child->IsVisible() ) {
						cursor->SetVisible();
						return;
					}
				}
			}
			if ( topWindow->IsCutsceneMode() ) {
				if ( options != NULL && options->IsCompletelyVisible() == true ) {
					cursor->SetVisible();
					return;
				}
				cursor->SetNotVisible();
				return;
			}
			MainWindow *bridge = topWindow->FindMainWindow( MWT_BRIDGE );
			if ( bridge != NULL && bridge->IsWindowActive() == true ) {
				cursor->SetNotVisible();
				return;
			}
			if ( options != NULL && UtopiaApp::GetApp()->IsGamePaused() && !options->IsCompletelyVisible() ) {
				TGUIObject *focus = topWindow->GetFocus();
				if ( focus == NULL || focus->IsTypeOf( MAINWINDOW_TYPE ) ) {
					cursor->SetNotVisible();
					return;
				}
				cursor->SetVisible();
				return;
			}
			MainWindow *map = topWindow->FindMainWindow( MWT_TACTICAL_MAP );
			if ( map != NULL && map->IsWindowActive() == true ) {
				cursor->SetNotVisible();
				return;
			}
		}
	}
	cursor->SetVisible();
}

/*
================
InterfaceModule::HandleUISwitchIconGroups

After the icon groups changed with the resolution: the icon manager's
pixel adjustment is set again, the menus' arrow offset and the repair
buttons' font size are read again, and every paragraph, stylized window,
numeric bar and repair button takes the new groups.
================
*/
// FUNCTION: STBC 0x00555ea0
void InterfaceModule::HandleUISwitchIconGroups( TGEventHandlerObject *object, TGEvent *event ) {
	g_kIconManager.SetPixelCenterAdj( g_kIconManager.GetPixelCenterAdj() );
	STMenu::GetArrowOffsetX( true );
	STRepairButton::LoadFontSize();

	NiTMap<long, TGObject *> *objects = TGObject::objectTable;
	NiTMapIterator pos = objects->GetFirstPos();
	while ( pos != NULL ) {
		long id;
		TGObject *candidate;
		objects->GetNext( pos, id, candidate );
		TGParagraph *paragraph = TGParagraph::Cast( candidate );
		if ( paragraph != NULL ) {
			paragraph->RecalcBounds( false );
		}
		STStylizedWindow *window = STStylizedWindow::Cast( candidate );
		if ( window != NULL ) {
			window->UISwitchIconGroups();
		}
		STNumericBar *bar = STNumericBar::Cast( candidate );
		if ( bar != NULL ) {
			bar->RefreshIconGroups();
		}
		STRepairButton *button = STRepairButton::Cast( candidate );
		if ( button != NULL && button->GetText() != NULL ) {
			char *fontName = g_kFontManager.GetDefaultFont()->GetFontName();
			button->GetText()->SetFontGroup( g_kFontManager.GetFontGroup( fontName, STRepairButton::fontSize ), true );
		}
	}
}

/*
================
InterfaceModule::HandleUIResize

After a change of resolution: every interface object is moved to the
origin and sized again, the menus and buttons and stylized windows lay
themselves out, then every subpane fits its contents, the subtitles are
sized, and the cursor is set again in the group of its size.
================
*/
// FUNCTION: STBC 0x00556050
void InterfaceModule::HandleUIResize( TGEventHandlerObject *object, TGEvent *event ) {
	NiTMap<long, TGObject *> *objects = TGObject::objectTable;
	NiTMapIterator pos = objects->GetFirstPos();
	while ( pos != NULL ) {
		long id;
		TGObject *candidate;
		objects->GetNext( pos, id, candidate );
		if ( candidate->IsTypeOf( TGUIOBJECT_TYPE ) && candidate != &g_kRootWindow ) {
			TGUIObject *uiObject = (TGUIObject *)candidate;
			uiObject->Move( 0.0f, 0.0f, false );
			uiObject->Resize( uiObject->GetWidth(), uiObject->GetHeight(), true );
		}
		STMenu *menu = STMenu::Cast( candidate );
		if ( menu != NULL ) {
			menu->VirtualSlot60();
			menu->ForceUpdate( true );
		}
		STButton *button = STButton::Cast( candidate );
		if ( button != NULL ) {
			button->VirtualSlot60();
			button->Layout();
		}
		STRoundedButton *roundedButton = STRoundedButton::Cast( candidate );
		if ( roundedButton != NULL ) {
			roundedButton->VirtualSlot60();
			roundedButton->Layout();
		}
		STStylizedWindow *window = STStylizedWindow::Cast( candidate );
		if ( window != NULL ) {
			window->UIResize();
			window->Layout();
		}
	}

	pos = objects->GetFirstPos();
	while ( pos != NULL ) {
		long id;
		TGObject *candidate;
		objects->GetNext( pos, id, candidate );
		STSubPane *subPane = STSubPane::Cast( candidate );
		if ( subPane != NULL ) {
			subPane->ResizeToContents( true );
		}
	}

	TopWindow *topWindow = TopWindow::GetTopWindow();
	if ( topWindow != NULL ) {
		SubtitleWindow *subtitles = (SubtitleWindow *)topWindow->FindMainWindow( MWT_SUBTITLE );
		if ( subtitles != NULL ) {
			subtitles->ResizeUI();
		}
	}

	// The cursor's icon number says which group it belongs to.
	char *function;
	int icon = g_kRootWindow.GetMouseCursor()->GetIconNum();
	if ( icon < 10 ) {
		function = "SetTrekCursor";
	} else if ( icon < 20 ) {
		function = "SetTargetCursor";
	} else {
		function = "SetCircleCursor";
	}
	PyEmbed::CallFunction( "SystemIcons", function, NULL, NULL, NULL );
}

/*
================
InterfaceModule::HandleUIReposition

After a change of resolution: the characters' status boxes are refreshed
and every subpane takes the default spacing and fits its contents, then
every stylized window is placed and sized again, and the subtitles are
placed.
================
*/
// FUNCTION: STBC 0x00556290
void InterfaceModule::HandleUIReposition( TGEventHandlerObject *object, TGEvent *event ) {
	NiTMap<long, TGObject *> *objects = TGObject::objectTable;
	NiTMapIterator pos = objects->GetFirstPos();
	while ( pos != NULL ) {
		long id;
		TGObject *candidate;
		objects->GetNext( pos, id, candidate );
		CharacterClass *character = CharacterClass::Cast( candidate );
		if ( character != NULL ) {
			character->RefreshStatusBox();
		}
		STSubPane *subPane = STSubPane::Cast( candidate );
		if ( subPane != NULL ) {
			subPane->SetVertSpacing( DEFAULT_ST_INDENT_VERT );
			subPane->Layout();
			subPane->ResizeToContents( true );
		}
	}

	pos = objects->GetFirstPos();
	while ( pos != NULL ) {
		long id;
		TGObject *candidate;
		objects->GetNext( pos, id, candidate );
		STStylizedWindow *window = STStylizedWindow::Cast( candidate );
		if ( window != NULL ) {
			window->UIReposition();
			window->Unknown5314F0();
			window->InteriorChangedSize( false );
			window->Layout();
		}
	}

	TopWindow *topWindow = TopWindow::GetTopWindow();
	if ( topWindow != NULL ) {
		SubtitleWindow *subtitles = (SubtitleWindow *)topWindow->FindMainWindow( MWT_SUBTITLE );
		if ( subtitles != NULL ) {
			subtitles->RepositionUI();
		}
	}
}

/*
================
InterfaceModule::SwitchFonts

Every paragraph drawn in one font group is drawn in the other.
================
*/
// FUNCTION: STBC 0x005563f0
void InterfaceModule::SwitchFonts( TGFontGroup *oldGroup, TGFontGroup *newGroup ) {
	NiTMap<long, TGObject *> *objects = TGObject::objectTable;
	NiTMapIterator pos = objects->GetFirstPos();
	while ( pos != NULL ) {
		long id;
		TGObject *object;
		objects->GetNext( pos, id, object );
		TGParagraph *paragraph = TGParagraph::Cast( object );
		if ( paragraph != NULL && paragraph->GetFontGroup() == oldGroup ) {
			paragraph->SetFontGroup( newGroup, true );
		}
	}
}

/*
================
InterfaceModule::TGParagraphSoundHandler

A key in a paragraph clicks, unless the click is already playing.
================
*/
// FUNCTION: STBC 0x00556490
void InterfaceModule::TGParagraphSoundHandler( TGEventHandlerObject *object, TGEvent *event ) {
	TGSound *sound = g_kSoundManager.GetSound( "UITextEntryClick" );
	if ( sound != NULL && g_kSoundManager.GetPlayingSound( "UITextEntryClick" ) == NULL ) {
		sound->Play( true );
	}
}

/*
================
InterfaceModule::STStylizedWindowMinimizeHandler

A stylized window about to be minimized or restored plays the sound of
the change, then the window handles the event.
================
*/
// FUNCTION: STBC 0x005564d0
void InterfaceModule::STStylizedWindowMinimizeHandler( TGEventHandlerObject *object, TGEvent *event ) {
	STStylizedWindow *window = (STStylizedWindow *)object;
	TGBoolEvent *minimize = (TGBoolEvent *)event;
	TGSound *sound;
	if ( minimize->GetBool() && !window->IsMinimized() ) {
		sound = g_kSoundManager.GetSound( "UIMinimize" );
		if ( sound != NULL && g_kSoundManager.GetPlayingSound( "UIMinimize" ) == NULL ) {
			sound->Play( true );
		}
	} else if ( !minimize->GetBool() && window->IsMinimized() ) {
		sound = g_kSoundManager.GetSound( "UIUnminimize" );
		if ( sound != NULL && g_kSoundManager.GetPlayingSound( "UIUnminimize" ) == NULL ) {
			sound->Play( true );
		}
	}
	window->CallNextHandler( event );
}

/*
================
InterfaceModule::IsUIElement

An object with every flag given that the keyboard can work: a menu, a
button, a numeric bar or an engineering power control.
================
*/
// FUNCTION: STBC 0x00556570
bool InterfaceModule::IsUIElement( TGUIObject *object, int flags ) {
	if ( object != NULL && ( object->GetFlags() & flags ) == flags ) {
		if ( object->IsTypeOf( STMENU_TYPE ) || object->IsTypeOf( STROUNDEDBUTTON_TYPE ) || object->IsTypeOf( STBUTTON_TYPE ) || object->IsTypeOf( TGBUTTON_TYPE ) || object->IsTypeOf( STNUMERICBAR_TYPE ) || object->IsTypeOf( ENGPOWERCTRL_TYPE ) ) {
			return true;
		}
	}
	return false;
}

/*
================
InterfaceModule::HasUIElement

The object is one, or a pane holds one at any depth.
================
*/
// FUNCTION: STBC 0x00556610
bool InterfaceModule::HasUIElement( TGUIObject *object, int flags ) {
	bool found = false;
	if ( IsUIElement( object, flags ) == true ) {
		return true;
	}
	TGPane *pane = TGPane::Cast( object );
	if ( pane != NULL ) {
		for ( TGUIObject *child = pane->GetFirstChild(); child != NULL; child = pane->GetNextChild( child ) ) {
			found = HasUIElement( child, flags );
			if ( found == true ) {
				break;
			}
		}
	}
	return found;
}

/*
================
InterfaceModule::GetFirstUIElement

The pane's first child that is one; NULL if none.
================
*/
// FUNCTION: STBC 0x00556680
TGUIObject *InterfaceModule::GetFirstUIElement( TGPane *pane, int flags ) {
	if ( pane != NULL ) {
		for ( TGUIObject *child = pane->GetFirstChild(); child != NULL; child = pane->GetNextChild( child ) ) {
			if ( IsUIElement( child, flags ) == true ) {
				return child;
			}
		}
	}
	return NULL;
}

/*
================
InterfaceModule::GetLastUIElement

The pane's last child that is one; NULL if none.
================
*/
// FUNCTION: STBC 0x005566d0
TGUIObject *InterfaceModule::GetLastUIElement( TGPane *pane, int flags ) {
	if ( pane != NULL ) {
		for ( TGUIObject *child = pane->GetLastChild(); child != NULL; child = pane->GetPrevChild( child ) ) {
			if ( IsUIElement( child, flags ) == true ) {
				return child;
			}
		}
	}
	return NULL;
}

/*
================
InterfaceModule::GetPrevUIElement

The nearest child before the focus that holds one; NULL if none.
================
*/
// FUNCTION: STBC 0x00556720
TGUIObject *InterfaceModule::GetPrevUIElement( TGPane *pane, int flags ) {
	if ( pane != NULL ) {
		TGUIObject *focus = pane->GetFocus();
		for ( TGUIObject *child = pane->GetPrevChild( focus ); child != NULL; child = pane->GetPrevChild( child ) ) {
			if ( HasUIElement( child, flags ) == true ) {
				return child;
			}
		}
	}
	return NULL;
}

/*
================
InterfaceModule::GetNextUIElement

The nearest child after the focus that holds one; NULL if none.
================
*/
// FUNCTION: STBC 0x00556780
TGUIObject *InterfaceModule::GetNextUIElement( TGPane *pane, int flags ) {
	if ( pane != NULL ) {
		TGUIObject *focus = pane->GetFocus();
		for ( TGUIObject *child = pane->GetNextChild( focus ); child != NULL; child = pane->GetNextChild( child ) ) {
			if ( HasUIElement( child, flags ) == true ) {
				return child;
			}
		}
	}
	return NULL;
}

/*
================
InterfaceModule::ForceFocusOnObject

Each pane up the chain gives the focus to the one below it.
================
*/
// FUNCTION: STBC 0x005567e0
void InterfaceModule::ForceFocusOnObject( TGUIObject *object ) {
	for ( TGPane *parent = object->GetParent(); parent != NULL && parent != UI_NO_PARENT; parent = parent->GetParent() ) {
		if ( parent->TGPane::GetFocus() != object ) {
			parent->TGPane::SetFocus( object );
		}
		object = parent;
	}
}

/*
================
InterfaceModule::RegisterClasses

Each interface class's creator; the handler tables take their parents'
as parents and join the array of the class tables. The graphics menu's
table takes STMenu's as its parent although the class derives from
TGPane.
================
*/
// FUNCTION: STBC 0x00556820
void InterfaceModule::RegisterClasses( void ) {
	TGObject::RegisterCreator( STBUTTON_TYPE, CreateSTButton );
	TGObject::RegisterCreator( STROUNDEDBUTTON_TYPE, CreateSTRoundedButton );
	TGObject::RegisterCreator( STREPAIRBUTTON_TYPE, CreateSTRepairButton );
	TGObject::RegisterCreator( STTOGGLE_TYPE, CreateSTToggle );
	TGObject::RegisterCreator( STMENU_TYPE, CreateSTMenu );
	TGObject::RegisterCreator( STTOPLEVELMENU_TYPE, CreateSTTopLevelMenu );
	TGObject::RegisterCreator( STCHARACTERMENU_TYPE, CreateSTCharacterMenu );
	TGObject::RegisterCreator( STSUBPANE_TYPE, CreateSTSubPane );
	TGObject::RegisterCreator( STTARGETMENUSUBPANE_TYPE, CreateSTTargetMenuSubPane );
	TGObject::RegisterCreator( STFILLGAUGE_TYPE, CreateSTFillGauge );
	TGObject::RegisterCreator( STNUMERICBAR_TYPE, UtopiaModule::CreateSTNumericBar );
	TGObject::RegisterCreator( STFILEMENU_TYPE, CreateSTFileMenu );
	TGObject::RegisterCreator( STFILEDIALOG_TYPE, CreateSTFileDialog );
	TGObject::RegisterCreator( STLOADDIALOG_TYPE, CreateSTLoadDialog );
	TGObject::RegisterCreator( STSAVEDIALOG_TYPE, CreateSTSaveDialog );
	TGObject::RegisterCreator( STTARGETMENU_TYPE, CreateSTTargetMenu );
	TGObject::RegisterCreator( STSUBSYSTEMMENU_TYPE, CreateSTSubsystemMenu );
	TGObject::RegisterCreator( STCOMPONENTMENU_TYPE, CreateSTComponentMenu );
	TGObject::RegisterCreator( STCOMPONENTMENUITEM_TYPE, CreateSTComponentMenuItem );
	TGObject::RegisterCreator( ENGREPAIRPANE_TYPE, CreateEngRepairPane );
	TGObject::RegisterCreator( SORTEDREGIONMENU_TYPE, CreateSortedRegionMenu );
	TGObject::RegisterCreator( STWARPBUTTON_TYPE, CreateSTWarpButton );
	TGObject::RegisterCreator( STCHECKBOX_TYPE, CreateSTCheckbox );
	TGObject::RegisterCreator( STSTYLIZEDWINDOW_TYPE, CreateSTStylizedWindow );
	TGObject::RegisterCreator( STMISSIONLOG_TYPE, CreateSTMissionLog );
	TGObject::RegisterCreator( STTILEDICON_TYPE, CreateSTTiledIcon );
	TGObject::RegisterCreator( SUBTITLEACTION_TYPE, UtopiaModule::CreateSubtitleAction );

	STButton::GetClassTable()->SetParent( TGButtonBase::GetClassTable() );
	TGEventHandlerObject::classTables.Add( STButton::GetClassTable() );
	STRoundedButton::GetClassTable()->SetParent( TGButtonBase::GetClassTable() );
	TGEventHandlerObject::classTables.Add( STRoundedButton::GetClassTable() );
	STRepairButton::GetClassTable()->SetParent( STButton::GetClassTable() );
	TGEventHandlerObject::classTables.Add( STRepairButton::GetClassTable() );
	STToggle::GetClassTable()->SetParent( STButton::GetClassTable() );
	TGEventHandlerObject::classTables.Add( STToggle::GetClassTable() );
	STMenu::GetClassTable()->SetParent( TGWindow::GetClassTable() );
	TGEventHandlerObject::classTables.Add( STMenu::GetClassTable() );
	STTopLevelMenu::GetClassTable()->SetParent( STMenu::GetClassTable() );
	TGEventHandlerObject::classTables.Add( STTopLevelMenu::GetClassTable() );
	STCharacterMenu::GetClassTable()->SetParent( STMenu::GetClassTable() );
	TGEventHandlerObject::classTables.Add( STCharacterMenu::GetClassTable() );
	STSubPane::GetClassTable()->SetParent( TGPane::GetClassTable() );
	TGEventHandlerObject::classTables.Add( STSubPane::GetClassTable() );
	STTargetMenuSubPane::GetClassTable()->SetParent( STSubPane::GetClassTable() );
	TGEventHandlerObject::classTables.Add( STTargetMenuSubPane::GetClassTable() );
	STFillGauge::GetClassTable()->SetParent( TGPane::GetClassTable() );
	TGEventHandlerObject::classTables.Add( STFillGauge::GetClassTable() );
	STNumericBar::GetClassTable()->SetParent( TGPane::GetClassTable() );
	TGEventHandlerObject::classTables.Add( STNumericBar::GetClassTable() );
	STFileMenu::GetClassTable()->SetParent( STTopLevelMenu::GetClassTable() );
	TGEventHandlerObject::classTables.Add( STFileMenu::GetClassTable() );
	STFileDialog::GetClassTable()->SetParent( TGPane::GetClassTable() );
	TGEventHandlerObject::classTables.Add( STFileDialog::GetClassTable() );
	STLoadDialog::GetClassTable()->SetParent( STFileDialog::GetClassTable() );
	TGEventHandlerObject::classTables.Add( STLoadDialog::GetClassTable() );
	STSaveDialog::GetClassTable()->SetParent( STFileDialog::GetClassTable() );
	TGEventHandlerObject::classTables.Add( STSaveDialog::GetClassTable() );
	STTargetMenu::GetClassTable()->SetParent( STTopLevelMenu::GetClassTable() );
	TGEventHandlerObject::classTables.Add( STTargetMenu::GetClassTable() );
	STSubsystemMenu::GetClassTable()->SetParent( STMenu::GetClassTable() );
	TGEventHandlerObject::classTables.Add( STSubsystemMenu::GetClassTable() );
	STComponentMenu::GetClassTable()->SetParent( STMenu::GetClassTable() );
	TGEventHandlerObject::classTables.Add( STComponentMenu::GetClassTable() );
	STComponentMenuItem::GetClassTable()->SetParent( STButton::GetClassTable() );
	TGEventHandlerObject::classTables.Add( STComponentMenuItem::GetClassTable() );
	EngRepairPane::GetClassTable()->SetParent( TGPane::GetClassTable() );
	TGEventHandlerObject::classTables.Add( EngRepairPane::GetClassTable() );
	SortedRegionMenu::GetClassTable()->SetParent( STCharacterMenu::GetClassTable() );
	TGEventHandlerObject::classTables.Add( SortedRegionMenu::GetClassTable() );
	STWarpButton::GetClassTable()->SetParent( STButton::GetClassTable() );
	TGEventHandlerObject::classTables.Add( STWarpButton::GetClassTable() );
	STStylizedWindow::GetClassTable()->SetParent( TGWindow::GetClassTable() );
	TGEventHandlerObject::classTables.Add( STStylizedWindow::GetClassTable() );
	STMissionLog::GetClassTable()->SetParent( TGPane::GetClassTable() );
	TGEventHandlerObject::classTables.Add( STMissionLog::GetClassTable() );

	TGObject::RegisterCreator( TOPWINDOW_TYPE, CreateTopWindow );
	TGObject::RegisterCreator( MAINWINDOW_TYPE, CreateMainWindow );
	TGObject::RegisterCreator( BRIDGEWINDOW_TYPE, CreateBridgeWindow );
	TGObject::RegisterCreator( TACTICALWINDOW_TYPE, CreateTacticalWindow );
	TGObject::RegisterCreator( TACTICALCONTROLWINDOW_TYPE, CreateTacticalControlWindow );
	TGObject::RegisterCreator( CONSOLEWINDOW_TYPE, CreateConsoleWindow );
	TGObject::RegisterCreator( OPTIONSWINDOW_TYPE, CreateOptionsWindow );
	TGObject::RegisterCreator( SUBTITLEWINDOW_TYPE, CreateSubtitleWindow );
	TGObject::RegisterCreator( MAPWINDOW_TYPE, UtopiaModule::CreateMapWindow );
	TGObject::RegisterCreator( CINEMATICWINDOW_TYPE, CreateCinematicWindow );
	TGObject::RegisterCreator( GRAPHICSMENU_TYPE, CreateGraphicsMenu );
	TGObject::RegisterCreator( CDCHECKWINDOW_TYPE, CreateCDCheckWindow );
	TGObject::RegisterCreator( MODALDIALOGWINDOW_TYPE, CreateModalDialogWindow );

	TopWindow::GetClassTable()->SetParent( TGPane::GetClassTable() );
	TGEventHandlerObject::classTables.Add( TopWindow::GetClassTable() );
	MainWindow::GetClassTable()->SetParent( TGPane::GetClassTable() );
	TGEventHandlerObject::classTables.Add( MainWindow::GetClassTable() );
	BridgeWindow::GetClassTable()->SetParent( MainWindow::GetClassTable() );
	TGEventHandlerObject::classTables.Add( BridgeWindow::GetClassTable() );
	TacticalWindow::GetClassTable()->SetParent( MainWindow::GetClassTable() );
	TGEventHandlerObject::classTables.Add( TacticalWindow::GetClassTable() );
	TacticalControlWindow::GetClassTable()->SetParent( TGPane::GetClassTable() );
	TGEventHandlerObject::classTables.Add( TacticalControlWindow::GetClassTable() );
	ConsoleWindow::GetClassTable()->SetParent( MainWindow::GetClassTable() );
	TGEventHandlerObject::classTables.Add( ConsoleWindow::GetClassTable() );
	OptionsWindow::GetClassTable()->SetParent( MainWindow::GetClassTable() );
	TGEventHandlerObject::classTables.Add( OptionsWindow::GetClassTable() );
	SubtitleWindow::GetClassTable()->SetParent( MainWindow::GetClassTable() );
	TGEventHandlerObject::classTables.Add( SubtitleWindow::GetClassTable() );
	MapWindow::GetClassTable()->SetParent( MainWindow::GetClassTable() );
	TGEventHandlerObject::classTables.Add( MapWindow::GetClassTable() );
	CinematicWindow::GetClassTable()->SetParent( MainWindow::GetClassTable() );
	TGEventHandlerObject::classTables.Add( CinematicWindow::GetClassTable() );
	GraphicsMenu::GetClassTable()->SetParent( STMenu::GetClassTable() );
	TGEventHandlerObject::classTables.Add( GraphicsMenu::GetClassTable() );
	CDCheckWindow::GetClassTable()->SetParent( MainWindow::GetClassTable() );
	TGEventHandlerObject::classTables.Add( CDCheckWindow::GetClassTable() );
	ModalDialogWindow::GetClassTable()->SetParent( MainWindow::GetClassTable() );
	TGEventHandlerObject::classTables.Add( ModalDialogWindow::GetClassTable() );

	TGObject::RegisterCreator( SHIPDISPLAY_TYPE, CreateShipDisplay );
	TGObject::RegisterCreator( SHIELDSDISPLAY_TYPE, CreateShieldsDisplay );
	TGObject::RegisterCreator( DAMAGEDISPLAY_TYPE, CreateDamageDisplay );
	TGObject::RegisterCreator( DAMAGEICON_TYPE, CreateDamageIcon );
	TGObject::RegisterCreator( WEAPONSDISPLAY_TYPE, CreateWeaponsDisplay );
	TGObject::RegisterCreator( RADARDISPLAY_TYPE, CreateRadarDisplay );
	TGObject::RegisterCreator( RADARSCOPE_TYPE, CreateRadarScope );
	TGObject::RegisterCreator( RADARBLIP_TYPE, CreateRadarBlip );
	TGObject::RegisterCreator( TACWEAPONSCTRL_TYPE, CreateTacWeaponsCtrl );
	TGObject::RegisterCreator( ENGPOWERCTRL_TYPE, CreateEngPowerCtrl );
	TGObject::RegisterCreator( ENGPOWERDISPLAY_TYPE, CreateEngPowerDisplay );

	ShipDisplay::GetClassTable()->SetParent( STStylizedWindow::GetClassTable() );
	TGEventHandlerObject::classTables.Add( ShipDisplay::GetClassTable() );
	ShieldsDisplay::GetClassTable()->SetParent( TGPane::GetClassTable() );
	TGEventHandlerObject::classTables.Add( ShieldsDisplay::GetClassTable() );
	DamageDisplay::GetClassTable()->SetParent( TGPane::GetClassTable() );
	TGEventHandlerObject::classTables.Add( DamageDisplay::GetClassTable() );
	DamageIcon::GetClassTable()->SetParent( TGIcon::GetClassTable() );
	TGEventHandlerObject::classTables.Add( DamageIcon::GetClassTable() );
	WeaponsDisplay::GetClassTable()->SetParent( STStylizedWindow::GetClassTable() );
	TGEventHandlerObject::classTables.Add( WeaponsDisplay::GetClassTable() );
	RadarDisplay::GetClassTable()->SetParent( STStylizedWindow::GetClassTable() );
	TGEventHandlerObject::classTables.Add( RadarDisplay::GetClassTable() );
	RadarScope::GetClassTable()->SetParent( TGPane::GetClassTable() );
	TGEventHandlerObject::classTables.Add( RadarScope::GetClassTable() );
	RadarBlip::GetClassTable()->SetParent( TGIcon::GetClassTable() );
	TGEventHandlerObject::classTables.Add( RadarBlip::GetClassTable() );
	TacWeaponsCtrl::GetClassTable()->SetParent( STStylizedWindow::GetClassTable() );
	TGEventHandlerObject::classTables.Add( TacWeaponsCtrl::GetClassTable() );
	EngPowerCtrl::GetClassTable()->SetParent( TGPane::GetClassTable() );
	TGEventHandlerObject::classTables.Add( EngPowerCtrl::GetClassTable() );
	EngPowerDisplay::GetClassTable()->SetParent( TGPane::GetClassTable() );
	TGEventHandlerObject::classTables.Add( EngPowerDisplay::GetClassTable() );

	TGObject::RegisterCreator( RETICLEWINDOW_TYPE, CreateReticleWindow );
	TGObject::RegisterCreator( NAMEDRETICLEWINDOW_TYPE, CreateNamedReticleWindow );
	TGObject::RegisterCreator( PLAYERRETICLEWINDOW_TYPE, CreatePlayerReticleWindow );

	ReticleWindow::GetClassTable()->SetParent( TGPane::GetClassTable() );
	TGEventHandlerObject::classTables.Add( ReticleWindow::GetClassTable() );
	NamedReticleWindow::GetClassTable()->SetParent( ReticleWindow::GetClassTable() );
	TGEventHandlerObject::classTables.Add( NamedReticleWindow::GetClassTable() );
	PlayerReticleWindow::GetClassTable()->SetParent( NamedReticleWindow::GetClassTable() );
	TGEventHandlerObject::classTables.Add( PlayerReticleWindow::GetClassTable() );

	TGObject::RegisterCreator( MULTIPLAYERWINDOW_TYPE, CreateMultiplayerWindow );
	MultiplayerWindow::GetClassTable()->SetParent( MainWindow::GetClassTable() );
	TGEventHandlerObject::classTables.Add( MultiplayerWindow::GetClassTable() );
}

/*
================
InterfaceModule::CreateSTMenu
================
*/
// FUNCTION: STBC 0x00557600
TGObject *InterfaceModule::CreateSTMenu( long id ) {
	return new STMenu( id );
}

/*
================
InterfaceModule::CreateSTButton
================
*/
// FUNCTION: STBC 0x00557650
TGObject *InterfaceModule::CreateSTButton( long id ) {
	return new STButton( id );
}

/*
================
InterfaceModule::CreateSTRoundedButton
================
*/
// FUNCTION: STBC 0x005577a0
TGObject *InterfaceModule::CreateSTRoundedButton( long id ) {
	return new STRoundedButton( id );
}

/*
================
InterfaceModule::CreateSTRepairButton
================
*/
// FUNCTION: STBC 0x005578a0
TGObject *InterfaceModule::CreateSTRepairButton( long id ) {
	return new STRepairButton( id );
}

/*
================
InterfaceModule::CreateSTToggle
================
*/
// FUNCTION: STBC 0x005579f0
TGObject *InterfaceModule::CreateSTToggle( long id ) {
	return new STToggle( id );
}

/*
================
InterfaceModule::CreateSTCharacterMenu
================
*/
// FUNCTION: STBC 0x00557ba0
TGObject *InterfaceModule::CreateSTCharacterMenu( long id ) {
	return new STCharacterMenu( id );
}

/*
================
InterfaceModule::CreateSTFillGauge
================
*/
// FUNCTION: STBC 0x00557bf0
TGObject *InterfaceModule::CreateSTFillGauge( long id ) {
	return new STFillGauge( id );
}

/*
================
InterfaceModule::CreateSTTopLevelMenu
================
*/
// FUNCTION: STBC 0x00557c30
TGObject *InterfaceModule::CreateSTTopLevelMenu( long id ) {
	return new STTopLevelMenu( id );
}

/*
================
InterfaceModule::CreateSTTargetMenu
================
*/
// FUNCTION: STBC 0x00557c80
TGObject *InterfaceModule::CreateSTTargetMenu( long id ) {
	return new STTargetMenu( id );
}

/*
================
InterfaceModule::CreateSTSubsystemMenu
================
*/
// FUNCTION: STBC 0x00557cc0
TGObject *InterfaceModule::CreateSTSubsystemMenu( long id ) {
	return new STSubsystemMenu( id );
}

/*
================
InterfaceModule::CreateSTComponentMenu
================
*/
// FUNCTION: STBC 0x00557d00
TGObject *InterfaceModule::CreateSTComponentMenu( long id ) {
	return new STComponentMenu( id );
}

/*
================
InterfaceModule::CreateSTComponentMenuItem
================
*/
// FUNCTION: STBC 0x00557d50
TGObject *InterfaceModule::CreateSTComponentMenuItem( long id ) {
	return new STComponentMenuItem( id );
}

/*
================
InterfaceModule::CreateSTSubPane
================
*/
// FUNCTION: STBC 0x00557ea0
TGObject *InterfaceModule::CreateSTSubPane( long id ) {
	return new STSubPane( id, true );
}

/*
================
InterfaceModule::CreateSTTargetMenuSubPane
================
*/
// FUNCTION: STBC 0x00557ed0
TGObject *InterfaceModule::CreateSTTargetMenuSubPane( long id ) {
	return new STTargetMenuSubPane( id, false );
}

/*
================
InterfaceModule::CreateEngRepairPane
================
*/
// FUNCTION: STBC 0x00557f00
TGObject *InterfaceModule::CreateEngRepairPane( long id ) {
	return new EngRepairPane( id );
}

/*
================
InterfaceModule::CreateSTWarpButton
================
*/
// FUNCTION: STBC 0x00557f30
TGObject *InterfaceModule::CreateSTWarpButton( long id ) {
	return new STWarpButton( id );
}

/*
================
InterfaceModule::CreateSTFileMenu
================
*/
// FUNCTION: STBC 0x00558080
TGObject *InterfaceModule::CreateSTFileMenu( long id ) {
	return new STFileMenu( id );
}

/*
================
InterfaceModule::CreateSTFileDialog
================
*/
// FUNCTION: STBC 0x005580d0
TGObject *InterfaceModule::CreateSTFileDialog( long id ) {
	return new STFileDialog( id );
}

/*
================
InterfaceModule::CreateSTLoadDialog
================
*/
// FUNCTION: STBC 0x00558120
TGObject *InterfaceModule::CreateSTLoadDialog( long id ) {
	return new STLoadDialog( id );
}

/*
================
InterfaceModule::CreateSTSaveDialog
================
*/
// FUNCTION: STBC 0x00558170
TGObject *InterfaceModule::CreateSTSaveDialog( long id ) {
	return new STSaveDialog( id );
}

/*
================
InterfaceModule::CreateSortedRegionMenu
================
*/
// FUNCTION: STBC 0x005581c0
TGObject *InterfaceModule::CreateSortedRegionMenu( long id ) {
	return new SortedRegionMenu( id );
}

/*
================
InterfaceModule::CreateSTCheckbox
================
*/
// FUNCTION: STBC 0x00558210
TGObject *InterfaceModule::CreateSTCheckbox( long id ) {
	return new STCheckbox( id );
}

/*
================
InterfaceModule::CreateSTStylizedWindow
================
*/
// FUNCTION: STBC 0x00558240
TGObject *InterfaceModule::CreateSTStylizedWindow( long id ) {
	return new STStylizedWindow( id );
}

/*
================
InterfaceModule::CreateSTTiledIcon
================
*/
// FUNCTION: STBC 0x00558300
TGObject *InterfaceModule::CreateSTTiledIcon( long id ) {
	return new STTiledIcon( id );
}

/*
================
InterfaceModule::CreateTopWindow
================
*/
// FUNCTION: STBC 0x00558360
TGObject *InterfaceModule::CreateTopWindow( long id ) {
	return new TopWindow( id );
}

/*
================
InterfaceModule::CreateMainWindow
================
*/
// FUNCTION: STBC 0x005583d0
TGObject *InterfaceModule::CreateMainWindow( long id ) {
	return new MainWindow( id );
}

/*
================
InterfaceModule::CreateConsoleWindow
================
*/
// FUNCTION: STBC 0x00558420
TGObject *InterfaceModule::CreateConsoleWindow( long id ) {
	return new ConsoleWindow( id );
}

/*
================
InterfaceModule::CreateOptionsWindow
================
*/
// FUNCTION: STBC 0x00558470
TGObject *InterfaceModule::CreateOptionsWindow( long id ) {
	return new OptionsWindow( id );
}

/*
================
InterfaceModule::CreateBridgeWindow
================
*/
// FUNCTION: STBC 0x00558520
TGObject *InterfaceModule::CreateBridgeWindow( long id ) {
	return new BridgeWindow( id );
}

/*
================
InterfaceModule::CreateTacticalWindow
================
*/
// FUNCTION: STBC 0x00558570
TGObject *InterfaceModule::CreateTacticalWindow( long id ) {
	return new TacticalWindow( id );
}

/*
================
InterfaceModule::CreateTacticalControlWindow
================
*/
// FUNCTION: STBC 0x005585c0
TGObject *InterfaceModule::CreateTacticalControlWindow( long id ) {
	return new TacticalControlWindow( id );
}

/*
================
InterfaceModule::CreateSubtitleWindow
================
*/
// FUNCTION: STBC 0x00558650
TGObject *InterfaceModule::CreateSubtitleWindow( long id ) {
	return new SubtitleWindow( id );
}

/*
================
InterfaceModule::CreateCinematicWindow
================
*/
// FUNCTION: STBC 0x00558710
TGObject *InterfaceModule::CreateCinematicWindow( long id ) {
	return new CinematicWindow( id );
}

/*
================
InterfaceModule::CreateGraphicsMenu
================
*/
// FUNCTION: STBC 0x00558760
TGObject *InterfaceModule::CreateGraphicsMenu( long id ) {
	return new GraphicsMenu( id );
}

/*
================
InterfaceModule::CreateMultiplayerWindow
================
*/
// FUNCTION: STBC 0x005587b0
TGObject *InterfaceModule::CreateMultiplayerWindow( long id ) {
	return new MultiplayerWindow( id );
}

/*
================
InterfaceModule::CreateCDCheckWindow
================
*/
// FUNCTION: STBC 0x00558880
TGObject *InterfaceModule::CreateCDCheckWindow( long id ) {
	return new CDCheckWindow( id );
}

/*
================
InterfaceModule::CreateModalDialogWindow
================
*/
// FUNCTION: STBC 0x005588b0
TGObject *InterfaceModule::CreateModalDialogWindow( long id ) {
	return new ModalDialogWindow( id );
}

/*
================
InterfaceModule::CreateShipDisplay
================
*/
// FUNCTION: STBC 0x005588e0
TGObject *InterfaceModule::CreateShipDisplay( long id ) {
	return new ShipDisplay( id );
}

/*
================
InterfaceModule::CreateShieldsDisplay
================
*/
// FUNCTION: STBC 0x005589a0
TGObject *InterfaceModule::CreateShieldsDisplay( long id ) {
	return new ShieldsDisplay( id );
}

/*
================
InterfaceModule::CreateDamageIcon
================
*/
// FUNCTION: STBC 0x00558a00
TGObject *InterfaceModule::CreateDamageIcon( long id ) {
	return new DamageIcon( id );
}

/*
================
InterfaceModule::CreateDamageDisplay
================
*/
// FUNCTION: STBC 0x00558a60
TGObject *InterfaceModule::CreateDamageDisplay( long id ) {
	return new DamageDisplay( id );
}

/*
================
InterfaceModule::CreateWeaponsDisplay
================
*/
// FUNCTION: STBC 0x00558ad0
TGObject *InterfaceModule::CreateWeaponsDisplay( long id ) {
	return new WeaponsDisplay( id );
}

/*
================
InterfaceModule::CreateRadarDisplay
================
*/
// FUNCTION: STBC 0x00558bb0
TGObject *InterfaceModule::CreateRadarDisplay( long id ) {
	return new RadarDisplay( id );
}

/*
================
InterfaceModule::CreateRadarBlip
================
*/
// FUNCTION: STBC 0x00558c70
TGObject *InterfaceModule::CreateRadarBlip( long id ) {
	return new RadarBlip( id );
}

/*
================
InterfaceModule::CreateRadarScope
================
*/
// FUNCTION: STBC 0x00558cd0
TGObject *InterfaceModule::CreateRadarScope( long id ) {
	return new RadarScope( id );
}

/*
================
InterfaceModule::CreateTacWeaponsCtrl
================
*/
// FUNCTION: STBC 0x00558d40
TGObject *InterfaceModule::CreateTacWeaponsCtrl( long id ) {
	return new TacWeaponsCtrl( id );
}

/*
================
InterfaceModule::CreateEngPowerCtrl
================
*/
// FUNCTION: STBC 0x00558e00
TGObject *InterfaceModule::CreateEngPowerCtrl( long id ) {
	return new EngPowerCtrl( id );
}

/*
================
InterfaceModule::CreateEngPowerDisplay
================
*/
// FUNCTION: STBC 0x00558ec0
TGObject *InterfaceModule::CreateEngPowerDisplay( long id ) {
	return new EngPowerDisplay( id );
}

/*
================
InterfaceModule::CreateReticleWindow
================
*/
// FUNCTION: STBC 0x00558f10
TGObject *InterfaceModule::CreateReticleWindow( long id ) {
	return new ReticleWindow( id );
}

/*
================
InterfaceModule::CreateNamedReticleWindow
================
*/
// FUNCTION: STBC 0x00558fa0
TGObject *InterfaceModule::CreateNamedReticleWindow( long id ) {
	return new NamedReticleWindow( id );
}

/*
================
InterfaceModule::CreatePlayerReticleWindow
================
*/
// FUNCTION: STBC 0x00559040
TGObject *InterfaceModule::CreatePlayerReticleWindow( long id ) {
	return new PlayerReticleWindow( id );
}

/*
================
InterfaceModule::CreateSTMissionLog
================
*/
// FUNCTION: STBC 0x005590e0
TGObject *InterfaceModule::CreateSTMissionLog( long id ) {
	return new STMissionLog( id );
}

/*
================
InterfaceModule::RegisterHandlerNames
================
*/
// FUNCTION: STBC 0x00559130
void InterfaceModule::RegisterHandlerNames( void ) {
	STToggle::RegisterHandlerNames();
	STSubPane::RegisterHandlerNames();
	STTargetMenuSubPane::RegisterHandlerNames();
	STMenu::RegisterHandlerNames();
	STRepairButton::RegisterHandlerNames();
	STTopLevelMenu::RegisterHandlerNames();
	STCharacterMenu::RegisterHandlerNames();
	STTargetMenu::RegisterHandlerNames();
	STSubsystemMenu::RegisterHandlerNames();
	STComponentMenu::RegisterHandlerNames();
	STFileMenu::RegisterHandlerNames();
	STFileDialog::RegisterHandlerNames();
	STLoadDialog::RegisterHandlerNames();
	STSaveDialog::RegisterHandlerNames();
	STNumericBar::RegisterHandlerNames();
	SortedRegionMenu::RegisterHandlerNames();
	STWarpButton::RegisterHandlerNames();
	STStylizedWindow::RegisterHandlerNames();
	STMissionLog::RegisterHandlerNames();
	ShipDisplay::RegisterHandlerNames();
	ShieldsDisplay::RegisterHandlerNames();
	DamageDisplay::RegisterHandlerNames();
	DamageIcon::RegisterHandlerNames();
	WeaponsDisplay::RegisterHandlerNames();
	RadarDisplay::RegisterHandlerNames();
	RadarScope::RegisterHandlerNames();
	TacWeaponsCtrl::RegisterHandlerNames();
	EngPowerCtrl::RegisterHandlerNames();
	EngPowerDisplay::RegisterHandlerNames();
	EngRepairPane::RegisterHandlerNames();
	TopWindow::RegisterHandlerNames();
	TacticalControlWindow::RegisterHandlerNames();
	Editor::RegisterHandlerNames();
	GraphicsMenu::RegisterHandlerNames();
	ReticleWindow::RegisterHandlerNames();
	NamedReticleWindow::RegisterHandlerNames();
	PlayerReticleWindow::RegisterHandlerNames();
	g_kEventManager.RegisterHandlerFunction( InterfaceModule::OverrideHandler, "InterfaceModule::OverrideHandler" );
	g_kEventManager.RegisterHandlerFunction( InterfaceModule::HandleUISwitchIconGroups, "InterfaceModule::HandleUISwitchIconGroups" );
	g_kEventManager.RegisterHandlerFunction( InterfaceModule::HandleUIResize, "InterfaceModule::HandleUIResize" );
	g_kEventManager.RegisterHandlerFunction( InterfaceModule::HandleUIReposition, "InterfaceModule::HandleUIReposition" );
	g_kEventManager.RegisterHandlerFunction( InterfaceModule::TGParagraphSoundHandler, "InterfaceModule::TGParagraphSoundHandler" );
	g_kEventManager.RegisterHandlerFunction( InterfaceModule::STStylizedWindowMinimizeHandler, "InterfaceModule::STStylizedWindowMinimizeHandler" );
}

/*
================
InterfaceModule::RegisterHandlers

The paragraphs' key sound and the stylized windows' minimize sound go in
those classes' tables.
================
*/
// FUNCTION: STBC 0x00559270
void InterfaceModule::RegisterHandlers( void ) {
	STToggle::RegisterHandlers();
	STSubPane::RegisterHandlers();
	STTargetMenuSubPane::RegisterHandlers();
	STMenu::RegisterHandlers();
	STRepairButton::RegisterHandlers();
	STTopLevelMenu::RegisterHandlers();
	STCharacterMenu::RegisterHandlers();
	STTargetMenu::RegisterHandlers();
	STSubsystemMenu::RegisterHandlers();
	STComponentMenu::RegisterHandlers();
	STFileMenu::RegisterHandlers();
	STFileDialog::RegisterHandlers();
	STLoadDialog::RegisterHandlers();
	STSaveDialog::RegisterHandlers();
	STNumericBar::RegisterHandlers();
	STWarpButton::RegisterHandlers();
	STStylizedWindow::RegisterHandlers();
	STMissionLog::RegisterHandlers();
	ShipDisplay::RegisterHandlers();
	ShieldsDisplay::RegisterHandlers();
	DamageDisplay::RegisterHandlers();
	DamageIcon::RegisterHandlers();
	RadarDisplay::RegisterHandlers();
	RadarScope::RegisterHandlers();
	WeaponsDisplay::RegisterHandlers();
	TacWeaponsCtrl::RegisterHandlers();
	EngPowerCtrl::RegisterHandlers();
	EngPowerDisplay::RegisterHandlers();
	EngRepairPane::RegisterHandlers();
	TopWindow::RegisterHandlers();
	TacticalControlWindow::RegisterHandlers();
	Editor::RegisterHandlers();
	GraphicsMenu::RegisterHandlers();
	ReticleManagerWindow::RegisterHandlers();
	ReticleWindow::RegisterHandlers();
	NamedReticleWindow::RegisterHandlers();
	PlayerReticleWindow::RegisterHandlers();
	TGEventHandlerObject::AddFuncHandlerForClass( TGParagraph::GetClassTable(), ET_PARAGRAPH_KEY, "InterfaceModule::TGParagraphSoundHandler" );
	TGEventHandlerObject::AddFuncHandlerForClass( STStylizedWindow::GetClassTable(), ET_ST_MINIMIZE, "InterfaceModule::STStylizedWindowMinimizeHandler" );
}

/*
================
InterfaceModule::WriteToStream
================
*/
// FUNCTION: STBC 0x00559360
bool InterfaceModule::WriteToStream( TGStream *stream ) {
	STMenu::WriteColorSchemes( stream );
	SortedRegionMenu::WriteMenus( stream );
	STTopLevelMenu::WriteOpenMenu( stream );
	return true;
}

/*
================
InterfaceModule::ReadFromStream
================
*/
// FUNCTION: STBC 0x00559380
bool InterfaceModule::ReadFromStream( TGStream *stream ) {
	STMenu::ReadColorSchemes( stream );
	SortedRegionMenu::ReadMenus( stream );
	STTopLevelMenu::ReadOpenMenu( stream );
	return true;
}

/*
================
InterfaceModule::FixUpPointers
================
*/
// FUNCTION: STBC 0x005593a0
bool InterfaceModule::FixUpPointers( void ) {
	if ( !SortedRegionMenu::FixUpMenus() ) {
		return false;
	}
	if ( !STTopLevelMenu::FixUpOpenMenu() ) {
		return false;
	}
	return true;
}

/*
================
InterfaceModule::PostFixUp
================
*/
// FUNCTION: STBC 0x005593c0
void InterfaceModule::PostFixUp( void ) {
	SortedRegionMenu::PostFixUpMenus();
	STTopLevelMenu::PostFixUpOpenMenu();
}
