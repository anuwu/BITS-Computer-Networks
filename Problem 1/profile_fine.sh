rm -f prof_fine.txt
touch prof_fine.txt

drop="1"
while [ $drop -lt 100 ]
do
	dr=$(echo "scale=2; $drop/100" | bc)
	echo $dr

	cnt=$(grep -c "^0$dr" prof.txt)
	while [ $cnt -lt 4 ]
	do
		(./server $dr 0) &
		sleep 1 ; ./client 0
		wait

		cnt=$(grep -c "^0$dr" prof.txt)
	done
	drop=$[$drop+1]
done

gnuplot -p -e "plot 'prof_fine.txt'" > /dev/null 2>&1