#pragma once

#include <cstddef>
#include <cstdint>
#include <tga/paramdefs.h>

#include "List.hpp"

#pragma pack(push, 1)

namespace gear_slot
{
static constexpr uint32_t left_weapon1_id = 0;
static constexpr uint32_t right_weapon1_id = 1;
static constexpr uint32_t left_weapon2_id = 2;
static constexpr uint32_t right_weapon2_id = 3;
static constexpr uint32_t left_weapon3_id = 4;
static constexpr uint32_t right_weapon3_id = 5;
static constexpr uint32_t arrow1_id = 6;
static constexpr uint32_t bolt1_id = 7;
static constexpr uint32_t arrow2_id = 8;
static constexpr uint32_t bolt2_id = 9;
static constexpr uint32_t unk1 = 10;
static constexpr uint32_t unk2 = 11;
static constexpr uint32_t head_protector_id = 12;
static constexpr uint32_t chest_protector_id = 13;
static constexpr uint32_t arms_protector_id = 14;
static constexpr uint32_t legs_protector_id = 15;
static constexpr uint32_t unk3 = 16;
static constexpr uint32_t accessory1_id = 17;
static constexpr uint32_t accessory2_id = 18;
static constexpr uint32_t accessory3_id = 19;
static constexpr uint32_t accessory4_id = 20;
static constexpr uint32_t unk4 = 21;
static constexpr uint32_t count = 22;
};

namespace CS
{
struct EquipMagicData;
struct EquipItemData;
struct ChrIns;

struct PlayerInventoryEntry
{
    uint32_t ga_item_id;
    int32_t item_id;
    uint32_t quantity;
    int32_t display_id;
    int32_t unk;
};

struct EquipInventoryData
{
    void **vftable;
    std::byte unk1[0x8];
    List<PlayerInventoryEntry> entries;
    int32_t start_index;
    std::byte unk2[0x8];
    int32_t tail_data_index;
    std::byte unk3[0x54];
    uint32_t length;
    std::byte unk6[0x1e];
};

struct ChrAsm
{
    void **vftable;
    std::byte unk1[0x74];
    int32_t gear[gear_slot::count];
};

struct EquipGameData
{
    std::byte unk1[0x70];
    ChrAsm chr_asm;
    std::byte unk2[0x14];
    EquipInventoryData equip_inventory_data;
    EquipMagicData *equip_magic_data;
    EquipItemData *equip_item_data;
    std::byte unk3[0x218];
};

struct PlayerGameData
{
    std::byte unk1[0x9c];
    wchar_t *name;
    std::byte unk2[0x20c];
    EquipGameData equip_game_data;
    std::byte unk3[0x360];
};

struct SpEffectEntry
{
    SpEffectParam *param;
    int32_t id;
    std::byte unk1[0x24];
    SpEffectEntry *next;
    std::byte unk2[0x8c];
};

struct SpEffectList
{
    void **vftable;
    SpEffectEntry *head;
};

struct ChrIns
{
    std::byte unk1[0x6C];
    std::byte team_type;
    std::byte unk2[0x10b];
    SpEffectList *speffects;
    std::byte unk3[0x3f0];
};

struct PlayerIns : ChrIns
{
    std::byte unk3[0x10];
    PlayerGameData *player_game_data;
    std::byte unk4[0x1b8];
};

struct NetPlayer
{
    PlayerIns *player;
    std::byte unk[0x8];
};

struct WorldChrManImp
{
    std::byte unk1[0x10EF8];
    NetPlayer *net_players;
    std::byte unk2[0xd608];
    PlayerIns *main_player;
};
}

#pragma pack(pop)
