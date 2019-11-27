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
int tr=0,recep,tempsAvanSimulation=0;
int etatDeSimulation=0,decont=0,dureSim=10000;

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
  MsTimer2::set(1,IntrerrupTimer);//tout les  seconde
  MsTimer2::start();
  analogWriteFrequency(PWM_PIN, frequancePwm);
  analogWrite(20,pwm);
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


        switch (annaliseMessage(tr)) {
          case 0:
          Serial1.println("ereur mauvaise ortografe");
          break;
          case 1:
          Serial1.println("retoure ");
          Serial1.print("frequancePwm=");
          Serial1.println(frequancePwm);
          Serial1.print("duree de simulation");
          Serial1.println(dureSim);
          break;
          case 2:
          Serial1.print("frequancePwm=");
          Serial1.println(frequancePwm);
          break;
          case 3:
          Serial1.println("lansement de la simulation dans :");
          break;
          case 4:
          Serial1.print("duree de simulation");
          Serial1.println(dureSim);
        }
        tr=0;
        for(int i=0;i<50;i++)message[i]=0;
        MsTimer2::start();
      }
    }
  }
    analogWrite(PWM_PIN,pwm);
}
int annaliseMessage(int tr){
  const int  TAILLE_TABLEAU= 4;
  boolean espase=false,messageTrouver=true/*,messageChifre=true*/;
  int empEspase=0,mult=1,analiseLecture=0;
  byte numeDeMessage=0;
  String  mess[]={"reg","freq","Tau","dureSim"};

  for(int i=0;i<tr;i++){
    if(message[i]==' '){
      if(espase){
        Serial1.println("le mot que tu a remplie  nes pas dans la liste");
        for(int i=0;i<TAILLE_TABLEAU;i++)Serial1.println(mess[i]);
        Serial1.println("pour plus de fonction contacete le developeur par fax");
        return 0;
      }
      espase=!espase;
      empEspase=i;
    }
  }
  if(espase)analiseLecture=empEspase;
  else analiseLecture=tr;
  //annalise des caractere
  for(int i=0;i<tr;i++){
    if(!(!(message[i]<=47 || message[i]>=58)||!(message[i]<=64 || message[i]>=91)||!(message[i]<=96 || message[i]>=123)||message[i]==' ' )){
      Serial1.println("utiliser que des caractere qui sont comprie entre 65 et 90 ou 97 et 122  de la table ascii chifre ou espasse");
      return 0;
    }
    if((espase &&(i>empEspase))&&(message[i]<=47 || message[i]>=58)){
      Serial1.println("utiliser que des chifre apree le espase");
      Serial1.println("espesse de gros Burgonde tu na pas besoin de metre d'uniter ou en invanter je te rapelle que le debie ne sexprime pas en Hz (1/T) mais belle et bien L/s");
      return 0;
    }
  }
  //recherche de que message
  for(int j=0;j<TAILLE_TABLEAU;j++){
    messageTrouver=true;
    if(analiseLecture==(int)mess[j].length()){
      for(int i=0;i<(int)mess[j].length();i++){
        if(mess[j][i]!=message[i])messageTrouver=false;
      }
    }else messageTrouver=false;
    numeDeMessage=j;
    if(messageTrouver)break;
  }
  if(!messageTrouver)return 0;

  if(espase){
    if(!(empEspase<tr&&!(message[empEspase+2]<=47 || message[empEspase+2]>=58))){
      //messageChifre = false;
      Serial1.println(empEspase+1<tr);
      Serial1.println((message[empEspase+2]<=47 || message[empEspase+2]>=58));
      Serial1.println("Triple buse du doit maitre un chiffre appre le espase");
      return 0;
    }
  }

  switch (numeDeMessage) {
    case 0:
      return 1;
    break;
    case 1:
      if(espase){
        frequancePwm=0;
        for(int i=tr-1;i>empEspase;i--){
          frequancePwm+=(message[i]-48)*mult;
          mult*=10;
        }
        return 2;
      }else return 0;
    break;
    case 2:
      if(espase){
        decont=0;
        for(int i=tr-1;i>empEspase;i--){
          decont+=(message[i]-48)*mult;
          mult*=10;
        }
      }else decont=10000;
      time=0;
      etatDeSimulation=1;
      return 3;
    break;
    case 3:
      if(espase){
        dureSim=0;
        for(int i=tr-1;i>empEspase;i--){
          dureSim+=(message[i]-48)*mult;
          mult*=10;
        }
        dureSim*=1000;
        return 4;
      }
      Serial1.println("a moin que tu ne vienne du cosmose sinon tu nes pas sans savoire que tu doit metre un chifre pour undiquer la dure que tu veux");
      return 0;
    break;
  }
  return 0;
}
void IntrerrupTimer(){
  pas =nbPas;
  nbPas=0;
  time++;
  switch (etatDeSimulation) {
    case 1:
      if(decont<time){
        pwm=255;
      }
    break;
  }
}
void up(){
  if(contage){
    if(digitalRead(15))nbPas++;
    else nbPas--;
  }
}
