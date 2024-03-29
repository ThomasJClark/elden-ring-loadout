#pragma once
#include <cstdint>
#include <map>
#include <string>

namespace erloadout
{
namespace msg
{
static const uint32_t msgbnd_goods_name = 10;
static const uint32_t msgbnd_weapon_name = 11;
static const uint32_t msgbnd_protector_name = 12;
static const uint32_t msgbnd_accessory_name = 13;
static const uint32_t msgbnd_goods_info = 20;
static const uint32_t msgbnd_goods_caption = 24;
static const uint32_t msgbnd_protector_caption = 26;
static const uint32_t msgbnd_event_text_for_talk = 33;
static const uint32_t msgbnd_menu_text = 200;
static const uint32_t msgbnd_line_help = 201;
static const uint32_t msgbnd_system_message_win64 = 203;
static const uint32_t msgbnd_dialogues = 204;

static constexpr int32_t event_text_for_talk_manage_loadouts = 68000000;
static constexpr int32_t event_text_for_talk_save_loadout = 68000001;
static constexpr int32_t event_text_for_talk_apply_loadout = 68000002;
static constexpr int32_t event_text_for_talk_cancel = 15000372;
static constexpr int32_t event_text_for_talk_sort_chest = 15000395;
static constexpr int32_t menu_text_number_held = 11004;
static constexpr int32_t menu_text_stored = 11005;
static constexpr int32_t menu_text_item_effect = 20006;
static constexpr int32_t menu_text_equipment = 20016;
static constexpr int32_t menu_text_talismans = 40509;
static constexpr int32_t menu_text_arrows_bolts = 40511;
static constexpr int32_t menu_text_armaments = 40550;
static constexpr int32_t menu_text_armor = 40551;
static constexpr int32_t menu_text_save_loadout = 680000;
static constexpr int32_t menu_text_apply_loadout = 680001;
static constexpr int32_t line_help_select_item_for_purchase = 231000;
static constexpr int32_t dialogues_purchase_item_for_runes = 231000;

struct LoadoutMessages
{
    std::u16string manage_loadouts;
    std::u16string save_loadout;
    std::u16string apply_loadout;
    std::u16string select_loadout_slot;
    std::u16string loadout_slot;
    std::u16string empty_slot;
    std::u16string press_x_to_view;
    std::u16string loadout_details;
};

void initialize();

void set_active_shop(int32_t shop_id);

const char16_t *get_message(int32_t msgbnd_id, int32_t id);

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