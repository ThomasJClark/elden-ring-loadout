#include <algorithm>
#include <map>

#include <coresystem/param.hpp>
#include <paramdefs/EQUIP_PARAM_ACCESSORY_ST.hpp>
#include <paramdefs/EQUIP_PARAM_PROTECTOR_ST.hpp>
#include <paramdefs/EQUIP_PARAM_WEAPON_ST.hpp>
#include <spdlog/spdlog.h>

#include "erloadout_gear_info.hpp"
#include "erloadout_loadout.hpp"
#include "erloadout_messages.hpp"
#include "erloadout_stringify.hpp"
#include "from/WorldChrManImp.hpp"
#include "utils/players.hpp"

using namespace std;
using namespace erloadout;

vector<array<loadout, loadout_count>> erloadout::loadout_books;

static int book_id = -1;

/**
 * @return a key unique to this playthrough. Loadouts are stored in a separate JSON file from the
 * actual save file, so this key allows us to determine which loadouts were created while this
 * character was loaded. If there isn't one already, one will be created.
 */
int get_loadout_book_id()
{
    auto world_chr_man = from::CS::WorldChrManImp::instance();
    if (!world_chr_man)
        return -1;

    auto player = world_chr_man.reference().get_main_player();
    if (!player)
        return -1;

    auto &equip_inventory_data =
        player->get_game_data()->get_equip_game_data().get_equip_inventory_data();

    // Determine the loadout key from the loadout book ID already in the player's inventory
    for (int i = 0; i < loadout_book_count; i++)
    {
        int item_id = item_type::goods + loadout_book_goods_base_id + i;
        if (players::get_inventory_id(&equip_inventory_data, &item_id) != -1)
        {
            spdlog::info("Determined book ID {} from key item {:x} in inventory", i, item_id);
            return i;
        }
    }

    // TODO give the player a book if not found
    spdlog::warn("Couldn't find book ID");

    return -1;
}

void erloadout::refresh_loadouts()
{
    constexpr unsigned char equip_type_accessory = 2;

    book_id = get_loadout_book_id();

    auto &loadout_book = loadout_books[book_id];
    for (int index = 0; index < loadout_book.size(); index++)
    {
        auto &loadout = loadout_book[index];
        loadout.index = index;
        loadout.save_accessory_param = {.saleValue = 0},
        loadout.apply_accessory_param = {.saleValue = 0},
        loadout.save_shop_lineup_param.equipId = save_loadout_accessory_base_id + index;
        loadout.save_shop_lineup_param.equipType = equip_type_accessory;
        loadout.save_shop_lineup_param.value_Magnification = 0;
        loadout.apply_shop_lineup_param.equipId = apply_loadout_accessory_base_id + index;
        loadout.apply_shop_lineup_param.equipType = equip_type_accessory;
        loadout.apply_shop_lineup_param.value_Magnification = 0;
        loadout.refresh();
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

        save_accessory_param.iconId = loadout_empty_slot_icon_ids[index];
        apply_accessory_param.iconId = loadout_empty_slot_icon_ids[index];
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

        save_accessory_param.iconId = loadout_slot_icon_ids[index];
        apply_accessory_param.iconId = loadout_slot_icon_ids[index];

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

loadout &erloadout::get_loadout(size_t pos)
{
    return loadout_books[book_id][pos];
}
