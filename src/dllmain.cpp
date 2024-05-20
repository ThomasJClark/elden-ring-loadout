#define WIN32_LEAN_AND_MEAN
#include <filesystem>
#include <spdlog/spdlog.h>
#include <thread>
#include <windows.h>

#include "erloadout_config.hpp"
#include "erloadout_logging.hpp"
#include "erloadout_messages.hpp"
#include "erloadout_save_file.hpp"
#include "erloadout_shop.hpp"
#include "erloadout_talkscript.hpp"
#include "utils/modutils.hpp"
#include "utils/players.hpp"

using namespace std;
namespace fs = std::filesystem;

thread mod_thread;

bool WINAPI DllMain(HINSTANCE dll_instance, unsigned int fdw_reason, void *lpv_reserved)
{
    if (fdw_reason == DLL_PROCESS_ATTACH)
    {
        wchar_t dll_filename[MAX_PATH] = {0};
        GetModuleFileNameW(dll_instance, dll_filename, MAX_PATH);
        auto dll_folder = fs::path(dll_filename).parent_path();

        erloadout::initialize_config(dll_folder);
        erloadout::initialize_logging(dll_folder / "logs" / "erloadout.log");

#ifdef PROJECT_VERSION
        spdlog::info("Loadout version {}", PROJECT_VERSION);
#endif

        erloadout::log_config();

        mod_thread = thread([dll_folder]() {
            try
            {
                modutils::initialize();
                players::initialize();

                spdlog::info("Loading save file...");
                erloadout::load_from_file();

                this_thread::sleep_for(erloadout::config::initialize_delay);

                spdlog::info("Hooking talk scripts...");
                erloadout::talkscript::initialize();

                spdlog::info("Hooking loadout messages...");
                erloadout::msg::initialize();

                spdlog::info("Adding loadout shops...");
                erloadout::initialize_shop();

                modutils::enable_hooks();

                spdlog::info("Initialized loadout mod");
            }
            catch (runtime_error const &e)
            {
                spdlog::error("Error initializing mod: {}", e.what());
                modutils::deinitialize();
            }
        });
    }
    else if (fdw_reason == DLL_PROCESS_DETACH && lpv_reserved != nullptr)
    {
        try
        {
            mod_thread.join();
            modutils::deinitialize();
            spdlog::info("Deinitialized mod");
        }
        catch (runtime_error const &e)
        {
            spdlog::error("Error deinitializing mod: {}", e.what());
            spdlog::shutdown();
            return false;
        }
        spdlog::shutdown();
    }
    return true;
}