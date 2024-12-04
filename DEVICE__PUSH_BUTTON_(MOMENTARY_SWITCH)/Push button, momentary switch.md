Saturday 2024.10.12 23:12:46

PUSH BUTTON, MOMENTARY SWITCH

Push buttons are momentary switches.
They need to be connected to Arduino with (the internal) pull-up resistor. Or 
 a pull-down resistor. There is no whatsovere difference between pull-up or down
 but up is better because it is integrated in every Arduino PIN so you don't
 even need an actual resistor.

The best choice is to use the lib IoAbstraction.

The most efficient solution is to use interrupts and debouncing, but it is a bit
 complex to do it myself (better to use IoAbstraction), see "push-button-toggle-with-interrupts-and-debouncing".
And Arduino UNO R4 has only 2 PINs that support interrupts: https://docs.arduino.cc/language-reference/en/functions/external-interrupts/attachInterrupt/

The second most efficient and the most simple is "push-button-toggle-wout-debouncing".

I should read this to dig further: https://www.ganssle.com/debouncing.htm