#include "gpio_lib.hpp"
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define TIMING 200000

int pins[]  = { 8, 10, 12, 16, 19, 21, 23, 24,  5, 15, 26, 29, 35};
int state[] = { 1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  1,  0,  0};

void flush()
{
    while (getchar() != '\n');
    printf("Not a number\n");
}

int pin_index(int pin)
{
    int i;
    for (i = 0; i < sizeof(pins) / sizeof(int) && pins[i] != pin; i++) { }
    if (i == sizeof(pins) / sizeof(int))
    {
        return -1;
    }    
    else
    {
        return i;
    }

}

void setup()
{
    gpioSetupPhys();
    for (int i = 0; i < sizeof(pins) / sizeof(int); i++)
    {
        pinMode(pins[i], 1);
        digitalWrite(pins[i], state[i]);
    }
}

void set(int pin, int val)
{
    int index = pin_index(pin);
    if (index == -1)
    {
        fprintf(stderr, "Unknown pin\n");
        return;
    }
    digitalWrite(pins[index], val);
    state[index] = val;
}

void toggle(int pin)
{
    int index = pin_index(pin);
    if (index == -1)
    {
        fprintf(stderr, "Unknown pin\n");
        return;
    }
    digitalWrite(pins[index], 1 - state[index]);
    state[index] = 1 - state[index];
}

void button(int pin)
{
    toggle(pin);
    usleep(TIMING);
    toggle(pin);
    usleep(TIMING);
}

int main()
{
    setup();
    int pin = 0, value = 0;
    char s[128];
    
    while (1)
    {
        if (scanf("%127s", s) < 1)
        {
            break;
        }
        if (strcmp(s, "toggle") == 0)
        {
            if (scanf("%d", &pin) < 1)
                flush();
            else
                toggle(pin);
            continue;
        }
        if (strcmp(s, "button") == 0)
        {
            if (scanf("%d", &pin) < 1)
                flush();
            else
                button(pin);
            continue;
        }
        if (strcmp(s, "set") == 0)
        {
            if (scanf("%d %d", &pin, &value) < 2)
                flush();
            else
                set(pin, value);
            continue;
        }
        if (strcmp(s, "exit") == 0)
        {
            break;
        }
    }
}