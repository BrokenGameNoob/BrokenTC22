#include "gear_handler_the_crew.hpp"

#include <system/controls_io/keystroke_sequencer.hpp>
#include <system/services/service_manager.hpp>

namespace btc2 {

using GearType = BaseGearHandler::GearType;
using GearMode = GearHandlerMode::Type;

GearType GearHandlerTheCrew::GetMinGear() const {
  return -1;
}
GearType GearHandlerTheCrew::GetMaxGear() const {
  return 10;
}

GearType GearHandlerTheCrew::GetMaxGearClutch() const {
  return 7;
}

void GearHandlerTheCrew::InternalGearUp() {
  const auto kGearMode{GetGearMode()};
  const auto kNextGear{GetGear() + 1};
  if (kGearMode == GearMode::CLUTCH_MODE && kNextGear <= GetMaxGearClutch()) {
    SetGear(kNextGear ? kNextGear : kNextGear + 1);
  } else if (kGearMode == GearMode::SEQ_MODE || kNextGear > GetMaxGearClutch()) {
    if (!m_game_profile) {
      return;
    }
    SetGear(kNextGear);
    io::KeySequence ks{{m_game_profile->GearUp(), true}, {17}, {m_game_profile->GearUp(), false}};
    io::AsynchronousKeySeqThread(ks);
  } else {
    SPDLOG_ERROR("Unknown gear mode");
  }
}
void GearHandlerTheCrew::InternalGearDown() {
  const auto kGearMode{GetGearMode()};
  const auto kNextGear{GetGear() - 1};
  if (kGearMode == GearMode::CLUTCH_MODE && kNextGear <= GetMaxGearClutch()) {
    SetGear(kNextGear ? kNextGear : kNextGear - 1);
  } else if (kGearMode == GearMode::SEQ_MODE || kNextGear > GetMaxGearClutch()) {
    SetGear(kNextGear);
    io::KeySequence ks{{m_game_profile->GearDown(), true}, {17}, {m_game_profile->GearDown(), false}};
    io::AsynchronousKeySeqThread(ks);
  } else {
    SPDLOG_ERROR("Unknown gear mode");
  }
}

std::optional<int32_t> GearHandlerTheCrew::GetVkCodeForGear(GearType gear) {
  if (!m_game_profile) {
    return {};
  }

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
  if (!m_game_profile) {
    return;
  }

  if (GetGearMode() == GearMode::SEQ_MODE || gear > GetMaxGearClutch()) {
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
