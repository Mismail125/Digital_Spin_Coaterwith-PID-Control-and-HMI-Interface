# Digital Spin Coater with Touch screen

Thanks to "https://github.com/klotzsch-lab/Maasi" for hardware design.

This project focuses on designing and building a spin coater system used for uniform thin film deposition. It combines mechanical design, electronics, and control systems.

**Demo Video**

https://github.com/user-attachments/assets/e0fda0b0-6889-4bda-abe8-318eeb32caf4

**Features**

* vacuumless chuck (vacuum chuck functionality will be added next version).
* Adjustable speed (350 to 10000 RPM).
* User defined ramp rate and coating time.
* Stable and uniform coating.
* PID based presice RPM control.

**Working Principle**

A small amount of liquid is deposited on a substrate, which is then rotated at high speed. The centrifugal force spreads the liquid evenly, forming a thin film.

**Components Used**

BLDC Motor
BLDC Motor RPM sensor
Arduino Uno
Blheli_S ESC
12V Power supply
Buck Converter 
3D printed Enclosure & rotating platform
ON/OFF Switch

**Circuit Design**

<img width="870" height="426" alt="Spincoater diagram" src="https://github.com/user-attachments/assets/b434a28d-2cf9-48be-9b48-6f848c5b5451" />

Source code is available in the /code directory.
<img width="835" height="629" alt="Spin" src="https://github.com/user-attachments/assets/9c6fbea5-1b10-4fa8-a289-13073673c4b4" />

I drilled a hole through the center of the motor shaft to enable vacuum chuck functionality in the next version.


**Author**

Muhammad Ismail
