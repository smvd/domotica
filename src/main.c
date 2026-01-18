#include "main.h"

LOG_MODULE_REGISTER(CORE, LOG_LEVEL_INF);

/*
	aliases {
		sw0 = &button0;
		led0 = &led0_green;
		led1 = &led1_red;
		led2 = &led1_green;
		led3 = &led1_blue;
		led0-green = &led0_green;
		led1-red = &led1_red;
		led1-green = &led1_green;
		led1-blue = &led1_blue;
		pwm-led0 = &red_pwm_led;
		pwm-led1 = &green_pwm_led;
		pwm-led2 = &blue_pwm_led;
		red-pwm-led = &red_pwm_led;
		green-pwm-led = &green_pwm_led;
		blue-pwm-led = &blue_pwm_led;
		mcuboot-button0 = &button0;
		mcuboot-led0 = &led0_green;
		watchdog0 = &wdt0;
	};
*/

// To init Router or node mode. Test is also included in GPIO for colour change!
#ifdef CONFIG_MESH_ROUTER_NODE
// Router-specific initialization
#else
// Regular node initialization
#endif

int main() {
	LOG_INF("Starting main");

    SHELL_Init();
    COAP_Init();
    GPIO_Init();

    while (1) {
        SHELL_HandleCommand();
        k_msleep(5);
    }
}
