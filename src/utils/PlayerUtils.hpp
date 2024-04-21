#pragma once

#include <cstdint>

#include "../internal/WorldChrMan.hpp"

namespace players
{
enum class ItemType : uint32_t
{
    weapon = 0,
    protector = 0x10000000,
    accessory = 0x20000000,
    unused = 0x30000000,
    goods = 0x40000000,
    gem = 0x50000000,
    invalid = 0xffffffff
};

typedef int32_t ApplySpEffectFn(CS::ChrIns *, uint32_t speffect_id, bool unk);
typedef int32_t ClearSpEffectFn(CS::ChrIns *, uint32_t speffect_id);
typedef void SpawnOneShotVFXOnChrFn(CS::ChrIns *, int32_t dummy_poly_id, int32_t sfx_id, void *unk);
typedef void EquipGearFn(CS::EquipGameData *, uint32_t gear_slot, uint32_t *ga_item_id,
                         int32_t index, bool, bool, bool);
typedef void EquipGoodsFn(CS::EquipGameData *, uint32_t slot, uint32_t *ga_item_id, int32_t index);

void initialize();
CS::PlayerIns *get_main_player();
bool has_item_in_inventory(ItemType, int32_t id);
bool has_speffect(CS::PlayerIns *, int32_t speffect_id);
extern ApplySpEffectFn *apply_speffect;
extern ClearSpEffectFn *clear_speffect;
extern SpawnOneShotVFXOnChrFn *spawn_one_shot_sfx_on_chr;
extern EquipGearFn *equip_gear;
extern EquipGoodsFn *equip_goods;

static ItemType get_item_type(uint32_t gear_slot)
{
    switch (gear_slot)
    {
    case gear_slot::left_weapon1_id:
    case gear_slot::right_weapon1_id:
    case gear_slot::left_weapon2_id:
    case gear_slot::right_weapon2_id:
    case gear_slot::left_weapon3_id:
    case gear_slot::right_weapon3_id:
    case gear_slot::arrow1_id:
    case gear_slot::bolt1_id:
    case gear_slot::arrow2_id:
    case gear_slot::bolt2_id:
        return ItemType::weapon;
    case gear_slot::head_protector_id:
    case gear_slot::chest_protector_id:
    case gear_slot::arms_protector_id:
    case gear_slot::legs_protector_id:
        return ItemType::protector;
    case gear_slot::accessory1_id:
    case gear_slot::accessory2_id:
    case gear_slot::accessory3_id:
    case gear_slot::accessory4_id:
        return ItemType::accessory;
    default:
        return ItemType::invalid;
    }
};
};
