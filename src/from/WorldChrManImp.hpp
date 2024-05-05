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

namespace from
{
namespace CS
{

class IGameDataElem
{
  public:
    virtual ~IGameDataElem() = default;
};

struct PlayerInventoryEntry
{
    unsigned int ga_item_id;
    int item_id;
    unsigned int quantity;
    int display_id;
    int liber_unknown;
};

struct EquipInventoryData : public IGameDataElem
{
    unsigned char liber_unknown[0x8];
    List<PlayerInventoryEntry> entries;
    int start_index;
    unsigned char liber_unknown[0x8];
    int tail_data_index;
    unsigned char liber_unknown[0x54];
    unsigned int length;
    unsigned char liber_unknown[0x8c];
};

LIBER_ASSERTS_BEGIN(EquipInventoryData);
LIBER_ASSERT_OFFS(0x10, entries);
LIBER_ASSERT_OFFS(0x1c, start_index);
LIBER_ASSERT_SIZE(0x110);
LIBER_ASSERTS_END;

struct ChrAsm
{
    void **vftable;
    unsigned char liber_unknown[0x74];
    int gear[gear_slot::count];
};

LIBER_ASSERTS_BEGIN(ChrAsm);
LIBER_ASSERT_OFFS(0x7c, gear);
LIBER_ASSERT_SIZE(0xd4);
LIBER_ASSERTS_END;

struct EquipGameData
{
  public:
    LIBER_CLASS(EquipGameData);

    ChrAsm &get_chr_asm()
    {
        return chr_asm;
    }

    EquipInventoryData &get_equip_inventory_data()
    {
        return equip_inventory_data;
    }

  private:
    unsigned char liber_unknown[0x70];
    ChrAsm chr_asm;
    unsigned char liber_unknown[0x14];
    EquipInventoryData equip_inventory_data;
    unsigned char liber_unknown[0x228];
};

LIBER_ASSERTS_BEGIN(EquipGameData);
LIBER_ASSERT_OFFS(0x70, chr_asm);
LIBER_ASSERT_OFFS(0x158, equip_inventory_data);
LIBER_ASSERT_SIZE(0x490);
LIBER_ASSERTS_END;

class PlayerGameData : public IGameDataElem
{
  public:
    LIBER_CLASS(PlayerGameData);

    EquipGameData &get_equip_game_data()
    {
        return equip_game_data;
    };

  private:
    unsigned char liber_unknown[0x2a8];
    EquipGameData equip_game_data;
    unsigned char liber_unknown[0x370];
};

LIBER_ASSERTS_BEGIN(PlayerGameData);
LIBER_ASSERT_OFFS(0x2b0, equip_game_data);
LIBER_ASSERT_SIZE(0xab0);
LIBER_ASSERTS_END;

class FieldInsBase
{
  public:
    virtual ~FieldInsBase() = default;

    int get_handle() const
    {
        return handle;
    }

    int get_map_id() const
    {
        return map_id;
    }

  private:
    int handle;
    int map_id;
};

class ChrIns : public FieldInsBase
{
  private:
    unsigned char liber_unknown[0x560];
};

class PlayerIns : public ChrIns
{
  public:
    LIBER_CLASS(PlayerIns);

    PlayerGameData *get_game_data()
    {
        return player_game_data;
    }

  private:
    void *liber_unknown;
    void *liber_unknown;
    PlayerGameData *player_game_data;
    unsigned char liber_unknown[0x1b8];
};

LIBER_ASSERTS_BEGIN(PlayerIns);
LIBER_ASSERT_OFFS(0x580, player_game_data);
LIBER_ASSERT_SIZE(0x740);
LIBER_ASSERTS_END;

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
    unsigned char liber_unknown[0x1e500];
    PlayerIns *main_player;
    unsigned char liber_unknown[0xed0];
};

LIBER_ASSERTS_BEGIN(WorldChrManImp);
LIBER_ASSERT_OFFS(0x1e508, main_player);
LIBER_ASSERT_SIZE(0x1f3e0);
LIBER_ASSERTS_END;

}
}

#pragma pack(pop)
