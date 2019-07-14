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



//---------configuração servo-----------------
Servo servo1;



//----------configuração relogio-----------------
RTC_DS1307 rtc;
int seg1, min1, hor1; 


//---------configuração teclado -----------------


const byte LINHAS = 4; //número de linhas do teclado
const byte COLUNAS = 3; //número de colunas do teclado

byte pinosLinhas [LINHAS] = {12,11,10,9}; //pinos que indicam as linhas do teclado

byte pinosColunas [COLUNAS] = {8,7,6};   //pinos que indicam as colunas do teclado


//define uma matriz com os símbolos que deseja ser lido do teclado
char teclas [LINHAS][COLUNAS] = { {'1', '2', '3'},
                                  {'4', '5', '6'},
                                  {'7', '8', '9'},
                                  {'*', '0', '#'}};

char entrada;

//instancia de Keypad, responsável por capturar a tecla pressionada
Keypad teclado = Keypad(makeKeymap(teclas), pinosLinhas, pinosColunas,LINHAS,COLUNAS);


//--------------configuração lcd ---------------------

int D4 = 2;                            //Declaração dos pinos para LCD
int D5 = 3;
int D6 = 4;
int D7 = 5;
int E = 1;                             //pino Enable
int RS = 0;                            //pino RS

LiquidCrystal lcd(RS,E,D4,D5,D6,D7);


//----------configuração buzzer e led------------

int led = A0;
int buzzer = A1;
int tempoBuzz = 200;






void setup() {
//-------------inicialização dos parametros e dispositivos--------

  pinMode(buzzer,OUTPUT);
  pinMode(led,OUTPUT);
  servo1.attach(pinServo);
  servo1.write(55);
  Wire.begin();
  lcd.begin(16, 2);
  rtc.begin();
  //rtc.adjust(DateTime(2018, 7, 13, 04, 13, 12));
  msg();
  racao();
horarios();

}

void loop() {

//--------------configuração inicial do despertador ------------


liberaRacao();
standby();



}

//-------------pega o horario para liberar comida
void horarios(){
  for(k = 1; k <= c; k++){
    erro =1;
    while(erro == 1){

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Informe horario ");                //pede para o usuario digitar a hora
      lcd.setCursor(0, 1); 
      lcd.print("#" + String(k) + " (hh:mm):");     //informa o formato que deve ser digitado a hora
      
      valor = 0;                                   //reseta o valor da variavel temporal


      for (i = 1; i <= 2; i++){                   //entrada dos digitos da hora
         entrada = teclado.waitForKey();
         digitalWrite(buzzer, HIGH);
         delay(tempoBuzz); 
         digitalWrite(buzzer, LOW);       
         valor = valor * 10 + (entrada - '0');    //acumula os digitos
         lcd.setCursor((i + 10), 1);
         lcd.print(entrada);              
         hor2 = valor;                            //A entrada dos dois dígitos é o valor por hora
        
          }

        lcd.setCursor((13), 1);                  //escreve o separador ":"
        lcd.print(":");

        valor = 0; 

        for (i = 1; i <= 2; i++){
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

          if (hor2 > 23 || min2 > 59){        //compara a hora errada
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

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("   Horario #" + String(k));
      lcd.setCursor(0, 1);
      lcd.print("    Guardado");
      delay(1200);    
    }  
  
  }




//-----------pega a quantidade de ração------------------
  
void racao(){

    erro = 1;
    while (erro == 1){
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Qtde de Racao");   //pede para inserir a qtde de ração por dia
      lcd.setCursor(0, 1);
      lcd.print("por dia? (1-2)");
      lcd.setCursor(11, 1);
      entrada = teclado.waitForKey();          //espera alguma tecla ser pressionada
      digitalWrite(buzzer, HIGH);              //aciona o buzzer para a tecla pressionada
      delay(tempoBuzz);                        //tempo do buzzer acionado
      digitalWrite(buzzer, LOW);                //desliga o buzzer
      lcd.print(entrada);                     //mostra o valor digitado nas teclas
      delay(100); 
      c = entrada - '0';                      //converte o valor de entrada e guarda na variavel C a "quantidade de vezes que deve alimentar o animal"


      if (c >= 1){                    //verificação de erro
         erro = 0;                    //se não houver erro marca como falso
         lcd.clear();
         lcd.setCursor(3, 0);
         lcd.print("Salvo");          //mostra a mensagem para o usuario que foi salvo
         lcd.setCursor(1, 1);
         lcd.print((String(c)) + " Racao");
         delay(1200);        
       }

       else{                            //caso tenha algum erro, marca como verdadeiro e repete o processo                       
        lcd.clear();      
        lcd.setCursor(0, 0);
        lcd.print(" Num 0 invalido");
        delay(1200);
        
        }       
     
      }   

    }

void liberaRacao(){
      lcd.clear();
      DateTime now = rtc.now();           //pega a hora atual
      hor1 = now.hour();                  //  atribui os valores das horas e minutos nas variaveis 
      min1 = now.minute();                //

      for (k = 1; k <= c; k++){           // verifica todos os horarios
        
         if ((hor1 == guardaHora[k]) && (min1 == guardaMin[k])){        //compara o horario atual com o horario que foi digitado para liberar a ração
               analogWrite(led, 255);                                   //liga o led, informando que esta sendo liberado a ração     
               lcd.setCursor(0,0);      
               lcd.write("Liberando");                                 //informa no lcd que esta liberando a ração
               servo1.write(100);                                      //faz o servo girar para 100°
               delay(6000);                                            //tempo que o servo fica para o angulo
               servo1.write(55);                                       //volta o servo para o inicio
               lcd.clear();
               analogWrite(led, 0);                                    //desliga o led
              
              

              while ((hor1 == guardaHora[k]) && (min1 == guardaMin[k])){        //condição para saber se continuamos ao mesmo tempo que foi liberado a ração

                standby();                                                      //evita que a ração seja liberada mais de uma vez no mesmo tempo
                
                } 
          }          
       } 
      }

void standby(){

        DateTime now = rtc.now();               //Se obtienen los valores de la hora y minutos actuales
        hor1 = now.hour();
        min1 = now.minute();
        seg1= now.second();

        for (l = 1; l <= c; l++){
          lcd.setCursor(0, 0);
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
          lcd.print("Horario#"+String(l));
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


void msg(){

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Alimentador");
  lcd.setCursor(0, 1);
  lcd.print("   para  pets");
  delay(1200);
   
}
        
  



  
