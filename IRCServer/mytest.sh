#!/bin/bash 
export PORT=1234
echo "ADD-USER avi nash\r\n" | telnet localhost $PORT

echo "CREATE-ROOM avi nash room1\r\n" | telnet localhost $PORT

echo "ENTER-ROOM avi nash room1\r\n" | telnet localhost $PORT

echo "SEND-MESSAGE avi nash room1 hello\r\n" | telnet localhost $PORT

echo "GET-MESSAGE avi nash 0 room1\r\n" | telnet localhost $PORT