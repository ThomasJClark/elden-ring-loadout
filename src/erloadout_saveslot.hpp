#pragma once

#include <array>
#include <string>
#include <tga/paramdefs.h>

#include "internal/WorldChrMan.hpp"
#include "utils/players.hpp"

namespace erloadout
{
namespace saveslots
{
static constexpr size_t max_slots = 25;

static constexpr int32_t bare_head_protector_id = 10000;
static constexpr int32_t bare_chest_protector_id = 10100;
static constexpr int32_t bare_arms_protector_id = 10200;
static constexpr int32_t bare_legs_protector_id = 10300;
static constexpr int32_t unarmed_weapon_id = 110000;
static constexpr int32_t empty_ammo_id = -1;
static constexpr int32_t empty_accessory_id = -1;

extern std::array<int32_t, gear_slot::count> default_gear;

struct SaveSlot
{
    int index;
    bool empty;
    std::array<int32_t, gear_slot::count> gear;

    // Info derived from the above equipment
    std::wstring name;
    std::wstring info;
    std::wstring caption;

    // Params used for the loadout shop
    EquipParamAccessory save_accessory_param;
    EquipParamAccessory apply_accessory_param;
    ShopLineupParam save_shop_lineup_param;
    ShopLineupParam apply_shop_lineup_param;

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
