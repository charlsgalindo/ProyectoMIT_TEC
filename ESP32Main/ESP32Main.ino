//Integracion proyecto MIT:
//Meta: Control de NEMA17 con ESP32 desde AppInventor
//Carlos Galindo A01381750

#include <Stepper.h>
//Librería para control de steppers
#define IN1 14
#define IN2 27
#define IN3 26
#define IN4 25
//Puertos de salida para ESP32

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

///////////////////////////// Objetos //////////////////////////////////

Stepper motor(motorResolution, IN1, IN2, IN3, IN4);
//Objeto de tipo "Stepper", instanciado con nuestros parametros de 
//resolucion (Grados/Paso), y puertos de salida

//////////////////////////// Prototipos ////////////////////////////////

void ControlClockWiseSpin(int, double, short, int);
//Prototipo de funcion para giro 

/////////////////////////// COMIENZO DE RUNTIME //////////////////////////

void setup()
{
  motor.setSpeed(60); 
  //Velocidad inicial, redefinida en runtime
  
  Serial.begin(115200);
  //Comunicacion con puerto serial COM4
}

void loop() 
{
  
  Serial.println("Rotating Clockwise...");
  motor.step(motorResolution);
  delay(500);

  Serial.println("Rotating Anti-clockwise...");
  motor.step(-motorResolution);
  delay(500);
  
}

/////////////////////FIN DE RUNTIME //////////////////////


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
void ControlMotorSpin(int resolution , double velocity , short spinDirection , int sampleTime)
{
  motor.setSpeed(velocity); 
  if(spinDirection == 1)  //Giro a favor de reloj
  {
   motor.step(resolution);
   delay(sampleTime);  
  } 
  else if(spinDirection == 0) //Giro Contrarreloj
  { 
   motor.step(-resolution);
   delay(sampleTime);  
  }
  else //Manejador de Errores 
  {
   motor.setSpeed(0); 
   motor.step(0);
  }
}
