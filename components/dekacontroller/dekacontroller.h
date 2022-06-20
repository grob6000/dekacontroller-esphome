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
    void set_track_sensor(text_sensor::TextSensor *s) { tracksensor = s; }
    void set_run_ok_sensor(binary_sensor::BinarySensor *s) { run_ok = s; }
    void set_drift_sensor(sensor::Sensor *s) { driftsensor = s; }
    void set_zero_m0_sensor(binary_sensor::BinarySensor *s) { zero_m0 = s; }
    void set_zero_m00_sensor(binary_sensor::BinarySensor *s) { zero_m00 = s; }
    void set_zero_h0_sensor(binary_sensor::BinarySensor *s) { zero_h0 = s; }
    void set_zero_h00_sensor(binary_sensor::BinarySensor *s) { zero_h00 = s; }
    void set_run_in_sensor(binary_sensor::BinarySensor *s) { run_in = s; }
    void set_run_out_sensor(binary_sensor::BinarySensor *s) { run_out = s; }
    void set_tz_sensor(text_sensor::TextSensor *s) { tzsensor = s; }
  protected:
    text_sensor::TextSensor *gpssensor;
    text_sensor::TextSensor *syncsensor;
    text_sensor::TextSensor *tracksensor;
    binary_sensor::BinarySensor *run_ok;
    sensor::Sensor *driftsensor;
    binary_sensor::BinarySensor *zero_m0;
    binary_sensor::BinarySensor *zero_m00;
    binary_sensor::BinarySensor *zero_h0;
    binary_sensor::BinarySensor *zero_h00;
    binary_sensor::BinarySensor *run_out;
    binary_sensor::BinarySensor *run_in;
    text_sensor::TextSensor *tzsensor;
    uint8_t hex2byte(char* s);
};

} // namespace dekacontroller
} // namespace esphome
