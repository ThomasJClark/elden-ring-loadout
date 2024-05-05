#pragma once

#include <utility>

#include <fd4/component.hpp>
#include <fd4/detail/singleton.hpp>
#include <paramdefs/SP_EFFECT_PARAM_ST.hpp>

#include "List.hpp"

#pragma pack(push, 1)

namespace gear_slot
{
static constexpr unsigned int left_weapon1_id = 0;
static constexpr unsigned int right_weapon1_id = 1;
static constexpr unsigned int left_weapon2_id = 2;
static constexpr unsigned int right_weapon2_id = 3;
static constexpr unsigned int left_weapon3_id = 4;
static constexpr unsigned int right_weapon3_id = 5;
static constexpr unsigned int arrow1_id = 6;
static constexpr unsigned int bolt1_id = 7;
static constexpr unsigned int arrow2_id = 8;
static constexpr unsigned int bolt2_id = 9;
static constexpr unsigned int unk1 = 10;
static constexpr unsigned int unk2 = 11;
static constexpr unsigned int head_protector_id = 12;
static constexpr unsigned int chest_protector_id = 13;
static constexpr unsigned int arms_protector_id = 14;
static constexpr unsigned int legs_protector_id = 15;
static constexpr unsigned int unk3 = 16;
static constexpr unsigned int accessory1_id = 17;
static constexpr unsigned int accessory2_id = 18;
static constexpr unsigned int accessory3_id = 19;
static constexpr unsigned int accessory4_id = 20;
static constexpr unsigned int unk4 = 21;
static constexpr unsigned int count = 22;
};

namespace item_type
{
static constexpr unsigned int weapon = 0;
static constexpr unsigned int protector = 0x10000000;
static constexpr unsigned int accessory = 0x20000000;
static constexpr unsigned int unused = 0x30000000;
static constexpr unsigned int goods = 0x40000000;
static constexpr unsigned int gem = 0x50000000;
static constexpr unsigned int invalid = 0xfffffff;
};

static inline unsigned int get_item_type(unsigned int item_id)
{
    return item_id & 0x70000000;
}

static inline std::pair<unsigned int, unsigned int> get_weapon_id_upgrade_level(
    unsigned int weapon_id)
{
    auto upgrade_level = weapon_id % 100;
    return {weapon_id - upgrade_level, upgrade_level};
}

namespace from
{
namespace CS
{

struct EquipMagicData;
struct EquipItemData;
struct ChrIns;

struct PlayerInventoryEntry
{
    unsigned int ga_item_id;
    int item_id;
    unsigned int quantity;
    int display_id;
    int unk;
};

struct EquipInventoryData
{
    void **vftable;
    unsigned char unk1[0x8];
    List<PlayerInventoryEntry> entries;
    int start_index;
    unsigned char unk2[0x8];
    int tail_data_index;
    unsigned char unk3[0x54];
    unsigned int length;
    unsigned char unk6[0x1e];
};

struct ChrAsm
{
    void **vftable;
    unsigned char unk1[0x74];
    int gear[gear_slot::count];
};

struct EquipGameData
{
    unsigned char unk1[0x70];
    ChrAsm chr_asm;
    unsigned char unk2[0x14];
    EquipInventoryData equip_inventory_data;
    EquipMagicData *equip_magic_data;
    EquipItemData *equip_item_data;
    unsigned char unk3[0x218];
};

struct PlayerGameData
{
    unsigned char unk1[0x9c];
    wchar_t *name;
    unsigned char unk2[0x20c];
    EquipGameData equip_game_data;
    unsigned char unk3[0x360];
};

struct SpEffectEntry
{
    from::paramdefs::SP_EFFECT_PARAM_ST *param;
    int id;
    unsigned char unk1[0x24];
    SpEffectEntry *next;
    unsigned char unk2[0x8c];
};

struct SpEffectList
{
    void **vftable;
    SpEffectEntry *head;
};

struct ChrIns
{
    unsigned char unk1[0x6C];
    unsigned char team_type;
    unsigned char unk2[0x10b];
    SpEffectList *speffects;
    unsigned char unk3[0x3f0];
};

struct PlayerIns : ChrIns
{
    unsigned char unk3[0x10];
    PlayerGameData *player_game_data;
    unsigned char unk4[0x1b8];
};

struct NetPlayer
{
    PlayerIns *player;
    unsigned char unk[0x8];
};

class WorldChrManImp : public FD4::FD4ComponentBase
{
  public:
    FD4_SINGLETON_CLASS(WorldChrManImp);

    virtual ~WorldChrManImp() = 0;

    PlayerIns *get_main_player()
    {
        return main_player;
    }

  private:
    unsigned char liber_unknown[0x10EF0];
    NetPlayer *net_players;
    unsigned char liber_unknown[0xd608];
    PlayerIns *main_player;
};

}
}

#pragma pack(pop)
