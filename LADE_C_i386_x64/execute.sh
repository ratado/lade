#!/bin/bash

N=$1
s=$2
dest="/home/user/results/2024" 

distmetr="sibson"
make all
./lade -e n -d s -t 0.1 -l 2.0 > $dest/$distmetr/without_attack_N${N}_T01_${s}.txt
./lade -e j -d s -t 0.1 -l 2.0 > $dest/$distmetr/jamming_attack_N${N}_T01_${s}.txt
./lade -e m -d s -t 0.1 -l 2.0 > $dest/$distmetr/malware_attack_N${N}_T01_${s}.txt
./lade -e n -d s -t 0.2 -l 2.0 > $dest/$distmetr/without_attack_N${N}_T02_${s}.txt
./lade -e j -d s -t 0.2 -l 2.0 > $dest/$distmetr/jamming_attack_N${N}_T02_${s}.txt
./lade -e m -d s -t 0.2 -l 2.0 > $dest/$distmetr/malware_attack_N${N}_T02_${s}.txt
./lade -e n -d s -t 0.35 -l 2.0 > $dest/$distmetr/without_attack_N${N}_T03_${s}.txt
./lade -e j -d s -t 0.35 -l 2.0 > $dest/$distmetr/jamming_attack_N${N}_T03_${s}.txt
./lade -e m -d s -t 0.35 -l 2.0 > $dest/$distmetr/malware_attack_N${N}_T03_${s}.txt


distmetr="euclidian"
./lade -e n -d e -t 0.1 -l 2.0 > $dest/$distmetr/without_attack_N${N}_T01_${s}.txt
./lade -e j -d e -t 0.1 -l 2.0 > $dest/$distmetr/jamming_attack_N${N}_T01_${s}.txt
./lade -e m -d e -t 0.1 -l 2.0 > $dest/$distmetr/malware_attack_N${N}_T01_${s}.txt
./lade -e n -d e -t 0.2 -l 2.0 > $dest/$distmetr/without_attack_N${N}_T02_${s}.txt
./lade -e j -d e -t 0.2 -l 2.0 > $dest/$distmetr/jamming_attack_N${N}_T02_${s}.txt
./lade -e m -d e -t 0.2 -l 2.0 > $dest/$distmetr/malware_attack_N${N}_T02_${s}.txt
./lade -e n -d e -t 0.35 -l 2.0 > $dest/$distmetr/without_attack_N${N}_T03_${s}.txt
./lade -e j -d e -t 0.35 -l 2.0 > $dest/$distmetr/jamming_attack_N${N}_T03_${s}.txt
./lade -e m -d e -t 0.35 -l 2.0 > $dest/$distmetr/malware_attack_N${N}_T03_${s}.txt


distmetr="hellinger"
./lade -e n -d h -t 0.1 -l 2.0 > $dest/$distmetr/without_attack_N${N}_T01_${s}.txt
./lade -e j -d h -t 0.1 -l 2.0 > $dest/$distmetr/jamming_attack_N${N}_T01_${s}.txt
./lade -e m -d h -t 0.1 -l 2.0 > $dest/$distmetr/malware_attack_N${N}_T01_${s}.txt
./lade -e n -d h -t 0.2 -l 2.0 > $dest/$distmetr/without_attack_N${N}_T02_${s}.txt
./lade -e j -d h -t 0.2 -l 2.0 > $dest/$distmetr/jamming_attack_N${N}_T02_${s}.txt
./lade -e m -d h -t 0.2 -l 2.0 > $dest/$distmetr/malware_attack_N${N}_T02_${s}.txt
./lade -e n -d h -t 0.35 -l 2.0 > $dest/$distmetr/without_attack_N${N}_T03_${s}.txt
./lade -e j -d h -t 0.35 -l 2.0 > $dest/$distmetr/jamming_attack_N${N}_T03_${s}.txt
./lade -e m -d h -t 0.35 -l 2.0 > $dest/$distmetr/malware_attack_N${N}_T03_${s}.txt

