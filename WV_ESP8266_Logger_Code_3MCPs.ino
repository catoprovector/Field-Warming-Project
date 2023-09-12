#include "RTClib.h"
#include <Wire.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_I2CRegister.h>
#include "Adafruit_MCP9600.h"
#include <Adafruit_SPIDevice.h>
#include <SPI.h>
#include <SD.h>

RTC_PCF8523 rtc;//RTC class

//Define thermocouple boards
Adafruit_MCP9600 mcp1;
Adafruit_MCP9600 mcp2;
Adafruit_MCP9600 mcp3;
Adafruit_MCP9600 mcp4;
Adafruit_MCP9600 mcp5;
Adafruit_MCP9600 mcp6;
Adafruit_MCP9600 mcp7;

//create a new file object
File logfile;

//define days of the week for the RTC
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};


void setup()
{
    Serial.begin(115200);

    #ifndef ESP8266 //connection of the ESP8266
  while (!Serial); // wait for serial port to connect. Needed for native USB
    #endif

    //initialize SD logger  
    if (!SD.begin(15)) {
          Serial.println("initialization failed!");
          while (1);
      }
      Serial.print("SD found");

    //RTC setup
    if (! rtc.begin()) {
        Serial.println("Couldn't find RTC");
        Serial.flush();
        while (1) delay(10);
      }
      
      if (! rtc.initialized() || rtc.lostPower()) {
    Serial.println("RTC is NOT initialized, let's set the time!"); // this section resets the time on the RTC if it has fully lost power (i.e. battery removed)
   
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));   
  }

    rtc.start();//start RTC

    /* Initialise the driver with I2C_ADDRESS and the default I2C bus. */
    if (! mcp1.begin(0x60)) {
        Serial.println("Sensor 1 not found. Check wiring!");
        while (1);
    }
    if (! mcp2.begin(0x61)) {
        Serial.println("Sensor 2 not found. Check wiring!");
        while (1);
    }
    if (! mcp3.begin(0x62)) {
        Serial.println("Sensor 3 not found. Check wiring!");
        while (1);
    }  
    
  Serial.println("Found thermocouples!");

  //the next section of code sets the ADC resolution for the themocouple amplifiers
  mcp1.setADCresolution(MCP9600_ADCRESOLUTION_18);
  Serial.print("ADC resolution set to ");
  switch (mcp1.getADCresolution()) {
    case MCP9600_ADCRESOLUTION_18:   Serial.print("18"); break;
    case MCP9600_ADCRESOLUTION_16:   Serial.print("16"); break;
    case MCP9600_ADCRESOLUTION_14:   Serial.print("14"); break;
    case MCP9600_ADCRESOLUTION_12:   Serial.print("12"); break;
  }
  Serial.println(" bits");

  mcp2.setADCresolution(MCP9600_ADCRESOLUTION_18);
  Serial.print("ADC resolution set to ");
  switch (mcp2.getADCresolution()) {
    case MCP9600_ADCRESOLUTION_18:   Serial.print("18"); break;
    case MCP9600_ADCRESOLUTION_16:   Serial.print("16"); break;
    case MCP9600_ADCRESOLUTION_14:   Serial.print("14"); break;
    case MCP9600_ADCRESOLUTION_12:   Serial.print("12"); break;
  }
  Serial.println(" bits");

  mcp3.setADCresolution(MCP9600_ADCRESOLUTION_18);
  Serial.print("ADC resolution set to ");
  switch (mcp3.getADCresolution()) {
    case MCP9600_ADCRESOLUTION_18:   Serial.print("18"); break;
    case MCP9600_ADCRESOLUTION_16:   Serial.print("16"); break;
    case MCP9600_ADCRESOLUTION_14:   Serial.print("14"); break;
    case MCP9600_ADCRESOLUTION_12:   Serial.print("12"); break;
  }
  Serial.println(" bits");

  
  //this section sets the amplifiers to thermocouple type T
  mcp1.setThermocoupleType(MCP9600_TYPE_T);
  Serial.print("Thermocouple type set to ");
  switch (mcp1.getThermocoupleType()) {
    case MCP9600_TYPE_K:  Serial.print("K"); break;
    case MCP9600_TYPE_J:  Serial.print("J"); break;
    case MCP9600_TYPE_T:  Serial.print("T"); break;
    case MCP9600_TYPE_N:  Serial.print("N"); break;
    case MCP9600_TYPE_S:  Serial.print("S"); break;
    case MCP9600_TYPE_E:  Serial.print("E"); break;
    case MCP9600_TYPE_B:  Serial.print("B"); break;
    case MCP9600_TYPE_R:  Serial.print("R"); break;
  }
  Serial.println(" type");

  mcp2.setThermocoupleType(MCP9600_TYPE_T);
  Serial.print("Thermocouple type set to ");
  switch (mcp2.getThermocoupleType()) {
    case MCP9600_TYPE_K:  Serial.print("K"); break;
    case MCP9600_TYPE_J:  Serial.print("J"); break;
    case MCP9600_TYPE_T:  Serial.print("T"); break;
    case MCP9600_TYPE_N:  Serial.print("N"); break;
    case MCP9600_TYPE_S:  Serial.print("S"); break;
    case MCP9600_TYPE_E:  Serial.print("E"); break;
    case MCP9600_TYPE_B:  Serial.print("B"); break;
    case MCP9600_TYPE_R:  Serial.print("R"); break;
  }
  Serial.println(" type");

  mcp3.setThermocoupleType(MCP9600_TYPE_T); 
  Serial.print("Thermocouple type set to ");
  switch (mcp3.getThermocoupleType()) {
    case MCP9600_TYPE_K:  Serial.print("K"); break;
    case MCP9600_TYPE_J:  Serial.print("J"); break;
    case MCP9600_TYPE_T:  Serial.print("T"); break;
    case MCP9600_TYPE_N:  Serial.print("N"); break;
    case MCP9600_TYPE_S:  Serial.print("S"); break;
    case MCP9600_TYPE_E:  Serial.print("E"); break;
    case MCP9600_TYPE_B:  Serial.print("B"); break;
    case MCP9600_TYPE_R:  Serial.print("R"); break;
  }
  Serial.println(" type");

  
  //this determines the filter coefficient
  mcp1.setFilterCoefficient(3);
  Serial.print("Filter coefficient value set to: ");
  Serial.println(mcp1.getFilterCoefficient());

  mcp2.setFilterCoefficient(3);
  Serial.print("Filter coefficient value set to: ");
  Serial.println(mcp2.getFilterCoefficient());

  mcp3.setFilterCoefficient(3);
  Serial.print("Filter coefficient value set to: ");
  Serial.println(mcp3.getFilterCoefficient());


  //This sets temperature alarms
  mcp1.setAlertTemperature(1, 60);
  Serial.print("Alert #1 temperature set to ");
  Serial.println(mcp1.getAlertTemperature(1));
  mcp1.configureAlert(1, true, true);  // alert 1 enabled, rising temp

  mcp2.setAlertTemperature(1, 60);
  Serial.print("Alert #1 temperature set to ");
  Serial.println(mcp2.getAlertTemperature(1));
  mcp2.configureAlert(1, true, true);  // alert 1 enabled, rising temp

  mcp3.setAlertTemperature(1, 60);
  Serial.print("Alert #1 temperature set to ");
  Serial.println(mcp3.getAlertTemperature(1));
  mcp3.configureAlert(1, true, true);  // alert 1 enabled, rising temp


  //this starts the thermocouple amplifiers
  mcp1.enable(true);

  mcp2.enable(true);

  mcp3.enable(true);
  
}


void loop() {

  //reads out the date and time from the RTC
  DateTime now = rtc.now();

    Serial.print(now.year(), DEC);
    Serial.print(' ');
    Serial.print(now.month(), DEC);
    Serial.print(' ');
    Serial.print(now.day(), DEC);
    Serial.print(" (");
    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    Serial.print(") ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();

  Serial.print("Thermocouple 1 Read: "); Serial.println(mcp1.readThermocouple()); //prints themrocouple data to the serial monitor
  Serial.print("Thermocouple 2 Read: "); Serial.println(mcp2.readThermocouple());
  Serial.print("Thermocouple 3 Read: "); Serial.println(mcp3.readThermocouple());

  Serial.print("Ambient 1 Read: "); Serial.println(mcp1.readAmbient()); //prints ambient temperature to the serial monitor
  Serial.print("Ambient 2 Read: "); Serial.println(mcp2.readAmbient());
  Serial.print("Ambient 3 Read: "); Serial.println(mcp3.readAmbient());
  
  Serial.print("ADC 1: "); Serial.print(mcp1.readADC() * 2); Serial.println(" uV"); //prints ADC value to the serial monitor
  Serial.print("ADC 2: "); Serial.print(mcp2.readADC() * 2); Serial.println(" uV");
  Serial.print("ADC 3: "); Serial.print(mcp3.readADC() * 2); Serial.println(" uV");
  
  //creates new variables for the thermocouple reads
  float temp1 = mcp1.readThermocouple();
  float temp2 = mcp2.readThermocouple();
  float temp3 = mcp3.readThermocouple();
  
  //creates new variables for the reads
  char read1[10];
  char read2[10];
  char read3[10];
  
  //combines the floats and characters into strings
  dtostrf(temp1, 3, 3, read1);
  dtostrf(temp2, 3, 3, read2);
  dtostrf(temp3, 3, 3, read3);
  
  //opens the logfile on the SD card
  logfile = SD.open("OTC_Control_Temp_Log.txt", FILE_WRITE);

  //prints the temperature strings to the file
  logfile.print(read1);
  logfile.print(',');
  logfile.print(read2);
  logfile.print(',');
  logfile.print(read3);
  logfile.print(',');
  
  //prints the date and time to the file
  logfile.print(now.year(), DEC);
  logfile.print(' ');
  logfile.print(now.month(), DEC);
  logfile.print(' ');
  logfile.print(now.day(), DEC);
  logfile.print(',');
  logfile.print(now.hour(), DEC);
  logfile.print(':');
  logfile.print(now.minute(), DEC);
  logfile.print(':');
  logfile.print(now.second(), DEC);
  logfile.println();

  //closes the file
  logfile.close();

  //prints the reads to the serial monitor as an extra check
  Serial.println(read1);
  Serial.println(read2);
  Serial.println(read3); 


  delay(60000);
  
}
