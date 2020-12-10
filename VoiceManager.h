//
//  VoiceManager.h
//  SpaceBass
//
//  Created by Martin on 08.04.14.
//
//

#ifndef __SpaceBass__VoiceManager__
#define __SpaceBass__VoiceManager__

#include "Voice.h"
#include <functional>
// #include <functional> if that doesn't work

class VoiceManager {
public:
    void onNoteOn(int noteNumber, int velocity);
    void onNoteOff(int noteNumber, int velocity);
    double nextSample();
    
    void setSampleRate(double sampleRate) {
        EnvelopeGenerator::setSampleRate(sampleRate);
        for (int i = 0; i < NumberOfVoices; i++) {
            Voice& voice = voices[i];
            voice.opManager.setSampleRate(sampleRate);
        }
        mLFO.setSampleRate(sampleRate);
    }
    
    inline void setLFOMode(Oscillator::OscillatorMode mode) { mLFO.setMode(mode); };
    inline void setLFOFrequency(double frequency) { mLFO.setFrequency(frequency); };
    
    typedef std::tr1::function<void (Voice&)> VoiceChangerFunction;
    inline void changeAllVoices(VoiceChangerFunction changer) {
        for (int i = 0; i < NumberOfVoices; i++) {
            changer(voices[i]);
        }
    }
    // Functions to change a single voice:
    static void setVolumeEnvelopeStageValue(Voice& voice, int op, EnvelopeGenerator::EnvelopeStage stage, double value) {
		voice.opManager.ops[op]->mVolumeEnvelope.setStageValue(stage, value);
    }
    static void setFilterEnvelopeStageValue(Voice& voice, int op, EnvelopeGenerator::EnvelopeStage stage, double value) {
		voice.opManager.ops[op]->mFilterEnvelope.setStageValue(stage, value);
    }
    static void setOscillatorMode(Voice& voice, int op, Oscillator::OscillatorMode mode) {
		voice.opManager.ops[op]->mOscillator.setMode(mode);
    }
    static void setOscillatorPitchMod(Voice& voice, int op, double amount) {
		voice.opManager.ops[op]->setOscillatorPitchAmount(amount);
    }
    static void setFilterCutoff(Voice& voice, int op, double cutoff) {
		voice.opManager.ops[op]->mFilter.setCutoff(cutoff);
    }
    static void setFilterResonance(Voice& voice, int op, double resonance) {
		voice.opManager.ops[op]->mFilter.setResonance(resonance);
    }
    static void setFilterMode(Voice& voice, int op, Filter::FilterMode mode) {
		voice.opManager.ops[op]->mFilter.setFilterMode(mode);
    }
    static void setFilterEnvAmount(Voice& voice, int op, double amount) {
        voice.setFilterEnvelopeAmount(amount, op);
    }
    static void setFilterLFOAmount(Voice& voice, int op, double amount) {
        voice.setFilterLFOAmount(amount, op);
    }
	static void setAlgorithm(Voice& voice, int alg) {
		voice.setAlgorithm(alg);
	}

	double getVolumeEnvelopeStageValue(EnvelopeGenerator::EnvelopeStage stage, int op) {
		return voices[0].opManager.ops[op]->mVolumeEnvelope.getStageValue(stage);
	}
	double getFilterEnvelopeStageValue(EnvelopeGenerator::EnvelopeStage stage, int op) {
		return voices[0].opManager.ops[op]->mFilterEnvelope.getStageValue(stage);
	}
	Oscillator::OscillatorMode getOscillatorMode(int op) {
		return voices[0].opManager.ops[op]->mOscillator.getMode();
	}
	double getOscillatorPitchMod(int op) {
		return voices[0].opManager.ops[op]->getPitchMod();
	}
	double getFilterCutoff(int op) {
		return voices[0].opManager.ops[op]->mFilter.getCutoff();
	}
	double getFilterResonance(int op) {
		return voices[0].opManager.ops[op]->mFilter.getResonance();
	}
	Filter::FilterMode getFilterMode(int op) {
		return voices[0].opManager.ops[op]->mFilter.getFilterMode();
	}
	double getFilterEnvAmount(int op) {
		return voices[0].getFilterEnvelopeAmount(op);
	}
	double getFilterLFOAmount(int op) {
		return voices[0].getFilterLFOAmount(op);
	}

private:
    static const int NumberOfVoices = 16;
    Voice voices[NumberOfVoices];
    Oscillator mLFO;
    Voice* findFreeVoice();
	Voice* findFreeVoice(int noteNumber);
};

#endif /* defined(__SpaceBass__VoiceManager__) */
