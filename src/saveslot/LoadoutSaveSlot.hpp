#pragma once

#include <array>
#include <string>
#include <tga/paramdefs.h>

namespace erloadout
{
namespace saveslots
{
static constexpr size_t max_slots = 25;

struct SaveSlot
{
    int index;

    // Equipment in this loadout
    int right_weapon_ids[3];
    int left_weapon_ids[3];
    int arrow_ids[2];
    int bolt_ids[2];
    int protector_ids[4];
    int accessory_ids[4];

    // Info derived from the above equipment
    bool empty;
    std::u16string name;
    std::u16string info;
    std::u16string caption;

    // Params used for the loadout shop
    EquipParamGoods save_goods_param;
    EquipParamGoods apply_goods_param;
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
