#include <SoftwareSerial.h>

SoftwareSerial phMeter(50,51); //RX, TX

const int messageIndex = 18; //Stores the number of characters sent by ph meter to be saved in the vector
const int unitaryIndex = 0;
const int tenthIndex = 2;
const int hundredthIndex = 3;

int i = 0; //vector's index
int j = 0; //testing index
int index = 0; //excel index
long lastReadingTime = 0; //Stotes the last reading time
long instantTime = 0; //Stores current time
double ph; //stores ph
double temperature; //stores temperature
char message[messageIndex]; //Vector of char that stores the message sent by ph 

double CharToInt_Ascii(char character){
  double number;
  if(isAscii(character)){ //Check if character is an Ascii element
    //Serial.println("Elemento Ascii");
  }
  else{
    Serial.println("Elemento nao eh um Ascii");
    return 0;
  }

  if(((double(character)) < 48.00) && ((double(character)) > 57.00)){ //Check if Ascii element is a number(0 to 9, represented by 48 to 57) 
    Serial.println("Elemento Ascii nao eh um numero");
    return 0;
  }
  number = double(character) - 48; //In Ascii, the characters 0 to 9 are represented in decimal by 48 to 57
  return number;
}

void setup() {
  Serial.begin(9600); //Arduino's set up
  phMeter.begin(9600); //Arduino's set up
  Serial.println("CLEARDATA"); //Reset serial communication
  Serial.println("LABEL,Time,Ph,Temperature,Index");
}

void loop() {
  if (phMeter.available()){
    message[i] = phMeter.read();//Reads each character sent by ph meter
    lastReadingTime = millis();//Gets the last reading time
    i++;
  }
  instantTime = millis();//Gets current time
  
  if((instantTime - lastReadingTime)>1000){ //Calculates if ph meter stopped to send measures to send new ones(this ph meter sends new measures every two seconds)
      if((message[2]) == '.'){
        ph = CharToInt_Ascii(message[0])*10 + CharToInt_Ascii(message[1]) + CharToInt_Ascii(message[3])/10 + CharToInt_Ascii(message[4])/100; //Gets ph
      }
      else if((message[1]) == '.'){
        ph = CharToInt_Ascii(message[0]) + CharToInt_Ascii(message[2])/10 + CharToInt_Ascii(message[3])/100; //Gets ph
      }

      //Temperatura
      if((message[9]) == '.'){
        temperature = CharToInt_Ascii(message[7])*10 + CharToInt_Ascii(message[8]) + CharToInt_Ascii(message[10])/10; //Gets temperature
      }
      else if((message[10]) == '.'){
        temperature = CharToInt_Ascii(message[8])*10 + CharToInt_Ascii(message[9]) + CharToInt_Ascii(message[11])/10; //Gets temperature
      }
      if(i != 0){
        Serial.println((String)"DATA,TIME," + ph + "," + temperature + "," + index); //print out of data
        index++;
      }
      i = 0;//Cleares the vector index to save the new measures in the vector
      if(index > 100){
        Serial.println("SAVEWORKBOOKAS,GraficosPh\\Teste"); //Save data
        Serial.println("FORCEEXCELQUIT"); //Close excel
        index = 0; //Restart index
      }
  }
  
}
