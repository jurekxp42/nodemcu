#ifndef OpenWeather_h
#define OpenWeather_h

#include <JsonListener.h>
#include <JsonStreamingParser.h>

//------------------------------------------ Universal weather request parcer for opewWeatherMap site ---------
#define OWM_max_layers 4
class OWMrequest: public JsonListener {
  public:
    void init(void);
    OWMrequest() : JsonListener()                   { }
    virtual void key(String key)                    { currentKey = String(key); }
    virtual void endObject();
    virtual void startObject();
    virtual void whitespace(char c)                 { }
    virtual void startDocument()                    { }
    virtual void endArray()                         { }
    virtual void endDocument()                      { }
    virtual void startArray()                       { }
  protected:
    void   doUpdate(String url, uint8_t maxForecasts = 0);
    String currentKey;
    String currentParent;
    String p_key[OWM_max_layers];
};



//------------------------------------------ Five day forecast from openweatrhermap.org -----------------------
typedef struct sOWM_fiveForecast {
  String dt;
  String temp;
  String t_min;
  String t_max;
  String pressure;
  String sea_pressure;
  String humidity;
  String id;
  String main;
  String description;
  String icon;
  String w_speed;
  String w_deg;
  String cond;                                      // conditions: cloud, rain, snow
  String cond_value;
  String feels_like;
  String wind_speed;
} OWM_fiveForecast;

class OWMfiveForecast : public OWMrequest {
  public:
    OWMfiveForecast()                               { }
    uint8_t updateForecast(OWM_fiveForecast *forecasts, uint8_t maxForecasts, String apiKey,
              String country, String city, String units = "", String language = "");
    virtual void value(String value);

  private:
    uint8_t     index;
    uint32_t 	timestamp;
    uint8_t     max_forecasts;
    OWM_fiveForecast *forecasts;
};




#endif
