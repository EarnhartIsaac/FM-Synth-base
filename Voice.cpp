//
//  Voice.cpp
//  SpaceBass
//
//  Created by Martin on 08.04.14.
//	Changes by Isaac on 12.09.20
//

#include "Voice.h"

void Voice::setFree() {
    isActive = false;
}

void Voice::reset() {
    mNoteNumber = -1;
    opManager.reset();
}