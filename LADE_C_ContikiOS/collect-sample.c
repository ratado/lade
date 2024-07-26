/**
 * ---------------------------------------------------------------------
 *
 * LADE - Lightweight Architecture for Detection of EDAs
 *
 * https://github.com/ratado/lade
 * 
 * @author Andre Proto
 * @copyright 2024 Andre Proto and contributors.
 * @licence   https://www.gnu.org/licenses/gpl-3.0.html
 *
 * ---------------------------------------------------------------------
 *
 * LICENSE
 *
 * This file is part of GLPI.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * ---------------------------------------------------------------------
 */

#include "collect-view.h"
#include "collect-view-sky.h"
//#include "powertrace.h"
#include "net/linkaddr.h"
#include "net/rime/timesynch.h"
#include "dev/leds.h"
#include <string.h>
#include <ctype.h>
#include "sys/energest.h"
#include "collect-sample.h"

#define VOLTAGE 3
static unsigned long POWER_CPU = 54ul; // Value in mW, 1.800*VOLTAGE*DIV_POWER_CPU
//static uint16_t DIV_POWER_CPU = 10ul;
static unsigned long POWER_LPM = 1635ul; // Value in mW, 0,0545*VOLTAGE*DIV_POWER_LPM
//static uint16_t DIV_POWER_LPM = 10000ul;
static unsigned long POWER_TRANSMIT = 531ul; // Value in mW, 17.7*VOLTAGE*DIV_POWER_TRANSMIT
//static uint16_t DIV_POWER_TRANSMIT = 10ul;
static unsigned long POWER_LISTEN = 60ul; // Value in mW, 20.0*VOLTAGE*DIV_POWER_LISTEN
//static uint16_t DIV_POWER_LISTEN = 1ul;

static energy_t time_energy;

/*
float get_total_energy(void){
	float a,b;
	a = ((float)msg.cpu)*POWER_CPU;
	a += ((float)msg.lpm)*POWER_LPM;
	a += ((float)msg.transmit)*POWER_TRANSMIT;
	a += ((float)msg.listen)*POWER_LISTEN;
  b = ((float)msg.lpm) + ((float)msg.cpu);
	//b = RTIMER_SECOND * COLLECT_INTERVAL;
	return a/b;
}
*/

/**
* To avoid precision errors, the equation bellow is calculated element by element. So, mmc of 
* each element is defined and used in calc.
* 
* total = (cpu + lpm + transmit + listen) / (cpu + lpm)
* 
* See in beggining of document the definition of power consumption for each elements cpu, lpm, 
* transmit and listen to understand the values used to calculate mmc variables. 
*
*/
float get_total_energy(void){
	static unsigned long a,b,aux,mmc,e_mmc;
	static unsigned short num_decimals;
	mmc = 10000; // Minimum multiple common of DIV_POWER_x
	e_mmc = mmc/10;
	a = time_energy.cpu * POWER_CPU * e_mmc;
	e_mmc = mmc/10000;
	aux = time_energy.lpm * POWER_LPM * e_mmc;
	a += aux;
	e_mmc = mmc/10;
	aux = time_energy.transmit * POWER_TRANSMIT * e_mmc;
	a += aux;
	e_mmc = mmc/1;	
	aux = time_energy.listen * POWER_LISTEN * e_mmc;
	a += aux;	

	b = (time_energy.lpm + time_energy.cpu) * mmc;

// We will get only two decimal digits. It will be sufficient
	num_decimals = 3;
	aux = a/b;
	e_mmc = a%b;
	mmc=1;
	while(num_decimals>0){
		mmc *= 10;
		e_mmc *= 10;
		aux = aux*10 + e_mmc/b;
		e_mmc = e_mmc%b;
		num_decimals--;
	}
	return aux/(float)mmc;

}

void collect_data_energy(){

	/* Update all energest times. */
    energest_flush();
		// Set current values
		time_energy.cpu = energest_type_time(ENERGEST_TYPE_CPU) - time_energy.last_cpu;
  	time_energy.lpm = energest_type_time(ENERGEST_TYPE_LPM) - time_energy.last_lpm;
  	time_energy.transmit = energest_type_time(ENERGEST_TYPE_TRANSMIT) - time_energy.last_transmit;
  	time_energy.listen = energest_type_time(ENERGEST_TYPE_LISTEN) - time_energy.last_listen;

		// Set last values
		time_energy.last_cpu += time_energy.cpu;
		time_energy.last_lpm += time_energy.lpm;
		time_energy.last_transmit += time_energy.transmit;
		time_energy.last_listen += time_energy.listen;
/*
    printf(" TICKS_PER_CLOCK: %u\nEnergest:\n", RTIMER_SECOND );
    printf(" CPU          %lu LPM      %lu  Total time %lu\n",
           time_energy.cpu,
           time_energy.lpm,
           //energest_type_time(ENERGEST_TYPE_DEEP_LPM),
           time_energy.cpu+time_energy.lpm);
    printf(" Radio LISTEN %lu TRANSMIT %lu OFF      %lu\n",
           time_energy.listen,
           time_energy.transmit,
           time_energy.cpu+time_energy.lpm
                      - time_energy.transmit
                      - time_energy.listen);
*/  

}


