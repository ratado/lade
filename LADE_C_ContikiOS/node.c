 /**
 * ---------------------------------------------------------------------
 *
 * LADE - Lightweight Architecture for Detection of EDAs
 *
 * https://github.com/ratado/lade
 * 
 * @author Andre Proto
 * @copyright 2024 Andre Proto and contributors.
 * @copyright 2007, Swedish Institute of Computer Science.
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
#include <stdlib.h>
#include <string.h>
#include "contiki.h"
#include "net/packetbuf.h"
#include "net/rime/rime.h"
#include "net/linkaddr.h"
#include "net/rime/timesynch.h"
#include "random.h"
#include "dev/button-sensor.h"
#include "dev/leds.h"
#include "ids-energy-based.h"
#include "collect-sample.h"
#include "environment.h"
//Libraries to manage the sensors
#include "../dev/sht11/sht11-sensor.h"
#include "../platform/sky/dev/light-sensor.h"
#include "dev/leds.h"
#include "core/sys/node-id.h"


static void unicast_recv(struct unicast_conn *c, const linkaddr_t *from);
static void broadcast_recv(struct broadcast_conn *c, const linkaddr_t *from);
static void unicast_send_data(struct unicast_conn *c);

/*---------------------------------------------------------------------------*/
static struct unicast_conn uc;
//static struct unicast_conn ucreceive;
static const struct broadcast_callbacks broadcast_call = {broadcast_recv};
static const struct unicast_callbacks unicast_callbacks = {unicast_recv};
static struct broadcast_conn broadcast;
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
PROCESS(node_sensor_process, "Node sensor");
PROCESS(task_ids, "Task IDS"); 
PROCESS(malware_task, "Malware Task"); 
AUTOSTART_PROCESSES(&node_sensor_process, &task_ids); //Start the process when the node boots
//AUTOSTART_PROCESSES(&node_sensor_process);
/*---------------------------------------------------------------------------*/

 
/*---------------------------------------------------------------------------*/
static int
get_light(void) //Get the light value from sensor
{
  return 10 * light_sensor.value(LIGHT_SENSOR_PHOTOSYNTHETIC) / 7;
}
/*---------------------------------------------------------------------------*/
static int
get_temp(void) //Get the temperature value from sensor
{
  return ((sht11_sensor.value(SHT11_SENSOR_TEMP) / 10) - 396) / 10;
}
/*---------------------------------------------------------------------------*/
  
int sequence = 0; // A sequence number that enumerates the data from 0 and increases in 1 each time.


static void collect_sensors(struct environment *envir)
{
  static int temporal; //Temporal Variable  
  
  SENSORS_ACTIVATE(light_sensor); // Activate light sensor
  SENSORS_ACTIVATE(sht11_sensor); // Activate temperature sensor
  
  //printf("Data\t"); // Print the string "Data"
  sequence++;
  //printf("%d\t", sequence);  // Print the sequence number
  (*envir).sequence = sequence; // To save the sequence in the struct envir
  temporal = get_temp();
	//printf("%d\t", temporal = get_temp()); // Print the temperature value
  (*envir).temp = temporal; // To save the temperature in the struct envir
	temporal = get_light();  
	//printf("%d\n", temporal = get_light());  // Print the light value
  (*envir).light = temporal; // To save the light in the struct envir
 
  //process_post(&example_unicast_process, PROCESS_EVENT_CONTINUE , &(envir) ); // This function posts an asynchronous event to the process example_unicast_process with the information of the structure called envir
  
  SENSORS_DEACTIVATE(light_sensor); // Deactivate light sensor
  SENSORS_DEACTIVATE(sht11_sensor);  // Deactivate temperature sensor

}

static void
unicast_recv(struct unicast_conn *c, const linkaddr_t *from)
{
  // do nothing
	printf("Received!\n");
}

static void
broadcast_recv(struct broadcast_conn *c, const linkaddr_t *from)
{
	static struct environment env; // Struct used to store the temperature and light values
  static linkaddr_t addr;

		collect_sensors(&env);
	unicast_open(&uc, 140, &unicast_callbacks);

		packetbuf_copyfrom(&env, sizeof(env));

			
		addr.u8[0] = from->u8[0];
		addr.u8[1] = from->u8[1];

		if(!linkaddr_cmp(&addr, &linkaddr_node_addr)) {

		  unicast_send(&uc, &addr);

		}
	unicast_close(&uc);
}

static void
unicast_send_data(struct unicast_conn *c){
	static struct environment env; // Struct used to store the temperature and light values
  static linkaddr_t addr;

	collect_sensors(&env);
	unicast_open(&uc, 140, &unicast_callbacks);
	packetbuf_copyfrom(&env, sizeof(env));
			
	addr.u8[0] = 1;
	addr.u8[1] = 0;
	if(!linkaddr_cmp(&addr, &linkaddr_node_addr)) {
	  unicast_send(&uc, &addr);
	}
	unicast_close(&uc);
}


PROCESS_THREAD(node_sensor_process, ev, data)
{
	static struct etimer et;
	static char typeScenario, scenario;
	static unsigned int randomTime, minTime;
	static unsigned short randomTimeArray[20],i;

  PROCESS_EXITHANDLER(broadcast_close(&broadcast);)
	//PROCESS_EXITHANDLER(unicast_close(&ucreceive);)

  PROCESS_BEGIN();

	scenario='E';
	i=(uint8_t)node_id;

	if(scenario=='A'){
		/* Uncomment to set Scenario A - Fixed time */
		typeScenario = 'f';
	}
	else if(scenario=='B'){
		/* Uncomment to set Scenario B - Random time between 2 and 8 sec */
		/* The random times are defined bellow to sync the several simulations when the IDS is active or not. The same values for all simulations is important to compare the results among them. */
		minTime = CLOCK_SECOND * 2;
		randomTimeArray[0] = 4; randomTimeArray[1] = 2; randomTimeArray[2] = 6; randomTimeArray[3] = 1; randomTimeArray[4] = 3;
		randomTimeArray[5] = 1; randomTimeArray[6] = 5; randomTimeArray[7] = 0; randomTimeArray[8] = 4; randomTimeArray[9] = 2;
		randomTimeArray[10] = 6; randomTimeArray[11] = 1; randomTimeArray[12] = 3; randomTimeArray[13] = 0; randomTimeArray[14] = 1;
		randomTimeArray[15] = 5; randomTimeArray[16] = 6; randomTimeArray[17] = 4; randomTimeArray[18] = 2; randomTimeArray[19] = 3;
		typeScenario = 'r';
	}
	else if(scenario=='C'){
		/* Uncomment to set Scenario C - Random time between 2 and 30 sec */
		/* The random times are defined bellow to sync the several simulations when the IDS is active or not. The same values for all simulations is important to compare the results among them. */
		minTime = CLOCK_SECOND * 2;
		randomTimeArray[0] = 15; randomTimeArray[1] = 20; randomTimeArray[2] = 6; randomTimeArray[3] = 13; randomTimeArray[4] = 18;
		randomTimeArray[5] = 3; randomTimeArray[6] = 9; randomTimeArray[7] = 0; randomTimeArray[8] = 7; randomTimeArray[9] = 26;
		randomTimeArray[10] = 16; randomTimeArray[11] = 10; randomTimeArray[12] = 3; randomTimeArray[13] = 20; randomTimeArray[14] = 11;
	  randomTimeArray[15] = 25; randomTimeArray[16] = 16; randomTimeArray[17] = 4; randomTimeArray[18] = 6; randomTimeArray[19] = 23;
		typeScenario = 'r';
	}
	else if(scenario=='D'){
		/* Uncomment to set Scenario D - Fixed time with attacker */
		typeScenario = 'f';
	}
	else if(scenario=='E'){
		/* Uncomment to set Scenario E - Random time between 2 and 30 sec with malware */
 		minTime = CLOCK_SECOND * 2;
		randomTimeArray[0] = 4; randomTimeArray[1] = 2; randomTimeArray[2] = 6; randomTimeArray[3] = 1; randomTimeArray[4] = 3;
		randomTimeArray[5] = 1; randomTimeArray[6] = 5; randomTimeArray[7] = 0; randomTimeArray[8] = 4; randomTimeArray[9] = 2;
		randomTimeArray[10] = 6; randomTimeArray[11] = 1; randomTimeArray[12] = 3; randomTimeArray[13] = 0; randomTimeArray[14] = 1;
		randomTimeArray[15] = 5; randomTimeArray[16] = 6; randomTimeArray[17] = 4; randomTimeArray[18] = 2; randomTimeArray[19] = 3;
		typeScenario = 'r';
		if(node_id==9){

			process_start(&malware_task, NULL); /* Start a malware_task */
		}
  }
	else if(scenario=='F'){
		/* Uncomment to set Scenario F - Random time with attacker requesting data */
		typeScenario = 'f';
	}
	else{ return;}

	/* Start  task */
	if(typeScenario=='f'){ 
		broadcast_open(&broadcast, 129, &broadcast_call);

		while(1) {
			PROCESS_WAIT_EVENT();
		}
	}
	else if(typeScenario=='r'){
		while(1) {
			if(i>=20) {	i=0; }
			randomTime = CLOCK_SECOND * randomTimeArray[i];

			etimer_set(&et, minTime + randomTime);
    	PROCESS_YIELD_UNTIL(etimer_expired(&et));
			unicast_send_data(&uc);

			etimer_reset(&et);	

			i++;
		}
	}
  PROCESS_END();
}

PROCESS_THREAD(task_ids, ev, data)
{
	static struct etimer et;

	//PROCESS_EXITHANDLER()
	PROCESS_BEGIN();
	etimer_set(&et, CLOCK_SECOND * COLLECT_INTERVAL);

	while(1) {
    PROCESS_YIELD_UNTIL(etimer_expired(&et));

		taskDetection();

		etimer_reset(&et);
	
   	//powertrace_get(&info);
    //printf("DATA: %lu %lu %lu %lu\n",info.all_cpu, info.all_lpm, info.all_transmit, info.all_listen);
	}

	PROCESS_END();
}
	

PROCESS_THREAD(malware_task, ev, data)
{
static struct etimer et;
	static float matrix[35][35];
	static int i,j,k;

	//PROCESS_EXITHANDLER()
	PROCESS_BEGIN();
	etimer_set(&et, CLOCK_SECOND * 2);
printf("Inicio ataque\n");
		while(1) {
			PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

			for(k=0;k<3;k++){
				for(i=0;i<35;i++){
					for(j=0;j<35;j++){
						matrix[i][j] = ((i+j)/((i+1)*(j+1))+i*j+i/(j+1))*((i+j)/((i+1)*(j+1))-i*j+i/(j+1));
					}
				}
			}
etimer_reset(&et);

	}

	PROCESS_END();
}
/*---------------------------------------------------------------------------*/
