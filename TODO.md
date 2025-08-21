- A new template `big-project-w-display-and-rotary-and-menu-template`
 built on top of `big-project-w-display-and-rotary-template`,
 with the following 2 lines on the display:

Note: BEFORE DOING THIS IT WOULD BE SMART TO INVESTIGATE TcMenu:
 https://tcmenu.github.io/documentation/arduino-libraries/tc-menu/

(pretend these are the 2 lines printed in the 16x2 LCD display)
OFF
[blank]

25.5°
0:02:35

If there is the possibility to set no timer:
25.5°
NO TIMER

Pressing the rotary selects the 1st row,
 and now turning the rotary edits the target T:
25.5°      <
0:02:35

Pressing the rotary again selects the 2nd row,
 and now turning the rotary edits the countdown timer:
25.5°
0:02:35    <

Pressing the rotary again exits the selections.

Pressing and holding the rotary when a row is selected,
 sets the value to default; when no row is selected,
 switches on/off the AC.

Optionally, there is a 3rd (and more) line not visible on the display until the 
 2nd line is selected and the rotary is pressed on emore time.