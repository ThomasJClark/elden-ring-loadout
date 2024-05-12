/**
 * erloadout_logging.hpp
 *
 * Sets up the logging configuration for the mod
 */
#pragma once

#include <filesystem>

namespace erloadout
{

/**
 * Set up the default logger, with log files stored in the given directory
 */
void initialize_logging(const std::filesystem::path &);

}
