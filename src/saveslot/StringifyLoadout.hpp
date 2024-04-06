#pragma once

#include <string>

#include "LoadoutSaveSlot.hpp"

namespace erloadout
{
namespace saveslots
{
std::wstring stringify_loadout(SaveSlot const &);
std::wstring iconify_loadout(SaveSlot const &);
}
}
