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
#define IN2 27
#define IN3 26
#define IN4 25
//Puertos de salida para ESP32

///////////////////////////////Variables////////////////////////////////

MDNSResponder mdns;
//Objeto de tipo DNS Responder para conexion por WebSockets

WebServer server(80);
String webpage = "";
//Objeto de tipo WebServer para establecimiento de conexion con servidor WIFI
//y declaracion de variable tipo string que contiene la direccion de la pagina

const char* ssid = "REPLACE_WITH_YOUR_SSID";
const char* password = "REPLACE_WITH_YOUR_PASSWORD";
//Credenciales Wifi, declaradas como apuntador y accedidas como paso por referencia

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
 
int getResolution(void);
double getSpeed(void);
short getDirection(void);
int getSampleTime(void);
//Prototipos de funciones GET para parametros de giro


/////////////////////////// COMIENZO DE RUNTIME ////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

void setup()
{
  motor.setSpeed(60); 
  //Velocidad inicial, redefinida en runtime
  
  Serial.begin(115200);
  //Comunicacion con puerto serial COM4
  
  webpage = "<h1>ESP32 Web Server</h1><p>GetDirection<a href=\"ClockWise\"><button>ON</button></a>&nbsp;<a href=\"CounterClockWise\"><button>OFF</button></a></p>";
  // TO-DO: webpage += Agregando valores tipo doble, int (arquitectura de 32bit) y short

  WiFi.begin(ssid, password);
  //Inicializamos la conexion por WiFi

  WiFi.begin(ssid, password);
  Serial.println("");
  while (WiFi.status() != WL_CONNECTED)
    {
    delay(500);
    Serial.print(".");
    }//Mostramos que la conexion esta en proceso. 
   Serial.print("IP address: ");
   Serial.println(WiFi.localIP());
   //Imprimimos la IP a la que nos conectamos
   if (mdns.begin("esp")) 
   {
    Serial.println("MDNS responder started");
   }
   //Confirmamos la conexion DNS
}

void loop() 
{
  server.handleClient();
  motorResolution = getResolution();
  Velocidad = getSpeed();
  direccionDeGiro = getDirection();
  tiempoDeMuestreo = getSampleTime();
  ControlMotorSpin(motorResolution,Velocidad,direccionDeGiro,tiempoDeMuestreo);
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

/*///////////////////////////////////////////////////////
 * Nombre: getResolution()
 * 
 * Descripcion: Obtiene la Resolucion (Grados/Step) del WebSocket del MIT App Inventor
 * 
 * Argumentos: Retorna un valor tipo int
 *//////////////////////////////////////////////////////

int getResolution(void)
{
  return(1); //TO-DO: tuvo que comentarse por fallos en la comunicacion
}

/*///////////////////////////////////////////////////////
 * Nombre: getSpeed()
 * 
 * Descripcion: Obtiene la velocidad de giro del WebSocket del MIT App Inventor
 * 
 * Argumentos: Retorna un valor tipo double
 *//////////////////////////////////////////////////////

double getSpeed(void)
{
  return(1.0); //TO-DO: tuvo que comentarse por fallos en la comunicacion
}

/*///////////////////////////////////////////////////////
 * Nombre: getDirection()
 * 
 * Descripcion: Obtiene la direccion de giro (CW/CCW) del WebSocket del MIT App Inventor
 * 
 * Argumentos: Retorna un valor tipo short
 *//////////////////////////////////////////////////////

short getDirection(void)
{
  server.on("/ClockWise", []()
  {
      return(1);
  });
  server.on("/CounterClockWise", []()
  {
      return(0);
  });
}

/*///////////////////////////////////////////////////////
 * Nombre: getSampleTime()
 * 
 * Descripcion: Obtiene el tiempo de muestreo en mS del WebSocket del MIT App Inventor
 * 
 * Argumentos: Retorna un valor tipo int
 *//////////////////////////////////////////////////////

int getSampleTime(void)
{
  return(1); //TO-DO: tuvo que comentarse por fallos en la comunicacion
}
