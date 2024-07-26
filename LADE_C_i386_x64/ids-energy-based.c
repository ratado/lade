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

//#define IDS_DELTA 0.1f
//#define IDS_RHO 4.0f
#define ANOMALY_VALIDATION_TIMES 10 

static double IDS_DELTA;
static char DISTANCE_METRIC;


void taskDetection(void){
	static unsigned short sampleFilled, analyze,detected, stopDetection;
	static double energy;
	static double metric;
    static short anomaly_times, anomaly_secs;

	sampleFilled = taskCollect();
    
    if(anomaly_times > 0) {
        anomaly_secs++;
        if(anomaly_secs > (ANOMALY_VALIDATION_TIMES*N*4)){
            //printf("Cancel anomaly detection\n");
            anomaly_times = 0;
            anomaly_secs = 0;
        }
    }
    
	
	if(sampleFilled) {
		detected=0;
		printSamples();
		if(stopDetection < (N-1)){
            
            if(DISTANCE_METRIC == 's'){
            
                //printf("Time %lu start Sibson: %lu\n", RTIMER_SECOND, RTIMER_NOW());
                metric = sibsonDistance();
                //printf("Time end Sibson: %lu\n", RTIMER_NOW());
                printf("Sibson: %lf\n ", metric);
                if (metric <= IDS_DELTA) {
                    anomaly_times++;
                    
                    if(anomaly_times > ANOMALY_VALIDATION_TIMES/3){
                        printf("Anomaly detected; DISTANCE: Sibson Distance; METRIC: %lf\n", metric);
                        anomaly_times = 0;
                        anomaly_secs = 0;
                    }
                    detected++;
                }
            }
            else if (DISTANCE_METRIC == 'e'){
                //printf("Time %lu Inicio Euclidian: %lu\n", RTIMER_SECOND, RTIMER_NOW());
                metric = euclidianDistance();		
                //printf("Time end Euclidian: %lu\n", RTIMER_NOW());
                printf("Euclidian: %lf\n", metric);
                if (metric <= IDS_DELTA) {
                    anomaly_times++;
                    
                    if(anomaly_times > ANOMALY_VALIDATION_TIMES/3){
                        printf("Anomaly detected; DISTANCE: Euclidian Distance; METRIC: %lf\n", metric);
                        detected++;
                        anomaly_times = 0;
                        anomaly_secs = 0;
                    }
                }
            }
            else if (DISTANCE_METRIC == 'h'){
                metric = hellingerDistance();		
                printf("Hellinger: %lf\n", metric);
                if (metric <= IDS_DELTA) {
                    anomaly_times++;
                    printf("Entrou\n");
                    if(anomaly_times > ANOMALY_VALIDATION_TIMES/3){
                        printf("Anomaly detected; DISTANCE: Hellinger Distance; METRIC: %lf\n", metric);
                        detected++;
                        anomaly_times = 0;
                        anomaly_secs = 0;
                        
                    }
                }
            }
		}
		sampleFilled=0;
		setResetSamples();
		if(!detected){
			analyze = 0;
			stopDetection = 0;
		}
	}
}

unsigned short taskCollect(void){
	static unsigned short sampleFilled, analyze,detected, stopDetection, p;
	static double energy, energyConv;
    static double LAMBDA;

    
    LAMBDA = getLambda();
	collect_data_energy();
//	analyze=1; // Insert only when you need to analyze energy consumption
	
	
  /* 
	 * We collect energy data each 1 second, but sample vectors work with 2 sec sample. 
	 * So, this task needs to collect energy two times before insert it in vector.	
	 */
	if(p < SAMPLE_SIZE-1) {
        if(p == 0) energy = 0.0;
		p++;
		energy += get_total_energy();
		//printf("Half Energy collected: %ld.%04u\n", (unsigned long)halfEnergy, (unsigned long)((halfEnergy-floor(halfEnergy))*10000));
	}
	else{
		p=0;
		energy += get_total_energy();
		
        
        energyConv = (energy * SAMPLE_MULTIPLIER) + 1;
        //printf("Energy collected: %lf, conv=%lf\n", energy, energyConv);
		if(energyConv <= LAMBDA){
			stopDetection++;
			/*
			 * Stop the detection whether the energy value is least then IDS_RHO for N-1 times. 
			 * This will preserve processing and energy.
			 */
			if(stopDetection >= (N-1)){ 
				//printf("Stop detection\n");
				analyze = 0;
				stopDetection = 0;
				setResetSamples();

			}
		}

		if(analyze > 0 || energyConv > LAMBDA){
			//printf("Add sample!\n");
			analyze=1;
			if(addSample(energy) > 0){
				stopDetection = 0;
				return 1;
			}
		}
	}
	return 0;
}

void setThreshold(char *val){
    IDS_DELTA = atof(val);

}
void setDistanceMetric(char dmetric){
    DISTANCE_METRIC = dmetric;

}
