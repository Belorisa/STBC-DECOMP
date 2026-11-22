/*
===============================================================================

	STColors.h

	The colours of the ST* interface classes and of the game's windows,
	which the scripts set (through the SWIG variables) when the interface
	is built.

	Evidence: the SWIG module's variable list (initApp, 0x65A6C0..) names
	each colour; its getter and setter give the address. One unit,
	linked between STCheckbox and STFileDialog, constructs them all in
	this order (the dynamic initialisers 0x51D050 to 0x51EB80), and its
	data holds the menu colour schemes (0x8E28EC to 0x8E2A3C). The unit's
	and this header's name are invented.

===============================================================================
*/

#ifndef __STCOLORS_H__
#define __STCOLORS_H__

#include "NiColor.h"

// The button colours of each menu depth: normal, selected, highlighted
// and disabled (STBUTTON_NUM_COLORS) for each of STBUTTON_NUM_UI_HEIGHTS
// depths.
extern NiColorA		g_kSTButtonColors[][4];
// The colours of a button's marker and checkbox.
extern NiColorA		g_kSTButtonMarkerDefault;
extern NiColorA		g_kSTButtonMarkerHighlighted;
extern NiColorA		g_kSTButtonMarkerSelected;
extern NiColorA		g_kSTButtonMarkerGray;
extern NiColorA		g_kSTButtonCheckmarkOn;
extern NiColorA		g_kSTButtonCheckmarkOff;
// The colour of a menu's arrows.
extern NiColorA		g_kSTMenuArrowColor;
// The title bar colours of a menu at each depth.
extern NiColorA		g_kSTMenu1NormalBase;
extern NiColorA		g_kSTMenu1HighlightedBase;
extern NiColorA		g_kSTMenu1Disabled;
extern NiColorA		g_kSTMenu1OpenedHighlightedBase;
extern NiColorA		g_kSTMenu1Selected;
extern NiColorA		g_kSTMenu2NormalBase;
extern NiColorA		g_kSTMenu2HighlightedBase;
extern NiColorA		g_kSTMenu2Disabled;
extern NiColorA		g_kSTMenu2OpenedHighlightedBase;
extern NiColorA		g_kSTMenu2Selected;
extern NiColorA		g_kSTMenu3NormalBase;
extern NiColorA		g_kSTMenu3HighlightedBase;
extern NiColorA		g_kSTMenu3Disabled;
extern NiColorA		g_kSTMenu3OpenedHighlightedBase;
extern NiColorA		g_kSTMenu3Selected;
extern NiColorA		g_kSTMenu4NormalBase;
extern NiColorA		g_kSTMenu4HighlightedBase;
extern NiColorA		g_kSTMenu4Disabled;
extern NiColorA		g_kSTMenu4OpenedHighlightedBase;
extern NiColorA		g_kSTMenu4Selected;
// The colours of a menu's or button's name.
extern NiColorA		g_kSTMenuTextColor;
extern NiColorA		g_kSTMenuTextSelectedColor;
extern NiColorA		g_kSTMenuTextHighlightColor;
// The text entry colours.
extern NiColorA		g_kTextEntryColor;
extern NiColorA		g_kTextHighlightColor;
extern NiColorA		g_kTextEntryBackgroundColor;
extern NiColorA		g_kTextEntryBackgroundHighlightColor;
// The torpedo readiness colours of the tactical interface.
extern NiColorA		g_kTIPhotonReadyColor;
extern NiColorA		g_kTIPhotonNotReadyColor;
extern NiColorA		g_kSTRadarBorderHighlighted;
// The window frame colours.
extern NiColorA		g_kTitleColor;
extern NiColorA		g_kInterfaceBorderColor;
extern NiColorA		g_kLeftSeparatorColor;
// The radar colours.
extern NiColorA		g_kRadarBorder;
extern NiColorA		g_kSTRadarIncomingTorpColor;
extern NiColorA		g_kRadarFriendlyColor;
extern NiColorA		g_kRadarEnemyColor;
extern NiColorA		g_kRadarNeutralColor;
extern NiColorA		g_kRadarUnknownColor;
// The colours of the ship's condition, from destroyed to undamaged.
extern NiColorA		g_pConditionColors[];
// The subsystem gauge colours.
extern NiColorA		g_kSubsystemFillColor;
extern NiColorA		g_kSubsystemEmptyColor;
extern NiColorA		g_kSubsystemDisabledColor;
extern NiColorA		g_kTacWeaponsCtrlHeaderTextColor;
// The damage display colours.
extern NiColorA		g_kDamageDisplayDestroyedColor;
extern NiColorA		g_kDamageDisplayDamagedColor;
extern NiColorA		g_kDamageDisplayDisabledColor;
// The main menu colours.
extern NiColorA		g_kMainMenuButtonColor;
extern NiColorA		g_kMainMenuButtonHighlightedColor;
extern NiColorA		g_kMainMenuButtonSelectedColor;
extern NiColorA		g_kMainMenuButton1Color;
extern NiColorA		g_kMainMenuButton1HighlightedColor;
extern NiColorA		g_kMainMenuButton1SelectedColor;
extern NiColorA		g_kMainMenuButton2Color;
extern NiColorA		g_kMainMenuButton2HighlightedColor;
extern NiColorA		g_kMainMenuButton2SelectedColor;
extern NiColorA		g_kMainMenuButton3Color;
extern NiColorA		g_kMainMenuButton3HighlightedColor;
extern NiColorA		g_kMainMenuButton3SelectedColor;
extern NiColorA		g_kMainMenuBorderMainColor;
extern NiColorA		g_kMainMenuBorderOffColor;
extern NiColorA		g_kMainMenuBorderBlock1Color;
extern NiColorA		g_kMainMenuBorderTopColor;
// The engineering colours.
extern NiColorA		g_kEngineeringShieldsColor;
extern NiColorA		g_kEngineeringEnginesColor;
extern NiColorA		g_kEngineeringWeaponsColor;
extern NiColorA		g_kEngineeringSensorsColor;
extern NiColorA		g_kEngineeringCloakColor;
extern NiColorA		g_kEngineeringTractorColor;
extern NiColorA		g_kEngineeringWarpCoreColor;
extern NiColorA		g_kEngineeringMainPowerColor;
extern NiColorA		g_kEngineeringBackupPowerColor;
extern NiColorA		g_kEngineeringCtrlBkgndLineColor;
// The multiplayer and quick battle colours.
extern NiColorA		g_kQuickBattleBrightRed;
extern NiColorA		g_kMultiplayerBorderBlue;
extern NiColorA		g_kMultiplayerBorderPurple;
extern NiColorA		g_kMultiplayerStylizedPurple;
extern NiColorA		g_kMultiplayerButtonOrange;
extern NiColorA		g_kMultiplayerButtonPurple;
// A colour no code reads, not a SWIG variable.
extern NiColorA		g_kUnknown9883D8;
extern NiColorA		g_kMultiplayerDividerPurple;

#endif /* !__STCOLORS_H__ */
