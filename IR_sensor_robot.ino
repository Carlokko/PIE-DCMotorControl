int IR_sensor = 0; // for incoming serial data

void setup(){
    pinMode(IR_sensor, INPUT); //initialize ldr sensor as INPUT
    Serial.begin( 115200); //begin the serial monitor at 9600 baud
}

void loop() {
    int data=analogRead(IR_sensor);
    Serial.print("IR_sensor reading=");
    Serial.println(data);
    delay(100);
}
