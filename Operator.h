//
//  Operator.h
//  FM Shmynth
//
//  Created by Isaac on 11.13.20.
//
//

#ifndef __SpaceBass__Operator__
#define __SpaceBass__Operator__

#include "Oscillator.h"
#include "EnvelopeGenerator.h"
#include "Filter.h"

class OperatorManager;

class Operator {
public:
	friend class OperatorManager;
	friend class Voice;
	friend class VoiceManager;
	/*
	Operator()
		: mFilterEnvelopeAmount(0.0),
		mFilterLFOAmount(0.0),
		mOscillatorPitchAmount(0.0),
		mLFOValue(0.0),
		isActive(false) { };
		*/
	Operator(OperatorManager * opManager)
		: mFilterEnvelopeAmount(0.0),
		mFilterLFOAmount(0.0),
		mOscillatorPitchAmount(0.0),
		mLFOValue(0.0),
		isActive(false),
		opManager(opManager) { };

	inline void setFilterEnvelopeAmount(double amount) { mFilterEnvelopeAmount = amount; }
	inline void setFilterLFOAmount(double amount) { mFilterLFOAmount = amount; }
	inline void setOscillatorPitchAmount(double amount) { mOscillatorPitchAmount = amount; }
	inline void setLFOValue(double value) { mLFOValue = value; }
	inline void setFrequency(double freq) { mOscillator.setFrequency(freq); }
	inline void setPitchMod(double amount) { mOscillator.setPitchMod(amount); }
	inline double getPitchMod() { return mOscillator.getPitchMod(); }
	inline void setFreqModulation(double amount) { mOscillator.setFreqModulation(amount); }

	double nextSample();

	inline void reset() {
		mVolumeEnvelope.reset();
		mFilterEnvelope.reset();
		mFilter.reset();
	};
private:
	OperatorManager * opManager;
	Oscillator mOscillator;
	EnvelopeGenerator mVolumeEnvelope;
	EnvelopeGenerator mFilterEnvelope;
	Filter mFilter;

	double mFilterEnvelopeAmount;
	double mFilterLFOAmount;
	double mOscillatorPitchAmount;
	double mLFOValue;
	bool isActive;
};

#endif /* defined(__SpaceBass__Voice__) */
