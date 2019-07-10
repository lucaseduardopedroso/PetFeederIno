// Para testar lembre de setar os pinos e instalar as bibliotecas

#include <DS3231.h>  // relogio
#include <Servo.h>	// servo
#include <LiquidCrystal.h> // lcd
#include <Keypad.h> // teclado
#include <Ultrasonic.h> // ultrassom
#include <EEPROM.h> // pode comentar pq não está usando EEPROM por enquanto

#define LED 10   // LED divindindo o pino com o buzzer
#define pino_trigger 9 // pino ultrassom
#define pino_echo 8 // pino ultrassom
#define pino_buzzer 10 // pino buzzer

/* prototipo das funções */
void setFeedingTime(); // data e hora
void hcsr04(); // ultrassom e buzzer
int levalor(int pos); // optaclopador e EEPROM

Ultrasonic ultrasonic(pino_trigger, pino_echo); // inicializa os pinos do ultrassom

float distancia = 0.0; // distancia da racao
int isfeeded = 0; // controle se alimentou ou não
int is_on = 0;
int feed_hour[] = {0,0};

int t1, t2, t3, t4, t5, t6;

const byte ROWS = 4;
const byte COLS = 4;

// Define the Keymap

char keys[ROWS][COLS] = {

  {'1','2','3','A'},

  {'4','5','6','B'},

  {'7','8','9','C'},

  {'*','0','#','D'}

};

// Connect keypad ROW0, ROW1, ROW2 and ROW3 to these Arduino pins.

byte rowPins[ROWS] = { 2, 3, 4, 5 };

// Connect keypad COL0, COL1 and COL2 to these Arduino pins.

byte colPins[COLS] = { 6, 7, 8, 9 };

//  Create the Keypad
Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );



DS3231  rtc(A4,A5); // relogio
RTCDateTime data;
Servo servo_test;      //initialize a servo object for the connected servo
LiquidCrystal lcd(A0, A1, A2, A3, 12, 13); // Creates an LC object. Parameters: (rs, enable, d4, d5, d6, d7)

// variaveis de tempo


boolean feed = true; // condition for alarm

 char key; // tecla
 int r[6]; // auxiliar

 void setup()
 {
  servo_test.attach(11);   // pino do servo
  rtc.begin(); // inicia relogio
  rtc.setDateTime(__DATE__, __TIME__);
  lcd.begin(16,2); // inicia lcd
  servo_test.write(55);
  pinMode(A0, OUTPUT); // lcd
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(10, OUTPUT); // led
  pinMode(pino_echo, INPUT); // ultrassom
  pinMode(pino_trigger, OUTPUT);
  pinMode(pino_buzzer, OUTPUT); // buzzer
  Serial.begin(9600); // monitor serial
  Serial.println("cachorro");
 }

 void loop()
 {

lcd.setCursor(0,0); // lcd
char buttonPress;
buttonPress = kpd.getkey(); //  //qlqr botão

 if (buttonPress) { // se estiver pressionado
 digitalWrite(LED, HIGH); // acende led
 delay(100);
 setFeedingTime(); // rotina de data e hora
 digitalWrite(LED, LOW); // apaga o led
 isfeeded = 1; // seta como alimentado
   }

 // Simula a queda de energia e dado da EPROM

  /*EEPROM.write(0,isfeeded);
 int aux = levalor(0);
 if(aux == 1)
 else
 return;
 */

//Serial.println(buttonPress);

 lcd.print("Time:  ");
 String t = "";
 t = rtc.getTimeStr();
 t1 = t.charAt(0)-48;
 t2 = t.charAt(1)-48;
 t3 = t.charAt(3)-48;
 t4 = t.charAt(4)-48;
 t5 = t.charAt(6)-48;
 t6 = t.charAt(7)-48;

 lcd.print(rtc.getTimeStr());
 lcd.setCursor(0,1);
 lcd.print("Date: ");
 lcd.print(rtc.getDateStr());

  hcsr04(); // chama o ultrassom
 if ( is_on == 1 && feed==true)
 {
  digitalWrite(LED, HIGH);
  servo_test.write(100);                   //command to rotate the servo to the specified angle
   delay(400);
  servo_test.write(55);
  feed=false;
   digitalWrite(LED, LOW);
   noTone(pino_buzzer);
 }
 feedingTime();
 }

// rotina criada no projeto original
void setFeedingTime()
{
  feed = true;
   int i=0;

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Set feeding Time");
  lcd.clear();
  lcd.print("HH:MM");
  lcd.setCursor(0,1);


  while(1){
    key = kpd.getKey();

    char j;

  if(key!=NO_KEY){

    lcd.setCursor(j,1);

    lcd.print(key);

    r[i] = key-48;
    i++;
    j++;

    if (j==2)
    {
      lcd.print(":"); j++;
    }
    delay(500);
  }

  if (key == 'D')
  {key=0; break; }
  }
}

// funcao de ultrassom
void hcsr04() {

  digitalWrite(pino_trigger, LOW); // manipula pinos do ultrassom
  delayMicroseconds(2);
  digitalWrite(pino_trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(pino_trigger, LOW);
  distancia = (ultrasonic.Ranging(CM)); // mede a distancia em cm
  delay(200);
  if (distancia <= 30) { // se a distancia for menor que determinado valor
      tone(pino_buzzer,2500); // aciona o buzzer
  }

}

// novo feeding time
void feedingTime() {
  char button;
  lcd.print("Feeding");
  int h, m;
  h = data.hour;
  m = data.minute;

  if (button) {


  if(button == '1') {
    lcd.setCursor(2,1);
    feed_hour[0] += 1;
    if (feed_hour[0]==24) {
      feed_hour[0] = 0
    }
    lcd.print(feed_hour[0]);
    lcd.print(":");
    lcd.print(feed_hour[1]);
    delay(100);
  }

  else if (button == '2') {
    lcd.setCursor(2,1);
    feed_hour[1] += 5;
    if (feed_hour[1] == 60) {
      feed_hour[1]=0;
    }
    lcd.print(feed_hour[0]);
    lcd.print(":");
    lcd.print(feed_hour[1]);
    delay(100);
  }

  else if (button == '3') {
    lcd.setCursor(2,1);
    feed_hour[1] += 5;
    if (feed_hour[1] == 60) {
      feed_hour[1]=0;
    }
    lcd.print(feed_hour[0]);
    lcd.print(":");
    lcd.print(feed_hour[1]);
    delay(1000);
  }
}

  if (h == feed_hour[0] && m == feed_hour[1]) {
    lcd.setCursor(2,1);
    lcd.print("quer comer");
    delay(1000);
    lcd.clear();
  }
}

// funcao utilizada somente se tiver optaclopador
// simula a queda de energia
// pode comentar
int levalor(int pos) {

  int flag = 0;

  int isfeeded2 = EEPROM.read(pos);

  int opta = 1;

  if(opta == 1 && isfeeded2 == 0) {

   flag = 1;

  }

  return flag;

}
