#pragma once

#include <cstdint>

namespace erloadout
{
namespace shop
{
static constexpr uint32_t item_type_accessory_begin = 0x20000000;
static constexpr uint32_t item_type_accessory_end = 0x30000000;
static constexpr int32_t save_loadout_shop_id = 3900000;
static constexpr int32_t apply_loadout_shop_id = 3901000;
static constexpr int32_t save_loadout_accessory_base_id = 6800000;
static constexpr int32_t apply_loadout_accessory_base_id = 6801000;

void initialize();
}
}
