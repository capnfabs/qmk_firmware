#include QMK_KEYBOARD_H
#include <lib/lib8tion/lib8tion.h>
#include "transactions.h"

enum layers {
  _QWERTY,
  _COLEMAK,
  _CURSED_COLEMAK_TRANSPOSED, // for when you want to type QWERTY but your computer is set to Colemak
  _FUNCTION,
  _KEYBOAD_SETTINGS,
};

#define NUM_DEFAULT_LAYERS 3

enum custom_keycodes {
  OLED_UP = SAFE_RANGE,
  OLED_DOWN,
  SWITCH_DEFAULT_LAYOUT,
};

typedef struct synced_config_t {
    uint8_t oled_brightness;
} synced_config_t;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	[_QWERTY] = LAYOUT(
		KC_ESC,		KC_1,	KC_2,	KC_3,	KC_4,	KC_5,					KC_6,		KC_7,	KC_8,		KC_9,	KC_0,		KC_MINS,
		KC_TAB,		KC_Q,	KC_W, 	KC_E, 	KC_R, 	KC_T,					KC_Y,		KC_U,	KC_I,		KC_O,	KC_P,		KC_EQL,
		MO(_FUNCTION),		KC_A,	KC_S,	KC_D,	KC_F,	KC_G,					KC_H,		KC_J,	KC_K,		KC_L,	KC_SCLN,	KC_ENT,
		KC_LSFT,	KC_Z,	KC_X,	KC_C,	KC_V,	KC_B,	KC_GRV,KC_NO,	KC_N, 		KC_M, 	KC_COMM,	KC_DOT,	KC_LBRC,	KC_RBRC,
								KC_LCTL, KC_LALT, KC_LGUI,	KC_SPC,KC_SPC,	KC_QUOT, KC_SLSH,	KC_BSLS),
    [_COLEMAK] = LAYOUT(
		KC_ESC,		KC_1,	KC_2,	KC_3,	KC_4,	KC_5,					KC_6,		KC_7,	KC_8,		KC_9,	KC_0,		KC_MINS,
		KC_TAB,		KC_Q,	KC_W, 	KC_F, 	KC_P, 	KC_G,					KC_J,		KC_L,	KC_U,		KC_Y,	KC_SCLN,	KC_EQL,
		MO(_FUNCTION),		KC_A,	KC_R,	KC_S,	KC_T,	KC_D,					KC_H,		KC_N,	KC_E,		KC_I,	KC_O,	KC_ENT,
		KC_LSFT,	KC_Z,	KC_X,	KC_C,	KC_V,	KC_B,	KC_GRV,KC_NO,	KC_K, 		KC_M, 	KC_COMM,	KC_DOT,	KC_LBRC,	KC_RBRC,
								KC_LCTL, KC_LALT, KC_LGUI,	KC_SPC,KC_SPC,	KC_QUOT, KC_SLSH,	KC_BSLS),
    [_CURSED_COLEMAK_TRANSPOSED] = LAYOUT(
		KC_ESC,		KC_1,	KC_2,	KC_3,	KC_4,	KC_5,					KC_6,		KC_7,	KC_8,		KC_9,	KC_0,		KC_MINS,
		KC_TAB,		KC_Q,	KC_W, 	KC_K, 	KC_S, 	KC_F,					KC_O,		KC_I,	KC_L,		KC_SCLN,	KC_R,		KC_EQL,
		MO(_FUNCTION),		KC_A,	KC_D,	KC_G,	KC_E,	KC_T,					KC_H,		KC_Y,	KC_N,		KC_U,	KC_P,	KC_ENT,
		KC_LSFT,	KC_Z,	KC_X,	KC_C,	KC_V,	KC_B,	KC_GRV,KC_NO,	KC_J, 		KC_M, 	KC_COMM,	KC_DOT,	KC_LBRC,	KC_RBRC,
								KC_LCTL, KC_LALT, KC_LGUI,	KC_SPC,KC_SPC,	KC_QUOT, KC_SLSH,	KC_BSLS),
	[_FUNCTION] = LAYOUT(
		_______, KC_F1,	KC_F2, KC_F3, KC_F4, KC_F5,			 KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11,
		XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_UP, XXXXXXX, XXXXXXX, KC_F12,
		_______, KC_LSFT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_LEFT, KC_DOWN, KC_RIGHT, XXXXXXX, XXXXXXX,
		_______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, SWITCH_DEFAULT_LAYOUT, TG(_KEYBOAD_SETTINGS), XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
						_______, _______, _______, KC_BSPC, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX),
	[_KEYBOAD_SETTINGS] = LAYOUT(
		TG(_KEYBOAD_SETTINGS),	XXXXXXX,XXXXXXX,XXXXXXX, XXXXXXX, XXXXXXX,						OLED_UP, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
		XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,					OLED_DOWN, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
		XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, 					XXXXXXX, XXXXXXX, XXXXXXX, RGB_HUI, RGB_SAI, RGB_VAI,
		XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, RGB_TOG, XXXXXXX, XXXXXXX, XXXXXXX, RGB_HUD, RGB_SAD, RGB_VAD,
		_______, _______, _______, _______, _______, RGB_SPD, RGB_SPI, XXXXXXX)
};

void keyboard_pre_init_user(void) {
  // Turn off the LED, incantation via
  // https://docs.splitkb.com/hc/en-us/articles/5799711553820-Power-LED
  setPinOutput(24);
  writePinHigh(24);
}

#define DEG_TO_BYTE(deg) (deg*255/360)

HSV shift_hue_for_keycode(HSV hsv, uint16_t keycode) {
    HSV newHsv = hsv;
    if (keycode >= KC_F1 && keycode <= KC_F12) {
        // Function key
        newHsv.h -= DEG_TO_BYTE(60);
    } else if (keycode >=  KC_RIGHT && keycode <= KC_UP) {
        // it's an arrow, see keycodes.h for ordering
        newHsv.h += DEG_TO_BYTE(60);
    } else if (keycode == KC_NO) {
        newHsv.v = 0;
    } else if (keycode == RGB_HUI || keycode == RGB_HUD) {
        newHsv.h -= DEG_TO_BYTE(60);
    } else if (keycode == RGB_SAI || keycode == RGB_SAD) {
        newHsv.s = 0;
    } else if (keycode == RGB_VAI  || keycode == RGB_VAD) {
        newHsv.h += DEG_TO_BYTE(60);
    }
    else if (keycode > KC_TRNS) {
        // theme color
    }
    return newHsv;
}

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
	uint8_t layer = get_highest_layer(layer_state);
    if (layer <= 0) {
		// nothing to do
		return false;
	}

	HSV theme_color = rgb_matrix_config.hsv;

	for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
		for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
			uint8_t index = g_led_config.matrix_co[row][col];

			if (index >= led_min && index < led_max && index != NO_LED) {
				uint16_t keycode = keymap_key_to_keycode(layer, (keypos_t){col,row});
                if (keycode != KC_TRANSPARENT) {
                    HSV hsv = shift_hue_for_keycode(theme_color, keycode);
                    RGB rgb = hsv_to_rgb(hsv);
                    rgb_matrix_set_color(index, rgb.r, rgb.g, rgb.b);
                }
			}
		}
	}
    return false;
}

#define OLED_BRIGHTNESS_INCREMENT 32

synced_config_t g_synced_config = {
    .oled_brightness = 255
};

void oled_set_brightness_split(uint8_t brightness) {
    if (g_synced_config.oled_brightness == brightness) {
        return;
    }
    g_synced_config.oled_brightness = brightness;
    oled_set_brightness(brightness);
    // TODO save value into eeprom
    // https://docs.qmk.fm/feature_eeprom#persistent-configuration-eeprom
    // https://docs.qmk.fm/custom_quantum_functions#keyboard-initialization-code
}

void housekeeping_task_user(void) {
    if (is_keyboard_master()) {
        // Keep track of the last state, so that we can tell if we need to sync.
        static synced_config_t last_synced_config = {0};
        static uint32_t last_sync = 0;

        bool needs_sync = false;

        // Check if the state values are different.
        if (memcmp(&g_synced_config, &last_synced_config, sizeof(g_synced_config))) {
            needs_sync = true;
            memcpy(&last_synced_config, &g_synced_config, sizeof(g_synced_config));
        }
        // Sync every 500ms regardless of state change.
        if (timer_elapsed32(last_sync) > 500) {
            needs_sync = true;
        }

        // Perform the sync if requested.
        if (needs_sync) {
            if (transaction_rpc_send(USER_SYNC_KEEB_PARAMS, sizeof(g_synced_config), &g_synced_config)) {
                last_sync = timer_read32();
            }
        }
    }
}

void cycle_next_default_layer(void) {
    // only the first 3 layers are default candidates, so get the first one of
    // the three that's on
    uint8_t layer = get_highest_layer(default_layer_state & ((1 << NUM_DEFAULT_LAYERS) - 1));
    dprintf("highest layer: %d\n", layer);
    uint8_t next_layer = (layer + 1) % NUM_DEFAULT_LAYERS;
    default_layer_set(1 << next_layer);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case OLED_UP:
      if (record->event.pressed) {
        uint8_t brightness = qadd8(g_synced_config.oled_brightness, OLED_BRIGHTNESS_INCREMENT);
        oled_set_brightness_split(brightness);
      }
      return false; // Skip all further processing
    case OLED_DOWN:
      if (record->event.pressed) {
        uint8_t brightness = qsub8(g_synced_config.oled_brightness, OLED_BRIGHTNESS_INCREMENT);
        oled_set_brightness_split(brightness);
      }
      return false; // Skip all further processing
    case SWITCH_DEFAULT_LAYOUT:
      if (record->event.pressed) {
        cycle_next_default_layer();
      }
      return false; // Skip all further processing
    default:
      return true; // Process all other keycodes normally
  }
}

void user_synced_config_handler(uint8_t in_buflen, const void* in_data, uint8_t out_buflen, void* out_data) {
    const synced_config_t *msg = (const synced_config_t*) in_data;
    oled_set_brightness(msg->oled_brightness);
}

void keyboard_post_init_user(void) {
    transaction_register_rpc(USER_SYNC_KEEB_PARAMS, user_synced_config_handler);
    rgb_matrix_mode_noeeprom(RGB_MATRIX_CUSTOM_sick_tricks);
}
