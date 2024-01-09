#include <Wire.h>
#include <MPU6050_light.h>

MPU6050 mpu(Wire);


const int SoundPin = 4;
const int SpeakerPIN = 11;
const int Button1 = 8;
const int Button2 = 9;
const int Button3 = 10;


static const float c_freq[] = { 16.35, 32.70, 65.41, 130.81, 261.63, 523.25, 1046.50, 2093.00, 4186.01};
static const float cs_freq[] = { 17.32, 34.65, 69.30, 138.59, 277.18, 554.37, 1108.73, 2217.46, 4434.92};
static const float d_freq[] = { 18.35, 36.71, 73.42, 146.83, 293.66, 587.33, 1174.66, 2349.32, 4698.63};
static const float ds_freq[] = { 19.45, 38.89, 77.78, 155.56, 311.13, 622.25, 1244.51, 2489.02, 4978.03};
static const float e_freq[] = { 20.60, 41.20, 82.41, 164.81, 329.63, 659.25, 1318.51, 2637.02, 5274.04};
static const float f_freq[] = { 21.83, 43.65, 87.31, 174.61, 349.23, 698.46, 1396.91, 2793.83, 5587.65};
static const float fs_freq[] = { 23.12, 46.25, 92.50, 185.00, 369.99, 739.99, 1479.98, 2959.96, 5919.91};
static const float g_freq[] = { 24.50, 49.00, 98.00, 196.00, 392.00, 783.99, 1567.98, 3135.96, 6271.93};
static const float gs_freq[] = { 25.96, 51.91, 103.83, 207.65, 415.30, 830.61, 1661.22, 3322.44, 6644.88};
static const float a_freq[] = { 27.50, 55.00, 110.00, 220.00, 440.00, 880.00, 1760.00, 3520.00, 7040.00};
static const float as_freq[] = { 29.14, 58.27, 116.54, 233.08, 466.16, 932.33, 1864.66, 3729.31, 7458.62};
static const float b_freq[] = { 30.87, 61.74, 123.47, 246.94, 493.88, 987.77, 1975.53, 3951.07, 7902.13 };


int getFrequency( char pitch, float y) {

    int octave;

    // angle to octave
    if (y > -15 && y <= 15) {
       octave = 3;
    } else if (y > 15 && y <= 45) {
       octave = 4;
    } else if (y > 45 && y <= 70) {
       octave = 5;
    } else if (y > 70 ) {
       octave = 6;
    } else if (y > -45 && y <= -15) {
       octave = 2;
    } else if (y > -70 && y <= -45) {
       octave = 1;
    } else if (y <= -70) {
       octave = 0;
    }

    // ensure we are not out of range
    if ( octave > 6) {
       octave = 6;
    } else if ( octave < 0 ) {
       octave = 0;
    }

    float freq;
    switch (pitch) {
      default:   // default to c
      case 'C':
          freq = c_freq[octave];
          break;
      case 'D':
          freq = d_freq[octave];
          break;
      case 'E':
          freq = e_freq[octave];
          break;
      case 'F':
          freq = f_freq[octave];
          break;
      case 'G':
          freq = g_freq[octave];
          break;
      case 'A':
          freq = a_freq[octave];
          break;
      case 'B':
          freq = b_freq[octave];
          break;
   }

    // cast to int for now
    return (int)freq;
}


void setup ()
{

  // serial output boardrate
  Serial.begin(9600);

  // input lines setup
  pinMode ( SoundPin, INPUT );
  pinMode ( Button1, INPUT);
  pinMode ( Button2, INPUT);
  pinMode ( Button3, INPUT);

  // speaker setup
  pinMode (SpeakerPIN, OUTPUT );
  digitalWrite(SpeakerPIN, LOW);
  //cout << "hi";
  // mpu6050 setup
  while (mpu.begin()!=0) {
   Serial.println("Failed to initialize Sensor chip");
      delay(100);
  }
  Serial.println(F("Calculating offsets, do not move MPU6050"));
  delay(1000);
  // mpu.upsideDownMounting = true; // uncomment this line if the MPU6050 is mounted upside-down
  mpu.calcOffsets(); // gyro and accelero
  Serial.println("Done!\n");



}

void loop ()
{

  // get the button states
  int button1 = digitalRead ( Button1 );
  int button2 = digitalRead ( Button2 );
  int button3 = digitalRead ( Button3 );

  //get the values from the mpu6050
  mpu.update();

  float y;
  // get and print the angles
  Serial.print("X : ");
  Serial.print(mpu.getAngleX());
  Serial.print("\tY : ");
  y = mpu.getAngleY();
  Serial.print(mpu.getAngleY());
  Serial.print("\tZ : ");
  Serial.println(z);


 
  char pitch;
  if (button1 == LOW && button2 == LOW && button3 == LOW) {
    pitch = 'C';
  } else if (button1 == LOW && button2 == LOW && button3 == HIGH) {
    pitch = 'D';
  } else if (button1 == LOW && button2 == HIGH && button3 == LOW) {
    pitch = 'E';
  } else if (button1 == HIGH && button2 == LOW && button3 == LOW) {
    pitch = 'F';
  } else if (button1 == LOW && button2 == HIGH && button3 == HIGH) {
    pitch = 'G';
  } else if (button1 == HIGH && button2 == LOW && button3 == HIGH) {
    pitch = 'A';
  } else if (button1 == HIGH && button2 == HIGH && button3 == HIGH) {
    pitch = 'B';
  }
 
  int frequency;
  frequency = getFrequency( pitch, angle);

  int button ;
  button = digitalRead ( SoundPin );
  if ( button ==  HIGH ) {

    digitalWrite(SpeakerPIN, HIGH); // why?
    tone(SpeakerPIN, frequency);
  } else {
    digitalWrite(SpeakerPIN, LOW);
    noTone(SpeakerPIN);
  }
  // too long assuming ms... try 50?
  delay (500);
}


  
