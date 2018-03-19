

enum GameMessages
{
    ID_SPAWN_PLAYER         = ID_USER_PACKET_ENUM + 1,
    ID_LOAD_CURRENT_PLAYERS = ID_USER_PACKET_ENUM + 2,
    ID_RE_SYNC_PLAYERS      = ID_USER_PACKET_ENUM + 3,
    ID_SEND_KEY_PRESS       = ID_USER_PACKET_ENUM + 4,
    ID_PLAYER_MOVE          = ID_USER_PACKET_ENUM + 5,
    ID_PLAYER_DISCONNECT    = ID_USER_PACKET_ENUM + 6,
    ID_REFRESH_SERVER       = ID_USER_PACKET_ENUM + 7,
    ID_PLAYER_STATE         = ID_USER_PACKET_ENUM + 8,
    ID_PLAYER_REFRESH       = ID_USER_PACKET_ENUM + 9,
    ID_PLAYER_THROW_OBJECT  = ID_USER_PACKET_ENUM + 10,
    ID_LOAD_CURRENT_CLIENTS = ID_USER_PACKET_ENUM + 11,
	ID_RACE_START			= ID_USER_PACKET_ENUM + 12,
    ID_CHANGE_CHARACTER     = ID_USER_PACKET_ENUM + 13,
    ID_READY_CLIENT         = ID_USER_PACKET_ENUM + 14,
    ID_RETURN_LOBBY         = ID_USER_PACKET_ENUM + 15,
    ID_CHECK_ALREADY_CONNECTED = ID_USER_PACKET_ENUM + 16
};