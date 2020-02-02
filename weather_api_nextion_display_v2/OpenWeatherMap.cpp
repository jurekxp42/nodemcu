#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include "OpenWeatherMap.h"

//------------------------------------------ Universal weather request parcer for opewWeatherMap site ---------
void OWMrequest::doUpdate(String url, byte maxForecasts) {
	JsonStreamingParser parser;
	parser.setListener(this);

	WiFiClient client;
  HTTPClient http;

	http.begin(client,url);
	bool isBody = false;
	char c;
	int httpCode = http.GET();
	if(httpCode > 0) {
		WiFiClient * client = http.getStreamPtr();
		while(client->connected()) {
			while(true) {
				if (client->available() > 0) {
					c = client->read();
					if (c == '{' || c == '[') {
						isBody = true;
					}
					if (isBody) {
						parser.parse(c);
					}
				} else {
					uint32_t now_ms = millis();
					while (millis() - now_ms < 200) {
						yield();
						if (client->available() > 0)
							break;
					}
					if (client->available() <= 0)
						break;
				}
			}
		}
	}
}

void OWMrequest::init(void) {
  for (byte i = 0; i < OWM_max_layers; ++i)
    p_key[i] = "";
  currentParent = currentKey = "";
}

void OWMrequest::startObject() {
  for (byte i = OWM_max_layers-1; i > 0; --i) {
    p_key[i] = p_key[i-1];
  }
  p_key[0] = currentParent;
  currentParent = currentKey;
}

void OWMrequest::endObject() {
  currentParent = p_key[0];
  for (byte i = 0; i < OWM_max_layers-1; ++i) {
    p_key[i] = p_key[i+1];
  }
  currentKey = "";
}
/*------------------------------------------ Current weather conditions from openweatrhermap.org --------------
/* Example:
/* {"coord":{"lon":-0.13,"lat":51.51},
  "weather":[{"id":300,"main":"Drizzle","description":"light intensity drizzle","icon":"09d"}],
  "base":"stations",
  "main":{"temp":280.32,"pressure":1012,"humidity":81,"temp_min":279.15,"temp_max":281.15},
  "visibility":10000,
  "wind":{"speed":4.1,"deg":80},
  "clouds":{"all":90},
  "dt":1485789600,
  "sys":{"type":1,"id":5091,"message":0.0103,"country":"GB","sunrise":1485762037,"sunset":1485794875},
  "id":2643743,"name":"London","cod":200}
*/
void OWMconditions::updateConditions(OWM_conditions *conditions, String apiKey, String country, String city, String units, String language) {
  this->conditions = conditions;
  OWMrequest::init();

  String url = "http://api.openweathermap.org/data/2.5/weather?id=" + city+"&lang=de&units=metric";
  url +=  + "&appid=" + apiKey;
  Serial.println( url );
  doUpdate(url);
  this->conditions = nullptr;
}

void OWMconditions::value(String value) {
  if (currentParent == "coord") {
    if (currentKey == "lon") {
      conditions->longtitude = value;
    } else if (currentKey == "lat") {
      conditions->latitude = value;
    }
  } else if (currentParent == "weather") {
    if (currentKey == "id") {
      conditions->id = value;
    } else if (currentKey == "main") {
      conditions->main = value;
    } else if (currentKey == "description") {
      conditions->description = value;
    } else if (currentKey == "icon") {
      conditions->icon = value;
    }
  } else if (currentParent == "main") {
    if (currentKey == "temp") {
      conditions->temp = value;
    } else if (currentKey == "feels_like") {
      conditions->feels_like = value;
    } else if (currentKey == "wind_speed") {
      conditions->wind_speed = value;
    } else if (currentKey == "pressure") {
      conditions->pressure = value;
    } else if (currentKey == "humidity") {
      conditions->humidity = value;
    } else if (currentKey == "temp_min") {
      conditions->t_min = value;
    } else if (currentKey == "temp_max") {
      conditions->t_max = value;
    }
  } else if (currentParent == "wind") {
    if (currentKey == "speed") {
      conditions->w_speed = value;
    } else if (currentKey == "deg") {
      conditions->w_deg = value;
    }
  } else if (currentParent == "clouds") {
    if (currentKey == "all") {
      conditions->cond = currentParent;
      conditions->cond_value = value;
    }
  } else if (currentParent == "rain") {
    if (currentKey == "3h") {
      conditions->cond = currentParent;
      conditions->cond_value = value;
    }  
  } else if (currentParent == "snow") {
    if (currentKey == "3h") {
      conditions->cond = currentParent;
      conditions->cond_value = value;
    } 
  } else if (currentParent == "sys") {
    if (currentKey == "sunrise") {
      conditions->sunrise = value;
    } else if (currentKey == "sunset") {
      conditions->sunset = value;
    }
  } else {
    if (currentKey == "visibility") {
      conditions->visibility = value;
    } else if (currentKey == "dt") {
      conditions->dt = value;
    } 
  }
}
//------------------------------------------ Five day forecast from openweatrhermap.org -----------------------
byte OWMfiveForecast::updateForecast(OWM_fiveForecast *forecasts, byte maxForecasts, String apiKey, String country, String city, String units, String language) {
  this->forecasts = forecasts;
  this->max_forecasts = maxForecasts;
  OWMrequest::init();
  index = 0;
  timestamp = 0;

  String url = "http://api.openweathermap.org/data/2.5/forecast?id=" + city+"&lang=de&units=metric" ;
  url +=  + "&appid=" + apiKey;
  doUpdate(url, maxForecasts);
  Serial.println( url );
  this->forecasts = nullptr;
  return index;
}

void OWMfiveForecast::value(String value) {
  if (currentKey == "dt") {
    if (timestamp == 0) {
      index = 0;
      forecasts[index].dt = value;
      timestamp = value.toInt();
    } else {
      uint32_t t = value.toInt();
      if (t > timestamp) {                          // new forecast time
	      if (index < max_forecasts - 1) {
    	    ++index;
          timestamp = t;
    	    forecasts[index].dt = value;
    	  }
      }
    }
  } else
  if (currentKey == "temp") {
    forecasts[index].temp = value;
  } else
  if (currentKey == "feels_like") {
    forecasts[index].feels_like = value;
  }else
  if (currentKey == "temp_min") {
    forecasts[index].t_min = value;
  } else
  if (currentKey == "temp_max") {
    forecasts[index].t_max = value;
  } else
  if (currentKey == "pressure") {
    forecasts[index].pressure = value;
  } else
  if (currentKey == "sea_level") {
    forecasts[index].sea_pressure = value;
  } else
  if (currentKey == "humidity") {
    forecasts[index].humidity = value;
  } else
  if (currentKey == "wind_speed") {
    forecasts[index].wind_speed = value;
  } else
  if (currentParent == "weather" && currentKey == "description") {
    forecasts[index].description = value;
  } else
  if (currentParent == "weather" && currentKey == "icon") {
    forecasts[index].icon = value;
  } else
  if (currentParent == "weather" && currentKey == "id") {
    forecasts[index].id = value;
  } else
  if (currentParent == "wind" && currentKey == "speed") {
    forecasts[index].w_speed = value;
  } else
  if (currentParent == "wind" && currentKey == "deg") {
    forecasts[index].w_deg = value;
  } else
  if (currentParent == "clouds" && currentKey == "all") {
    forecasts[index].cond = currentParent;
    forecasts[index].cond_value = value;
  } else
  if (currentParent == "rain" && currentKey == "3h") {
    forecasts[index].cond = currentParent;
    forecasts[index].cond_value = value;
  }  else
  if (currentParent == "snow" && currentKey == "3h") {
    forecasts[index].cond = currentParent;
    forecasts[index].cond_value = value;
  }
}
