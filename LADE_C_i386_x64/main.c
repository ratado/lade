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

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "ids-energy-based.h"
#include "collect-sample.h"
#include "math-operations.h"
#include "environment.h"


short loadParameter(char *param, char *value){
    if (strlen(param) < 2 || param[0] != '-') return 0;
    
    if(param[1] == 'e'){
        setEnergySample(*value);
    }
    else if (param[1] == 'd'){
        setDistanceMetric(*value);
    }
    else if (param[1] == 't'){
        setThreshold(value);   
    }
    else if (param[1] == 'l'){
        setLambda(value);   
    }
    /*else if (param[1] == 'n'){
        setN(value);   
    }*/
    return 1;
}

void printHelp(char *argv[]){
    printf("\nUsage: %s [parameters]\n\n" , argv[0]);
    
    printf("Parameters:\n\n");
    printf("\t-e: Select the set of energy samples. Ex.: n = no attack; j = jamming attack; m = malware attack\n" );       
    printf("\t-d: Select the distance metric. Ex.: s = Sibson; e = Euclidian; h = hellinger\n" ); 
    printf("\t-t: Set the threshold. Ex.: 0.2\n" ); 
    printf("\t-l: Set lambda value. Ex.: 2.0\n" ); 
    //printf("\t-n: Set N window size. Ex.: 8\n" ); 
    
    printf("\n\n Ex.: %s -e n -d s -t 0.2 -l 2.0 -n 8\n\n", argv[0]);

}

void main(int argc, char *argv[] ){
    int seconds = 1500;
    int i = 0;
	clock_t begin, end;
	double time_spent;
	
    if(argc < 8){
        printHelp(argv);
        exit(1);
    }
    
    for (i=1; i < argc; i=i+2){
        if(!loadParameter(argv[i], argv[i+1])){
            printHelp(argv);
            exit(1);
        }
    } 
    
	
	i = 0;
    //printf("N=%d\n",N);
    while(i < seconds){

		begin = clock();
        taskDetection();
		end = clock();
		
		//time_spent = (double)(end - begin) / (CLOCKS_PER_SEC / 100);
		//sleep(1);
		i++;
    }
}

