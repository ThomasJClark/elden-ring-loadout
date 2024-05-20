#include <array>
#include <string>

#include "erloadout_ids.hpp"
#include "from/WorldChrManImp.hpp"

namespace erloadout
{
struct gearinfotype
{
    /**
     * The name of this slot for logging (e.g. "left_weapon1")
     */
    std::string name;

    /**
     * The type of item in this slot (weapon, armor, etc.)
     */
    unsigned int item_type;

    /**
     * The "empty" value for this slot when nothing is equipped
     */
    int default_value;
};

static std::array<gearinfotype, gear_slot::count> gearinfo{
    gearinfotype{"left_weapon1", item_type::weapon, unarmed_weapon_id},
    gearinfotype{"right_weapon1", item_type::weapon, unarmed_weapon_id},
    gearinfotype{"left_weapon2", item_type::weapon, unarmed_weapon_id},
    gearinfotype{"right_weapon2", item_type::weapon, unarmed_weapon_id},
    gearinfotype{"left_weapon3", item_type::weapon, unarmed_weapon_id},
    gearinfotype{"right_weapon3", item_type::weapon, unarmed_weapon_id},
    gearinfotype{"arrow1", item_type::weapon, empty_ammo_id},
    gearinfotype{"bolt1", item_type::weapon, empty_ammo_id},
    gearinfotype{"arrow2", item_type::weapon, empty_ammo_id},
    gearinfotype{"bolt2", item_type::weapon, empty_ammo_id},
    gearinfotype{"unk1", item_type::invalid, -1},
    gearinfotype{"unk2", item_type::invalid, -1},
    gearinfotype{"head_protector", item_type::protector, bare_head_protector_id},
    gearinfotype{"chest_protector", item_type::protector, bare_chest_protector_id},
    gearinfotype{"arms_protector", item_type::protector, bare_arms_protector_id},
    gearinfotype{"legs_protector", item_type::protector, bare_legs_protector_id},
    gearinfotype{"unk3", item_type::invalid, -1},
    gearinfotype{"accessory1", item_type::accessory, empty_accessory_id},
    gearinfotype{"accessory2", item_type::accessory, empty_accessory_id},
    gearinfotype{"accessory3", item_type::accessory, empty_accessory_id},
    gearinfotype{"accessory4", item_type::accessory, empty_accessory_id},
    gearinfotype{"unk4", item_type::invalid, -1},
};

}