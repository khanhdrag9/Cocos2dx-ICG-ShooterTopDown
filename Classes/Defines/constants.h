#pragma once
#include <iostream>

namespace constants
{
    //Command
    const std::string command_move_by_none = "cmd_move_by_none";
    const std::string command_move_by_up = "cmd_move_by_up";
    const std::string command_move_by_down = "cmd_move_by_down";
    const std::string command_move_by_right = "cmd_move_by_right";
    const std::string command_move_by_left = "cmd_move_by_left";
    
    const std::string command_move_to = "cmd_move_to";
    
    const std::string commnad_shoot = "cmd_shoot";
    
    
    //Character
    const std::string character_none = "cha_none";
    const std::string character_player = "cha_player";
	const std::string character_enemy = "cha_enemy";
    
    
    //Object
    const std::string object_bullet_basic = "obj_bullet_basic";
    
}

namespace physics_code
{
	const int physics_player = 0x00000001;
	const int physics_enemy = 0x00000002;
	const int physics_edge = 0x00000003;
	const int physics_bullet_player = 0x00000006;
	const int physics_bullet_enemy = 0x00000004;
}
