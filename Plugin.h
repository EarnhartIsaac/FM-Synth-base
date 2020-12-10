#ifndef __SPACEBASS__
#define __SPACEBASS__

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wextra-tokens"
#include "IPlug_include_in_plug_hdr.h"
#pragma clang diagnostic pop

#include "MIDIReceiver.h"
#include "VoiceManager.h"

enum EParams
{
	mOperator = 0,
	mAlgorithm,

	mLFOWaveform,
	mLFOFrequency,

	mOscWaveform,
	mOscPitchMod,
	mFilterMode,
	mFilterCutoff,
	mFilterResonance,
	mFilterLfoAmount,
	mFilterEnvAmount,
	mVolumeEnvAttack,
	mVolumeEnvDecay,
	mVolumeEnvSustain,
	mVolumeEnvRelease,
	mFilterEnvAttack,
	mFilterEnvDecay,
	mFilterEnvSustain,
	mFilterEnvRelease,

	kNumParams
};

typedef struct {
	const char* name;
	const int x;
	const int y;
	const double defaultVal;
	const double minVal;
	const double maxVal;
} parameterProperties_struct;

const parameterProperties_struct parameterProperties[kNumParams] = {
	//+110
	{ "Operator", 535, 5, 0.0, 0.0, 0.0 },
	{ "Algorithm", 8, 8, 0.0, 0.0, 0.0 },
	{ "LFO Waveform", 170, 298, 0.0, 0.0, 0.0 },
	{ "LFO Frequency", 120, 293, 6.0, 0.01, 30.0 },
	{ "Osc Waveform", 170, 75, 0.0, 0.0, 0.0 },
	{ "Osc Pitch Mod", 120, 70, 0.0, 0.0, 1.0 },
	{ "Filter Mode", 170, 188, 0.0, 0.0, 0.0 },
	{ "Filter Cutoff", 323, 155, 0.99, 0.0, 0.99 },
	{ "Filter Resonance", 378, 155, 0.0, 0.0, 1.0 },
	{ "Filter LFO Amount", 433, 155, 0.0, 0.0, 1.0 },
	{ "Filter Envelope Amount", 488, 155, 0.0, -1.0, 1.0 },
	{ "Volume Env Attack", 323, 61, 0.01, 0.01, 10.0 },
	{ "Volume Env Decay", 378, 61, 0.5, 0.01, 15.0 },
	{ "Volume Env Sustain", 433, 61, 0.1, 0.001, 1.0 },
	{ "Volume Env Release", 488, 61, 1.0, 0.01, 15.0 },
	{ "Filter Env Attack", 323, 250, 0.01, 0.01, 10.0 },
	{ "Filter Env Decay", 378, 250, 0.5, 0.01, 15.0 },
	{ "Filter Env Sustain", 433, 250, 0.1, 0.001, 1.0 },
	{ "Filter Env Release", 488, 250, 1.0, 0.01, 15.0 }
};

enum ELayout
{
	kWidth = GUI_WIDTH,
	kHeight = GUI_HEIGHT,
	kKeybX = 62,
	kKeybY = 425
};

class SpaceBass : public IPlug
{
public:
  SpaceBass(IPlugInstanceInfo instanceInfo);
  ~SpaceBass();

  void Reset();
  void OnParamChange(int paramIdx);
  void ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames);
  // to receive MIDI messages:
  void ProcessMidiMsg(IMidiMsg* pMsg);
  
  // Needed for the GUI keyboard:
  // Should return non-zero if one or more keys are playing.
  inline int GetNumKeys() const { return mMIDIReceiver.getNumKeys(); };
  // Should return true if the specified key is playing.
  inline bool GetKeyStatus(int key) const { return mMIDIReceiver.getKeyStatus(key); };
  static const int virtualKeyboardMinimumNoteNumber = 48;
  int lastVirtualKeyboardNoteNumber;
  
private:
  double mFrequency;
  void CreatePresets();
  MIDIReceiver mMIDIReceiver;
  IControl* mVirtualKeyboard;
  void processVirtualKeyboard();

  //Pointers to non-param graphics
  //IControl * algoControl;
  //IControl * oscWaveControl;
  //IControl * LFOWaveControl;
  //IControl * filterModeControl;
  
  IGraphics * pGraphics;
  int paramToGraphicIndex[kNumParams];

  int opIndex;
  ITextControl * opTextOutput;
  
  void adjustControlsForOp();
  inline IControl * getControlByParam(int paramIdx) {
	  return pGraphics->GetControl(paramToGraphicIndex[paramIdx]);
  }

  double filterEnvelopeAmount;
  
  double lfoFilterModAmount;
  
  void InitParam(int paramIdx);
  void CreateParams();
  void CreateGraphics();
  
  VoiceManager voiceManager;
};

#endif
