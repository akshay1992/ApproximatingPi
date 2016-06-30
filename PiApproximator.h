#ifndef PI_APPROXIMATOR
#define PI_APPROXIMATOR

#include <atomic>	// to share the approximation value between display and audio threads
#include <sstream>	// for converting float to string
#include <cmath>	// for pow, M_PI
#include <iomanip> 	// for setprecision
#include <vector>	// for n-channels of transposed approximations
#include "SquareOsc.h" // for audio synthesis

//! Number of digits to consider in the approximation of Pi.
#define APPROXIMATOR_PRECISION 10

const int TRANSPOSITION_FACTOR[] = { 9, 28, 50, 73, 98, 123, 149, 175, 202, 229, 257, 285, 314, 343, 372, 402};

void getDigits(double number, int digits[]);

std::string numberToString(double number);

/* 
This class is a generaic pi approximator implemented for use in the sonification of it's digits.
It approximates based on the infinite series:
Pi = 4(1 - 1/3 + 1/5 - 1/7 + 1/9 .....)
 
It is designed to be framework independent. No handles are provided for the visuals since this is usually framework dependent. However, atomic member variables are available for access to the current approximation and a sample counter. 
 
 This class can (technically) be reused to do any kind of series approximations by modifying the contents of the approximate() method. But this is not supported.
*/

class PiApproximator
{
public:
	PiApproximator(int transpose_factor = 1);

    void setTransposeFactor(int transpose_factor);

	std::atomic<double> currentApprox;

    // Moves forward in time by one sample.
    float tick();
    
    void Reset(void);
    void end();
    bool hasEnded(void){ return reachedEnd; }

    
    virtual void drawDigits(void) {};
    virtual void drawStatus(void) {};
    
    bool isMuted(void) { return audioDisabled; }
    void toggleMute(void) { audioDisabled = !audioDisabled; }
    
    std::atomic<unsigned long> sampleCounter;

private:
    void approximate();
    void computePartialAmps(double number, float output_array[APPROXIMATOR_PRECISION]);
    float getAudioSample(void);

	int sample_drop;
	int ticker = 0;
	long int currentApproxIndex;
    bool audioDisabled = false;
    bool reachedEnd = false;
    SquareOsc partials[APPROXIMATOR_PRECISION];
	int currentSign;
};

#endif
