## Self-destructing Arduino ESP32 Firmware - Stage 1

![Arduino ESP32 Self-destructing Firmware](/self-destructing-arduino-esp32-firmware-platformio.png)

> Is it possible to erase the firmware of an ESP32 from within itself? Let's try to find out.

If you want to erase the ESP32's flash content, the easiest way would be using `esptool.py` with the `erase_flash` parameter. But how we can do this from within the running Arduino Sketch?

## Installation

![PlatformIO](/platformio.png)

First `build` the [ESP32-SelfDestruct-Payload](https://github.com/technopolistv/ESP32-SelfDestruct-Payload/) project and copy the `firmware.bin` into the `data` folder of this project. Rename `firmware.bin` into `stub.bin`.

Open the `ESP32-SelfDestruct` project. Click on the _PlatformIO Icon_ on the sidebar.

Under `Platform` click first on `Erase Flash`, then on `Build Filesystem Image` and finally on `Upload Filesystem Image`.

Now you can upload the _Stage 1 Firmware_ on your ESP32. Under `General` click on `Upload and Monitor`.

The Self-destruction function gets triggered, if you connect a wire from `GND` to `PIN 32`. You can use a green and red LED to indicate the different stages.

## Demo Screenshot

The green part highlights the main firmware (Stage 1) and the red part the stub firmware (Stage 2).

![Serial Monitor Arduino ESP32 Self-destructing Firmware](/self-destruct-serial-monitor.png)

## Partition layout of Self-Destructing Firmware

<table>
<thead>
  <tr>
    <th>Offset</th>
    <th>Before Update</th>
    <th>After Update</th>
    <th>After Erase</th>
  </tr>
</thead>
<tbody>
  <tr>
    <td>0x9000</td>
    <td>NVS (Data)</td>
    <td>NVS (Data)</td>
    <td>NVS (Empty)</td>
  </tr>
  <tr>
    <td>0xe000</td>
    <td>OTA<br></td>
    <td>OTA</td>
    <td>OTA (Empty)</td>
  </tr>
  <tr>
    <td>0x10000</td>
    <td>APP0 (Firmware)</td>
    <td>APP0 (Firmware disabled)</td>
    <td>APP0 (Empty)</td>
  </tr>
  <tr>
    <td>0x150000</td>
    <td>APP1 (Empty)</td>
    <td>APP1 (Stub Firmware actived)</td>
    <td>APP1 (Stub Firmware)</td>
  </tr>
  <tr>
    <td>0x290000</td>
    <td>SPIFFS (Stub Firmware)</td>
    <td>SPIFFS (Stub Firmware)</td>
    <td>SPIFFS (Empty)</td>
  </tr>
</tbody>
</table>

## More
- Stage 1 - This Project
- Stage 2 - https://github.com/technopolistv/ESP32-SelfDestruct-Payload/

✍️ Blog: https://www.technopolis.tv/PlatformIO-Self-destructing-Arduino-ESP32-Firmware/