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

array<saveslots::SaveSlot, saveslots::max_slots> saveslots::slots;

static constexpr array<uint16_t, saveslots::max_slots> icon_ids = {
    361, 362, 363, 364, 365, 366, 367, 368,  369,  370,  371,  372,  373,
    374, 375, 377, 378, 379, 380, 359, 3504, 3505, 3508, 3531, 3538,
};

static constexpr array<uint16_t, saveslots::max_slots> empty_icon_ids = {
    653, 654, 655, 656, 657, 658, 659, 660,  661,  662,  663,  664, 665,
    666, 667, 669, 670, 671, 672, 651, 3704, 3705, 3708, 3732, 3739};

array<int32_t, gear_slot::count> saveslots::default_gear;

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

    default_gear[gear_slot::right_weapon1_id] = unarmed_weapon_id;
    default_gear[gear_slot::right_weapon2_id] = unarmed_weapon_id;
    default_gear[gear_slot::right_weapon3_id] = unarmed_weapon_id;
    default_gear[gear_slot::left_weapon1_id] = unarmed_weapon_id;
    default_gear[gear_slot::left_weapon2_id] = unarmed_weapon_id;
    default_gear[gear_slot::left_weapon3_id] = unarmed_weapon_id;
    default_gear[gear_slot::arrow1_id] = empty_ammo_id;
    default_gear[gear_slot::arrow2_id] = empty_ammo_id;
    default_gear[gear_slot::bolt1_id] = empty_ammo_id;
    default_gear[gear_slot::bolt2_id] = empty_ammo_id;
    default_gear[gear_slot::head_protector_id] = bare_head_protector_id;
    default_gear[gear_slot::chest_protector_id] = bare_chest_protector_id;
    default_gear[gear_slot::arms_protector_id] = bare_arms_protector_id;
    default_gear[gear_slot::legs_protector_id] = bare_legs_protector_id;
    default_gear[gear_slot::accessory1_id] = empty_accessory_id;
    default_gear[gear_slot::accessory2_id] = empty_accessory_id;
    default_gear[gear_slot::accessory3_id] = empty_accessory_id;
    default_gear[gear_slot::accessory4_id] = empty_accessory_id;
    default_gear[gear_slot::unk1] = -1;
    default_gear[gear_slot::unk2] = -1;
    default_gear[gear_slot::unk3] = -1;
    default_gear[gear_slot::unk4] = -1;

    int index = 0;
    for (auto &slot : slots)
    {
        slot = {
            .index = index,
            .empty = true,
            .gear = {0},
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
            slot.gear[gear_slot::right_weapon1_id] = 11050025; // Morning Star +25
            slot.gear[gear_slot::left_weapon1_id] = 31340000;  // Black Leather Shield
            slot.gear[gear_slot::arrow1_id] = 50020000;        // Serpent Arrow
            slot.gear[gear_slot::arrow2_id] = 50040000;        // St. Trina's Arrow
            slot.gear[gear_slot::bolt1_id] = 52030000;         // Black-Key Bolt
            slot.gear[gear_slot::head_protector_id] = 1840000; // Foot Soldier Helm
            slot.gear[gear_slot::chest_protector_id] = 290100; // Nox Monk Armor
            slot.gear[gear_slot::arms_protector_id] = 630200;  // Astrologer Gloves
            slot.gear[gear_slot::legs_protector_id] = 740300;  // Noble's Trousers
            slot.gear[gear_slot::accessory1_id] = 1150;        // Green Turtle Talisman
            slot.gear[gear_slot::accessory2_id] = 2160;        // Lord of Blood's Exultation
            slot.gear[gear_slot::accessory3_id] = 2170;        // Kindred of Rot's Exultation
            slot.gear[gear_slot::accessory4_id] = 1210;        // Bull-Goat's Talisman
        }
#endif

        index++;

        slot.refresh();
    }
}

void saveslots::SaveSlot::refresh()
{
    empty = true;

    // Set any empty gear to the default ID for that gear slot (e.g. bare fist for weapon slots)
    // and check if the save slot is empty overall
    for (auto i = 0; i < gear.size(); i++)
    {
        if (gear[i] <= 0)
        {
            gear[i] = default_gear[i];
        }

        if (gear[i] != default_gear[i])
        {
            empty = false;
        }
    }

    if (empty)
    {
        // If there's no gear saved to this slot, set a default empty name and caption
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
        // Otherwise, set the name based on the slot number, and the caption and info based on
        // the saved gear
        name = wstring(msg::loadout_messages.loadout) + L" " + to_wstring(index + 1);
        caption = stringify_loadout(*this);
        info = iconify_loadout(*this);

        save_accessory_param.iconId = icon_ids[index];
        apply_accessory_param.iconId = icon_ids[index];

        // Count the total weight of the gear in this save slot, to display in the loadout shop
        auto weight = 0.0f;

        auto equip_param_weapon = params::get_param<EquipParamWeapon>(L"EquipParamWeapon");
        for (auto gear_slot :
             {gear_slot::left_weapon1_id, gear_slot::right_weapon1_id, gear_slot::left_weapon2_id,
              gear_slot::right_weapon2_id, gear_slot::left_weapon3_id, gear_slot::right_weapon3_id})
        {
            auto weapon_id = gear[gear_slot];
            weight += equip_param_weapon[weapon_id - (weapon_id % 100)].weight;
        }

        auto equip_param_protector = params::get_param<EquipParamProtector>(L"EquipParamProtector");
        for (auto gear_slot : {gear_slot::head_protector_id, gear_slot::chest_protector_id,
                               gear_slot::arms_protector_id, gear_slot::legs_protector_id})
        {
            auto protector_id = gear[gear_slot];
            weight += equip_param_protector[protector_id].weight;
        }

        auto equip_param_accessory = params::get_param<EquipParamAccessory>(L"EquipParamAccessory");
        for (auto gear_slot : {gear_slot::accessory1_id, gear_slot::accessory2_id,
                               gear_slot::accessory3_id, gear_slot::accessory4_id})
        {
            auto accessory_id = gear[gear_slot];
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
    copy(begin(chr_asm.gear), end(chr_asm.gear), begin(gear));
    empty = false;

    spdlog::info("TODO: save slot {}", index + 1);

    refresh();
}

void saveslots::SaveSlot::apply_to_player()
{
    auto main_player = players::get_main_player();
    if (main_player == nullptr || main_player->player_game_data == nullptr)
    {
        spdlog::error("Can't apply loadout, main player is null");
        return;
    }

    // spdlog::info("Applying [{}] to player", name);

    auto &equip_game_data = main_player->player_game_data->equip_game_data;
    auto &inventory_entries = equip_game_data.equip_inventory_data.entries;

    auto find_inventory_index = [&inventory_entries](players::ItemType item_type, int32_t id) {
        for (int32_t i = 0; i < inventory_entries.count; i++)
            if (inventory_entries[i].item_id == (int32_t)item_type + id)
                return i;

        return -1;
    };

    auto equip_gear = [&equip_game_data, &inventory_entries, find_inventory_index](uint32_t slot,
                                                                                   int32_t id) {
        auto item_type = players::get_item_type(slot);
        auto inventory_index = find_inventory_index(item_type, id);
        if (inventory_index == -1)
        {
            spdlog::info("Item type {:x} id {} not in player inventory", (int32_t)item_type, id);
            return;
        }

        auto ga_item_id = inventory_entries[inventory_index].ga_item_id;
        auto index = equip_game_data.equip_inventory_data.start_index + inventory_index;

        players::equip_gear(&equip_game_data, slot, &ga_item_id, index, true, true, false);
    };

    equip_gear(gear_slot::left_weapon1_id, gear[gear_slot::left_weapon1_id]);
    equip_gear(gear_slot::right_weapon1_id, gear[gear_slot::right_weapon1_id]);
    equip_gear(gear_slot::left_weapon2_id, gear[gear_slot::left_weapon2_id]);
    equip_gear(gear_slot::right_weapon2_id, gear[gear_slot::right_weapon2_id]);
    equip_gear(gear_slot::left_weapon3_id, gear[gear_slot::left_weapon3_id]);
    equip_gear(gear_slot::right_weapon3_id, gear[gear_slot::right_weapon3_id]);
    equip_gear(gear_slot::arrow1_id, gear[gear_slot::arrow1_id]);
    equip_gear(gear_slot::bolt1_id, gear[gear_slot::bolt1_id]);
    equip_gear(gear_slot::arrow2_id, gear[gear_slot::arrow2_id]);
    equip_gear(gear_slot::bolt2_id, gear[gear_slot::bolt2_id]);
    equip_gear(gear_slot::head_protector_id, gear[gear_slot::head_protector_id]);
    equip_gear(gear_slot::chest_protector_id, gear[gear_slot::chest_protector_id]);
    equip_gear(gear_slot::arms_protector_id, gear[gear_slot::arms_protector_id]);
    equip_gear(gear_slot::legs_protector_id, gear[gear_slot::legs_protector_id]);
    equip_gear(gear_slot::accessory1_id, gear[gear_slot::accessory1_id]);
    equip_gear(gear_slot::accessory2_id, gear[gear_slot::accessory2_id]);
    equip_gear(gear_slot::accessory3_id, gear[gear_slot::accessory3_id]);
    equip_gear(gear_slot::accessory4_id, gear[gear_slot::accessory4_id]);

    // Show a cool effect on the player
    players::spawn_one_shot_sfx_on_chr(main_player, 900, 8020, nullptr);
}
