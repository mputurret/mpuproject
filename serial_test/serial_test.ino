void setup() {
  
  Serial.begin(9600);
}

void loop() {
  if (Serial.available()) {
    String message = Serial.readStringUntil('\n');
    Serial.println(message);
    if (message == "1235.264412, 321.46536"){
      Serial.println("yeah");
    }
    else{
      Serial.println("ohno");
    }
  }

}
