//
//  Voice.h
//  SpaceBass
//
//  Created by Martin on 08.04.14.
//
//

#ifndef __SpaceBass__Voice__
#define __SpaceBass__Voice__

#include "OperatorManager.h"
#include "Oscillator.h"
#include "EnvelopeGenerator.h"
#include "Filter.h"

class Voice {
public:
    friend class VoiceManager;
    Voice()
    : mNoteNumber(-1),
    isActive(false) {
        // Set myself free everytime my volume envelope has fully faded out of RELEASE stage:
        opManager.ops[0]->mVolumeEnvelope.finishedEnvelopeCycle.Connect(this, &Voice::setFree);
    };
	inline void setFilterEnvelopeAmount(double amount, int op) { opManager.ops[op]->mFilterEnvelopeAmount = amount; }
	inline double getFilterEnvelopeAmount(int op) { return opManager.ops[op]->mFilterEnvelopeAmount; }
	inline void setFilterLFOAmount(double amount, int op) { opManager.ops[op]->mFilterLFOAmount = amount; }
	inline double getFilterLFOAmount(int op) { return opManager.ops[op]->mFilterLFOAmount; }
	inline void setOscillatorPitchAmount(double amount, int op) { opManager.ops[op]->mOscillatorPitchAmount = amount; }
	//No pitch setter for 2 oscillators
	//No Mix setter
	inline void setLFOValue(double value, int op) { opManager.ops[op]->mLFOValue = value; }
	inline void setLFOValue(double value) {
		for (int i = 0; i < opManager.numOperators; i++)
			setLFOValue(value, i);
	}
    
	inline void setAlgorithm(int alg) { opManager.algorithm = alg; }

    inline void setNoteNumber(int noteNumber) {
        mNoteNumber = noteNumber;
        double frequency = 440.0 * pow(2.0, (mNoteNumber - 69.0) / 12.0);
        opManager.setFrequency(frequency);
    }
	inline double nextSample() {
		if (!isActive) return 0.0;
		return opManager.nextSample(); 
	}
    void setFree();
    
    void reset();
private:
	//Filter, envelope, lfo amounts all moved to Operator
	OperatorManager opManager;
    int mNoteNumber;
    bool isActive;
};

#endif /* defined(__SpaceBass__Voice__) */
