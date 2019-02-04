


/*Conexiones fisicas: 
 * pines analógicos A4 (SDA) y A5 (SCL).
 * VCC a 5V
 * GND a GND del arduino
 * */

#define printByte(args)  write(args); //para escribir iconos como clock, heart...

 
#include <Keypad.h>
#include <LiquidCrystal_I2C.h> 
#include <Wire.h>

LiquidCrystal_I2C lcd(0x27,16,2); //definimos la variable para nuestra pantalla


// ************************************** Teclado 3x3 **************************************************
  const byte filas = 3; //four rows
  const byte columnas = 3; //three columns
  byte pinsFilas[filas] = {4, 3, 2}; //connect to the row pinouts of the keypad
  byte pinsColumnas[columnas] = {7, 6, 5}; //connect to the column pinouts of the keypad

  char teclas[filas][columnas] = 
  {
    {'1','2','3'},
    {'4','5','6'},
    {'7','8','9'}/*,
    {'*','0','#'}*/
  };

  Keypad teclado = Keypad( makeKeymap(teclas), pinsFilas, pinsColumnas, filas, columnas );

uint8_t clock[8] = {0x0,0xe,0x15,0x17,0x11,0xe,0x0}; //para definir icono clock
char tecla;     //variable para almacenar la tecla pulsada

// ************************************** Teclado 3x3 **************************************************

int zumbador = 9 ;    //establecemos el pin Digital 9 al zumbador

int numero=0;   //definimos esta variable para tener un tipo de variable con el que operar
int contador=0; //variable para contar el número de columna/digitos máximo de la pantalla: 16

int horasTotal=0;                          // Definimos  las HORAS
int minutosTotal=15;                        // Definimos  los  MINUTOS
int segundosTotal=0;                       // Definimos  los  SEGUNDOS
int delayCountDown = 800;

boolean defuseando;
boolean activada=LOW;
int tiempoActivacion = 5; //tiempo necesario para activar/desactivar


int ahoras = 0;            //Variable a mostrar por LCD de las horas
int aminutos = 0;          //Variable a mostrar por LCD de los minutos
int asegundos = 0;         //Variable a mostrar por LCD de los segundos

const int passwordLength = 3;    // Definimos cuantos digitos queremos que tenga nuestra contraseña
int  currentLength        = 0;   // Define que numero de la contraseña estamos escribiendo actualmente sobre el teclado
char password[passwordLength]={1,2,3};   // Almacena la contraseña o codigo de ACTIVACION/DESACTIVACION   
char codIntro[passwordLength];   // Almacena los valores de los códigos introducidos 
char entered[3];                 // Almacena el codigo introducido por el teclado para compararlo con el almacenado en password[5]

int j = 0;

int bufferTiempo[3];

///////////////////////////////////// Variables Juego Zonas //////////////////////////////////////////////////////////////////////////

int tiempoDescuento=150; //dos minutos y medio antes de que empiece la partida
int aminutosZonas=0;
int asegundosZonas=0;

int z1=0;
int z=0;

int tiempoRojo=0;      //suma el tiempo del equipo Rojo
int tiempoAmarillo=0;  //suma el tiempo del equipo Amarillo
int tiempoTotal=0;     //Acumula el tiempo desde que se inicia el juego
int tiempoPartida=2700;  //Establece el tiempo total de la partida en segundos 2700 segundos es igual que 45 minutos

int botonRojo=10;     //establecemos el pin Digital 10 al botón Rojo
int botonRojoAnterior= 0;
int salidaRojo=0;

int botonAmarillo=11; //establecemos el pin Digital 9 al botón Amarillo
int botonAmarilloAnterior= 0;
int salidaAmarillo=0;

const int tiempoAntirebote=10;

int delayRojo=0;
int delayAmarillo=0;


//------------------------------------------------------------------------------------------------
//---------------------------------------------------- SETUP -------------------------------------
//------------------------------------------------------------------------------------------------
unsigned long x=0;
  int y=0;
void setup() {
  Serial.begin(9600); //iniciamos la comunicación serial con una tasa de 9600 bps

  pinMode (botonRojo, INPUT);
  pinMode (botonAmarillo, INPUT);
  pinMode (zumbador, OUTPUT); //establecemos el pin digital 9 como salida
  lcd.init();     //iniciamos la pantalla
  lcd.backlight(); //iniciamos el fondo retroiluminado

lcd.createChar(2, clock); //para definir clock
lcd.home();
     


  lcd.clear();lcd.setCursor(3,0);
  lcd.print("Paintball");
     lcd.setCursor(0,1);
  lcd.print("Calip");
 
  delay(1500);

  lcd.clear();
  lcd.print("Selecciona tipo");
  lcd.setCursor(0,1);
  lcd.print("de partida:");
  lcd.setCursor(0,2);
  delay(1500);

  lcd.clear();
  lcd.print("1. Bomba");//lcd.print("1. Bomba");
  lcd.setCursor(0,1);
  lcd.print("2. Zona");//lcd.print("2. Zonas");
  lcd.setCursor(0,1);
  delay(2000);
 
}

void loop() {
 
   x=millis();
Serial.println(x);
delay(1000);
Serial.println(y);
y++;

if (x==2000){

 
}
  seleccionpartida();
  
}


// ---------------------------- Funciones ----------------------------


void seleccionpartida(){
  botonRojo=digitalRead(10);
  botonAmarillo=digitalRead(11);

  
 // if (tecla=teclado.getKey()){ //almacena la tecla pulsada en variable: tecla
                      
        //if (tecla != NO_KEY){   //IF tecla pulsada. Si tecla es distinto de vacio/no tecla pulsada
          if (botonAmarillo==HIGH){numero=1;}
          else if(botonRojo==HIGH){
            numero=2;
          }
         //numero= tecla-48; //almacena y convierte (-48) la variable de tipo ASCII a Entero(int) para poder operar
            switch (numero) {
              case 1:
                lcd.clear();
                lcd.print("Juego Bomba");
                delay(1000);
                iniciobomba();                                   
               break;
              case 2:
                lcd.clear();
                lcd.print("Juego Zonas");
                delay(1000);
                inicioZonas();
                break;
              default:
                // statements
              break;
            }
        
         contador++;    //va contando la columna escrita
           
       //  } //IF tecla pulsada

}//Función: seleccionpartida()

//**************************************************** JUEGO BOMBA *************************************************************
void iniciobomba(){

   segundosTotal = segundosTotal + (minutosTotal * 60) + (horasTotal * 60 * 60);
   cuentaAtrasBomba();
   

} //Función: iniciobomba()


void cuentaAtrasBomba(){

   comprobarContrasena();    
  while (segundosTotal>0){ //mientras haya tiempo ir descontando 1 segundo

   ahoras = ((segundosTotal / 60)/ 60);   //Convertimos los segundos totales en horas
   aminutos = (segundosTotal / 60) % 60;  //Convertimos los segundos totales en minutos
   asegundos = segundosTotal % 60;        //Convertimos los segundos totales en periodos de 60 segundos
      
    
    segundosTotal--;
    ponerTiempoBomba();
    delay(1000);

//--------------- probando activacion----------------------------------
defuseando=digitalRead(10);
 //start disabling time
defuse(1);

//---------------------------------------------------

   if (segundosTotal==0)
    {
      bombaExplosion();
    }
  }
}//Función: cuentaAtrasBomba()


void cuentaAtrasActivada(){

  while (segundosTotal>0){ //mientras haya tiempo ir descontando 1 segundo

   ahoras = ((segundosTotal / 60)/ 60);   //Convertimos los segundos totales en horas
   aminutos = (segundosTotal / 60) % 60;  //Convertimos los segundos totales en minutos
   asegundos = segundosTotal % 60;        //Convertimos los segundos totales en periodos de 60 segundos
      
    
    segundosTotal--;
    ponerTiempoBomba();
    delay(1000);
    defuseando=digitalRead(10);
    defuse(2);
    }
}//Fin Función: cuentaAtrasActivada


void comprobarContrasena()
{
    if (tecla=teclado.getKey()){
      Serial.print(tecla);

      Serial.print(entered[j]);
      Serial.println();
      delay(1000);

        
        if (entered==password){
          bombaDesactivada();
        }
       j++;
    }  
} //Función: comprobarcontraseña()

void ponerTiempoBomba()
{  
     cls();
     if (activada==HIGH){
          lcd.setCursor(0,0); lcd.print("Explosion en:");
          pitidoSimple();
     } 
      else{
          lcd.setCursor(0,0); lcd.print("Tiempo de juego");
      }
          
          
     char bufferTiempo[9]=""; 
     sprintf(bufferTiempo, "%02d:%02d:%02d", ahoras, aminutos, asegundos); 
     lcd.setCursor(1,4); lcd.printByte(2); //escribe el icono clock en pantalla
     lcd.setCursor(4,4); lcd.print(bufferTiempo);               
}


void bombaExplosion()
{
    lcd.clear();
    cls();
    lcd.setCursor(6,0);
    lcd.print("BOMBA");
    lcd.setCursor(4, 1);
    lcd.print("DETONADA!");
    tone(11, 200, 2500);

  pitidoContinuo();
  delay(1000000);
}

void bombaDesactivada()
{
    lcd.clear();
    lcd.setCursor(3,0);
    lcd.print("BOMBA");
    lcd.setCursor(4, 1);
    lcd.print("DESACTIVADA");
    tone(11, 200, 2500);

    pitidoContinuo();
    delay(1000000);
}

void pitidoContinuo(){
  for (int i=0;i<1000;i++){       //FOR: para que emita un pitido cada vez que pulsamos una tecla
     digitalWrite(zumbador, HIGH);
     delayMicroseconds(500);
     digitalWrite(zumbador, LOW);
     delayMicroseconds(500);
  }
}

void pitidoSimple(){
  for (int i=0;i<1000;i++){       //FOR: para que emita un pitido cada vez que pulsamos una tecla
     digitalWrite(zumbador, HIGH);
     delayMicroseconds(50);
     digitalWrite(zumbador, LOW);
     delayMicroseconds(50);
  }
}

void leerCodigo(){

  if(j < passwordLength)
   {
    lcd.setCursor(1, 5+j);
    
    char key = teclado.getKey();
    
    key == NO_KEY;
    if (key != NO_KEY)
    {
      if ((key != '*')&&(key != '#'))
      { 
        lcd.print(key);
        codIntro[j] = key;
        j++;
        //tone1.play(NOTE_C6, 200);
      }
    }
  }
}//Función: leerCodigo


//**************************************************** JUEGO ZONAS *************************************************************
void inicioZonas(){

  int cuentaPitidoRojo=0;
  int cuentaPitidoAmarillo=0;
  // tiempoRojo
  
  cuentaAtrasZonas();
  lcd.clear();
                
  ponerTiempoRojo();
  ponerTiempoAmarillo();
  
  do {
     //----------------------------------- Rojo 
      //botonRojo = digitalRead(10); //Lee el estado del botonrRojo por si es el primero en ser pulsado
      for (int r=0; r<20;r++){
            if (rojo()){goto marcaRojo;}
           }

      
      if(botonRojo!=botonRojoAnterior){ //antirebote
        if(antiRebote(10)){ //antirebote
      for (z;(botonRojo==HIGH) && (botonRojoAnterior ==LOW) && (tiempoTotal<tiempoPartida);z++){
     //if (botonRojo==HIGH){
        salidaRojo=1-salidaRojo;
       
        marcaRojo:
        if (cuentaPitidoRojo<1){
                pitidoSimple();
                cuentaPitidoRojo=1;
        }
          cuentaPitidoAmarillo=0;
           tiempoRojo++;
           tiempoTotal++;
           //delay(1000);
           ponerTiempoRojo();
           
           for (int a=0; a<20;a++){
            if (amarillo()){goto marcaAmarillo;}
           }
           /* ;if (amarillo()){goto marcaAmarillo;};if (amarillo()){goto marcaAmarillo;};if (amarillo()){goto marcaAmarillo;};if (amarillo()){goto marcaAmarillo;};
           }
           if (amarillo()){goto marcaAmarillo;};if (amarillo()){goto marcaAmarillo;};if (amarillo()){goto marcaAmarillo;};if (amarillo()){goto marcaAmarillo;};if (amarillo()){goto marcaAmarillo;};
           if (amarillo()){goto marcaAmarillo;};if (amarillo()){goto marcaAmarillo;};if (amarillo()){goto marcaAmarillo;};if (amarillo()){goto marcaAmarillo;};if (amarillo()){goto marcaAmarillo;};
           if (amarillo()){goto marcaAmarillo;};if (amarillo()){goto marcaAmarillo;};if (amarillo()){goto marcaAmarillo;};if (amarillo()){goto marcaAmarillo;};if (amarillo()){goto marcaAmarillo;};*/

           /*if(botonAmarillo==HIGH){
            goto marcaAmarillo;
           }*/
           
      }//Fin For ROjo
      }}//antirebote fin if´s
      botonRojoAnterior=botonRojo;     
  //----------- Fin Rojo
  
  //----------- Amarillo
      //botonAmarillo = digitalRead(11); //Lee el estado del botonAmarillo por si es el primero en ser pulsado
       for (int a=0; a<20;a++){
            if (amarillo()){goto marcaAmarillo;}
       }

      
      for (z1;(botonAmarillo==HIGH) && (botonAmarilloAnterior ==LOW) && (tiempoTotal<tiempoPartida) ;z1++){
    //   if ((botonAmarillo==HIGH) && (botonAmarilloAnterior ==LOW)){
        salidaAmarillo=1-salidaAmarillo;
       
        marcaAmarillo:

        if (cuentaPitidoAmarillo<1){
                pitidoSimple();
                cuentaPitidoAmarillo=1;
        }
        cuentaPitidoRojo=0;
           tiempoAmarillo++;
           tiempoTotal++;
           //delay(1000);
           ponerTiempoAmarillo();

           for (int r=0; r<20;r++){
            if (rojo()){goto marcaRojo;}
           }

          /* if (rojo()== HIGH){goto marcaRojo;};if (rojo()){goto marcaRojo;};if (rojo()){goto marcaRojo;};if (rojo()){goto marcaRojo;};if (rojo()){goto marcaRojo;};
           if (rojo()){goto marcaRojo;};if (rojo()){goto marcaRojo;};if (rojo()){goto marcaRojo;};if (rojo()){goto marcaRojo;};if (rojo()){goto marcaRojo;};
           if (rojo()){goto marcaRojo;};if (rojo()){goto marcaRojo;};if (rojo()){goto marcaRojo;};if (rojo()){goto marcaRojo;};if (rojo()){goto marcaRojo;};
           if (rojo()){goto marcaRojo;};if (rojo()){goto marcaRojo;};if (rojo()){goto marcaRojo;};if (rojo()){goto marcaRojo;};if (rojo()){goto marcaRojo;};*/

                     
           //botonRojo = digitalRead(10);
          /* if(botonRojo==HIGH){
            goto marcaRojo;
           }*/
      }     
   //----------- Fin Amarillo
    
     tiempoTotal++; //este contador solo suma mientras no se haya entrado en el bucle de algún botón
     delay(1000);
     
    }while(tiempoTotal<tiempoPartida);//Fin while. Se ejecuta mientras el tiempoTotal (que está en cuenta atrás) sea inferior a tiempoPartida (el establecido para jugar)
    //Termina el tiempo de partida y se paralizan tiempos de Rojo y Amarillo
 
  lcd.setCursor(15,0); lcd.printByte(2); //escribe el icono clock en pantalla 
  lcd.setCursor(15,1); lcd.printByte(2); //escribe el icono clock en pantalla
  pitidoContinuo();
  delay(1000000);

} //Fin función: inicioZonas()

void cuentaAtrasZonas(){
  
  while (tiempoDescuento>0){ //mientras haya tiempo ir descontando 1 segundo

   aminutosZonas = (tiempoDescuento / 60) % 60;  //Convertimos los segundos totales en minutos
   asegundosZonas = tiempoDescuento % 60;        //Convertimos los segundos totales en periodos de 60 segundos
          
    tiempoDescuento--;
    ponerTiempoZonas();
    delay(1000);
  }
  pitidoSimple();delay(300);pitidoSimple();
  
}//Función: cuentaAtrasZonas()

void ponerTiempoZonas()
{  
     lcd.setCursor(0,0); lcd.print("Fuego en.. "); 
     char bufferTiempo[9]=""; 
     sprintf(bufferTiempo, "%02d:%02d", aminutosZonas, asegundosZonas); 
     lcd.print(bufferTiempo);          
}

void ponerTiempoRojo()
{  
     lcd.setCursor(0,0); lcd.print("Rojo     "); 
     char bufferTiempo[6]=""; 
     sprintf(bufferTiempo, "%02d:%02d", (tiempoRojo/60)%60, (tiempoRojo%60)); 
     lcd.print(bufferTiempo);             
}

void ponerTiempoAmarillo()
{  
     lcd.setCursor(0,1); lcd.print("Amarillo "); 
     char bufferTiempo[6]=""; 
     sprintf(bufferTiempo, "%02d:%02d", (tiempoAmarillo/60)%60, (tiempoAmarillo%60)); 
     lcd.print(bufferTiempo);             
}

boolean antiRebote (int pin){
  int contador=0;
  boolean estado;         //Guarda el estado del botón
  boolean estadoAnterior; //Guarda el último estado del botón

  do {
    estado=digitalRead(pin);
    if(estado != estadoAnterior){
      contador=0;
      estadoAnterior = estado;
    }
    else{
        contador++;
    }
    delay(1);
    
  }while (contador < tiempoAntirebote);
  return estado; //devuelve un HIGH o LOW
} //Fin función antiRebote

boolean rojo(){
delay(50);
delayRojo=delayRojo+50;
           botonRojo = digitalRead(10);

           if(botonRojo==HIGH){
              if (delayRojo<1000){
                delay(1000-delayRojo);
                delayRojo=0;
                
              }
                 
            
           }
           return botonRojo;
           
}

boolean amarillo(){
delay(50);
delayAmarillo=delayAmarillo+50;
           botonAmarillo = digitalRead(11);

           if(botonAmarillo==HIGH){

             if (delayAmarillo<1000){
                delay(1000-delayAmarillo);
                delayAmarillo=0;
                
              }
          //  goto marcaAmarillo;
            
           }
           return botonAmarillo;
           
}


  



