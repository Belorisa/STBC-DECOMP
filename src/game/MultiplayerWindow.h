/*
===============================================================================

	MultiplayerWindow.h

	The members of MultiplayerWindow that the SWIG wrappers MultiplayerWindow_* call, and
	its layout. Initialize builds the menus through the scripts
	(Multiplayer.MultiplayerMenus, MultiplayerInterfaceHandlers) and takes
	the network's message, connect and disconnect events; showing and
	hiding the window wait for the options window while it is open.

	0xB8 bytes. Evidence: the constructor (0x504390) calls MainWindow's
	with MWT_MULTIPLAYER, 1.0 by 1.0, constructs the TGStrings at 0x7C,
	0x8C and 0x9C, stores the vtable 0x88E74C (84 slots; the destructor,
	GetObjType, IsTypeOf, the stream functions, the class strings,
	GetClassHandlers, GotFocus, LostFocus, SetVisible, SetNotVisible,
	Initialize and Update overridden) and clears the rest; the class strings
	"MultiplayerWindow", "_p_MultiplayerWindow" and "MultiplayerWindowPtr";
	the handler table at 0x9872E0 and the handler names
	"MultiplayerWindow::...Handler" (0x5046B0); the top
	window's constructor (0x50C430) allocates 0xB8 bytes for it. The SWIG
	inline accessors read the panes 0x50-0x74 (TGPane *), IsServerSelected
	0x78, IsGameOver 0xB1, IsDirectJoin 0xB2, IsTeamChat 0xB3 and
	IsHostUsingModem 0xB4; ClearCurrentServer (0x506910) and
	UtopiaModule::TerminateGameSpy (0x446010) clear 0x78.

===============================================================================
*/

#ifndef __MULTIPLAYERWINDOW_H__
#define __MULTIPLAYERWINDOW_H__

#include "MainWindow.h"
#include "TGString.h"

class STMenu;
class STStylizedWindow;
class TGEvent;
class TGEventHandlerTable;
class TGMessage;
class TGMessageEvent;
class TGObject;
class TGPlayerEvent;
class TGStream;
class TGString;

// GetObjType's value for a MultiplayerWindow (the type its Cast checks).
#define MULTIPLAYERWINDOW_TYPE	0x80D5

// The event a mission button sends the window: the mission's folder name.
#define ET_SET_MISSION_NAME		0x8000F0

// The events the multiplayer menus send the window (the App constants).
#define ET_START					0x8000D3
#define ET_PLAYER_BOOT_EVENT		0x8000F6
#define ET_REFRESH_SERVER_LIST		0x8000F7
#define ET_SELECT_SERVER_ENTRY		0x8000F9
#define ET_SORT_SERVER_LIST			0x8000FB

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x00504530 ??_GMultiplayerWindow@@UAEPAXI@Z
class MultiplayerWindow : public MainWindow {
public:
	// The stream loader's: the stream fills the members (InterfaceModule's creator).
	// FUNCTION: STBC 0x005587f0 ??0MultiplayerWindow@@QAE@J@Z @game/InterfaceModule
					MultiplayerWindow( long id ) : MainWindow( id ) {}
	// The class's handler table, for the module that links the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

					MultiplayerWindow( void );
	virtual			~MultiplayerWindow( void );

	// TGObject
	virtual int		GetObjType( void );
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	virtual bool	FixUpPointers( void );
	virtual void	PostFixUp( void );
	virtual char *	GetClassName( void );
	virtual char *	GetSwigTypeName( void );
	virtual char *	GetPythonClassName( void );

	// TGEventHandlerObject
	virtual TGEventHandlerTable *	GetClassHandlers( void );

	// TGUIObject
	virtual void	GotFocus( bool layout = true );
	virtual void	LostFocus( bool layout = true );
	virtual void	SetVisible( bool layout = true );
	virtual void	SetNotVisible( bool layout = true );

	// MainWindow
	virtual void	Initialize( void );
	virtual void	Update( float time );

	static void		RegisterHandlerNames( void );		// invented name
	static void		RegisterHandlers( void );			// invented name

	static MultiplayerWindow *	Cast( TGObject * );
	void	ToggleChatWindow( bool );
	bool	IsChatWindowActive( void );
	void	ClearCurrentServer( void );
	STMenu *	BuildMissionMenu( STStylizedWindow * );
	void	HideAllChildren( void );
	bool	IsAnyChildVisible( void );
	static void	SetStatusMessage( TGString * );

	TGPane *	GetMultiplayerMenu( void ) { return multiplayerMenu; }
	void		SetMultiplayerMenu( TGPane *pane ) { multiplayerMenu = pane; }
	TGPane *	GetHostMenu( void ) { return hostMenu; }
	void		SetHostMenu( TGPane *pane ) { hostMenu = pane; }
	TGPane *	GetJoinMenu( void ) { return joinMenu; }
	void		SetJoinMenu( TGPane *pane ) { joinMenu = pane; }
	TGPane *	GetMissionPane( void ) { return missionPane; }
	void		SetMissionPane( TGPane *pane ) { missionPane = pane; }
	TGPane *	GetStatusWindow( void ) { return statusWindow; }
	void		SetStatusWindow( TGPane *pane ) { statusWindow = pane; }
	TGPane *	GetEndWindow( void ) { return endWindow; }
	void		SetEndWindow( TGPane *pane ) { endWindow = pane; }
	TGPane *	GetPlayerListWindow( void ) { return playerListWindow; }
	void		SetPlayerListWindow( TGPane *pane ) { playerListWindow = pane; }
	TGPane *	GetDedicatedServerMenu( void ) { return dedicatedServerMenu; }
	void		SetDedicatedServerMenu( TGPane *pane ) { dedicatedServerMenu = pane; }
	TGPane *	GetDirectJoinMenu( void ) { return directJoinMenu; }
	void		SetDirectJoinMenu( TGPane *pane ) { directJoinMenu = pane; }
	TGPane *	GetChatWindow( void ) { return chatWindow; }
	void		SetChatWindow( TGPane *pane ) { chatWindow = pane; }
	bool		IsServerSelected( void ) { return currentServer != NULL; }
	void		SetCurrentServer( void *server ) { currentServer = server; }	// invented name
	bool		IsGameOver( void ) { return gameOver; }
	void		SetGameOver( bool gameOver_ ) { gameOver = gameOver_; }
	bool		IsDirectJoin( void ) { return directJoin; }
	void		SetDirectJoin( bool directJoin_ ) { directJoin = directJoin_; }
	bool		IsTeamChat( void ) { return teamChat; }
	void		SetTeamChat( bool teamChat_ ) { teamChat = teamChat_; }
	bool		IsHostUsingModem( void ) { return hostUsingModem; }
	void		SetHostUsingModem( bool hostUsingModem_ ) { hostUsingModem = hostUsingModem_; }

protected:
	void			Unknown506D40( TGPane *chat );		// ToggleChatWindow calls it before showing the chat window

	// The handlers, by their registered names.
	void			StartGameHandler( TGEvent *event );
	void			ReceiveMessageHandler( TGMessageEvent *event );
	void			ConnectHandler( TGEvent *event );
	void			DisconnectHandler( TGEvent *event );
	void			SetMissionNameHandler( TGEvent *event );
	void			RefreshServerListHandler( TGEvent *event );
	void			BootPlayerHandler( TGPlayerEvent *event );
	void			SelectServerHandler( TGEvent *event );
	void			SortServerListHandler( TGEvent *event );

	// The game messages ReceiveMessageHandler passes on, by their types
	// (invented names).
	void			ReceiveGameInitializeMessage( TGMessage *message );
	void			ReceiveGameInitializeDoneMessage( TGMessage *message );
	void			ReceiveCollisionEnabledMessage( TGMessage *message );

	static TGEventHandlerTable	classHandlers;		// invented name

	TGPane *		multiplayerMenu;		// 0x50
	TGPane *		hostMenu;				// 0x54
	TGPane *		joinMenu;				// 0x58
	TGPane *		missionPane;			// 0x5C
	TGPane *		statusWindow;			// 0x60
	TGPane *		endWindow;				// 0x64
	TGPane *		playerListWindow;		// 0x68
	TGPane *		dedicatedServerMenu;	// 0x6C
	TGPane *		directJoinMenu;			// 0x70
	TGPane *		chatWindow;				// 0x74
	void *			currentServer;			// 0x78 the server chosen in the list; its type is not known yet (invented name)
	TGString		gameName;				// 0x7C Get/SetGameName
	TGString		playerName;				// 0x8C Get/SetPlayerName
	TGString		password;				// 0x9C Get/SetPassword
	long			chosenMissionButtonID;	// 0xAC the ID of the mission button chosen last (invented name)
	bool			connected;				// 0xB0 set by ConnectHandler, cleared on disconnect; the game set-up messages wait for it (invented name)
	bool			gameOver;				// 0xB1
	bool			directJoin;				// 0xB2
	bool			teamChat;				// 0xB3
	bool			hostUsingModem;			// 0xB4 true after construction

	// Declared for the SWIG wrappers that call them.
public:
	void	SetGameName( TGString *string ) { gameName = *string; }	// the SWIG wrapper MultiplayerWindow_SetGameName expands it
	void	GetGameName( TGString *string ) { *string = gameName; }	// the SWIG wrapper MultiplayerWindow_GetGameName expands it
	void	SetPlayerName( TGString *string ) { playerName = *string; }	// the SWIG wrapper MultiplayerWindow_SetPlayerName expands it
	void	GetPlayerName( TGString *string ) { *string = playerName; }	// the SWIG wrapper MultiplayerWindow_GetPlayerName expands it
	void	SetPassword( TGString *string ) { password = *string; }	// the SWIG wrapper MultiplayerWindow_SetPassword expands it
	void	GetPassword( TGString *string ) { *string = password; }	// the SWIG wrapper MultiplayerWindow_GetPassword expands it
};

#endif /* !__MULTIPLAYERWINDOW_H__ */
