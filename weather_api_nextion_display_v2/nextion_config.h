
NexText Nexttemp(0, 1, "main_temp");
NexText Nextfeels(0, 2, "feels_like");
NexText Nextclouds(0, 3, "clouds");
NexText Nextpressure(0, 4, "pressure");
NexText Nexthumidity(0, 5, "humidity");
NexText Nextwind(0, 6, "wind_speed");
NexText Nextzeitaktuell(0, 8, "zeitaktuell");
NexText NexttempInnen(0, 9, "tempInnen");
NexText NexthumidityInnen(0, 10, "humidityInnen");

NexText Nextv1Temp(1, 1, "v1Temp");
NexText NextvdateMorgen(1, 2, "dateMorgen");
NexText Nextv2Temp(1, 4, "v2Temp");
NexText NextvdateUmorgen(1, 3, "dateUmorgen");
NexText Nextv1Clouds( 1 , 6 , "v1Clouds" );
NexText Nextv2Clouds( 1 , 7 , "v2Clouds" );
NexText NextvdateUUmorgen(1, 8, "dateUUmorgen");
NexText Nextv3Temp( 1 , 9 , "v3Temp" );
NexText Nextv3Clouds( 1 , 10 , "v3Clouds" );

void b00PopCallback(void *ptr);
NexButton b00(0, 7, "b0");
void b01PopCallback(void *ptr);
NexButton b01(0, 9, "b1");
void b10PopCallback(void *ptr);
NexButton b10(1, 5, "b0");
void b20PopCallback(void *ptr);
NexButton b20(2, 1, "b0");
void b21PopCallback(void *ptr);
NexButton b21(2, 2, "b1");
void b22PopCallback(void *ptr);
NexButton b22(2, 3, "b2");

NexPage page0    = NexPage(0, 0, "page0");
NexPage page1    = NexPage(1, 0, "page1");
NexPage page2    = NexPage(2, 0, "page2");

NexTouch *nex_listen_list[] = 
{
    &b00,
    &b01,
    &b10,
    &b20,
    &b21,
    &b22,
    NULL
};
