// performs pedometer and misc functions 
#include <math.h> 
#include <stdlib.h>
#include "app_env.h"
#include "lib.h"
#include "accel_lib.h"
#include "pedometer_lib.h"

#define PED_THRESHOLD_LOW 6
#define PED_THRESHOLD_HIGH 20

#define PED_WALK_FUDGE 1
#define PED_AVGLEN 8
#define PED_LATENCY 4
#define PED_AVGCONST 1 //can be optimized if left as 1
#define PED_AVGTHRESH 3

// vars used by fancy_pedometer 
char stepflag; 
float stride, avgstride, accel_dat[50]; 
float maxavg, minavg, accel_avg, velocity, displace; 
volatile int distance;
volatile int steps;

uint8_t i, cycle_count, tot_samples; 
int16_t rssdat, newmax, newmin, oldavg, newavg; 


//helper function to get total steps
int getSteps(void)
{
	QPRINTF("Total Steps: %d\r\n", steps);
	return steps;
}

//helper function to get total distance
int getDistance(void)
{
	return (int)distance;
}
/**
 ****************************************************************************************
 * @brief Measures difference between two samples, if within threshold bounds it is 
 *        marked as a step. A small debounc is added to make sure one large event is
 *        registered twice. About 50%-80% accurate.
 ****************************************************************************************
 */
void basic_pedometer(void)
{
	
	int8_t oldx = 0;
	int8_t xdat, ydat, zdat; 
	steps = 0;
	uint8_t debounce = 0;
	//while(1)
	//{
		accel_get_sample(&xdat, &ydat, &zdat);
		if(xdat - oldx > PED_THRESHOLD_LOW && xdat - oldx < PED_THRESHOLD_HIGH)
		{
			if(debounce > 0)
			{
				debounce--;
			}
			else
			{
				steps++;
				QPRINTF("%d, %d\r\n", steps, xdat - oldx);
				debounce = 2;
			}
		}
		//QPRINTF("%d\r\n", (xdat - oldx));
		oldx = xdat;
	//}
	
}

/**
 ****************************************************************************************
 * @brief Setup vars for fancy pedometer function.
 ****************************************************************************************
 */
void setup_fancy_pedometer()
{
	stepflag = 2;
	maxavg = -10; 
	minavg = 10; 
	newmax = -10; 
	newmin = 10; 
	oldavg = 0; 
	newavg = 0; 
	cycle_count = 0; 
	tot_samples = 0; 
	steps = 0; 
	distance = 0; 
	accel_avg = 0; 
	velocity = 0; 
	displace = 0; 
	avgstride = 0; 
}

/**
 ****************************************************************************************
 * @brief Elaborate pedometer function based on Analog devices app note.
 *        After initial setup this function is called every time there is new data.
 *        About 90% accurate. Works very well.
 ****************************************************************************************
 */
void fancy_pedometer() 
{ 
	int8_t xdat, ydat, zdat;
	
	if (tot_samples > 7) // subtract first sample in sliding boxcar avg 
	{ 
		oldavg = newavg; 
		newavg -= accel_dat[cycle_count - PED_AVGLEN]; 
	} // if 
	accel_get_sample(&xdat, &ydat, &zdat); // get data from accelerometer 
	//xdat -= 255; // subtract Zero g value 
	//ydat -= 255; 
	rssdat = sqrt((float)(xdat*xdat + ydat*ydat)/16.0); // vector sum 
	accel_dat[cycle_count] = rssdat; // place current sample data in buffer 
	newavg += rssdat; // add new sample to sliding boxcar avg 
	if((abs(newavg-oldavg)) < PED_AVGTHRESH) 
	newavg = oldavg; 
	if (rssdat > newmax) 
	newmax = rssdat; 
	if (rssdat < newmin) 
	newmin = rssdat; 
	tot_samples++; 
	cycle_count++; // increment count of samples in current step 
	if (tot_samples > 8) 
	{ 
		//QPRINTF("%d, %d, %d, %d\r\n", newavg, oldavg, cycle_count, avglen);
		if (IsStep(newavg, oldavg)) 
		{ 
			for (i = PED_LATENCY; i < (cycle_count - PED_LATENCY); i++) 
				accel_avg += accel_dat[i]; 
			
			accel_avg /= (cycle_count - PED_AVGLEN); 
			for (i = PED_LATENCY; i < (cycle_count - PED_LATENCY); i++) 
			{ 
				velocity += (accel_dat[i] - accel_avg); 
				displace += velocity; 
			} // create integration and double integration 
			// calculate stride length 
			stride = displace * (newmax - newmin) / (accel_avg - newmin); 
			stride = sqrt(abs(stride)); 
			// use appropriate constant to get stride length 
			stride *= PED_WALK_FUDGE; 
			// generate exponential average of stride length to smooth data 
			if (steps < 2) 
				avgstride = stride; 
			else 
				avgstride = ((PED_AVGCONST-1)*avgstride + stride)/PED_AVGCONST; 

			steps++; 
			distance += avgstride; 
			// need all data used in calculating newavg 
			for (i = 0; i < PED_AVGLEN; i++) 
				accel_dat[i] = accel_dat[cycle_count + i - PED_AVGLEN]; 
			
			QPRINTF("STEPS: %d\t DIST: %d\t Vel: %d\r\n", steps, distance, avgstride);
			
			//reset tracking vars
			cycle_count = PED_AVGLEN; 
			newmax = -10; 
			newmin = 10;
			maxavg = -10; 
			minavg = 10; 
			accel_avg = 0; 
			velocity = 0; 
			displace = 0; 
			 
		} // we have a new step 
	} // enough samples to start checking for step (need at least 8) 
} // fancy_pedometer() 




/**
 ****************************************************************************************
 * @brief Called by fancy_pedometer() to determine based on sliding window if a step is
 *        taken. Needs to be updated to take ints, not floats.
 ****************************************************************************************
 */
char IsStep(float avg, float oldavg) 
{ 
	// this function attempts to determine when a step is complete 
	float step_thresh = 5.0; // used to prevent noise from fooling the algorithm 
	if (stepflag == 2) 
	{ 
		if (avg > (oldavg + step_thresh)) 
			stepflag = 1; 
		if (avg < (oldavg - step_thresh)) 
			stepflag = 0; 
		return 0; 
	} // first time through this function 
	if (stepflag == 1) 
	{ 
		if ((maxavg > minavg) && (avg > ((maxavg+minavg)/2)) && (oldavg < ((maxavg+minavg/2)))) 
			return 1; 
		if (avg < (oldavg - step_thresh)) 
		{ 
			stepflag = 0; 
			if (oldavg > maxavg) 
				maxavg = oldavg; 
		} // slope has turned down 
		return 0; 
	} // slope has been up 
	if (stepflag == 0) 
	{ 
		if (avg > (oldavg + step_thresh)) 
		{ 
			stepflag = 1; 
			if (oldavg < minavg) 
				minavg = oldavg; 
		} // slope has turned up 
		return 0; 
	} // slope has been down 
	return 0; 
} // IsStep() 

