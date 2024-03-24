#pragma once

#include <map>
#include <string>

using namespace std;

struct Messages
{
    u16string manage_loadouts;
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
