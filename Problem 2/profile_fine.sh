rm -f prof_fine.txt
touch prof_fine.txt

drop="1"
while [ $drop -lt 100 ]
do
	dr=$(echo "scale=2; $drop/100" | bc)
	echo $dr
	for i in {1..4}
	do
		(./server $dr 0) &
		sleep 1 ; (./relay $dr 0) &
		sleep 1 ; ./client 0 ;
		wait
	done
	drop=$[$drop+1]
done

gnuplot -p -e "plot 'prof_fine.txt'" > /dev/null 2>&1