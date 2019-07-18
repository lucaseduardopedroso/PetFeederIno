#include <EEPROM.h>

#include <LiquidCrystal.h>
#include "RTClib.h"
#include <Keypad.h>
#include <Wire.h>
#include <Servo.h>


#define pinServo 13


//---------variaveis globais ----------

int c;                         //variavel qtd de ração
int i;                        //contador 1
int j;                        //contador 2
int k;                        //contador 3
int l;                        //contador 4
int valor;                    //armazenar valores temporários
int hor2;                     //armazena os valores inteiros em horas para a entrada do horario
int min2;                      //armazena os valores inteiros em minutos para a entrada do horario
int guardaHora[15];                //armazena a hora do horario
int guardaMin[15];                  //armazena minutos do horario
bool erro;                    //armazenar sinalização de erro
String hora = "";
int tempoAlim = 6000;
int confSerial = 0; //se tiver 0, não vai configurar por serial

int proxAlim = 1;



//---------configuração servo-----------------
Servo servo1;



//----------configuração relogio-----------------
RTC_DS1307 rtc;
int seg1, min1, hor1;


//---------configuração teclado -----------------


const byte LINHAS = 4; //número de linhas do teclado
const byte COLUNAS = 3; //número de colunas do teclado

byte pinosLinhas [LINHAS] = {12, 11, 10, 9}; //pinos que indicam as linhas do teclado

byte pinosColunas [COLUNAS] = {8, 7, 6}; //pinos que indicam as colunas do teclado


//define uma matriz com os símbolos que deseja ser lido do teclado
char teclas [LINHAS][COLUNAS] = { {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

char entrada;

//instancia de Keypad, responsável por capturar a tecla pressionada
Keypad teclado = Keypad(makeKeymap(teclas), pinosLinhas, pinosColunas, LINHAS, COLUNAS);


//--------------configuração lcd ---------------------

int D4 = 2;                            //Declaração dos pinos para LCD
int D5 = 3;
int D6 = 4;
int D7 = 5;
int E = A3;                             //pino Enable
int RS = A2;                            //pino RS

LiquidCrystal lcd(RS, E, D4, D5, D6, D7);



//----------configuração buzzer e led------------

int led = A0;
int buzzer = A0;
int tempoBuzz = 200;

//------------------conf botão--------------------

int botao = A1;


int val = 0; //1 ativo 0 desligado

void setup() {
  //-------------inicialização dos parametros e dispositivos--------
  Serial.begin(9600);
  //attachInterrupt(digitalPinToInterrupt(botao), teste, FALLING);
  pinMode(buzzer, OUTPUT);
  pinMode(led, OUTPUT);
  pinMode(botao, INPUT);
  servo1.attach(pinServo);
  servo1.write(15);
  Wire.begin();
  lcd.begin(16, 2);
  rtc.begin();
  
  //rtc.adjust( DateTime(2019, 7, 17, 19, 55, 30) );
  
  //ZERA O LOG!!!!!
  //escreveString(200, " ");

  msg();
  configure();
  if (confSerial == 0) {
    racao();
    horarios();
  }
  standby();
  menu();

//  EEPROMWriteInt(1, 0);
//  if (EEPROMWriteInt, 10) {
//    lcd.display();
//  } else {
//    lcd.noDisplay();
//  }
//  
}

void menu() {
  Serial.print("\n\n\n---------MENU--------------\n\n");
  Serial.print("1 - Log:\n");
  Serial.print("2 - Add Hora:\n");
  Serial.print("3 - Definir tempo abertura:\n");
  Serial.print("4 - Listar Horários:\n");
  Serial.print("Digite uma opção-> ");
}

//int ultimoEstBot = 0;

void loop() {

  //--------------configuração inicial do despertador ------------


//  testaBotao();
  standby();
  liberaRacao();
  serialEvento();           //Espera o teclado ser pressionado



}

void serialEvento() {

  while (Serial.available()) {

    //Serial.print("\n----------entro--------------\n");
    char valorSerial;
    Serial.readBytesUntil('\n', &valorSerial, 1);
    limpaSerial();
    Serial.flush();

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Configurando...");
    
    
//menu do serial
    switch (valorSerial) {
      case '4':
        Serial.print("Lista");
        for (int i = 1; i <= c; i++) {
          Serial.print("\n[" + String(guardaHora[i], DEC) + ":" + String(guardaMin[i], DEC) + "]");
        }
        //return;
        break;
      
      case '1':
        Serial.print("\n\nRELATÓRIO\n");
        Serial.print(leString(200));
        break;

      case '2':
        Serial.print("\nHoras:");
        while (Serial.available() < 2) {}
        //        if (Serial.available() < 2) {
        int value1;
        value1 = Serial.parseInt();
        Serial.print(String(value1));
        Serial.flush();

        Serial.print("\nMinutos:");
        while (Serial.available() < 2) {}
        int value2;
        value2 = Serial.parseInt();
        Serial.print(String(value2));

        if (value1 > 24 || value2 > 59) {
          Serial.print("\nHora Inválida!!");
          delay(500);
          break;
        }
        c++;
        guardaHora[c] = value1;
        guardaMin[c] = value2;
        Serial.print("\nHorário salvo!!");
        delay(300);
        //return;
        break;

      case '3':
        Serial.print("\nDigite o valor da abertura em segundos (1-9):");
        while (Serial.available() < 1) {}
        int tempo = Serial.parseInt();
        Serial.print("\n" + String(tempo) + " Segundos");
        delay(200);
        tempoAlim = tempo * 1000;
        //return;
        break;

      
      
      default:
        Serial.print("OPÇÃO INVÁLIDA!!!!!!!!!");
        break;
    }

    lcd.clear();

    //Serial.print(valorSerial + " --++");
    menu();
  }
}

void limpaSerial() {
  while (Serial.available() > 0) {
    char t = Serial.read();
  }
}
//
//void testaBotao() {
//  int estadoBotao = digitalRead(botao);
//  delay(10);
//
//  if (estadoBotao != ultimoEstBot) {
//    if (estadoBotao == HIGH) {
//      if (EEPROMReadInt(10) == 1) {
//        EEPROMWriteInt(10, 0);
//        Serial.print("LCD DESLIGADO\n");
//        lcd.noDisplay();
//
//      } else {
//
//        EEPROMWriteInt(10, 1);
//        lcd.display();
//        Serial.print("LCD LIGADO\n");
//
//      }
//    }
//  }
//  ultimoEstBot = estadoBotao;
//}

//-------------pega o horario para liberar comida
void horarios() {
  for (k = 1; k <= c; k++) {
    erro = 1;
    while (erro == 1) {

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Informe horario ");                //pede para o usuario digitar a hora
      lcd.setCursor(0, 1);
      lcd.print("#" + String(k) + " (hh:mm):");     //informa o formato que deve ser digitado a hora


      valor = 0;                                   //reseta o valor da variavel temporal

      // horas h;

      for (i = 1; i <= 2; i++) {                  //entrada dos digitos da hora
        entrada = teclado.waitForKey();
        digitalWrite(buzzer, HIGH);
        delay(tempoBuzz);
        digitalWrite(buzzer, LOW);
        valor = valor * 10 + (entrada - '0');    //acumula os digitos
        lcd.setCursor((i + 10), 1);
        lcd.print(entrada);
        hor2 = valor;                            //A entrada dos dois dígitos é o valor por hora

        //hora = hora + String(hor2, DEC);
      }
      //Serial.print(hora);

      lcd.setCursor((13), 1);                  //escreve o separador ":"
      lcd.print(":");

      valor = 0;

      for (i = 1; i <= 2; i++) {
        entrada = teclado.waitForKey();
        digitalWrite(buzzer, HIGH);
        delay(tempoBuzz);
        digitalWrite(buzzer, LOW);
        valor = valor * 10 + (entrada - '0');
        lcd.setCursor((i + 13), 1);
        lcd.print(entrada);
        delay(400);
        min2 = valor;                       //A entrada dos dois dígitos é o valor por minuto

      }

      if (hor2 > 23 || min2 > 59) {       //compara a hora errada
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Hora errada");
        delay(1200);
      }



      else {
        erro = 0;

      }

    }

    guardaHora[k] = hor2;
    guardaMin[k] = min2;
    //cria a string que vai ser salva na eeprom
    hora = "Horario #" + String(k) + ": " + String(hor2, DEC) + ":" +  String(min2, DEC) + "\n";
    //salva na eeprom
    escreveString(k + 1, hora);
    //imprime da eeprom
    Serial.print(leString(k + 1));

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("   Horario #" + String(k));
    lcd.setCursor(0, 1);
    lcd.print("    Guardado");
    delay(1200);

  }

}



//Verifica se vai ser feito a configuração manual ou pelo serial
void configure() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("*->Conf. Manual");
  lcd.setCursor(0, 1);
  lcd.print("#->Conf. Serial");
  entrada = teclado.waitForKey();
  if (entrada == '#') {
    confSerial = 1;
  }
}


//-----------pega a quantidade de ração------------------
void racao() {
  erro = 1;
  while (erro == 1) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Qtd de racao ");   //pede para inserir a qtde de ração por dia
    lcd.setCursor(0, 1);
    lcd.print("por dia");
    lcd.setCursor(11, 1);
    entrada = teclado.waitForKey();          //espera alguma tecla ser pressionada
    digitalWrite(buzzer, HIGH);              //aciona o buzzer para a tecla pressionada
    delay(tempoBuzz);                        //tempo do buzzer acionado
    digitalWrite(buzzer, LOW);                //desliga o buzzer
    lcd.print(entrada);                     //mostra o valor digitado nas teclas
    delay(100);
    c = entrada - '0';                      //converte o valor de entrada e guarda na variavel C a "quantidade de vezes que deve alimentar o animal"


    if (c >= 1) {                   //verificação de erro
      erro = 0;                    //se não houver erro marca como falso
      lcd.clear();
      lcd.setCursor(3, 0);
      lcd.print("Salvo");          //mostra a mensagem para o usuario que foi salvo
      lcd.setCursor(1, 1);
      lcd.print((String(c)) + " Racao");
      delay(1200);
      lcd.clear();
    }

    else {                           //caso tenha algum erro, marca como verdadeiro e repete o processo
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(" Num 0 invalido");
      delay(1200);

    }

  }

}

void liberaRacao() {
  lcd.clear();
  DateTime now = rtc.now();           //pega a hora atual
  hor1 = now.hour();                  //  atribui os valores das horas e minutos nas variaveis
  min1 = now.minute();                //

  for (k = 1; k <= c; k++) {          // verifica todos os horarios

    if ((hor1 == guardaHora[k]) && (min1 == guardaMin[k]) ) {       //compara o horario atual com o horario que foi digitado para liberar a ração
      analogWrite(led, 255);                                   //liga o led, informando que esta sendo liberado a ração
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.write("Liberando");                                 //informa no lcd que esta liberando a ração
      servo1.write(180);                                      //faz o servo girar para 100°
      delay(tempoAlim);                                            //tempo que o servo fica para o angulo
      servo1.write(15);                                       //volta o servo para o inicio
      lcd.clear();
      analogWrite(led, 0);                                    //desliga o led

//      int qtdeVezesAlimentado = EEPROMReadInt(1);
//      EEPROMWriteInt(1, qtdeVezesAlimentado + 1);

      //      proxAlim = (proxAlim + 1) % (c + 1);
      //      if (proxAlim == 0) {
      //        proxAlim = 1;
      //      }



      //escreve na eeprom para o log

      String base = '\n' + String(now.day(), DEC) + "/" + String(now.month(), DEC) + "/" + String(now.year(), DEC) + " " + String(guardaHora[k], DEC) + ":" + String(guardaMin[k], DEC);
      String nString = base + leString(200);        //concatena a nova string com os logs antigos
      escreveString(200, nString);                  //escreve a string concatenada na da eeprom

      //      msgLog = leString(200) + msgLog;/

      //      Serial.print(nString);


      while ((hor1 == guardaHora[k]) && (min1 == guardaMin[k])) {       //condição para saber se continuamos ao mesmo tempo que foi liberado a ração
        //Serial.print(EEPROMReadInt(1));
        //testaBotao();
        standby();                                                      //evita que a ração seja liberada mais de uma vez no mesmo tempo
        serialEvento();

      }
    }
   
  }
}

void standby() {

  DateTime now = rtc.now();               //é obtiddo a hora e minuto atual
  hor1 = now.hour();
  min1 = now.minute();
  seg1 = now.second();

  if(c==0){
    lcd.setCursor(0,0);
    lcd.print("Esperando conf.");
    lcd.setCursor(5,1);
    lcd.print("serial");
    delay(100);
    }

  //tenta prever o próximo
//   proxAlim = 1;
//    for (int i = 2; i <= c; i++) {
//      if ((guardaHora[i] - now.hour() ) < (guardaHora[proxAlim] - now.hour() )) {
//        proxAlim = i;
//      } else if ((now.hour() - guardaHora[i]) == (now.hour() - guardaHora[proxAlim])) {
//        if(guardaMin[proxAlim] <= now.minute()){
//          proxAlim = i;
//          continue;
//        }
//
//        if((guardaMin[i] - now.minute()) < (guardaMin[proxAlim] - now.minute())){
//          proxAlim = i;
//        }
//        if (guardaMin[i] > now.minute()) {
//          if ((guardaMin[i] - now.minute()) < (guardaMin[proxAlim] - now.minute()) ) {
//            proxAlim = i;
//          }
//        }
//      }
//    }

  for (l = 1; l <= c; l++) {
  lcd.setCursor(0, 0);
  //lcd.print(EEPROMReadInt(1));

  lcd.setCursor(2, 0);
  lcd.print("Hora:");
  lcd.setCursor(8, 0);
  hor1 = now.hour();

  if (hor1 < 10) {
    lcd.print("0");
    lcd.setCursor(9, 0);
  }
  lcd.print(hor1);
  lcd.setCursor(10, 0);
  lcd.print(":") ;
  lcd.setCursor(11, 0);
  min1 = now.minute();


  if (min1 < 10) {
    lcd.print("0");
    lcd.setCursor(12, 0);
  }
  lcd.print(min1);
  lcd.setCursor(13, 0);
  lcd.print(":") ;
  lcd.setCursor(14, 0);
  seg1 = now.second();


  if (seg1 < 10) {
    lcd.print("0");
    lcd.setCursor(15, 0);
  }
  lcd.print(seg1);


  lcd.setCursor(0, 1);
  //lcd.print("Horario#" + String(l));
  lcd.print("Prox alim:");
  lcd.setCursor(11, 1);
  if (guardaHora[l] < 10) {
    lcd.print("0");
    lcd.setCursor(12, 1);
  }

  lcd.print(guardaHora[l]);
  lcd.setCursor(13, 1);
  lcd.print(":") ;
  lcd.setCursor(14, 1);
  if (guardaMin[l] < 10) {
    lcd.print("0");
    lcd.setCursor(15, 1);
  }
  lcd.print(guardaMin[l]);
  delay(900);
  }
}


void msg() {

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Alimentador");
  lcd.setCursor(0, 1);
  lcd.print("   para  pets");
  delay(1200);
  //lcd.clear();

}

void EEPROMWriteInt(int address, int value) {
  byte hiByte = highByte(value);
  byte loByte = lowByte(value);

  EEPROM.write(address, hiByte);
  EEPROM.write(address + 1, loByte);
}

int EEPROMReadInt(int address) {
  byte hiByte = EEPROM.read(address);
  byte loByte = EEPROM.read(address + 1);

  return word(hiByte, loByte);
}



void escreveString(int enderecoBase, String mensagem) { // Salva a string nos endereços de forma sequencial
  if (mensagem.length() > EEPROM.length() || (enderecoBase + mensagem.length()) > EEPROM.length() ) { // verificamos se a string cabe na memória a partir do endereço desejado
    Serial.println ("A sua String não cabe na EEPROM"); // Caso não caiba mensagem de erro é mostrada
  }
  else { // Caso seja possível armazenar
    for (int i = 0; i < mensagem.length(); i++) {
      EEPROM.write(enderecoBase, mensagem[i]); // Escrevemos cada byte da string de forma sequencial na memória
      enderecoBase++; // Deslocamos endereço base em uma posição a cada byte salvo
    }
    EEPROM.write(enderecoBase, '\0'); // Salvamos marcador de fim da string
  }
}

String leString(int enderecoBase) {
  String mensagem = "";
  if (enderecoBase > EEPROM.length()) { // Se o endereço base for maior que o espaço de endereçamento da EEPROM retornamos uma string vazia
    return mensagem;
  }
  else { // Caso contrário, lemos byte a byte de cada endereço e montamos uma nova String
    char pos;
    do {
      pos = EEPROM.read(enderecoBase); // Leitura do byte com base na posição atual
      enderecoBase++; // A cada leitura incrementamos a posição a ser lida
      mensagem = mensagem + pos; // Montamos string de saídaa
    }
    while (pos != '\0'); // Fazemos isso até encontrar o marcador de fim de string
  }
  return mensagem; // Retorno da mensagem
}
