# for ((i = 0; i < 10; i++)); do
# 	sh -c 'curl -X GET http://localhost:8080/cgi-bin/file.sh \
#      -H "Host: Linode.com" \
#      -H "User-Agent: Mozilla/5.0 (Windows; U; Windows NT 6.1; en-US; rv:1.9.1.8) Gecko/20091102 Firefox/3.5.5" \
#      -H "Accept-Encoding: gzip,deflate" \
#      -H "Accept-Charset: ISO-8859-1,utf-8" \
#      -H "Cache-Control: no-cache" \
#      -d $'body\r\n' &'
# done

for ((i = 0; i < 20; i++)); do
    curl -X GET http://localhost:8001/cgi-bin/file.sh \
     -H "Host: Linode.com" \
     -H "User-Agent: Mozilla/5.0 (Windows; U; Windows NT 6.1; en-US; rv:1.9.1.8) Gecko/20091102 Firefox/3.5.5" \
     -H "Accept-Encoding: gzip,deflate" \
     -H "Accept-Charset: ISO-8859-1,utf-8" \
     -H "Cache-Control: no-cache" \
     -d $'body\r\n' &

	 sleep 1
done

# Wait for all background jobs to complete
wait
