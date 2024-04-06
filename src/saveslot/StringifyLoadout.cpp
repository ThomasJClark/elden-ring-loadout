#include <algorithm>
#include <format>
#include <iomanip>
#include <sstream>

#include "../messages/LoadoutMessages.hpp"
#include "../utils/ParamUtils.hpp"
#include "StringifyLoadout.hpp"

using namespace std;
using namespace erloadout;

static const wstring begin_line =
    L"<img src='img://MENU_DummyTransparent.dds' width='22' height='1'/>";

static const wstring end_line = L"\n";

static const wstring vertical_spacer =
    L"<font size='1'><img src='img://MENU_DummyTransparent.dds' width='1' height='24'/>\n</font>";

static void write_header(wstringstream &stream, wstring_view str)
{
    stream << L"<font color='#c0b194' size='22'>" << str << L"</font>\n";
}

static bool write_weapons(wstringstream &stream, initializer_list<const int> weapon_ids)
{
    bool is_first = true;
    for (auto weapon_id : weapon_ids)
    {
        if (weapon_id != saveslots::unarmed_weapon_id && weapon_id != -1)
        {
            if (is_first)
                stream << begin_line;
            else
                stream << L", ";

            auto upgrade_level = weapon_id % 100;
            auto name = msg::get_message(msg::msgbnd_weapon_name, weapon_id - upgrade_level);

            if (name != nullptr)
                stream << name;

            if (upgrade_level != 0)
            {
                stream << L" +" << upgrade_level;
            }

            is_first = false;
        }
    }
    if (!is_first)
    {
        stream << end_line;
    }
    return !is_first;
}

static bool write_protectors(wstringstream &stream, initializer_list<const int> protector_ids)
{
    bool is_first = true;
    for (auto protector_id : protector_ids)
    {
        if (protector_id != saveslots::bare_head_protector_id &&
            protector_id != saveslots::bare_chest_protector_id &&
            protector_id != saveslots::bare_arms_protector_id &&
            protector_id != saveslots::bare_legs_protector_id)
        {
            if (is_first)
                stream << begin_line;
            else
                stream << L", ";

            auto name = msg::get_message(msg::msgbnd_protector_name, protector_id);
            if (name != nullptr)
                stream << name;

            is_first = false;
        }
    }
    if (!is_first)
    {
        stream << end_line;
    }
    return !is_first;
}

static bool write_accessories(wstringstream &stream, initializer_list<const int> accessory_ids)
{
    bool is_first = true;
    for (auto accessory_id : accessory_ids)
    {
        if (accessory_id != saveslots::empty_accessory_id)
        {
            if (is_first)
                stream << begin_line;
            else
                stream << L", ";

            auto name = msg::get_message(msg::msgbnd_accessory_name, accessory_id);
            if (name != nullptr)
                stream << name;

            is_first = false;
        }
    }
    if (!is_first)
    {
        stream << end_line;
    }
    return !is_first;
}

static void write_accessory(wstringstream &stream, int accessory_id)
{
    if (accessory_id != -1)
    {
        stream << begin_line << msg::get_message(msg::msgbnd_accessory_name, accessory_id)
               << end_line;
    }
}

static void write_icon(wstringstream &stream, int icon_id)
{
    wstringstream ss;
    ss << setw(5) << setfill(L'0') << icon_id;
    stream << L"<img src='img://MENU_ItemIcon_" << ss.str() << L".png' width='48' height='48'/>";
}

wstring saveslots::iconify_loadout(saveslots::SaveSlot const &slot)
{
    auto &gear = slot.gear;

    wstringstream stream;

    auto equip_param_weapon = params::get_param<EquipParamWeapon>(L"EquipParamWeapon");
    auto equip_param_protector = params::get_param<EquipParamProtector>(L"EquipParamProtector");
    auto equip_param_accessory = params::get_param<EquipParamAccessory>(L"EquipParamAccessory");

    for (auto weapon_id : {gear.right_weapon1_id, gear.right_weapon2_id, gear.right_weapon3_id})
        if (weapon_id != unarmed_weapon_id)
            write_icon(stream, equip_param_weapon[weapon_id - (weapon_id % 100)].iconId);

    for (auto weapon_id : {gear.arrow1_id, gear.arrow2_id})
        if (weapon_id != empty_ammo_id)
            write_icon(stream, equip_param_weapon[weapon_id].iconId);

    stream << L"\n";

    for (auto weapon_id : {gear.left_weapon1_id, gear.left_weapon2_id, gear.left_weapon3_id})
        if (weapon_id != unarmed_weapon_id)
            write_icon(stream, equip_param_weapon[weapon_id - (weapon_id % 100)].iconId);

    for (auto weapon_id : {gear.bolt1_id, gear.bolt2_id})
        if (weapon_id != empty_ammo_id)
            write_icon(stream, equip_param_weapon[weapon_id].iconId);

    stream << L"\n";

    if (gear.head_protector_id != bare_head_protector_id)
        write_icon(stream, equip_param_protector[gear.head_protector_id].iconIdM);

    if (gear.chest_protector_id != bare_chest_protector_id)
        write_icon(stream, equip_param_protector[gear.chest_protector_id].iconIdM);

    if (gear.arms_protector_id != bare_arms_protector_id)
        write_icon(stream, equip_param_protector[gear.arms_protector_id].iconIdM);

    if (gear.legs_protector_id != bare_legs_protector_id)
        write_icon(stream, equip_param_protector[gear.legs_protector_id].iconIdM);

    stream << L"\n";

    for (auto accessory_id :
         {gear.accessory1_id, gear.accessory2_id, gear.accessory3_id, gear.accessory4_id})
        if (accessory_id != empty_accessory_id)
            write_icon(stream, equip_param_accessory[accessory_id].iconId);

    return stream.str();
}

// Generates a string with a list of equipment in the given loadout
wstring saveslots::stringify_loadout(saveslots::SaveSlot const &slot)
{
    auto &gear = slot.gear;

    wstringstream stream;

    // Also display a categorized list of names of each piece of gear
    stream << L"<font size='18'>";

    // Armaments
    stream << L"<img src='img://MENU_Tab_Weapon.png' width='22' height='25' vspace='-25'/>";
    write_header(stream, msg::loadout_messages.armaments);
    bool any_right_weapons = write_weapons(
        stream, {gear.right_weapon1_id, gear.right_weapon2_id, gear.right_weapon3_id});
    bool any_left_weapons =
        write_weapons(stream, {gear.left_weapon1_id, gear.left_weapon2_id, gear.left_weapon3_id});
    if (!any_right_weapons && !any_left_weapons)
    {
        // Unarmed
        stream << begin_line
               << msg::get_message(msg::msgbnd_weapon_name, saveslots::unarmed_weapon_id)
               << end_line;
    }

    stream << vertical_spacer;

    if (gear.arrow1_id != empty_ammo_id || gear.arrow2_id != empty_ammo_id ||
        gear.bolt1_id != empty_ammo_id || gear.bolt2_id != empty_ammo_id)
    {
        // Arrows/Bolts
        stream << L"<img src='img://MENU_Tab_14.png' width='22' height='30' vspace='-25'/>";
        write_header(stream, msg::loadout_messages.arrows_bolts);
        write_weapons(stream, {gear.arrow1_id, gear.arrow2_id, gear.bolt1_id, gear.bolt2_id});
        stream << vertical_spacer;
    }

    // Armor
    stream << L"<img src='img://MENU_Tab_Armor.png' width='22' height='27' vspace='-25'/>";
    write_header(stream, msg::loadout_messages.armor);
    bool any_head_chest_protectors =
        write_protectors(stream, {gear.head_protector_id, gear.chest_protector_id});
    bool any_arms_legs_protectors =
        write_protectors(stream, {gear.arms_protector_id, gear.legs_protector_id});
    if (!any_head_chest_protectors && !any_arms_legs_protectors)
    {
        // None
        stream << begin_line << msg::loadout_messages.none << end_line;
    }

    stream << vertical_spacer;

    // Talismans
    stream << L"<img src='img://MENU_Tab_15.png' width='22' height='24' vspace='-25'/>";
    write_header(stream, msg::loadout_messages.talismans);
    bool any_first_two_accessories =
        write_accessories(stream, {gear.accessory1_id, gear.accessory2_id});
    bool any_second_two_accessories =
        write_accessories(stream, {gear.accessory3_id, gear.accessory4_id});
    if (!any_first_two_accessories && !any_second_two_accessories)
    {
        // None
        stream << begin_line << msg::loadout_messages.none << end_line;
    }

    stream << L"</font>";
    return stream.str();
}