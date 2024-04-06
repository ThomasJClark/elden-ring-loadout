#include <algorithm>
#include <format>
#include <iomanip>
#include <span>
#include <sstream>

#include "../messages/LoadoutMessages.hpp"
#include "StringifyLoadout.hpp"

using namespace std;
using namespace erloadout;

static const wstring begin_bullet =
    L"<img src='img://MENU_DummyTransparent.dds' width='22' height='1'/>\u2022   ";

static const wstring end_bullet = L"\n";

static const wstring vertical_spacer =
    L"<font size='1'><img src='img://MENU_DummyTransparent.dds' width='1' height='24'/>\n</font>";

static void write_header(wstringstream &stream, wstring_view str)
{
    stream << L"<font color='#c0b194' size='20'>" << str << L"</font>\n";
}

static bool write_weapons(wstringstream &stream, span<const int> weapon_ids)
{
    bool is_first = true;
    for (auto weapon_id : weapon_ids)
    {
        if (weapon_id != saveslots::unarmed_weapon_id && weapon_id != -1)
        {
            if (is_first)
                stream << begin_bullet;
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
        stream << end_bullet;
    }
    return !is_first;
}

static bool write_protector(wstringstream &stream, int protector_id)
{
    if (protector_id != saveslots::bare_head_protector_id &&
        protector_id != saveslots::bare_chest_protector_id &&
        protector_id != saveslots::bare_arms_protector_id &&
        protector_id != saveslots::bare_legs_protector_id && protector_id != -1)
    {
        stream << begin_bullet << msg::get_message(msg::msgbnd_protector_name, protector_id)
               << end_bullet;
        return true;
    }
    return false;
}

static void write_accessory(wstringstream &stream, int accessory_id)
{
    if (accessory_id != -1)
    {
        stream << begin_bullet << msg::get_message(msg::msgbnd_accessory_name, accessory_id)
               << end_bullet;
    }
}

static void write_icon(wstringstream &stream, int icon_id)
{
    wstringstream ss;
    ss << setw(5) << setfill(L'0') << icon_id;
    stream << L"<img src='img://MENU_ItemIcon_" << ss.str() << L".png' width='32' height='32'/>";
}

// Generates a string with a list of equipment in the given loadout
wstring saveslots::stringify_loadout(saveslots::SaveSlot const &slot)
{
    auto &gear = slot.gear;

    wstringstream stream;
    stream << L"<img src='img://MENU_ItemIcon_14683.png' width='40' "
              L"height='40'/>\n";
    stream << L"<font size='16'>";

    int right_weapon_ids[] = {gear.right_weapon1_id, gear.right_weapon2_id, gear.right_weapon3_id};
    int left_weapon_ids[] = {gear.left_weapon1_id, gear.left_weapon2_id, gear.left_weapon3_id};

    // Armaments
    stream << L"<img src='img://MENU_Tab_Weapon.png' width='22' height='25' vspace='-25'/>";
    write_header(stream, msg::loadout_messages.armaments);
    bool any_right_weapons = write_weapons(stream, right_weapon_ids);
    bool any_left_weapons = write_weapons(stream, left_weapon_ids);
    if (!any_right_weapons && !any_left_weapons)
    {
        // Unarmed
        stream << begin_bullet
               << msg::get_message(msg::msgbnd_weapon_name, saveslots::unarmed_weapon_id)
               << end_bullet;
    }

    stream << vertical_spacer;

    if (gear.arrow1_id != saveslots::empty_ammo_id || gear.arrow2_id != saveslots::empty_ammo_id ||
        gear.bolt1_id != saveslots::empty_ammo_id || gear.bolt2_id != saveslots::empty_ammo_id)
    {
        int arrow_ids[] = {gear.arrow1_id, gear.arrow2_id};
        int bolt_ids[] = {gear.bolt1_id, gear.bolt2_id};

        // Arrows/Bolts
        stream << L"<img src='img://MENU_Tab_14.png' width='22' height='30' vspace='-25'/> ";
        write_header(stream, msg::loadout_messages.arrows_bolts);
        write_weapons(stream, arrow_ids);
        write_weapons(stream, bolt_ids);
        stream << vertical_spacer;
    }

    // Armor
    stream << L"<img src='img://MENU_Tab_Armor.png' width='22' height='27' vspace='-25'/> ";
    write_header(stream, msg::loadout_messages.armor);
    bool any_head_protector = write_protector(stream, gear.head_protector_id);
    bool any_chest_protector = write_protector(stream, gear.chest_protector_id);
    bool any_arms_protector = write_protector(stream, gear.arms_protector_id);
    bool any_legs_protector = write_protector(stream, gear.legs_protector_id);
    if (!any_head_protector && !any_chest_protector && !any_arms_protector && !any_legs_protector)
    {
        // None
        stream << begin_bullet << msg::loadout_messages.none << end_bullet;
    }

    stream << vertical_spacer;

    if (gear.accessory1_id != -1 || gear.accessory2_id != -1 || gear.accessory3_id != -1 ||
        gear.accessory4_id != -1)
    {
        // Talismans
        stream << L"<img src='img://MENU_Tab_15.png' width='22' height='24' vspace='-25'/> ";
        write_header(stream, msg::loadout_messages.talismans);
        write_accessory(stream, gear.accessory1_id);
        write_accessory(stream, gear.accessory2_id);
        write_accessory(stream, gear.accessory3_id);
        write_accessory(stream, gear.accessory4_id);
    }

    stream << L"</font>";
    return stream.str();
}