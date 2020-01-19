# Dweet_HTTPS

Here is a Project to push Data of a DS18B20 Temperature-Sensor, to Dweet.io via HTTPSRequest.
As its the Future to use HTTPS to do anything, here is a Solution (maybe they will cancel HTTP-Request in the near Future).


To send any Data to Dweet, you have to choose a "Thing"-Name for you.
If you have a suitable Name, you can push your stuff like:
// https://dweet.io/dweet/for/DWEETHTTPS?Entry=Value

"Thing" is your desired "ChannelName" --- like eg. DWEETHTTPS
----- https://dweet.io/dweet/for/DWEETHTTPS

then you want to push a Sensors Value to Dweet, where: "DS18B20" is the Name and "Value"
is the temperature in Celsius
----- ?DS18B20=13,37

now you drop a Request:
https://dweet.io/dweet/for/DWEETHTTPS?DS18B20=13,37
Thats it !

To get the latest reading, you only have to go to:
https://dweet.io/get/latest/dweet/for/DWEETHTTPS

