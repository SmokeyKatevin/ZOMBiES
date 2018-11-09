modded class PlayerBaseClient
{
	bool m_godMode = false;
	bool m_isInvisible = false;
	bool m_isAdmin = false;

	void PlayerBaseClient() {
	}

	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
	{
		super.OnRPC(sender, rpc_type, ctx);

		PlayerBaseClient player = this;

		switch( rpc_type )
		{
			// This sets the player invisible on A client
			case MRPCs.RPC_SERVER_REQUEST_INVISIBILITY:
			{
				ref Param1<int> paraminv = new Param1<int>(0);

				if (ctx.Read(paraminv)) {
					int toggle = paraminv.param1;
					//PlayerBase foundPlayer = paraminv.param2;

					if (player) {
						if (toggle == 1) {
							//Print(string.Format("Setting %1 invisible via the hands of %2 on client.", player.GetIdentity().GetPlainId(), sender.GetPlainId()));
							player.SetInvisible(true);

							player.ClearFlags(EntityFlags.VISIBLE, true);
							player.ClearEventMask(EntityEvent.VISIBLE );
						} else {
							player.SetInvisible(false);

							player.SetFlags(EntityFlags.VISIBLE, true);
							player.SetEventMask( EntityEvent.VISIBLE );
						}
					}
				}
				break;
			}
			case MRPCs.RPC_SERVER_REQUEST_ADMINITY:
			{
				if (!GetGame().IsServer()) {
					player.m_isAdmin = true;
				}
				break;
			}
			case MRPCs.RPC_SERVER_MSG:
			{
				ref Param1<string> parammsg = new Param1<string>("");

				if (ctx.Read(parammsg)) {
					string msg = parammsg.param1;
					player.MessageStatus(msg);
				}
				break;
			}
			case MRPCs.RPC_SERVER_RECEIVE_CAM_POS:
			{
				ref Param1<vector> parampos = new Param1<vector>(vector.Zero);

				if (ctx.Read(parampos)) {
					vector position = parampos.param1;
					Camera.GetCurrentCamera().SetPosition(position);
				}
				break;
			}
		}
	}
}
