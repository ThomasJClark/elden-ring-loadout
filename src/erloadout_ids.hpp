#pragma once

#include <array>

/**
 * Various param and message IDs added or used by the mod
 */
namespace erloadout
{
static constexpr int event_text_for_talk_manage_loadouts = 68000000;
static constexpr int event_text_for_talk_save_loadout = 68000001;
static constexpr int event_text_for_talk_apply_loadout = 68000002;
static constexpr int event_text_for_talk_cancel = 15000372;
static constexpr int event_text_for_talk_sort_chest = 15000395;

static constexpr int menu_text_number_held = 11004;
static constexpr int menu_text_stored = 11005;
static constexpr int menu_text_save_loadout = 680000;
static constexpr int menu_text_apply_loadout = 680001;

static constexpr int line_help_select_item_for_purchase = 231000;

static constexpr int dialogues_purchase_item_for_runes = 231000;

static constexpr int save_loadout_shop_id = 3900000;
static constexpr int apply_loadout_shop_id = 3901000;

static constexpr int save_loadout_accessory_base_id = 6800000;
static constexpr int apply_loadout_accessory_base_id = 6801000;

static constexpr int loadout_book_goods_base_id = 6800000;

static constexpr int bare_head_protector_id = 10000;
static constexpr int bare_chest_protector_id = 10100;
static constexpr int bare_arms_protector_id = 10200;
static constexpr int bare_legs_protector_id = 10300;

static constexpr int unarmed_weapon_id = 110000;

static constexpr int empty_ammo_id = -1;

static constexpr int empty_accessory_id = -1;

/**
 * The number of distinct arrays of loadout slots that can be saved, each represented by the same
 * loadout book key item with different goods IDs.
 *
 * There are only 10 character slots, but players can have more playthoughs with the alt saves
 * mod, seamless co-op, or other methods of having multiple saveload files.
 */
constexpr size_t loadout_book_count = 1000;

/**
 * The number of loadout save slots per character
 */
static constexpr size_t loadout_count = 25;

static constexpr unsigned short loadout_book_icon_id = 307;

static constexpr std::array<unsigned short, loadout_count> loadout_slot_icon_ids = {
    361, 362, 363, 364, 365, 366, 367, 368,  369,  370,  371,  372,  373,
    374, 375, 377, 378, 379, 380, 359, 3504, 3505, 3508, 3531, 3538,
};

static constexpr std::array<unsigned short, loadout_count> loadout_empty_slot_icon_ids = {
    653, 654, 655, 656, 657, 658, 659, 660,  661,  662,  663,  664, 665,
    666, 667, 669, 670, 671, 672, 651, 3704, 3705, 3708, 3732, 3739};
}
