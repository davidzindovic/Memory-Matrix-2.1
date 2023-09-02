# Memory-Matrix-2.1

Memory Matrix 2.1, August 2023. Author: David Zindović

The Memory Matrix is a device intedted for promotional purposes of Zavod 404.

The device is a memory tester, consisting of 25 playing buttons, 1 start button and 2 seven segment displays. The goal is to climb as high as possible through "Simon Says" like level, with each level forcing the player to remember one step more of the pattern.

The Circuit:
- controlled by Arduino Uno
- keyboard is digital, works by turning on collumns and reading digital values of rows one by one
- the LEDs under the buttons and the 7 segment displays are driven by the 74HC595 shift registers
- 7 segment displays are driven also with the help of P-channel MOSFETS and NPN transistors, due to 12V supply voltage
- the LEDs in the 5x5 array are driven by driving the cathode to a high logic level voltage and the anode to a low level. This way the LED does not conduct/shine. When we want to turn on a specific LED we just connect the anode/row to high logic level and cathode/collumn to low logic level


The Software:
- the loop just flashese the start button LED, while the start button enables a variable value change resulting in the start of the game
- the game is in the function igra(), where we get a random value by reading the analog value of an unconnected pin and calling the random function. Then we call the function izziv() that lights up the pattern of LED, refferencing the array of values from 1-25 that the player needs to memorize
- after we get to see the challenge, there is a loop while the keyboard value is being read. This is done by looping logical values and reading which button is pressed
- once the function branje_tipkovnice() return a value we compare it with the correct value. Also upon pressing the button the corresponding LED is lit up. If the pressed button/combination of buttons is correct the game continues and the score on the 2 digit 7 semgent display is updated, otherwise an "X" is drawn on the keyboard by using PWM
- after "Game Over" we are returned to the waiting loop
- The 7 segment display is being driven with PWM modulation due to heating, which is why the brightness is dimmed during: waiting for keyboard press, instead of delay function, waiting loop and while LEDs are turned on without PWM
