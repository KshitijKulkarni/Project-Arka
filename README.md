# Project-Arka
The project code base is split on two folders
1. Microcontroller Code - All code that will be run/tested on microcontrollers (Teensy 4.1, Arduino UNO etc)
2. Ground Control Code - All code that will be run on a system external to the engine (includes code for the GUI)

## Microcontroller Code
The files in this folder are sorted as follows
```
Microcontroller Code
├── moduleTestCodes
└── <microcontrollerBoardname>
    ├── <microcontrollerBoardname>.ino
    └── src
        ├── HelperModule1
        │   ├── HelperModule1.h
        │   └── HelperModule1.cpp
        ├── HelperModule2
        │   ├── HelperModule2.h
        │   └── HelperModule2.cpp
        └── HelperModule3
            ├── HelperModule3.h
            └── HelperModule3.cpp
```

The moduleTestCodes directory contains code for testing individual sensor/motor modules, which are not directly suitable for reuse in the final project.

## Ground Control Code