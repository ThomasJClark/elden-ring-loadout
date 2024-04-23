#define WIN32_LEAN_AND_MEAN
#include <cstdio>
#include <filesystem>
#include <memory>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
#include <stdexcept>
#include <thread>
#include <windows.h>

#include "erloadout_messages.hpp"
#include "erloadout_saveslot.hpp"
#include "erloadout_shop.hpp"
#include "erloadout_talkscript.hpp"
#include "utils/modutils.hpp"
#include "utils/params.hpp"
#include "utils/players.hpp"

using namespace std;

thread mod_thread;

void setup_logger(const filesystem::path &logs_path)
{
    auto logger = make_shared<spdlog::logger>("loadout");
    logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%n] %^[%l]%$ %v");
    logger->sinks().push_back(
        make_shared<spdlog::sinks::daily_file_sink_st>(logs_path.string(), 0, 0, false, 5));
    logger->flush_on(spdlog::level::info);

#if _DEBUG
    AllocConsole();
    FILE *stream;
    freopen_s(&stream, "CONOUT$", "w", stdout);
    freopen_s(&stream, "CONOUT$", "w", stderr);
    freopen_s(&stream, "CONIN$", "r", stdin);
    logger->sinks().push_back(make_shared<spdlog::sinks::stdout_color_sink_st>());
    logger->set_level(spdlog::level::trace);
#endif

    spdlog::set_default_logger(logger);
}

bool WINAPI DllMain(HINSTANCE dll_instance, uint32_t fdw_reason, void *lpv_reserved)
{
    if (fdw_reason == DLL_PROCESS_ATTACH)
    {
        wchar_t dll_filename[MAX_PATH] = {0};
        GetModuleFileNameW(dll_instance, dll_filename, MAX_PATH);
        auto folder = filesystem::path(dll_filename).parent_path();

        setup_logger(folder / "logs" / "erloadout.log");

#ifdef PROJECT_VERSION
        spdlog::info("Loadout version {}", PROJECT_VERSION);
#endif

        mod_thread = thread([]() {
            try
            {
                modutils::initialize();
                params::initialize();
                players::initialize();

                this_thread::sleep_for(chrono::milliseconds(11000));

                spdlog::info("Hooking talk scripts...");
                erloadout::talkscript::initialize();

                spdlog::info("Hooking loadout messages...");
                erloadout::msg::initialize();

                erloadout::saveslots::initialize();

                spdlog::info("Adding loadout shops...");
                erloadout::shop::initialize();

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