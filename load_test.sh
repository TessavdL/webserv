# #!/bin/bash

# Set the number of requests to send
REQUESTS=1000

# Set the base URL of the web server
BASE_URL=http://localhost/cgi-bin/upload.php
# Send the requests
for i in $(seq 1 $REQUESTS)
do
  # Generate a random string
  # tr may complain about getting an Illegal byte sequence, this is not our webservs fault
  # the reason is that tr does not recognice the /dev/urandom outbut as utf-8.
  # on linux tr should work on all binary but on mac it does not or something like that. not my issue.
  RANDOM_STRING=$(cat /dev/urandom | tr -dc 'a-zA-Z0-9' | fold -w 32 | head -n 1)

  # Append the random string to the base URL
  URL="$BASE_URL/$RANDOM_STRING"

  # Send the request
  curl $URL &
done

# Wait for all the curl processes to finish
wait