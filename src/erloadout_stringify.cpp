#include <algorithm>
#include <format>
#include <iomanip>
#include <sstream>

#include <coresystem/param.hpp>
#include <spdlog/spdlog.h>

#include "erloadout_config.hpp"
#include "erloadout_gear_info.hpp"
#include "erloadout_ids.hpp"
#include "erloadout_messages.hpp"
#include "erloadout_stringify.hpp"
#include "utils/players.hpp"

using namespace std;
using namespace erloadout;

/**
 * @returns true if the given item cannot be equipped because the player doesn't have it, and
 * auto_gib isn't enabled.
 *
 * @todo Use the same fallback logic as the actual equip code
 */
static bool is_missing_item(unsigned int item_type, int id)
{
    if (erloadout::config::auto_gib)
    {
        return false;
    }

    auto player = from::CS::WorldChrManImp::instance().reference().get_main_player();
    if (player == nullptr)
    {
        return false;
    }

    auto &equip_game_data = player->get_game_data()->get_equip_game_data();
    auto item_id = (int)item_type + id;
    return players::get_inventory_id(&equip_game_data.get_equip_inventory_data(), &item_id) == -1;
}

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

static const wstring chest_placeholder =
    L"<img src='img://MENU_DummyTransparent.dds' width='5' height='48'/>"
    L"<img src='img://MENU_SL_Body' width='33' height='48'/>"
    L"<img src='img://MENU_DummyTransparent.dds' width='6' height='48'/>";

static const wstring arms_placeholder =
    L"<img src='img://MENU_DummyTransparent.dds' width='7' height='48'/>"
    L"<img src='img://MENU_SL_Hand' width='29' height='48'/>"
    L"<img src='img://MENU_DummyTransparent.dds' width='8' height='48'/>";

static const wstring legs_placeholder =
    L"<img src='img://MENU_DummyTransparent.dds' width='13' height='48'/>"
    L"<img src='img://MENU_SL_Foot' width='18' height='48'/>"
    L"<img src='img://MENU_DummyTransparent.dds' width='13' height='48'/>";

static const wstring accessory_placeholder =
    L"<img src='img://MENU_SL_Talisman' width='44' height='48'/>"
    L"<img src='img://MENU_DummyTransparent.dds' width='2' height='48'/>";

static void write_header(wstringstream &stream, wstring_view str)
{
    stream << L"<font color='#c0b194' size='22'>" << str << L"</font>\n";
}

static void write_item_name(wstringstream &stream, const wchar_t *str, bool is_first,
                            bool is_missing_item)
{
    if (is_first)
        stream << begin_line;
    else
        stream << L",</font> ";

    if (str == nullptr)
        stream << L"<font color='#c5242a'>???";
    else if (is_missing_item)
        stream << L"<font color='#c5242a'>" << str;
    else
        stream << L"<font color='#ffffff'>" << str;
}

static bool write_weapons(wstringstream &stream, initializer_list<const int> weapon_ids)
{
    bool is_first = true;
    for (auto weapon_id : weapon_ids)
    {
        if (weapon_id != unarmed_weapon_id && weapon_id != -1)
        {
            auto upgrade_level = weapon_id % 100;

            write_item_name(stream,
                            msg::get_message(msgbnd::weapon_name, weapon_id - upgrade_level),
                            is_first, is_missing_item(item_type::weapon, weapon_id));

            if (upgrade_level != 0)
            {
                stream << L" +" << upgrade_level;
            }

            is_first = false;
        }
    }
    if (!is_first)
    {
        stream << L"</font>\n";
    }
    return !is_first;
}

static bool write_protectors(wstringstream &stream, initializer_list<const int> protector_ids)
{
    bool is_first = true;
    for (auto protector_id : protector_ids)
    {
        if (protector_id != bare_head_protector_id && protector_id != bare_chest_protector_id &&
            protector_id != bare_arms_protector_id && protector_id != bare_legs_protector_id)
        {
            write_item_name(stream, msg::get_message(msgbnd::protector_name, protector_id),
                            is_first, is_missing_item(item_type::protector, protector_id));

            is_first = false;
        }
    }
    if (!is_first)
    {
        stream << L"</font>\n";
    }
    return !is_first;
}

static bool write_accessories(wstringstream &stream, initializer_list<const int> accessory_ids)
{
    bool is_first = true;
    for (auto accessory_id : accessory_ids)
    {
        if (accessory_id != empty_accessory_id)
        {
            write_item_name(stream, msg::get_message(msgbnd::accessory_name, accessory_id),
                            is_first, is_missing_item(item_type::accessory, accessory_id));

            is_first = false;
        }
    }
    if (!is_first)
    {
        stream << L"</font>\n";
    }
    return !is_first;
}

static wstring item_icon(int icon_id)
{
    wstringstream ss;
    ss << setw(5) << setfill(L'0');
    ss << L"<img src='img://MENU_ItemIcon_" << icon_id << L".png' width='48' height='48'/>";
    return ss.str();
}

wstring erloadout::iconify_loadout(loadout const &loadout)
{

    auto &gear = loadout.gear;

    wstringstream stream;

    stream << L"<font size='4'>\n</font>";

    auto params_ref = from::CS::SoloParamRepositoryImp::instance();
    if (!params_ref)
    {
        spdlog::error("No CS::SoloParamRepositoryImp instance");
        return L"";
    }
    auto &params = params_ref.reference();

    auto get_weapon_icon_id = [&params](unsigned long long weapon_id) {
        auto row = params.get_equip_param_weapon()->get_row_by_id(weapon_id);
        return row == nullptr ? -1 : row->iconId;
    };
    auto get_protector_icon_id = [&params](unsigned long long protector_id) {
        auto row = params.get_equip_param_protector()->get_row_by_id(protector_id);
        return row == nullptr ? -1 : row->iconIdM;
    };
    auto get_accessory_icon_id = [&params](unsigned long long accessory_id) {
        auto row = params.get_equip_param_accessory()->get_row_by_id(accessory_id);
        return row == nullptr ? -1 : row->iconId;
    };

    for (auto gear_slot :
         {gear_slot::right_weapon1_id, gear_slot::right_weapon2_id, gear_slot::right_weapon3_id})
    {
        auto weapon_id = gear[gear_slot];
        if (weapon_id != gearinfo[gear_slot].default_value)
            stream << item_icon(get_weapon_icon_id(weapon_id - (weapon_id % 100)));
        else
            stream << right_weapon_placeholder;
    }

    for (auto gear_slot : {gear_slot::arrow1_id, gear_slot::arrow2_id})
    {
        auto weapon_id = gear[gear_slot];
        if (weapon_id != gearinfo[gear_slot].default_value)
            stream << item_icon(get_weapon_icon_id(weapon_id));
        else
            stream << arrow_placeholder;
    }

    stream << L"\n";

    for (auto gear_slot :
         {gear_slot::left_weapon1_id, gear_slot::left_weapon2_id, gear_slot::left_weapon3_id})
    {
        auto weapon_id = gear[gear_slot];
        if (weapon_id != gearinfo[gear_slot].default_value)
            stream << item_icon(get_weapon_icon_id(weapon_id - (weapon_id % 100)));
        else
            stream << left_weapon_placeholder;
    }

    for (auto gear_slot : {gear_slot::bolt1_id, gear_slot::bolt2_id})
    {
        auto weapon_id = gear[gear_slot];
        if (weapon_id != gearinfo[gear_slot].default_value)
            stream << item_icon(get_weapon_icon_id(weapon_id));
        else
            stream << bolt_placeholder;
    }

    stream << L"\n";

    for (auto gear_slot : {gear_slot::head_protector_id, gear_slot::chest_protector_id,
                           gear_slot::arms_protector_id, gear_slot::legs_protector_id})
    {
        if (gear[gear_slot] != gearinfo[gear_slot].default_value)
            stream << item_icon(get_protector_icon_id(gear[gear_slot]));
        else if (gear_slot == gear_slot::head_protector_id)
            stream << head_placeholder;
        else if (gear_slot == gear_slot::chest_protector_id)
            stream << chest_placeholder;
        else if (gear_slot == gear_slot::arms_protector_id)
            stream << arms_placeholder;
        else if (gear_slot == gear_slot::legs_protector_id)
            stream << legs_placeholder;
    }

    stream << L"\n";

    for (auto gear_slot : {gear_slot::accessory1_id, gear_slot::accessory2_id,
                           gear_slot::accessory3_id, gear_slot::accessory4_id})
    {
        auto accessory_id = gear[gear_slot];
        if (accessory_id != gearinfo[gear_slot].default_value)
            stream << item_icon(get_accessory_icon_id(accessory_id));
        else
            stream << accessory_placeholder;
    }

    return stream.str();
}

// Generates a string with a list of equipment in the given loadout
wstring erloadout::stringify_loadout(loadout const &loadout)
{
    auto &gear = loadout.gear;

    wstringstream stream;

    stream << L"<font size='18'>";

    // Armaments
    stream << L"<img src='img://MENU_Tab_Weapon.png' width='22' height='25' vspace='-25'/>";
    write_header(stream, msg::loadout_messages.armaments);
    bool any_right_weapons =
        write_weapons(stream, {gear[gear_slot::right_weapon1_id], gear[gear_slot::right_weapon2_id],
                               gear[gear_slot::right_weapon3_id]});
    bool any_left_weapons =
        write_weapons(stream, {gear[gear_slot::left_weapon1_id], gear[gear_slot::left_weapon2_id],
                               gear[gear_slot::left_weapon3_id]});
    if (!any_right_weapons && !any_left_weapons)
    {
        // Unarmed
        stream << begin_line << msg::get_message(msgbnd::weapon_name, unarmed_weapon_id) << L"\n";
    }

    stream << vertical_spacer;

    if (gear[gear_slot::arrow1_id] != empty_ammo_id ||
        gear[gear_slot::arrow2_id] != empty_ammo_id || gear[gear_slot::bolt1_id] != empty_ammo_id ||
        gear[gear_slot::bolt2_id] != empty_ammo_id)
    {
        // Arrows/Bolts
        stream << L"<img src='img://MENU_Tab_14.png' width='22' height='30' vspace='-25'/>";
        write_header(stream, msg::loadout_messages.arrows_bolts);
        write_weapons(stream, {gear[gear_slot::arrow1_id], gear[gear_slot::arrow2_id],
                               gear[gear_slot::bolt1_id], gear[gear_slot::bolt2_id]});
        stream << vertical_spacer;
    }

    // Armor
    stream << L"<img src='img://MENU_Tab_Armor.png' width='22' height='27' vspace='-25'/>";
    write_header(stream, msg::loadout_messages.armor);
    bool any_head_chest_protectors = write_protectors(
        stream, {gear[gear_slot::head_protector_id], gear[gear_slot::chest_protector_id]});
    bool any_arms_legs_protectors = write_protectors(
        stream, {gear[gear_slot::arms_protector_id], gear[gear_slot::legs_protector_id]});
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
        write_accessories(stream, {gear[gear_slot::accessory1_id], gear[gear_slot::accessory2_id]});
    bool any_second_two_accessories =
        write_accessories(stream, {gear[gear_slot::accessory3_id], gear[gear_slot::accessory4_id]});
    if (!any_first_two_accessories && !any_second_two_accessories)
    {
        // None
        stream << begin_line << msg::loadout_messages.none << L"\n";
    }

    stream << L"</font>";
    return stream.str();
}