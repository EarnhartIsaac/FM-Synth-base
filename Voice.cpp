//
//  Voice.cpp
//  SpaceBass
//
//  Created by Martin on 08.04.14.
//
//

#include "Voice.h"

void Voice::setFree() {
    isActive = false;
}

void Voice::reset() {
    mNoteNumber = -1;
    opManager.reset();
}