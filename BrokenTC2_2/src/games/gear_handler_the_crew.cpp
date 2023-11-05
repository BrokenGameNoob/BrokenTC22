#include "gear_handler_the_crew.hpp"

#include <system/controls_io/keystroke_sequencer.hpp>

namespace btc2 {

using GearType = BaseGearHandler::GearType;
using GearMode = BaseGearHandler::GearMode;

GearType GearHandlerTheCrew::GetMinGear() const {
  return -1;
}
GearType GearHandlerTheCrew::GetMaxGear() const {
  return 7;
}

void GearHandlerTheCrew::GearUp() {
  const auto kGearMode{GetGearMode()};
  if (kGearMode == CLUTCH_MODE) {
    SetGear(GetGear() + 1);
  } else if (kGearMode == SEQ_MODE) {
    io::KeySequence ks{{69, true}, {20}, {69, false}};
    io::AsynchronousKeySeq(ks);
  } else {
    SPDLOG_ERROR("Unknown gear mode");
  }
}
void GearHandlerTheCrew::GearDown() {
  const auto kGearMode{GetGearMode()};
  if (kGearMode == CLUTCH_MODE) {
    SetGear(GetGear() - 1);
  } else if (kGearMode == SEQ_MODE) {
    SPDLOG_ERROR("Not done yet");
  } else {
    SPDLOG_ERROR("Unknown gear mode");
  }
}

void GearHandlerTheCrew::OnGearSet(GearType old_gear, GearType gear) {
  if (GetGearMode() == SEQ_MODE) {
    /* Incompatible type. We should not do enything here */
    SPDLOG_WARN("Incompatible gear mode ({}) to call this function. Not doing anything.", GetGearModeStr());
    return;
  }
}
void GearHandlerTheCrew::OnGearModeSet(GearMode old_mode, GearMode mode) {
  //
}

}  // namespace btc2
