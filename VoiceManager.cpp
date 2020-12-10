//
//  VoiceManager.cpp
//  SpaceBass
//
//  Created by Martin on 08.04.14.
//
//

#include "VoiceManager.h"

Voice* VoiceManager::findFreeVoice(int noteNumber) {
	Voice* freeVoice = NULL;
	bool matchesNote = false;
	for (int i = 0; i < NumberOfVoices; i++) {
		if (!voices[i].isActive) {
			freeVoice = &(voices[i]);
			//break;
		}
		if (voices[i].mNoteNumber == noteNumber) {
			freeVoice = &(voices[i]);
			break;
		}
	}
	return freeVoice;
}


Voice* VoiceManager::findFreeVoice() {
    Voice* freeVoice = NULL;
    for (int i = 0; i < NumberOfVoices; i++) {
        if (!voices[i].isActive) {
            freeVoice = &(voices[i]);
            break;
        }
    }
    return freeVoice;
}

void VoiceManager::onNoteOn(int noteNumber, int velocity) {
    Voice* voice = findFreeVoice(noteNumber);
    if (!voice) {
        return;
    }
    voice->reset();
    voice->setNoteNumber(noteNumber);
    voice->opManager.mVelocity = velocity;
    voice->isActive = true;
	voice->opManager.onNoteOn();
}

void VoiceManager::onNoteOff(int noteNumber, int velocity) {
    // Find the voice(s) with the given noteNumber:
    for (int i = 0; i < NumberOfVoices; i++) {
        Voice& voice = voices[i];
        if (voice.isActive && voice.mNoteNumber == noteNumber) {
			voice.opManager.onNoteOff();
        }
    }
}

double VoiceManager::nextSample() {
    double output = 0.0;
    double lfoValue = mLFO.nextSample();
    for (int i = 0; i < NumberOfVoices; i++) {
        Voice& voice = voices[i];
        voice.setLFOValue(lfoValue);
        output += voice.nextSample();
    }
    return output;
}

