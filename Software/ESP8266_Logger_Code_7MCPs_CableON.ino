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

File logfile;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

void setup()
{
    Serial.begin(115200);

    #ifndef ESP8266
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
    Serial.println("RTC is NOT initialized, let's set the time!");
   
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));   
  }

    rtc.start();//start RTC

    /* Initialise the driver with I2C_ADDRESS and the default I2C bus. */
    if (! mcp1.begin(0x67)) {
        Serial.println("Sensor 1 not found. Check wiring!");
        while (1);
    }
    if (! mcp2.begin(0x66)) {
        Serial.println("Sensor 2 not found. Check wiring!");
        while (1);
    }
    if (! mcp3.begin(0x60)) {
        Serial.println("Sensor 3 not found. Check wiring!");
        while (1);
    }  
    if (! mcp4.begin(0x61)) {
        Serial.println("Sensor 4 not found. Check wiring!");
        while (1);
    }
    if (! mcp5.begin(0x65)) {
        Serial.println("Sensor 5 not found. Check wiring!");
        while (1);
    }
    if (! mcp6.begin(0x64)) {
       Serial.println("Sensor 6 not found. Check wiring!");
       while (1);
    }  
    if (! mcp7.begin(0x63)) {
       Serial.println("Sensor 7 not found. Check wiring!");
       while (1);
    }  
  Serial.println("Found thermocouples!");

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

  mcp4.setADCresolution(MCP9600_ADCRESOLUTION_18);
  Serial.print("ADC resolution set to ");
  switch (mcp4.getADCresolution()) {
    case MCP9600_ADCRESOLUTION_18:   Serial.print("18"); break;
    case MCP9600_ADCRESOLUTION_16:   Serial.print("16"); break;
    case MCP9600_ADCRESOLUTION_14:   Serial.print("14"); break;
    case MCP9600_ADCRESOLUTION_12:   Serial.print("12"); break;
  }
  Serial.println(" bits");

  mcp5.setADCresolution(MCP9600_ADCRESOLUTION_18);
  Serial.print("ADC resolution set to ");
  switch (mcp5.getADCresolution()) {
    case MCP9600_ADCRESOLUTION_18:   Serial.print("18"); break;
    case MCP9600_ADCRESOLUTION_16:   Serial.print("16"); break;
    case MCP9600_ADCRESOLUTION_14:   Serial.print("14"); break;
    case MCP9600_ADCRESOLUTION_12:   Serial.print("12"); break;
  }
  Serial.println(" bits");

  mcp6.setADCresolution(MCP9600_ADCRESOLUTION_18);
  Serial.print("ADC resolution set to ");
  switch (mcp6.getADCresolution()) {
    case MCP9600_ADCRESOLUTION_18:   Serial.print("18"); break;
    case MCP9600_ADCRESOLUTION_16:   Serial.print("16"); break;
    case MCP9600_ADCRESOLUTION_14:   Serial.print("14"); break;
    case MCP9600_ADCRESOLUTION_12:   Serial.print("12"); break;
  }
  Serial.println(" bits");

  mcp7.setADCresolution(MCP9600_ADCRESOLUTION_18);
  Serial.print("ADC resolution set to ");
  switch (mcp6.getADCresolution()) {
    case MCP9600_ADCRESOLUTION_18:   Serial.print("18"); break;
    case MCP9600_ADCRESOLUTION_16:   Serial.print("16"); break;
    case MCP9600_ADCRESOLUTION_14:   Serial.print("14"); break;
    case MCP9600_ADCRESOLUTION_12:   Serial.print("12"); break;
  }
  Serial.println(" bits");

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

  mcp4.setThermocoupleType(MCP9600_TYPE_T);
  Serial.print("Thermocouple type set to ");
  switch (mcp4.getThermocoupleType()) {
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

  mcp5.setThermocoupleType(MCP9600_TYPE_T);
  Serial.print("Thermocouple type set to ");
  switch (mcp5.getThermocoupleType()) {
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

  mcp6.setThermocoupleType(MCP9600_TYPE_T); 
  Serial.print("Thermocouple type set to ");
  switch (mcp6.getThermocoupleType()) {
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

  mcp7.setThermocoupleType(MCP9600_TYPE_T); 
  Serial.print("Thermocouple type set to ");
  switch (mcp6.getThermocoupleType()) {
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

  mcp1.setFilterCoefficient(3);
  Serial.print("Filter coefficient value set to: ");
  Serial.println(mcp1.getFilterCoefficient());

  mcp2.setFilterCoefficient(3);
  Serial.print("Filter coefficient value set to: ");
  Serial.println(mcp2.getFilterCoefficient());

  mcp3.setFilterCoefficient(3);
  Serial.print("Filter coefficient value set to: ");
  Serial.println(mcp3.getFilterCoefficient());

  mcp4.setFilterCoefficient(3);
  Serial.print("Filter coefficient value set to: ");
  Serial.println(mcp4.getFilterCoefficient());

  mcp5.setFilterCoefficient(3);
  Serial.print("Filter coefficient value set to: ");
  Serial.println(mcp5.getFilterCoefficient());

  mcp6.setFilterCoefficient(3);
  Serial.print("Filter coefficient value set to: ");
  Serial.println(mcp6.getFilterCoefficient());

  mcp7.setFilterCoefficient(3);
  Serial.print("Filter coefficient value set to: ");
  Serial.println(mcp6.getFilterCoefficient());




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

  mcp4.setAlertTemperature(1, 60);
  Serial.print("Alert #1 temperature set to ");
  Serial.println(mcp4.getAlertTemperature(1));
  mcp4.configureAlert(1, true, true);  // alert 1 enabled, rising temp

  mcp5.setAlertTemperature(1, 60);
  Serial.print("Alert #1 temperature set to ");
  Serial.println(mcp5.getAlertTemperature(1));
  mcp5.configureAlert(1, true, true);  // alert 1 enabled, rising temp

  mcp6.setAlertTemperature(1, 60);
  Serial.print("Alert #1 temperature set to ");
  Serial.println(mcp6.getAlertTemperature(1));
  mcp6.configureAlert(1, true, true);  // alert 1 enabled, rising temp

  mcp7.setAlertTemperature(1, 60);
  Serial.print("Alert #1 temperature set to ");
  Serial.println(mcp6.getAlertTemperature(1));
  mcp6.configureAlert(1, true, true);  // alert 1 enabled, rising temp



  mcp1.enable(true);

  mcp2.enable(true);

  mcp3.enable(true);

  mcp4.enable(true);

  mcp5.enable(true);

  mcp6.enable(true);

  mcp7.enable(true);


  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);

}



void loop() {

  DateTime now = rtc.now();

    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
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

  Serial.print("Thermocouple 1 Read: "); Serial.println(mcp1.readThermocouple());
  Serial.print("Thermocouple 2 Read: "); Serial.println(mcp2.readThermocouple());
  Serial.print("Thermocouple 3 Read: "); Serial.println(mcp3.readThermocouple());
  Serial.print("Thermocouple 4 Read: "); Serial.println(mcp4.readThermocouple());
  Serial.print("Thermocouple 5 Read: "); Serial.println(mcp5.readThermocouple());
  Serial.print("Thermocouple 6 Read: "); Serial.println(mcp6.readThermocouple());
  Serial.print("Thermocouple 7 Read: "); Serial.println(mcp7.readThermocouple());
  Serial.print("Ambient 1 Read: "); Serial.println(mcp1.readAmbient());
  Serial.print("Ambient 2 Read: "); Serial.println(mcp2.readAmbient());
  Serial.print("Ambient 3 Read: "); Serial.println(mcp3.readAmbient());
  Serial.print("Ambient 4 Read: "); Serial.println(mcp4.readAmbient());
  Serial.print("Ambient 5 Read: "); Serial.println(mcp5.readAmbient());
  Serial.print("Ambient 6 Read: "); Serial.println(mcp6.readAmbient());
  Serial.print("Ambient 7 Read: "); Serial.println(mcp7.readAmbient());
  Serial.print("ADC 1: "); Serial.print(mcp1.readADC() * 2); Serial.println(" uV");
  Serial.print("ADC 2: "); Serial.print(mcp2.readADC() * 2); Serial.println(" uV");
  Serial.print("ADC 3: "); Serial.print(mcp3.readADC() * 2); Serial.println(" uV");
  Serial.print("ADC 4: "); Serial.print(mcp4.readADC() * 2); Serial.println(" uV");
  Serial.print("ADC 5: "); Serial.print(mcp5.readADC() * 2); Serial.println(" uV");
  Serial.print("ADC 6: "); Serial.print(mcp6.readADC() * 2); Serial.println(" uV");
  Serial.print("ADC 7: "); Serial.print(mcp7.readADC() * 2); Serial.println(" uV");

  float temp1 = mcp1.readThermocouple();
  float temp2 = mcp2.readThermocouple();
  float temp3 = mcp3.readThermocouple();
  float temp4 = mcp4.readThermocouple();
  float temp5 = mcp5.readThermocouple();
  float temp6 = mcp6.readThermocouple();
  float temp7 = mcp7.readThermocouple();

  char read1[10];
  char read2[10];
  char read3[10];
  char read4[10];
  char read5[10];
  char read6[10];
  char read7[10];

  dtostrf(temp1, 3, 3, read1);
  dtostrf(temp2, 3, 3, read2);
  dtostrf(temp3, 3, 3, read3);
  dtostrf(temp4, 3, 3, read4);
  dtostrf(temp5, 3, 3, read5);
  dtostrf(temp6, 3, 3, read6);
  dtostrf(temp7, 3, 3, read7);


  logfile = SD.open("temp_log_7.txt", FILE_WRITE);

  logfile.print(read1);
  logfile.print(',');
  logfile.print(read2);
  logfile.print(',');
  logfile.print(read3);
  logfile.print(',');
  logfile.print(read4);
  logfile.print(',');
  logfile.print(read5);
  logfile.print(',');
  logfile.print(read6);
  logfile.print(',');
  logfile.print(read7);
  logfile.print(',');
    
  logfile.print(now.year(), DEC);
  logfile.print('-');
  logfile.print(now.month(), DEC);
  logfile.print('-');
  logfile.print(now.day(), DEC);
  logfile.print(',');
  logfile.print(now.hour(), DEC);
  logfile.print(':');
  logfile.print(now.minute(), DEC);
  logfile.print(':');
  logfile.print(now.second(), DEC);
  logfile.println();

  logfile.close();

  Serial.println(read1);
  Serial.println(read2);
  Serial.println(read3); 
  Serial.println(read4);
  Serial.println(read5);
  Serial.println(read6);
  Serial.println(read7);


  delay(10000);
  
}
