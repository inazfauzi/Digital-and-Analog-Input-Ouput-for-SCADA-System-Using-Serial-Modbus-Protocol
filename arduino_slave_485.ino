/**
 *  Modbus slave example 2:
 *  The purpose of this example is to link the Arduino digital and analog
 *  pins to an external device.
 *
 *  Recommended Modbus Master: QModbus
 *  http://qmodbus.sourceforge.net/
 *
 *  Editado al español por LuxARTS
 */

//Include library protokol Modbus
#include <ModbusRtu.h>
#define ID   1
#define LEDPin 13

//Buat Contoh
Modbus slave(ID, Serial, 4); //ID simpul. 0 untuk master, 1-247 untuk slave
                        // Port serial (0 = TX: 1 - RX: 0)
                        // Protokol serial. 0 untuk RS-232 + USB (default), pin apa pun yang lebih besar dari 1 untuk RS-485
boolean led;
int8_t state = 0;
unsigned long tempus;

uint16_t au16data[9]; //Tabel catatan yang ingin Anda bagikan melalui jaringan

/*********************************************************
 Configuración del programa
*********************************************************/
void setup() {
  io_setup(); //konfigurasikan input dan output

  Serial.begin(9600); //Buka komunikasi sebagai budak
  slave.start();
  tempus = millis() + 100; //Hemat waktu saat ini + 100ms
  digitalWrite(13, HIGH ); //Nyalakan led pin 13 (yang ada di board)
}

/*********************************************************
 Inicio del programa
*********************************************************/
void loop() {
  //Periksa buffer masukan
  state = slave.poll( au16data, 9 ); //Parameter: Tabel register untuk pertukaran info
                                     // Ukuran tabel record
                                     // Mengembalikan 0 jika tidak ada permintaan data
                                     // Mengembalikan 1 hingga 4 jika ada kesalahan komunikasi
                                     // Mengembalikan lebih dari 4 jika pesanan diproses dengan benar

  if (state > 4) { //Jika lebih besar dari 4 = urutannya benar
    tempus = millis() + 50; //Waktu saat ini + 50ms
    digitalWrite(13, HIGH);//Menyalakan LED
  }
  if (millis() > tempus) digitalWrite(13, LOW );//Matikan led 50ms kemudian
  
  // Perbarui pin Arduino dengan papan Modbus
  io_poll();
} 

/**
 * pin maping:
 * 2 - digital input
 * 3 - digital input
 * 4 - digital input
 * 5 - digital input
 * 6 - digital output
 * 7 - digital output
 * 8 - digital output
 * 9 - digital output
 * 10 - analog output
 * 11 - analog output
 * 14 - analog input
 * 15 - analog input
 *
 * pin 13 reservado para ver el estado de la comunicación
 */
void io_setup() {
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  //pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
//  pinMode(A2, INPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(LEDPin, OUTPUT);

  digitalWrite(6, LOW );
  digitalWrite(7, LOW );
  digitalWrite(8, LOW );
  digitalWrite(9, LOW );
  digitalWrite(LEDPin, LOW ); //Led del pin 13 de la placa
//  analogWrite(27, 0 ); //PWM 0%
//  analogWrite(26, 0 ); //PWM 0%
  
}

/*********************************************************
Enlaza la tabla de registros con los pines
*********************************************************/
void io_poll() {
  // digital inputs -> au16data[0]
  // Membaca input digital dan menyimpannya dalam bit dari variabel pertama vektor
  // (sama seperti membuat topeng)
//  bitWrite( au16data[0], 0, digitalRead( A0 )); //Lee el pin 2 de Arduino y lo guarda en el bit 0 de la variable au16data[0] 
//  bitWrite( au16data[0], 1, digitalRead( A1 ));
  //bitWrite( au16data[0], 2, digitalRead( 4 ));
//  bitWrite( au16data[0], 3, digitalRead( A2 ));

  // digital outputs -> au16data[1]
  // Lee los bits de la segunda variable y los pone en las salidas digitales
  digitalWrite( 6, bitRead( au16data[1], 0 )); //Lee el bit 0 de la variable au16data[1] y lo pone en el pin 6 de Arduino
  digitalWrite( 7, bitRead( au16data[1], 1 ));
  digitalWrite( 8, bitRead( au16data[1], 2 ));
  digitalWrite( 9, bitRead( au16data[1], 3 ));
  digitalWrite( LEDPin, bitRead( au16data[1], 4 ));

  // Cambia el valor del PWM
//  analogWrite( 27, au16data[5] ); //El valor de au16data[2] se escribe en la salida de PWM del pin 10 de Arduino. (siendo 0=0% y 255=100%)
//  analogWrite( 26, au16data[8] );
 
  

  // Lee las entradas analógicas (ADC)
  au16data[4] = analogRead(A0);//analogRead( 0 ); //El valor analógico leido en el pin A0 se guarda en au16data[4]. (siendo 0=0v y 1023=5v)
  //au16data[5] = analogRead(300);//analogRead( 1 );

  // Diagnóstico de la comunicación (para debug)
  au16data[6] = slave.getInCnt();  //Devuelve cuantos mensajes se recibieron
  au16data[7] = slave.getOutCnt(); //Devuelve cuantos mensajes se transmitieron
  au16data[8] = slave.getErrCnt(); //Devuelve cuantos errores hubieron
}
