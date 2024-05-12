#include <fstream>

#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

#include "erloadout_config.hpp"
#include "erloadout_gear_info.hpp"
#include "erloadout_loadout.hpp"
#include "erloadout_save_file.hpp"

using namespace std;
using json = nlohmann::json;
namespace fs = std::filesystem;

namespace erloadout
{
void to_json(json &j, const erloadout::loadout &loadout)
{
    if (!loadout.empty)
    {
        j = json::object();
        for (int i = 0; i < gear_slot::count; i++)
        {
            auto &info = erloadout::gearinfo[i];
            if (info.item_type == item_type::invalid)
                continue;

            j[info.name] = loadout.gear[i];
        }
    }
    else
    {
        j = json{};
    }
}

void from_json(const json &j, erloadout::loadout &loadout)
{
    if (j.is_object())
    {
        for (int i = 0; i < gear_slot::count; i++)
        {
            auto &info = erloadout::gearinfo[i];
            if (info.item_type == item_type::invalid)
                continue;

            if (j[info.name].is_number_integer())
                loadout.gear[i] = j[info.name];
            else
                loadout.gear[i] = info.default_value;
        }
    }
}
}

void erloadout::load_from_file()
{
    ifstream file(erloadout::config::save_file_path);
    if (!file.is_open())
    {
        spdlog::warn("Save file {} doesn't exist yet, will create on save",
                     erloadout::config::save_file_path.string());
        return;
    }

    try
    {
        json data_json = json::parse(file);
        json loadouts_json = data_json["loadouts"];

        for (int i = 0; i < loadouts.size(); i++)
        {
            loadouts_json[i].get_to(loadouts[i]);
        }

        spdlog::info("Save file loaded from {}", erloadout::config::save_file_path.string());
    }
    catch (const json::exception &e)
    {
        spdlog::error("Error loading {}: {}", erloadout::config::save_file_path.string(), e.what());
    }
}

void erloadout::save_to_file()
{
    json loadouts_json = json::array();

    for (int i = 0; i < loadouts.size(); i++)
    {
        loadouts_json[i] = loadouts[i];
    }

    json data_json = {{"loadouts", loadouts_json}};

    ofstream(erloadout::config::save_file_path) << data_json;

    spdlog::info("Save file written to {}", erloadout::config::save_file_path.string());
}
