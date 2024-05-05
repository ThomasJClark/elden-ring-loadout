#pragma once

#include <array>
#include <string>

#include <paramdefs/EQUIP_PARAM_ACCESSORY_ST.hpp>
#include <paramdefs/SHOP_LINEUP_PARAM.hpp>

#include "from/WorldChrManImp.hpp"
#include "utils/players.hpp"

namespace erloadout
{
namespace saveslots
{
static constexpr size_t max_slots = 25;

static constexpr int bare_head_protector_id = 10000;
static constexpr int bare_chest_protector_id = 10100;
static constexpr int bare_arms_protector_id = 10200;
static constexpr int bare_legs_protector_id = 10300;
static constexpr int unarmed_weapon_id = 110000;
static constexpr int empty_ammo_id = -1;
static constexpr int empty_accessory_id = -1;

extern std::array<int, gear_slot::count> default_gear;

struct SaveSlot
{
    int index;
    bool empty;
    std::array<int, gear_slot::count> gear;

    // Info derived from the above equipment
    std::wstring name;
    std::wstring info;
    std::wstring caption;

    // Params used for the loadout shop
    from::paramdefs::EQUIP_PARAM_ACCESSORY_ST save_accessory_param;
    from::paramdefs::EQUIP_PARAM_ACCESSORY_ST apply_accessory_param;
    from::paramdefs::SHOP_LINEUP_PARAM save_shop_lineup_param;
    from::paramdefs::SHOP_LINEUP_PARAM apply_shop_lineup_param;

    // Update the slot to contain the player's current equipment
    void save_from_player();

    // Update the player's equipment with the contents of this loadout
    void apply_to_player();

    // Update the derived info (empty state & messages) for this loadout
    void refresh();
};

extern std::array<SaveSlot, max_slots> slots;

void initialize();
}
}
