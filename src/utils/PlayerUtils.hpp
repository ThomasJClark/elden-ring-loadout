#pragma once

#include <cstdint>

#include "../internal/WorldChrMan.hpp"

namespace players
{
typedef int32_t ApplySpEffectFn(CS::ChrIns *, uint32_t speffect_id, bool unk);
typedef int32_t ClearSpEffectFn(CS::ChrIns *, uint32_t speffect_id);
typedef void SpawnOneShotVFXOnChrFn(CS::ChrIns *, int32_t dummy_poly_id, int32_t sfx_id, void *unk);
typedef void EquipGearFn(CS::EquipGameData *, uint32_t gear_slot, uint32_t *ga_item_id,
                         int32_t index, bool, bool, bool);
typedef void EquipGoodsFn(CS::EquipGameData *, uint32_t slot, uint32_t *ga_item_id, int32_t index);

void initialize();
CS::PlayerIns *get_main_player();
bool has_item_in_inventory(uint32_t, int32_t id);
bool has_speffect(CS::PlayerIns *, int32_t speffect_id);
extern ApplySpEffectFn *apply_speffect;
extern ClearSpEffectFn *clear_speffect;
extern SpawnOneShotVFXOnChrFn *spawn_one_shot_sfx_on_chr;
extern EquipGearFn *equip_gear;
extern EquipGoodsFn *equip_goods;
};
