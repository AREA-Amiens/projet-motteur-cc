#include <Arduino.h>
#include <MsTimer2.h>

#define PWM_PIN 20

int typeMessage(int tr);/*
retourne
0 si message invalide
1 si regarder les variable (reg)
2 si changement de pwm (frecp)
3 si changement d'echantionage (frece)
*/
void IntrerrupTimer();/*
raz des pas fait
remplissage du tablau pour lanvoie
pour les aiser de constante de temps rajout du pwm ou d'un conteur
*/
void up();/*
incrémente ou decremente le nbPaq
sulement si contage
*/
