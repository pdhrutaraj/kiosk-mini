

Build & install ZXing-cpp
cd ~
git clone https://github.com/zxing-cpp/zxing-cpp.git
cd zxing-cpp
mkdir build
cd build
cmake -DBUILD_SHARED_LIBS=ON ..
make -j$(nproc)
sudo make install
sudo ldconfig

This installs:

/usr/local/include/ZXing/
    BarcodeWriter.h
    ReadBarcode.h
    ImageView.h
/usr/local/lib/libZXing.so
/usr/local/lib/cmake/ZXing/ZXingConfig.cmake


✅ Qt 6 Widgets
✅ MVC
✅ V4L2 only (no QtMultimedia)
✅ NO libjpeg
✅ NO YUYV
✅ GRAY8 only (best for ZXing)
✅ SQLite3 storage
✅ USB camera
✅ i.MX93 / Yocto friendly

STEP 0 — What we are building (mental model)
USB Camera (GRAY8)
   │
   ▼
V4L2 mmap (platform)
   │
   ▼
CameraController (thread)
   │ emits QImage(Grayscale8)
   ├────────────► CameraViewWidget (preview)
   │
   └────────────► BarcodeScanner (ZXing)
                       │
                       ▼
                  BarcodeModel
                       │
                       ▼
                 SQLiteRecorder


ZXing loves grayscale — this is the fastest and cleanest path.

STEP 1 — Project skeleton (from scratch)
kiosk-mini/
├── CMakeLists.txt
├── src/
│   ├── main.cpp
│   ├── model/
│   ├── view/
│   ├── controller/
│   ├── platform/
│   │   └── v4l2/
│   └── storage/
└── third_party/
    └── zxing-cpp/
