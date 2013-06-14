struct adc;		// Tell C not to worry about what this is

void adc_init( struct adc **a, int n );
extern void adc_sleep( int a );
extern void adc_stay_awake( int a );
extern void adc_start( int a, int m );
extern int adc_ready( int a, int m );
extern int adc_fetch( int a, int n );
