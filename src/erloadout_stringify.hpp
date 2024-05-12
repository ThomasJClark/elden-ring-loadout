#pragma once

#include <string>

#include "erloadout_loadout.hpp"

namespace erloadout
{
std::wstring stringify_loadout(loadouts::loadout const &);
std::wstring iconify_loadout(loadouts::loadout const &);
}
