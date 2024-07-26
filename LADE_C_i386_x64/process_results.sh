#!/bin/bash


myfile="without_attack"
distancemetr="Hellinger"
filesdir="/home/user/results/2024/${distancemetr,}" 
sheets_dir="/home/user/results/2024/sheets/${distancemetr,}" 


i=1; while [ $i -le 8 ]; do cat ${filesdir}/${myfile}_N4_T01_$i.txt | grep Sample | awk -v node="$i" -F ';' '{ print node";"$2";"$3";"$4";"$5";;;;;"$7";"$8";"$9";"$10 }' >> $sheets_dir/results_${myfile}_T01.csv; ((i+=1)); done
i=1; while [ $i -le 8 ]; do cat ${filesdir}/${myfile}_N6_T01_$i.txt | grep Sample | awk -v node="$i" -F ';' '{ print node";"$2";"$3";"$4";"$5";"$6";"$7";;;"$9";"$10";"$11";"$12";"$13";"$14 }' >> $sheets_dir/results_${myfile}_T01.csv; ((i+=1)); done
i=1; while [ $i -le 8 ]; do cat ${filesdir}/${myfile}_N8_T01_$i.txt | grep Sample | awk -v node="$i" -F ';' '{ print node";"$2";"$3";"$4";"$5";"$6";"$7";"$8";"$9";"$11";"$12";"$13";"$14";"$15";"$16";"$17";"$18 }' >> $sheets_dir/results_${myfile}_T01.csv; ((i+=1)); done

i=1; while [ $i -le 8 ]; do cat ${filesdir}/${myfile}_N4_T01_$i.txt | grep Dists | awk -F ';' '{ print $2";"$3";"$4";"$5";;;;;"$7";"$8";"$9";"$10 }' >> $sheets_dir/results_${myfile}_T01.csv; ((i+=1)); done
i=1; while [ $i -le 8 ]; do cat ${filesdir}/${myfile}_N6_T01_$i.txt | grep Dists | awk -F ';' '{ print $2";"$3";"$4";"$5";"$6";"$7";;;"$9";"$10";"$11";"$12";"$13";"$14 }' >> $sheets_dir/results_${myfile}_T01.csv; ((i+=1)); done
i=1; while [ $i -le 8 ]; do cat ${filesdir}/${myfile}_N8_T01_$i.txt | grep Dists | awk -F ';' '{ print $2";"$3";"$4";"$5";"$6";"$7";"$8";"$9";"$11";"$12";"$13";"$14";"$15";"$16";"$17";"$18 }' >> $sheets_dir/results_${myfile}_T01.csv; ((i+=1)); done

i=1; while [ $i -le 8 ]; do cat ${filesdir}/${myfile}_N4_T01_$i.txt | grep $distancemetr >> $sheets_dir/results_${myfile}_T01.csv; ((i+=1)); done
i=1; while [ $i -le 8 ]; do cat ${filesdir}/${myfile}_N6_T01_$i.txt | grep $distancemetr >> $sheets_dir/results_${myfile}_T01.csv; ((i+=1)); done
i=1; while [ $i -le 8 ]; do cat ${filesdir}/${myfile}_N8_T01_$i.txt | grep $distancemetr >> $sheets_dir/results_${myfile}_T01.csv; ((i+=1)); done



i=1; while [ $i -le 8 ]; do cat ${filesdir}/${myfile}_N4_T02_$i.txt | grep Sample | awk -v node="$i" -F ';' '{ print node";"$2";"$3";"$4";"$5";;;;;"$7";"$8";"$9";"$10 }' >> $sheets_dir/results_${myfile}_T02.csv; ((i+=1)); done
i=1; while [ $i -le 8 ]; do cat ${filesdir}/${myfile}_N6_T02_$i.txt | grep Sample | awk -v node="$i" -F ';' '{ print node";"$2";"$3";"$4";"$5";"$6";"$7";;;"$9";"$10";"$11";"$12";"$13";"$14 }' >> $sheets_dir/results_${myfile}_T02.csv; ((i+=1)); done
i=1; while [ $i -le 8 ]; do cat ${filesdir}/${myfile}_N8_T02_$i.txt | grep Sample | awk -v node="$i" -F ';' '{ print node";"$2";"$3";"$4";"$5";"$6";"$7";"$8";"$9";"$11";"$12";"$13";"$14";"$15";"$16";"$17";"$18 }' >> $sheets_dir/results_${myfile}_T02.csv; ((i+=1)); done

i=1; while [ $i -le 8 ]; do cat ${filesdir}/${myfile}_N4_T02_$i.txt | grep Dists | awk -F ';' '{ print $2";"$3";"$4";"$5";;;;;"$7";"$8";"$9";"$10 }' >> $sheets_dir/results_${myfile}_T02.csv; ((i+=1)); done
i=1; while [ $i -le 8 ]; do cat ${filesdir}/${myfile}_N6_T02_$i.txt | grep Dists | awk -F ';' '{ print $2";"$3";"$4";"$5";"$6";"$7";;;"$9";"$10";"$11";"$12";"$13";"$14 }' >> $sheets_dir/results_${myfile}_T02.csv; ((i+=1)); done
i=1; while [ $i -le 8 ]; do cat ${filesdir}/${myfile}_N8_T02_$i.txt | grep Dists | awk -F ';' '{ print $2";"$3";"$4";"$5";"$6";"$7";"$8";"$9";"$11";"$12";"$13";"$14";"$15";"$16";"$17";"$18 }' >> $sheets_dir/results_${myfile}_T02.csv; ((i+=1)); done

i=1; while [ $i -le 8 ]; do cat ${filesdir}/${myfile}_N4_T02_$i.txt | grep $distancemetr >> $sheets_dir/results_${myfile}_T02.csv; ((i+=1)); done
i=1; while [ $i -le 8 ]; do cat ${filesdir}/${myfile}_N6_T02_$i.txt | grep $distancemetr >> $sheets_dir/results_${myfile}_T02.csv; ((i+=1)); done
i=1; while [ $i -le 8 ]; do cat ${filesdir}/${myfile}_N8_T02_$i.txt | grep $distancemetr >> $sheets_dir/results_${myfile}_T02.csv; ((i+=1)); done


i=1; while [ $i -le 8 ]; do cat ${filesdir}/${myfile}_N4_T03_$i.txt | grep Sample | awk -v node="$i" -F ';' '{ print node";"$2";"$3";"$4";"$5";;;;;"$7";"$8";"$9";"$10 }' >> $sheets_dir/results_${myfile}_T03.csv; ((i+=1)); done
i=1; while [ $i -le 8 ]; do cat ${filesdir}/${myfile}_N6_T03_$i.txt | grep Sample | awk -v node="$i" -F ';' '{ print node";"$2";"$3";"$4";"$5";"$6";"$7";;;"$9";"$10";"$11";"$12";"$13";"$14 }' >> $sheets_dir/results_${myfile}_T03.csv; ((i+=1)); done
i=1; while [ $i -le 8 ]; do cat ${filesdir}/${myfile}_N8_T03_$i.txt | grep Sample | awk -v node="$i" -F ';' '{ print node";"$2";"$3";"$4";"$5";"$6";"$7";"$8";"$9";"$11";"$12";"$13";"$14";"$15";"$16";"$17";"$18 }' >> $sheets_dir/results_${myfile}_T03.csv; ((i+=1)); done

i=1; while [ $i -le 8 ]; do cat ${filesdir}/${myfile}_N4_T03_$i.txt | grep Dists | awk -F ';' '{ print $2";"$3";"$4";"$5";;;;;"$7";"$8";"$9";"$10 }' >> $sheets_dir/results_${myfile}_T03.csv; ((i+=1)); done
i=1; while [ $i -le 8 ]; do cat ${filesdir}/${myfile}_N6_T03_$i.txt | grep Dists | awk -F ';' '{ print $2";"$3";"$4";"$5";"$6";"$7";;;"$9";"$10";"$11";"$12";"$13";"$14 }' >> $sheets_dir/results_${myfile}_T03.csv; ((i+=1)); done
i=1; while [ $i -le 8 ]; do cat ${filesdir}/${myfile}_N8_T03_$i.txt | grep Dists | awk -F ';' '{ print $2";"$3";"$4";"$5";"$6";"$7";"$8";"$9";"$11";"$12";"$13";"$14";"$15";"$16";"$17";"$18 }' >> $sheets_dir/results_${myfile}_T03.csv; ((i+=1)); done

i=1; while [ $i -le 8 ]; do cat ${filesdir}/${myfile}_N4_T03_$i.txt | grep $distancemetr >> $sheets_dir/results_${myfile}_T03.csv; ((i+=1)); done
i=1; while [ $i -le 8 ]; do cat ${filesdir}/${myfile}_N6_T03_$i.txt | grep $distancemetr >> $sheets_dir/results_${myfile}_T03.csv; ((i+=1)); done
i=1; while [ $i -le 8 ]; do cat ${filesdir}/${myfile}_N8_T03_$i.txt | grep $distancemetr >> $sheets_dir/results_${myfile}_T03.csv; ((i+=1)); done