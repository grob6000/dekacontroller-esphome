import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, binary_sensor, text_sensor, uart
from esphome.const import CONF_ID, CONF_VOLTAGE, CONF_CURRENT, CONF_STATE, UNIT_SECOND, ICON_TIMER, ICON_SIGNAL, ICON_PULSE, ICON_RESTART, DEVICE_CLASS_DURATION, UNIT_SECOND, STATE_CLASS_MEASUREMENT, DEVICE_CLASS_DURATION
dekacontroller_ns = cg.esphome_ns.namespace('dekacontroller')
DekaControllerComponent = dekacontroller_ns.class_('DekaControllerComponent', cg.Component)

DEPENDENCIES = ['uart']
AUTO_LOAD = ['uart', 'sensor', 'text_sensor', 'binary_sensor']

CONF_GPSSTATESENSOR = "gps"
CONF_SYNCSTATESENSOR = "sync"
CONF_TRACKSTATESENSOR = "track"
CONF_DRIFTSENSOR = "drift"
CONF_RUNOKSENSOR = "run"

CONF_ZEROM0SENSOR = "zerom0"
CONF_ZEROM00SENSOR = "zerom00"
CONF_ZEROH0SENSOR = "zeroh0"
CONF_ZEROH00SENSOR = "zeroh00"
CONF_RUNINSENSOR = "runin"
CONF_RUNOUTSENSOR = "runout"

CONF_TIMEZONESENSOR = "timezone"

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(DekaControllerComponent),
    cv.Optional(CONF_GPSSTATESENSOR):
        text_sensor.text_sensor_schema(icon=ICON_SIGNAL).extend(),
    cv.Optional(CONF_SYNCSTATESENSOR):
        text_sensor.text_sensor_schema(icon=ICON_RESTART).extend(),
    cv.Optional(CONF_TRACKSTATESENSOR):
        text_sensor.text_sensor_schema(icon=ICON_TIMER).extend(),
    cv.Optional(CONF_DRIFTSENSOR):
        sensor.sensor_schema(icon=ICON_TIMER,device_class=DEVICE_CLASS_DURATION,unit_of_measurement=UNIT_SECOND,accuracy_decimals=1,state_class=STATE_CLASS_MEASUREMENT).extend(),
    cv.Optional(CONF_RUNOKSENSOR):
        binary_sensor.binary_sensor_schema(icon=ICON_PULSE).extend(),
    cv.Optional(CONF_ZEROM0SENSOR):
        binary_sensor.binary_sensor_schema(icon=ICON_PULSE).extend(),
    cv.Optional(CONF_ZEROM00SENSOR):
        binary_sensor.binary_sensor_schema(icon=ICON_PULSE).extend(),
    cv.Optional(CONF_ZEROH0SENSOR):
        binary_sensor.binary_sensor_schema(icon=ICON_PULSE).extend(),
    cv.Optional(CONF_ZEROH00SENSOR):
        binary_sensor.binary_sensor_schema(icon=ICON_PULSE).extend(),
    cv.Optional(CONF_RUNINSENSOR):
        binary_sensor.binary_sensor_schema(icon=ICON_PULSE).extend(),
    cv.Optional(CONF_RUNOUTSENSOR):
        binary_sensor.binary_sensor_schema(icon=ICON_PULSE).extend(),
    cv.Optional(CONF_TIMEZONESENSOR):
        text_sensor.text_sensor_schema(icon=ICON_TIMER).extend(),
}).extend(cv.COMPONENT_SCHEMA).extend(uart.UART_DEVICE_SCHEMA)

def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)
    yield uart.register_uart_device(var, config)
    
    if CONF_GPSSTATESENSOR in config:
        conf = config[CONF_GPSSTATESENSOR]
        sens = yield text_sensor.new_text_sensor(conf)
        cg.add(var.set_gps_sensor(sens))

    if CONF_SYNCSTATESENSOR in config:
        conf = config[CONF_SYNCSTATESENSOR]
        sens = yield text_sensor.new_text_sensor(conf)
        cg.add(var.set_sync_sensor(sens))

    if CONF_TRACKSTATESENSOR in config:
        conf = config[CONF_TRACKSTATESENSOR]
        sens = yield text_sensor.new_text_sensor(conf)
        cg.add(var.set_track_sensor(sens))

    if CONF_DRIFTSENSOR in config:
        conf = config[CONF_DRIFTSENSOR]
        sens = yield sensor.new_sensor(conf)
        cg.add(var.set_drift_sensor(sens))

    if CONF_RUNOKSENSOR in config:
        conf = config[CONF_RUNOKSENSOR]
        sens = yield binary_sensor.new_binary_sensor(conf)
        cg.add(var.set_run_ok_sensor(sens))

    if CONF_ZEROM0SENSOR in config:
        conf = config[CONF_ZEROM0SENSOR]
        sens = yield binary_sensor.new_binary_sensor(conf)
        cg.add(var.set_zero_m0_sensor(sens))

    if CONF_ZEROM00SENSOR in config:
        conf = config[CONF_ZEROM00SENSOR]
        sens = yield binary_sensor.new_binary_sensor(conf)
        cg.add(var.set_zero_m00_sensor(sens))

    if CONF_ZEROH0SENSOR in config:
        conf = config[CONF_ZEROH0SENSOR]
        sens = yield binary_sensor.new_binary_sensor(conf)
        cg.add(var.set_zero_h0_sensor(sens))

    if CONF_ZEROH00SENSOR in config:
        conf = config[CONF_ZEROH00SENSOR]
        sens = yield binary_sensor.new_binary_sensor(conf)
        cg.add(var.set_zero_h00_sensor(sens))

    if CONF_RUNINSENSOR in config:
        conf = config[CONF_RUNINSENSOR]
        sens = yield binary_sensor.new_binary_sensor(conf)
        cg.add(var.set_run_in_sensor(sens))

    if CONF_RUNOUTSENSOR in config:
        conf = config[CONF_RUNOUTSENSOR]
        sens = yield binary_sensor.new_binary_sensor(conf)
        cg.add(var.set_run_out_sensor(sens))

    if CONF_TIMEZONESENSOR in config:
        conf = config[CONF_TIMEZONESENSOR]
        sens = yield text_sensor.new_text_sensor(conf)
        cg.add(var.set_tz_sensor(sens))