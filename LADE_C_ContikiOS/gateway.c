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
#include <string.h>
#include "contiki.h"
#include "net/packetbuf.h"
#include "net/rime/rime.h"
#include "random.h"
#include "dev/button-sensor.h"
#include "dev/leds.h"
#include "environment.h"

static void unicast_recv(struct unicast_conn *c, const linkaddr_t *from);
static void broadcast_recv(struct broadcast_conn *c, const linkaddr_t *from);
/*---------------------------------------------------------------------------*/
static const struct broadcast_callbacks broadcast_call = {broadcast_recv};
static const struct unicast_callbacks unicast_callbacks = {unicast_recv};
static struct broadcast_conn broadcast;
static struct unicast_conn uc;
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
PROCESS(gateway_process, "Gateway Sensor");
PROCESS(receive_data, "Receiver");
AUTOSTART_PROCESSES(&gateway_process, &receive_data);
/*---------------------------------------------------------------------------*/
static void
unicast_recv(struct unicast_conn *c, const linkaddr_t *from)
{
	static struct environment *env;
	env = (struct environment *)packetbuf_dataptr();
  //printf("unicast message received from %d.%d, data: %d %d %d\n",
	// from->u8[0], from->u8[1], env->sequence, env->temp, env->light);
	packetbuf_clear();
}
static void
broadcast_recv(struct broadcast_conn *c, const linkaddr_t *from)
{
  // do nothing
//printf("broadcast message received from %d.%d, data: %s\n",
	 //from->u8[0], from->u8[1], (char *)packetbuf_dataptr());
	//packetbuf_clear();
}

PROCESS_THREAD(gateway_process, ev, data)
{
  static struct etimer et;
	static unsigned int minTime, randomTime;
 	static char requestData[20], typeScenario, scenario;
	static short randomTimeArray[20], i;

  PROCESS_EXITHANDLER(broadcast_close(&broadcast);)

  PROCESS_BEGIN();

	scenario='F';

	if(scenario=='A'){
  	/* Uncomment to set Scenario A - Fixed time */
		minTime = CLOCK_SECOND * 5;
		randomTime = 0;
		typeScenario='f';
	}
	else if(scenario=='B'){
		/* Uncomment to set Scenario B - Random time between 2 and 8 sec */
		typeScenario='r';
	}
	else if(scenario=='C'){
		/* Uncomment to set Scenario C - Random time between 2 and 30 sec */
		typeScenario='r';
	}
	else if(scenario=='D'){
		/* Uncomment to set Scenario D - Fixed time with attacker */
		minTime = CLOCK_SECOND * 5;
		randomTime = 0;
		typeScenario='f';
	}
	else if(scenario=='E'){
		/* Uncomment to set Scenario E - Random time between 2 and 30 sec with malware */
		typeScenario='r';
  }
	else if(scenario=='F'){
		/* Uncomment to set Scenario F - Random time  2 to 8 with attacker requesting data */
		randomTimeArray[0] = 4; randomTimeArray[1] = 2; randomTimeArray[2] = 8; randomTimeArray[3] = 2; randomTimeArray[4] = 3;
		randomTimeArray[5] = 3; randomTimeArray[6] = 5; randomTimeArray[7] = 2; randomTimeArray[8] = 7; randomTimeArray[9] = 3;
		randomTimeArray[10] = 4; randomTimeArray[11] = 2; randomTimeArray[12] = 4; randomTimeArray[13] = 2; randomTimeArray[14] = 5;
  	randomTimeArray[15] = 7; randomTimeArray[16] = 8; randomTimeArray[17] = 3; randomTimeArray[18] = 6; randomTimeArray[19] = 2;
		typeScenario = 'f';
		minTime = 0;
		i = 0;
	}
	else {return;}

	/* Starting task */
	if(typeScenario=='f'){
  	broadcast_open(&broadcast, 129, &broadcast_call);
  
	  while(1) {

			//printf("GATEWAY: %lu Waiting for %lu seconds.\n",CLOCK_SECOND,randomTime+minTime);
		  
		  if(scenario=='F'){
				if(i>=20) {	i=0; }
				randomTime = CLOCK_SECOND * randomTimeArray[i++];
			}

			etimer_set(&et, minTime + randomTime);
		  PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

			sprintf(requestData, "request data");
		  packetbuf_copyfrom(requestData, strlen(requestData)+1);
		  broadcast_send(&broadcast);

			etimer_reset(&et);
  	}
	}
	else if(typeScenario=='r'){
		while(1){
			PROCESS_WAIT_EVENT();
		}
	}

  PROCESS_END();
}

PROCESS_THREAD(receive_data, ev, data){

	PROCESS_EXITHANDLER(unicast_close(&uc);)
	PROCESS_BEGIN();

	unicast_open(&uc, 140, &unicast_callbacks);

	while(1) {
		PROCESS_WAIT_EVENT();
	}
	PROCESS_END();
}
/*---------------------------------------------------------------------------*/
