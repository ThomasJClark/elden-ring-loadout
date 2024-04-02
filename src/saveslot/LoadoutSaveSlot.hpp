#pragma once

#include <array>
#include <string>
#include <tga/paramdefs.h>

namespace erloadout
{
namespace saveslots
{
static constexpr size_t max_slots = 25;

static constexpr int32_t icon_id_empty_slot = 249;
static constexpr int32_t icon_id_slot = 250;

static constexpr int32_t bare_head_protector_id = 10000;
static constexpr int32_t bare_chest_protector_id = 10100;
static constexpr int32_t bare_arms_protector_id = 10200;
static constexpr int32_t bare_legs_protector_id = 10300;
static constexpr int32_t unarmed_weapon_id = 110000;
static constexpr int32_t empty_ammo_id = -1;
static constexpr int32_t empty_accessory_id = -1;

struct SaveSlot
{
    int index;

    // Equipment in this loadout
    bool empty;
    int right_weapon_ids[3];
    int left_weapon_ids[3];
    int arrow_id1;
    int arrow_id2;
    int bolt_id1;
    int bolt_id2;
    int head_protector_id;
    int chest_protector_id;
    int arms_protector_id;
    int legs_protector_id;
    int accessory_ids[4];

    // Info derived from the above equipment
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
