/*
 * Copyright (c) 2008 by Naohide Sano, All rights reserved.
 *
 * Programmed by Naohide Sano
 */

package vavi.sensor.accelerometer.thinkpad;

import vavi.sensor.accelerometer.Accelerometer;


/**
 * ThinkpadAccelerometer. 
 *
 * @author <a href="mailto:vavivavi@yahoo.co.jp">Naohide Sano</a> (nsano)
 * @version 0.00 080715 nsano initial version <br>
 */
public class ThinkpadAccelerometer implements Accelerometer {

    /** */
    private native int init();
    
    /** */
    public native int sense();

    /** */
    private native void destroy();
    
    public ThinkpadAccelerometer() {
        init();
    }

    protected void finalize() throws Throwable {
        destroy();
    }

    /* */
    public native int getX();

    /* */
    public native int getY();

    /* */
    public int getZ() {
        return 0;
    }

    static {
        try {
            System.loadLibrary("accelerometer");
        } catch (Exception e) {
            throw (RuntimeException) new IllegalStateException().initCause(e);
        }
    }
}

/* */
