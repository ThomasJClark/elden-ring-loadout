/**
 * erlaodout_to_player.hpp
 *
 * Functions for copying a saved loadout to a player's current gear and vice versa
 */
#pragma once

#include <string>

#include "erloadout_loadout.hpp"
#include "from/WorldChrManImp.hpp"

namespace erloadout
{

/**
 * Update a loadout slot to contain a player's current loadout
 */
void save_from_player(loadout &, from::CS::PlayerIns &);

/**
 * Update a player's current loadout based on a saved slot
 */
void apply_to_player(loadout const &, from::CS::PlayerIns &);

}
