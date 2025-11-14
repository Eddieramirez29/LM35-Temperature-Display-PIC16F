# LM35-Temperature-Display-PIC16F887

# LM35 Temperature Display on 7-Segment (PIC16F688)

This project reads temperature data from an **LM35 analog sensor** using the **PIC16F688 ADC** and displays the value on three **7-segment displays** (tens, units, and decimal).  
Only temperatures in the range **0.0 °C to 99.9 °C** are supported.  
If the temperature exceeds 100 °C, the system shows an **"Err"** message.

Future support for **negative temperatures** may be added in hardware and software.

---

## Features
- Reads LM35 output through **AN0**.
- Uses PIC ADC in **right-justified mode**.
- Converts raw ADC value to °C using:


- Displays:
- Tens digit
- Units digit
- Decimal digit (0.1 °C resolution)
- Decimal point indicator
- Error message when T ≥ 100 °C.
- Code structure prepared for future negative-temperature handling.

---

## Hardware Used
- PIC16F688
- LM35 temperature sensor
- Three 7-segment displays (common anode/cathode depending on configuration)
- Power supply 5V
- Supporting resistors/wiring

---

## How It Works
1. Sensor output goes into **AN0**.  
2. The ADC converts the 0–5 V input to a 10-bit value (0–1023).  
3. The firmware computes temperature using the LM35 scale factor (10 mV/°C).  
4. The temperature is split into:
 - **Tens**
 - **Units**
 - **Decimal**  
5. Digits are sent to 7-segment lookup tables.  
6. For values ≥ 100 °C, an **Err** blinking pattern is shown.

---

## Future Improvements
- Hardware level-shifting or biasing to allow negative temperatures.
- Software path (commented) for negative values.
- Multiplexing to reduce pin usage.
- Configurable ADC reference voltage.

---

## License
This project is open-source under the **MIT License**.
