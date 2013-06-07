/*
 * Quick & dirty ADC driver
 */
 
#include "adc_driver.h"

static const struct adc *adcs[] = {ADC0, ADC1};

#define ADCCLK_HZ 5000000

extern const unsigned mck_hz;

/*
 * Initialize, set speed to max, sleep between conversions
 */

void adc_init( int a )
{
	adcs[a].cr = SWRST;
	adcs[a].mr = ((mck_hz/(2*ADCCLK_HZ))<<PRESCAL_SHIFT) |
		(12<<STARTUP_SHIFT) |
		(3<<SHTIM_SHIFT) |
		SLEEP;	
}

/* Tell ADC to sleep between conversions */

void adc_sleep( int a )
{
	adcs[a].mr |= SLEEP;
}

/* Tell ADC to stay awake between conversions */

void adc_stay_awake( int a )
{
	adcs[a].mr &= ~SLEEP;
}

/* Set channel mask to m, initiate conversion */

void adc_start( int a, m )
{
	adcs[a].chdr = 0xff;	/* disable 'em all */
	adcs[a].cher = m;	/* enable what we want */
	adcs[a].cr = START;
}

/* True if all channels in mask m have converted data available */

int adc_ready( int a, m )
{
	return m == (m & adcs[a].sr );
}

/* Fetch data from channel n */

int adc_fetch( int a, n )
{
	return adcs[a].cdr[n];
}
	
