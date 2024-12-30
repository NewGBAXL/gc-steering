# Gamecube Logitech Force Feedback Wheel Usage Example

This is a very bare-bones project that shows how to read the [Logitech Speed Force: Force Feedback Wheel](https://nintendo.fandom.com/wiki/Speed_Force_Racing_Wheel)
using devkitPPC and libogc (from [devkitPro](https://devkitpro.org/), of course)

Currently, libogc contains no support for reading steering wheel input from this controller, and only somewhat 
"wonky" support for detecting it. Though, to be fair to the libogc authors, my feeling of "wonky support" for detecting
the presence of a keyboard controller could very well just be due to libogc's SI API being undocumented.

More documentation for this keyboard controller can be found on the 
[Yet Another Gamecube Documentation](http://hitmen.c02.at/files/yagcd/yagcd/index.html) website. Specifically, 
[section 9](http://hitmen.c02.at/files/yagcd/yagcd/chap9.html#sec9) and [section 5.8](http://hitmen.c02.at/files/yagcd/yagcd/chap5.html#sec5.8) 
are both useful. [Dolphin Emulator's implementation](https://github.com/dolphin-emu/dolphin/blob/master/Source/Core/Core/HW/SI/SI_DeviceGCSteeringWheel.cpp) was used for reference of the protocol. This example was based off of my fork of [gered's ASCII Keyboard Controller sample(https://github.com/NewGBAXL/gc-ascii-keyboard-hl).
