#pragma once

#include "players.hpp"
#include "modutils.hpp"

using namespace std;

static CS::WorldChrManImp **world_chr_man_addr = nullptr;

typedef int GetInventoryIdFn(CS::EquipInventoryData *, int32_t *item_id);
static GetInventoryIdFn *get_inventory_id = nullptr;

players::ApplySpEffectFn *players::apply_speffect = nullptr;
players::ClearSpEffectFn *players::clear_speffect = nullptr;
players::SpawnOneShotVFXOnChrFn *players::spawn_one_shot_sfx_on_chr = nullptr;
players::EquipGearFn *players::equip_gear;
players::EquipGoodsFn *players::equip_goods;

void players::initialize()
{
    world_chr_man_addr = modutils::scan<CS::WorldChrManImp *>({
        .aob = "48 8b 05 ?? ?? ?? ??"  // mov rax, [WorldChrMan]
               "48 85 c0"              // test rax, rax
               "74 0f"                 // jz end_label
               "48 39 88 08 e5 01 00", // cmp [rax + 0x1e508], rcx
        .relative_offsets = {{3, 7}},
    });

    get_inventory_id = modutils::scan<GetInventoryIdFn>({
        .aob = "48 8d 8f 58 01 00 00" // lea rcx, [rdi + 0x158] ;
                                      // &equipGameData->equipInventoryData
               "e8 ?? ?? ?? ??"       // call GetInventoryId
               "8b d8"                // mov ebx, eax
               "85 c0"                // test eax, eax
               "78 6a",               // js label
        .offset = 7,
        .relative_offsets = {{1, 5}},
    });

    // Locate both ChrIns::ApplyEffect() and ChrIns::ClearSpEffect() from this snippet that manages
    // speffect 4270 (Disable Grace Warp)
    auto disable_enable_grace_warp_address = modutils::scan<byte>({
        .aob = "45 33 c0"        // xor r8d, r8d
               "ba ae 10 00 00"  // mov edx, 4270 ; Disable Grace Warp
               "48 8b cf"        // mov rcx, rdi
               "e8 ?? ?? ?? ??"  // call ChrIns::ApplyEffect
               "eb 16"           // jmp end_label
               "e8 ?? ?? ?? ??"  // call HasSpEffect
               "84 c0"           // test al, al
               "74 0d"           // jz end_label
               "ba ae 10 00 00"  // mov edx, 4270 ; Disable Grace Warp
               "48 8b cf"        // mov rcx, rdi
               "e8 ?? ?? ?? ??", // call ChrIns::ClearSpEffect});
    });

    apply_speffect = modutils::scan<ApplySpEffectFn>({
        .address = disable_enable_grace_warp_address + 11,
        .relative_offsets = {{1, 5}},
    });

    clear_speffect = modutils::scan<ClearSpEffectFn>({
        .address = disable_enable_grace_warp_address + 35,
        .relative_offsets = {{1, 5}},
    });

    spawn_one_shot_sfx_on_chr = modutils::scan<SpawnOneShotVFXOnChrFn>({
        .aob = "45 8b 46 04"    // mov r8d, [r14 + 0x4]
               "41 8b 16"       // mov edx, [r14]
               "48 8b 0b"       // mov rcx, [rbx]
               "e8 ?? ?? ?? ??" // call EMEVD::SpawnOneShotSFXOnChr
               "48 8d 5b 08"    // lea rbx, [rbx + 0x8]
               "48 ff c7"       // inc rdi
               "48 3b fe"       // cmp rdi, rsi
               "75 e5",         // jnz start
        .offset = 10,
        .relative_offsets = {{1, 5}},
    });

    equip_gear = modutils::scan<EquipGearFn>({
        .aob = "?? 8b f1"  // mov        esi, index
               "?? 8b d8"  // mov        rbx, item_id
               "?? 63 ea"  // movsxd     rbp, slot
               "?? 8b f9", // mov        rdi, equip_game_data
        .offset = -23,
    });

    equip_goods = modutils::scan<EquipGoodsFn>({
        .aob =
            "?? fa 0a"             // cmp slot, 10
            "?? 0f"                // jnc lab_140248d74
            "?? 81 c1 70 02 00 00" // add equip_game_data, 0x270 ; equip_game_data->equip_item_data
            "?? 8b c1"             // mov item_id, index
            "e9 ?? ?? ?? ??",      // jmp ???
    });
}

CS::PlayerIns *players::get_main_player()
{
    auto world_chr_man = *world_chr_man_addr;
    if (world_chr_man != nullptr)
    {
        return world_chr_man->main_player;
    }

    return nullptr;
}

bool players::has_item_in_inventory(uint32_t item_type, int32_t id)
{
    auto player = get_main_player();
    if (player == nullptr)
    {
        return false;
    }

    auto &equip_game_data = player->player_game_data->equip_game_data;
    auto item_id = (int32_t)item_type + id;
    return get_inventory_id(&equip_game_data.equip_inventory_data, &item_id) != -1;
}

bool players::has_speffect(CS::PlayerIns *player, int32_t speffect_id)
{
    if (player == nullptr)
    {
        return false;
    }

    for (auto entry = player->speffects->head; entry != nullptr; entry = entry->next)
    {
        if (entry->id == speffect_id)
        {
            return true;
        }
    }

    return false;
}