#include "application.h"
#include <math.h>
#include <string.h>

#define ALERT_FLASHES 3
#define UPDATE_INTERVAL 60000

SYSTEM_MODE(AUTOMATIC);

int RED = A4;
int GREEN = A5;
int BLUE = A6;

int red = 0;
int green = 127;
int blue = 255;

typedef struct {
    int r;
    int g;
    int b;
} Color;

typedef struct {
    Color rgb;
    int intensity;
} Weather;

Color temperature;

String weatherCodes;

int random(int min, int max);
void fadeOut(void);
void fadeIn(void);
void fadeOutIn(int num_fades);
void fadeInOut(int num_fades);
void alertWeather(Weather condition);
void displayWeather(void);
void updateDisplay(void);

Color rgbFromTemp(double temp);
double hueToRGB(double p, double q, double t);
Color hslToRGB(double h, double s, double l);

int setTemperature(String TEMP);
int setWeather(String WEATHER_CODES);

int random(int min, int max) {
    return rand() % (min - max + 1) + min;
}

void fadeOut(void) {
    int inc = -5, brightness = 255;
    
    while(brightness >= 0) {
        brightness = constrain(brightness, 0, 255);
        analogWrite(RED, int(red * (brightness / 255.0)));
        analogWrite(GREEN, int(green * (brightness / 255.0)));
        analogWrite(BLUE, int(blue * (brightness / 255.0)));
        
        brightness += inc;
        delay(20);
    }
    
    return;
}

void fadeIn(void) {
    int inc = 5, brightness = 0;
    
    while(brightness <= 255) {
        brightness = constrain(brightness, 0, 255);
        analogWrite(RED, int(red * (brightness / 255.0)));
        analogWrite(GREEN, int(green * (brightness / 255.0)));
        analogWrite(BLUE, int(blue * (brightness / 255.0)));
        
        brightness += inc;
        delay(20);
    }
    
    return;
}

void fadeOutIn(int num_fades) {
    int i;
    
    for(i = 0; i < num_fades; i++) {
        fadeOut();
        fadeIn();
    }
    
    return;
}

void fadeInOut(int num_fades) {
    int i;
    
    for(i = 0; i < num_fades; i++) {
        fadeIn();
        fadeOut();
    }
    
    return;
}

void alertWeather(Weather condition) {
    red = condition.rgb.r;
    green = condition.rgb.g;
    blue = condition.rgb.b;
    
    fadeInOut(ALERT_FLASHES);
    
    red = temperature.r;
    green = temperature.g;
    blue = temperature.b;
    
    return;
}

void displayWeather() {
    Weather cond;
    
    Serial.print("\tdisplayWeather ");
    Serial.println(weatherCodes);
    
    char * codes = new char[weatherCodes.length() + 1];
    strcpy(codes, weatherCodes.c_str());
    char * code = strtok(codes, ",");
        
    while(code != NULL) {
        int codeInt = atoi(code);
        Serial.print("\t\t");
        Serial.println(code);
        switch(codeInt / 100) {
            case 2:
                // thunderstorm
                cond.rgb.r = 255;
                cond.rgb.g = 0;
                cond.rgb.g = 0;
                alertWeather(cond);
                break;
            case 3:
                // drizzle
                cond.rgb.r = 0;
                cond.rgb.g = 255;
                cond.rgb.b = 0;
                alertWeather(cond);
                break;
            case 5:
                // rain
                cond.rgb.r = 0;
                cond.rgb.g = 0;
                cond.rgb.b = 255;
                alertWeather(cond);
                break;
            case 6:
                // snow
                cond.rgb.r = 255;
                cond.rgb.g = 255;
                cond.rgb.b = 0;
                alertWeather(cond);
                break;
            case 7:
                //atmosphere
                cond.rgb.r = 255;
                cond.rgb.g = 0;
                cond.rgb.b = 255;
                alertWeather(cond);
                break;
            case 8:
                // clouds
                cond.rgb.r = 0;
                cond.rgb.g = 255;
                cond.rgb.b = 255;
                alertWeather(cond);
                break;
            case 9:
                // extreme and additional
                cond.rgb.r = 255;
                cond.rgb.g = 255;
                cond.rgb.b = 255;
                alertWeather(cond);
                break;
        }
        code = strtok(NULL, ",");
    }
    
    fadeIn();
    
    return;
}

int setTemperature(String TEMP) {
    Serial.print("setTemp ");
    Serial.println(TEMP);
    
    double temp = atof(TEMP.c_str());
    Color rgb = rgbFromTemp(temp);
    
    temperature.r = rgb.r;
    temperature.g = rgb.g;
    temperature.b = rgb.b;
    
    return 1;
}

int setWeather(String WEATHER_CODES) {
    Serial.print("setWeather ");
    Serial.println(WEATHER_CODES);
    
    weatherCodes = WEATHER_CODES;
    
    return 1;
}

void updateDisplay() {
    Serial.println("updateDisplay");
    fadeOut();
    
    red = temperature.r;
    green = temperature.g;
    blue = temperature.b;
    
    displayWeather();
}


double hueToRGB(double p, double q, double t) {
    if(t < 0) {
      t += 1;  
    } 
    
    if(t > 1) {
        t -= 1;
    }
    
    if(t < 1/6.0) {
        return p + (q - p) * 6.0 * t;
    }
    
    if(t < 1/2.0) {
        return q;
    }
    
    if(t < 2/3.0) {
        return p + (q - p) * (2/3.0 - t) * 6.0;
    }
    
    return p;
}

Color hslToRGB(double h, double s, double l) {
    Color rgb;
    
    if(s == 0) {
        rgb.r = rgb.g =rgb.b = l; // achromatic
    }
    else {
        
        double q = l < 0.5 ? l * (1.0 + s) : l + s - l * s;
        double p = 2 * l - q;
        
        rgb.r = round(255 * hueToRGB(p, q, h + 1/3.0));
        rgb.g = round(255 * hueToRGB(p, q, h));
        rgb.b = round(255 * hueToRGB(p, q, h - 1/3.0));
    }
    
    return rgb;
}

Color rgbFromTemp(double temp) {
    double hue = (30 + 240 * (30 - temp) / 60.0) / 360.0;
    double saturation = 1;
    double lightness = 0.25;
    
    return hslToRGB(hue, saturation, lightness);
}

void setup() {
    Spark.function("setTemp", setTemperature);
    Spark.function("setWeather", setWeather);
    
    pinMode(RED, OUTPUT);
    pinMode(GREEN, OUTPUT);
    pinMode(BLUE, OUTPUT);
    
    Serial.begin(9600);
    
    temperature.r = 0;
    temperature.g = 127;
    temperature.b = 255;
}

void loop() {
    updateDisplay();
    delay(UPDATE_INTERVAL);
}