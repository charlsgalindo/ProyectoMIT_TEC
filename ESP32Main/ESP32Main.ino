//Integracion proyecto MIT:
//Meta: Control de NEMA17 con ESP32 desde AppInventor
//Carlos Galindo A01381750

#include <Stepper.h>
//Librería para control de steppers
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
//Librerías para establecer conexion con appinventor

#define IN1 14
#define IN2 15
#define IN3 16
#define IN4 17 //Motor 1 (Roller)

#define M2IN1 18
#define M2IN2 19
#define M2IN3 20
#define M2IN4 21 //Motor 2 (Extruder)

#define HeatSensor 10
#define WidthSensor 11
#define HotEnd 12

///////////////////////////////Variables////////////////////////////////

int motorResolution = 1; 
//Elegimos la presicion del motor, coordinar con equipo de CONTROL
//para obtener el tiempo de muestreo y los grados por step del motor.

double Velocidad = 60;
//Velocidad de guiro, controlada a tiempo real.

short direccionDeGiro = 1;
//Direccion de giro. 1= ClockWise, 0=CCW

int tiempoDeMuestreo = 100;
//Variable para aplicacion de PID. Tiempo de muestreo en MiliSegundos 
//entre cada ciclo de operacion del motor

// setting PWM properties
const int freq = 5000;
const int ledChannel = 0;
const int resolution = 8;

//////////////////////SET Functions //////////////////////////////////
int dutyCycle()
{
  return 120; //Hardcoded, will be fully populated when Websockets are implemented
}

int sampleTime()
{
  return 500; ////Hardcoded, will be fully populated when Websockets are implemented
}

///////////////////////////// Objetos //////////////////////////////////

Stepper roller(motorResolution, IN1, IN2, IN3, IN4);
Stepper extruder(motorResolution, M2IN1, M2IN2, M2IN3, M2IN4);
//Objeto de tipo "Stepper", instanciado con nuestros parametros de 
//resolucion (Grados/Paso), y puertos de salida

//////////////////////////// Prototipos ////////////////////////////////

void ControlClockWiseSpin(int, double, short, int);
//Prototipo de funcion para giro
 
int getResolution(void);
double getSpeed(void);
short getDirection(void);
int getSampleTime(void);
//Prototipos de funciones GET para parametros de giro


/////////////////////////// COMIENZO DE RUNTIME ////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

void setup()
{
  Serial.begin(115200);
  //Comunicacion con puerto serial COM4
   ledcSetup(ledChannel, freq, resolution); //pwm output
   ledcAttachPin(HotEnd, ledChannel);
}

void loop() 
{
  analogRead(HeatSensor);
  analogRead(WidthSensor);
  ledcWrite(ledChannel,dutyCycle());
  ControlRollerSpin(motorResolution,Velocidad,direccionDeGiro,tiempoDeMuestreo);
  ControlExtruderSpin(motorResolution,Velocidad,direccionDeGiro,tiempoDeMuestreo);
}

/////////////////////FIN DE RUNTIME /////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////


/*/////////////////////////////////////////////////////////
 * Nombre: ControlMotorSpin(int, double, short)
 * 
 * Descripcion: Esta funcion controla todos los parametros de giro del motor, a tiempo real.
 * 
 * Argumentos: 
 *    Primer int -> motorResolution (Resolucion de giro del motor, Degrees per step)
 *    Segundo double -> velocidad (Velocidad de giro del motor)
 *    tercer short -> direccion de giro (1=CW, 0=CCW)
 *    cuarto int -> Tiempo de muestreo (Duracion de cada ciclo, preferiblemente no moverlo en runtime)
 * 
 *////////////////////////////////////////////////////////////
void ControlRollerSpin(int resolution , double velocity , short spinDirection , int sampleTime)
{
  roller.setSpeed(velocity); 
  if(spinDirection == 1)  //Giro a favor de reloj
  {
   roller.step(resolution);
   delay(sampleTime);  
  } 
  else if(spinDirection == 0) //Giro Contrarreloj
  { 
   roller.step(-resolution);
   delay(sampleTime);  
  }
  else //Manejador de Errores 
  {
   roller.setSpeed(0); 
   roller.step(0);
  }
}

void ControlExtruderSpin(int resolution , double velocity , short spinDirection , int sampleTime)
{
  extruder.setSpeed(velocity); 
  if(spinDirection == 1)  //Giro a favor de reloj
  {
   extruder.step(resolution);
   delay(sampleTime);  
  } 
  else if(spinDirection == 0) //Giro Contrarreloj
  { 
   extruder.step(-resolution);
   delay(sampleTime);  
  }
  else //Manejador de Errores 
  {
   extruder.setSpeed(0); 
   extruder.step(0);
  }
}
