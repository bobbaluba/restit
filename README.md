restit
======

A reinforcement learner for tetris.


Video demonstation
------------------

[![Youtube video](http://img.youtube.com/vi/DXgtzFiRHdc/0.jpg)](http://www.youtube.com/watch?v=DXgtzFiRHdc)


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
* qmake ../src (or qmake ../src/restit.pro on older qt versions)
* make
* ./restit

---> in case you cannot build the program yourself, let us know and we will visit you for a short demonstration. Or just watch the video on youtube.


File structure
--------------

build/                      --> folder for built binaries
logs/                       --> logs created by the tetrix binary
src/
  * boardmodel.cpp/-.h      --> Data structure for the restit board
  * boris.cpp/-.h           --> Agent who plays the game, translates simple higher-order instructions into chain of low-level actions
  * tetrisagent.cpp/-.h     --> Interface for instructor-classes composing player instructions
  * complextetris.cpp/-.h   --> Controller for the game model (implements all actions: sliding, rotation, movement..)
  * gamemodel.cpp/-.h       --> Data structure for the current state of the tetrix game
  * greedyagent.cpp/-.h      --> Implementation of an instructor class (uses given weight vector to play, no learning implementation, used for testing quality of the weights calculated by reinforcement learner)
  * randomagent.cpp/-.h        --> Implementation of an instructor class (sends random instructions to agent)
  * main.cpp                --> Starting point of program
  * simpleaction.cpp/-.h    --> Rotation + Position of a Tetronimo
  * state.cpp/-.h           --> Wrapper for the gameModel (only used in zuckermaas)
  * tetrisboard.cpp/-.h     --> Game implementation
  * tetronimo.cpp/-.h     --> Implementation of Tetronimos
  * tetriswindow.cpp/-.h    --> GUI implementation of the game
  * vector.cpp/-.h          --> Our own implemenation of a Vector containing doubles, and operations on it
  * zuckermaas.cpp/-.h     --> Implementation of an instructor class (actual reinforcement learner according to algorithm described by Zucker & Maas (see Project report))
README                      --> This file
build.sh                    --> script file to build package
build_n_run.sh              --> script file to build package, run program and create log file


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
