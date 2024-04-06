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
    wstringstream stream;
    stream << L"<img src='img://MENU_ItemIcon_14683.png' width='40' "
              L"height='40'/>\n";
    stream << L"<font size='16'>";

    // Armaments
    stream << L"<img src='img://MENU_Tab_Weapon.png' width='22' height='25' vspace='-25'/>";
    write_header(stream, msg::loadout_messages.armaments);
    bool any_right_weapons = write_weapons(stream, slot.right_weapon_ids);
    bool any_left_weapons = write_weapons(stream, slot.left_weapon_ids);
    if (!any_right_weapons && !any_left_weapons)
    {
        // Unarmed
        stream << begin_bullet
               << msg::get_message(msg::msgbnd_weapon_name, saveslots::unarmed_weapon_id)
               << end_bullet;
    }

    stream << vertical_spacer;

    if (slot.arrow_id1 != saveslots::empty_ammo_id || slot.arrow_id2 != saveslots::empty_ammo_id ||
        slot.bolt_id1 != saveslots::empty_ammo_id || slot.bolt_id2 != saveslots::empty_ammo_id)
    {
        int arrow_ids[] = {slot.arrow_id1, slot.arrow_id2};
        int bolt_ids[] = {slot.bolt_id1, slot.bolt_id2};

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
    bool any_head_protector = write_protector(stream, slot.head_protector_id);
    bool any_chest_protector = write_protector(stream, slot.chest_protector_id);
    bool any_arms_protector = write_protector(stream, slot.arms_protector_id);
    bool any_legs_protector = write_protector(stream, slot.legs_protector_id);
    if (!any_head_protector && !any_chest_protector && !any_arms_protector && !any_legs_protector)
    {
        // None
        stream << begin_bullet << msg::loadout_messages.none << end_bullet;
    }

    stream << vertical_spacer;

    if (slot.accessory_ids[0] != -1 || slot.accessory_ids[1] != -1 || slot.accessory_ids[2] != -1 ||
        slot.accessory_ids[3] != -1)
    {
        // Talismans
        stream << L"<img src='img://MENU_Tab_15.png' width='22' height='24' vspace='-25'/> ";
        write_header(stream, msg::loadout_messages.talismans);
        write_accessory(stream, slot.accessory_ids[0]);
        write_accessory(stream, slot.accessory_ids[1]);
        write_accessory(stream, slot.accessory_ids[2]);
        write_accessory(stream, slot.accessory_ids[3]);
    }

    stream << L"</font>";
    return stream.str();
}