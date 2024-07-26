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

#ifndef MATH_OPERATIONS_H
#define MATH_OPERATIONS_H

#include <stdio.h>
#include <stdlib.h>

#define N 8
#define LAMBDA 4

unsigned short addSample(float x);
void printSample();
unsigned short addSampleA(float x);
void printSampleA();
unsigned short addSampleB(float x);
void printSampleB();
float sibsonDistance();
float jeffreyDistance();
float hellingerDistance();
float euclidianDistance();
void setResetSample();
void setResetSampleA();
void setResetSampleB();

#endif /* MATH_OPERATIONS_H */
