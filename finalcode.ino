const byte npulse = 15; // number of pulses to charge the capacitor before each measurement
 
const byte pin_pulse = A0; // sends pulses from arduino to charge the capacitor (can be a digital pin)
const byte pin_cap  = A1; // goes to arduino, measures the capacitor charge
const byte pin_LED = 53; // LED that turns on when metal is detected
const byte pin_LED2= 41;

int S = 0;
int nbcars = 0;

int t = 5; //variable temps

//pins du chiffre des unite
int pinA1 = 26;
int pinB1 = 40;
int pinC1 = 48;
int pinD1 = 33;
int pinE1 = 34;
int pinF1 = 30;
int pinG1 = 44;  

//pins du chiffre de dizaine


//digit; pin entrant au transistor (a la base du transistor)
int digit_dizaine = 22;
int digit_unite = 52;
void setup() {
  pinMode(pin_pulse, OUTPUT);
  digitalWrite(pin_pulse, LOW);
  pinMode(pin_cap, INPUT);
  pinMode(pin_LED, OUTPUT);
  pinMode(pin_LED2, OUTPUT);
  digitalWrite(pin_LED, LOW);
  digitalWrite(pin_LED2, LOW);

  pinMode(pinA1,OUTPUT);
  pinMode(pinB1,OUTPUT);
  pinMode(pinC1,OUTPUT);
  pinMode(pinD1,OUTPUT);
  pinMode(pinE1,OUTPUT);
  pinMode(pinF1,OUTPUT);
  pinMode(pinG1,OUTPUT);

  pinMode(digit_dizaine,OUTPUT);
  pinMode(digit_unite,OUTPUT);
}
 
const int nmeas = 256; //measurements to take
long int sumsum = 0; //running sum of 64 sums
long int skip = 0; //number of skipped sums
long int diff = 0;      //difference between sum and avgsum
long int flash_period = 0; //period (in ms)
long unsigned int prev_flash = 0; //time stamp of previous flash

void choose_number(char num){
  switch(num){
    default:
    digitalWrite(pinA1, LOW);
    digitalWrite(pinB1,HIGH);
    digitalWrite(pinC1,HIGH);
        digitalWrite(pinD1,HIGH);
    digitalWrite(pinG1,LOW);
    digitalWrite(pinF1,LOW);
    digitalWrite(pinE1,LOW);
    break;
    
    //display by default
    case 0:
    digitalWrite(pinA1, LOW);
    digitalWrite(pinB1,LOW);
    digitalWrite(pinC1,LOW);
    digitalWrite(pinD1,LOW);
    digitalWrite(pinG1,HIGH);
    digitalWrite(pinF1,LOW);
    digitalWrite(pinE1,LOW);
    break;
    
    case 1:
    digitalWrite(pinA1, HIGH);
    digitalWrite(pinB1,LOW);
    digitalWrite(pinC1,LOW);
    digitalWrite(pinD1,HIGH);
    digitalWrite(pinG1,HIGH);
    digitalWrite(pinF1,HIGH);
    digitalWrite(pinE1,HIGH);
    break;

    case 2:
    digitalWrite(pinA1, LOW);
    digitalWrite(pinB1,LOW);
    digitalWrite(pinC1,HIGH);
    digitalWrite(pinD1,LOW);
    digitalWrite(pinG1,LOW);
    digitalWrite(pinF1,HIGH);
    digitalWrite(pinE1,LOW);
    break;

    case 3:
    digitalWrite(pinA1, LOW);
    digitalWrite(pinB1,LOW);
    digitalWrite(pinC1,LOW);
    digitalWrite(pinD1,LOW);
    digitalWrite(pinG1,LOW);
    digitalWrite(pinF1,HIGH);
    digitalWrite(pinE1,HIGH);
    break;

    case 4:
    digitalWrite(pinA1, HIGH);
    digitalWrite(pinB1,LOW);
    digitalWrite(pinC1,LOW);
    digitalWrite(pinD1,HIGH);
    digitalWrite(pinG1,LOW);
    digitalWrite(pinF1,LOW);
    digitalWrite(pinE1,HIGH);
    break;

    case 5:
    digitalWrite(pinA1, LOW);
    digitalWrite(pinB1,HIGH);
    digitalWrite(pinC1,LOW);
    digitalWrite(pinD1,LOW);
    digitalWrite(pinG1,LOW);
    digitalWrite(pinF1,LOW);
    digitalWrite(pinE1,HIGH);
    break;

    case 6:
    digitalWrite(pinA1, LOW);
    digitalWrite(pinB1,HIGH);
    digitalWrite(pinC1,LOW);
    digitalWrite(pinD1,LOW);
    digitalWrite(pinG1,LOW);
    digitalWrite(pinF1,LOW);
    digitalWrite(pinE1,LOW);
    break;

    case 7:
    digitalWrite(pinA1, LOW);
    digitalWrite(pinB1,LOW);
    digitalWrite(pinC1,LOW);
    digitalWrite(pinD1,HIGH);
    digitalWrite(pinG1,HIGH);
    digitalWrite(pinF1,HIGH);
    digitalWrite(pinE1,HIGH);
    break;

    case 8:
    digitalWrite(pinA1, LOW);
    digitalWrite(pinB1,LOW);
    digitalWrite(pinC1,LOW);
    digitalWrite(pinD1,LOW);
    digitalWrite(pinG1,LOW);
    digitalWrite(pinF1,LOW);
    digitalWrite(pinE1,LOW);
    break;

    case 9:
    digitalWrite(pinA1, LOW);
    digitalWrite(pinB1,LOW);
    digitalWrite(pinC1,LOW);
    digitalWrite(pinD1,LOW);
    digitalWrite(pinG1,LOW);
    digitalWrite(pinF1,LOW);
    digitalWrite(pinE1,HIGH);
    break;   
  }
}



void pick_digit (int num){
  digitalWrite(digit_dizaine,HIGH);
  digitalWrite(digit_unite,HIGH);//eteint les pins base du transistor -> eteint les 2 display
  switch(num){
    case 1:
      //allume digit des unites
      digitalWrite (digit_unite,LOW);break;//envoie a la base du transistor -> allume le display 
    case 2:
      digitalWrite (digit_dizaine,LOW);break;   
  }
}


void displaye(int number){
  if (number >=0 && number <=99){
    unsigned char dizaine = int((number/10)%10);
    unsigned char unite = int (number%10);
    
    if  (dizaine !=0){
      delay(10);// delays are random i was playing with them ;)
      choose_number(unite);//dire a quels pins s'allumer pour afficher le nombre
      pick_digit(1);//eteint les 2 seven segment display puis allume celle des unite pour afficher le nombre au dessus ^
      delay(t);

      choose_number(dizaine);//dire a quels pins s'allumer pour afficher le nombre
      pick_digit(2);;//eteint les 2 seven segment display puis allume celle des dizaines pour afficher le nombre au dessus ^
      delay(t);
    }
    else{
      
      pick_digit(2);
      choose_number(0);// affiche 0 sur le nombre des dizaines
      delay(20);

      pick_digit(1);
      choose_number(unite);//affiche le nombre des unites --> si on affiche 1 on obtient 0 1 
      delay(t);
    }
  }
  
}

void afficher(int number){
  int F = 0;//1000 === 20 seconds ; 100 == 2 seconds; 50 ==== 1 second (chaque nbre et sont temps d'excecution pour savoir a peu pres combien on veut que le nombresoit afficher) 
  do
  {
    displaye(number);//cette fonction appelle la fonction affiche plusieurs fois pour eliminer le flickering
    digitalWrite(pin_LED, LOW);
    ++F;  
  }while(F<2);
}

void loop() {
  digitalWrite(pin_LED2, LOW);
  int minval = 2000;
  int maxval = 0;
 
  //perform measurement
  long unsigned int sum = 0;
  for (int imeas = 0; imeas < nmeas + 2; imeas++) {
    //reset the capacitor
    pinMode(pin_cap, OUTPUT);
    digitalWrite(pin_cap, LOW);
    delayMicroseconds(80);
    pinMode(pin_cap, INPUT);
    //apply pulses
    for (int ipulse = 0; ipulse < npulse; ipulse++) {
      digitalWrite(pin_pulse, HIGH); //takes 3.5 microseconds
      delayMicroseconds(3);
      digitalWrite(pin_pulse, LOW); //takes 3.5 microseconds
      delayMicroseconds(3);
    }
    //read the charge on the capacitor
    int val = analogRead(pin_cap); //takes 13x8=104 microseconds
    minval = min(val, minval);
    maxval = max(val, maxval);
      sum += val;
 /////////////////////////////////////////////////////////////////////////////////////////////////////
    //determine if LEDs should be on or off
    long unsigned int timestamp = millis();
    byte ledstat = 0;
     
    if (timestamp < prev_flash +12) {
      if (diff > 0)ledstat = 1;
      if (diff < 0)ledstat = 2;
    }
    if (timestamp > prev_flash + flash_period) {
      if (diff > 0)ledstat = 1;
      if (diff < 0)ledstat = 2;
      prev_flash = timestamp;
    }
    if (flash_period > 1000)ledstat = 0;
 
    //switch the LEDs to this setting
    if (ledstat == 0) {
      digitalWrite(pin_LED, LOW);
    }
    if (ledstat == 1) {
      digitalWrite(pin_LED, LOW);
    }
    if (ledstat == 2) {
      digitalWrite(pin_LED, HIGH);
      S += 1; 
      //afficher(S);
       
    }
    
 
  }
 
  //subtract minimum and maximum value to remove spikes
  sum -= minval; sum -= maxval;
 
  //process
  if (sumsum == 0) sumsum = sum << 6; //set sumsum to expected value
  long int avgsum = (sumsum + 32) >> 6;
  diff = sum - avgsum;
  if (abs(diff)<avgsum >> 10) {   //adjust for small changes
    sumsum = sumsum + sum - avgsum;
    skip = 0;
  } else {
    skip++;
  }
  if (skip > 64) {  // break off in case of prolonged skipping
    sumsum = sum << 6;
    skip = 0;
  }
 
  // one permille change = 2 ticks/s
  if (diff == 0) flash_period = 1000000;
  else flash_period = avgsum / (2 * abs(diff));
  if (S>1500){
      S=0;
      nbcars+=1;
      digitalWrite(pin_LED2, HIGH);
      afficher(nbcars);
    
    }
}
