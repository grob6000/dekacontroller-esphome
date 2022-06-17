#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/uart/uart.h"
#include "esphome/core/log.h"
#include "dekacontroller.h"

namespace esphome {
namespace dekacontroller {

static const char *TAG = "dekacontroller";

void DekaControllerComponent::setup() {
  // nothing to do here
}

void DekaControllerComponent::loop() {
  // Use Arduino API to read data, for example
  static char buffer[32] = {0};
  static uint8_t index = 0;
  while (available()) {
    buffer[index] = read();
    index++;
  }
  if ((index >= 6) && (buffer[index-1] == '\n')) {
    // null terminate the buffer
    if (buffer[index-2] == '\r') {
      buffer[index-2] = 0;
    }
    buffer[index-1] = 0;
    // log
    ESP_LOGD(TAG, "Received: %s", buffer);
    // process line
    if (this->gpssensor != nullptr) {
      switch (buffer[0]) {
        case 'O':
          this->gpssensor->publish_state("OK");
          break;
        case 'N':
          this->gpssensor->publish_state("No Fix");
          break;         
        case 'E':
          this->gpssensor->publish_state("Error");
          break;
        case 'C':
          this->gpssensor->publish_state("No Comms");
          break;
      }
    }
    if (this->syncsensor != nullptr) {
      switch (buffer[1]) {
        case 'O':
          this->syncsensor->publish_state("OK");
          break;
        case 'N':
          this->syncsensor->publish_state("None");
          break;         
        case 'E':
          this->syncsensor->publish_state("Error");
          break;
        case 'D':
          this->syncsensor->publish_state("Drift");
          break;
      }
    }
    if (this->runsensor != nullptr) {
      this->runsensor->publish_state((bool)(buffer[2] == '1'));
    }
    if (this->driftsensor != nullptr) {
      int16_t d = (int16_t)strtol(&buffer[2], NULL, 16);
      this->driftsensor->publish_state(d);
    }
    index = 0;
  } 
}

void DekaControllerComponent::dump_config() {
  ESP_LOGCONFIG(TAG, "DekaController");
}

} // namespace dekacontroller
} // namespace esphome
