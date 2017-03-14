﻿#include "app_main/life_cycle.h"

#include "utils/logger.h"
#include "utils/containers/string.h"
#include "utils/profile_impl.h"

#include "drone_message_handler/drone_message_handler_impl.h"
#include "snapshot_processor/snapshot_processor_impl.h"
#include "map_assembler/map_assembler_impl.h"
#include "map_detalization_analyzer/map_detalization_analyzer_impl.h"
#include "guide/guide_impl.h"
#include "drone_mover/drone_mover_impl.h"

CREATE_LOGGER("Main")

#include "messages/move_message.h"

void LifeCycle::InitComponents() {
  LOG_AUTO_TRACE();

  const utils::String file_name = "enviroment_scanner_server.ini";
  settings_ = new utils::ProfileImpl(file_name);
  drone_mover_ = new drone_mover::DroneMoverImpl(*settings_);
  guide_ = new guide::GuideImpl(*settings_);
  map_detalization_analyzer_ =
      new map_detalization_analyzer::MapDetalizationAnalyzerImpl(*settings_);
  map_assembler_ = new map_assembler::MapAssemblerImpl(*settings_);
  snapshot_processor_ =
      new snapshot_processor::SnapshotProcessorImpl(*settings_);

  message_handler_ =
      new drone_message_handler::DroneMessageHandlerImpl(*settings_, *this);
}

void LifeCycle::DeinitComponents() {
  LOG_AUTO_TRACE();
  delete snapshot_processor_;
  delete map_assembler_;
  delete guide_;
  delete drone_mover_;

  delete message_handler_;

  delete settings_;
}

void LifeCycle::StartThreads() {
  LOG_AUTO_TRACE();
}

int LifeCycle::ListenToClient() {
  LOG_AUTO_TRACE();
  message_handler_->Run();
}

void LifeCycle::OnDataMessageReceived(
    const messages::SensorDataMessage& message) {
  LOG_AUTO_TRACE();
  snapshot_processor_->OnMessageReceived(message);
}

void LifeCycle::OnFinalMessageReceived() {
  LOG_AUTO_TRACE();
  snapshot_processor_->GetGeneratedSnapshot();
  // and call all components in sequence
}