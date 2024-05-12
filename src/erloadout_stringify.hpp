#pragma once

#include <string>

#include "erloadout_loadout.hpp"

namespace erloadout
{

/**
 * Convert a loadout to a markup string that can be displayed as an item description
 */
std::wstring stringify_loadout(loadout const &);

/**
 * Convert a loadout to markup that displays icons for the equipment in the loadout, as a quick
 * preview for the loadout menu
 */
std::wstring iconify_loadout(loadout const &);

}
