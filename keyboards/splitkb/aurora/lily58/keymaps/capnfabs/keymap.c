#include QMK_KEYBOARD_H

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	[0] = LAYOUT(
		KC_ESC,		KC_1,	KC_2,	KC_3,	KC_4,	KC_5,					KC_6,		KC_7,	KC_8,		KC_9,	KC_0,		KC_MINS,
		KC_TAB,		KC_Q,	KC_W, 	KC_E, 	KC_R, 	KC_T,					KC_Y,		KC_U,	KC_I,		KC_O,	KC_P,		KC_EQL,
		MO(1),		KC_A,	KC_S,	KC_D,	KC_F,	KC_G,					KC_H,		KC_J,	KC_K,		KC_L,	KC_SCLN,	KC_ENT,
		KC_LSFT,	KC_Z,	KC_X,	KC_C,	KC_V,	KC_B,	KC_GRV,KC_NO,	KC_N, 		KC_M, 	KC_COMM,	KC_DOT,	KC_SLSH,	KC_BSLS,
								KC_LCTL, KC_LALT, KC_LGUI,	KC_SPC,KC_SPC,	KC_QUOT, KC_LBRC, KC_RBRC),
	[1] = LAYOUT(
		_______, KC_F1,	KC_F2, KC_F3, KC_F4, KC_F5,			 KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11,
		TG(3), XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_UP, XXXXXXX, XXXXXXX, KC_F12,
		_______, KC_LSFT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_LEFT, KC_DOWN, KC_RIGHT, XXXXXXX, XXXXXXX,
		_______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
						_______, _______, _______, KC_BSPC, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX),
	[2] = LAYOUT(_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_GRV, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, _______, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, XXXXXXX, KC_LEFT, KC_DOWN, KC_UP, KC_RGHT, XXXXXXX, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, _______, _______, KC_PLUS, KC_MINS, KC_EQL, KC_LBRC, KC_RBRC, KC_BSLS, _______, _______, MO(3), _______, _______, _______, _______, _______),
	[3] = LAYOUT(
		TG(3),	XXXXXXX,XXXXXXX,XXXXXXX, XXXXXXX, XXXXXXX,						XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
		XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,					XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
		XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, 					XXXXXXX, XXXXXXX, RGB_TOG, RGB_HUI, RGB_SAI, RGB_VAI,
		XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, RGB_MOD, RGB_HUD, RGB_SAD, RGB_VAD,
		_______, _______, _______, _______, _______, _______, _______, _______)
};

void keyboard_pre_init_user(void) {
  // Turn off the LED, incantation via
  // https://docs.splitkb.com/hc/en-us/articles/5799711553820-Power-LED
  setPinOutput(24);
  writePinHigh(24);
}

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
	uint8_t layer = get_highest_layer(layer_state);
    if (layer <= 0) {
		// nothing to do
		return false;
	}

	for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
		for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
			uint8_t index = g_led_config.matrix_co[row][col];

			if (index >= led_min && index < led_max && index != NO_LED) {
				uint16_t keycode = keymap_key_to_keycode(layer, (keypos_t){col,row});
				if (keycode >= KC_F1 && keycode <= KC_F12) {
					// Function key
					rgb_matrix_set_color(index, RGB_CYAN);
				} else if (keycode >= KC_RIGHT && keycode <= KC_UP) {
					// it's an arrow, see keycodes.h for ordering
					rgb_matrix_set_color(index, RGB_PURPLE);
				}
				else if (keycode > KC_TRNS) {
					rgb_matrix_set_color(index, RGB_GREEN);
				} else {
					// trying to switch off other keys does not work
					rgb_matrix_set_color(index, RGB_BLACK);
				}
			}
		}
	}
    return false;
}
