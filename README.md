# Button-LED Mapper for STM32F407

## Overview
This project demonstrates a modular, maintainable approach to mapping button inputs to LED outputs on the STM32F407 microcontroller. The firmware is designed following best practices inspired by Michael Barr, focusing on code clarity, modularity, and robust embedded design.

## Features
- Maps three user buttons (GPIOA pins 0, 1, 2) to three LEDs (GPIOD pins 12, 13, 14)
- Modular code structure with clear separation of concerns
- Uses function pointers for flexible button-to-action mapping
- Software debouncing using SysTick timer
- Direct register access for maximum control
- Well-documented and easy to extend

## Hardware Requirements
- STM32F407 development board (e.g., STM32F407 Discovery)
- Three push-buttons connected to GPIOA pins 0, 1, and 2 (with pull-up configuration)
- Three LEDs connected to GPIOD pins 12, 13, and 14
- (Optional) Breadboard and jumper wires

## Schematic Example
```
Button 1 ---- GPIOA Pin 0 ----+3.3V (pull-up)
Button 2 ---- GPIOA Pin 1 ----+3.3V (pull-up)
Button 3 ---- GPIOA Pin 2 ----+3.3V (pull-up)
LED 1    ---- GPIOD Pin 12 ---- GND (with resistor)
LED 2    ---- GPIOD Pin 13 ---- GND (with resistor)
LED 3    ---- GPIOD Pin 14 ---- GND (with resistor)
```

## Getting Started

### 1. Clone the Repository
```bash
git clone <repo-url>
cd Button_Led_Mapper
```

### 2. Open the Project
Open the project in your preferred IDE (STM32CubeIDE, Keil, IAR, etc.) or use command-line tools.

### 3. Build the Project
- Ensure your toolchain is set up for STM32F407.
- Build the project using your IDE or `make` if a Makefile is provided.

### 4. Flash to Device
- Connect your STM32F407 board via ST-Link.
- Flash the compiled binary/hex file to the board using your IDE or `st-flash`/`openocd`.

### 5. Run and Test
- Press any of the three buttons. The corresponding LED will toggle its state.
- Each button press is debounced with a 2-second delay for demonstration purposes.

## Code Structure
```
Button_Led_Mapper/
├── Inc/           # Header files (device, CMSIS, project-specific)
├── Src/           # Source files (main.c, system, etc.)
└── README.md      # Project documentation
```

### Key Files
- `Src/main.c` — Main application logic, GPIO setup, button-to-LED mapping, polling loop
- `Inc/stm32f407xx.h`, `Inc/stm32f4xx.h` — Device-specific headers

## Technical Details
- **Button Mapping:** Uses a struct array and function pointers for flexible button-to-action assignment.
- **Debouncing:** Implements a 2-second software debounce using SysTick timer and a blocking delay (for demonstration; adjust as needed).
- **Polling:** Continuously scans button states in the main loop, triggering actions only on new presses (flag-based edge detection).
- **Direct Register Access:** Configures GPIO and SysTick by direct register manipulation for maximum control and learning value.

## Best Practices (Inspired by Michael Barr)
- **Simplicity & Readability:** Code is clear, well-commented, and easy to follow.
- **Modularity:** Functions are separated by responsibility (initialization, scanning, actions).
- **Minimal Globals:** Only essential global variables are used.
- **Defensive Programming:** Button state is checked before action; released flags prevent repeated toggling.
- **Extensibility:** Adding new buttons or actions is straightforward via the mapping table.
- **Documentation:** Code and project are well-documented for maintainability.

## Customization
- To change button or LED pins, modify the `SW_ButtonTable` and `LED_Init()` in `main.c`.
- Adjust debounce timing in `SW_Button_Scanning()` by changing the `delay_ms()` parameter

## Author
Muskaan Jain