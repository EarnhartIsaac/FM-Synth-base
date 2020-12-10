#include "Plugin.h"
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmain"
#include "IPlug_include_in_plug_src.h"
#pragma clang diagnostic pop
#include "IControl.h"
#include "IKeyboardControl.h"
#include "resource.h"

#include <math.h>
#include <algorithm>

#include <functional>

const int kNumPrograms = 5;

const double parameterStep = 0.001;



SpaceBass::SpaceBass(IPlugInstanceInfo instanceInfo) : IPLUG_CTOR(kNumParams, kNumPrograms, instanceInfo), lastVirtualKeyboardNoteNumber(virtualKeyboardMinimumNoteNumber - 1) {
  TRACE;
  
  CreateParams();
  CreateGraphics();
  CreatePresets();
  
  mMIDIReceiver.noteOn.Connect(&voiceManager, &VoiceManager::onNoteOn);
  mMIDIReceiver.noteOff.Connect(&voiceManager, &VoiceManager::onNoteOff);
}

SpaceBass::~SpaceBass() {}

void SpaceBass::CreateParams() {
  for (int i = 0; i < kNumParams; i++) {
    IParam* param = GetParam(i);
    const parameterProperties_struct& properties = parameterProperties[i];
    switch (i) {
        // Enum Parameters:
	  case mAlgorithm:
		param->InitEnum(properties.name,
						0,
						OperatorManager::numAlgorithms);
		break;
	  case mOperator:
		param->InitEnum(properties.name,
			            0,
			            OperatorManager::numOperators);
	    break;
      case mOscWaveform:
        param->InitEnum(properties.name,
                        Oscillator::OSCILLATOR_MODE_SAW,
                        Oscillator::kNumOscillatorModes);
        // For VST3:
        param->SetDisplayText(0, properties.name);
        break;
      case mLFOWaveform:
        param->InitEnum(properties.name,
                        Oscillator::OSCILLATOR_MODE_TRIANGLE,
                        Oscillator::kNumOscillatorModes);
        // For VST3:
        param->SetDisplayText(0, properties.name);
        break;
      case mFilterMode:
        param->InitEnum(properties.name,
                        Filter::FILTER_MODE_LOWPASS,
                        Filter::kNumFilterModes);
        break;
        // Double Parameters:
      default:
        param->InitDouble(properties.name,
                          properties.defaultVal,
                          properties.minVal,
                          properties.maxVal,
                          parameterStep);
        break;
    }
  }
  GetParam(mFilterCutoff)->SetShape(2);
  GetParam(mVolumeEnvAttack)->SetShape(3);
  GetParam(mFilterEnvAttack)->SetShape(3);
  GetParam(mVolumeEnvDecay)->SetShape(3);
  GetParam(mFilterEnvDecay)->SetShape(3);
  GetParam(mVolumeEnvSustain)->SetShape(2);
  GetParam(mFilterEnvSustain)->SetShape(2);
  GetParam(mVolumeEnvRelease)->SetShape(3);
  GetParam(mFilterEnvRelease)->SetShape(3);

  for (int i = 0; i < kNumParams; i++) {
    InitParam(i);
  }
}

void SpaceBass::CreateGraphics() {
  pGraphics = MakeGraphics(this, kWidth, kHeight);
  pGraphics->AttachBackground(BG_ID, BG_FN);
  IBitmap whiteKeyImage = pGraphics->LoadIBitmap(WHITE_KEY_ID, WHITE_KEY_FN, 6);
  IBitmap blackKeyImage = pGraphics->LoadIBitmap(BLACK_KEY_ID, BLACK_KEY_FN);
  //                            C#     D#          F#      G#      A#
  int keyCoordinates[12] = { 0, 10, 17, 30, 35, 52, 61, 68, 79, 85, 97, 102 };
  mVirtualKeyboard = new IKeyboardControl(this, kKeybX, kKeybY, virtualKeyboardMinimumNoteNumber, /* octaves: */ 4, &whiteKeyImage, &blackKeyImage, keyCoordinates);
  pGraphics->AttachControl(mVirtualKeyboard);
  IBitmap waveformBitmap = pGraphics->LoadIBitmap(WAVEFORM_ID, WAVEFORM_FN, 4);
  IBitmap filterModeBitmap = pGraphics->LoadIBitmap(FILTERMODE_ID, FILTERMODE_FN, 3);
  IBitmap knobBitmap = pGraphics->LoadIBitmap(KNOB_ID, KNOB_FN, 128);
  IBitmap algoBitmap = pGraphics->LoadIBitmap(ALGO_ID, ALGO_FN, 14);
  IBitmap buttonBitmap = pGraphics->LoadIBitmap(BUTTON_ID, BUTTON_FN, 2);
  IBitmap opBitmap = pGraphics->LoadIBitmap(OP_ID, OP_FN, 4);
  for (int i = 0; i < kNumParams; i++) {
    const parameterProperties_struct& properties = parameterProperties[i];
    IBitmap* graphic;
	IBitmap* button = &buttonBitmap;
	IControl* control1;
	IControl* control2;
	
    switch (i) {
        // Switches:
	  case mOperator:
		graphic = &opBitmap;
		control1 = new IBitmapControl(this, properties.x - 30, properties.y, i, graphic);
		control2 = new IButtonControl(this, properties.x, properties.y, i, button, OperatorManager::numOperators);
		pGraphics->AttachControl(control2);
		break;
	  case mAlgorithm:
		//algorithm bitmap will be just a graphic bitmap
		//button is switch bitmap that will change algorithm bitmap
		graphic = &algoBitmap;
		control1 = new IBitmapControl(this, properties.x, properties.y, i, graphic);
		control2 = new IButtonControl(this, properties.x + 110, properties.y, i, button, OperatorManager::numAlgorithms);
		pGraphics->AttachControl(control2);
		break;
	  case mOscWaveform:
      case mLFOWaveform:
        graphic = &waveformBitmap;
		control1 = new IBitmapControl(this, properties.x, properties.y, i, graphic);
		control2 = new IButtonControl(this, properties.x, properties.y - 35, i, button, 4);
		pGraphics->AttachControl(control2);
        break;
      case mFilterMode:
        graphic = &filterModeBitmap;
		control1 = new IBitmapControl(this, properties.x, properties.y, i, graphic);
		control2 = new IButtonControl(this, properties.x, properties.y - 35, i, button, 3);
		pGraphics->AttachControl(control2);
        break;
        // Knobs:
      default:
        graphic = &knobBitmap;
        control1 = new IKnobMultiControl(this, properties.x, properties.y, i, graphic);
        break;
    }
    paramToGraphicIndex[i] = pGraphics->AttachControl(control1);
  }
  AttachGraphics(pGraphics);
}

void SpaceBass::CreatePresets() {
}

void SpaceBass::ProcessDoubleReplacing(
                                       double** inputs,
                                       double** outputs,
                                       int nFrames)
{
  // Mutex is already locked for us.
  
  double *leftOutput = outputs[0];
  double *rightOutput = outputs[1];
  processVirtualKeyboard();
  for (int i = 0; i < nFrames; ++i) {	
    mMIDIReceiver.advance();
    leftOutput[i] = rightOutput[i] = voiceManager.nextSample();
  }
  
  mMIDIReceiver.Flush(nFrames);
}

void SpaceBass::Reset()
{
  TRACE;
  IMutexLock lock(this);
  double sampleRate = GetSampleRate();
  voiceManager.setSampleRate(sampleRate);
}

void SpaceBass::InitParam(int paramIdx)
{
	IMutexLock lock(this);
	IParam* param = GetParam(paramIdx);
	if (paramIdx == mLFOWaveform) {
		voiceManager.setLFOMode(static_cast<Oscillator::OscillatorMode>(param->Int()));
		//getControlByParam(paramIdx)->SetValueFromPlug(param->Int());
	}
	else if (paramIdx == mLFOFrequency) {
		voiceManager.setLFOFrequency(param->Value());
	}
	else if (paramIdx == mOperator) {
		opIndex = param->Int();
	}
	else if (paramIdx == mAlgorithm) {
		using std::tr1::placeholders::_1;
		using std::tr1::bind;
		VoiceManager::VoiceChangerFunction changer;
		changer = bind(&VoiceManager::setAlgorithm,
			_1,
			static_cast<Oscillator::OscillatorMode>(param->Int()));
		voiceManager.changeAllVoices(changer);
	}
	else {
		using std::tr1::placeholders::_1;
		using std::tr1::bind;
		VoiceManager::VoiceChangerFunction changer;
		for (int i = 0; i < OperatorManager::numOperators; i++) {
			switch (paramIdx) {
			case mOscWaveform:
				changer = bind(&VoiceManager::setOscillatorMode,
				_1,
				i,
				static_cast<Oscillator::OscillatorMode>(param->Int()));
				//getControlByParam(paramIdx)->SetValueFromPlug(param->Int());
				break;
			case mOscPitchMod:
				changer = bind(&VoiceManager::setOscillatorPitchMod, _1, i, param->Value());
				break;
				// Filter Section:
			case mFilterMode:
				changer = bind(&VoiceManager::setFilterMode, _1, i, static_cast<Filter::FilterMode>(param->Int()));
				//getControlByParam(paramIdx)->SetValueFromPlug(param->Int());
				break;
			case mFilterCutoff:
				changer = bind(&VoiceManager::setFilterCutoff, _1, i, param->Value());
				break;
			case mFilterResonance:
				changer = bind(&VoiceManager::setFilterResonance, _1, i, param->Value());
				break;
			case mFilterLfoAmount:
				changer = bind(&VoiceManager::setFilterLFOAmount, _1, i, param->Value());
				break;
			case mFilterEnvAmount:
				changer = bind(&VoiceManager::setFilterEnvAmount, _1, i, param->Value());
				break;
				// Volume Envelope:
			case mVolumeEnvAttack:
				changer = bind(&VoiceManager::setVolumeEnvelopeStageValue, _1, i, EnvelopeGenerator::ENVELOPE_STAGE_ATTACK, param->Value());
				break;
			case mVolumeEnvDecay:
				changer = bind(&VoiceManager::setVolumeEnvelopeStageValue, _1, i, EnvelopeGenerator::ENVELOPE_STAGE_DECAY, param->Value());
				break;
			case mVolumeEnvSustain:
				changer = bind(&VoiceManager::setVolumeEnvelopeStageValue, _1, i, EnvelopeGenerator::ENVELOPE_STAGE_SUSTAIN, param->Value());
				break;
			case mVolumeEnvRelease:
				changer = bind(&VoiceManager::setVolumeEnvelopeStageValue, _1, i, EnvelopeGenerator::ENVELOPE_STAGE_RELEASE, param->Value());
				break;
				// Filter Envelope:
			case mFilterEnvAttack:
				changer = bind(&VoiceManager::setFilterEnvelopeStageValue, _1, i, EnvelopeGenerator::ENVELOPE_STAGE_ATTACK, param->Value());
				break;
			case mFilterEnvDecay:
				changer = bind(&VoiceManager::setFilterEnvelopeStageValue, _1, i, EnvelopeGenerator::ENVELOPE_STAGE_DECAY, param->Value());
				break;
			case mFilterEnvSustain:
				changer = bind(&VoiceManager::setFilterEnvelopeStageValue, _1, i, EnvelopeGenerator::ENVELOPE_STAGE_SUSTAIN, param->Value());
				break;
			case mFilterEnvRelease:
				changer = bind(&VoiceManager::setFilterEnvelopeStageValue, _1, i, EnvelopeGenerator::ENVELOPE_STAGE_RELEASE, param->Value());
				break;
			}
			voiceManager.changeAllVoices(changer);
		}
	}
}

void SpaceBass::OnParamChange(int paramIdx)
{
  IMutexLock lock(this);
  IParam* param = GetParam(paramIdx);
  if(paramIdx == mLFOWaveform) {
    voiceManager.setLFOMode(static_cast<Oscillator::OscillatorMode>(param->Int()));
	getControlByParam(paramIdx)->SetValueFromPlug(param->GetNormalized());
  } else if(paramIdx == mLFOFrequency) {
    voiceManager.setLFOFrequency(param->Value());
  }
  else if (paramIdx == mOperator) {
	  opIndex = param->Int();
	  getControlByParam(paramIdx)->SetValueFromPlug(param->GetNormalized());
	  adjustControlsForOp();
  } 
  else{
    using std::tr1::placeholders::_1;
    using std::tr1::bind;
    VoiceManager::VoiceChangerFunction changer;
    switch(paramIdx) {
	  case mAlgorithm:
		  getControlByParam(paramIdx)->SetValueFromPlug(param->GetNormalized());
		  changer = bind(&VoiceManager::setAlgorithm,
			  _1,
			  param->Int());
		  break;
      case mOscWaveform:
		  getControlByParam(paramIdx)->SetValueFromPlug(param->GetNormalized());
          changer = bind(&VoiceManager::setOscillatorMode,
                       _1,
					   opIndex,
                       static_cast<Oscillator::OscillatorMode>(param->Int()));
          break;
      case mOscPitchMod:
		  changer = bind(&VoiceManager::setOscillatorPitchMod, _1, opIndex, param->Value());
          break;
        // Filter Section:
      case mFilterMode:
		  getControlByParam(paramIdx)->SetValueFromPlug(param->GetNormalized());
		  changer = bind(&VoiceManager::setFilterMode, _1, opIndex, static_cast<Filter::FilterMode>(param->Int()));
          break;
      case mFilterCutoff:
		  changer = bind(&VoiceManager::setFilterCutoff, _1, opIndex, param->Value());
        break;
      case mFilterResonance:
		  changer = bind(&VoiceManager::setFilterResonance, _1, opIndex, param->Value());
        break;
      case mFilterLfoAmount:
		  changer = bind(&VoiceManager::setFilterLFOAmount, _1, opIndex, param->Value());
        break;
      case mFilterEnvAmount:
		  changer = bind(&VoiceManager::setFilterEnvAmount, _1, opIndex, param->Value());
        break;
        // Volume Envelope:
      case mVolumeEnvAttack:
		  changer = bind(&VoiceManager::setVolumeEnvelopeStageValue, _1, opIndex, EnvelopeGenerator::ENVELOPE_STAGE_ATTACK, param->Value());
        break;
      case mVolumeEnvDecay:
		  changer = bind(&VoiceManager::setVolumeEnvelopeStageValue, _1, opIndex, EnvelopeGenerator::ENVELOPE_STAGE_DECAY, param->Value());
        break;
      case mVolumeEnvSustain:
		  changer = bind(&VoiceManager::setVolumeEnvelopeStageValue, _1, opIndex, EnvelopeGenerator::ENVELOPE_STAGE_SUSTAIN, param->Value());
        break;
      case mVolumeEnvRelease:
		  changer = bind(&VoiceManager::setVolumeEnvelopeStageValue, _1, opIndex, EnvelopeGenerator::ENVELOPE_STAGE_RELEASE, param->Value());
        break;
        // Filter Envelope:
      case mFilterEnvAttack:
		  changer = bind(&VoiceManager::setFilterEnvelopeStageValue, _1, opIndex, EnvelopeGenerator::ENVELOPE_STAGE_ATTACK, param->Value());
        break;
      case mFilterEnvDecay:
		  changer = bind(&VoiceManager::setFilterEnvelopeStageValue, _1, opIndex, EnvelopeGenerator::ENVELOPE_STAGE_DECAY, param->Value());
        break;
      case mFilterEnvSustain:
		  changer = bind(&VoiceManager::setFilterEnvelopeStageValue, _1, opIndex, EnvelopeGenerator::ENVELOPE_STAGE_SUSTAIN, param->Value());
        break;
      case mFilterEnvRelease:
		  changer = bind(&VoiceManager::setFilterEnvelopeStageValue, _1, opIndex, EnvelopeGenerator::ENVELOPE_STAGE_RELEASE, param->Value());
        break;
    }
    voiceManager.changeAllVoices(changer);
  }
}

void SpaceBass::ProcessMidiMsg(IMidiMsg* pMsg) {
  mMIDIReceiver.onMessageReceived(pMsg);
  mVirtualKeyboard->SetDirty();
}

void SpaceBass::processVirtualKeyboard() {
  IKeyboardControl* virtualKeyboard = (IKeyboardControl*) mVirtualKeyboard;
  int virtualKeyboardNoteNumber = virtualKeyboard->GetKey() + virtualKeyboardMinimumNoteNumber;
  
  if(lastVirtualKeyboardNoteNumber >= virtualKeyboardMinimumNoteNumber && virtualKeyboardNoteNumber != lastVirtualKeyboardNoteNumber) {
    // The note number has changed from a valid key to something else (valid key or nothing). Release the valid key:
    IMidiMsg midiMessage;
    midiMessage.MakeNoteOffMsg(lastVirtualKeyboardNoteNumber, 0);
    mMIDIReceiver.onMessageReceived(&midiMessage);
  }
  
  if (virtualKeyboardNoteNumber >= virtualKeyboardMinimumNoteNumber && virtualKeyboardNoteNumber != lastVirtualKeyboardNoteNumber) {
    // A valid key is pressed that wasn't pressed the previous call. Send a "note on" message to the MIDI receiver:
    IMidiMsg midiMessage;
    midiMessage.MakeNoteOnMsg(virtualKeyboardNoteNumber, virtualKeyboard->GetVelocity(), 0);
    mMIDIReceiver.onMessageReceived(&midiMessage);
  }
  
  lastVirtualKeyboardNoteNumber = virtualKeyboardNoteNumber;
}

void SpaceBass::adjustControlsForOp() {
	IParam * param;
	for (int i = 0; i < kNumParams; i++) {
		param = GetParam(i);
		switch (i) {
		case mOscWaveform:
			getControlByParam(i)->SetValueFromUserInput(param->GetNormalized(voiceManager.getOscillatorMode(opIndex)));
			break;
		case mOscPitchMod:
			getControlByParam(i)->SetValueFromUserInput(param->GetNormalized(voiceManager.getOscillatorPitchMod(opIndex)));
			break;
			// Filter Section:
		case mFilterMode:
			getControlByParam(i)->SetValueFromUserInput(param->GetNormalized(voiceManager.getFilterMode(opIndex)));
			break;
		case mFilterCutoff:
			getControlByParam(i)->SetValueFromUserInput(param->GetNormalized(voiceManager.getFilterCutoff(opIndex)));
			break;
		case mFilterResonance:
			getControlByParam(i)->SetValueFromUserInput(param->GetNormalized(voiceManager.getFilterResonance(opIndex)));
			break;
		case mFilterLfoAmount:
			getControlByParam(i)->SetValueFromUserInput(param->GetNormalized(voiceManager.getFilterLFOAmount(opIndex)));
			break;
		case mFilterEnvAmount:
			getControlByParam(i)->SetValueFromUserInput(param->GetNormalized(voiceManager.getFilterEnvAmount(opIndex)));
			break;
			// Volume Envelope:
		case mVolumeEnvAttack:
			getControlByParam(i)->SetValueFromUserInput(param->GetNormalized(voiceManager.getVolumeEnvelopeStageValue(EnvelopeGenerator::ENVELOPE_STAGE_ATTACK, opIndex)));
			break;
		case mVolumeEnvDecay:
			getControlByParam(i)->SetValueFromUserInput(param->GetNormalized(voiceManager.getVolumeEnvelopeStageValue(EnvelopeGenerator::ENVELOPE_STAGE_DECAY, opIndex)));
			break;
		case mVolumeEnvSustain:
			getControlByParam(i)->SetValueFromUserInput(param->GetNormalized(voiceManager.getVolumeEnvelopeStageValue(EnvelopeGenerator::ENVELOPE_STAGE_SUSTAIN, opIndex)));
			break;
		case mVolumeEnvRelease:
			getControlByParam(i)->SetValueFromUserInput(param->GetNormalized(voiceManager.getVolumeEnvelopeStageValue(EnvelopeGenerator::ENVELOPE_STAGE_RELEASE, opIndex)));
			break;
			// Filter Envelope:
		case mFilterEnvAttack:
			getControlByParam(i)->SetValueFromUserInput(param->GetNormalized(voiceManager.getFilterEnvelopeStageValue(EnvelopeGenerator::ENVELOPE_STAGE_ATTACK, opIndex)));
			break;
		case mFilterEnvDecay:
			getControlByParam(i)->SetValueFromUserInput(param->GetNormalized(voiceManager.getFilterEnvelopeStageValue(EnvelopeGenerator::ENVELOPE_STAGE_DECAY, opIndex)));
			break;
		case mFilterEnvSustain:
			getControlByParam(i)->SetValueFromUserInput(param->GetNormalized(voiceManager.getFilterEnvelopeStageValue(EnvelopeGenerator::ENVELOPE_STAGE_SUSTAIN, opIndex)));
			break;
		case mFilterEnvRelease:
			getControlByParam(i)->SetValueFromUserInput(param->GetNormalized(voiceManager.getFilterEnvelopeStageValue(EnvelopeGenerator::ENVELOPE_STAGE_RELEASE, opIndex)));
			break;
		default:
			break;
		}
		
	}
}