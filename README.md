# dekacontroller-esphome

An [ESPHome External Component](https://esphome.io/components/external_components.html) for the ESP8266 in my dekaclock controller.

It talks to the [dekacontroller microcontroller firmware](https://github.com/grob6000/dekacontroller-firmware) over serial, and posts the various parameters to HomeAssistant as sensors.

One output pin can be used to trigger a re-sync of the clock (the microcontroller has a button input for re-sync). This is exposed as a switch in Home Assistant).

## Protocol

### Description 

This is a simple one-way serial protocol to communicate from the microcontroller in the dekacontroller (ATMEGA328PA) to the ESP8266, providing the various values.
The protocol is only one-way, as the microcontroller RX is connected to a GPS device.
Protocol is ASCII text, values are formatted in hexadecimal. A checksum is included at the end.
It's similar to a NEMA01853 frame without the `$` at the start.

### Example

`1F000300020258*79\n`

### Structure

| Bytes | Name | Decoded Format | Values |
| --- | --- | --- | --- |
| 0-1 | Status | uint8 | Bits 0-6 are _Status_, bit 7 reserved |
| 2-3 | Mode | uint8 | Bits 0-1 are _DisplayMode_, bits 2-5 are _SyncState_, bits 6-7 reserved |
| 4-5 | _ZeroIO_ | uint8 | Bits 0-5 are the zero monitoring input state, bits 6-7 reserved|
| 6-9 | Drift | int16 | Current drift in seconds  (+ is clock ahead of UTC, - is clock behind UTC) |
| 10-13 | TimeZone | int16 | number of minutes offset from UTC (-720 to +720) |
| 14 | Separator | ascii | `*` |
| 15-16 | Checksum | uint8 | XOR of all bytes from 0 to 13 (all prior to `*`) |
| 17 | Newline | ascii | `\n` |

### Interpretation of values

See dekacontroller_types.h.
The type definitions are shared between this and the microcontroller firmware.

| Name | Values |
| --- | --- |
| _Status_ | Bit0: Run OK (0 = not OK, 1 = OK)<br>Bit1: GPS Has Time (0 = doesn't, 1 = does)<br>Bit2: GPS Has Fix (0 = doesn't, 1 = does)<br>Bit3: GPS Old Fix (0 = never had fix, 1 = had fix in the past)<br>Bit4: GPS Has Comms (0 = no comms, 1 = has comms)<br>Bit5: Time Drift (0 = no drift, 1 = drift warning)<br>Bit6: Time Error (0 = OK, 1 = Error) |
| _DisplayMode_ | 0 = Main<br>1 = Timezone<br>2 = Drift<br>3 = Sync |
| _SyncState_ | 0 = Ok (synced and OK) <br>1 = Begin<br>2 = Zero Minute<br>3 = Zero Hour<br>4 = Set Hour<br>5 = Set Minute<br>6 = Wait Mark<br>7 = Error<br>8 = None (not synced)
| _ZeroIO_ | Bit0: 0 = 1x minute not zero, 1 = 1x minute is zero<br>Bit1: 0 = 10x minute not zero, 1 = 10x minute is zero<br>Bit3: 0 = 1x hour not zero, 1 = 1x hour is zero<br>Bit3: 0 = 10x hour not zero, 1 = 10x hour is zero<br>Bit4: Run Output (0 = not run, 1 = run)<br>Bit5: Run Input (0 = not run, 1 = run) |
