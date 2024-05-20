#pragma once

#include <array>
#include <string>
#include <vector>

#include <paramdefs/EQUIP_PARAM_ACCESSORY_ST.hpp>
#include <paramdefs/SHOP_LINEUP_PARAM.hpp>

#include "erloadout_ids.hpp"
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

void refresh_loadouts();

loadout &get_loadout(size_t pos);

extern std::vector<std::array<loadout, loadout_count>> loadout_books;
}
