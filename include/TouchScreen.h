#include "GT911.h"
#include <HID-Project.h>
#include <HID-Settings.h>

#define TOUCH_DEBUG 0

#define TOUCH_COUNT 2
#define INT_PIN 1
#define RST_PIN 0
#define INTERRUPT INT3
#define TOUCH_AREA_MUL 4

GT911 gt911 = GT911(&Wire, INT_PIN, INTERRUPT, RST_PIN, GT911_MODE_INTERRUPT);

bool fingers[TOUCH_COUNT];
int touchXResolution = 0;
int touchYResolution = 0;

void handleTouch(int8_t contacts, GTPoint *points)
{
    if (TOUCH_DEBUG)
    {
        Serial.print("handleTouch ");
        Serial.println(contacts);
    }

    if (contacts == 0)
    {
        for (int i = 0; i < TOUCH_COUNT; i++)
        {
            if (fingers[i])
            {
                fingers[i] = false;
                Touchscreen.releaseFinger(i);
            }
        }
    }
    else
    {
        for (int i = 0; i < TOUCH_COUNT; i++)
        {
            bool contains = false;
            int idx = -1;
            for (int j = 0; j < contacts; j++)
            {
                if (points[j].trackId == i)
                {
                    contains = true;
                    idx = j;
                    break;
                }
            }
            if (contains)
            {
                uint16_t x = (float)2560 / touchXResolution * points[idx].x;
                uint16_t y = (float)2560 / touchYResolution * points[idx].y;
                Touchscreen.setFinger(i, x, y, points[idx].area * TOUCH_AREA_MUL);
                fingers[i] = true;
            }
            else
            {
                if (fingers[i])
                {
                    Touchscreen.releaseFinger(i);
                    fingers[i] = false;
                }
            }
        }
    }
    Touchscreen.send();
    yield();
}

void beginTouchScreen()
{
    if (TOUCH_DEBUG)
    {
        delay(1000);
    }
    Touchscreen.begin();

    uint8_t begined = gt911.begin();

    if (TOUCH_DEBUG)
    {
        Serial.print("gt911 begin ");
        Serial.println(begined);
    }

    gt911.readConfig();
    if (gt911.readConfig()->touchNumber != TOUCH_COUNT)
    {
        if (TOUCH_DEBUG)
        {
            Serial.print("Config->touchNumber ");
            Serial.println(gt911.getReadedConfig()->touchNumber);
            Serial.print("Set touch number to ");
            Serial.println(TOUCH_COUNT);
        }
        gt911.getReadedConfig()->touchNumber = TOUCH_COUNT;
        gt911.writeConfig();
    }

    touchXResolution = gt911.getReadedConfig()->xResolution;
    touchYResolution = gt911.getReadedConfig()->yResolution;

    gt911.setHandler(handleTouch);
}

void loopTouchScreen()
{
    gt911.loop();
}