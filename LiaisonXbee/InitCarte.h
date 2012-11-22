
void initModesAndClock(); 				/* Initialize mode entries and system clock */
void initPeriClkGen();  				/* Initialize peripheral clock generation for DSPIs */
void disableWatchdog(); 				/* Disable watchdog */
	
void initPads();             			/* Initialize pads used in example */
void initADC();              			/* Init. ADC for normal conversions but don't start yet*/
void initCTU();              			/* Configure desired CTU event(s) */
void init_LinFLEX_0_UART();             /* Init. UART Transmission/REception */

void enableIrq(void);