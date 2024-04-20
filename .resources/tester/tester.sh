#!/bin/bash

RESPONSE="response.log"
INDEX="../sample_pages/index.html"
COMPARE="compare"
ORIGINAL_DIFF="original_diff"
ITER=700
total_time=0
requests_served=0

# Function to print headers
print_headers () {
	cat $ORIGINAL_DIFF $INDEX > $COMPARE
}

print_headers

for ((i = 0; i < ITER; i++)); do
	# start timer
	start_time=$(date +%s.%N)
	
	# Curl request
	curl --silent -i -X GET http://localhost:8080 -H \
		"Content-Type: text/html" > $RESPONSE

	# End time
	end_time=$(date +%s.%N)

	# Calculate the duration for this iteration
	duration=$(echo "$end_time - $start_time" | bc)
    total_time=$(echo "$total_time + $duration" | bc)

	# Comparison of files
	DIFF=$(diff -q $RESPONSE $COMPARE)
	if [ "$DIFF" != "" ]; then
		echo -e "\033[0;31mFalse\033[0m"
	else
		requests_served=$(echo "1 + $requests_served" | bc) 
	fi
	rm $RESPONSE
done

echo $requests_served
echo $total_time

# Average time
average_time=$(echo "($total_time * 1000) / $ITER" | bc)

# output average time
printf "Average time per request: %.5f ms\n" $average_time

