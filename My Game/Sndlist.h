/// \file SndList.h
/// \brief Enumerated type for sounds.

#pragma once

#include "Sound.h"

/// \brief Game sound enumerated type. 
///
/// These are the sounds used in actual gameplay. 
/// The sounds must be listed here in the same order that
/// they are in the sound settings XML file.

enum class eSound : UINT {
    Beep, Boop, Crunch, Buzz, Launch,
    Size  // MUST BE LAST
}; // eSound