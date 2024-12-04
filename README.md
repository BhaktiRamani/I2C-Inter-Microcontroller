[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/9IMO547c)
[![Open in Visual Studio Code](https://classroom.github.com/assets/open-in-vscode-2e0aaae1b6195c2367325f4f02e2d04e9abb55f0b24a779b69b11b9e10269abc.svg)](https://classroom.github.com/online_ide?assignment_repo_id=17298579&assignment_repo_type=AssignmentRepo)
# ecen_5813_assignment7_f24
# STM32F091RC Audio Signal Generator and Analyzer
## ECEN 5813 - Principles of Embedded Software
### University of Colorado Boulder

**Author:** Bhakti Ramani
**Date:** November 26, 2024

## Project Overview
This project implements a digital audio signal generation and analysis system using the STM32F091RC microcontroller. The system generates multiple musical frequencies through DAC output while simultaneously analyzing the generated signals through ADC input, demonstrating real-time digital signal processing capabilities.

## Instructions
Every Signal is played for 2 second and to confirm that, you can check the frequency of A5/D13 pin which is toggling inside the interrupt.
While starting the board for the first time, it can give garbage value for first read, Hard reset can be done for correct values.
Photos of all the implementation are saved in Photos folder of github and in this readme file.

## Features
- Multiple frequency sine wave generation (440Hz, 587Hz, 659Hz, 880Hz)
- Real-time signal analysis using autocorrelation
- DMA-based continuous audio output
- Automatic frequency switching every 2 seconds
- Real-time frequency detection and analysis
- Statistical analysis of captured signals

## Photos
I'll incorporate the images into the README.md in a more structured way:

## Signal Waveforms
The system generates four different musical frequencies, each implemented as a high-precision sine wave.

### A4 Note (440 Hz)
![Alt text](photos/sine_wave_440.PNG)
- Fundamental frequency: 440 Hz
- Musical note: A4 (Concert A)
- Sample size: 109 samples per period

### D5 Note (587 Hz)
![Alt text](photos/sine_wave_589.PNG)
- Fundamental frequency: 587 Hz
- Musical note: D5
- Sample size: 82 samples per period

### E5 Note (659 Hz)
![Alt text](photos/sine_wave_659.PNG)
- Fundamental frequency: 659 Hz
- Musical note: E5
- Sample size: 73 samples per period

### A5 Note (880 Hz)
![Alt text](photos/sine_wave_880.PNG)
- Fundamental frequency: 880 Hz
- Musical note: A5 (One octave above A4)
- Sample size: 54 samples per period

### Technical Notes
- All waveforms are generated using fixed-point arithmetic for efficiency
- DMA ensures continuous, glitch-free output
- Sampling frequency: 48 kHz
- 12-bit resolution for high-quality audio output


You can also add additional visualization details:
- Oscilloscope captures of the actual output
- Frequency spectrum analysis
- System block diagram
- Hardware setup photos

Would you like me to help structure any additional image sections or provide more technical details about the waveforms?

## Hardware Requirements
- STM32F091RC Development Board
- Audio output circuit connected to PA4 (DAC output)
- Audio input circuit connected to ADC
- LED indicator on PA5

## System Architecture

### Clock Configuration
- System Clock: 48 MHz
- AHB Bus: 48 MHz
- APB Bus: 24 MHz

### Peripherals Used
1. **DAC (Digital-to-Analog Converter)**
   - Channel 1 on PA4
   - DMA-driven continuous output
   - 12-bit resolution

2. **ADC (Analog-to-Digital Converter)**
   - Channel 4 configuration
   - Timer1 triggered sampling
   - 12-bit resolution
   - 96kHz sampling rate

3. **DMA (Direct Memory Access)**
   - Channel 3 for DAC data transfer
   - Circular mode operation
   - Memory-to-peripheral configuration

4. **Timers**
   - Timer6: DAC trigger source
   - Timer1: ADC trigger configuration
   - SysTick: 2-second frequency switching

## Software Architecture

### Core Components
1. **Signal Generation (`sine.c/h`)**
   - Fixed-point sine wave generation
   - Multiple frequency support
   - Buffer management

2. **DMA Control (`DMA.c/h`)**
   - DMA configuration and management
   - Buffer switching support
   - Interrupt handling

3. **Analog I/O (`analog_in.c/h`, `analog_out.c/h`)**
   - ADC/DAC initialization
   - Sample capture and processing
   - Signal analysis

4. **Timer Management (`timer1.c/h`, `timer6.c/h`)**
   - Precise timing control
   - Trigger source configuration
   - Interrupt management

5. **System Control (`systick.c/h`)**
   - System timing management
   - Frequency switching control
   - LED status indication

### Signal Processing
- Real-time autocorrelation for frequency detection
- Statistical analysis (min, max, mean calculations)
- Continuous signal monitoring and analysis

## Implementation Details

### Audio Generation
```c
/* Frequency configurations */
#define SAMPLE_SIZE_440Hz 109
#define SAMPLE_SIZE_587Hz 82
#define SAMPLE_SIZE_659Hz 73
#define SAMPLE_SIZE_880Hz 54
```

### Signal Analysis
- Buffer Size: 1024 samples
- Automatic period detection
- Frequency calculation based on detected period
- Statistical analysis of captured samples

## Usage and Setup

### Development Environment
- STM32CubeIDE
- Compiler: GCC ARM
- Debugger: ST-Link
- PA4 GPIO pin

### Serial Console Configuration
- Baud Rate: 9600
- Parity: None
- Stop Bits: 1
- Flow Control: None

### Building and Flashing
1. Clone the repository
2. Open project in STM32CubeIDE
3. Build the project
4. Flash to STM32F091RC board

### Debug Output
The system provides debug information through UART:
- Generated sample information
- Frequency analysis results
- Signal statistics

## API Reference

### Key Functions
```c
void tone_to_samples(uint16_t *buffer, int Desired_freq, int Sampling_freq);
void DMA_Init(void);
void ADC_init(void);
void Init_DAC(void);
void ADC_calculations(void);
```

## Future Enhancements
1. Additional waveform support (square, triangle)
2. User-configurable frequency selection
3. Enhanced signal analysis capabilities
4. USB interface for data logging
5. Real-time frequency spectrum analysis

## Known Limitations
- Fixed frequency set (440Hz, 587Hz, 659Hz, 880Hz)
- Single-channel audio processing
- Limited to 12-bit resolution

## Contributing
1. Fork the repository
2. Create a feature branch
3. Commit changes
4. Push to the branch
5. Create a Pull Request

## License
This project is licensed under the terms found in the LICENSE file.

## Acknowledgments
- STMicroelectronics for STM32F091RC platform
- University of Colorado Boulder
- ECEN 5813 course staff

## Contact
Bhakti Ramani - BhaktiRajeshKumarRamani@Colorado.edu



