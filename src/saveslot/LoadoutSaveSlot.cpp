#include <spdlog/spdlog.h>

#include "../messages/LoadoutMessages.hpp"
#include "../shop/LoadoutShop.hpp"
#include "../utils/PlayerUtils.hpp"
#include "LoadoutSaveSlot.hpp"
#include "StringifyLoadout.hpp"

using namespace std;
using namespace erloadout;

std::array<saveslots::SaveSlot, saveslots::max_slots> erloadout::saveslots::slots;

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
                                                   .weight = 1,
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
        // if (index == 0)
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
        info = L"-";
        caption = L"-";
        save_accessory_param.iconId = icon_id_empty_slot;
        apply_accessory_param.iconId = icon_id_empty_slot;
    }
    else
    {
        name = msg::loadout_messages.loadout;
        name += L" ";
        name += to_wstring(index + 1);

        caption = stringify_loadout(*this);

        info = iconify_loadout(*this);
        info += L"\n";
        info += msg::loadout_messages.press_x_to_view;

        save_accessory_param.iconId = icon_id_slot;
        apply_accessory_param.iconId = icon_id_slot;
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
    // TODO
    spdlog::info("TODO: apply slot {}", index);
}
