rm -f prof.txt
touch prof.txt

# Upto 20 x 1

drop="5"
while [ $drop -lt 100 ]
do
	dr=$(echo "scale=2; $drop/100" | bc)
	echo $dr

	cnt=$(grep -c "^0$dr" prof.txt)
	while [ $cnt -lt 10 ]
	do
		(./server $dr 0) &
		sleep 1 ; ./client 0
		wait

		cnt=$(grep -c "^0$dr" prof.txt)
	done
	drop=$[$drop+5]
done

drop="96"
while [ $drop -lt 100 ]
do
	dr=$(echo "scale=2; $drop/100" | bc)
	echo $dr

	cnt=$(grep -c "^0$dr" prof.txt)
	while [ $cnt -lt 5 ]
	do
		(./server $dr 0) &
		sleep 1 ; ./client 0
		wait

		cnt=$(grep -c "^0$dr" prof.txt)
	done
	drop=$[$drop+1]
done

gnuplot -p -e "plot 'prof.txt'" > /dev/null 2>&1