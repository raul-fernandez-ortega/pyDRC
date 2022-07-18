#ifndef gammatone_h
#define gammatone_h

#include "stl_type.h"

//#define M_PI ((double) 3.14159265358979323846264338327950288)

struct ERBdata {
  double EarQ;
  double minBW;
  int Order;
};

double ERB(double freq, double EarQ, double minBW, int Order);
double ERBfromModel(double freq, ERBdata ERBd);
double StepFactor(ERBdata ERBd, double lowFreq, double highFreq, int NChannels);
STLvectorReal CenterFrequencies(ERBdata ERBd, double StepFactor, double lowfreq, double highfreq);
STLvectorReal gammatone(const int length, const int order, double freq, double bandw,
			int samplerate);
STLvectorReal PlackOxemhamWin(int SampleRate, double threshold);
double PlackOxenhamWindow(STLvectorReal Indata, int center, int SampleRate);

#endif
