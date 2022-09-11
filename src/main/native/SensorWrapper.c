/*
 * Copyright (c) 2008 by Naohide Sano, All rights reserved.
 *
 * Programmed by Naohide Sano
 */

#include "vavi_sensor_accelerometer_thinkpad_ThinkpadAccelerometer.h"

#include <stdio.h>
#include <windows.h>


/**
 * â¡ë¨ìxç\ë¢ëÃ
 */
struct ShockData {
    int status;
    short int  x0;
    short int  y0;
    short int  x1;
    short int  y1;
    short int  x2;
    short int  y2;
    short int  x3;
    short int  y3;
    short int  x4;
    short int  y4;
    short int  x5;
    short int  y5;
    short int  x6;
    short int  y6;
    short int  x7;
    short int  y7;
    short int  x8;
    short int  y8;
    short int  x9;
    short int  y9;
    short int  x10;
    short int  y10;
    short int  x11;
    short int  y11;
    short int  x12;
    short int  y12;
    short int  x13;
    short int  y13;
    short int  unknown0;
    short int  unknown1;
};

typedef struct _ACCELREPORT {
    // Current internal state (stable: 0, unstable1: 1: unstable2: 2)
    int PresentState;
    // latest raw acceleration data of X axis    <-- works!
    unsigned short LatestRawAccelDataX;
    // latest raw acceleration data of Y axis    <-- works!
    unsigned short LatestRawAccelDataY;
    // latest acceleration data of X axis (average in 40ms)   <-- Works even better?
    unsigned short LatestAccelDataX;
    // latest acceleration data of Y axis (average in 40ms)   <-- Works even better?
    unsigned short LatestAccelDataY;
    // latest temperature
    char Temperature;
    // latest zero-G offset of X axis  <-- Seems to be the current notion of "center"
    unsigned short LatestZeroG_X;
    // latest zero-G offset of Y axis  <-- ""
    unsigned short LatestZeroG_Y;
} ACCELREPORT, *PACCELREPORT;

typedef int SENSOR_API;
typedef SENSOR_API (__stdcall *readSensor_t)(PACCELREPORT  pAcceleration);

static HINSTANCE instance;
static ACCELREPORT data;
static readSensor_t sensorFunction;

static int OffsetX;
static int OffsetY;

static int X;
static int Y;

/*
 * Class:     vavi_sensor_accelerometer_thinkpad_ThinkpadAccelerometer
 * Method:    init
 * Signature: ()V
 */
JNIEXPORT jint JNICALL Java_vavi_sensor_accelerometer_thinkpad_ThinkpadAccelerometer_init
  (JNIEnv *env, jobject obj) {

    instance = LoadLibrary("Sensor.dll");
    if (instance != NULL) {
        sensorFunction = (readSensor_t) GetProcAddress(instance, "ShockproofGetAccelerometerData");
    } else {
        return (jint) -1;
    }

    OffsetX = 0;
    OffsetY = 0;

    if (sensorFunction(&data)) {
        OffsetX = -data.LatestRawAccelDataX;
        OffsetY = -data.LatestRawAccelDataY;
        Java_vavi_sensor_accelerometer_thinkpad_ThinkpadAccelerometer_sence(env, obj);
    } else {
        return (jint) -1;
    }

    return (jint) 0;
}

/*
 * Class:     vavi_sensor_accelerometer_thinkpad_ThinkpadAccelerometer
 * Method:    sence
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_vavi_sensor_accelerometer_thinkpad_ThinkpadAccelerometer_sence
  (JNIEnv *env, jobject obj) {

    if (sensorFunction(&data)) {
        X = data.LatestRawAccelDataX + OffsetX;
        Y = data.LatestRawAccelDataY + OffsetY;
/*
fprintf(stderr, "PresentState:\t%d\n"
                "LatestRawAccelDataX:\t%d\n"
                "LatestRawAccelDataY:\t%d\n"
                "LatestAccelDataX:\t%d\n"
                "LatestAccelDataY:\t%d\n"
                "Temperature:\t%d\n"
                "LatestZeroG_X:\t%d\n"
                "LatestZeroG_Y:\t%d\n", 
 data.PresentState,
 data.LatestRawAccelDataX,
 data.LatestRawAccelDataY,
 data.LatestAccelDataX,
 data.LatestAccelDataY,
 data.Temperature,
 data.LatestZeroG_X,
 data.LatestZeroG_Y);
fflush(stderr);
*/
        return (jint) 0;
    } else {
        return (jint) -1;
    }
}

/*
 * Class:     vavi_sensor_accelerometer_thinkpad_ThinkpadAccelerometer
 * Method:    destroy
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_vavi_sensor_accelerometer_thinkpad_ThinkpadAccelerometer_destroy
  (JNIEnv *env, jobject obj) {

    FreeLibrary(instance);
}

/*
 * Class:     vavi_sensor_accelerometer_thinkpad_ThinkpadAccelerometer
 * Method:    getX
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_vavi_sensor_accelerometer_thinkpad_ThinkpadAccelerometer_getX
  (JNIEnv *env, jobject obj) {

    return X;
}

/*
 * Class:     vavi_sensor_accelerometer_thinkpad_ThinkpadAccelerometer
 * Method:    getY
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_vavi_sensor_accelerometer_thinkpad_ThinkpadAccelerometer_getY
  (JNIEnv *env, jobject obj) {

    return Y;
}

/* */
