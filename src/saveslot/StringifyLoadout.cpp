#include <algorithm>
#include <format>
#include <span>
#include <sstream>

#include "../messages/LoadoutMessages.hpp"
#include "StringifyLoadout.hpp"

using namespace std;
using namespace erloadout;

using u16stringstream = basic_stringstream<char16_t>;

static const u16string begin_bullet = u"\u2022 ";
static const u16string end_bullet = u"\n";

static void write_header(u16stringstream &stream, u16string_view str)
{
    stream << u"<font color=\"#c0b194\" size=\"20\">" << str << u"</font>\n";
}

static bool write_weapons(u16stringstream &stream, span<const int> weapon_ids)
{
    bool is_first = true;
    for (auto weapon_id : weapon_ids)
    {
        if (weapon_id != saveslots::unarmed_weapon_id && weapon_id != -1)
        {
            if (is_first)
                stream << begin_bullet;
            else
                stream << u", ";

            auto upgrade_level = weapon_id % 100;
            auto name = msg::get_message(msg::msgbnd_weapon_name, weapon_id - upgrade_level);

            if (name != nullptr)
                stream << name;

            if (upgrade_level != 0)
            {
                auto upgrade_level_str = std::to_wstring(upgrade_level);
                stream << u" +" << u16string{upgrade_level_str.begin(), upgrade_level_str.end()};
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

static bool write_protector(u16stringstream &stream, int protector_id)
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

static void write_accessory(u16stringstream &stream, int accessory_id)
{
    if (accessory_id != -1)
    {
        stream << begin_bullet << msg::get_message(msg::msgbnd_accessory_name, accessory_id)
               << end_bullet;
    }
}

// Generates a string with a list of equipment in the given loadout
u16string saveslots::stringify_loadout(saveslots::SaveSlot const &slot)
{
    u16stringstream stream;
    stream << u"<font size=\"16\">";

    // "Armaments"
    write_header(stream, msg::get_message(msg::msgbnd_menu_text, msg::menu_text_armaments));
    bool any_right_weapons = write_weapons(stream, slot.right_weapon_ids);
    bool any_left_weapons = write_weapons(stream, slot.left_weapon_ids);
    if (!any_right_weapons && !any_left_weapons)
    {
        // "Unarmed"
        stream << begin_bullet
               << msg::get_message(msg::msgbnd_weapon_name, saveslots::unarmed_weapon_id)
               << end_bullet;
    }

    stream << u"\n";

    if (slot.arrow_id1 != saveslots::empty_ammo_id || slot.arrow_id2 != saveslots::empty_ammo_id ||
        slot.bolt_id1 != saveslots::empty_ammo_id || slot.bolt_id2 != saveslots::empty_ammo_id)
    {
        int arrow_ids[] = {slot.arrow_id1, slot.arrow_id2};
        int bolt_ids[] = {slot.bolt_id1, slot.bolt_id2};

        // "Arrows/Bolts"
        write_header(stream, msg::get_message(msg::msgbnd_menu_text, msg::menu_text_arrows_bolts));
        write_weapons(stream, arrow_ids);
        write_weapons(stream, bolt_ids);

        stream << u"\n";
    }

    // "Armor"
    write_header(stream, msg::get_message(msg::msgbnd_menu_text, msg::menu_text_armor));
    bool any_head_protector = write_protector(stream, slot.head_protector_id);
    bool any_chest_protector = write_protector(stream, slot.chest_protector_id);
    bool any_arms_protector = write_protector(stream, slot.arms_protector_id);
    bool any_legs_protector = write_protector(stream, slot.legs_protector_id);
    if (!any_head_protector && !any_chest_protector && !any_arms_protector && !any_legs_protector)
    {
        // "None"
        stream << begin_bullet << msg::get_message(msg::msgbnd_menu_text, msg::menu_text_none)
               << end_bullet;
    }

    stream << u"\n";

    if (slot.accessory_ids[0] != -1 || slot.accessory_ids[1] != -1 || slot.accessory_ids[2] != -1 ||
        slot.accessory_ids[3] != -1)
    {
        // "Talismans"
        write_header(stream, msg::get_message(msg::msgbnd_menu_text, msg::menu_text_talismans));
        write_accessory(stream, slot.accessory_ids[0]);
        write_accessory(stream, slot.accessory_ids[1]);
        write_accessory(stream, slot.accessory_ids[2]);
        write_accessory(stream, slot.accessory_ids[3]);
        stream << u"\n";
    }

    stream << u"</font>";
    return stream.str();
}