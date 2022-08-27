//Compile with `gcc -o output send_data.c -lwiringPi`

#include <wiringPi.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define TIMING 889

void setup()
{
    wiringPiSetupPhys();
    pinMode(26, 1);
    digitalWrite(26, 1);
}

void send(int to_transmit, int toggle_bit)
{
    int data[14];
    unsigned long time;
    struct timespec t;

    to_transmit += 12288;
    to_transmit += toggle_bit * 2048;

    for (int i = 0; i < 14; i++)
    {
        data[13 - i] = to_transmit % 2;
        to_transmit /= 2;
    }

    clock_gettime(CLOCK_BOOTTIME, &t);

    for (int i = 0; i < 14; i++)
    {
        time = t.tv_sec * 1000 * 1000 + t.tv_nsec / 1000;
        digitalWrite(26, 0 + data[i]);

        while (t.tv_sec * 1000 * 1000 + t.tv_nsec / 1000 - time < TIMING)
        {
            clock_gettime(CLOCK_BOOTTIME, &t);
        }

        time = t.tv_sec * 1000 * 1000 + t.tv_nsec / 1000;
        digitalWrite(26, 1 - data[i]);

        while (t.tv_sec * 1000 * 1000 + t.tv_nsec / 1000 - time < TIMING)
        {
            clock_gettime(CLOCK_BOOTTIME, &t);
        }
    }

    digitalWrite(26, 1);
}

int main(int argc, char *argv[])
{
    setup();

    for (int i = 1; i < argc / 2; i += 2)
    {
        send(atoi(argv[i]), argv[i + 1]);
        usleep(300 * 1000);
    }
}
