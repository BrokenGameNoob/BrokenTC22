#include "service_manager.hpp"

#include <QDebug>
#include <QtConcurrent>

#include <DataStructures/structures.hpp>
#include <Logger/logger.hpp>
#include <Utils/json_utils.hpp>
#include <utils/qt_utils.hpp>
#include <utils/time.hpp>

namespace btc2 {

ServiceManager::~ServiceManager() {
  SPDLOG_DEBUG("Deleting service manager");
}

void ServiceManager::Init() {
  SPDLOG_INFO("Init");

  CREGISTER_QML_SINGLETON_I(btc2, ServiceManager);
}

ServiceManager::ServiceManager()
    : m_game_selector{std::make_unique<GameSelector>()},
      m_gear_handler{std::make_unique<GearHandlerTheCrew>(nullptr)},
      m_dummy{std::make_unique<Dummy>(nullptr)} {
  //  m_tmp.actions()[0] = {};
}

void ServiceManager::OnMainWindowLoaded() {
  m_controller_handler = std::make_unique<ControllerHandler>();
}

void ServiceManager::test() {
  SPDLOG_INFO("Test function called");

  m_dummy->SetClutch(1);
  m_dummy->SetGearUp(2);
  m_dummy->SetGearDown(3);
  m_dummy->SetName("Bidule");

  SPDLOG_INFO("{}", *m_dummy);

  const auto kSaved{utils::Save(ToJson(*m_dummy), "TEST.json")};
  SPDLOG_DEBUG("Success save? {}", kSaved);

  if (FillFromFile("TEST2.json", m_dummy.get())) {
    SPDLOG_INFO("{}", *m_dummy);
  } else {
    SPDLOG_ERROR("Failed to read TEST2.json");
  }

  Games g{Game::kTheCrew2 | Game::kTheCrewMotorfist};
}

}  // namespace btc2
