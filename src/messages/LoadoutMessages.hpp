#pragma once
#include <cstdint>
#include <string>

namespace LoadoutMessages
{
namespace EventTextForTalk
{
static constexpr int32_t manage_loadouts = 68000000;
static constexpr int32_t save_loadout = 68000001;
static constexpr int32_t apply_loadout = 68000002;
static constexpr int32_t cancel = 15000372;
static constexpr int32_t sort_chest = 15000395;
} // namespace EventTextForTalk

namespace MenuText
{
static constexpr int32_t save_loadout = 680000;
static constexpr int32_t apply_loadout = 680001;
static constexpr int32_t number_held = 11004;
static constexpr int32_t stored = 11005;
static constexpr int32_t item_effect = 20006;
static constexpr int32_t equipment = 20016;
} // namespace MenuText

namespace LineHelp
{
static constexpr int32_t select_item_for_purchase = 231000;
}

namespace Dialogues
{
static constexpr int32_t purchase_item_for_runes = 231000;
}

void initialize();
void set_active_shop(int32_t shop_id);
}; // namespace LoadoutMessages
