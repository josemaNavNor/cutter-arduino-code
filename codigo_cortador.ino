//------------------------------- librarys ----------------------------------

#include <LiquidCrystal.h>
#include <Servo.h>
 
//------------------------------- lcd ----------------------------------
LiquidCrystal lcd(12, 11, 2, 3, 4, 5);
 
//------------------------------- stepper ----------------------------------
#define stepPin 7
#define dirPin 8
 
//------------------------------- servo ----------------------------------
Servo snippers;
#define servo 10
#define openAngle 180
#define closedAngle 0
 
//------------------------------- input ----------------------------------
 
#define leftButton 14
#define rightButton 9
#define upButton 15
#define downButton 6
 
//------------------------------- configuracion usuario ----------------------------------
unsigned int wireLength = 0;
unsigned int wireQuantity = 0;
 
//------------------------------- configuracion sistema ----------------------------------
int state = 0;
int incrementSpeed = 1;
int previousWireLength = 0;
int previousWireQuantity = 0;
float mmPerStep = 0.18096;
 
 
void setup() {
  Serial.begin(9600);
 
 
  lcd.begin(16, 2); //LCD columnas y filas
 
 
  pinMode(upButton, INPUT_PULLUP);
  pinMode(downButton, INPUT_PULLUP);
  pinMode(leftButton, INPUT_PULLUP);
  pinMode(rightButton, INPUT_PULLUP);
 
  pinMode(stepPin,OUTPUT);
  pinMode(dirPin,OUTPUT);
 
  snippers.attach(servo);
 
  snippers.write(openAngle);
   
  delay(1000);
}
 
void loop() {
  if (!digitalRead(rightButton)){
    if(state == 5){
      state = 0;
    }
    else{
      state += 1;
    }
    delay(200);
    lcd.clear();
  }
  if (!digitalRead(leftButton) && state > 0 && state < 4){
    state -=1;
    delay(200);
    lcd.clear();
  }
 
 
  switch (state){
    case 0:
      homeScreen();
      break;
    case 1:
       chooseWireLength();
       break;
    case 2:
      chooseWireQuantity();
      break;
    case 3:
      confirm();
      break;
    case 4:
      currentlyCutting();
      break;
    case 5:
      finishedCutting();
      break;
  }
 
}
 
 
void homeScreen(){
  lcd.setCursor(0, 0);
  lcd.print("CORTADOR DE CABLES");
  lcd.setCursor(11, 1);
  lcd.print("SIGUIENTE>");
  delay(100);
}
 
 
void chooseWireLength(){
  wireLength = changeValue(wireLength);
 
  // limpiar la pantalla si es necesario
  if(previousWireLength != wireLength){
    lcd.clear();
    previousWireLength = wireLength;
  }
 
  // mostrar info en la pantalla LCD
  lcd.setCursor(0, 0);
  lcd.print("TAMAÑO:" + (String)wireLength + "mm");
  displayNavigation();
}
 
void chooseWireQuantity(){
  wireQuantity = changeValue(wireQuantity);
 
  // limpiar la pantalla si es necesario
  if(previousWireQuantity != wireQuantity){
    lcd.clear();
    previousWireQuantity = wireQuantity;
  }
 
  // mostrar info en la pantalla LCD
  lcd.setCursor(0, 0);
  lcd.print("QUANTITY:" + (String)wireQuantity);
  displayNavigation();
}
 
void confirm(){
  lcd.setCursor(0, 0);
  lcd.print((String)wireLength + "mm x " + (String)wireQuantity + "pcs");
  lcd.setCursor(0, 1);
  lcd.print("<ATRAS");
  lcd.setCursor(10, 1);
  lcd.print("COMENZAR>");
  delay(100);
}
 
void currentlyCutting(){
  lcd.setCursor(0, 0);
  lcd.print((String)0 + "/" + (String)wireQuantity);
  lcd.setCursor(0, 1);
  lcd.print("???s");
  int stepsToTake = (int)wireLength/mmPerStep;
  for(int i = 0; i < wireQuantity; i++){
    unsigned long timeForOneCycle = millis();
    digitalWrite(dirPin,HIGH);
    for(int x = 0; x < stepsToTake; x++) {
      digitalWrite(stepPin,HIGH);
      delayMicroseconds(500);
      digitalWrite(stepPin,LOW);
      delayMicroseconds(500);
    }
   
    lcd.setCursor(0, 0);
    lcd.print((String)(i+1) + "/" + (String)wireQuantity);
   
    snippers.write(closedAngle);
    delay(600);
    snippers.write(openAngle);
    delay(600);
 
   
    lcd.setCursor(0, 1);
 
    unsigned long timeRemaining = ((millis() - timeForOneCycle)*(wireQuantity - (i+1)))/1000;
    lcd.print((String)timeRemaining + "s    ");
   
  }
  wireLength = 0;
  wireQuantity = 0;
  state = 5;
}
 
void finishedCutting(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("CORTE COMPLETADO");
  lcd.setCursor(11, 1);
  lcd.print("SIGUIENTE>");
  delay(100);
}
 
 
int changeValue(int currentValue){
  if (!digitalRead(upButton)) {
    delay(100);
    currentValue += incrementSpeed;
    
  }
  if (!digitalRead(downButton)) {
    if(currentValue - incrementSpeed >= 0){
      delay(100);
      currentValue -= incrementSpeed;
    
    }
    else{
      currentValue = 0;
    }
  }
  if (!digitalRead(downButton) && !digitalRead(upButton)){
    incrementSpeed = 1;
  }
  return currentValue;
}
 
void displayNavigation(){
  lcd.setCursor(0, 1);
  lcd.print("<ATRAS");
  lcd.setCursor(11, 1);
  lcd.print("SIGUIENTE>");
  delay(100);
}
