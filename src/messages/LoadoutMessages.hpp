#pragma once
#include <cstdint>
#include <string>

namespace erloadout
{
namespace msg
{
static constexpr int32_t event_text_for_talk_manage_loadouts = 68000000;
static constexpr int32_t event_text_for_talk_save_loadout = 68000001;
static constexpr int32_t event_text_for_talk_apply_loadout = 68000002;
static constexpr int32_t event_text_for_talk_cancel = 15000372;
static constexpr int32_t event_text_for_talk_sort_chest = 15000395;
static constexpr int32_t menu_text_save_loadout = 680000;
static constexpr int32_t menu_text_apply_loadout = 680001;
static constexpr int32_t menu_text_number_held = 11004;
static constexpr int32_t menu_text_stored = 11005;
static constexpr int32_t menu_text_item_effect = 20006;
static constexpr int32_t menu_text_equipment = 20016;
static constexpr int32_t line_help_select_item_for_purchase = 231000;
static constexpr int32_t dialogues_purchase_item_for_runes = 231000;

void initialize();
void set_active_shop(int32_t shop_id);
};
};