#pragma once

#include <cstdint>

#include "../internal/WorldChrMan.hpp"

namespace players
{
enum class EquipGearSlot : uint32_t
{
    left_weapon1_id = 0,
    right_weapon1_id = 1,
    left_weapon2_id = 2,
    right_weapon2_id = 3,
    left_weapon3_id = 4,
    right_weapon3_id = 5,
    arrow1_id = 6,
    bolt1_id = 7,
    arrow2_id = 8,
    bolt2_id = 9,
    head_protector_id = 12,
    chest_protector_id = 13,
    arms_protector_id = 14,
    legs_protector_id = 15,
    accessory1_id = 17,
    accessory2_id = 18,
    accessory3_id = 19,
    accessory4_id = 20,
};

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
typedef void EquipGearFn(CS::EquipGameData *, EquipGearSlot slot, uint32_t *ga_item_id,
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

static ItemType get_item_type(EquipGearSlot slot)
{
    switch (slot)
    {
    case EquipGearSlot::left_weapon1_id:
    case EquipGearSlot::right_weapon1_id:
    case EquipGearSlot::left_weapon2_id:
    case EquipGearSlot::right_weapon2_id:
    case EquipGearSlot::left_weapon3_id:
    case EquipGearSlot::right_weapon3_id:
    case EquipGearSlot::arrow1_id:
    case EquipGearSlot::bolt1_id:
    case EquipGearSlot::arrow2_id:
    case EquipGearSlot::bolt2_id:
        return ItemType::weapon;
    case EquipGearSlot::head_protector_id:
    case EquipGearSlot::chest_protector_id:
    case EquipGearSlot::arms_protector_id:
    case EquipGearSlot::legs_protector_id:
        return ItemType::protector;
    case EquipGearSlot::accessory1_id:
    case EquipGearSlot::accessory2_id:
    case EquipGearSlot::accessory3_id:
    case EquipGearSlot::accessory4_id:
        return ItemType::accessory;
    default:
        return ItemType::invalid;
    }
};
};
