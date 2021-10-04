int x;
void setup() {
 Serial.begin(9600);
 Serial.setTimeout(1);
 
 
}
void loop() {
  int prev_x=0;
  int exponent=0;
 while (!Serial.available());
 x = Serial.readString().toInt();
 x=prev_x+pow(10,exponent)*x;
 prev_x=x;
  exponent=1+exponent;
 Serial.print(x);
}
