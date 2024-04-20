#include <algorithm>
#include <spdlog/spdlog.h>

#include "../messages/LoadoutMessages.hpp"
#include "../shop/LoadoutShop.hpp"
#include "../utils/ParamUtils.hpp"
#include "../utils/PlayerUtils.hpp"
#include "LoadoutSaveSlot.hpp"
#include "StringifyLoadout.hpp"

using namespace std;
using namespace erloadout;

array<saveslots::SaveSlot, saveslots::max_slots> erloadout::saveslots::slots;

static constexpr array<uint16_t, saveslots::max_slots> icon_ids = {
    361, 362, 363, 364, 365, 366, 367, 368,  369,  370,  371,  372,  373,
    374, 375, 377, 378, 379, 380, 359, 3504, 3505, 3508, 3531, 3538,
};

static constexpr array<uint16_t, saveslots::max_slots> empty_icon_ids = {
    653, 654, 655, 656, 657, 658, 659, 660,  661,  662,  663,  664, 665,
    666, 667, 669, 670, 671, 672, 651, 3704, 3705, 3708, 3732, 3739};

void saveslots::initialize()
{
    ShopLineupParam initial_shop_lineup_param = {.value = -1,
                                                 .mtrlId = -1,
                                                 .sellQuantity = -1,
                                                 .equipType = 2, // accessory
                                                 .setNum = 1,
                                                 .iconId = -1,
                                                 .nameMsgId = -1};

    EquipParamAccessory initial_accessory_param = {.sfxVariationId = -1,
                                                   .weight = 0,
                                                   .shopLv = -1,
                                                   .trophySGradeId = -1,
                                                   .trophySeqId = -1,
                                                   .vagrantItemLotId = -1,
                                                   .vagrantBonusEneDropItemLotId = -1,
                                                   .vagrantItemEneDropItemLotId = -1,
                                                   .isDeposit = 1,
                                                   .isDiscard = 1,
                                                   .isDrop = 1,
                                                   .showLogCondType = 1,
                                                   .showDialogCondType = 2,
                                                   .compTrophySedId = -1};

    int index = 0;
    for (auto &slot : slots)
    {
        slot = {
            .index = index,
            .empty = true,
            .gear = {.left_weapon1_id = -1,
                     .right_weapon1_id = -1,
                     .left_weapon2_id = -1,
                     .right_weapon2_id = -1,
                     .left_weapon3_id = -1,
                     .right_weapon3_id = -1,
                     .arrow1_id = -1,
                     .bolt1_id = -1,
                     .arrow2_id = -1,
                     .bolt2_id = -1,
                     .head_protector_id = -1,
                     .chest_protector_id = -1,
                     .arms_protector_id = -1,
                     .legs_protector_id = -1,
                     .accessory1_id = -1,
                     .accessory2_id = -1,
                     .accessory3_id = -1,
                     .accessory4_id = -1},
            .save_accessory_param = initial_accessory_param,
            .apply_accessory_param = initial_accessory_param,
            .save_shop_lineup_param = initial_shop_lineup_param,
            .apply_shop_lineup_param = initial_shop_lineup_param,
        };

        slot.save_shop_lineup_param.equipId = shop::save_loadout_accessory_base_id + index;
        slot.save_shop_lineup_param.nameMsgId = shop::save_loadout_accessory_base_id + index;
        slot.apply_shop_lineup_param.equipId = shop::apply_loadout_accessory_base_id + index;
        slot.apply_shop_lineup_param.equipId = shop::apply_loadout_accessory_base_id + index;

#if _DEBUG
        if (index == 0)
        {
            slot.empty = false;
            slot.gear.right_weapon1_id = 11050025; // Morning Star +25
            slot.gear.left_weapon1_id = 31340000;  // Black Leather Shield
            slot.gear.arrow1_id = 50020000;        // Serpent Arrow
            slot.gear.arrow2_id = 50040000;        // St. Trina's Arrow
            slot.gear.bolt1_id = 52030000;         // Black-Key Bolt
            slot.gear.head_protector_id = 1840000; // Foot Soldier Helm
            slot.gear.chest_protector_id = 290100; // Nox Monk Armor
            slot.gear.arms_protector_id = 630200;  // Astrologer Gloves
            slot.gear.legs_protector_id = 740300;  // Noble's Trousers
            slot.gear.accessory1_id = 1150;        // Green Turtle Talisman
            slot.gear.accessory2_id = 2160;        // Lord of Blood's Exultation
            slot.gear.accessory3_id = 2170;        // Kindred of Rot's Exultation
            slot.gear.accessory4_id = 1210;        // Bull-Goat's Talisman
        }
#endif

        index++;

        slot.refresh();
    }
}

void saveslots::SaveSlot::refresh()
{
    if (gear.right_weapon1_id == -1)
        gear.right_weapon1_id = unarmed_weapon_id;
    if (gear.right_weapon2_id == -1)
        gear.right_weapon2_id = unarmed_weapon_id;
    if (gear.right_weapon3_id == -1)
        gear.right_weapon3_id = unarmed_weapon_id;
    if (gear.left_weapon1_id == -1)
        gear.left_weapon1_id = unarmed_weapon_id;
    if (gear.left_weapon2_id == -1)
        gear.left_weapon2_id = unarmed_weapon_id;
    if (gear.left_weapon3_id == -1)
        gear.left_weapon3_id = unarmed_weapon_id;
    if (gear.head_protector_id == -1)
        gear.head_protector_id = bare_head_protector_id;
    if (gear.chest_protector_id == -1)
        gear.chest_protector_id = bare_chest_protector_id;
    if (gear.arms_protector_id == -1)
        gear.arms_protector_id = bare_arms_protector_id;
    if (gear.legs_protector_id == -1)
        gear.legs_protector_id = bare_legs_protector_id;

    if (empty)
    {
        name = msg::loadout_messages.empty_slot;
        info = L"<img src='img://MENU_FL_Box.png' width='246' height='232'/>";
        caption = L"-";

        save_accessory_param.iconId = empty_icon_ids[index];
        apply_accessory_param.iconId = empty_icon_ids[index];
        save_accessory_param.weight = 0;
        apply_accessory_param.weight = 0;
    }
    else
    {
        name = wstring(msg::loadout_messages.loadout) + L" " + to_wstring(index + 1);
        caption = stringify_loadout(*this);
        info = iconify_loadout(*this);

        save_accessory_param.iconId = icon_ids[index];
        apply_accessory_param.iconId = icon_ids[index];

        auto weight = 0.0f;

        auto equip_param_weapon = params::get_param<EquipParamWeapon>(L"EquipParamWeapon");
        for (auto weapon_id : {gear.left_weapon1_id, gear.right_weapon1_id, gear.left_weapon2_id,
                               gear.right_weapon2_id, gear.left_weapon3_id, gear.right_weapon3_id})
        {
            weight += equip_param_weapon[weapon_id - (weapon_id % 100)].weight;
        }

        auto equip_param_protector = params::get_param<EquipParamProtector>(L"EquipParamProtector");
        for (auto protector_id : {gear.head_protector_id, gear.chest_protector_id,
                                  gear.arms_protector_id, gear.legs_protector_id})
        {
            weight += equip_param_protector[protector_id].weight;
        }

        auto equip_param_accessory = params::get_param<EquipParamAccessory>(L"EquipParamAccessory");
        for (auto accessory_id :
             {gear.accessory1_id, gear.accessory2_id, gear.accessory3_id, gear.accessory4_id})
        {
            if (accessory_id != empty_accessory_id)
                weight += equip_param_accessory[accessory_id].weight;
        }

        save_accessory_param.weight = weight;
        apply_accessory_param.weight = weight;
    }
}

void saveslots::SaveSlot::save_from_player()
{
    auto main_player = players::get_main_player();
    if (main_player == nullptr)
    {
        spdlog::error("Can't save loadout, main player is null");
        return;
    }

    auto &chr_asm = main_player->player_game_data->equip_game_data.chr_asm;
    gear = chr_asm.gear;
    empty = false;

    spdlog::info("TODO: save slot {}", index);

    refresh();
}

void saveslots::SaveSlot::apply_to_player()
{
    auto main_player = players::get_main_player();
    if (main_player == nullptr || main_player->player_game_data == nullptr)
    {
        return;
    }

    auto &equip_game_data = main_player->player_game_data->equip_game_data;
    auto &inventory_entries = equip_game_data.equip_inventory_data.entries;

    auto find_inventory_index = [&inventory_entries](players::ItemType item_type, int32_t id) {
        for (int32_t i = 0; i < inventory_entries.count; i++)
            if (inventory_entries[i].item_id == (int32_t)item_type + id)
                return i;

        return -1;
    };

    auto equip_gear = [&equip_game_data, &inventory_entries,
                       find_inventory_index](players::EquipGearSlot slot, int32_t id) {
        auto item_type = players::get_item_type(slot);
        auto inventory_index = find_inventory_index(item_type, id);
        if (inventory_index == -1)
        {
            spdlog::info("Item type={:x} id={} not in player inventory", (int32_t)item_type, id);
            return;
        }

        auto ga_item_id = inventory_entries[inventory_index].ga_item_id;
        auto index = equip_game_data.equip_inventory_data.start_index + inventory_index;

        players::equip_gear(&equip_game_data, slot, &ga_item_id, index, true, true, false);
    };

    equip_gear(players::EquipGearSlot::left_weapon1_id, gear.left_weapon1_id);
    equip_gear(players::EquipGearSlot::right_weapon1_id, gear.right_weapon1_id);
    equip_gear(players::EquipGearSlot::left_weapon2_id, gear.left_weapon2_id);
    equip_gear(players::EquipGearSlot::right_weapon2_id, gear.right_weapon2_id);
    equip_gear(players::EquipGearSlot::left_weapon3_id, gear.left_weapon3_id);
    equip_gear(players::EquipGearSlot::right_weapon3_id, gear.right_weapon3_id);
    equip_gear(players::EquipGearSlot::arrow1_id, gear.arrow1_id);
    equip_gear(players::EquipGearSlot::bolt1_id, gear.bolt1_id);
    equip_gear(players::EquipGearSlot::arrow2_id, gear.arrow2_id);
    equip_gear(players::EquipGearSlot::bolt2_id, gear.bolt2_id);
    equip_gear(players::EquipGearSlot::head_protector_id, gear.head_protector_id);
    equip_gear(players::EquipGearSlot::chest_protector_id, gear.chest_protector_id);
    equip_gear(players::EquipGearSlot::arms_protector_id, gear.arms_protector_id);
    equip_gear(players::EquipGearSlot::legs_protector_id, gear.legs_protector_id);
    equip_gear(players::EquipGearSlot::accessory1_id, gear.accessory1_id);
    equip_gear(players::EquipGearSlot::accessory2_id, gear.accessory2_id);
    equip_gear(players::EquipGearSlot::accessory3_id, gear.accessory3_id);
    equip_gear(players::EquipGearSlot::accessory4_id, gear.accessory4_id);
}
