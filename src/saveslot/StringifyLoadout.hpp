#include <algorithm>
#include <span>
#include <sstream>

#include "../messages/LoadoutMessages.hpp"
#include "LoadoutSaveSlot.hpp"

using namespace std;
using namespace erloadout;
using u16stringstream = basic_stringstream<char16_t>;

static void stringify_label(u16stringstream &stream, u16string_view str)
{
    stream << u"<font color=\"#c0b194\" size=\"20\">" << str << u"</font>\n";
}

static void stringify_equipment(u16stringstream &stream, uint32_t msgbnd_id, span<int> ids)
{
    bool is_first = true;
    stream << u"\u2022 ";
    for (auto id : ids)
    {
        if (id != -1)
        {
            if (!is_first)
                stream << u", ";

            auto name = msg::get_message(msgbnd_id, id);
            if (name != nullptr)
                stream << name;

            is_first = false;
        }
    }
    stream << u"\n";
}

static void stringify_equipment(u16stringstream &stream, uint32_t msgbnd_id, int id)
{
    bool is_first = true;
    stream << u"\u2022 ";
    if (id != -1)
    {
        if (!is_first)
        {
            stream << u", ";
        }

        auto name = msg::get_message(msgbnd_id, id);
        if (name != nullptr)
        {
            stream << name;
        }
        else
        {
            stream << "?";
        }

        is_first = false;
    }
    stream << u"\n";
}

inline bool has_any(const span<int> equipment_ids)
{
    return any_of(begin(equipment_ids), end(equipment_ids), [](int value) { return value != -1; });
}

static void stringify_loadout(u16stringstream &stream, saveslots::SaveSlot &slot)
{
    stream << u"<font size=\"16\">";

    bool has_right_weapon = has_any(slot.right_weapon_ids);
    bool has_left_weapon = has_any(slot.left_weapon_ids);
    if (has_right_weapon || has_left_weapon)
    {
        stringify_label(stream, msg::get_message(msg::msgbnd_menu_text, msg::menu_text_armaments));
        if (has_right_weapon)
            stringify_equipment(stream, msg::msgbnd_weapon_name, slot.right_weapon_ids);
        if (has_left_weapon)
            stringify_equipment(stream, msg::msgbnd_weapon_name, slot.left_weapon_ids);
        stream << u"\n";
    }

    bool has_arrows = has_any(slot.arrow_ids);
    bool has_bolts = has_any(slot.bolt_ids);
    if (has_arrows || has_bolts)
    {
        stringify_label(stream,
                        msg::get_message(msg::msgbnd_menu_text, msg::menu_text_arrows_bolts));
        if (has_arrows)
            stringify_equipment(stream, msg::msgbnd_weapon_name, slot.arrow_ids);
        if (has_bolts)
            stringify_equipment(stream, msg::msgbnd_weapon_name, slot.bolt_ids);
        stream << u"\n";
    }

    bool has_protectors = has_any(slot.protector_ids);
    if (has_protectors)
    {
        stringify_label(stream, msg::get_message(msg::msgbnd_menu_text, msg::menu_text_armor));
        for (auto protector_id : slot.protector_ids)
        {
            if (protector_id != -1)
                stringify_equipment(stream, msg::msgbnd_protector_name, protector_id);
        }
        stream << u"\n";
    }

    bool has_accessories = has_any(slot.accessory_ids);
    if (has_protectors)
    {
        stringify_label(stream, msg::get_message(msg::msgbnd_menu_text, msg::menu_text_talismans));
        for (auto accessory_id : slot.accessory_ids)
        {
            if (accessory_id != -1)
                stringify_equipment(stream, msg::msgbnd_accessory_name, accessory_id);
        }
        stream << u"\n";
    }
    stream << u"</font>";
}