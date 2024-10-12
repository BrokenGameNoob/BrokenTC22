#include "gear_handler_the_crew.hpp"

#include <system/controls_io/keystroke_sequencer.hpp>

namespace btc2 {

using GearType = BaseGearHandler::GearType;
using GearMode = GearHandlerMode::Type;

GearType GearHandlerTheCrew::GetMinGear() const {
  return -1;
}
GearType GearHandlerTheCrew::GetMaxGear() const {
  return 7;
}

void GearHandlerTheCrew::GearUp() {
  const auto kGearMode{GetGearMode()};
  if (kGearMode == GearMode::CLUTCH_MODE) {
    const auto kNextGear{GetGear() + 1};
    SetGear(kNextGear ? kNextGear : kNextGear + 1);
  } else if (kGearMode == GearMode::SEQ_MODE) {
    // io::KeySequence ks{{2000},
    //                    {160, true},
    //                    {2000},
    //                    {88, true},
    //                    {10},
    //                    {88, false},
    //                    {20},
    //                    {160, false},
    //                    {88, true},
    //                    {10},
    //                    {88, false},
    //                    {20},
    //                    {160, true}};
    // io::AsynchronousKeySeqOld(ks);
  } else {
    SPDLOG_ERROR("Unknown gear mode");
  }
}
void GearHandlerTheCrew::GearDown() {
  const auto kGearMode{GetGearMode()};
  if (kGearMode == GearMode::CLUTCH_MODE) {
    const auto kNextGear{GetGear() - 1};
    SetGear(kNextGear ? kNextGear : kNextGear - 1);
  } else if (kGearMode == GearMode::SEQ_MODE) {
    SPDLOG_ERROR("Not done yet");
  } else {
    SPDLOG_ERROR("Unknown gear mode");
  }
}

std::optional<int32_t> GearHandlerTheCrew::GetVkCodeForGear(GearType gear) {
  switch (gear) {
    case -1:
      return m_game_profile->GearReverse();
    case 0:
      return {};
    case 1:
      return m_game_profile->Gear1();
    case 2:
      return m_game_profile->Gear2();
    case 3:
      return m_game_profile->Gear3();
    case 4:
      return m_game_profile->Gear4();
    case 5:
      return m_game_profile->Gear5();
    case 6:
      return m_game_profile->Gear6();
    case 7:
      return m_game_profile->Gear7();
  }
  return {};
}

void GearHandlerTheCrew::OnGearSet(GearType old_gear, GearType gear) {
  if (GetGearMode() == GearMode::SEQ_MODE) {
    /* Incompatible type. We should not do enything here */
    SPDLOG_WARN("Incompatible gear mode ({}) to call this function. Not doing anything.", GetGearModeStr());
    return;
  }

  const auto kGearKeyOpt{GetVkCodeForGear(gear)};
  if (!kGearKeyOpt) {
    return;
  }

  io::KeySequence ks{{m_game_profile->Clutch(), true},
                     {kGearKeyOpt.value(), true},
                     {17},
                     {kGearKeyOpt.value(), false},
                     {m_game_profile->Clutch(), false}};
  io::AsynchronousKeySeqThread(ks);
}
void GearHandlerTheCrew::OnGearModeSet(GearMode old_mode, GearMode mode) {
  //
}

}  // namespace btc2
