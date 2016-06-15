#include "PiApproximator.h"
#include <iostream>

#define N_SAMPS 5040

PiApproximator::PiApproximator(int transpose_factor)
{
    setTransposeFactor(transpose_factor);
    Reset();
}

void PiApproximator::Reset(void)
{
    sampleCounter = 0;
    currentApprox = 4;
    currentApproxIndex = 0;
    currentSign = 1;
    reachedEnd = false;
}

void PiApproximator::setTransposeFactor(int transpose_factor)
{
    currentApprox = 4;
    currentApproxIndex = 0;
    currentSign = 1;
    
    sample_drop = 5040/transpose_factor;
    
    for (int i=0; i<APPROXIMATOR_PRECISION; i++)
    {
        float frequency = (8.75 * (i+1)) * transpose_factor;
        partials[i].set_freq(frequency);
    }
}

void PiApproximator::approximate(void)
{
	currentApproxIndex++;
	currentSign *= -1;
	double previousApprox = currentApprox;
	currentApprox = previousApprox + currentSign* 4.0/(2.0*(currentApproxIndex+1) - 1);
}

float PiApproximator::tick()
{
    if(!reachedEnd)
    {
        ++sampleCounter;
        if(ticker < sample_drop)
        {
            ticker++;
        }
        else
        {
            ticker = 0;
            approximate();
        }
        
        return audioDisabled ? 0 : getAudioSample();
    }
    else
    {
        return 0;
    }
}

void PiApproximator::computePartialAmps(double number, float output_array[])
{
	int digits[APPROXIMATOR_PRECISION];

	getDigits(number, digits);
    

	for(int i=0; i<APPROXIMATOR_PRECISION; i++)
	{
		output_array[i] = pow(2, digits[i]);
		output_array[i] *= 2 * M_PI/(i+1);		//Sawtooth-spectral factor
        
        output_array[i] /= pow(2, 10) * M_PI;
	}
}

void PiApproximator::end(void)
{
    reachedEnd = true;
}

float PiApproximator::getAudioSample(void)
{
    float out=0;
    float amplitudes[APPROXIMATOR_PRECISION];
    computePartialAmps(currentApprox, amplitudes);
    
    for (int i=0; i<APPROXIMATOR_PRECISION; i++)
    {
        out += amplitudes[i] * partials[i].tick();
    }
    return out*1.0/APPROXIMATOR_PRECISION;
}

void getDigits(double number, int digits[APPROXIMATOR_PRECISION])
{
	for(int i=0; i<APPROXIMATOR_PRECISION; i++)
	{
		digits[i] = number;
		number = number - digits[i];
		number = number * 10;
	}
}

std::string numberToString(double number)
{
	std::stringstream s;
	s << std::fixed << std::setprecision(APPROXIMATOR_PRECISION) << number;
	return s.str();
}
