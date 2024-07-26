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

#ifndef COLLECT_SAMPLE_H
#define COLLECT_SAMPLE_H

#include <stdio.h>
#define COLLECT_INTERVAL 1
#define ENERGEST_CONF_ON 1

double get_total_energy(void);
void collect_data_energy(void);
void setEnergySample(char c);

typedef struct {
	unsigned long cpu, lpm, transmit, listen;
	unsigned long last_cpu, last_lpm, last_transmit, last_listen;
} energy_t;

#endif /* COLLECT_SAMPLE_H */

