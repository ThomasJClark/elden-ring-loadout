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

static const wstring vertical_spacer =
    L"<font size='1'><img src='img://MENU_DummyTransparent.dds' width='1' height='16'/>\n</font>";

static const wstring right_weapon_placeholder =
    L"<img src='img://MENU_DummyTransparent.dds' width='2' height='48'/>"
    L"<img src='img://MENU_SL_R_Weapon' width='40' height='48'/>"
    L"<img src='img://MENU_DummyTransparent.dds' width='2' height='48'/>";

static const wstring left_weapon_placeholder =
    L"<img src='img://MENU_DummyTransparent.dds' width='8' height='48'/>"
    L"<img src='img://MENU_SL_L_Weapon' width='27' height='48'/>"
    L"<img src='img://MENU_DummyTransparent.dds' width='9' height='48'/>";

static const wstring arrow_placeholder =
    L"<img src='img://MENU_DummyTransparent.dds' width='5' height='48'/>"
    L"<img src='img://MENU_SL_Arrow' width='33' height='48'/>"
    L"<img src='img://MENU_DummyTransparent.dds' width='6' height='48'/>";

static const wstring bolt_placeholder =
    L"<img src='img://MENU_DummyTransparent.dds' width='8' height='48'/>"
    L"<img src='img://MENU_SL_Bolt' width='28' height='48'/>"
    L"<img src='img://MENU_DummyTransparent.dds' width='8' height='48'/>";

static const wstring head_placeholder =
    L"<img src='img://MENU_DummyTransparent.dds' width='5' height='48'/>"
    L"<img src='img://MENU_SL_Head' width='33' height='48'/>"
    L"<img src='img://MENU_DummyTransparent.dds' width='6' height='48'/>";

static const wstring body_placeholder =
    L"<img src='img://MENU_DummyTransparent.dds' width='5' height='48'/>"
    L"<img src='img://MENU_SL_Body' width='33' height='48'/>"
    L"<img src='img://MENU_DummyTransparent.dds' width='6' height='48'/>";

static const wstring hand_placeholder =
    L"<img src='img://MENU_DummyTransparent.dds' width='7' height='48'/>"
    L"<img src='img://MENU_SL_Hand' width='29' height='48'/>"
    L"<img src='img://MENU_DummyTransparent.dds' width='8' height='48'/>";

static const wstring foot_placeholder =
    L"<img src='img://MENU_DummyTransparent.dds' width='13' height='48'/>"
    L"<img src='img://MENU_SL_Foot' width='18' height='48'/>"
    L"<img src='img://MENU_DummyTransparent.dds' width='13' height='48'/>";

static const wstring talisman_placeholder =
    L"<img src='img://MENU_SL_Talisman' width='44' height='48'/>"
    L"<img src='img://MENU_DummyTransparent.dds' width='2' height='48'/>";

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
            auto name = msg::get_message(msgbnd::weapon_name, weapon_id - upgrade_level);

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
        stream << L"\n";
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

            auto name = msg::get_message(msgbnd::protector_name, protector_id);
            if (name != nullptr)
                stream << name;

            is_first = false;
        }
    }
    if (!is_first)
    {
        stream << L"\n";
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

            auto name = msg::get_message(msgbnd::accessory_name, accessory_id);
            if (name != nullptr)
                stream << name;

            is_first = false;
        }
    }
    if (!is_first)
    {
        stream << L"\n";
    }
    return !is_first;
}

static void write_accessory(wstringstream &stream, int accessory_id)
{
    if (accessory_id != -1)
    {
        stream << begin_line << msg::get_message(msgbnd::accessory_name, accessory_id) << L"\n";
    }
}

static wstring item_icon(int icon_id)
{
    wstringstream ss;
    ss << setw(5) << setfill(L'0');
    ss << L"<img src='img://MENU_ItemIcon_" << icon_id << L".png' width='48' height='48'/>";
    return ss.str();
}

wstring saveslots::iconify_loadout(saveslots::SaveSlot const &slot)
{
    auto &gear = slot.gear;

    wstringstream stream;

    stream << L"<font size='4'>\n</font>";

    auto equip_param_weapon = params::get_param<EquipParamWeapon>(L"EquipParamWeapon");
    auto equip_param_protector = params::get_param<EquipParamProtector>(L"EquipParamProtector");
    auto equip_param_accessory = params::get_param<EquipParamAccessory>(L"EquipParamAccessory");

    for (auto weapon_id : {gear.right_weapon1_id, gear.right_weapon2_id, gear.right_weapon3_id})
        if (weapon_id != unarmed_weapon_id)
            stream << item_icon(equip_param_weapon[weapon_id - (weapon_id % 100)].iconId);
        else
            stream << right_weapon_placeholder;

    for (auto weapon_id : {gear.arrow1_id, gear.arrow2_id})
        if (weapon_id != empty_ammo_id)
            stream << item_icon(equip_param_weapon[weapon_id].iconId);
        else
            stream << arrow_placeholder;

    stream << L"\n";

    for (auto weapon_id : {gear.left_weapon1_id, gear.left_weapon2_id, gear.left_weapon3_id})
        if (weapon_id != unarmed_weapon_id)
            stream << item_icon(equip_param_weapon[weapon_id - (weapon_id % 100)].iconId);
        else
            stream << left_weapon_placeholder;

    for (auto weapon_id : {gear.bolt1_id, gear.bolt2_id})
        if (weapon_id != empty_ammo_id)
            stream << item_icon(equip_param_weapon[weapon_id].iconId);
        else
            stream << bolt_placeholder;

    stream << L"\n";

    if (gear.head_protector_id != bare_head_protector_id)
        stream << item_icon(equip_param_protector[gear.head_protector_id].iconIdM);
    else
        stream << head_placeholder;

    if (gear.chest_protector_id != bare_chest_protector_id)
        stream << item_icon(equip_param_protector[gear.chest_protector_id].iconIdM);
    else
        stream << body_placeholder;

    if (gear.arms_protector_id != bare_arms_protector_id)
        stream << item_icon(equip_param_protector[gear.arms_protector_id].iconIdM);
    else
        stream << hand_placeholder;

    if (gear.legs_protector_id != bare_legs_protector_id)
        stream << item_icon(equip_param_protector[gear.legs_protector_id].iconIdM);
    else
        stream << foot_placeholder;

    stream << L"\n";

    for (auto accessory_id :
         {gear.accessory1_id, gear.accessory2_id, gear.accessory3_id, gear.accessory4_id})
        if (accessory_id != empty_accessory_id)
            stream << item_icon(equip_param_accessory[accessory_id].iconId);
        else
            stream << talisman_placeholder;

    return stream.str();
}

// Generates a string with a list of equipment in the given loadout
wstring saveslots::stringify_loadout(saveslots::SaveSlot const &slot)
{
    auto &gear = slot.gear;

    wstringstream stream;

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
        stream << begin_line << msg::get_message(msgbnd::weapon_name, saveslots::unarmed_weapon_id)
               << L"\n";
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
        stream << begin_line << msg::loadout_messages.none << L"\n";
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
        stream << begin_line << msg::loadout_messages.none << L"\n";
    }

    stream << L"</font>";
    return stream.str();
}