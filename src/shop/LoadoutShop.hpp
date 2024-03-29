#pragma once

#include <cstdint>

namespace erloadout
{
namespace shop
{
static constexpr uint32_t item_type_goods_begin = 0x40000000;
static constexpr uint32_t item_type_goods_end = 0x50000000;
static constexpr int32_t save_loadout_shop_id = 3900000;
static constexpr int32_t apply_loadout_shop_id = 3900100;
static constexpr int32_t loadout_goods_base_id = 6800000;
static constexpr uint32_t loadout_slots = 25;

void initialize();
}
}
