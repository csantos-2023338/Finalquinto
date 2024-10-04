#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>
#include <DFRobotDFPlayerMini.h>
SoftwareSerial mySoftwareSerial(12, 10); 
DFRobotDFPlayerMini myDFPlayer;
LiquidCrystal_I2C lcd(0x27, 16, 2);
int poten = A3;
int ocupado = 6;
int reproducir = 5;
int parar = 4;
int proxima = 3;
int anterior = 2;
int ant, pro, par, rep, pot, VOL, VOL_OLD, ocu;
int estado = 1;
int flag = 0;
byte negra[] = {B00011, B00010, B00010, B00010, B01110, B11110, B11110, B01100};
byte play_[] = {B01000, B01100, B01110, B01111, B01110, B01100, B01000, B00000};
byte pause_[] = {B00000, B01010, B01010, B01010, B01010, B01010, B00000, B00000};
int negra0 = negra;
int play_1 = play_;
int pause2 = pause_;
int contseg = 0;
int contmin = 0;
int playing = 0;
void setup() {
  pinMode(poten, INPUT);
  pinMode(reproducir, INPUT);
  pinMode(parar, INPUT);
  pinMode(proxima, INPUT);
  pinMode(anterior, INPUT);
  pinMode(ocupado, INPUT);
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.createChar(0, negra0);
  lcd.createChar(1, play_1);
  lcd.createChar(2, pause2);
  lcd.setCursor(0, 0);
  lcd.write(0);
  lcd.print("Santos Music");
  lcd.write(0);
  lcd.leftToRight();
  lcd.setCursor (0, 1);
  lcd.print("** MP3 Player **");
  delay(3000);

  mySoftwareSerial.begin(9600);
  myDFPlayer.begin(mySoftwareSerial);
  myDFPlayer.setTimeOut(500);
  myDFPlayer.volume(25);
  myDFPlayer.EQ(DFPLAYER_EQ_BASS);
  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);
  lcd.setCursor (7, 1);
  lcd.print("PressPlay");
}
void loop()
{
  ant = digitalRead(anterior);
  pro = digitalRead(proxima);
  par = digitalRead(parar);
  rep = digitalRead(reproducir);
  pot = analogRead(poten);
  ocu = digitalRead(ocupado);
  if ((ocu == 1) && (flag == 0)) {
    lcd.setCursor(0, 0);
    lcd.write(0);
    lcd.print("Santos Music");
    lcd.write(0);
    lcd.leftToRight();
    lcd.setCursor (7, 1);
    lcd.print("PressPlay");
    delay(20);
  }
  if (ocu == 0) {
    contseg++;
    Serial.println("segundos");
    Serial.println(contseg);
    Serial.println("playing");
    Serial.println(playing);
    lcd.setCursor(11, 1);
    lcd.print('0');
    lcd.print(contmin);
    lcd.print(":");
    lcd.print('0');
    lcd.print('0');
    if (contseg < 10) {
      lcd.setCursor(15, 1);
    }
    else if (contseg >= 10) {
      lcd.setCursor(14, 1);
      if (contseg > 59) {
        contmin++;
        lcd.setCursor(11, 1);
        lcd.print('0');
        lcd.print(contmin);
        lcd.print(":");
        contseg = 0;
      }
    }
    lcd.print(contseg);
    delay(1000);
  }
  VOL = map(pot, 0, 1023, 0, 30);
  delay(20);
  if (VOL != VOL_OLD) {
    VOL_OLD = VOL;
    lcd.setCursor(0, 1);
    lcd.print("Vol:");
    if (VOL >= 10) {
      lcd.setCursor(4, 1);
    }
    else if (VOL < 10) {
      lcd.print('0');
    }
    lcd.print(VOL);
    lcd.print(" ");
    myDFPlayer.volume(VOL);
  }

  if (rep == LOW) {
    delay(2);
    myDFPlayer.play();
    reprod();
  }
  if (ant == LOW) {
    delay(2);
    myDFPlayer.previous();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Anterior...");
    contseg = 0;
    contmin = 0;
    delay(1000);
    reprod();
  }
  if (pro == LOW) {
    myDFPlayer.next();
    delay(2);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Siguiente...");
    delay(1000);
    contseg = 0;
    contmin = 0;
    reprod();
  }
  if (par == LOW) {
    if (flag == 0) {
      delay(20);
      flag = 1;
      playing = 0;
      myDFPlayer.pause();
      lcd.setCursor(0, 0);
      lcd.print("Pause  ");
      lcd.setCursor(12, 0);
      lcd.write(2);
      delay(1000);
    }
    else if (flag == 1) {
      delay(20);
      myDFPlayer.start();
      flag = 0;
      reprod();
    }
  }
}
void reprod() {
  lcd.clear();
  lcd.setCursor(0, 0);
  playing = 1;
  lcd.print("Playing");
  delay(1000);
  lcd.setCursor(9, 0);
  lcd.print('0');
  lcd.print(myDFPlayer.readCurrentFileNumber());
  lcd.print(" ");
  lcd.write(1);
  flag=0;
  delay(1000);
}
