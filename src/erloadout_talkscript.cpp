#include <spdlog/spdlog.h>

#include "erloadout_loadout.hpp"
#include "erloadout_messages.hpp"
#include "erloadout_shop.hpp"
#include "erloadout_talkscript.hpp"
#include "erloadout_talkscript_states.hpp"
#include "from/EzState.hpp"
#include "utils/modutils.hpp"

using namespace std;
using namespace erloadout;

namespace
{
extern LoadoutMenuNextState loadout_menu_next_state;
extern OpenShopState save_loadout_state;
extern OpenShopState apply_loadout_state;

LoadoutMenuState loadout_menu_state(68000, &loadout_menu_next_state);
LoadoutMenuNextState loadout_menu_next_state(68001, &save_loadout_state, &apply_loadout_state);
OpenShopState save_loadout_state(68002, shop::save_loadout_shop_id,
                                 shop::save_loadout_shop_id + loadouts.size() - 1,
                                 &loadout_menu_state);
OpenShopState apply_loadout_state(68003, shop::apply_loadout_shop_id,
                                  shop::apply_loadout_shop_id + loadouts.size() - 1,
                                  &loadout_menu_state);
};

// AddTalkListData(68, "Manage loadouts", -1)
static from::EzState::IntValue loadout_talk_list_index = 68;
static from::EzState::IntValue loadout_menu_text_id = msg::event_text_for_talk_manage_loadouts;
static from::EzState::IntValue unk = -1;
static from::EzState::CommandArg loadout_arg_list[] = {loadout_talk_list_index,
                                                       loadout_menu_text_id, unk};
static from::EzState::Call main_menu_loadout_command(from::EzState::Commands::add_talk_list_data,
                                                     loadout_arg_list);

// GetTalkListEntryResult() == 68
static from::EzState::Transition main_menu_loadout_transition(
    &loadout_menu_state, "\x57\x84\x82\x44\x00\x00\x00\x95\xa1");

static from::EzState::Call patched_commands[100];
static from::EzState::Transition *patched_transitions[100];

/**
 * Return true if the given EzState call is AddTalkListData(??, message_id, ??)
 */
static bool is_add_talk_list_data_call(from::EzState::Call &call, int message_id)
{
    return call.command == from::EzState::Commands::add_talk_list_data && call.args.count == 3 &&
           *reinterpret_cast<const int *>(call.args[1].data + 1) == message_id;
}

/**
 * Return true if the given EzState transition goes to a state that opens the storage chest
 */
static bool is_sort_chest_transition(const from::EzState::Transition *transition)
{
    auto target_state = transition->target_state;
    return target_state != nullptr && target_state->entry_commands.count != 0 &&
           target_state->entry_commands[0].command == from::EzState::Commands::open_repository;
}

static from::EzState::StateGroup *state_group = nullptr;

static void (*ezstate_enter_state)(from::EzState::State *state, from::EzState::MachineImpl *machine,
                                   void *unk);

/**
 * Patch the site of grace menu to contain a "Manage loadouts" option
 */
static void ezstate_enter_state_detour(from::EzState::State *state,
                                       from::EzState::MachineImpl *machine, void *unk)
{
    auto state_group = machine->state_group;
    if (state == state_group->initial_state)
    {
        from::EzState::State *add_menu_state = nullptr;
        from::EzState::Call *call_iter = nullptr;

        from::EzState::State *menu_transition_state = nullptr;
        from::EzState::Transition **transition_iter = nullptr;

        // Look for a state that adds a "Sort chest" menu option, and a state that opens the storage
        // chest.
        for (auto &state : state_group->states)
        {
            for (auto &call : state.entry_commands)
            {
                if (is_add_talk_list_data_call(call, msg::event_text_for_talk_sort_chest))
                {
                    add_menu_state = &state;
                    call_iter = &call + 1;
                }
                else if (is_add_talk_list_data_call(call, msg::event_text_for_talk_manage_loadouts))
                {
                    spdlog::debug("Not patching state group x{}, already patched",
                                  0x7fffffff - state_group->id);
                    return;
                }
            }

            for (auto &transition : state.transitions)
            {
                if (is_sort_chest_transition(transition))
                {
                    menu_transition_state = &state;
                    transition_iter = &transition + 1;
                }
            }
        }

        // If this is the site of grace menu, patch it
        if (add_menu_state != nullptr && menu_transition_state != nullptr)
        {
            // Add "Manage loadouts" menu option
            auto &commands = add_menu_state->entry_commands;

            int command_index = call_iter - commands.begin();
            copy(commands.begin(), call_iter, patched_commands);
            copy(call_iter, commands.end(), patched_commands + command_index + 1);
            patched_commands[command_index] = main_menu_loadout_command;

            commands.elements = patched_commands;
            commands.count++;

            // Add a transition to the "Manage loadouts" menu
            auto &transitions = menu_transition_state->transitions;

            int transition_index = transition_iter - transitions.begin();
            copy(transitions.begin(), transition_iter, patched_transitions);
            copy(transition_iter, transitions.end(), patched_transitions + transition_index + 1);
            patched_transitions[transition_index] = &main_menu_loadout_transition;

            transitions.elements = patched_transitions;
            transitions.count++;

            // When closing the transmog menu, return to the main site of grace menu
            loadout_menu_next_state.set_return_state(state_group->initial_state);

            spdlog::info("Patched state group x{}", 0x7fffffff - state_group->id);
        }
    }

    ezstate_enter_state(state, machine, unk);
}

void erloadout::talkscript::initialize()
{
    modutils::hook(
        {
            .aob = "80 7e 18 00"     // cmp byte ptr [rsi + 0x18], 0x0
                   "74 15"           // jz end_label
                   "4c 8d 44 24 40"  // lea r8=>local_4c8, [rsp + 0x40]
                   "48 8b d6"        // mov rdx,rsi
                   "48 8b 4e 20"     // mov rcx,qword ptr [rsi + 0x20]
                   "e8 ?? ?? ?? ??", // call from::EzState::EnterState
            .offset = 18,
            .relative_offsets = {{1, 5}},
        },
        ezstate_enter_state_detour, ezstate_enter_state);
}
