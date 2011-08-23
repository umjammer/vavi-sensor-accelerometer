/* motion.c
 *
 * a little program to display the coords returned by
 * the powerbook motion sensor
 *
 * A fine piece of c0de, brought to you by
 *
 *               ---===---
 * *** teenage mutant ninja hero coders ***
 *               ---===---
 *
 * All of the software included is copyrighted by Christian Klein <chris@5711.org>.
 *
 * Copyright 2005 Christian Klein. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by Christian Klein.
 * 4. The name of the author must not be used to endorse or promote
 *    products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * Modified for iBook compatibility by Pall Thayer <http://www.this.is/pallit>
 */

#include "vavi_sensor_accelerometer_macbook_MacbookAccelerometer.h"

#include <stdio.h>
#include <string.h>
#include <IOKit/IOKitLib.h>

struct data {
	char x;
	char y;
	char z;
	char pad[37];
};

static kern_return_t result;
static mach_port_t masterPort;
static io_iterator_t iterator;
static io_object_t aDevice;
static io_connect_t  dataPort;

#if __LP64__
size_t		structureInputSize;
size_t		structureOutputSize;
#else
IOItemCount   structureInputSize;
IOByteCount   structureOutputSize;
#endif

struct data inputStructure;
struct data outputStructure;
	
/*
 * Class:     vavi_sensor_accelerometer_macbook_MacbookAccelerometer
 * Method:    init
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_vavi_sensor_accelerometer_macbook_MacbookAccelerometer_init
(JNIEnv *env, jobject obj) {

	result = IOMasterPort(MACH_PORT_NULL, &masterPort);

	CFMutableDictionaryRef matchingDictionary = IOServiceMatching("SMCMotionSensor");

	result = IOServiceGetMatchingServices(masterPort, matchingDictionary, &iterator);

	if (result != KERN_SUCCESS)	{
		return -1;
	}

	aDevice = IOIteratorNext(iterator);
	IOObjectRelease(iterator);

	if(aDevice == 0) {
		return -2;
	}

	result = IOServiceOpen(aDevice, mach_task_self(), 0, &dataPort);
	IOObjectRelease(aDevice);

	if (result != KERN_SUCCESS) {
		return -3;
	}

	structureInputSize = sizeof(struct data);
	structureOutputSize = sizeof(struct data);

	memset(&inputStructure, 1, sizeof(inputStructure));
	memset(&outputStructure, 0, sizeof(outputStructure));

#if MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_5	
    result = IOConnectCallStructMethod(
		   dataPort,
		   5,				/* index to kernel function */
		   &inputStructure,
		   structureInputSize,
		   &outputStructure,
		   &structureOutputSize
		   );
#else
	result = IOConnectMethodStructureIStructureO(
			dataPort,
			5,				/* index to kernel function */
		    structureInputSize,
		    &structureOutputSize,
			&inputStructure,
			&outputStructure
		);
#endif

	if (result != KERN_SUCCESS) {
		return -4;
	}
		
	return 0;
}

/*
 * Class:     vavi_sensor_accelerometer_macbook_MacbookAccelerometer
 * Method:    sence
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_vavi_sensor_accelerometer_macbook_MacbookAccelerometer_sense
(JNIEnv *env, jobject obj) {

#if MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_5	
    result = IOConnectCallStructMethod(
									   dataPort,
									   5,				/* index to kernel function */
									   &inputStructure,
									   structureInputSize,
									   &outputStructure,
									   &structureOutputSize
									   );
#else
	result = IOConnectMethodStructureIStructureO(
		 dataPort,
		 5,				/* index to kernel function */
		 structureInputSize,
		 &structureOutputSize,
		 &inputStructure,
		 &outputStructure
	 );
#endif
	
	if (result != KERN_SUCCESS) {
		return -4;
	}
	
	return 0;
}

/*
 * Class:     vavi_sensor_accelerometer_macbook_MacbookAccelerometer
 * Method:    destroy
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_vavi_sensor_accelerometer_macbook_MacbookAccelerometer_destroy
(JNIEnv *env, jobject obj) {

	IOServiceClose(dataPort);
}


/*
 * Class:     vavi_sensor_accelerometer_macbook_MacbookAccelerometer
 * Method:    getX
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_vavi_sensor_accelerometer_macbook_MacbookAccelerometer_getX
(JNIEnv *env, jobject obj) {

	return outputStructure.x;
}

/*
 * Class:     vavi_sensor_accelerometer_macbook_MacbookAccelerometer
 * Method:    getY
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_vavi_sensor_accelerometer_macbook_MacbookAccelerometer_getY
(JNIEnv *env, jobject obj) {

	return outputStructure.y;
}

/*
 * Class:     vavi_sensor_accelerometer_macbook_MacbookAccelerometer
 * Method:    getZ
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_vavi_sensor_accelerometer_macbook_MacbookAccelerometer_getZ
(JNIEnv *env, jobject obj) {

	return outputStructure.z;
}
