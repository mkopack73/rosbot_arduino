/*
 * Test Program to try publishing HC-SR04 ultrasonic data to ROS
 */

#include <ros.h>
//#include <std_msgs/Int32.h>
#include <sensor_msgs/Range.h>

// Set up pin constants
#define trigPin 13
#define echoPin 12


// Characteristics of the sensor, needed to populate the message
#define FOV 15.0 // degrees, might need to change to radians
#define MINRANGE 2.0
#define MAXRANGE 400.0


ros::NodeHandle  nh;

//std_msgs::Int32 str_msg;
sensor_msgs::Range str_msg;
ros::Publisher chatter("/sensors/ultrasonic_range", &str_msg);

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  // put your setup code here, to run once:
  nh.initNode();
  nh.advertise(chatter);

}

void loop() {
  // read the sensor and determine the distance
  long duration, distance;
  digitalWrite(trigPin, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); // Added this line
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;

  // if out of range, set to -1 to indicate out of range
  if (distance >= MAXRANGE || distance <= MINRANGE){
    distance = MAXRANGE;
  }
  
  // copy the data into the Message
  str_msg.radiation_type = sensor_msgs::Range::ULTRASOUND;
  str_msg.field_of_view = FOV;
  str_msg.min_range = MINRANGE;
  str_msg.max_range = MAXRANGE;
  str_msg.range = distance;
  
  // now publish it out to ROS
  chatter.publish( &str_msg );
  nh.spinOnce();
  // publish roughly 4 times a second
  delay(250);
}
