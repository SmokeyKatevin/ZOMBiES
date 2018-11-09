modded class MissionGameplay
{
	//bool cam = false;
	vector oldPos = vector.Zero;
	FreeDebugCamera cam;
	private float m_SendUpdateTimer = 0.0;
	bool isHoldingLShift = false;
	bool isHoldingLControl = false;

	vector cursorPosition(PlayerBaseClient player) {
		vector contactPos, begPos;
		begPos = g_Game.GetCurrentCameraPosition();
		DayZPhysics.RaycastRV(begPos, begPos + g_Game.GetCurrentCameraDirection() * 5000, contactPos, NULL, NULL, NULL, NULL, player);
		return contactPos;
	}
	
	override void OnInit()
	{
		g_Game.SetProfileOption( EDayZProfilesOptions.GAME_MESSAGES, 0 );
		g_Game.SetProfileOption( EDayZProfilesOptions.ADMIN_MESSAGES, 0 );
		g_Game.SetProfileOption( EDayZProfilesOptions.PLAYER_MESSAGES, 0 );
		super.OnInit();
	}
	
    void ToggleCursor()
    {
        if ( GetGame().GetInput().HasGameFocus( INPUT_DEVICE_MOUSE ) )
        {
            GetGame().GetInput().ChangeGameFocus( 1 );
            GetGame().GetUIManager().ShowUICursor( true );
        }
        else
        {
            GetGame().GetUIManager().ShowUICursor( false );
            GetGame().GetInput().ResetGameFocus();
        }
    }

	override void OnKeyPress(int key)
	{
		super.OnKeyPress(key);

		PlayerBaseClient player = PlayerBaseClient.Cast(GetGame().GetPlayer());

		if (!player) {
			return;
		}

		vector position;
		ref Param2<vector, int> spawnParams;
		ref Param1<int> tpParam;

		if (player.m_isAdmin) {
			switch( key )
			{
				case KeyCode.KC_Y:
				{
					// Teleport
					if (cam && !GetUIManager().IsMenuOpen( MENU_ATOOL ) && isHoldingLShift) {
						oldPos = cursorPosition(player);
						player.RPCSingleParam(MRPCs.RPC_CLIENT_REQUEST_TELEPORT, new Param1<vector>(oldPos), false);

					// Update freecam bubble pos (tp player object under the freecam under the ground)
					} else if (cam && isHoldingLControl) {
						position = cam.GetPosition();
						position[1] = GetGame().SurfaceY(position[0], position[2]) - 3;

						player.RPCSingleParam(MRPCs.RPC_CLIENT_REQUEST_TELEPORT, new Param1<vector>(position), false);
					}
					break;
				}
				// Admin tool
				case KeyCode.KC_DELETE:
				{
					GetGame().GetUIManager().CloseAll();
					aTool menu = new aTool;
					menu.Init();
					menu.Init(player);
					GetGame().GetUIManager().ShowScriptedMenu( menu, NULL );
					GetGame().GetInput().ChangeGameFocus( 1 );
					GetGame().GetUIManager().ShowUICursor( true );
					break;
				}
				/*case KeyCode.KC_O:
				{
					ToggleCursor();
					break;
				}*/
				// Freecam toggle
				case KeyCode.KC_INSERT:
				{
					if (cam) {
						if (player.m_godMode) {
							player.m_godMode = false;
							tpParam = new Param1<int>(0);
							player.RPCSingleParam(MRPCs.RPC_CLIENT_REQUEST_GODMODE_TOGGLE, tpParam, true);
						}
						player.GetInputController().OverrideMovementSpeed( false, 0 );
						player.GetInputController().OverrideAimChangeX( false, 0 );
						player.GetInputController().OverrideAimChangeY( false, 0 );
						tpParam = new Param1<int>(0);
						player.RPCSingleParam(MRPCs.RPC_CLIENT_REQUEST_FREEZE, tpParam, true);

						GetGame().SelectPlayer(NULL, player);

						cam.SetActive(false);

						GetGame().ObjectDelete(cam);

						if (oldPos != vector.Zero) {
							player.RPCSingleParam(MRPCs.RPC_CLIENT_REQUEST_TELEPORT, new Param1<vector>(oldPos), false);
							oldPos = vector.Zero;
						}

						dBodyEnableGravity(player, true);
					} else {
						if (!player.m_godMode) {
							player.m_godMode = true;
							tpParam = new Param1<int>(0);
							player.RPCSingleParam(MRPCs.RPC_CLIENT_REQUEST_GODMODE_TOGGLE, tpParam, true);
						}

						oldPos = player.GetPosition();
						player.GetInputController().OverrideMovementSpeed( true, 0 );
						player.GetInputController().OverrideAimChangeX( true, 0 );
						player.GetInputController().OverrideAimChangeY( true, 0 );

						GetGame().SelectPlayer( NULL, NULL );

						cam = GetGame().CreateObject( "FreeDebugCamera", player.GetPosition(), true );

						cam.SetActive( true );

						tpParam = new Param1<int>(1);
						player.RPCSingleParam(MRPCs.RPC_CLIENT_REQUEST_FREEZE, tpParam, true);

						dBodyEnableGravity(player, false);
					}
					break;
				}
				// Spawn zombies / wolves
				case KeyCode.KC_END:
				{
					if (isHoldingLControl || isHoldingLShift) {
						position = cursorPosition(player);
						spawnParams = new Param2<vector, int>(position, 2);
						int rpcToSend = MRPCs.RPC_CLIENT_REQUEST_SPAWN_INFECTED;
						if (isHoldingLShift) {
							rpcToSend = MRPCs.RPC_CLIENT_REQUEST_SPAWN_WOLFIE;
						}
						player.RPCSingleParam(rpcToSend, spawnParams, false);
					}
					break;
				}
				// Check player name on cursor pos
				case KeyCode.KC_U:
				{
					if (cam && isHoldingLShift) {
						position = cursorPosition(player);
						array<Object> objects = new array<Object>;
						array<CargoBase> proxyCargos = new array<CargoBase>;

						GetGame().GetObjectsAtPosition(position, 3, objects, proxyCargos);
						
						bool found = false;
						for (int i = 0; (i < objects.Count() && !found); i++) {
							Object obj = objects.Get(i);
							if ( obj.IsInherited( Man ) && obj.IsAlive() ) {
								found = true;
								player.MessageStatus(PlayerBase.Cast(obj).GetIdentity().GetName());
							}
						}
					}
					break;
				}
			}
		}
		switch( key )
		{
			case KeyCode.KC_LSHIFT:
			{
				isHoldingLShift = true;
				break;
			}
			case KeyCode.KC_LCONTROL:
			{
				isHoldingLControl = true;
				break;
			}

			case KeyCode.KC_PERIOD:
			{
				if ( !GetUIManager().IsMenuOpen( MENU_GESTURES ) )
				{
					GesturesMenu.OpenMenu();
				}
				break;
			}
		}
	}

	override void OnKeyRelease(int key)
	{
		super.OnKeyRelease(key);

		switch( key )
		{
			case KeyCode.KC_LSHIFT:
			{
				isHoldingLShift = false;
				break;
			}
			case KeyCode.KC_LCONTROL:
			{
				isHoldingLControl = false;
				break;
			}

			case KeyCode.KC_PERIOD:
			{
				//close gestures menu
				if ( GetUIManager().IsMenuOpen( MENU_GESTURES ) )
				{
					//TODO reconnect when appropriate
					GesturesMenu.CloseMenu();
				}
				break;
			}
		}
	}

	// Ran on each tick, timeslice is in seconds
	override void OnUpdate(float timeslice)
	{
		super.OnUpdate(timeslice);
		// This PBC thing extends PB and houses a few variables and RPCs
		PlayerBaseClient player = PlayerBaseClient.Cast(GetGame().GetPlayer());

		// If player is in freecam (and is an admin)
		if( cam && player.m_isAdmin )
		{
			// And it's been 5 seconds since the last update
			if (m_SendUpdateTimer > 5) {
				vector pos = cam.GetPosition();
				// If player is far away from freecam pos, let's place the player object under the camera, under the ground
				if (vector.Distance(pos, player.GetPosition()) > 100) {
					pos[1] = GetGame().SurfaceY(pos[0], pos[2]) - 3;

					// Call rpc to update player position globally
					player.RPCSingleParam(MRPCs.RPC_CLIENT_REQUEST_TELEPORT, new Param1<vector>(pos), false);
					// Reset timer
					m_SendUpdateTimer = 0;
				}
			}
			m_SendUpdateTimer += timeslice;
		}
	}
}
