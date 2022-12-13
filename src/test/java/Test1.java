/*
 * Copyright (c) 2022 by Naohide Sano, All rights reserved.
 *
 * Programmed by Naohide Sano
 */

import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.condition.EnabledIf;
import vavi.sensor.accelerometer.Accelerometer;
import vavi.sensor.accelerometer.macbook.MacbookAccelerometer;


/**
 * Test1.
 *
 * @author <a href="mailto:umjammer@gmail.com">Naohide Sano</a> (nsano)
 * @version 0.00 2022-09-03 nsano initial version <br>
 */
@EnabledIf("available")
class Test1 {

    static Accelerometer accelerometer;

    static boolean available() {
        System.loadLibrary("accelerometer");

        try {
            accelerometer = new MacbookAccelerometer();
            return true;
        } catch (IllegalStateException e) {
            e.printStackTrace();
            return false;
        }
    }

    @Test
    void test1() throws Exception {
        for (int i = 0; i < 10; i++) {
            accelerometer.sense();
            System.err.printf("%d, %d, %d%n", accelerometer.getX(), accelerometer.getY(), accelerometer.getZ());
        }
    }
}
