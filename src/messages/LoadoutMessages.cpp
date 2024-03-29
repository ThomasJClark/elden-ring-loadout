#include <chrono>
#include <spdlog/spdlog.h>
#include <string>
#include <thread>

#include "../shop/LoadoutShop.hpp"
#include "../utils/ModUtils.hpp"
#include "LoadoutMessages.hpp"
#include "messages.hpp"

using namespace erloadout;
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
static u16string loadout_slot_names[shop::loadout_slots];

static int32_t active_shop_id = -1;

inline bool is_loadout_shop_open()
{
    return active_shop_id == shop::save_loadout_shop_id ||
           active_shop_id == shop::apply_loadout_shop_id;
}

static const char16_t *(*get_message)(CS::MsgRepository *, uint32_t, uint32_t, int32_t);
static const char16_t *get_message_detour(CS::MsgRepository *msg_repository, uint32_t unknown,
                                          uint32_t bnd_id, int32_t msg_id)
{
    switch (bnd_id)
    {
    case msgbnd_event_text_for_talk:
        if (msg_id == msg::event_text_for_talk_manage_loadouts)
            return loadout_messages.manage_loadouts.c_str();
        else if (msg_id == msg::event_text_for_talk_save_loadout)
            return loadout_messages.save_loadout.c_str();
        else if (msg_id == msg::event_text_for_talk_apply_loadout)
            return loadout_messages.apply_loadout.c_str();
        break;

    case msgbnd_menu_text:
        if (msg_id == msg::menu_text_save_loadout)
            return loadout_messages.save_loadout.c_str();
        else if (msg_id == msg::menu_text_apply_loadout)
            return loadout_messages.apply_loadout.c_str();

        if (is_loadout_shop_open())
        {
            if (msg_id == msg::menu_text_item_effect)
                return get_message(msg_repository, unknown, bnd_id, msg::menu_text_equipment);
            else if (msg_id == msg::menu_text_number_held || msg_id == msg::menu_text_stored)
                return u"";
        }
        break;

    case msgbnd_line_help:
        if (msg_id == msg::line_help_select_item_for_purchase)
        {
            if (is_loadout_shop_open())
                return loadout_messages.select_loadout_slot.c_str();
        }
        break;

    case msgbnd_dialogues:
        if (msg_id == msg::dialogues_purchase_item_for_runes)
        {
            if (active_shop_id == shop::save_loadout_shop_id)
                return loadout_messages.save_loadout.c_str();
            else if (active_shop_id == shop::apply_loadout_shop_id)
                return loadout_messages.apply_loadout.c_str();
        }
        break;

    case msgbnd_goods_name:
        if (msg_id >= shop::loadout_goods_base_id &&
            msg_id < shop::loadout_goods_base_id + shop::loadout_slots)
        {
            auto loadout_slot = msg_id - shop::loadout_goods_base_id;
            if (loadout_slot < 4)
            {
                return loadout_slot_names[loadout_slot].c_str();
            }
            return loadout_messages.empty_slot.c_str();
        }
        break;

    case msgbnd_goods_caption:
        if (msg_id >= shop::loadout_goods_base_id &&
            msg_id < shop::loadout_goods_base_id + shop::loadout_slots)
        {
            auto loadout_slot = msg_id - shop::loadout_goods_base_id;
            if (loadout_slot < 4)
            {
                return u"TODO more info";
            }
            return u"";
        }
        break;

    case msgbnd_goods_info:
        if (msg_id >= shop::loadout_goods_base_id &&
            msg_id < shop::loadout_goods_base_id + shop::loadout_slots)
        {
            auto loadout_slot = msg_id - shop::loadout_goods_base_id;
            if (loadout_slot == 0)
            {
                return u"\u2022 Vulgar Militia Helm\n"
                       u"\u2022 Spellblade's Trousers\n"
                       u"\u2022 Morningstar\n"
                       u"\n"
                       u"Press <?keyicon@31?> to view more info";
            }
            if (loadout_slot < 4)
            {
                return u"TODO";
            }
            return u"";
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

    spdlog::info("Waiting for loadout_messages...");
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

    for (int loadout_slot = 0; loadout_slot < shop::loadout_slots; loadout_slot++)
    {
        auto slot_str = std::to_wstring(loadout_slot + 1);
        loadout_slot_names[loadout_slot] =
            loadout_messages.loadout_slot + u" " + u16string{slot_str.begin(), slot_str.end()};
    }
}
