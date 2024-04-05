#include <chrono>
#include <locale>
#include <spdlog/spdlog.h>
#include <string>
#include <thread>

#include "../saveslot/LoadoutSaveSlot.hpp"
#include "../shop/LoadoutShop.hpp"
#include "../utils/ModUtils.hpp"
#include "LoadoutMessages.hpp"
#include "Messages.hpp"

using namespace erloadout;
using namespace std;

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

static int32_t active_shop_id = -1;

inline bool is_save_loadout_shop_open()
{
    return active_shop_id == shop::save_loadout_shop_id;
}

inline bool is_apply_loadout_shop_open()
{
    return active_shop_id == shop::apply_loadout_shop_id;
}

inline bool is_loadout_shop_open()
{
    return is_save_loadout_shop_open() || is_apply_loadout_shop_open();
}

static const wchar_t *(*get_message)(CS::MsgRepository *, uint32_t, uint32_t, int32_t);
static const wchar_t *get_message_detour(CS::MsgRepository *msg_repository, uint32_t unknown,
                                         uint32_t bnd_id, int32_t msg_id)
{
    switch (bnd_id)
    {
    case msg::msgbnd_event_text_for_talk:
        switch (msg_id)
        {
        case msg::event_text_for_talk_manage_loadouts:
            return msg::loadout_messages.manage_loadouts;
        case msg::event_text_for_talk_save_loadout:
            return msg::loadout_messages.save_loadout;
        case msg::event_text_for_talk_apply_loadout:
            return msg::loadout_messages.apply_loadout;
        }
        break;

    case msg::msgbnd_menu_text:
        switch (msg_id)
        {
        case msg::menu_text_save_loadout:
            return msg::loadout_messages.save_loadout;
        case msg::menu_text_apply_loadout:
            return msg::loadout_messages.apply_loadout;
        case msg::menu_text_number_held:
        case msg::menu_text_stored:
            if (is_loadout_shop_open())
                return L"";
            break;
        }
        break;

    case msg::msgbnd_line_help:
        if (msg_id == msg::line_help_select_item_for_purchase)
        {
            if (is_save_loadout_shop_open())
                return msg::loadout_messages.select_loadout_to_save;
            else if (is_apply_loadout_shop_open())
                return msg::loadout_messages.select_loadout_to_apply;
        }
        break;

    case msg::msgbnd_dialogues:
        if (msg_id == msg::dialogues_purchase_item_for_runes)
        {
            if (is_save_loadout_shop_open())
                return msg::loadout_messages.save_loadout_prompt;
            else if (is_apply_loadout_shop_open())
                return msg::loadout_messages.apply_loadout_prompt;
        }
        break;

    case msg::msgbnd_goods_name:
        if (msg_id >= shop::save_loadout_goods_base_id &&
            msg_id < shop::save_loadout_goods_base_id + saveslots::slots.size())
        {
            return saveslots::slots[msg_id - shop::save_loadout_goods_base_id].name.c_str();
        }
        if (msg_id >= shop::apply_loadout_goods_base_id &&
            msg_id < shop::apply_loadout_goods_base_id + saveslots::slots.size())
        {
            return saveslots::slots[msg_id - shop::apply_loadout_goods_base_id].name.c_str();
        }
        break;

    case msg::msgbnd_goods_caption:
        if (msg_id >= shop::save_loadout_goods_base_id &&
            msg_id < shop::save_loadout_goods_base_id + saveslots::slots.size())
        {
            return saveslots::slots[msg_id - shop::save_loadout_goods_base_id].caption.c_str();
        }
        if (msg_id >= shop::apply_loadout_goods_base_id &&
            msg_id < shop::apply_loadout_goods_base_id + saveslots::slots.size())
        {
            return saveslots::slots[msg_id - shop::apply_loadout_goods_base_id].caption.c_str();
        }
        break;

    case msg::msgbnd_goods_info:
        if (msg_id >= shop::save_loadout_goods_base_id &&
            msg_id < shop::save_loadout_goods_base_id + saveslots::slots.size())
        {
            return saveslots::slots[msg_id - shop::save_loadout_goods_base_id].info.c_str();
        }
        if (msg_id >= shop::apply_loadout_goods_base_id &&
            msg_id < shop::apply_loadout_goods_base_id + saveslots::slots.size())
        {
            return saveslots::slots[msg_id - shop::apply_loadout_goods_base_id].info.c_str();
        }
        break;
    }

    return get_message(msg_repository, unknown, bnd_id, msg_id);
}

// Set a flag to adjust some UI strings for the loadout shops, but not other shops
void erloadout::msg::set_active_shop(int32_t shop_id)
{
    active_shop_id = shop_id;
}

void erloadout::msg::initialize()
{
    auto msg_repository_address = modutils::scan<CS::MsgRepository *>({
        .aob = "48 8B 3D ?? ?? ?? ?? 44 0F B6 30 48 85 FF 75",
        .relative_offsets = {{3, 7}},
    });

    spdlog::info("Waiting for messages...");
    while (!(msg_repository = *msg_repository_address))
    {
        this_thread::sleep_for(chrono::milliseconds(100));
    }

    // Hook MsgRepositoryImp::LookupEntry() to return message strings used by the mod
    modutils::hook(
        {
            .aob = "8B DA 44 8B CA 33 D2 48 8B F9 44 8D 42 6F",
            .offset = 14,
            .relative_offsets = {{1, 5}},
        },
        get_message_detour, ::get_message);

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

const wchar_t *msg::get_message(int32_t msgbnd_id, int32_t id)
{
    return ::get_message(msg_repository, 0, msgbnd_id, id);
}
