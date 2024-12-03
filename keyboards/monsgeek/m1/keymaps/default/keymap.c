/* Copyright (C) 2023 jonylee@hfd
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include QMK_KEYBOARD_H

enum __layers {
    BASE,
    WASD,
    FUNC
};

#define KC_TASK LGUI(KC_TAB)
#define KC_FLXP LGUI(KC_E)

enum custom_keycodes {
    QMKADDR = SAFE_RANGE,
    QMKMVGR,
    JIGGLE, // FN - J
};

const key_override_t home_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_HOME, LSFT(KC_INS));

// This globally defines all key overrides to be used
const key_override_t *key_overrides[] = {
	&home_key_override
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
	// mouse jiggler
  if (record->event.pressed) {
    static deferred_token token = INVALID_DEFERRED_TOKEN;
    static report_mouse_t report = {0};
    if (token) {
      // If jiggler is currently running, stop when any key is pressed.
      cancel_deferred_exec(token);
      token = INVALID_DEFERRED_TOKEN;
      report = (report_mouse_t){};  // Clear the mouse.
      host_mouse_send(&report);
    } else if (keycode == JIGGLE) {

      uint32_t jiggler_callback(uint32_t trigger_time, void* cb_arg) {
        // Deltas to move in a circle of radius 20 pixels over 32 frames.
        static const int8_t deltas[32] = {
            0, -1, -2, -2, -3, -3, -4, -4, -4, -4, -3, -3, -2, -2, -1, 0,
            0, 1, 2, 2, 3, 3, 4, 4, 4, 4, 3, 3, 2, 2, 1, 0};
        static uint8_t phase = 0;
        // Get x delta from table and y delta by rotating a quarter cycle.
        report.x = deltas[phase];
        report.y = deltas[(phase + 8) & 31];
        phase = (phase + 1) & 31;
        host_mouse_send(&report);
        return 16;  // Call the callback every 16 ms.
      }

      token = defer_exec(1, jiggler_callback, NULL);  // Schedule callback.
    }
  }
  
  // other macros
    switch (keycode) {
        case QMKADDR:
            if (record->event.pressed) {
                SEND_STRING("Ronald Jonkman\nJaromirgaarde 140\n7329CM Apeldoorn\n");
            }
            return false;
        case QMKMVGR:
            if (record->event.pressed) {
                SEND_STRING("Met vriendelijke groet,\nRonald Jonkman\n");
            }
            return false;
    }
  return true;
}

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [BASE] = LAYOUT_all( /* Base */
        KC_ESC,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_DEL,           KC_MUTE,  // FUNCTION KEYS
        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,          KC_HOME,  // NUMBER
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,          KC_PGUP,  // ALPHAS1
        KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_NUHS, KC_ENT,           KC_PGDN,  // ALPHAS2
        KC_LSFT, KC_NUBS, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,          KC_RSFT, KC_UP  , KC_END ,  // ALPHAS3
        KC_LCTL, KC_LGUI, KC_LALT,                   KC_SPC,                             KC_RALT,MO(FUNC), KC_RCTL,          KC_LEFT, KC_DOWN, KC_RGHT), // MODIFIERS
        
    [WASD] = LAYOUT_all( /* WASD/↑←↓→ */
    //     ~        1         2       3        4        5        6        7        8         9       0        -        =         BCSPC           HOME
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,  // FUNCTION KEYS
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,  // NUMBER
        _______, _______, KC_UP,   _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,  // ALPHAS1
        _______, KC_LEFT, KC_DOWN, KC_RGHT, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,  // ALPHAS2
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, KC_W,    _______,  // ALPHAS3
        _______, _______, _______,                   _______,                            _______,MO(FUNC),DF(BASE),          KC_A   , KC_S,    KC_D  ),    // MODIFIERS

    [FUNC] = LAYOUT_all( /* Functions */
        QK_BOOT, KC_MUTE, KC_VOLD, KC_VOLU, _______, QMKADDR, QMKMVGR, _______, _______, _______, _______, _______, _______, _______,          RGB_MOD,  // FUNCTION KEYS
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, RGB_SPD, RGB_SPI, _______,          KC_INS ,   // NUMBER
        _______, _______,TG(WASD), _______, _______, _______, _______, _______, KC_INS , _______, KC_PSCR, _______, _______, RGB_MOD,          _______,  // ALPHAS1
        _______, _______, _______, _______, _______, _______, _______, JIGGLE , _______, RGB_TOG, _______, _______, RGB_MOD, RGB_HUI,          _______,  // ALPHAS2
        _______, _______, _______, _______, KC_CALC, _______, _______, _______, KC_MUTE, KC_VOLD, KC_VOLU, _______,          _______, RGB_VAI, _______,  // ALPHAS3
        _______, _______, _______,                   _______,                            _______, _______, _______,          RGB_SAD, RGB_VAD, RGB_SAI), // MODIFIERS
        

};


// clang-format off
#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [BASE] = { ENCODER_CCW_CW(KC_VOLU, KC_VOLD) },
    [WASD] = { ENCODER_CCW_CW(KC_VOLU, KC_VOLD) },
    [FUNC] = { ENCODER_CCW_CW(RGB_MOD, RGB_RMOD) }
};
#endif

