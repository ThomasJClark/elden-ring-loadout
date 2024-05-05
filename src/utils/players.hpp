#pragma once

#include "../from/WorldChrManImp.hpp"

namespace players
{
typedef int ApplySpEffectFn(from::CS::ChrIns *, unsigned int speffect_id, bool unk);
typedef int ClearSpEffectFn(from::CS::ChrIns *, unsigned int speffect_id);
typedef void SpawnOneShotVFXOnChrFn(from::CS::ChrIns *, int dummy_poly_id, int sfx_id, void *unk);
typedef void EquipGearFn(from::CS::EquipGameData *, unsigned int gear_slot,
                         unsigned int *ga_item_id, int index, bool, bool, bool);
typedef void EquipGoodsFn(from::CS::EquipGameData *, unsigned int slot, unsigned int *ga_item_id,
                          int index);

void initialize();
from::CS::PlayerIns *get_main_player();
bool has_item_in_inventory(unsigned int, int id);
bool has_speffect(from::CS::PlayerIns *, int speffect_id);
extern ApplySpEffectFn *apply_speffect;
extern ClearSpEffectFn *clear_speffect;
extern SpawnOneShotVFXOnChrFn *spawn_one_shot_sfx_on_chr;
extern EquipGearFn *equip_gear;
extern EquipGoodsFn *equip_goods;
};
