#include <algorithm>

#include <coresystem/param.hpp>
#include <paramdefs/EQUIP_PARAM_ACCESSORY_ST.hpp>
#include <paramdefs/EQUIP_PARAM_PROTECTOR_ST.hpp>
#include <paramdefs/EQUIP_PARAM_WEAPON_ST.hpp>
#include <spdlog/spdlog.h>

#include "erloadout_messages.hpp"
#include "erloadout_saveslot.hpp"
#include "erloadout_shop.hpp"
#include "erloadout_stringify.hpp"
#include "from/WorldChrManImp.hpp"
#include "utils/players.hpp"

using namespace std;
using namespace erloadout;

static inline unsigned int get_item_type(unsigned int item_id)
{
    return item_id & 0x70000000;
}

static inline std::pair<unsigned int, unsigned int> get_weapon_id_upgrade_level(
    unsigned int weapon_id)
{
    auto upgrade_level = weapon_id % 100;
    return {weapon_id - upgrade_level, upgrade_level};
}

array<saveslots::SaveSlot, saveslots::max_slots> saveslots::slots;

static constexpr array<unsigned short, saveslots::max_slots> icon_ids = {
    361, 362, 363, 364, 365, 366, 367, 368,  369,  370,  371,  372,  373,
    374, 375, 377, 378, 379, 380, 359, 3504, 3505, 3508, 3531, 3538,
};

static constexpr array<unsigned short, saveslots::max_slots> empty_icon_ids = {
    653, 654, 655, 656, 657, 658, 659, 660,  661,  662,  663,  664, 665,
    666, 667, 669, 670, 671, 672, 651, 3704, 3705, 3708, 3732, 3739};

array<int, gear_slot::count> saveslots::default_gear;

static array<unsigned int, gear_slot::count> gear_item_types;
static array<string, gear_slot::count> gear_slot_debug_names;

void saveslots::initialize()
{
    from::paramdefs::SHOP_LINEUP_PARAM initial_shop_lineup_param = {.value = -1,
                                                                    .mtrlId = -1,
                                                                    .sellQuantity = -1,
                                                                    .equipType = 2, // accessory
                                                                    .setNum = 1,
                                                                    .value_Magnification = 0,
                                                                    .iconId = -1,
                                                                    .nameMsgId = -1,
                                                                    .menuTitleMsgId = 0,
                                                                    .menuIconId = 0};

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

    gear_item_types[gear_slot::left_weapon1_id] = item_type::weapon;
    gear_item_types[gear_slot::right_weapon1_id] = item_type::weapon;
    gear_item_types[gear_slot::left_weapon2_id] = item_type::weapon;
    gear_item_types[gear_slot::right_weapon2_id] = item_type::weapon;
    gear_item_types[gear_slot::left_weapon3_id] = item_type::weapon;
    gear_item_types[gear_slot::right_weapon3_id] = item_type::weapon;
    gear_item_types[gear_slot::arrow1_id] = item_type::weapon;
    gear_item_types[gear_slot::bolt1_id] = item_type::weapon;
    gear_item_types[gear_slot::arrow2_id] = item_type::weapon;
    gear_item_types[gear_slot::bolt2_id] = item_type::weapon;
    gear_item_types[gear_slot::head_protector_id] = item_type::protector;
    gear_item_types[gear_slot::chest_protector_id] = item_type::protector;
    gear_item_types[gear_slot::arms_protector_id] = item_type::protector;
    gear_item_types[gear_slot::legs_protector_id] = item_type::protector;
    gear_item_types[gear_slot::accessory1_id] = item_type::accessory;
    gear_item_types[gear_slot::accessory2_id] = item_type::accessory;
    gear_item_types[gear_slot::accessory3_id] = item_type::accessory;
    gear_item_types[gear_slot::accessory4_id] = item_type::accessory;
    gear_item_types[gear_slot::unk1] = item_type::invalid;
    gear_item_types[gear_slot::unk2] = item_type::invalid;
    gear_item_types[gear_slot::unk3] = item_type::invalid;
    gear_item_types[gear_slot::unk4] = item_type::invalid;

    gear_slot_debug_names[gear_slot::left_weapon1_id] = "left_weapon1";
    gear_slot_debug_names[gear_slot::right_weapon1_id] = "right_weapon1";
    gear_slot_debug_names[gear_slot::left_weapon2_id] = "left_weapon2";
    gear_slot_debug_names[gear_slot::right_weapon2_id] = "right_weapon2";
    gear_slot_debug_names[gear_slot::left_weapon3_id] = "left_weapon3";
    gear_slot_debug_names[gear_slot::right_weapon3_id] = "right_weapon3";
    gear_slot_debug_names[gear_slot::arrow1_id] = "arrow1";
    gear_slot_debug_names[gear_slot::bolt1_id] = "bolt1";
    gear_slot_debug_names[gear_slot::arrow2_id] = "arrow2";
    gear_slot_debug_names[gear_slot::bolt2_id] = "bolt2";
    gear_slot_debug_names[gear_slot::head_protector_id] = "head_protector";
    gear_slot_debug_names[gear_slot::chest_protector_id] = "chest_protector";
    gear_slot_debug_names[gear_slot::arms_protector_id] = "arms_protector";
    gear_slot_debug_names[gear_slot::legs_protector_id] = "legs_protector";
    gear_slot_debug_names[gear_slot::accessory1_id] = "accessory1";
    gear_slot_debug_names[gear_slot::accessory2_id] = "accessory2";
    gear_slot_debug_names[gear_slot::accessory3_id] = "accessory3";
    gear_slot_debug_names[gear_slot::accessory4_id] = "accessory4";
    gear_slot_debug_names[gear_slot::unk1] = "unk1";
    gear_slot_debug_names[gear_slot::unk2] = "unk2";
    gear_slot_debug_names[gear_slot::unk3] = "unk3";
    gear_slot_debug_names[gear_slot::unk4] = "unk4";

    int index = 0;
    for (auto &slot : slots)
    {
        slot = {
            .index = index,
            .empty = true,
            .gear = {0},
            .save_accessory_param = {.saleValue = 0},
            .apply_accessory_param = {.saleValue = 0},
            .save_shop_lineup_param = initial_shop_lineup_param,
            .apply_shop_lineup_param = initial_shop_lineup_param,
        };

        slot.save_shop_lineup_param.equipId = shop::save_loadout_accessory_base_id + index;
        slot.save_shop_lineup_param.nameMsgId = shop::save_loadout_accessory_base_id + index;
        slot.apply_shop_lineup_param.equipId = shop::apply_loadout_accessory_base_id + index;
        slot.apply_shop_lineup_param.equipId = shop::apply_loadout_accessory_base_id + index;

        index++;
    }

    slots[0].gear[gear_slot::right_weapon1_id] = 11050025; // Morning Star +25
    slots[0].gear[gear_slot::left_weapon1_id] = 31340000;  // Black Leather Shield
    slots[0].gear[gear_slot::arrow1_id] = 50020000;        // Serpent Arrow
    slots[0].gear[gear_slot::arrow2_id] = 50040000;        // St. Trina's Arrow
    slots[0].gear[gear_slot::bolt1_id] = 52030000;         // Black-Key Bolt
    slots[0].gear[gear_slot::head_protector_id] = 1840000; // Foot Soldier Helm
    slots[0].gear[gear_slot::chest_protector_id] = 290100; // Nox Monk Armor
    slots[0].gear[gear_slot::arms_protector_id] = 630200;  // Astrologer Gloves
    slots[0].gear[gear_slot::legs_protector_id] = 740300;  // Noble's Trousers
    slots[0].gear[gear_slot::accessory1_id] = 1150;        // Green Turtle Talisman
    slots[0].gear[gear_slot::accessory2_id] = 2160;        // Lord of Blood's Exultation
    slots[0].gear[gear_slot::accessory3_id] = 2170;        // Kindred of Rot's Exultation
    slots[0].gear[gear_slot::accessory4_id] = 1210;        // Bull-Goat's Talisman

    slots[1].gear[gear_slot::right_weapon1_id] = 23050010; // Axe of Godfrey +10
    slots[1].gear[gear_slot::left_weapon1_id] = 32130025;  // Fingerprint Stone Shield +25
    slots[1].gear[gear_slot::head_protector_id] = 140000;  // Bull-Goat Helm
    slots[1].gear[gear_slot::chest_protector_id] = 140100; // Bull-Goat Armor
    slots[1].gear[gear_slot::arms_protector_id] = 140200;  // Bull-Goat Gauntlets
    slots[1].gear[gear_slot::legs_protector_id] = 140300;  // Bull-Goat Greaves
    slots[1].gear[gear_slot::accessory1_id] = 4100;        // Greatshield Talisman
    slots[1].gear[gear_slot::accessory2_id] = 2200;        // Curved Sword Talisman
    slots[1].gear[gear_slot::accessory3_id] = 2050;        // Ritual Sword Talisman
    slots[1].gear[gear_slot::accessory4_id] = 4003;        // Dragoncrest Greatshield Talisman
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

        auto params_ref = from::CS::SoloParamRepositoryImp::instance();
        if (!params_ref)
        {
            spdlog::error("No CS::SoloParamRepositoryImp instance");
            return;
        }
        auto &params = params_ref.reference();

        for (auto gear_slot :
             {gear_slot::left_weapon1_id, gear_slot::right_weapon1_id, gear_slot::left_weapon2_id,
              gear_slot::right_weapon2_id, gear_slot::left_weapon3_id, gear_slot::right_weapon3_id})
        {
            auto weapon_id = gear[gear_slot];
            auto weapon =
                params.get_equip_param_weapon()->get_row_by_id(weapon_id - (weapon_id % 100));
            if (weapon != nullptr)
                weight += weapon->weight;
        }

        for (auto gear_slot : {gear_slot::head_protector_id, gear_slot::chest_protector_id,
                               gear_slot::arms_protector_id, gear_slot::legs_protector_id})
        {
            auto protector_id = gear[gear_slot];
            auto protector = params.get_equip_param_protector()->get_row_by_id(protector_id);
            if (protector)
                weight += protector->weight;
        }

        for (auto gear_slot : {gear_slot::accessory1_id, gear_slot::accessory2_id,
                               gear_slot::accessory3_id, gear_slot::accessory4_id})
        {
            auto accessory_id = gear[gear_slot];
            auto accessory = params.get_equip_param_accessory()->get_row_by_id(accessory_id);
            if (accessory)
                weight += accessory->weight;
        }

        save_accessory_param.weight = weight;
        apply_accessory_param.weight = weight;
    }
}

void saveslots::SaveSlot::save_from_player()
{
    auto main_player = from::CS::WorldChrManImp::instance().reference().get_main_player();
    if (main_player == nullptr)
    {
        spdlog::error("Can't save loadout, main player is null");
        return;
    }

    auto &chr_asm = main_player->get_game_data()->get_equip_game_data().get_chr_asm();
    copy(begin(chr_asm.gear), end(chr_asm.gear), begin(gear));

    spdlog::info("TODO: save slot {}", index + 1);

    refresh();
}

void saveslots::SaveSlot::apply_to_player()
{
    auto main_player = from::CS::WorldChrManImp::instance().reference().get_main_player();
    if (main_player == nullptr || main_player->get_game_data() == nullptr)
    {
        spdlog::error("Can't apply loadout, main player is missing");
        return;
    }

    spdlog::info("Applying loadout {} to player", index + 1);

    auto &equip_game_data = main_player->get_game_data()->get_equip_game_data();
    auto &inventory_entries = equip_game_data.get_equip_inventory_data().entries;

    auto find_inventory_index = [this, &inventory_entries](unsigned int gear_slot) {
        auto item_type = gear_item_types[gear_slot];
        auto item_id = gear[gear_slot];

        if (item_type == item_type::invalid)
        {
            return -1;
        }

        // Look for an item with the same type and ID in the player's inventory
        for (auto i = 0; i < inventory_entries.count; i++)
        {
            if (inventory_entries[i].item_id == item_type + item_id)
            {
                spdlog::info("Found {} {} in index {}", gear_slot_debug_names[gear_slot], item_id,
                             i);
                return i;
            }
        }

        // For weapons, check if there's one at a different upgrade level
        if (item_type == item_type::weapon)
        {
            auto [weapon_id, weapon_upgrade] =
                get_weapon_id_upgrade_level(item_id - item_type::weapon);
            auto best_index = -1;
            auto best_upgrade = 0;

            for (int i = 0; i < inventory_entries.count; i++)
            {
                if (get_item_type(inventory_entries[i].item_id) == item_type::weapon)
                {
                    auto [entry_weapon_id, entry_weapon_upgrade] = get_weapon_id_upgrade_level(
                        inventory_entries[i].item_id - item_type::weapon);
                    if (entry_weapon_id == weapon_id && entry_weapon_upgrade > best_upgrade)
                    {
                        best_index = i;
                        best_upgrade = entry_weapon_upgrade;
                    }
                }
            }

            spdlog::info("Found substitute {} {} in index {} (+{} instead of +{})",
                         gear_slot_debug_names[gear_slot], item_id, best_index, best_upgrade,
                         weapon_upgrade);

            return best_index;
        }

        // If the item wasn't found, equip the default placeholder
        for (auto i = 0; i < inventory_entries.count; i++)
        {
            if (inventory_entries[i].item_id == item_type + default_gear[gear_slot])
            {
                spdlog::info("Couldn't find {} {} in inventory, equipping {}",
                             gear_slot_debug_names[gear_slot], item_id, default_gear[gear_slot]);
                return i;
            }
        }

        spdlog::warn("Couldn't find {} {} or default in inventory",
                     gear_slot_debug_names[gear_slot], item_id);

        return -1;
    };

    for (unsigned int gear_slot = 0; gear_slot < gear_slot::count; gear_slot++)
    {
        auto item_type = gear_item_types[gear_slot];
        if (item_type == item_type::invalid)
        {
            continue;
        }

        auto item_id = gear[gear_slot];
        auto inventory_index = find_inventory_index(gear_slot);
        if (inventory_index != -1)
        {
            auto inventory_entry = inventory_entries[inventory_index];
            spdlog::info("Equipping {} {}", gear_slot_debug_names[gear_slot],
                         inventory_entry.item_id - get_item_type(inventory_entry.item_id));

            auto ga_item_id = inventory_entry.ga_item_id;
            auto index = equip_game_data.get_equip_inventory_data().start_index + inventory_index;
            players::equip_gear(&equip_game_data, gear_slot, &ga_item_id, index, true, true, false);
        }
    }

    // Show a cool effect on the player
    players::spawn_one_shot_sfx_on_chr(main_player, 900, 8020, nullptr);
}
