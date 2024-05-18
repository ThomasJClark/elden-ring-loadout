#pragma once

#include "../from/MapItemManImpl.hpp"
#include "../from/WorldChrManImp.hpp"

namespace players
{
void initialize();

struct item_gib_entry
{
    unsigned int item_id;
    int quantity;
    int unk;
    int gem_id;
};

struct item_gib_list
{
    int count{1};
    item_gib_entry entries[];
};

typedef void SpawnOneShotVFXOnChrFn(from::CS::ChrIns *, int dummy_poly_id, int sfx_id, void *unk);
typedef void EquipGearFn(from::CS::EquipGameData *, unsigned int gear_slot,
                         unsigned int *ga_item_id, int index, bool, bool, bool);
typedef void EquipGoodsFn(from::CS::EquipGameData *, unsigned int slot, unsigned int *ga_item_id,
                          int index);
typedef int GetInventoryIdFn(from::CS::EquipInventoryData *, int *item_id);
typedef void ItemGibFn(from::CS::MapItemManImpl *, item_gib_list *, void *, int unk);

extern SpawnOneShotVFXOnChrFn *spawn_one_shot_sfx_on_chr;
extern EquipGearFn *equip_gear;
extern EquipGoodsFn *equip_goods;
extern GetInventoryIdFn *get_inventory_id;
extern ItemGibFn *item_gib;
};
