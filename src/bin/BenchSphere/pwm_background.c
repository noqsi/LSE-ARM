#include "memory_7a3.h"
#include "pwm.h"
#include "aic.h"
#include "pio.h"
#include "peripheral_id_7a3.h"
#include <math.h>
#include "lse-arm.h"


/* The ATMEL PWM goes off the rails if you set the duty cycle too
 * close to the limits via the update register. 
 * Experimentally, it appears that staying more than two ticks away from
 * the extremes suffices to prevent this.
 */
 
#define PWM_GUARD 2

#define ROTATION 64	/* number of states per rotation */
#define POLES 5		/* number of poles in stator */

static int idty[ROTATION][POLES];
static int isusp[POLES];

/* Adjustable parameters */

static float 
	alpha,	/* azimuth of rotation axis, degrees */
	delta,	/* elevation of rotation axis, degrees */
	zsf,	/* z suspension, -1<zsf<1 */
	xysf,	/* xy suspension, -1<xysf<1 */
	rotf;	/* rotation field magnitude, -1<rotf<1 */

static unsigned
	period=1000,	/* default PWM clock period */
	hcycles = 4,	/* half cycle of suspension field */
	steps = 100;	/* periods per rotation state */

/* Specs for the PWM channels */
/* Z, +X, -X, +Y, -Y */

static const int channels[] = {7, 6, 5, 4, 3};

#define ALL_CHANNELS	0xff
#define MY_CHANNELS	0xf8

/* trigger bits for scope */

#define SUSP_BIT 0x20000000
#define PHASE_BIT 0x10000000	

static void pwm_isr( void )
{
	uint32_t maxdty = period-PWM_GUARD;
	
	static unsigned step, state, sstep;
	static int ss=1;		/* suspension field sign */
	int *dty = idty[state];
	int i;
	
	for( i=0; i<POLES; i+=1 ) {
		int d = dty[i] + ss * isusp[i];
		if( d > maxdty ) d = maxdty;
		else if( d < PWM_GUARD ) d = PWM_GUARD;
		PWM->channel[channels[i]].cupd = d;
	}

	if( ++sstep >= hcycles ) {	/* switch suspension sign */
		sstep = 0;
		ss = -ss;
		if( ss > 0 ) PIOA->sodr = SUSP_BIT;
		else PIOA->codr = SUSP_BIT;
	}
	
	PIOA->codr = PHASE_BIT;
	if( ++step >= steps ) {		/* advance rotation state */
		step = 0;
		
		if( ++state >= ROTATION ) {
			state = 0;
			PIOA->sodr = PHASE_BIT;	/* trigger scope */
		}
	}

	/* Do the touches to finish with the PWM and AIC interrupt hw */
	
	AIC->eoicr = PWM->isr;
}

/* LSE interface */

/* Since any glitch will be much shorter the the coil time constants,
 * we can simply recompute the duty cycle tables here, without
 * race condition worries.
 */

static void pwm_update( void )
{
	const float dr = M_PI/180., nr = 2.0*M_PI/(float) ROTATION;
	float	a = dr*alpha,
		d = dr*delta,
		sa = sin( a ),
		ca = cos( a ),
		sd = sin( d ),
		cd = cos( d ),
		mid = ((float) period)/2.0 + 0.5,
		scale = rotf * (mid - (float) PWM_GUARD);
	int n;
	
	for( n = 0; n < ROTATION; n += 1 ) {
		float 	sn = sin( nr * n ),
			cn = cos( nr * n ),
			x = scale * (ca * cd * cn + sd * sn),
			y = scale * (-ca * sd * cn + cd * sn),
			z = scale * (-sa * cn);
		int *dp = idty[n];
		
		dp[0] = mid + z;
		dp[1] = mid + x;
		dp[2] = mid - x;
		dp[3] = mid + y;
		dp[4] = mid - y;
	}
	
	isusp[0] = scale * zsf;
	isusp[1] = isusp[2] = isusp[3] = isusp[4] = scale * xysf;
}	
	

static void pwm_start( void )
{
	int i;
	
	PWM->dis = ALL_CHANNELS;
	PWM->mr = 0;		/* MCK straight through */
	PWM->ier = MY_CHANNELS;
	PWM->imr = MY_CHANNELS;
	for( i=0; i<POLES; i+=1 ) {
		struct pwm_channel *c = PWM->channel + channels[i];
		c->cmr = 0;
		c->cprd = period;
		c->cdty = period/2;	/* safe value to start */
	}
	
	/* Set up to vector the interrupt */

	AIC->svr[ PWMC_ID ] = (uint32_t) pwm_isr;
	AIC->iecr = bit( PWMC_ID );
	
	/* Make sure the tables are set up */
	
	pwm_update();

	/* Here we go... */
	
	PWM->ena = MY_CHANNELS;
}

static void pwm_stop( void ) { PWM->dis = ALL_CHANNELS; }


void build_pwm_primitives( void ) {

/* Addresses of directly accessible variables */

	build_named_constant( (cell) &period, "period");
	build_named_constant( (cell) &hcycles, "hcycles");
	build_named_constant( (cell) &steps, "steps");
	build_named_constant( (cell) &alpha, "alpha");
	build_named_constant( (cell) &delta, "delta");
	build_named_constant( (cell) &zsf, "zsf");
	build_named_constant( (cell) &xysf, "xysf");
	build_named_constant( (cell) &rotf, "rotf");
	build_named_constant( (cell) idty, "idty" );
	build_named_constant( (cell) isusp, "isusp" );
	
/* control functions */
	
	build_primitive( pwm_start, "pwm_start" );
	build_primitive( pwm_stop, "pwm_stop" );
	build_primitive( pwm_update, "pwm_update" );
}
