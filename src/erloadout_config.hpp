/**
 * erloadout_config.hpp
 *
 * Global configuration options for the mod
 */
#pragma once

#include <chrono>
#include <filesystem>

namespace erloadout
{

namespace config
{
extern bool debug;
extern std::chrono::milliseconds initialize_delay;
extern std::filesystem::path save_file_path;
extern bool auto_upgrade;
extern bool auto_gib;
extern bool npc_loadouts;
}

/**
 * Load the mod config from the given directory
 */
void initialize_config(const std::filesystem::path &);

/**
 * Log the config settings, for debugging
 */
void log_config();

}
