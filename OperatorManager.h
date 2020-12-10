#ifndef __SpaceBass__OperatorManager__
#define __SpaceBass__OperatorManager__

//#define SPARSE_MODULATION
//#define PROPORTIONAL_MOD
#include "Operator.h"


class OperatorManager {
public:
	friend class Operator;
	friend class Voice;
	friend class VoiceManager;
	OperatorManager(): mVelocity(0), algorithm(0) {
#ifdef SPARSE_MODULATION
		mCurrStep = 0;
#endif
		for (int i = 0; i < numOperators; i++) {
			ops[i] = new Operator(this);
		}
	}

	~OperatorManager() {
		for (int i = 0; i < numOperators; i++) {
			delete ops[i];
		}
	}

	inline void reset() {
		mVelocity = 0;
		for (int i = 0; i < numOperators; i++) {
			ops[i]->reset();
		}
	}

	inline void onNoteOn() {
		for (int i = 0; i < numOperators; i++) {
			ops[i]->mVolumeEnvelope.enterStage(EnvelopeGenerator::ENVELOPE_STAGE_ATTACK);
			ops[i]->mFilterEnvelope.enterStage(EnvelopeGenerator::ENVELOPE_STAGE_ATTACK);
		}
	}

	inline void onNoteOff() {
		for (int i = 0; i < numOperators; i++) {
			ops[i]->mVolumeEnvelope.enterStage(EnvelopeGenerator::ENVELOPE_STAGE_RELEASE);
			ops[i]->mFilterEnvelope.enterStage(EnvelopeGenerator::ENVELOPE_STAGE_RELEASE);
		}
	}

	inline void setSampleRate(double sampleRate) {
		//sampleRate is static within oscillator
		ops[0]->mOscillator.setSampleRate(sampleRate);
	}

	inline void setFrequency(double freq) {
		for (int i = 0; i < numOperators; i++) {
			ops[i]->setFrequency(freq);
		}
	}

	inline void setPitchMod(double amount) {
		for (int i = 0; i < numOperators; i++) {
			ops[i]->setPitchMod(amount);
		}
	}

	inline void setAlgorithm(int alg) { algorithm = alg; }

	double nextSample();

	static const int numOperators = 4;
	static const int numAlgorithms = 14;
	static const int stepsBetweenMod = 4;
	
private:
	const double fmMagnitude = 1.0;
	Operator * ops[numOperators];
#ifdef SPARSE_MODULATION
	int mCurrStep;
#endif
	int mVelocity;
	int algorithm;
};



#endif