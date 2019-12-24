/*
 * odroid-c2.h
 *
 *  Created on: Apr 22, 2017
 *      Author: Alex Konshin
 */

#ifndef ARCH_ODROID_C2_H_
#define ARCH_ODROID_C2_H_

/*

See http://odroid.com/dokuwiki/doku.php?id=en:c2_hardware_irq

gpio readall
 +------+-----+----------+------+ Model  ODROID-C2 +------+----------+-----+------+
 | GPIO | wPi |   Name   | Mode | V | Physical | V | Mode |   Name   | wPi | GPIO |
 +------+-----+----------+------+---+----++----+---+------+----------+-----+------+
 |      |     |     3.3v |      |   |  1 || 2  |   |      | 5v       |     |      |
 |      |   8 |    SDA.1 |      |   |  3 || 4  |   |      | 5V       |     |      |
 |      |   9 |    SCL.1 |      |   |  5 || 6  |   |      | 0v       |     |      |
 |  249 |   7 | GPIO.249 |   IN | 1 |  7 || 8  |   |      | TxD1     | 15  |      |
 |      |     |       0v |      |   |  9 || 10 |   |      | RxD1     | 16  |      |
 |  247 |   0 | GPIO.247 |   IN | 1 | 11 || 12 | 1 | IN   | GPIO.238 | 1   |  238 |
 |  239 |   2 | GPIO.239 |   IN | 1 | 13 || 14 |   |      | 0v       |     |      |
 |  237 |   3 | GPIO.237 |   IN | 1 | 15 || 16 | 1 | IN   | GPIO.236 | 4   |  236 |
 |      |     |     3.3v |      |   | 17 || 18 | 1 | IN   | GPIO.233 | 5   |  233 |
 |  235 |  12 | GPIO.235 |   IN | 1 | 19 || 20 |   |      | 0v       |     |      |
 |  232 |  13 | GPIO.232 |   IN | 1 | 21 || 22 | 1 | IN   | GPIO.231 | 6   |  231 |
 |  230 |  14 | GPIO.230 |   IN | 1 | 23 || 24 | 1 | IN   | GPIO.229 | 10  |  229 |
 |      |     |       0v |      |   | 25 || 26 | 1 | OUT  | GPIO.225 | 11  |  225 |
 |      |  30 |    SDA.2 |      |   | 27 || 28 |   |      | SCL.2    | 31  |      |
 |  228 |  21 | GPIO.228 |   IN | 1 | 29 || 30 |   |      | 0v       |     |      |
 |  219 |  22 | GPIO.219 |   IN | 1 | 31 || 32 | 1 | IN   | GPIO.224 | 26  |  224 |
 |  234 |  23 | GPIO.234 |   IN | 0 | 33 || 34 |   |      | 0v       |     |      |
 |  214 |  24 | GPIO.214 |   IN | 1 | 35 || 36 | 1 | IN   | GPIO.218 | 27  |  218 |
 |      |  25 |    AIN.1 |      |   | 37 || 38 |   |      | 1v8      | 28  |      |
 |      |     |       0v |      |   | 39 || 40 |   |      | AIN.0    | 29  |      |
 +------+-----+----------+------+---+----++----+---+------+----------+-----+------+
 */

#ifndef TEST_DECODING
#define MAX_GPIO 249
#define DEFAULT_PIN 239
#endif

#endif /* ARCH_ODROID_C2_H_ */
