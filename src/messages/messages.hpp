#pragma once

#include "LoadoutMessages.hpp"

using namespace std;
using namespace erloadout;

msg::LoadoutMessages msg::loadout_messages;

const map<string, msg::LoadoutMessages> msg::loadout_messages_by_lang = {
    {"english",
     {
         .manage_loadouts = u"Tarnished loadouts",
         .save_loadout = u"Save loadout",
         .apply_loadout = u"Apply loadout",
         .loadouts_to_save = u"Loadout slots",
         .loadouts_to_apply = u"Available loadouts",
         .select_loadout_to_save = u"Select a slot to save your current loadout",
         .select_loadout_to_apply = u"Select a loadout to apply",
         .save_loadout_prompt = u"Save current loadout to <?itemName?>?",
         .apply_loadout_prompt = u"Apply <?itemName?>?",
         .loadout = u"Loadout",
         .empty_slot = u"Empty slot",
         .press_x_to_view = u"Press <?keyicon@31?> to view equipment",
         .loadout_details = u"Loadout details",
     }},

    // Português-Brasil (Portuguese - Brazil) not yet supported
    // {"brazilian", {}}

    // Français (French) not yet supported
    // {"french", {}}

    // Deutsch (German) not yet supported
    // {"german", {}}

    // Italiano (Italian) not yet supported
    // {"italian", {}}

    // 日本語 (Japanese) not yet supported
    // {"japanese", {}}

    // 한국어 (Korean) not yet supported
    // {"koreana", {}}

    // Español-Latinoamérica (Spanish - Latin America) not yet supported
    // {"latam", {}}

    // Polski (Polish) not yet supported
    // {"polish", {}}

    // Русский (Russian) not yet supported
    // {"russian", {}}

    // 简体中文 (Simplified Chinese) not yet supported
    // {"schinese", {}}

    // Español-España (Spanish - Spain) not yet supported
    // {"spanish", {}}

    // 繁體中文 (Traditional Chinese) not yet supported
    // {"tchinese", {}}

    // ไทย (Thai) not yet supported
    // {"thai", {}}
};
