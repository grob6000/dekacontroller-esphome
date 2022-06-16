#include "esphome.h"
//#include "esphome/core/component.h"
//#include "esphome/components/sensor/sensor.h"
//#include "esphome/components/text_sensor/text_sensor.h"
//#include "esphome/components/binary_sensor/binary_sensor.h"
//#include "esphome/components/uart/uart.h"

//using namespace esphome;

//namespace esphome {
//namespace dekacontroller {

class DekaController : public Component, public UARTDevice {
 public:
  TextSensor *gps_state = new TextSensor();
  Sensor *drift = new Sensor();
  TextSensor *sync_state = new TextSensor();
  BinarySensor *run_state = new BinarySensor();
  
  DekaController(UARTComponent *parent) : UARTDevice(parent) {}

  void setup() override {
    // nothing to do here
  }

  void loop() override {
    // Use Arduino API to read data, for example
    static char buffer[32] = {0};
    static uint8_t index = 0;
    while (available()) {
      buffer[index] = read();
      index++;
    }
    if ((index >= 6) && (buffer[index-1] == '\n')) {
      // process line
      switch (buffer[0]) {
        case 'O':
          gps_state->publish_state("ok");
          break;
        case 'N':
          gps_state->publish_state("nofix");
          break;         
        case 'E':
          gps_state->publish_state("error");
          break;
        case 'C':
          gps_state->publish_state("comms");
          break;
      }
      switch (buffer[1]) {
        case 'O':
          sync_state->publish_state("ok");
          break;
        case 'N':
          sync_state->publish_state("none");
          break;         
        case 'E':
          sync_state->publish_state("error");
          break;
        case 'D':
          sync_state->publish_state("drift");
          break;
      }
      run_state->publish_state(buffer[2] == '1');
      int16_t d = (int16_t)strtol(&sync_state[2], NULL, 16);
      drift->publish_state(d);
    }
    
  }
};

//} // namespace dekacontroller
//} // namespace esphome
