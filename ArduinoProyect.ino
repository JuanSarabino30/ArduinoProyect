/*
  LiquidCrystal Library - Hello World

 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.

 This sketch prints "Hello World!" to the LCD
 and shows the time.

  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)

 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 22 Nov 2010
 by Tom Igoe
 modified 7 Nov 2016
 by Arturo Guadalupi

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/LiquidCrystalHelloWorld

*/
//Juan Camilo Sarabino Alegria

// include the library code:
#include <LiquidCrystal.h>
#include <Keypad.h>


//KeyPad
const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] =
{

  { '1', '2', '3'},
  { '4', '5', '6'},
  { '7', '8', '9'},
  { '*', '0', '#'},

};
byte rowPins[ROWS] = {5, 4, 3, 2}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {8, 7, 6}; //connect to the column pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );//Inicializo el teclado con el numero de filas, columnas, los pines del Arduino utilizados y la matriz


//LCD interface pin
const int rs = 12, en = 11, d4 = 31, d5 = 32, d6 = 33, d7 = 34;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


// Define Pins_Led
#define LED_BLUE 35
#define LED_GREEN 36
#define LED_RED 37


//Variables
int posicion=0;    // necesaria para la clave
int cursor=5;      // posicion inicial de la clave en el LCD
int clave=0;       // para el LCD
int intentos = 0;
char password[4] = {'2','0','2','2'};


void setup() {

  //Estados de los LEDS
  pinMode(LED_BLUE, OUTPUT);  
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);

  //Secuencia inicial
  //VERDE
  digitalWrite(LED_BLUE, LOW);
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_RED, LOW);
  delay(500);
  
  //AZUL
  digitalWrite(LED_BLUE, HIGH);
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_RED, LOW);
  delay(500);
  
  //ROJO
  digitalWrite(LED_BLUE, LOW);
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_RED, HIGH);
  delay(500);

  digitalWrite(LED_BLUE, LOW);
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_RED, LOW);
  
  
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  
  // Print a message to the LCD.
  lcd.setCursor(0, 0);
  lcd.print("---Bienvenido---");
  delay(1500);
  lcd.clear();

  //Mensaje para pedir clave
  lcd.setCursor(0, 0);
  lcd.print("Ingrese la clave:");
  lcd.setCursor(cursor,1); // cursor en la posicion de la variable, linea 1
  
}

void loop() {
  if(intentos == 3){
    digitalWrite(LED_BLUE, LOW);
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_RED, HIGH);

    lcd.clear();
    lcd.setCursor(0,0);// situamos el cursor el la pos 0 de la linea 0.
    lcd.print("Bloqueado");// escribimos en LCD
    delay(2000);
    lcd.setCursor(0,1);// situamos el cursor el la pos 0 de la linea 0.
    lcd.print("Reiniciando...");// escribimos en LCD
    delay(2000);
    intentos = 0;

    setup();
    
  }else{
    char key = keypad.getKey(); //Almaceno en una variable la tecla presionada

    if (key != 0){ //Si el valor es 0 es que no se ha pulsado ninguna tecla 
    
      if (key != '#' && key != '*' && clave==0){// descartamos almohadilla y asterisco
        lcd.print(key); // imprimimos la tecla
        cursor++;// incrementamos el cursor

        //--- Condicionales para comprobar la clave introducida -----------
        // comparamos entrada con cada uno de los digitos, uno a uno
        if (key == password[posicion]){
            posicion ++; // aumentamos posicion si es correcto el digito
        }

        //--- En el caso de que se ha introducido los 4 correctamente ----------
        if (posicion == 4){
          lcd.clear();
          lcd.setCursor(0,0);      // situamos el cursor el la pos 0 de la linea 0.
          lcd.print("Clave correcta");// escribimos en LCD

          posicion = 0;// reiniciamos la clave introducida
          cursor = 5;// reiniciamos el cursor
          intentos = 0;// reiniciamos los intentos
          //clave=1; // indicamos que se ha introducido la clave, sirve para proximas condiciones
        
          // encendemos LED_GREEN
          digitalWrite(LED_BLUE, LOW);
          digitalWrite(LED_GREEN, HIGH);
          digitalWrite(LED_RED, LOW);
          delay(3000);

          lcd.setCursor(0,1);// situamos el cursor el la pos 0 de la linea 0.
          lcd.print("Reiniciando...");// escribimos en LCD
          delay(500);
          
          setup();
        }
     
        //--- En el caso de que este incompleta o no hayamos acertado ----------
        if(cursor>9){ // comprobamos que no pase de la cuarta posicion
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Clave incorrecta");
            digitalWrite(LED_BLUE, HIGH);
            digitalWrite(LED_GREEN, LOW);
            digitalWrite(LED_RED, LOW);
            delay(1500);
            digitalWrite(LED_BLUE, LOW);
            digitalWrite(LED_GREEN, LOW);
            digitalWrite(LED_RED, LOW);
            
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Ingrese la clave:");
          
            cursor=5;// lo volvemos a colocar al inicio
            posicion=0;// borramos clave introducida
            lcd.setCursor(cursor,1); // cursor en la posicion de la variable, linea 1
          
            if(clave==0){ // comprobamos que no hemos acertado
              intentos ++;
            }
        }
      }
    }
  }
}
