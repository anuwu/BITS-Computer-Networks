rm -f prof_fine.txt
touch prof_fine.txt

drop="1"
while [ $drop -lt 100 ]
do
	dr=$(echo "scale=2; $drop/100" | bc)
	echo $dr
	for i in {1..2}
	do
		(./server $dr 0) &
		sleep 1 ; (./relay $dr 0) &
		sleep 1 ; ./client 0 ;
		wait
	done
	drop=$[$drop+1]
done

### Averaging

rm -f prof_fine_avg.txt

drp="1"
while [ $drp -lt 100 ]
do
        dr=$(echo "scale=2; $drp/100"|bc)
        cnt=$(grep -c "^0$dr" prof_fine.txt)
        s=$(grep "^0$dr" prof_fine.txt | grep -o " [0-9]*.[0-9]*" | tr -d \  | awk '{s+=$1} END {print s}')
        (printf "0$dr " && echo "scale=2;$s/$cnt" | bc) >> prof_avg.txt

        drp=$[$drp+1]
done

gnuplot -p -e "set xlabel 'Dropping probability'; set ylabel 'Time taken'; set title 'File size : 1580 bytes' ; set key off ;
plot 'prof_fine.txt', 'prof_fine_avg.txt' smooth csplines" > /dev/null 2>&1