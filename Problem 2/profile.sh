rm -f prof.txt
touch prof.txt

# Upto 95

drop="5"
while [ $drop -lt 100 ]
do
	dr=$(echo "scale=2; $drop/100" | bc)
	echo $dr
	for i in {1..10}
	do
		(./server $dr 0) &
		sleep 1 ; (./relay $dr 0) &
		sleep 1 ; ./client 0 ;
		wait
	done
	drop=$[$drop+5]
done

# From 96 to 99

drop="96"
while [ $drop -lt 100 ]
do
	dr=$(echo "scale=2; $drop/100" | bc)
	echo $dr
	for i in {1..5}
	do
		(./server $dr 0) &
		sleep 1 ; (./relay $dr 0) &
		sleep 1 ; ./client 0 ;
		wait
	done
	drop=$[$drop+1]
done

gnuplot -p -e "plot 'prof.txt'" > /dev/null 2>&1