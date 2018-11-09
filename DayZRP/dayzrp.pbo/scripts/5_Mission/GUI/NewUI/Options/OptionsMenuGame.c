modded class OptionsMenuGame
{
	void OptionsMenuGame( Widget parent, Widget details_root, GameOptions options, OptionsMenu menu )
	{
		Widget chat;
		chat = m_Root.FindAnyWidget( "game_setting_option" );
		chat.Enable(false);
		chat.Show(false);
		chat = m_Root.FindAnyWidget( "admin_setting_option" );
		chat.Enable(false);
		chat.Show(false);
		chat = m_Root.FindAnyWidget( "player_setting_option" );
		chat.Enable(false);
		chat.Show(false);
	}

	override void UpdateGameOption( int new_index )
	{
		//m_Menu.OnChanged();
	}
	
	override void UpdateAdminOption( int new_index )
	{
		//m_Menu.OnChanged();
	}
	
	override void UpdatePlayerOption( int new_index )
	{
		//m_Menu.OnChanged();
	}
}
