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
                                                 .equipType = 3,
                                                 .setNum = 1,
                                                 .iconId = -1,
                                                 .nameMsgId = -1};

    EquipParamGoods initial_goods_param = {.refId_default = -1,
                                           .sfxVariationId = -1,
                                           .weight = 1,
                                           .replaceItemId = -1,
                                           .sortId = -1,
                                           .appearanceReplaceItemId = -1,
                                           .yesNoDialogMessageId = -1,
                                           .potGroupId = -1,
                                           .compTrophySedId = -1,
                                           .trophySeqId = -1,
                                           .maxNum = 1,
                                           .goodsType = 12,
                                           .refId_1 = -1,
                                           .refVirtualWepId = -1,
                                           .castSfxId = -1,
                                           .fireSfxId = -1,
                                           .effectSfxId = -1,
                                           .showLogCondType = 1,
                                           .showDialogCondType = 2,
                                           .sortGroupId = 0,
                                           .isUseNoAttackRegion = 1,
                                           .aiUseJudgeId = -1,
                                           .reinforceGoodsId = -1,
                                           .reinforceMaterialId = -1};

    int index = 0;
    for (auto &slot : slots)
    {
        slot = {
            .index = index,
            .empty = true,
            .right_weapon_ids = {unarmed_weapon_id, unarmed_weapon_id, unarmed_weapon_id},
            .left_weapon_ids = {unarmed_weapon_id, unarmed_weapon_id, unarmed_weapon_id},
            .arrow_id1 = empty_ammo_id,
            .arrow_id2 = empty_ammo_id,
            .bolt_id1 = empty_ammo_id,
            .bolt_id2 = empty_ammo_id,
            .head_protector_id = bare_head_protector_id,
            .chest_protector_id = bare_chest_protector_id,
            .arms_protector_id = bare_arms_protector_id,
            .legs_protector_id = bare_legs_protector_id,
            .accessory_ids = {empty_accessory_id, empty_accessory_id, empty_accessory_id,
                              empty_accessory_id},
            .save_goods_param = initial_goods_param,
            .apply_goods_param = initial_goods_param,
            .save_shop_lineup_param = initial_shop_lineup_param,
            .apply_shop_lineup_param = initial_shop_lineup_param,
        };

        slot.save_shop_lineup_param.equipId = shop::save_loadout_goods_base_id + index;
        slot.apply_shop_lineup_param.equipId = shop::apply_loadout_goods_base_id + index;

#if _DEBUG
        if (index == 0)
        {
            slot.empty = false;
            slot.right_weapon_ids[0] = 11050000; // Morning Star
            slot.left_weapon_ids[0] = 31340000;  // Black Leather Shield
            slot.arrow_id1 = 50020000;           // Serpent Arrow
            slot.arrow_id2 = 50040000;           // St. Trina's Arrow
            slot.bolt_id1 = 52030000;            // Black-Key Bolt
            slot.head_protector_id = 1840000;    // Foot Soldier Helm
            slot.chest_protector_id = 290100;    // Nox Monk Armor
            slot.arms_protector_id = 630200;     // Astrologer Gloves
            slot.legs_protector_id = 740300;     // Noble's Trousers
            slot.accessory_ids[0] = 1150;        // Green Turtle Talisman
            slot.accessory_ids[1] = 2160;        // Lord of Blood's Exultation
            slot.accessory_ids[2] = 2170;        // Kindred of Rot's Exultation
            slot.accessory_ids[3] = 1210;        // Bull-Goat's Talisman
        }
#endif

        index++;

        slot.refresh();
    }
}

void saveslots::SaveSlot::refresh()
{
    if (empty)
    {
        name = msg::loadout_messages.empty_slot;
        info = L"-";
        caption = L"-";
        save_goods_param.iconId = icon_id_empty_slot;
        apply_goods_param.iconId = icon_id_empty_slot;
    }
    else
    {
        name = msg::loadout_messages.loadout;
        name += L" ";
        name += index + 1;

        caption = stringify_loadout(*this);

        info = L"TODO slot info\n\n";
        info += msg::loadout_messages.press_x_to_view;

        save_goods_param.iconId = icon_id_slot;
        apply_goods_param.iconId = icon_id_slot;
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

    auto value_or_default = [](int value, int default_value) {
        return value == default_value ? -1 : value;
    };

    empty = false;
    right_weapon_ids[0] = value_or_default(chr_asm.right_weapon_id1, unarmed_weapon_id);
    right_weapon_ids[1] = value_or_default(chr_asm.right_weapon_id2, unarmed_weapon_id);
    right_weapon_ids[2] = value_or_default(chr_asm.right_weapon_id3, unarmed_weapon_id);
    left_weapon_ids[0] = value_or_default(chr_asm.left_weapon_id1, unarmed_weapon_id);
    left_weapon_ids[1] = value_or_default(chr_asm.left_weapon_id2, unarmed_weapon_id);
    left_weapon_ids[2] = value_or_default(chr_asm.left_weapon_id3, unarmed_weapon_id);
    arrow_id1 = chr_asm.arrow_id1;
    arrow_id2 = chr_asm.arrow_id2;
    bolt_id1 = chr_asm.bolt_id1;
    bolt_id2 = chr_asm.bolt_id2;
    head_protector_id = value_or_default(chr_asm.head_protector_id, bare_head_protector_id);
    chest_protector_id = value_or_default(chr_asm.chest_protector_id, bare_chest_protector_id);
    arms_protector_id = value_or_default(chr_asm.arms_protector_id, bare_arms_protector_id);
    legs_protector_id = value_or_default(chr_asm.legs_protector_id, bare_legs_protector_id);
    accessory_ids[0] = chr_asm.accessory_ids[0];
    accessory_ids[1] = chr_asm.accessory_ids[1];
    accessory_ids[2] = chr_asm.accessory_ids[2];
    accessory_ids[3] = chr_asm.accessory_ids[3];

    spdlog::info("TODO: save slot {}", index);

    refresh();
}

void saveslots::SaveSlot::apply_to_player()
{
    // TODO
    spdlog::info("TODO: apply slot {}", index);
}
