
#include "device.h"
#include "plib_clk.h"





/*********************************************************************************
Initialize Main Clock (MAINCK) 
*********************************************************************************/
static void CLK_MainClockInitialize(void)
{	

    /* Enable Main Crystal Oscillator */
    PMC_REGS->CKGR_MOR = (PMC_REGS->CKGR_MOR & ~CKGR_MOR_MOSCXTST_Msk) | CKGR_MOR_KEY_PASSWD | CKGR_MOR_MOSCXTST(255) | CKGR_MOR_MOSCXTEN_Msk;

	/* Wait until the main oscillator clock is ready */
    while ( (PMC_REGS->PMC_SR & PMC_SR_MOSCXTS_Msk) != PMC_SR_MOSCXTS_Msk);
	
    /* Main Crystal Oscillator is selected as the Main Clock (MAINCK) source. 
	Switch Main Clock (MAINCK) to Main Crystal Oscillator clock */
    PMC_REGS->CKGR_MOR|= CKGR_MOR_KEY_PASSWD | CKGR_MOR_MOSCSEL_Msk;

	/* Wait until MAINCK is switched to Main Crystal Oscillator */
    while ( (PMC_REGS->PMC_SR & PMC_SR_MOSCSELS_Msk) != PMC_SR_MOSCSELS_Msk);
	
}

/*********************************************************************************
Initialize PLLA (PLLACK)
*********************************************************************************/

static void CLK_PLLAInitialize(void)
{
	/* Configure and Enable PLLA */ 
    PMC_REGS->CKGR_PLLAR = CKGR_PLLAR_ONE_Msk | CKGR_PLLAR_PLLACOUNT(0x3f) |
                              CKGR_PLLAR_MULA(25 - 1) |
                              CKGR_PLLAR_DIVA(1);

    while ( (PMC_REGS->PMC_SR & PMC_SR_LOCKA_Msk) != PMC_SR_LOCKA_Msk);
}


/*********************************************************************************
Initialize Master clock (MCK)
*********************************************************************************/

static void CLK_MasterClockInitialize(void)
{
	/* Program PMC_MCKR.PRES and wait for PMC_SR.MCKRDY to be set 	*/
	PMC_REGS->PMC_MCKR = (PMC_REGS->PMC_MCKR & ~PMC_MCKR_PRES_Msk) | PMC_MCKR_PRES_CLK_1;
    while ((PMC_REGS->PMC_SR & PMC_SR_MCKRDY_Msk) != PMC_SR_MCKRDY_Msk);
	
    /* Program PMC_MCKR.MDIV and Wait for PMC_SR.MCKRDY to be set 	*/	
	PMC_REGS->PMC_MCKR = (PMC_REGS->PMC_MCKR & ~PMC_MCKR_MDIV_Msk) | PMC_MCKR_MDIV_PCK_DIV2;
    while ((PMC_REGS->PMC_SR & PMC_SR_MCKRDY_Msk) != PMC_SR_MCKRDY_Msk);	
	
    /* Program PMC_MCKR.CSS and Wait for PMC_SR.MCKRDY to be set	*/	
	PMC_REGS->PMC_MCKR = (PMC_REGS->PMC_MCKR & ~PMC_MCKR_CSS_Msk) | PMC_MCKR_CSS_PLLA_CLK;
    while ((PMC_REGS->PMC_SR & PMC_SR_MCKRDY_Msk) != PMC_SR_MCKRDY_Msk);		

}





/*********************************************************************************
Initialize Programmable Clock (PCKx)
*********************************************************************************/

static void CLK_ProgrammableClockInitialize(void)
{
	/* Disable selected programmable clock	*/
	PMC_REGS->PMC_SCDR = PMC_SCDR_PCK2_Msk;

	/* Configure selected programmable clock	*/
	PMC_REGS->PMC_PCK[2]= PMC_PCK_CSS_MAIN_CLK | PMC_PCK_PRES(0);

	/* Enable selected programmable clock	*/
	PMC_REGS->PMC_SCER = 	PMC_SCER_PCK2_Msk;
	
	/* Wait for clock to be ready	*/	
	while( (PMC_REGS->PMC_SR & (PMC_SR_PCKRDY2_Msk) ) != (PMC_SR_PCKRDY2_Msk));


}


/*********************************************************************************
Initialize Peripheral clock
*********************************************************************************/

static void CLK_PeripheralClockInitialize(void)
{
    /* Enable clock for the selected peripherals */
   	PMC_REGS->PMC_PCER0=0xcb1c00;
    PMC_REGS->PMC_PCER1=0x4000000;
}

/*********************************************************************************
Clock Initialize
*********************************************************************************/

void CLK_Initialize( void )
{ 
    /* Set Flash Wait States and  Enable Code Loop Optimization */
    EFC_REGS->EEFC_FMR = EEFC_FMR_FWS(6) | EEFC_FMR_CLOE_Msk;
	

	/* Initialize Main Clock */
	CLK_MainClockInitialize();
	
	/* Initialize PLLA */
	CLK_PLLAInitialize();
	

	/* Initialize Master Clock */
	CLK_MasterClockInitialize();
	


	/* Initialize Programmable Clock */
	CLK_ProgrammableClockInitialize();

	/* Initialize Peripheral Clock */
	CLK_PeripheralClockInitialize();
}

