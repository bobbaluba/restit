restit
======

A reinforcement learner for tetris.


Video demonstation
------------------

[![Youtube video](http://img.youtube.com/vi/eblep6rzEAU/0.jpg)](http://www.youtube.com/watch?v=eblep6rzEAU)


License
-------

zlib license v0.7

    restit - Copyright (c) 2013 Johan Klokkhammer Helsing, Mareike Picklum

    This software is provided 'as-is', without any express or implied
    warranty. In no event will the authors be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

       1. The origin of this software must not be misrepresented; you must not
       claim that you wrote the original software. If you use this software
       in a product, an acknowledgment in the product documentation would be
       appreciated but is not required.

       2. Altered source versions must be plainly marked as such, and must not be
       misrepresented as being the original software.

       3. This notice may not be removed or altered from any source
       distribution.



System requirements
-------------------

* Qt 4
* c++11 support, has been tested with gcc >4.7



Installation instructions
-------------------------

Option 1:
run build.sh to build package automagically and run program manually

Option 2:
run build_n_run.sh to build package, run program and create log file automagically

Option 3:
Do everything manually:

* cd build/
* qmake .. (or qmake ../src/tetrix.pro on older qt versions)
* make
* ./tetrix

---> in case you cannot build the program yourself, try using the binary we added (you can trust us ;) ) or let us
know and we will visit you for a short demonstration.



File structure
--------------

build/                      --> folder for built binaries
src/
  * boardmodel.cpp/-.h      --> Data structure for the restit board
  * boris.cpp/-.h           --> Agent who plays the game, translates simple higher-order instructions into chain of low-level actions
  * bossofboris.cpp/-.h     --> Interface for instructor-classes composing player instructions
  * complextetris.cpp/-.h   --> Controller for the game model (implements all actions: sliding, rotation, movement..)
  * gamemodel.cpp/-.h       --> Data structure for the current state of the tetrix game
  * greedyboss.cpp/-.h      --> Implementation of an instructor class (uses given weight vector to play, no learning implementation, used for testing quality of the weights calculated by reinforcement learner)
  * locoboss.cpp/-.h        --> Implementation of an instructor class (sends random instructions to agent)
  * main.cpp                --> Starting point of program
  * simpleaction.cpp/-.h    --> Rotation + Position of a Tetronimo
  * state.cpp/-.h           --> Wrapper for the gameModel (only used in zuckermaas)
  * tetrixboard.cpp/-.h     --> Game implementation
  * tetrixpiece.cpp/-.h     --> Implementation of Tetronimos
  * tetrixwindow.cpp/-.h    --> GUI implementation of the game
  * vector.cpp/-.h          --> Our own implemenation of a Vector containing doubles, and operations on it
  * zuckermaass.cpp/-.h     --> Implementation of an instructor class (actual reinforcement learner according to algorithm described by Zucker & Maas (see Project report))
README                      --> This file
