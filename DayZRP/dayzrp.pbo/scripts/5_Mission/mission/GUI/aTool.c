class aTool extends UIScriptedMenu
{
	MultilineTextWidget txt_guide;
	ButtonWidget btn_close, btn_teleport, btn_godmode, btn_invisible, btn_spawnZombie, btn_spawnWolf, btn_spawnItem, btn_spawnItemInv, btn_spawnItemAI, btn_spawnZombieMany, btn_spawnWolfMany, btn_removeEntity, btn_removeEntityMany, btn_teleportToPlayer, btn_teleportToMe, btn_copyFromClipboard;
	EditBoxWidget inpt_search;
	TextListboxWidget tlb_playerList;

	const string ATOOL_VERSION 	= "1.1.0";
	bool playerInvisible = false;

	private PlayerBaseClient player;
	private MissionGameplay mission;

	void aTool()
	{
		m_id = MENU_ATOOL;
	}

	PlayerBase findPlayer(int id) {
		ref array<Man> players = new array<Man>;
		GetGame().GetPlayers(players);

		PlayerBase returnedPlayer;

		for ( int i = 0; i < players.Count(); i++ )
		{
			PlayerBase temp = PlayerBase.Cast(players.Get(i));
			if (temp.GetIdentity().GetPlayerId() == id) {
				returnedPlayer = temp;
			}
		}

		return returnedPlayer;
	}

	override Widget Init() {
		layoutRoot = GetGame().GetWorkspace().CreateWidgets("drp/dayzrp/scripts/5_Mission/mission/GUI/aTool.layout");

		txt_guide = TextWidget.Cast( layoutRoot.FindAnyWidget( "txt_guide" ) );
		txt_guide.SetText("aTool "+ATOOL_VERSION+" keybindings:\n- Freecam (Insert)\n- Teleport (Shift+Y in freecam)\n-Refresh Freecam bubble/position (Ctrl+Y)\n- Spawn infected (Ctrl+End)\n- Spawn a wolfie (Shift+End)");

		btn_close = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "btn_close" ) );
		btn_teleport = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "btn_teleport" ) );
		btn_godmode = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "btn_godmode" ) );
		btn_invisible = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "btn_invisible" ) );
		btn_spawnZombie = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "btn_spawnZombie" ) );
		btn_spawnWolf = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "btn_spawnWolf" ) );
		inpt_search = EditBoxWidget.Cast( layoutRoot.FindAnyWidget( "search_input" ) );

		btn_spawnItem = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "btn_spawnItem" ) );
		btn_spawnItemInv = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "btn_spawnItemInv" ) );
		btn_spawnItemAI = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "btn_spawnItemAI" ) );
		btn_spawnZombieMany = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "btn_spawnZombieMany" ) );
		btn_spawnWolfMany = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "btn_spawnWolfMany" ) );
		btn_removeEntity = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "btn_removeEntity" ) );
		btn_removeEntityMany = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "btn_removeEntityMany" ) );
		btn_copyFromClipboard = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "btn_copyFromClipboard" ) );

		tlb_playerList = TextListboxWidget.Cast( layoutRoot.FindAnyWidget( "playerlist" ) );
		btn_teleportToPlayer = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "btn_teleportToPlayer" ) );
		btn_teleportToMe = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "btn_teleportToMe" ) );

		return layoutRoot;
	}
	void Init(PlayerBase missionPlayer) {
		player = PlayerBaseClient.Cast(missionPlayer);
	}

	override void OnShow() {
		super.OnShow();
		UpdateList();
	}

	override void OnHide() {
		super.OnHide();
		GetGame().GetUIManager().ShowUICursor( false );
		GetGame().GetInput().ResetGameFocus();
	}

	override bool OnChange( Widget w, int x, int y, bool finished ) {
		super.OnChange( w, x, y, finished );

		if ( w == tlb_playerList )
		{
			UpdateList();
			return true;
		}

		return false;
	}

	override bool OnClick( Widget w, int x, int y, int button ) {
		super.OnClick(w, x, y, button);
		vector position;
		Camera cam;
		ref Param2<string, vector> tpPlayerParam;
		ref Param2<string, bool> tpToPlayerParam;
		ref Param2<vector, int> vectorIntParam;

		switch( true )
		{
			case (w == btn_close):
			{
				GetGame().GetUIManager().CloseAll();
				return true;
				break;
			}
			case (w == btn_spawnItem || w == btn_spawnItemInv || w == btn_spawnItemAI):
			{
				string strSelection = inpt_search.GetText();

				if( strSelection != "" )
				{
					vector itempos = "0 0 0";
					int mode = 0;

					if( w == btn_spawnItem || w == btn_spawnItemAI )
					{
						itempos = MissionGameplay.Cast(GetGame().GetMission()).cursorPosition(player);
					}

					if ( w == btn_spawnItemAI )
					{
						mode = 1;
					}

					ref Param3<string, vector, int> itemparams = new Param3<string, vector, int>(strSelection, itempos, mode);
					player.RPCSingleParam(MRPCs.DEV_RPC_SPAWN_ITEM, itemparams, false);
				}
				return true;
				break;
			}
			case (w == btn_godmode):
			{
				if (player.m_godMode) {
					player.m_godMode = false;
					player.MessageStatus("You are no longer a god.");
				} else {
					player.m_godMode = true;
					player.MessageStatus("You are now a god.");
				}
				Param1<int> p = new Param1<int>(0);
				player.RPCSingleParam(MRPCs.RPC_CLIENT_REQUEST_GODMODE_TOGGLE, p, true);
				return true;
				break;
			}
			case (w == btn_teleport):
			{
				position = MissionGameplay.Cast(GetGame().GetMission()).cursorPosition(player);
				ref Param1<vector> params = new Param1<vector>(position);
				player.RPCSingleParam(MRPCs.RPC_CLIENT_REQUEST_TELEPORT, params, true);
				return true;
				break;
			}
			case (w == btn_invisible):
			{
				player.MessageStatus("Invisibility is broken as of right now. Sorry.");
				if (player.m_isInvisible) {
					player.m_isInvisible = false;
					//player.MessageStatus("You are no longer invisible.");
				} else {
					player.m_isInvisible = true;
					//player.MessageStatus("You are invisible.");
				}
				Param1<int> pi = new Param1<int>(0);
				player.RPCSingleParam(MRPCs.RPC_CLIENT_REQUEST_INVISIBLE_TOGGLE, pi, true);
				return true;
				break;
			}
			case (w == btn_spawnZombie || w == btn_spawnZombieMany):
			{
				position = MissionGameplay.Cast(GetGame().GetMission()).cursorPosition(player);
				int infCount = 1;
				if (w == btn_spawnZombieMany) {
					infCount = 5;
				}
				vectorIntParam = new Param2<vector, int>(position, infCount);
				player.RPCSingleParam(MRPCs.RPC_CLIENT_REQUEST_SPAWN_INFECTED, vectorIntParam, false);
				return true;
				break;
			}
			case (w == btn_spawnWolf || w == btn_spawnWolfMany):
			{
				position = MissionGameplay.Cast(GetGame().GetMission()).cursorPosition(player);
				int wolfCount = 1;
				if (w == btn_spawnWolfMany) {
					wolfCount = 5;
				}
				vectorIntParam = new Param2<vector, int>(position, wolfCount);
				player.RPCSingleParam(MRPCs.RPC_CLIENT_REQUEST_SPAWN_WOLFIE, vectorIntParam, false);
				return true;
				break;
			}
			case (w == btn_removeEntity || w == btn_removeEntityMany):
			{
				position = MissionGameplay.Cast(GetGame().GetMission()).cursorPosition(player);
				int many = 0;
				if (w == btn_removeEntityMany) {
					many = 1;
				}
				vectorIntParam = new Param2<vector, int>(position, many);
				player.RPCSingleParam(MRPCs.RPC_CLIENT_REQUEST_DESPAWN_ITEM, vectorIntParam, false);
				return true;
				break;
			}
			case (w == btn_teleportToPlayer):
			{
				cam = MissionGameplay.Cast(GetGame().GetMission()).cam;
				if (cam) {
					tpToPlayerParam = new Param2<string, bool>(GetCurrentSelection(), true);
				} else {
					tpToPlayerParam = new Param2<string, bool>(GetCurrentSelection(), false);
				}
				player.RPCSingleParam(MRPCs.RPC_CLIENT_REQUEST_TELEPORT_TO_PLAYER, tpToPlayerParam, true);
				return true;
				break;
			}
			case (w == btn_teleportToMe):
			{
				cam = MissionGameplay.Cast(GetGame().GetMission()).cam;
				if (cam) {
					tpPlayerParam = new Param2<string, vector>(GetCurrentSelection(), MissionGameplay.Cast(GetGame().GetMission()).cursorPosition(player));
				} else {
					tpPlayerParam = new Param2<string, vector>(GetCurrentSelection(), vector.Zero);
				}
				player.RPCSingleParam(MRPCs.RPC_CLIENT_REQUEST_TELEPORT_PLAYER_TO_ME, tpPlayerParam, true);
				return true;
				break;
			}
			case (w == btn_copyFromClipboard):
			{
				string txt = "";
				GetGame().CopyFromClipboard(txt);
				inpt_search.SetText(txt);
				return true;
				break;
			}
		}
		return false;
	}


    void UpdateList()
    {
	    tlb_playerList.ClearItems();
			SyncPlayerList syncPlayers = ClientData.m_PlayerList;

			for( int i = 0; i < syncPlayers.m_PlayerList.Count(); i++ )
			{
				SyncPlayer listPlayer = syncPlayers.m_PlayerList.Get(i);
				tlb_playerList.AddItem( string.Format("%1:%2", i, listPlayer.m_PlayerName), listPlayer, 0 );
			}
    }

	string GetCurrentSelection() {
		if ( tlb_playerList.GetSelectedRow() != -1 ) {
			ref SyncPlayer result;
			tlb_playerList.GetItemData(tlb_playerList.GetSelectedRow(), 0, result);

			return result.m_UID;
		}
		return "";
	}

	override bool OnKeyPress( Widget w, int x, int y, int key ) {
		if( key == KeyCode.KC_ESCAPE ) {
			GetGame().GetUIManager().CloseAll();
		} else if( key == KeyCode.KC_P ) {
			GetGame().GetUIManager().CloseAll();
		}
		return false;
	}
}
