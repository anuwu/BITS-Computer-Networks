drop="5"
while [ $drop -le 100 ]
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