#include <base-CC.h>
//#include <String>
int nbPas=0;
int pwm=0;
int frequancePwm=30000;
boolean contage=false;
int time=0;
int pas;
byte resultat[30000];
int message[50];
int tr=0,recep;



int interpretation(int caractere){

  int interprete;
  switch (caractere) {
    case (int)'0':
    interprete=0;
    break;
    case (int)'1':
    interprete=1;
    break;
    case (int)'2':
    interprete=2;
    break;
    case (int)'3':
    interprete=3;
    break;
    case (int)'4':
    interprete=4;
    break;
    case (int)'5':
    interprete=5;
    break;
    case (int)'6':
    interprete=6;
    break;
    case (int)'7':
    interprete=7;
    break;
    case (int)'8':
    interprete=8;
    break;
    case (int)'9':
    interprete=9;
    break;
    case 0x0D:
    interprete=0x0D;
    break;
  }
  return interprete;
}

void setup() {
  resultat[1]=10;
  // put your setup code here, to run once:
  pinMode(15,INPUT_PULLUP);//trigeur ap2
  pinMode(14,INPUT_PULLUP);//ap2
  pinMode(20,OUTPUT);
  pinMode(19,OUTPUT);
  digitalWrite(19,HIGH);
  pinMode(13,OUTPUT);
  digitalWrite(13,HIGH);
  attachInterrupt(digitalPinToInterrupt(14),up,RISING);
  Serial1.begin(115200);
  Serial.begin(115200);

  Serial1.println("teset 1");
  MsTimer2::set(10,IntrerrupTimer);//tout les  seconde
  MsTimer2::start();
  analogWriteFrequency(PWM_PIN, frequancePwm);
  //analogWrite(20,128);
}

void loop() {
  if ( Serial1.available() ) {
    MsTimer2::stop();
    recep=Serial1.read();
    if(recep!=0x0D){
      message[tr]=recep;
      tr++;
    }else{
      if(tr!=0){
        for(int i=0;i<tr;i++){
          Serial1.print((char)message[i]);
        }
        Serial1.println();


        switch (typeMessage(tr)) {
          case 0:
          Serial1.println("ereur mauvaise ortografe");
          break;
          case 1:
          Serial1.println("retoure ");
          Serial1.print("frequancePwm=");
          Serial1.println(frequancePwm);
          break;
          case 2:
          Serial1.print("frequancePwm=");
          Serial1.println(frequancePwm);
        }
        tr=0;
        MsTimer2::start();
      }
    }
  }
    analogWrite(PWM_PIN,pwm);
}
int typeMessage(int tr){
  boolean espase=false,messageTrouver=true;
  int empEspase=0,mult=1;
  String reg="reg",frece="frec";

  for(int i=0;i<tr;i++){
    if(message[i]==' '){
      if(espase)return 0;
      espase=!espase;
      empEspase=i;
    }
  }
  //annalise des caractere
  for(int i=0;i<tr;i++){
    if(((message[i]<=48 && message[i]>=57)||(message[i]<=65 && message[i]>=90)||(message[i]<=97 && message[i]>=122)|| ! (message[i]==' '))){
      return 0;
    }
    if(espase &&(i>empEspase)&&(message[i]<=48 && message[i]>=57)){
      return 0;
    }
  }
  Serial1.println("truc truc");
  //recherche reg
  for(int i=0;i<3;i++){
    if(reg[i]!=message[i])messageTrouver=false;
  }
  if(messageTrouver&&tr<=4)return 1;
  else messageTrouver=true;
  //recherche frec
  for(int i=0;i<4;i++){
    if(frece[i]!=message[i])messageTrouver=false;
  }
  if(messageTrouver&&empEspase==4){
    frequancePwm=0;
    for(int i=tr-1;i>empEspase;i--){
      frequancePwm+=(message[i]-48)*mult;
      mult*=10;
    }
return 2;
  }

  else messageTrouver=true;
  return 0;
}
void IntrerrupTimer(){
  pas =nbPas;
  nbPas=0;
  time++;
  if(time==10){
    pwm=255;
  }
}
void up(){
  if(contage){
    if(digitalRead(15))nbPas++;
    else nbPas--;
  }
}
