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

#include "ids-energy-based.h"
#include "math-operations.h"
#include "collect-sample.h"
#include <stdio.h>
#include <stdlib.h>

#define IDS_DELTA 0.1f
//#define IDS_RHO 4.0f

void taskDetection(void){
	static unsigned short sampleFilled, analyze,detected, stopDetection;
	static float energy;
	static float metric;	

	sampleFilled = taskCollect();
	
	if(sampleFilled) {
		detected=1;
		printSamples();
//		if(stopDetection < (N-1)){
//			metric = sibsonDistance();
//			printf("Sibson: %lu.%04lu; ", (unsigned long)metric, (unsigned long)((metric-floor(metric))*10000));
//			if (metric <= IDS_DELTA) {
				//printf("Anomaly detected; DISTANCE: Sibson Distance; METRIC: %ld.%04u\n", (long)metric, (unsigned)((metric-floor(metric))*10000));
//				detected++;
//			}
//			metric = euclidianDistance();		
//			printf("Euclidian: %ld.%04u; \n", (unsigned long)metric, (unsigned long)((metric-floor(metric))*10000));
//			if (metric <= IDS_DELTA) {
		//printf("Anomaly detected; DISTANCE: Euclidian Distance; METRIC: %ld.%04u\n", (long)metric, (unsigned)((metric-floor(metric))*10000));
//				detected++;
//			}
			metric = hellingerDistance();		
			printf("Hellinger: %ld.%04u\n", (unsigned long)metric, (unsigned long)((metric-floor(metric))*10000));
			if (metric <= IDS_DELTA) {
		//printf("Anomaly detected; DISTANCE: Hellinger Distance; METRIC: %ld.%04u\n", (long)metric, (unsigned)((metric-floor(metric))*10000));
				detected++;
			}
//		}
		sampleFilled=0;
		setResetSamples();
//		if(!detected){
//			analyze = 0;
//			stopDetection = 0;
//		}
	}
}

unsigned short taskCollect(void){
	static unsigned short sampleFilled, analyze,detected, stopDetection, p;
	static float energy, halfEnergy;

	analyze=1;
	collect_data_energy();
	//analyze=1; // Insert only when you need to analyze energy consumption
	
	
  /* 
	 * We collect energy data each 1 second, but sample vectors work with 2 sec sample. 
	 * So, this task needs to collect energy two times before insert it in vector.	
	 */
	if(p==0){		 
		p++;
		halfEnergy = get_total_energy();
		//printf("Half Energy collected: %ld.%04u\n", (unsigned long)halfEnergy, (unsigned long)((halfEnergy-floor(halfEnergy))*10000));
	}
	else{
		p=0;
		energy = get_total_energy() + halfEnergy;
		//printf("Energy collected: %ld.%04u\n", (unsigned long)energy, (unsigned long)((energy-floor(energy))*10000));
//		if(energy <= LAMBDA){
//			stopDetection++;
			/*
			 * Stop the detection whether the energy value is least then IDS_RHO for N-1 times. 
			 * This will preserve processing and energy.
			 */
//			if(stopDetection >= (N-1)){ 
				//printf("Stop detection\n");
//				analyze = 0;
//				stopDetection = 0;
//				setResetSamples();

//			}
//		}

		if(analyze > 0 || energy > LAMBDA){
			//printf("Add sample!\n");
			analyze=1;
			if(addSample(energy) > 0){
//				stopDetection = 0;
				return 1;
			}
		}
	}
	return 0;
}

