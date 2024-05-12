#include <algorithm>

#include <spdlog/spdlog.h>

#include "erloadout_gear_info.hpp"
#include "erloadout_to_player.hpp"
#include "utils/players.hpp"

using namespace erloadout;
using namespace std;

static inline unsigned int get_item_type(unsigned int item_id)
{
    return item_id & 0x70000000;
}

static inline pair<unsigned int, unsigned int> get_weapon_id_upgrade_level(unsigned int weapon_id)
{
    auto upgrade_level = weapon_id % 100;
    return {weapon_id - upgrade_level, upgrade_level};
}

void erloadout::save_from_player(loadout &loadout, from::CS::PlayerIns &player)
{
    auto &chr_asm = player.get_game_data()->get_equip_game_data().get_chr_asm();
    copy(begin(chr_asm.gear), end(chr_asm.gear), begin(loadout.gear));
    loadout.refresh();
}

void erloadout::apply_to_player(loadout const &loadout, from::CS::PlayerIns &player)
{
    spdlog::info("Applying loadout {} to player", loadout.index + 1);

    auto &equip_game_data = player.get_game_data()->get_equip_game_data();
    auto &inventory_entries = equip_game_data.get_equip_inventory_data().entries;

    auto find_inventory_index = [&loadout, &inventory_entries](unsigned int gear_slot) {
        auto &info = gearinfo[gear_slot];
        auto item_id = loadout.gear[gear_slot];

        if (info.item_type == item_type::invalid)
        {
            return -1;
        }

        // Look for an item with the same type and ID in the player's inventory
        for (auto i = 0; i < inventory_entries.count; i++)
        {
            if (inventory_entries[i].item_id == info.item_type + item_id)
            {
                spdlog::info("Found {} {} in index {}", info.name, item_id, i);
                return i;
            }
        }

        // For weapons, check if there's one at a different upgrade level
        if (info.item_type == item_type::weapon)
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

            spdlog::info("Found substitute {} {} in index {} (+{} instead of +{})", info.name,
                         item_id, best_index, best_upgrade, weapon_upgrade);

            return best_index;
        }

        // If the item wasn't found, equip the default placeholder
        for (auto i = 0; i < inventory_entries.count; i++)
        {
            if (inventory_entries[i].item_id == info.item_type + info.default_value)
            {
                spdlog::info("Couldn't find {} {} in inventory, equipping {}", info.name, item_id,
                             info.default_value);
                return i;
            }
        }

        spdlog::warn("Couldn't find {} {} or default in inventory", info.name, item_id);

        return -1;
    };

    for (unsigned int gear_slot = 0; gear_slot < gear_slot::count; gear_slot++)
    {
        auto &info = gearinfo[gear_slot];
        if (info.item_type == item_type::invalid)
        {
            continue;
        }

        auto item_id = loadout.gear[gear_slot];
        auto inventory_index = find_inventory_index(gear_slot);
        if (inventory_index != -1)
        {
            auto inventory_entry = inventory_entries[inventory_index];
            spdlog::info("Equipping {} {}", info.name,
                         inventory_entry.item_id - get_item_type(inventory_entry.item_id));

            auto ga_item_id = inventory_entry.ga_item_id;
            auto index = equip_game_data.get_equip_inventory_data().start_index + inventory_index;
            players::equip_gear(&equip_game_data, gear_slot, &ga_item_id, index, true, true, false);
        }
    }

    // Show a cool effect on the player
    players::spawn_one_shot_sfx_on_chr(&player, 900, 8020, nullptr);
}