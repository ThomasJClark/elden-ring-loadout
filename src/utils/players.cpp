#pragma once

#include <spdlog/spdlog.h>

#include "modutils.hpp"
#include "players.hpp"

using namespace std;

players::SpawnOneShotVFXOnChrFn *players::spawn_one_shot_sfx_on_chr = nullptr;
players::EquipGearFn *players::equip_gear = nullptr;
players::EquipGoodsFn *players::equip_goods = nullptr;
players::GetInventoryIdFn *players::get_inventory_id = nullptr;
players::ItemGibFn *players::item_gib = nullptr;

void players::initialize()
{
    item_gib = modutils::scan<ItemGibFn>({
        .aob = "8B 02"     // mov eax, [item_gib_list]
               "83 F8 0A", // cmp eax, 10
        .offset = -82,
    });

    get_inventory_id = modutils::scan<GetInventoryIdFn>({
        .aob = "48 8d 8f 58 01 00 00" // lea rcx, [rdi + 0x158] ; &equipGameData->equipInventoryData
               "e8 ?? ?? ?? ??"       // call GetInventoryId
               "8b d8"                // mov ebx, eax
               "85 c0"                // test eax, eax
               "78 6a",               // js label
        .offset = 7,
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
