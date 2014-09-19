console.log(process.env);

var request = require('request');

var core = {
  accessToken: process.env.CORE_ACCESS_TOKEN,
  id: process.env.CORE_ID
};

var setTemperature = function(temp) {
  request.post('https://api.spark.io/v1/devices/' + process.env.CORE_ID + '/setTemp', 
    {form: {access_token: process.env.CORE_ACCESS_TOKEN, args: temp}}, function(err, res) {
      if(err) console.log(err);
    });
};

var setWeather = function(weatherCodes) {
  request.post('https://api.spark.io/v1/devices/' + process.env.CORE_ID + '/setWeather', 
    {form: {access_token: process.env.CORE_ACCESS_TOKEN, args: weatherCodes.toString()}}, function(err, res) {
      if(err) console.log(err);
    });
};

var updateCore = function() {
  request('http://api.openweathermap.org/data/2.5/weather?id=' + process.env.OWM_LOCATION_ID + '&APPID=' + process.env.OWM_API_KEY + '&units=metric', 
    function(err, res, body) {
      if(err) {
        return;
      } 
      body = JSON.parse(body);
      var weatherCodes = [];
      
      for(var i = 0; i < body.weather.length; i++) {
        weatherCodes.push(body.weather[i].id);
      }

      setWeather(weatherCodes);
      setTemperature(body.main.temp);
    });
};

var interval = setInterval(updateCore, 30000);
updateCore();