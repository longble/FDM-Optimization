#define BUTTON_PIN A0
#define DIR_PIN 2
#define STEP_PIN 3
#define ENABLE_PIN 4

const uint8_t STEPS_PER_REVOLUTION = 200;
const uint8_t PULSES_PER_STEP = 4;
const uint32_t PULSES_PER_REVOLUTION = (uint32_t)PULSES_PER_STEP * (uint32_t)STEPS_PER_REVOLUTION;
const uint32_t MICROSECONDS_PER_RPS = 1000000 / PULSES_PER_REVOLUTION;
const float RPS = 0.09843; // Define Me For Speeeed
const uint16_t PULSETIME = (float)MICROSECONDS_PER_RPS / RPS;
const uint16_t DESIRED_REVOLUTIONS = 20;


void setup() {
  // Declare pins as output:
  Serial.begin(115200);
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(ENABLE_PIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  // Set the spinning direction CW/CCW:
  digitalWrite(DIR_PIN, LOW);
  digitalWrite(ENABLE_PIN, LOW);
  Serial.println("Ready");
}

void loop() {
  if(!digitalRead(BUTTON_PIN)){
    uint32_t totalSteps = (uint32_t)PULSES_PER_STEP * (uint32_t)STEPS_PER_REVOLUTION * (uint32_t)DESIRED_REVOLUTIONS;
    bool interrupted = stepSomeTimes(totalSteps, millis());
    Serial.println("End steps!");
    if(interrupted){
      digitalWrite(LED_BUILTIN, HIGH);
      delay(3000);
    }
  }
  if((millis() / 1000) % 2 == 1){
    digitalWrite(LED_BUILTIN, HIGH);
  }else{
    digitalWrite(LED_BUILTIN, LOW);
  }
  delay(50);
}

/*
 *   REVOLUTIONS    STEP           PULSES   
 *   SECOND         REVOLUTION     STEP     
 */

boolean stepSomeTimes(uint32_t steps, uint32_t startTime){
  uint32_t pulse_time_micros = PULSETIME;
  //uint16_t pulse_time_micros = 1000;
  uint32_t disableAllowedTime = startTime + 1000;
  delay(5);
  Serial.println("Steps: " + String(steps));
  delay(5);
  for(uint32_t i = 0; i < steps; i++){
      // These four lines result in 1 step:
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(pulse_time_micros / 2);
    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(pulse_time_micros / 2);
    if(!digitalRead(BUTTON_PIN) && (millis() > disableAllowedTime)){
      delay(5);
      if(!digitalRead(BUTTON_PIN)){
        return true;
      }
    }
    if(i % (PULSES_PER_STEP * STEPS_PER_REVOLUTION) < 100){
      digitalWrite(LED_BUILTIN, HIGH);
//      if(i % (PULSES_PER_STEP * STEPS_PER_REVOLUTION) == 0){
//        Serial.println("Rev " + String((float)i / (float)(PULSES_PER_STEP * STEPS_PER_REVOLUTION)) );
//      }      
    }else{
      digitalWrite(LED_BUILTIN, LOW);
    }
  }
  return false;
}
