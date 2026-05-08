# ❤️ Signal Processing for Pulse Oximeter (C)
> **Software Engineering & Signal Processing Project**
> Implementation of a complete physiological signal analysis pipeline (SpO2 / BPM) in standard C.

This project forms the core software component of a **complete biomedical system** designed to measure blood oxygen saturation (SpO2) and heart rate (BPM).

### Global System Context

Originally, this project is built around three interconnected modules:

1. **Analog Electronics:** Optical sensors (Red/Infrared LED + Photodiode) and signal conditioning (amplification, analog filtering).
2. **Digital Electronics (STM32):** Analog-to-Digital Conversion (ADC) and sensor control via a microcontroller.
3. **Software Processing (This repository):** Algorithmic pipeline responsible for analyzing raw data streams to extract physiological metrics.

*Note: This repository focuses on the **algorithmic implementation in standard C**. Hardware acquisition is simulated here by ingesting raw data files (`.dat`) from the real acquisition chain.*

---

### Processing Architecture

The processing pipeline is designed to be modular and efficient:

1. **Acquisition (Simulation):** Reading binary `.dat` files containing raw samples (Red/Infrared Absorbance).
2. **FIR Filtering (Finite Impulse Response):** High-frequency noise removal to clean the raw signal.
3. **IIR Filtering (Infinite Impulse Response):** DC component removal to isolate the pulsatile (AC) signal.
4. **Decision Algorithm:**
   * Peak detection and zero-crossing detection.
   * **BPM** (Beats Per Minute) calculation.
   * **SpO2** (Pulse Oxygen Saturation) calculation.

---

### Project Structure

The project follows an MVC-inspired structure adapted for C, clearly separating interfaces from implementation:

```text
.
├── Makefile          # Automated build script
├── src/              # Source files (.c) - Main, Filters, IO
├── inc/              # Headers (.h) - Prototypes and Structures
├── data/             # Raw data files (.dat)
└── docs/             # Technical documentation (PDF)
```

---

### Tech Stack

* **Language:** C (ANSI Standard)
* **Build System:** GNU Make (optimized Makefile with dependency management)
* **Mathematics:** Digital filters, integration, circular buffer management.
* **Visualization:** Gnuplot-compatible output for signal curve display.

---

### Installation & Usage

#### 1. Prerequisites
* A C compiler (GCC recommended).
* The `make` tool installed on your system.

#### 2. Build
Navigate to the project root and run:
```bash
make
```
This generates two executables at the root: `simulation` (the main program) and `autotests` (for validation).

#### 3. Run the simulation
```bash
./simulation
```
The program will automatically read the files in the `data/` folder and print the results (BPM/SpO2) to the console.

#### 4. Run unit tests
```bash
./autotests
```
Verifies the correct behavior of individual software components (filters, file reading) independently of the main program.

#### 5. Clean up
To remove object files (`.o`) and executables and start from a clean state:
```bash
make clean
```

---

*Developed as part of the Computer Science & Electronics module at ISEN Brest.*
