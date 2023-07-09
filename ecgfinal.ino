long instance1=0, timer;              //instance1: instance when heart beat first time
double hrv =0, hr = 72, interval = 0; //hr: HeartRate (initialised to 72);hrv: Heart Rate variability (takes 10-15 seconds to stabilise);
                                      //interval: interval between second beat and first beat
int value = 0, count = 0;   
int newvalue;                         //value: raw sensor value of output pin
                                      //count: variable to hold count of rr peaks detected in 10 seconds
bool flag = 0;                        //prevents multiple rr peak detections in a single heatbeat


#define shutdown_pin 10 
#define threshold 100                 // to identify R peak
#define timer_value 10000             // 10 seconds timer to calculate hr


void setup() {

  Serial.begin(9600);
  pinMode(8, INPUT); // Setup for leads off detection LO +
  pinMode(9, INPUT); // Setup for leads off detection LO -

}

void loop() { 

  if((digitalRead(8) == 1)||(digitalRead(9) == 1)){
    Serial.println("leads off!");
    digitalWrite(shutdown_pin, LOW); //standby mode
    instance1 = micros();
    timer = millis();

  }

  else {

    digitalWrite(shutdown_pin, HIGH); //normal mode
    value = analogRead(A0);
    newvalue = map(value, 250, 400, 0, 100); //to flatten the ecg values a bit

    if((value > threshold) && (!flag)) {

      count++;  
      Serial.println("in");
      flag = 1;
      interval = micros() - instance1; //RR interval
      instance1 = micros(); 

    }

    else if((value < threshold)) {

      flag = 0;

    }

    if ((millis() - timer) > 10000) {

      hr = count*6;
      timer = millis();
      count = 0; 

    }

    hrv = hr/60 - interval/1000000;

    Serial.print(hr);
    Serial.print(",");
    Serial.print(hrv);
    Serial.print(",");
    Serial.println(newvalue);
    delay(1);

  }

}
