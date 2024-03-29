#pragma once

#include <map>
#include <string>

using namespace std;

struct Messages
{
    u16string manage_loadouts;
    u16string save_loadout;
    u16string apply_loadout;
    u16string select_loadout_slot;
    u16string loadout_slot;
    u16string empty_slot;
    u16string press_x_to_view;
    u16string equipment;
};

/**
 * Map of Steam language names to messages used by the mod
 *
 * https://partner.steamgames.com/doc/store/localization/languages
 */
static const map<string, Messages> loadout_messages_by_lang = {
    {"english",
     {
         .manage_loadouts = u"Manage loadouts",
         .save_loadout = u"Save loadout",
         .apply_loadout = u"Apply loadout",
         .select_loadout_slot = u"Select loadout slot",
         .loadout_slot = u"Loadout",
         .empty_slot = u"Empty",
         .press_x_to_view = u"Press <?keyicon@31?> to view more info",
         .equipment = u"Equipment",
     }},

    // 简体中文 (Simplified Chinese) not yet supported
    // {"schinese", {}}

    // 繁體中文 (Traditional Chinese) not yet supported
    // {"tchinese", {}}

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

    // Polski (Polish) not yet supported
    // {"polish", {}}

    // Português-Brasil (Portuguese - Brazil) not yet supported
    // {"brazilian", {}}

    // Русский (Russian) not yet supported
    // {"russian", {}}

    // Español-España (Spanish - Spain) not yet supported
    // {"spanish", {}}

    // Español-Latinoamérica (Spanish - Latin America) not yet supported
    // {"latam", {}}

    // ไทย (Thai) not yet supported
    // {"thai", {}}
};
