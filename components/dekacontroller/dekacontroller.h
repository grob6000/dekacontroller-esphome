#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/uart/uart.h"

namespace esphome {
namespace dekacontroller {

class DekaControllerComponent : public Component, public uart::UARTDevice {
  public:
    void setup() override;
    void loop() override;
    void dump_config() override;
    void set_gps_sensor(text_sensor::TextSensor *s) { gpssensor = s; }
    void set_sync_sensor(text_sensor::TextSensor *s) { syncsensor = s; }
    void set_run_sensor(binary_sensor::BinarySensor *s) { runsensor = s; }
    void set_drift_sensor(sensor::Sensor *s) { driftsensor = s; }
  protected:
    text_sensor::TextSensor *gpssensor;
    text_sensor::TextSensor *syncsensor;
    binary_sensor::BinarySensor *runsensor;
    sensor::Sensor *driftsensor;
};

} // namespace dekacontroller
} // namespace esphome
