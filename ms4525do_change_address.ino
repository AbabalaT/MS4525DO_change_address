#include <Wire.h>   //I2C library 0x28H

#define TRUE 1
#define FALSE 0

#define new_address 0x36

byte fetch_pressure(unsigned int *p_Pressure); //convert value to byte data type

int i = 0;
float buff[6];

void setup(void)
{
  Serial.begin(115200);
  Wire.begin();
  
  int result = 0;
  uint8_t address = 0x28;
  uint8_t eep_rx[3];
  uint8_t eep_tx[3];
  uint16_t eepromTemp= 0x0;
  delay(5000);
  
  pinMode(34, OUTPUT);
  digitalWrite(34, LOW);
  delay(2);

  Wire.beginTransmission(address);
  delay(1);
  Wire.endTransmission();
  delay(1);

  eep_tx[0]=0xA0;
  eep_tx[1]=0x00;
  eep_tx[2]=0x00;
  Wire.beginTransmission(address);
  // Wire.write(eep_tx[0]);
  // Wire.write(eep_tx[1]);
  // Wire.write(eep_tx[2]);
  Wire.write(eep_tx, 3);
  Wire.endTransmission();
  delay(10);

  eep_tx[0] = 0x02;
  eep_tx[1] = 0x00;
  eep_tx[2] = 0x00;
  Wire.beginTransmission(address);
  Wire.write(eep_tx, 3);
  Wire.endTransmission();
  delay(10);

  eep_rx[0] = 0x15;
  eep_rx[1] = 0x15;
  eep_rx[2] = 0x15;
  
  Wire.requestFrom((int)address, (int)3, false);
  delay(5);
  eep_rx[0]=Wire.read();
  eep_rx[1]=Wire.read();
  eep_rx[2]=Wire.read();
  Wire.endTransmission();
  Serial.println("Fetch Finished.");

  Serial.println(eep_rx[0]);
  Serial.println(eep_rx[1]);
  Serial.println(eep_rx[2]);

  delay(10);
  if(eep_rx[0] == 0x5A){
    Serial.println("Success 02");

    i = eep_rx[0];
    eepromTemp=(((eep_rx[1] << 8) + eep_rx[2]) & 0xE007) + (new_address<<3) + 0xC00;
    eep_tx[0]=0x42;
    eep_tx[1]=(uint8_t)((eepromTemp & 0xFF00)>>8);
    eep_tx[2]=(uint8_t)(eepromTemp & 0x00FF);
    Wire.beginTransmission(address);
    Wire.write(eep_tx[0]);
    Wire.write(eep_tx[1]);
    Wire.write(eep_tx[2]);
    Wire.endTransmission();
    Serial.println("Success 03");

    eep_tx[0]=0x80;
    eep_tx[1]=0x00;
    eep_tx[2]=0x00;
    Wire.beginTransmission(address);
    Wire.write(eep_tx[0]);
    Wire.write(eep_tx[1]);
    Wire.write(eep_tx[2]);
    Wire.endTransmission();
    Serial.println("Success 04");
    delay(5000);
  }else{
    i = eep_rx[0];
  }
}

void loop()
{
  byte _status;
  unsigned int P_dat;
  unsigned int T_dat;
  double PR;
  double TR;
  double V;
  double VV;
  while (1)
  {
    _status = fetch_pressure(&P_dat, &T_dat);

    PR = (double)((P_dat-819.15)/(14744.7)) ;
    PR = (PR - 0.49060678) ;
    PR = abs(PR);
    V = ((PR*13789.5144)/1.225);
    VV = (sqrt((V)));
    TR = (double)((T_dat*0.09770395701));
    TR = TR-50;

    Serial.print("i:");
    Serial.print(i);
    Serial.print(", raw Pressure:");
    Serial.print(P_dat);
    Serial.print(", pressure psi:");
    Serial.print(PR,10);
    Serial.print(" ");
    Serial.print(", raw Temp:");
    Serial.print(T_dat);
    Serial.print(", temp:");
    Serial.print(TR);
    Serial.print(", speed m/s :");
    Serial.println(VV,5);

    delay(1000);
  }
}
  


byte fetch_pressure(unsigned int *p_P_dat, unsigned int *p_T_dat)
{
  byte address, Press_H, Press_L, _status;

  unsigned int P_dat;
  unsigned int T_dat;
  address = new_address;

  Wire.requestFrom((int)address, (int)4);
  Press_H = Wire.read();
  Press_L = Wire.read();
  byte Temp_H = Wire.read();
  byte Temp_L = Wire.read();
  Wire.endTransmission();

  _status = (Press_H >> 6) & 0x03;
  Press_H = Press_H & 0x3f;

  P_dat = (((unsigned int)Press_H) << 8) | Press_L;
  *p_P_dat = P_dat;
  
  Temp_L = (Temp_L >> 5);
  T_dat = (((unsigned int)Temp_H) << 3) | Temp_L;

  *p_T_dat = T_dat;

  return (_status);
}