#define WIN32_LEAN_AND_MEAN

#include <memory>

#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
#include <windows.h>

#include "erloadout_config.hpp"
#include "erloadout_logging.hpp"

using namespace std;

void erloadout::initialize_logging(const std::filesystem::path &mod_folder)
{
    auto logger = make_shared<spdlog::logger>("loadout");
    logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%n] %^[%l]%$ %v");
    logger->sinks().push_back(make_shared<spdlog::sinks::daily_file_sink_st>(
        (mod_folder / "logs" / "erloadout.log").string(), 0, 0, false, 5));
    logger->flush_on(spdlog::level::info);

    if (erloadout::config::debug)
    {
        AllocConsole();
        FILE *stream;
        freopen_s(&stream, "CONOUT$", "w", stdout);
        freopen_s(&stream, "CONOUT$", "w", stderr);
        freopen_s(&stream, "CONIN$", "r", stdin);
        logger->sinks().push_back(make_shared<spdlog::sinks::stdout_color_sink_st>());
        logger->set_level(spdlog::level::trace);
    }

    spdlog::set_default_logger(logger);
}