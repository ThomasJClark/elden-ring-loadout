#include <mini/ini.h>
#include <spdlog/spdlog.h>

#include "erloadout_config.hpp"

#if _DEBUG
#define DEBUG_DEFAULT true
#else
#define DEBUG_DEFAULT false
#endif

using namespace std;
namespace fs = std::filesystem;

static fs::path config_path;

bool erloadout::config::debug;
chrono::milliseconds erloadout::config::initialize_delay;
fs::path erloadout::config::save_file_path;
bool erloadout::config::auto_upgrade;
bool erloadout::config::auto_gib;
bool erloadout::config::npc_loadouts;

void erloadout::initialize_config(const std::filesystem::path &mod_folder)
{
    config_path = mod_folder / "erloadout.ini";
    config::debug = DEBUG_DEFAULT;
    config::initialize_delay = chrono::milliseconds(11000);
    config::save_file_path = mod_folder / "loadouts.json";

    mINI::INIFile file(config_path.string());
    mINI::INIStructure ini;

    if (file.read(ini) && ini.has("loadout"))
    {
        for (auto [key, value] : ini["loadout"])
        {
            if (key == "debug")
                config::debug = value != "false";
            else if (key == "initialize_delay")
                config::initialize_delay = chrono::milliseconds(stoi(value));
            else if (key == "save_file")
                config::save_file_path = fs::weakly_canonical(config_path.parent_path() / value);
            else if (key == "auto_upgrade")
                config::auto_upgrade = value != "false";
            else if (key == "auto_gib")
                config::auto_gib = value != "false";
            else if (key == "npc_loadouts")
                config::npc_loadouts = value != "false";
        }
    }
}

void erloadout::log_config()
{
    spdlog::info("Loaded config from {}", config_path.string());
    spdlog::info(" - debug = {}", config::debug);
    spdlog::info(" - initialize_delay = {} ", config::initialize_delay);
    spdlog::info(" - save_file = {} ", config::save_file_path.string());
    spdlog::info(" - auto_upgrade = {}", config::auto_upgrade);
    spdlog::info(" - auto_gib = {}", config::auto_gib);
    spdlog::info(" - npc_loadouts = {}", config::npc_loadouts);
}