#include "app.h"


namespace
{
    const gpio_num_t interruptPin = GPIO_NUM_27;
    const float acceleromenter_sensitivity_g = 0.25;
    const uint64_t wake_timer_seconds = 30;
}


void App::begin()
{
    // Start this before any other initialization code
    // So we can get initial acceleration data as close as possible
    // to what triggered the wake interrupt.
    auto accel_started = accel.begin();

    // Turn on the builtin LED to show actvitiy
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    // Iintialize the serial port
    Serial.begin(115200);
    delay(250);
    Serial.println();
    Serial.println("Movement detector");

    if (!accel_started)
    {
        Serial.println("failed to intialize accelerometer!");
        Serial.flush();
        delay(1000);
        ESP.restart();
    }

    print_debug_data();

    accel.enable_activity_interrupt(acceleromenter_sensitivity_g);

    deepSleep.wakeupOnPin(interruptPin, RISING);
    deepSleep.wakeupOnTimer(wake_timer_seconds * 1000 * 1000);

    digitalWrite(LED_BUILTIN, HIGH);

    // TODO: Publish MQTT message.

    // Go to sleep
    deepSleep.sleep();
}

void App::loop()
{
}

void App::print_debug_data()
{
    switch (deepSleep.wakeupCause())
    {
    case ESP_SLEEP_WAKEUP_UNDEFINED:
        Serial.println("Normal startup");
        break;
    case ESP_SLEEP_WAKEUP_EXT0:
        Serial.println("Woke from activity interrupt");
        break;
    case ESP_SLEEP_WAKEUP_TIMER:
        Serial.println("Woke from timer");
        break;
    default:
        Serial.printf("Wake cause: %u\n", deepSleep.wakeupCause());
        break;
    }

    const auto& data = accel.get_startup_accel_data();

    Serial.print("Accelerometer data: (");
    Serial.print(data.x);
    Serial.print(", ");
    Serial.print(data.y);
    Serial.print(", ");
    Serial.print(data.z);
    Serial.println(")");
}
