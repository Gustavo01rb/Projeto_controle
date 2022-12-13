#define SENSOR A0
#define WRITTER 6
#define ACTIVE 7

bool active_writer = false;

void setup() {
  Serial.begin(9600);
  pinMode(SENSOR, INPUT);
  pinMode(WRITTER, OUTPUT);
  pinMode(ACTIVE, INPUT);
}

void loop(){
  int activation = analogRead(SENSOR);
  Serial.print("Valor lido em porta A0: ");
  Serial.println(activation);
  int val = map(activation, 0, 1023, 0, 255);
  Serial.print("Valor convertido: ");
  Serial.println(val);

  while(!active_writer){
    active_writer = digitalRead(ACTIVE);
    delay(50);
  }
  while(active_writer){
    analogWrite(WRITTER, val);
    active_writer = digitalRead(ACTIVE);
  }
  Serial.println("\n\n");
}