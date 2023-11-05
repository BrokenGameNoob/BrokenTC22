#pragma once

#include <games/gear_handler.hpp>
#include <system/logs/logger.hpp>

namespace btc2 {

class GearHandlerTheCrew : public BaseGearHandler {
 public:
  GearHandlerTheCrew(QObject* parent = nullptr) : BaseGearHandler{parent} {}

  virtual GearType GetMinGear() const override;
  virtual GearType GetMaxGear() const override;

  virtual void GearUp() override;
  virtual void GearDown() override;

 protected:
  void OnGearSet(GearType old_gear, GearType gear) override;
  void OnGearModeSet(GearMode old_mode, GearMode mode) override;
};

}  // namespace btc2
