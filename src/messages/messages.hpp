#pragma once

#include "LoadoutMessages.hpp"

using namespace std;
using namespace erloadout;

msg::LoadoutMessages msg::loadout_messages;

const map<string, msg::LoadoutMessages> msg::loadout_messages_by_lang = {
    {"english",
     {
         .manage_loadouts = L"Tarnished loadouts",
         .save_loadout = L"Save loadout",
         .apply_loadout = L"Apply loadout",
         .select_loadout_to_save = L"Select a slot to save your current loadout",
         .select_loadout_to_apply = L"Select a loadout to apply",
         .save_loadout_prompt = L"Save current loadout to <?itemName?>?",
         .apply_loadout_prompt = L"Apply <?itemName?>?",
         .loadout = L"Loadout",
         .empty_slot = L"Empty slot",
         .press_x_to_view = L"Press <?keyicon@31?> to view equipment",
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
