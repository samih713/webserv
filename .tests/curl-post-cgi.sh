curl -X POST http://localhost:8080/cgi-bin/test.py \
     -H "Content-Type: application/x-www-form-urlencoded" \
	 -d "data=hashim"
     #-d $'body\r\n'
#curl -X POST -d "data=HelloWorld" http://localhost:8080/cgi-bin/test.py