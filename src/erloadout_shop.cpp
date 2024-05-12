#include <unordered_map>
#include <unordered_set>

#include <paramdefs/SHOP_LINEUP_PARAM.hpp>

#include "erloadout_loadout.hpp"
#include "erloadout_messages.hpp"
#include "erloadout_shop.hpp"
#include "utils/modutils.hpp"
#include "utils/players.hpp"

using namespace erloadout;
using namespace std;

namespace from
{
namespace CS
{

enum class shop_type : unsigned char
{
    regular = 0
};

struct FindShopMenuResult
{
    shop_type shop_type;
    int id;
    from::paramdefs::SHOP_LINEUP_PARAM *row;
};

struct FindShopLineupParamResult
{
    shop_type shop_type;
    int id;
    from::paramdefs::SHOP_LINEUP_PARAM *row;
};

struct FindEquipParamAccessoryResult
{
    int id;
    int unknown;
    from::paramdefs::EQUIP_PARAM_ACCESSORY_ST *row;
};

}
}

typedef void AddRemoveItemFn(unsigned long long item_type, unsigned int item_id, int quantity);
static AddRemoveItemFn *add_remove_item = nullptr;

static from::CS::FindShopMenuResult *(*get_shop_menu)(from::CS::FindShopMenuResult *result,
                                                      from::CS::shop_type shop_type, int begin_id,
                                                      int end_id);

from::CS::FindShopMenuResult *get_shop_menu_detour(from::CS::FindShopMenuResult *result,
                                                   from::CS::shop_type shop_type, int begin_id,
                                                   int end_id)
{
    if (begin_id == shop::save_loadout_shop_id)
    {
        static from::paramdefs::SHOP_LINEUP_PARAM save_loadout_shop_menu = {
            .menuTitleMsgId = msg::menu_text_save_loadout,
            .menuIconId = 5,
        };

        result->shop_type = from::CS::shop_type::regular;
        result->id = shop::save_loadout_shop_id;
        result->row = &save_loadout_shop_menu;
    }
    else if (begin_id == shop::apply_loadout_shop_id)
    {
        static from::paramdefs::SHOP_LINEUP_PARAM apply_loadout_shop_menu = {
            .menuTitleMsgId = msg::menu_text_apply_loadout,
            .menuIconId = 5,
        };

        result->shop_type = from::CS::shop_type::regular;
        result->id = shop::apply_loadout_shop_id;
        result->row = &apply_loadout_shop_menu;
    }
    else
    {
        get_shop_menu(result, shop_type, begin_id, end_id);
    }

    return result;
}

static void (*get_shop_lineup_param)(from::CS::FindShopLineupParamResult *, from::CS::shop_type,
                                     int id);

static void get_shop_lineup_param_detour(from::CS::FindShopLineupParamResult *result,
                                         from::CS::shop_type shop_type, int id)
{
    if (id >= shop::save_loadout_shop_id &&
        id < shop::save_loadout_shop_id + loadouts::loadouts.size())
    {
        auto loadout_slot = id - shop::save_loadout_shop_id;
        result->shop_type = shop_type;
        result->id = id;
        result->row = &loadouts::loadouts[loadout_slot].save_shop_lineup_param;
        return;
    }
    else if (id >= shop::apply_loadout_shop_id &&
             id < shop::apply_loadout_shop_id + loadouts::loadouts.size())
    {
        auto loadout_slot = id - shop::apply_loadout_shop_id;
        result->shop_type = shop_type;
        result->id = id;
        result->row = &loadouts::loadouts[loadout_slot].apply_shop_lineup_param;
        return;
    }

    get_shop_lineup_param(result, shop_type, id);
}

static void (*get_equip_param_accessory)(from::CS::FindEquipParamAccessoryResult *result, int id);

static void get_equip_param_accessory_detour(from::CS::FindEquipParamAccessoryResult *result,
                                             int id)
{
    if (id >= shop::save_loadout_accessory_base_id &&
        id < shop::save_loadout_accessory_base_id + loadouts::loadouts.size())
    {
        auto &slot = loadouts::loadouts[id - shop::save_loadout_accessory_base_id];
        result->id = id;
        result->row = &slot.save_accessory_param;
        result->unknown = 3;
        return;
    }
    if (id >= shop::apply_loadout_accessory_base_id &&
        id < shop::apply_loadout_accessory_base_id + loadouts::loadouts.size())
    {
        auto &slot = loadouts::loadouts[id - shop::apply_loadout_accessory_base_id];

        // Only show non-empty slots in the "Apply loadout" menu
        if (!slot.empty)
        {
            result->id = id;
            result->row = &slot.apply_accessory_param;
            result->unknown = 3;
        }
        return;
    }

    get_equip_param_accessory(result, id);
}

static void (*open_regular_shop)(void *, unsigned long long, unsigned long long);

static void open_regular_shop_detour(void *unk, unsigned long long begin_id,
                                     unsigned long long end_id)
{
    if (begin_id == shop::save_loadout_shop_id || begin_id == shop::apply_loadout_shop_id)
    {
        msg::set_active_shop(begin_id);

        for (auto &slot : loadouts::loadouts)
            slot.refresh();
    }
    else
    {
        msg::set_active_shop(-1);
    }

    open_regular_shop(unk, begin_id, end_id);
}

static bool (*add_inventory_from_shop)(int *new_item_id, int quantity) = nullptr;

static bool add_inventory_from_shop_detour(int *item_id_address, int quantity)
{
    auto item_id = *item_id_address;
    if (item_id >= shop::item_type_accessory_begin && item_id < shop::item_type_accessory_end)
    {
        auto accessory_id = item_id - shop::item_type_accessory_begin;
        if (accessory_id >= shop::save_loadout_accessory_base_id &&
            accessory_id < shop::save_loadout_accessory_base_id + loadouts::loadouts.size())
        {
            auto &loadout = loadouts::loadouts[accessory_id - shop::save_loadout_accessory_base_id];
            loadout.save_from_player();
            return false;
        }
        if (accessory_id >= shop::apply_loadout_accessory_base_id &&
            accessory_id < shop::apply_loadout_accessory_base_id + loadouts::loadouts.size())
        {
            auto &loadout =
                loadouts::loadouts[accessory_id - shop::apply_loadout_accessory_base_id];
            loadout.apply_to_player();
            return false;
        }
    }

    return add_inventory_from_shop(item_id_address, quantity);
}

void erloadout::shop::initialize()
{
    add_remove_item = modutils::scan<AddRemoveItemFn>({
        .aob = "8b 99 90 01 00 00" // mov ebx, [rcx + 0x190] ; param->hostModeCostItemId
               "41 83 c8 ff"       // or r8d, -1
               "8b d3"             // mov edx, ebx
               "b9 00 00 00 40"    // mov ecx, item_type_goods_begin
               "e8 ?? ?? ?? ??",   // call AddRemoveItem
        .offset = 17,
        .relative_offsets = {{1, 5}},
    });

    modutils::hook(
        {
            // Note - the mov instructions are 44 or 45 depending on if this is the Japanese or
            // international .exe, and the stack offset is either -10 or -08. This pattern works
            // for both versions.
            .aob = "?? 8b 4e 14"     // mov r9d, [rsi + 14]
                   "?? 8b 46 10"     // mov r8d, [rsi + 10]
                   "33 d2"           // xor edx, edx
                   "48 8d 4d ??"     // lea rcx, [rbp + ??]
                   "e8 ?? ?? ?? ??", // call FindShopMenu
            .offset = 14,
            .relative_offsets = {{1, 5}},
        },
        get_shop_menu_detour, get_shop_menu);

    // Hook get_equip_param_accessory() to return the above items
    modutils::hook(
        {
            .offset = 0xcef040,
        },
        get_equip_param_accessory_detour, get_equip_param_accessory);

    // Hook get_shop_lineup_param() to return the above shop entries
    modutils::hook(
        {
            .aob = "48 8d 15 ?? ?? ?? ??" // lea rdx, [shop_lineup_param_indexes]
                   "45 33 c0"             // xor r8d, r8d
                   "?? ?? ??"             // ???
                   "e8 ?? ?? ?? ??"       // call SoloParamRepositoryImp::GetParamResCap
                   "48 85 c0"             // test rax, rax
                   "74 ??",               // jz end_lbl
            .offset = -129,
        },
        get_shop_lineup_param_detour, get_shop_lineup_param);

    // Hook add_inventory_from_shop() to save or load a loadout when a shop item is chosen
    modutils::hook(
        {
            .aob = "8b 93 80 00 00 00" // mov edx, [rbx + 0x80]
                   "0f af d1"          // imul edx, ecx
                   "48 8b c8"          // mov rcx, rax
                   "e8 ?? ?? ?? ??",   // call AddInventoryFromShop
            .offset = 12,
            .relative_offsets = {{1, 5}},
        },
        add_inventory_from_shop_detour, add_inventory_from_shop);

    // Hook OpenRegularShop() to perform some memory hacks when opening up the a shop
    modutils::hook(
        {
            .aob = "4c 8b 49 18"           // mov    r9, [rcx + 0x18]
                   "48 8b d9"              // mov    rbx,rcx
                   "48 8d 4c 24 20"        // lea    rcx, [rsp + 0x20]
                   "e8 ?? ?? ?? ??"        // call   OpenRegularShopInner
                   "48 8d 4c 24 20"        // lea    rcx, [rsp + 0x20]
                   "0f 10 00"              // movups xmm0, [rax]
                   "c7 43 10 05 00 00 00", // mov    [rbx + 0x10], 5
            .offset = -6,
        },
        open_regular_shop_detour, open_regular_shop);
}
