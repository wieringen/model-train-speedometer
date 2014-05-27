/*
  Sender.pde

  Model Train Speed-o-meter, http://www.baijs.com

  Copyright (c) 2014, Maarten Baijs
  All rights reserved.

*/

#include <math.h>
#include <JeeLib.h>

// Currently set to Marklin H0 scale 1=87.
//
const float MODEL_SCALE_MM = 87;
const float WHEEL_DIAMETER_MM = 11.9;

const float WHEEL_DIAMETER_METERS = WHEEL_DIAMETER_MM * 1000;
const float WHEEL_CIRCUMFERENCE_METERS = WHEEL_DIAMETER_METERS * M_PI;
const float TRAIN_DISTANCE_KMH = WHEEL_CIRCUMFERENCE_METERS * 60 * 60 * MODEL_SCALE_MM;
const float TRAIN_DISTANCE_MPH = TRAIN_DISTANCE_KMH * 0.621371;
 
const int RF12B_NETWORK = 33;
const int RF12B_NODE_ID = 2;

const int HALL_SENSOR_PIN = 1;

typedef struct
{
    float kmh;
    float mph;
    float wheel_revisions;
} Train_data;

Train_data train_data;

MilliTimer timer_transmit_data;
MilliTimer timer_last_measure;

float send_frequency = 0;
float send_last = 0;
    
Port led_sending_data (1);

void setup ()
{
    send_last = millis();

    // Serial Debugging
    //
    Serial.begin(9600);

    // interrupt called when sensors sends digital 2 high (every wheel rotation)
    //
    attachInterrupt(HALL_SENSOR_PIN, measure_speed, FALLING);

    // Init rf12
    //
    rf12_initialize(RF12B_NODE_ID, RF12_868MHZ, RF12B_NETWORK);

    // Set defaults
    //
    train_data.kmh = 0;
    train_data.mph = 0;
    train_data.wheel_revisions = 0;
}

void loop ()
{
     rf12_recvDone();

     if( timer_last_measure.poll(5000) )
     {
        train_data.kmh = 0;
        train_data.mph = 0;
        train_data.wheel_revisions = 0;
     }

     if( timer_transmit_data.poll(1500) && rf12_canSend() )
     {
       rf12_sendStart(0, &train_data, sizeof train_data);
       
       toggle_send_led(1);
       delay(100);
       toggle_send_led(0);
     }
}

void measure_speed()
{
    send_frequency = millis() - send_last;
    send_last = millis();

    if( send_frequency == send_frequency && send_frequency != INFINITY && send_frequency > 50 && send_frequency < 4000 ) {
      
        train_data.kmh = TRAIN_DISTANCE_KMH / send_frequency;
        train_data.mph = TRAIN_DISTANCE_MPH / send_frequency;
        train_data.wheel_revisions = send_frequency;

        timer_last_measure.set(0);
        //Serial.print(outData.speedKM);
        //Serial.print(" KM/H \n");
        //Serial.print(outData.speedMPH);
        //Serial.print(" MPH \n");
        //Serial.print(outData.revisions);
        //Serial.print(" ms/rev \n");
    }
}

static void toggle_send_led (byte on)
{
    led_sending_data.mode(OUTPUT);
    led_sending_data.digiWrite(on);
}
