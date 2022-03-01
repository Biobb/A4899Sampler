#include <Arduino.h>
#include "A4988.h"
#include <array>
#include <vector>

// using a 200-step motor (most common)
#define MOTOR_STEPS 200

// configure the pins connected
#define sampler_rotation 90 // Modify as needed
#define sampler_direction 8
#define sampler_step 9
#define sampler_MS1 10
#define sampler_MS2 11
#define sampler_MS3 12
#define sampler_enable 13 // toimiiko paremmin sleepillä?
#define sampler_RPM 120

// check before connecting
#define rotator_direction 14
#define rotator_step 15
#define rotator_MS1 16
#define rotator_MS2 17
#define rotator_MS3 18
#define rotator_enable 19
#define rotator_RPM 120

// One pin for edge/home switch for rotator.
#define rotator_edge 20
// Sampling time in seconds _init_
int sampling_time = 1; 


#define init_offset 200 //110 toimii?

using namespace std;
// Offsets for slots, if not perfectly symmetrical
array<int,16> slot_diff = { 160,200,200,200,190,200,200,220, 
                             180,220,220,180,200,200,180,180};
int slot_diff_size = slot_diff.size();

// setup for rotator stepper
A4988 rotator(MOTOR_STEPS, rotator_direction, rotator_step, rotator_MS1, rotator_MS2, rotator_MS3);
// setup for sampler stepper
A4988 sampler(MOTOR_STEPS, sampler_direction, sampler_step, sampler_MS1, sampler_MS2, sampler_MS3);


void setup() {
    // Set target motor RPM to 1RPM and microstepping to 1 (full step mode)
    //stepper.begin(1, 1);
    sampler.begin(sampler_RPM);
    rotator.begin(rotator_RPM);
    
    sampler.setMicrostep(16);
    rotator.setMicrostep(16);
    
    sampler.enable();
    rotator.enable();
                             
   
}

int grabSample(int sampling_time){
  Serial.println("Starting sampling");
  sampler.move(sampler_rotation);
  delay(sampling_time);
  sampler.move(-sampler_rotation);
  }
  
// this should include vector of desired stop times for each samples wanted.
// also get this from serial, not hardwired like here
// max samples 16, should limit that with error,
std::vector<int> schedule{1,1,2,1};

int schedule_size = schedule.size();

void loop() {
  delay(1000);
  rotator.move(MOTOR_STEPS);
  delay(1000);
  grabSample(2000); // delaytime in microseconds
  rotator.move(-MOTOR_STEPS);
  delay(3000);
  
  // for future use, first check that steppers work.
  /* 
  for (int i = 0; i < schedule_size; i++) {
    delay(500); // delay for thinking
    // first collection vessel is already at location, continue with sample collection
    grabSample(schedule[i]);
    //Serial.println(schedule[i]);
    rotator.enable();
    if (i+1 != schedule_size){
      // this should be reconfigured with new stepper library to work with slot_diff offsets.
      rotator.move(slot_diff[i]);
    }
    rotator.disable();
    */
    
    }
}
