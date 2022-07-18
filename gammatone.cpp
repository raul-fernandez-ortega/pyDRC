#include <math.h>
#include "gammatone.h"

// See http://rvl4.ecn.purdue.edu/~malcolm/apple/tr35/PattersonsEar.pdf
//     http://www.tml.tkk.fi/~ktlokki/Publs/lokkidafx.pdf

struct ERBdata ERBGlasberg = {(double)9.26449, (double)24.7, 1};
struct ERBdata ERBLyon = {(double)125.0, (double)8.0, 2};
struct ERBdata ERBGreenwood= {(double)7.23824,(double)22.8509,1 };

double ERB(double freq, double EarQ, double minBW, int Order)
{
  return (pow(freq/EarQ,(double)Order)+pow(pow(minBW,(double)Order),(double)1.0/(double)Order));
};

double ERBfromModel(double freq, ERBdata ERBd)
{
  return ERB(freq, ERBd.EarQ, ERBd.minBW, ERBd.Order);
};

double StepFactor(ERBdata ERBd, double lowFreq, double highFreq, int NChannels)
{
  return (ERBd.EarQ*(log(highFreq + ERBd.EarQ * ERBd.minBW)-log(lowFreq + ERBd.EarQ * ERBd.minBW)))/NChannels;
};

STLvectorReal CenterFrequencies(ERBdata ERBd, double StepFactor, double lowfreq, double highFreq)
{
  STLvectorReal freqarray;
  double fpoint;
  unsigned int I = 1;
  do 
    {
      fpoint = (highFreq + ERBd.EarQ * ERBd.minBW)/(exp(I*StepFactor/ERBd.EarQ)) - ERBd.EarQ * ERBd.minBW;
      if(fpoint > 0)
	freqarray.push_back(fpoint);
      I++;
    } while(fpoint > lowfreq);
  return freqarray;
};

STLvectorReal gammatone(const int length, const int order, double freq, double bandw, 
			int samplerate)
{
  double t;
  unsigned int I;
  STLvectorReal function;
  for( I = 0; I < (unsigned int)length; I++)
    {
      t = (double)I/(double)samplerate;
      function.push_back((pow(t, (double)(order - 1))* cos((double)2*M_PI*freq*t))
			*exp((double)-2*M_PI*bandw*t));
    }
  return function;
		       
};

STLvectorReal PlackOxemhamWin(int SampleRate, double threshold)
{
  STLvectorReal PreMasking, PostMasking, FinalWindow;
  double t, weight;
  unsigned int I;
  // premasking
  I = 0;
  do {
    t = (double)(-I)/(double)SampleRate*1000;
    weight = (double)0.975*exp(t/(double)4.)+(double)0.025*exp(t/(double)29.);
    PreMasking.push_back(weight);
    I++;
  } while (weight > threshold);
  //postmasking
  I = 0;
  do {
    t = (double)(I)/(double)SampleRate*1000;
    weight = exp(-t/(double)3.5);
    PostMasking.push_back(weight);
    I++;
  } while (weight > threshold);
  for(I = 1; I <= PostMasking.size(); I++)
    FinalWindow.push_back(PostMasking[PostMasking.size()-I]);
  for(I = 0; I < PreMasking.size(); I++)
    FinalWindow.push_back(PreMasking[I]);
  return FinalWindow;
};

double PlackOxenhamWindow(STLvectorReal Indata, int center, int SampleRate)
{
  unsigned int I, elements = 0;
  double weight, result = 0, t;
  I = center;
  do 
    {
      t = ((double)I - (double)center)/(double)SampleRate*1000;
      weight = (double)0.975*exp(t/(double)4.)+(double)0.025*exp(t/(double)29.);
      //printf("NEG:%e\t%e\t%e\n",t,weight,result);
      result +=Indata[I]*Indata[I]*weight;
      elements++;
      I--;
    } while(I >= 0 && weight > 0.00001);
  I = center;
  do 
    {
      t = ((double)I - (double)center)/(double)SampleRate*1000;
      weight = exp(-t/(double)3.5);
      //printf("POS:%e\t%e\t%e\n",t,weight,result);
      result +=Indata[I]*Indata[I]*weight;
      elements++;
      I++;
    } while(I < Indata.size() && weight > 0.00001);
  return result/(double) elements;
};
