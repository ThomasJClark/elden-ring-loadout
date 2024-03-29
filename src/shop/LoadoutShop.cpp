#include <cstdint>
#include <tga/paramdefs.h>
#include <unordered_map>
#include <unordered_set>

#include "../messages/LoadoutMessages.hpp"
#include "../utils/ModUtils.hpp"
#include "../utils/ParamUtils.hpp"
#include "../utils/PlayerUtils.hpp"
#include "LoadoutShop.hpp"

using namespace erloadout;
using namespace std;

typedef void AddRemoveItemFn(uint64_t item_type, uint32_t item_id, int32_t quantity);
static AddRemoveItemFn *add_remove_item = nullptr;

#pragma pack(push, 1)
struct FindShopMenuResult
{
    byte shop_type;
    byte padding[3];
    int32_t id;
    ShopLineupParam *row;
};

struct FindShopLineupParamResult
{
    byte shop_type;
    byte padding[3];
    int32_t id;
    ShopLineupParam *row;
};

struct FindEquipParamGoodsResult
{
    int32_t id;
    int32_t unknown;
    EquipParamGoods *row;
};
#pragma pack(pop)

static ShopLineupParam save_loadout_shop_menu = {0};
static ShopLineupParam apply_loadout_shop_menu = {0};
static ShopLineupParam save_loadout_shop_lineups[shop::loadout_slots] = {0};
static ShopLineupParam apply_loadout_shop_lineups[shop::loadout_slots] = {0};
static EquipParamGoods loadout_slot_goods[shop::loadout_slots] = {0};

static FindShopMenuResult *(*get_shop_menu)(FindShopMenuResult *result, byte shop_type,
                                            int32_t begin_id, int32_t end_id);

FindShopMenuResult *get_shop_menu_detour(FindShopMenuResult *result, byte shop_type,
                                         int32_t begin_id, int32_t end_id)
{
    if (begin_id == shop::save_loadout_shop_id)
    {
        result->shop_type = (byte)0;
        result->id = shop::save_loadout_shop_id;
        result->row = &save_loadout_shop_menu;
    }
    else if (begin_id == shop::apply_loadout_shop_id)
    {
        result->shop_type = (byte)0;
        result->id = shop::apply_loadout_shop_id;
        result->row = &apply_loadout_shop_menu;
    }
    else
    {
        get_shop_menu(result, shop_type, begin_id, end_id);
    }

    return result;
}

static void (*get_shop_lineup_param)(FindShopLineupParamResult *result, byte shop_type, int32_t id);

static void get_shop_lineup_param_detour(FindShopLineupParamResult *result, byte shop_type,
                                         int32_t id)
{
    if (id >= shop::save_loadout_shop_id && id < shop::save_loadout_shop_id + shop::loadout_slots)
    {
        auto loadout_slot = id - shop::save_loadout_shop_id;
        result->shop_type = shop_type;
        result->id = id;
        result->row = &save_loadout_shop_lineups[loadout_slot];
        return;
    }
    else if (id >= shop::apply_loadout_shop_id &&
             id < shop::apply_loadout_shop_id + shop::loadout_slots)
    {
        auto loadout_slot = id - shop::apply_loadout_shop_id;
        result->shop_type = shop_type;
        result->id = id;
        result->row = &apply_loadout_shop_lineups[loadout_slot];
        return;
    }

    get_shop_lineup_param(result, shop_type, id);
}

static void (*get_equip_param_goods)(FindEquipParamGoodsResult *result, int32_t id);

void get_equip_param_goods_detour(FindEquipParamGoodsResult *result, int32_t id)
{
    if (id >= shop::loadout_goods_base_id && id < shop::loadout_goods_base_id + shop::loadout_slots)
    {
        auto loadout_slot = id - shop::loadout_goods_base_id;
        result->id = id;
        result->row = &loadout_slot_goods[loadout_slot];
        result->unknown = 3;
        return;
    }

    get_equip_param_goods(result, id);
}

static void (*open_regular_shop)(void *, uint64_t, uint64_t);

static void open_regular_shop_detour(void *unk, uint64_t begin_id, uint64_t end_id)
{
    if (begin_id == shop::save_loadout_shop_id || begin_id == shop::apply_loadout_shop_id)
    {
        msg::set_active_shop(begin_id);
    }
    else
    {
        msg::set_active_shop(-1);
    }

    open_regular_shop(unk, begin_id, end_id);
}

static bool (*add_inventory_from_shop)(int32_t *new_item_id, int32_t quantity) = nullptr;

static bool add_inventory_from_shop_detour(int32_t *item_id_address, int32_t quantity)
{
    auto item_id = *item_id_address;
    if (item_id >= shop::item_type_goods_begin && item_id < shop::item_type_goods_end)
    {
        auto goods_id = item_id - shop::item_type_goods_begin;
        if (goods_id >= shop::loadout_goods_base_id &&
            goods_id < shop::loadout_goods_base_id + shop::loadout_slots)
        {
            auto loadout_slot = goods_id - shop::loadout_goods_base_id;

            spdlog::info("TODO: loadout slot {}", loadout_slot);

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

    save_loadout_shop_menu.menuTitleMsgId = msg::menu_text_save_loadout;
    save_loadout_shop_menu.menuIconId = 5;

    apply_loadout_shop_menu.menuTitleMsgId = msg::menu_text_apply_loadout;
    apply_loadout_shop_menu.menuIconId = 5;

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

    for (int loadout_slot = 0; loadout_slot < loadout_slots; loadout_slot++)
    {
        loadout_slot_goods[loadout_slot] = {
            .refId_default = -1,
            .sfxVariationId = -1,
            .weight = 1,
            .replaceItemId = -1,
            .sortId = -1,
            .appearanceReplaceItemId = -1,
            .yesNoDialogMessageId = -1,
            .potGroupId = -1,
            .iconId = loadout_slot < 4 ? uint16_t(250) : uint16_t(249),
            .compTrophySedId = -1,
            .trophySeqId = -1,
            .maxNum = 1,
            .goodsType = uint8_t(12),
            .refId_1 = -1,
            .refVirtualWepId = -1,
            .castSfxId = -1,
            .fireSfxId = -1,
            .effectSfxId = -1,
            .showLogCondType = 1,
            .showDialogCondType = 2,
            .sortGroupId = 0,
            .isUseNoAttackRegion = 1,
            .aiUseJudgeId = -1,
            .reinforceGoodsId = -1,
            .reinforceMaterialId = -1,
        };

        save_loadout_shop_lineups[loadout_slot] = {
            .equipId = loadout_goods_base_id + loadout_slot,
            .value = -1,
            .mtrlId = -1,
            .sellQuantity = -1,
            .equipType = 3,
            .setNum = 1,
            .iconId = -1,
            .nameMsgId = -1,
        };

        apply_loadout_shop_lineups[loadout_slot] = {
            .equipId = loadout_goods_base_id + loadout_slot,
            .value = -1,
            .mtrlId = -1,
            .sellQuantity = loadout_slot < 4 ? int16_t(1) : int16_t(0),
            .equipType = 3,
            .iconId = -1,
            .nameMsgId = -1,
        };
    }

    // Hook get_equip_param_goods() to return the above items
    modutils::hook(
        {
            .aob = "41 8d 50 03"        // lea edx, [r8 + 3]
                   "e8 ?? ?? ?? ??"     // call SoloParamRepositoryImp::GetParamResCap
                   "48 85 c0"           // test rax rax
                   "0f 84 ?? ?? ?? ??", // jz end_lbl
            .offset = -106,
        },
        get_equip_param_goods_detour, get_equip_param_goods);

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
