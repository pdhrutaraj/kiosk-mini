
kiosk mini 

1. High-level Architecture (Clean MVC for Embedded)
Layered View
┌────────────────────────────────────┐
│            View (Qt Widgets)        │
│  - SensorDashboardWidget            │
│  - CameraViewWidget                 │
│  - BarcodeResultWidget              │
│  - RotaryUIController               │
└───────────────▲────────────────────┘
                │ signals/slots
┌───────────────┴────────────────────┐
│            Controller               │
│  - SensorController                 │
│  - CameraController                 │
│  - BarcodeController                │
│  - InputController (rotary)         │
└───────────────▲────────────────────┘
                │
┌───────────────┴────────────────────┐
│            Model                    │
│  - SensorModel                      │
│  - CameraFrameModel                 │
│  - BarcodeModel                     │
│  - SystemStateModel                 │
└───────────────▲────────────────────┘
                │
┌───────────────┴────────────────────┐
│        Hardware / Platform          │
│  - I2C / SPI / GPIO sensors         │
│  - V4L2 USB camera                  │
│  - evdev rotary encoder             │
│  - ZXing (vendored)                 │
└────────────────────────────────────┘

2. Qt6 Widgets + MVC (Concrete Structure)
Project Layout
kiosk-mini/
├── main.cpp
├── CMakeLists.txt
├── model/
│   ├── sensormodel.h/.cpp
│   ├── cameramodel.h/.cpp
│   ├── barcodemodel.h/.cpp
│   └── systemstatemodel.h/.cpp
├── controller/
│   ├── sensorcontroller.h/.cpp
│   ├── cameracontroller.h/.cpp
│   ├── barcodecontroller.h/.cpp
│   └── inputcontroller.h/.cpp
├── view/
│   ├── mainwindow.h/.cpp
│   ├── cameraviewwidget.h/.cpp
│   ├── sensordashboardwidget.h/.cpp
│   └── barcodeviewwidget.h/.cpp
├── platform/
│   ├── v4l2camera.cpp
│   ├── rotaryencoder.cpp
│   └── sensors/
│       ├── i2c_sensor.cpp
│       └── spi_sensor.cpp
└── external/
    └── zxing/

3. ZXing (Vendored, No System Install)
Why this is correct for i.MX93

Yocto images often don’t ship ZXing

Static or internal build avoids ABI pain

No OpenCV dependency 

kiosk-mini/
├── CMakeLists.txt
├── main.cpp
├── model/
├── controller/
├── view/
├── platform/
└── external/
    └── zxing/

