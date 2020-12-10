//
//  Oscillator.cpp
//  SpaceBass
//
//  Created by Martin on 08.04.14.
//  Changes by Isaac on 12.09.20
//

#include "Oscillator.h"
#define PHASE_MODULATION

double Oscillator::mSampleRate = 44100.0;

void Oscillator::setMode(OscillatorMode mode) {
    mOscillatorMode = mode;
}

void Oscillator::setFrequency(double frequency) {
    mFrequency = frequency;
    updateIncrement();
}

void Oscillator::setSampleRate(double sampleRate) {
    mSampleRate = sampleRate;
	mIncrementCoeff = 2 * mPI / mSampleRate;
    updateIncrement();
}

void Oscillator::generate(double* buffer, int nFrames) {
    const double twoPI = 2 * mPI;
    switch (mOscillatorMode) {
        case OSCILLATOR_MODE_SINE:
            for (int i = 0; i < nFrames; i++) {
                buffer[i] = sin(mPhase);
                mPhase += mPhaseIncrement;
                while (mPhase >= twoPI) {
                    mPhase -= twoPI;
                }
            }
            break;
        case OSCILLATOR_MODE_SAW:
            for (int i = 0; i < nFrames; i++) {
                buffer[i] = 1.0 - (2.0 * mPhase / twoPI);
                mPhase += mPhaseIncrement;
                while (mPhase >= twoPI) {
                    mPhase -= twoPI;
                }
            }
            break;
        case OSCILLATOR_MODE_SQUARE:
            for (int i = 0; i < nFrames; i++) {
                if (mPhase <= mPI) {
                    buffer[i] = 1.0;
                } else {
                    buffer[i] = -1.0;
                }
                mPhase += mPhaseIncrement;
                while (mPhase >= twoPI) {
                    mPhase -= twoPI;
                }
            }
            break;
        case OSCILLATOR_MODE_TRIANGLE:
            for (int i = 0; i < nFrames; i++) {
                double value = -1.0 + (2.0 * mPhase / twoPI);
                buffer[i] = 2.0 * (fabs(value) - 0.5);
                mPhase += mPhaseIncrement;
                while (mPhase >= twoPI) {
                    mPhase -= twoPI;
                }
            }
            break;
    }
}

double Oscillator::nextSample() {
    double value = 0.0;
#ifdef PHASE_MODULATION
	double tPhase = mPhase + mFreqModulation;
	while (tPhase >= twoPI) {
		tPhase -= twoPI;
	}
	//necessary when our frequency modulation moves us backwards
	if (tPhase < 0) {
		tPhase += twoPI;
	}
#else
	double tPhase = mPhase;
#endif

    switch (mOscillatorMode) {
        case OSCILLATOR_MODE_SINE:
            value = sin(tPhase);
            break;
        case OSCILLATOR_MODE_SAW:
            value = 1.0 - (2.0 * tPhase / twoPI);
            break;
        case OSCILLATOR_MODE_SQUARE:
            if (tPhase <= mPI) {
                value = 1.0;
            } else {
                value = -1.0;
            }
            break;
        case OSCILLATOR_MODE_TRIANGLE:
            value = -1.0 + (2.0 * tPhase / twoPI);
            value = 2.0 * (fabs(value) - 0.5);
            break;
    }
    mPhase += mPhaseIncrement;
#ifndef PHASE_MODULATION
	mPhase += mFreqModulation;
	if (mPhase < 0) {
		mPhase += twoPI;
	}
#endif
    while (mPhase >= twoPI) {
        mPhase -= twoPI;
    }
	
    return value;
}

void Oscillator::setPitchMod(double amount) {
	mPitchMod = pow(2.0, fabs(amount) * 14.0) - 1;
	if (amount < 0) {
		mPitchMod = -mPitchMod;
	}
    updateIncrement();
}

void Oscillator::setFreqModulation(double amount) {
	mFreqModulation = amount; // *2 * mPI / mSampleRate;;
	//updateIncrement();
}

void Oscillator::updateIncrement() {
    double calculatedFrequency = fmin(fmax(mFrequency + mPitchMod, 0), mSampleRate/2.0);
    mPhaseIncrement = calculatedFrequency * mIncrementCoeff;
}
