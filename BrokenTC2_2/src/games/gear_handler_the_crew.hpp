#pragma once

#include <DataStructures/structures.hpp>
#include <Logger/logger.hpp>
#include <games/gear_handler.hpp>

namespace btc2 {

class GearHandlerTheCrew : public BaseGearHandler {
 public:
  GearHandlerTheCrew(std::shared_ptr<GameProfileTheCrew> game_profile, QObject* parent = nullptr)
      : BaseGearHandler{parent}, m_game_profile{std::move(game_profile)} {}

  virtual GearType GetMinGear() const override;
  virtual GearType GetMaxGear() const override;
  virtual GearType GetMaxGearClutch() const override;

 protected:
  virtual void InternalGearUp() override;
  virtual void InternalGearDown() override;

  std::optional<int32_t> GetVkCodeForGear(GearType gear);
  void OnGearSet(GearType old_gear, GearType gear) override;
  void OnGearModeSet(GearHandlerMode::Type old_mode, GearHandlerMode::Type mode) override;

 private:
  std::shared_ptr<GameProfileTheCrew> m_game_profile;
};

}  // namespace btc2
