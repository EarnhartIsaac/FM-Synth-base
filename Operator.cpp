//
//  Operator.cpp
//  FM Shmynth
//
//  Created by Isaac on 11.13.20.
//
//

#include "Operator.h"
#include "OperatorManager.h"

double Operator::nextSample() {
	double oscillatorOutput = mOscillator.nextSample();

	double volumeEnvelopeValue = mVolumeEnvelope.nextSample();
	double filterEnvelopeValue = mFilterEnvelope.nextSample();

	mFilter.setCutoffMod(filterEnvelopeValue * mFilterEnvelopeAmount + mLFOValue * mFilterLFOAmount);
	setPitchMod(mLFOValue * mOscillatorPitchAmount);

	return mFilter.process(oscillatorOutput * volumeEnvelopeValue * (opManager->mVelocity) / 127.0);
}