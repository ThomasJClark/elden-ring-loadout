#pragma once

#include <array>
#include <string>

#include <paramdefs/EQUIP_PARAM_ACCESSORY_ST.hpp>
#include <paramdefs/SHOP_LINEUP_PARAM.hpp>

#include "from/WorldChrManImp.hpp"

namespace erloadout
{

/**
 * A single loadout slot that can be applied to the player
 */
struct loadout
{
    int index;

    std::array<int, gear_slot::count> gear;

    // Info derived from the above equipment
    bool empty;
    std::wstring name;
    std::wstring info;
    std::wstring caption;

    // Params used for the loadout shop
    from::paramdefs::EQUIP_PARAM_ACCESSORY_ST save_accessory_param;
    from::paramdefs::EQUIP_PARAM_ACCESSORY_ST apply_accessory_param;
    from::paramdefs::SHOP_LINEUP_PARAM save_shop_lineup_param;
    from::paramdefs::SHOP_LINEUP_PARAM apply_shop_lineup_param;

    // Update the derived info (empty state & messages) for this loadout
    void refresh();
};

extern std::array<loadout, 25> loadouts;

void initialize_loadouts();

}
