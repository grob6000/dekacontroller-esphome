#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/uart/uart.h"
#include "esphome/core/log.h"
#include "dekacontroller.h"
#include "dekacontroller_types.h" // only used for data structures (here)

namespace esphome {
namespace dekacontroller {

static const char *TAG = "dekacontroller";

void DekaControllerComponent::setup() {
  // nothing to do here
}

uint8_t DekaControllerComponent::hex2byte(char *s) {
  return (uint8_t) ( ((s[0] >= 'A') ? (s[0]-'A'+10) : (s[0]-'0'))*0x10 + ((s[1] >= 'A') ? (s[1]-'A'+10) : (s[1]-'0')) );
}

// new message format
// [status][mode][is][drift]*[csum]
void DekaControllerComponent::loop() {
  // Use Arduino API to read data, for example
  static char buffer[32] = {0};
  static uint8_t index = 0;
  while (available()) {
    buffer[index] = read();
    index++;
  }
  if ((index >= 17) && (buffer[index-1] == '\n')) {
    // strip
    buffer[index-1] = 0;
    if (buffer[index-2]=='\r') { buffer[index-2] = 0; }
    // log input
    ESP_LOGD(TAG, "Received: %s", buffer);
    // get data
    uint8_t b_status = hex2byte(&buffer[0]);
    StatusStruct *status = reinterpret_cast<StatusStruct *>(&b_status);
    uint8_t b_mode = hex2byte(&buffer[2]);
    ModeStruct *mode = reinterpret_cast<ModeStruct *>(&b_mode);
    uint8_t b_io = hex2byte(&buffer[4]);
    int16_t drift = hex2byte(&buffer[6])*0x100 + hex2byte(&buffer[8]);
    int16_t tz = hex2byte(&buffer[10])*0x100 + hex2byte(&buffer[12]);
    // calculate checksum
    uint8_t csum_check = 0;
    uint8_t i = 0;
    for (i = 0; i < strlen(buffer); i++) {
      if (buffer[i] == '*') { break; }
      csum_check ^= buffer[i];
    }
    uint8_t b_csum = hex2byte(&buffer[i+1]);
    ESP_LOGD(TAG, "csum received: 0x%02X csum calculated: 0x%02X", b_csum, csum_check);
    if (b_csum == csum_check) {
      // gps sensor
      if (this->gpssensor != nullptr) {
        if (status->gps_hascomms) {
          if (status->gps_hasfix) {
            this->gpssensor->publish_state("OK");
          } else if (status->gps_hastime) {
            this->gpssensor->publish_state("No Fix");
          } else {
            this->gpssensor->publish_state("Init");
          }
        } else {
          this->gpssensor->publish_state("No Comms");
        }
      }
      // sync sensor
      if (this->syncsensor != nullptr) {
        const char* s;
        switch (mode->syncstate) {
          case Ok:
            s = "OK";
            break;
          case Begin:
            s = "Begin";
            break;
          case ZeroM0:
            s = "Zero M";
            break;
          case ZeroH:
            s = "Zero H";
            break;
          case SetH:
            s = "Set H";
            break;
          case SetM:
            s = "Set M";
            break;
          case WaitMark:
            s = "Wait Mark";
            break;
          case None:
            s = "None";
            break;
          case Error:
          default:
            s = "Error";
            break;
        }
        this->syncsensor->publish_state(s);
      }
      // track sensor
      if (this->tracksensor != nullptr) {
        if (status->time_error) {
          this->tracksensor->publish_state("Error");
        } else if (status->time_drift) {
          this->tracksensor->publish_state("Drift");
        } else {
          this->tracksensor->publish_state("OK");
        }
      }
      // run sensor
      if (this->run_ok != nullptr) {
        this->run_ok->publish_state(status->run_ok);
      }
      // drift sensor
      if (this->driftsensor != nullptr) {
        this->driftsensor->publish_state(drift);
      }
      // io sensors
      if (this->zero_m0 != nullptr) {
        this->zero_m0->publish_state((bool)(b_io & 0x01));
      }
      if (this->zero_m00 != nullptr) {
        this->zero_m00->publish_state((bool)(b_io & 0x02));
      }
      if (this->zero_h0 != nullptr) {
        this->zero_h0->publish_state((bool)(b_io & 0x04));
      }
      if (this->zero_h00 != nullptr) {
        this->zero_h00->publish_state((bool)(b_io & 0x08));
      }
      if (this->run_in != nullptr) {
        this->run_out->publish_state((bool)(b_io & 0x10));
      }
      if (this->run_out != nullptr) {
        this->run_in->publish_state((bool)(b_io & 0x20));
      }
      if (this->tzsensor != nullptr) {
        char s[8];
        sprintf(s, "%s%02d:%02d", (tz>=0) ? "+" : "-", abs(tz)/60, abs(tz)%60);
        this->tzsensor->publish_state(s);
      }
    } // csum check
    // reset buffer index
    index = 0;
  } 
}

void DekaControllerComponent::dump_config() {
  ESP_LOGCONFIG(TAG, "DekaController");
}

} // namespace dekacontroller
} // namespace esphome
