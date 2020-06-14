# Upto 20 x 1

drop="5"
while [ $drop -lt 20 ]
do
	dr=$(echo "scale=2; $drop/100" | bc)
	echo $dr
	for i in {1..10}
	do
		(./server $dr 0) &
		sleep 1 ; ./client 0
		wait
	done
	drop=$[$drop+5]
done

# Upto 20 x 2

drop="5"
while [ $drop -lt 20 ]
do
	dr=$(echo "scale=2; $drop/100" | bc)
	echo $dr
	for i in {1..10}
	do
		(./server $dr 0) &
		sleep 1 ; ./client 0
		wait
	done
	drop=$[$drop+5]
done

# Upto 20 x 3

drop="5"
while [ $drop -lt 20 ]
do
	dr=$(echo "scale=2; $drop/100" | bc)
	echo $dr
	for i in {1..10}
	do
		(./server $dr 0) &
		sleep 1 ; ./client 0
		wait
	done
	drop=$[$drop+5]
done

# Upto 95

drop="25"
while [ $drop -lt 100 ]
do
	dr=$(echo "scale=2; $drop/100" | bc)
	echo $dr
	for i in {1..10}
	do
		(./server $dr 0) &
		sleep 1 ; ./client 0
		wait
	done
	drop=$[$drop+5]
done

# Upto 96 to 99

drop="96"
while [ $drop -lt 100 ]
do
	dr=$(echo "scale=2; $drop/100" | bc)
	echo $dr
	for i in {1..5}
	do
		(./server $dr 0) &
		sleep 1 ; ./client 0
		wait
	done
	drop=$[$drop+1]
done