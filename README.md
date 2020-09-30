# ArduinoForElsass

This project is made for people with CP (cerebral paralysis) the goal is to make a keyboard easier to use.
It aims to be 100% configurable with a single simple webserver and the local storage on the ESP32.

Known bugs
1# When the server starts up you're sometimes meed with a net::ERR CONTENT LENGTH MISMATCH
the only known solution is to restart the ESP32 or wait for a while and reload the page. 
