#pragma once
#include <map>
#include <string>

namespace erloadout
{
enum class msgbnd : unsigned int
{
    goods_name = 10,
    weapon_name = 11,
    protector_name = 12,
    accessory_name = 13,
    goods_info = 20,
    accessory_info = 23,
    goods_caption = 24,
    accessory_caption = 27,
    event_text_for_talk = 33,
    menu_text = 200,
    line_help = 201,
    dialogues = 204,
};

namespace msg
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

struct LoadoutMessages
{
    const wchar_t *manage_loadouts;
    const wchar_t *save_loadout;
    const wchar_t *apply_loadout;
    const wchar_t *select_loadout_to_save;
    const wchar_t *select_loadout_to_apply;
    const wchar_t *save_loadout_prompt;
    const wchar_t *apply_loadout_prompt;
    const wchar_t *loadout;
    const wchar_t *empty_slot;
    const wchar_t *armaments;
    const wchar_t *arrows_bolts;
    const wchar_t *armor;
    const wchar_t *talismans;
    const wchar_t *unarmed;
    const wchar_t *none;
};

void initialize();

void set_active_shop(int shop_id);

const wchar_t *get_message(msgbnd msgbnd_id, int id);

/**
 * Map of Steam language names to messages used by the mod
 *
 * https://partner.steamgames.com/doc/store/localization/languages
 */
extern const std::map<std::string, LoadoutMessages> loadout_messages_by_lang;

/**
 * Localized messages based on the player's current language preference
 */
extern LoadoutMessages loadout_messages;
};
};