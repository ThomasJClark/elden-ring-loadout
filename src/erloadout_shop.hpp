#pragma once

#include <cstdint>

namespace erloadout
{
namespace shop
{
static constexpr unsigned int item_type_accessory_begin = 0x20000000;
static constexpr unsigned int item_type_accessory_end = 0x30000000;
static constexpr int save_loadout_shop_id = 3900000;
static constexpr int apply_loadout_shop_id = 3901000;
static constexpr int save_loadout_accessory_base_id = 6800000;
static constexpr int apply_loadout_accessory_base_id = 6801000;

void initialize();
}
}
