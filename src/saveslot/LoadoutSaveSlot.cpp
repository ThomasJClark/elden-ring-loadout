#include <spdlog/spdlog.h>

#include "../messages/LoadoutMessages.hpp"
#include "../shop/LoadoutShop.hpp"
#include "LoadoutSaveSlot.hpp"
#include "StringifyLoadout.hpp"

using namespace std;
using namespace erloadout;

std::array<saveslots::SaveSlot, saveslots::max_slots> erloadout::saveslots::slots;

static constexpr int32_t icon_id_empty_slot = 249;
static constexpr int32_t icon_id_slot = 250;

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
            .right_weapon_ids = {-1, -1, -1},
            .left_weapon_ids = {-1, -1, -1},
            .arrow_ids = {-1, -1},
            .bolt_ids = {-1, -1},
            .protector_ids = {-1, -1, -1, -1},
            .accessory_ids = {-1, -1, -1, -1},
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
            slot.right_weapon_ids[0] = 11050000; // Morning Star
            slot.left_weapon_ids[0] = 31340000;  // Black Leather Shield
            slot.arrow_ids[0] = 50020000;        // Serpent Arrow
            slot.arrow_ids[1] = 50040000;        // St. Trina's Arrow
            slot.bolt_ids[0] = 52030000;         // Black-Key Bolt
            slot.protector_ids[0] = 1840000;     // Foot Soldier Helm
            slot.protector_ids[1] = 290100;      // Nox Monk Armor
            slot.protector_ids[2] = 630200;      // Astrologer Gloves
            slot.protector_ids[3] = 740300;      // Noble's Trousers
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
    empty =
        all_of(begin(right_weapon_ids), end(accessory_ids), [](int value) { return value == -1; });

    if (empty)
    {
        name = msg::loadout_messages.empty_slot;
        info = u"-";
        caption = u"-";
        save_goods_param.iconId = icon_id_empty_slot;
        apply_goods_param.iconId = icon_id_empty_slot;
    }
    else
    {
        u16stringstream name_stream;
        auto id_str = std::to_wstring(index + 1);
        name_stream << msg::loadout_messages.loadout_slot << u" "
                    << u16string{id_str.begin(), id_str.end()};
        name = name_stream.str();

        u16stringstream caption_stream;
        stringify_loadout(caption_stream, *this);
        caption = caption_stream.str();

        u16stringstream info_stream;
        info_stream << u"TODO slot info\n";
        info_stream << u"\n";
        info_stream << msg::loadout_messages.press_x_to_view;
        info = info_stream.str();

        save_goods_param.iconId = icon_id_slot;
        apply_goods_param.iconId = icon_id_slot;
    }
}

void saveslots::SaveSlot::save_from_player()
{
    // TODO
    spdlog::info("TODO: save slot {}", index);
    refresh();
}

void saveslots::SaveSlot::apply_to_player()
{
    // TODO
    spdlog::info("TODO: apply slot {}", index);
}
