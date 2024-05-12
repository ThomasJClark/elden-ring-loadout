/**
 * erloadout_save_file.hpp
 *
 * Functions for saving and loading loadouts from a JSON file. Loadouts are stored in a separate
 * file from the player's save file.
 */
#pragma once

namespace erloadout
{

/**
 * Write loadouts to the save file
 */
void save_to_file();

/**
 * Load saved loadouts from the save file
 */
void load_from_file();

}
