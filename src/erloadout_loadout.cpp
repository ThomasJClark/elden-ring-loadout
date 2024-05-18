#include <algorithm>

#include <coresystem/param.hpp>
#include <paramdefs/EQUIP_PARAM_ACCESSORY_ST.hpp>
#include <paramdefs/EQUIP_PARAM_PROTECTOR_ST.hpp>
#include <paramdefs/EQUIP_PARAM_WEAPON_ST.hpp>
#include <spdlog/spdlog.h>

#include "erloadout_gear_info.hpp"
#include "erloadout_loadout.hpp"
#include "erloadout_messages.hpp"
#include "erloadout_shop.hpp"
#include "erloadout_stringify.hpp"
#include "from/WorldChrManImp.hpp"

using namespace std;
using namespace erloadout;

array<erloadout::loadout, 25> erloadout::loadouts;

static constexpr array<unsigned short, erloadout::loadouts.size()> icon_ids = {
    361, 362, 363, 364, 365, 366, 367, 368,  369,  370,  371,  372,  373,
    374, 375, 377, 378, 379, 380, 359, 3504, 3505, 3508, 3531, 3538,
};

static constexpr array<unsigned short, erloadout::loadouts.size()> empty_icon_ids = {
    653, 654, 655, 656, 657, 658, 659, 660,  661,  662,  663,  664, 665,
    666, 667, 669, 670, 671, 672, 651, 3704, 3705, 3708, 3732, 3739};

void erloadout::initialize_loadouts()
{
    constexpr unsigned char equip_type_accessory = 2;

    for (int index = 0; index < loadouts.size(); index++)
    {
        loadouts[index] = {
            .index = index,
            .save_accessory_param = {.saleValue = 0},
            .apply_accessory_param = {.saleValue = 0},
            .save_shop_lineup_param =
                {
                    .equipId = shop::save_loadout_accessory_base_id + index,
                    .equipType = equip_type_accessory,
                    .value_Magnification = 0,
                },
            .apply_shop_lineup_param =
                {
                    .equipId = shop::apply_loadout_accessory_base_id + index,
                    .equipType = equip_type_accessory,
                    .value_Magnification = 0,
                },
        };
    }
}

void erloadout::loadout::refresh()
{
    empty = true;

    // Set any empty gear to the default ID for that gear slot (e.g. bare fist for weapon slots)
    // and check if the save slot is empty overall
    for (auto i = 0; i < gear.size(); i++)
    {
        if (gear[i] <= 0)
            gear[i] = erloadout::gearinfo[i].default_value;

        if (gear[i] != erloadout::gearinfo[i].default_value)
            empty = false;
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
