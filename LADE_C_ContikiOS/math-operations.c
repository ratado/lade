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

#include "math-operations.h"
#include <math.h>


static float sampleA[N], sampleB[N];
static float p[N], q[N];
static unsigned short distsFilled;
static unsigned short resetSampleA, resetSampleB, fullA, fullB;

unsigned short addSample(float energy){
	if(!fullA || resetSampleA){
		//printf("Add Energy in A: %ld.%03u\n", (long)energy, (unsigned)((energy-floor(energy))*1000));
		fullA =	addSampleA(energy);
	}
	else if(!fullB || resetSampleB){
		//printf("Add Energy in B: %ld.%03u\n", (long)energy, (unsigned)((energy-floor(energy))*1000));
		fullB = addSampleB(energy);
	}
	if(fullA && fullB){
		return 1;
	}
	return 0;
}

void printSamples(){
	printSampleA();
	printSampleB();
}

unsigned short addSampleA(float x){
  static unsigned short i; 
  // Reset i
  if(i<0 || i >= N || resetSampleA>0){
    i=0;
		distsFilled = 0;
		resetSampleA = 0;
  }
//printf("X = %ld.%04lu, i=%u, sampleA[i] = %ld.%04lu ", (unsigned long)x, (unsigned long)((x-floor(x))*10000), i, (unsigned long)sampleA[i], (unsigned long)((sampleA[i]-floor(sampleA[i]))*10000));
//printf("filled: %ld.%04lu\n", (unsigned long)sampleA[i-1], (unsigned long)((sampleA[i-1]-floor(sampleA[i-1]))*10000));

	sampleA[i++] = x;

	if(i==N){
		return 1;
	}
	return 0;
}

void printSampleA(){
	static unsigned short x;
	printf("Sample A; ");
	for(x=0;x<N;x++){
		printf("%lu.%03lu; ", (unsigned long)sampleA[x], (unsigned long)((sampleA[x]-floor(sampleA[x]))*1000));
	}
	// printf("\n");
}

unsigned short addSampleB(float x){
  static unsigned short j;
  // Reset j
  if(j<0 || j >= N || resetSampleB>0){
    j=0;
		distsFilled = 0;
		resetSampleB = 0;
  }
//printf("X = %ld.%04lu ", (unsigned long)x, (unsigned long)((x-floor(x))*10000));
//printf("filled: %ld.%04lu\n", (unsigned long)sampleB[j-1], (unsigned long)((sampleB[j-1]-floor(sampleB[j-1]))*10000));
	sampleB[j++] = x;

	if(j==N){
		return 1;
	}
	return 0;
}

void printSampleB(){
	static unsigned short x;
	printf("Sample B; ");
	for(x=0;x<N;x++){
		printf("%lu.%03lu; ", (unsigned long)sampleB[x], (unsigned long)((sampleB[x]-floor(sampleB[x]))*1000));
	}
	printf("\n");
}

void poissonProbabilityDistribution(float *sample, float *dist, unsigned int expec){
	static unsigned short x, k, i;
	static float f, p;
	p = 1/expf(expec);
	printf("Dists;");
	for (x=0; x<N; x++){
		f=1.0;
		k = sample[x]*2 + 1; // Convert to integer (samples of 0.5 - sample[x]/0.5 + 1)
		for(i=1;i<=k; i++){
			f *= expec/(float)i;
		}    
  	dist[x] = p*f;
		printf("%lu.%05lu; ", (unsigned long)dist[x], (unsigned long)((dist[x]-floor(dist[x]))*100000));
	}

}


void probabilityDistribution(float *sample, float *distr){
	static unsigned short x;
	static float sumx;
	sumx = 0;
	for (x=0; x<N; x++){
		sumx += sample[x];	
	}
	for(x=0; x<N; x++){
		distr[x] = (sample[x]/sumx);
		//printf("%ld.%03u ", (long)distr[x], (unsigned)((distr[x]-floor(distr[x]))*1000));
	}
	//printf ("\n");
}

float D(float *p, float *q){
	static float sumR,log;
	static unsigned short x;
	sumR=0.0;
	for (x=0; x<N; x++){
		//sumR += p[x]*(float)(__ieee754_logf(p[x]/(0.5*(p[x]+q[x]))));
		log = log2f(p[x]) - log2f(q[x]);
		sumR += p[x]*log;
		//printf("Log of %ld.%03u / %ld.%03u: %ld.%03u\n",(long)p[x], (unsigned)((p[x]-floor(p[x]))*1000), (long)q[x], (unsigned)((q[x]-floor(q[x]))*1000), (long)sumR, (unsigned)((sumR-floor(sumR))*1000));
		
  }
//printf("SumR = %ld.%05u \n", (long)sumR, (unsigned)((sumR-floor(sumR))*100000));
	return sumR;
}

float sibsonDistance(){
	static float pq[N];
	static unsigned short x;
	if(!distsFilled){
		poissonProbabilityDistribution(&sampleA[0], &p[0], LAMBDA);	
		poissonProbabilityDistribution(&sampleB[0], &q[0], LAMBDA);
		printf("\n");
		distsFilled = 1;
	}

	for(x=0;x<N;x++){
		pq[x] = (p[x]+q[x]);
		pq[x] = pq[x]/2.0;
	}
	return (0.5)*(D(&p[0],&pq[0])+D(&q[0],&pq[0]));
}

float jeffreyDistance(){
	static float sumR,aux;
	static unsigned short x;
	if(!distsFilled){
		poissonProbabilityDistribution(&sampleA[0], &p[0], LAMBDA);	
		poissonProbabilityDistribution(&sampleB[0], &q[0], LAMBDA);
		printf("\n");
		distsFilled = 1;
	}
	sumR=0.0;
	/*for (x=0; x<N; x++){
		log = log2f(p[x])-log2f(q[x]);
		sumR += (p[x]-q[x])*log;
	}*/
	for(x=0;x<N; x++){
		aux = sqrtf(p[x]) - sqrtf(q[x]);
		sumR += aux*aux;
	}
  //printf("Jeffrey debug: D(P,Q)=%ld.%03u, D(Q,P)=%ld.%03u ", (long)a, (unsigned)((a-floor(a))*1000),(long)b, (unsigned)((b-floor(b))*1000));
	return sumR;
}

float hellingerDistance(){
	static unsigned short x;
	static float sumR, aux;
	if(!distsFilled){
		poissonProbabilityDistribution(&sampleA[0], &p[0], LAMBDA);	
		poissonProbabilityDistribution(&sampleB[0], &q[0], LAMBDA);
		printf("\n");
		distsFilled = 1;
	}
	sumR = 0.0;
	for(x=0;x<N; x++){
		aux = sqrtf(p[x]) - sqrtf(q[x]);
		sumR += aux*aux;
	}
// 1/sqrt(2) = 0,707106781
	return 0.707106781f*sqrtf(sumR);
}

float euclidianDistance(){
	static unsigned short x;
	static float sumR, aux;
	if(!distsFilled){
		poissonProbabilityDistribution(&sampleA[0], &p[0], LAMBDA);	
		poissonProbabilityDistribution(&sampleB[0], &q[0], LAMBDA);
		printf("\n");
		distsFilled = 1;
	}
	sumR = 0.0;
	for(x=0;x<N; x++){
		aux = q[x]-p[x];
		sumR += aux*aux;
	}
	return sqrtf(sumR);

}

void setResetSamples(){
	setResetSampleA();
	setResetSampleB();
}

void setResetSampleA(){
	resetSampleA=1;
	fullA=0;
}
void setResetSampleB(){
	resetSampleB=1;
	fullB=0;
}
