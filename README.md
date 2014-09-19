spark-weather
=============

A simple Spark Core firmware for driving an RGB led to display information about current weather conditions, along with an accompanying Node.js application to feed it data. 

The firmware assumes that your circuit matches the one in the included diagram, or at least that you utilize the same pins to drive the RGB values of the LED.

![Circuit diagram](https://github.com/shoja/spark-weather/blob/master/spark-weather.png?raw=true)


How-To
=============
1. Flash application.cpp to your core through your preferred means (spark.io/build, spark-cli, dfu-util, etc.)
2. Run `npm install` to install the Node app's dependencies
3. Run app.js with node. Note that you will need to provide the following env variables:
 * CORE_ID: The ID of your Core
 * CORE_ACCESS_TOKEN: An OAuth token for accessing the Spark Core Cloud
 * OWM_LOCATION_ID: The Open Weather Map ID for the location of which you would like to display the weather
 * OWM_API_KEY: An Open Weather Map API key