RGB_MATRIX_EFFECT(sick_tricks)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

// static bool sick_tricks_old(effect_params_t* params) {
//   RGB_MATRIX_USE_LIMITS(led_min, led_max);
//   for (uint8_t i = led_min; i < led_max; i++) {
//     // separate intensity for backlight and key lights
//     uint8_t intensity = 0xff;
//     if (HAS_FLAGS(g_led_config.flags[i], LED_FLAG_KEYLIGHT)) {
//       intensity = rgb_matrix_config.hsv.v;
//     }
//     if (i % 2) {
//         rgb_matrix_set_color(i, 0x00, 0x00, intensity);
//     } else {
//         rgb_matrix_set_color(i, intensity, 0x00, intensity);
//     }
//   }
//   return rgb_matrix_check_finished_leds(led_max);
// }

// see gradient_left_right_anim.h for inspo
static bool sick_tricks(effect_params_t *params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    // change the middle number, suggested values are 120 or 90
    uint8_t range = (uint8_t)(256ul * 90ul / 360ul);
    HSV     hsv   = rgb_matrix_config.hsv;
    uint8_t lower = hsv.h - range / 2;
    //  364 * (scale / 256)
    uint8_t scale = scale8(64, range);
    for (uint8_t i = led_min; i < led_max; i++) {
        RGB_MATRIX_TEST_LED_FLAGS();

        // if (HAS_FLAGS(g_led_config.flags[i], LED_FLAG_UNDERGLOW)) {
        //     rgb_matrix_set_color(i, 255, 0, 0);
        //     continue;
        // }

        // https://docs.qmk.fm/features/rgb_matrix#common-configuration
        // The x range will be 0..224, map this to 0..7
        // Relies on hue being 8-bit and wrapping
        hsv.h   = lower + (scale * g_led_config.point[i].x >> 5);
        RGB rgb = rgb_matrix_hsv_to_rgb(hsv);
        rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
    }
    return rgb_matrix_check_finished_leds(led_max);
}

#endif
