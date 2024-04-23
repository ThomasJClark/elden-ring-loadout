#pragma once

#include "erloadout_messages.hpp"
#include "internal/EzState.hpp"

#pragma pack(push, 1)

/**
 * TalkESD state that applies a speffect to the player and immediately exits
 */
class ApplySpEffectState : public EzState::State
{
  private:
    EzState::IntValue speffect_id_arg;
    EzState::CommandArg arg_list[1] = {speffect_id_arg};
    EzState::Call entry_commands[1] = {{EzState::Commands::give_speffect_to_player, arg_list}};
    EzState::Transition pass_transition;
    EzState::Transition *transitions[1] = {&pass_transition};

  public:
    ApplySpEffectState(int32_t id, int32_t speffect_id, EzState::State *main_menu_state)
        : EzState::State({.id = id, .transitions = transitions, .entry_commands = entry_commands}),
          speffect_id_arg(speffect_id), pass_transition(main_menu_state, "\x41\xa1")
    {
    }
};

/**
 * TalkESD state that opens a regular shop, and exits when the shop menu is closed
 */
class OpenShopState : public EzState::State
{
  private:
    EzState::IntValue begin_arg;
    EzState::IntValue end_arg;
    EzState::CommandArg arg_list[2] = {begin_arg, end_arg};
    EzState::Call entry_commands[1] = {{EzState::Commands::open_regular_shop, arg_list}};
    EzState::Transition close_shop_transition;
    EzState::Transition *transitions[1] = {&close_shop_transition};

  public:
    OpenShopState(int32_t id, int32_t begin_shop_lineup_id, int32_t end_up_lineup_id,
                  EzState::State *main_menu_state)
        : EzState::State({.id = id, .transitions = transitions, .entry_commands = entry_commands}),
          begin_arg(begin_shop_lineup_id), end_arg(end_up_lineup_id),
          close_shop_transition(main_menu_state,
                                // CheckSpecificPersonMenuIsOpen(5, 0) == 1
                                "\x7b"
                                "\x82\x05\x00\x00\x00"
                                "\x82\x00\x00\x00\x00"
                                "\x86"
                                "\x82\x01\x00\x00\x00"
                                "\x95"
                                // CheckSpecificPersonGenericDialogIsOpen(0) == 0
                                "\x7a"
                                "\x82\x00\x00\x00\x00"
                                "\x85"
                                "\x82\x00\x00\x00\x00"
                                "\x95"
                                // &&
                                "\x98"
                                // == 0
                                "\x82\x00\x00\x00\x00"
                                "\x95"
                                "\xa1")
    {
    }
};

/**
 * TalkESD state with "Save loadout", "Apply loadout", etc. options
 */
class LoadoutMenuState : public EzState::State
{
  private:
    EzState::IntValue unk = -1;

    EzState::IntValue save_loadout_talk_list_index = 1;
    EzState::IntValue save_loadout_menu_text_id = erloadout::msg::event_text_for_talk_save_loadout;
    EzState::CommandArg save_loadout_arg_list[3] = {save_loadout_talk_list_index,
                                                    save_loadout_menu_text_id, unk};

    EzState::IntValue apply_loadout_talk_list_index = 2;
    EzState::IntValue apply_loadout_menu_text_id =
        erloadout::msg::event_text_for_talk_apply_loadout;
    EzState::CommandArg apply_loadout_arg_list[3] = {apply_loadout_talk_list_index,
                                                     apply_loadout_menu_text_id, unk};

    EzState::IntValue cancel_talk_list_index = 99;
    EzState::IntValue cancel_menu_text_id = erloadout::msg::event_text_for_talk_cancel;
    EzState::CommandArg cancel_arg_list[3] = {cancel_talk_list_index, cancel_menu_text_id, unk};

    EzState::IntValue generic_dialog_shop_message = 0;
    EzState::CommandArg show_shop_message_arg_list[1] = {generic_dialog_shop_message};

    EzState::Call entry_commands[6] = {
        // CloseShopMessage()
        {EzState::Commands::close_shop_message},
        // ClearTalkListData()
        {EzState::Commands::clear_talk_list_data},
        // AddTalkListData(1, "Save loadout", -1)
        {EzState::Commands::add_talk_list_data, save_loadout_arg_list},
        // AddTalkListData(2, "Apply loadout", -1)
        {EzState::Commands::add_talk_list_data, apply_loadout_arg_list},
        // AddTalkListData(99, "Cancel", -1)
        {EzState::Commands::add_talk_list_data, cancel_arg_list},
        // ShowShopMessage(1)
        {EzState::Commands::show_shop_message, show_shop_message_arg_list},
    };
    EzState::Transition next_menu_transition;
    EzState::Transition *transitions[1] = {&next_menu_transition};

  public:
    LoadoutMenuState(int32_t id, EzState::State *next_state)
        : EzState::State({.id = id, .transitions = transitions, .entry_commands = entry_commands}),
          next_menu_transition(next_state,
                               // CheckSpecificPersonMenuIsOpen(1, 0) == 1
                               "\x7b"
                               "\x82\x01\x00\x00\x00"
                               "\x82\x00\x00\x00\x00"
                               "\x86"
                               "\x82\x01\x00\x00\x00"
                               "\x95"
                               // CheckSpecificPersonGenericDialogIsOpen(0) == 0
                               "\x7a"
                               "\x82\x00\x00\x00\x00"
                               "\x85"
                               "\x82\x00\x00\x00\x00"
                               "\x95"
                               // &&
                               "\x98"
                               // == 0
                               "\x82\x00\x00\x00\x00"
                               "\x95"
                               "\xa1")
    {
    }
};

/**
 * TalkESD state that advances to one of the shop states based on the selected menu entry
 */
class LoadoutMenuNextState : public EzState::State
{
  private:
    EzState::Transition select_save_loadout_transition;
    EzState::Transition select_apply_loadout_transition;
    EzState::Transition return_transition;
    EzState::Transition *transitions[3] = {&select_save_loadout_transition,
                                           &select_apply_loadout_transition, &return_transition};

  public:
    LoadoutMenuNextState(int32_t id, EzState::State *save_loadout_state,
                         EzState::State *apply_loadout_state)

        : EzState::State({.id = id, .transitions = transitions, .entry_commands = entry_commands}),
          // GetTalkListEntryResult() == 1
          select_save_loadout_transition(save_loadout_state, "\x57\x84\xa7\x41\x95\xa1"),
          // GetTalkListEntryResult() == 2
          select_apply_loadout_transition(apply_loadout_state, "\xaf\x42\x95\xa1"),
          return_transition(nullptr, "\x41\xa1")
    {
    }

    void set_return_state(EzState::State *state)
    {
        return_transition.target_state = state;
    }
};

#pragma pack(pop)
