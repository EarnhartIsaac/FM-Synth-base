#include "OperatorManager.h"

double OperatorManager::nextSample() {

	double op0 = ops[0]->nextSample();
	double op1 = 0.0;
	double op2 = 0.0;
	double op3 = 0.0;

#ifndef SPARSE_MODULATION
	switch (algorithm) {
	case 0:
		break;
	case 1:
		ops[0]->setFreqModulation(op0 * fmMagnitude);
		break;
	case 2:
		op1 = ops[1]->nextSample();
		ops[0]->setFreqModulation(op1 * fmMagnitude);
		break;
	case 3:
		op1 = ops[1]->nextSample();
		ops[0]->setFreqModulation(op1 * fmMagnitude);
		ops[1]->setFreqModulation(op1 * fmMagnitude);
		break;
	case 4:
		op1 = ops[1]->nextSample();
		ops[0]->setFreqModulation(op1 * fmMagnitude);
		ops[1]->setFreqModulation(op0 * fmMagnitude);
		break;
	case 5:
		op1 = ops[1]->nextSample();
		op2 = ops[2]->nextSample();
		ops[0]->setFreqModulation(op1 * fmMagnitude);
		ops[1]->setFreqModulation(op2 * fmMagnitude);
		break;
	case 6:
		op1 = ops[1]->nextSample();
		op2 = ops[2]->nextSample();
		ops[0]->setFreqModulation(op1 * fmMagnitude);
		ops[1]->setFreqModulation(op2 * fmMagnitude);
		ops[2]->setFreqModulation(op2 * fmMagnitude);
		break;
	case 7:
		op1 = ops[1]->nextSample();
		op2 = ops[2]->nextSample();
		ops[0]->setFreqModulation(op1 * fmMagnitude);
		ops[1]->setFreqModulation(op2 * fmMagnitude);
		ops[2]->setFreqModulation(op1 * fmMagnitude);
		break;
	case 8:
		op1 = ops[1]->nextSample();
		op2 = ops[2]->nextSample();
		ops[0]->setFreqModulation(op1 * fmMagnitude);
		ops[1]->setFreqModulation(op2 * fmMagnitude);
		ops[2]->setFreqModulation(op0 * fmMagnitude);
		break;
	case 9:
		op1 = ops[1]->nextSample();
		op2 = ops[2]->nextSample();
		op3 = ops[3]->nextSample();
		ops[0]->setFreqModulation(op1 * fmMagnitude);
		ops[1]->setFreqModulation(op2 * fmMagnitude);
		ops[2]->setFreqModulation(op3 * fmMagnitude);
		break;
	case 10:
		op1 = ops[1]->nextSample();
		op2 = ops[2]->nextSample();
		op3 = ops[3]->nextSample();
		ops[0]->setFreqModulation(op1 * fmMagnitude);
		ops[1]->setFreqModulation(op2 * fmMagnitude);
		ops[2]->setFreqModulation(op3 * fmMagnitude);
		ops[3]->setFreqModulation(op3 * fmMagnitude);
		break;
	case 11:
		op1 = ops[1]->nextSample();
		op2 = ops[2]->nextSample();
		op3 = ops[3]->nextSample();
		ops[0]->setFreqModulation(op1 * fmMagnitude);
		ops[1]->setFreqModulation(op2 * fmMagnitude);
		ops[2]->setFreqModulation(op3 * fmMagnitude);
		ops[3]->setFreqModulation(op2 * fmMagnitude);
		break;
	case 12:
		op1 = ops[1]->nextSample();
		op2 = ops[2]->nextSample();
		op3 = ops[3]->nextSample();
		ops[0]->setFreqModulation(op1 * fmMagnitude);
		ops[1]->setFreqModulation(op2 * fmMagnitude);
		ops[2]->setFreqModulation(op3 * fmMagnitude);
		ops[3]->setFreqModulation(op1 * fmMagnitude);
		break;
	case 13:
		op1 = ops[1]->nextSample();
		op2 = ops[2]->nextSample();
		op3 = ops[3]->nextSample();
		ops[0]->setFreqModulation(op1 * fmMagnitude);
		ops[1]->setFreqModulation(op2 * fmMagnitude);
		ops[2]->setFreqModulation(op3 * fmMagnitude);
		ops[3]->setFreqModulation(op0 * fmMagnitude);
		break;
	}
#else
	//Update frequency modulations
	if (!mCurrStep) {
		switch (algorithm) {
		case 0:
			break;
		case 1:
			ops[0]->setFreqModulation(op0 * fmMagnitude);
			break;
		case 2:
			op1 = ops[1]->nextSample();
			ops[0]->setFreqModulation(op1 * fmMagnitude);
			break;
		case 3:
			op1 = ops[1]->nextSample();
			ops[0]->setFreqModulation(op1 * fmMagnitude);
			ops[1]->setFreqModulation(op1 * fmMagnitude);
			break;
		case 4:
			op1 = ops[1]->nextSample();
			ops[0]->setFreqModulation(op1 * fmMagnitude);
			ops[1]->setFreqModulation(op0 * fmMagnitude);
			break;
		case 5:
			op1 = ops[1]->nextSample();
			op2 = ops[2]->nextSample();
			ops[0]->setFreqModulation(op1 * fmMagnitude);
			ops[1]->setFreqModulation(op2 * fmMagnitude);
			break;
		case 6:
			op1 = ops[1]->nextSample();
			op2 = ops[2]->nextSample();
			ops[0]->setFreqModulation(op1 * fmMagnitude);
			ops[1]->setFreqModulation(op2 * fmMagnitude);
			ops[2]->setFreqModulation(op2 * fmMagnitude);
			break;
		case 7:
			op1 = ops[1]->nextSample();
			op2 = ops[2]->nextSample();
			ops[0]->setFreqModulation(op1 * fmMagnitude);
			ops[1]->setFreqModulation(op2 * fmMagnitude);
			ops[2]->setFreqModulation(op1 * fmMagnitude);
			break;
		case 8:
			op1 = ops[1]->nextSample();
			op2 = ops[2]->nextSample();
			ops[0]->setFreqModulation(op1 * fmMagnitude);
			ops[1]->setFreqModulation(op2 * fmMagnitude);
			ops[2]->setFreqModulation(op0 * fmMagnitude);
			break;
		case 9:
			op1 = ops[1]->nextSample();
			op2 = ops[2]->nextSample();
			op3 = ops[3]->nextSample();
			ops[0]->setFreqModulation(op1 * fmMagnitude);
			ops[1]->setFreqModulation(op2 * fmMagnitude);
			ops[2]->setFreqModulation(op3 * fmMagnitude);
			break;
		case 10:
			op1 = ops[1]->nextSample();
			op2 = ops[2]->nextSample();
			op3 = ops[3]->nextSample();
			ops[0]->setFreqModulation(op1 * fmMagnitude);
			ops[1]->setFreqModulation(op2 * fmMagnitude);
			ops[2]->setFreqModulation(op3 * fmMagnitude);
			ops[3]->setFreqModulation(op3 * fmMagnitude);
			break;
		case 11:
			op1 = ops[1]->nextSample();
			op2 = ops[2]->nextSample();
			op3 = ops[3]->nextSample();
			ops[0]->setFreqModulation(op1 * fmMagnitude);
			ops[1]->setFreqModulation(op2 * fmMagnitude);
			ops[2]->setFreqModulation(op3 * fmMagnitude);
			ops[3]->setFreqModulation(op2 * fmMagnitude);
			break;
		case 12:
			op1 = ops[1]->nextSample();
			op2 = ops[2]->nextSample();
			op3 = ops[3]->nextSample();
			ops[0]->setFreqModulation(op1 * fmMagnitude);
			ops[1]->setFreqModulation(op2 * fmMagnitude);
			ops[2]->setFreqModulation(op3 * fmMagnitude);
			ops[3]->setFreqModulation(op1 * fmMagnitude);
			break;
		case 13:
			op1 = ops[1]->nextSample();
			op2 = ops[2]->nextSample();
			op3 = ops[3]->nextSample();
			ops[0]->setFreqModulation(op1 * fmMagnitude);
			ops[1]->setFreqModulation(op2 * fmMagnitude);
			ops[2]->setFreqModulation(op3 * fmMagnitude);
			ops[3]->setFreqModulation(op0 * fmMagnitude);
			break;

		}
	}
	else {
#ifndef PROPORTIONAL_MOD
		switch (algorithm) {
		case 0:
			break;
		case 1:
			break;
		case 2:
			op1 = ops[1]->nextSample();
			break;
		case 3:
			op1 = ops[1]->nextSample();
			break;
		case 4:
			op1 = ops[1]->nextSample();
			break;
		case 5:
			op1 = ops[1]->nextSample();
			op2 = ops[2]->nextSample();
			break;
		case 6:
			op1 = ops[1]->nextSample();
			op2 = ops[2]->nextSample();
			break;
		case 7:
			op1 = ops[1]->nextSample();
			op2 = ops[2]->nextSample();
			break;
		case 8:
			op1 = ops[1]->nextSample();
			op2 = ops[2]->nextSample();
			break;
		case 9:
			op1 = ops[1]->nextSample();
			op2 = ops[2]->nextSample();
			op3 = ops[3]->nextSample();
			break;
		case 10:
			op1 = ops[1]->nextSample();
			op2 = ops[2]->nextSample();
			op3 = ops[3]->nextSample();
			break;
		case 11:
			op1 = ops[1]->nextSample();
			op2 = ops[2]->nextSample();
			op3 = ops[3]->nextSample();
			break;
		case 12:
			op1 = ops[1]->nextSample();
			op2 = ops[2]->nextSample();
			op3 = ops[3]->nextSample();
			break;
		case 13:
			op1 = ops[1]->nextSample();
			op2 = ops[2]->nextSample();
			op3 = ops[3]->nextSample();
			break;
		}
#endif
	}
	mCurrStep = (mCurrStep + 1) % stepsBetweenMod;
#endif

	return op0;
}