#include "adc_driver.h"
#include "lse-arm.h"

static void adc_star( void )
{
	cell mask = *sp++;
	adc_start( *sp++, mask );
}


static void adc_rdy( void )
{
	cell mask = *sp++;
	flag = adc_ready( *sp++, mask );
}


static void adc_a( void )
{
	cell chan = *sp++;
	*sp = adc_fetch( *sp, chan );
}


void adc_primitives( void )
{
	build_primitive( adc_star, "adc*" );
	build_primitive( adc_rdy, "adc?" );
	build_primitive( adc_a, "@adc" );
}
