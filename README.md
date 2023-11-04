# EasyGFX
A simple wrapper library for SDL2, using modern C++

# Description:
This interface is intended to provide an easy way to start using SDL for graphics, in a more "C++" way, without worrying about writing your own wrapper, or including a bunch of C-style code in your project.
This is some source code I had lying around for personal use, to get up and running with graphics quickly while making games. I plan on cleaning this up and extending further with new features over time, such as adding my
sprite renderer and timing functions.

There are two parts: smart pointer wrappers for various raw pointers used by SDL, and a class called Render_Context that lets you do some basic renderering.

# Dependencies:
By default, You will need both SDL2 and SDL2_images. You can omit images by defining GFX_OMIT_IMAGES if you just want drawing primitives.

# Features:
1. RAII: The Render_Context class automatically initializes SDL graphics and cleans up when the destructor is called.
2. Error Handling: SDL2 functions return integer error codes. This wrapper throws an exception if an SDL function fails.
3. Simple: Functions have simple names that are easy to understand and remember.

# Caveats:
1. The feature set is extremely limited for now, and only includes the SDL functions I use for some tile-based games and test programs.
2. Smart pointers are still pointers, and must be handled correctly.
3. Render_Context is not copyable or moveable, and you can only have 1 Render_Context safely. This will be fixed.
4. The SDL library is exposed in the header. I may change this later after more features have been added, to make this a proper library.
5. Read the source code first and change what you want, it's very short.

# Planned features:
1. Sprite renderer
2. Timing functions
3. Wrapper for SDL events
4. Shaders and other fun things, using SDL_gpu (waiting for SDL 3)
5. Further encapsulation
