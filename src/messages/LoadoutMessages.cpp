#include <chrono>
#include <spdlog/spdlog.h>
#include <string>
#include <thread>

#include "../ModUtils.hpp"
#include "LoadoutMessages.hpp"
#include "messages.hpp"

using namespace LoadoutMessages;
using namespace std;

static const uint32_t msgbnd_goods_name = 10;
static const uint32_t msgbnd_protector_name = 12;
static const uint32_t msgbnd_goods_info = 20;
static const uint32_t msgbnd_goods_caption = 24;
static const uint32_t msgbnd_protector_caption = 26;
static const uint32_t msgbnd_event_text_for_talk = 33;
static const uint32_t msgbnd_menu_text = 200;
static const uint32_t msgbnd_line_help = 201;
static const uint32_t msgbnd_system_message_win64 = 203;
static const uint32_t msgbnd_dialogues = 204;

struct ISteamApps;
extern "C" __declspec(dllimport) ISteamApps *__cdecl SteamAPI_SteamApps_v008();
extern "C" __declspec(dllimport) const
    char *__cdecl SteamAPI_ISteamApps_GetCurrentGameLanguage(ISteamApps *);

/**
 * Return the player's selected language using the Steamworks SDK
 *
 * https://partner.steamgames.com/doc/api/ISteamApps#GetCurrentGameLanguage
 */
static string get_steam_language()
{
    auto steam_api = SteamAPI_SteamApps_v008();
    auto steam_language = SteamAPI_ISteamApps_GetCurrentGameLanguage(steam_api);
    return steam_language != nullptr ? steam_language : "";
}

namespace CS
{
struct MsgRepository;
}
static CS::MsgRepository *msg_repository = nullptr;

/**
 * Assigned the list of localized messages based on the player's language preference
 */
static Messages loadout_messages;

static int32_t active_shop_id = -1;

static const char16_t *(*get_message)(CS::MsgRepository *, uint32_t, uint32_t, int32_t);
static const char16_t *get_message_detour(CS::MsgRepository *msg_repository, uint32_t unknown,
                                          uint32_t bnd_id, int32_t msg_id)
{
    switch (bnd_id)
    {
    case msgbnd_event_text_for_talk:
        if (msg_id == EventTextForTalk::manage_loadouts)
            return loadout_messages.manage_loadouts.c_str();
        else if (msg_id == EventTextForTalk::save_loadout)
            return loadout_messages.save_loadout.c_str();
        else if (msg_id == EventTextForTalk::apply_loadout)
            return loadout_messages.apply_loadout.c_str();
        break;

    case msgbnd_menu_text:
        if (msg_id == MenuText::save_loadout)
            return loadout_messages.save_loadout.c_str();
        else if (msg_id == MenuText::apply_loadout)
            return loadout_messages.apply_loadout.c_str();
        break;

    case msgbnd_line_help:
        if (msg_id == LineHelp::select_item_for_purchase)
        {
            if (active_shop_id == 3900000 || active_shop_id == 3900100)
                return loadout_messages.select_loadout_slot.c_str();
        }
        break;

    case msgbnd_dialogues:
        if (msg_id == Dialogues::purchase_item_for_runes)
        {
            if (active_shop_id == 3900000)
                return loadout_messages.save_loadout.c_str();
            else if (active_shop_id == 3900100)
                return loadout_messages.apply_loadout.c_str();
        }
        break;

    case msgbnd_goods_name:
        break;

    case msgbnd_goods_caption:
        break;

    case msgbnd_goods_info:
        break;
    }

    return get_message(msg_repository, unknown, bnd_id, msg_id);
}

// Set a flag to adjust some UI strings for the loadout shops, but not other shops
void LoadoutMessages::set_active_shop(int32_t shop_id)
{
    active_shop_id = shop_id;
}

void LoadoutMessages::initialize()
{
    auto msg_repository_address = ModUtils::scan<CS::MsgRepository *>({
        .aob = "48 8B 3D ?? ?? ?? ?? 44 0F B6 30 48 85 FF 75",
        .relative_offsets = {{3, 7}},
    });

    spdlog::info("Waiting for loadout_messages...");
    while (!(msg_repository = *msg_repository_address))
    {
        this_thread::sleep_for(chrono::milliseconds(100));
    }

    // Hook MsgRepositoryImp::LookupEntry() to return message strings used by the mod
    ModUtils::hook(
        {
            .aob = "8B DA 44 8B CA 33 D2 48 8B F9 44 8D 42 6F",
            .offset = 14,
            .relative_offsets = {{1, 5}},
        },
        get_message_detour, get_message);

    // Pick the messages to use based on the player's selected language for the game in Steam
    auto language = get_steam_language();

    auto messages_iterator = loadout_messages_by_lang.find(language);
    if (messages_iterator == loadout_messages_by_lang.end())
    {
        spdlog::warn("Detected game language = {} (not supported, defaulting to English)",
                     language);
        loadout_messages = loadout_messages_by_lang.at("english");
    }
    else
    {
        spdlog::info("Detected game language = {}", language);
        loadout_messages = messages_iterator->second;
    }
}
