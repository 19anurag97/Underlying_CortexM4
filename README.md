# Underlying_CortexM4_In_STM

## Overview

This repository documents my milestone-driven exploration of the ARM Cortex-M4 internals using STM32 microcontrollers. Each folder represents a focused step toward mastering privilege separation, handler/thread modes, stack pointers, fault handling, and building a mini RTOS.

The repo emphasizes:
- Hands-on experimentation with registers and hardware features.
- Git-friendly structure for reproducibility and collaboration.

## Tools Used

- STM32CubeIDE (for code development, build, and debugging)
- ARM GCC Toolchain (compiler and linker)
- STM32WB55RG BLE Development Board (all codes tested on this hardware)
- (Optional) VS Code for editing and documentation

> All codes and experiments are tested on the STM32WB55RG BLE development board.

## Folder Structure

```
STM32_LearningProject/
├── 02_NVIC_Program/      # NVIC programming experiments
│   ├── Core/Inc/         # Header files
│   ├── Core/Src/         # Source files
│   ├── Startup/          # Startup assembly
│   ├── Debug/            # Build outputs
│   ├── Drivers/          # HAL and BSP drivers
│   └── Notes.md          # Documentation for this milestone
├── 01_Modes_Stack_Program/ # CPU modes, stack pointer, privilege separation
│   ├── Core/Inc/
│   ├── Core/Src/
│   ├── Startup/
│   ├── Debug/
│   ├── Drivers/
│   └── Asm_learn.md      # Assembly learning notes
├── STM32IDE_SETUP/       # Reference manuals and schematics
│   ├── ReferenceManual/
│   ├── Schematics/
│   └── UserManual/
└── README.md             # Project overview and documentation
```

> Each main folder represents a learning milestone or experiment. See individual folders for detailed notes and code.

## Documentation

- Each milestone includes:
  - NOTES.md inside the folder with explanations.
  - It may contain Asm_learn.md/Issues.md for learning purpose.  
  - Debugging snippets for visual clarity added in .md files.

## Roadmap
- Core CPU modes and privilege separation
- Fault handling and stack pointer management

## Contribution

This repo is primarily for personal learning, but contributions are welcome:
- Suggestions for better structuring.
- Additional debugging techniques.
- Improvements in documentation workflows.

