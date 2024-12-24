/* Copyright 2019 Thomas Baart <thomas@splitkb.com>
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

/*
// Macro
enum custom_keycodes {
  M_LEFT_DESKTOP = SAFE_RANGE,
  M_RIGHT_DESKTOP,
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case M_LEFT_DESKTOP:
      if (record->event.pressed) {
        // switch to left desktop
        SEND_STRING(SS_LCTL(SS_TAP(X_LEFT)));
      }
      break;
    case M_RIGHT_DESKTOP:
      if (record->event.pressed) {
        // switch to right desktop
        SEND_STRING(SS_LCTL(SS_TAP(X_RIGHT)));
      }
      break;

  }
  return true;
};
// --- end macro ---
*/

typedef struct {
  bool is_press_action;
  int state;
} tap;

//Define a type for as many tap dance states as you need
enum {
  SINGLE_TAP = 1,
  SINGLE_HOLD = 2,
  DOUBLE_TAP = 3,
  DOUBLE_HOLD = 4
};

enum layers {
    _QWR = 0,
    _MED,
    _NAV,
    _MSE,
    _SYM,
    _NUM,
    _FUN
};

//Declare the functions to be used with your tap dance key(s)

//Function associated with all tap dances
int cur_dance (tap_dance_state_t *state);

//Determine the current tap dance state
int cur_dance (tap_dance_state_t *state) {
  if (state->count == 1) {
    if (!state->pressed) {
      return SINGLE_TAP;
    } else {
      return SINGLE_HOLD;
    }
  } else if (state->count == 2) {
    if (!state->pressed) {
      return DOUBLE_TAP;
    } else {
      return DOUBLE_HOLD;
    }
  }
  else return 8;
}

/*
// ---- ---- ---- ---- ----
//Functions associated with COLEMAK / QWERTY
void clmk_toggle (tap_dance_state_t *state, void *user_data);
void clmk_reset (tap_dance_state_t *state, void *user_data);

//Initialize tap structure associated with example tap dance key
static tap cmlk_tap_state = {
  .is_press_action = true,
  .state = 0
};

//single tap, single hold, double tap, double tap hold
// KC_UP    , KC_UP,     , Colemak toggle, _
//Toggle colemak / qwerty
void clmk_toggle (tap_dance_state_t *state, void *user_data) {
  cmlk_tap_state.state = cur_dance(state);
  switch (cmlk_tap_state.state) {
    case SINGLE_TAP:
      tap_code(KC_UP);
      break;
    case SINGLE_HOLD:
      tap_code(KC_UP);
      break;
    case DOUBLE_TAP:
      //check to see if the layer is already set
      if (layer_state_is(_COLEMAK)) {
        //if already set, then switch it off
        layer_off(_COLEMAK);
      } else {
        //if not already set, then switch the layer on
        layer_on(_COLEMAK);
      }
      break;
  }
}


void clmk_reset (tap_dance_state_t *state, void *user_data) {
  // if key is held down it should work fine as LSFT
  if (cmlk_tap_state.state==SINGLE_HOLD) {
    tap_code(KC_LSFT);
  }
}
*/

// ---- ---- ---- ---- ----
//Functions associated with shift - ctrl tap dance
void shift_ctrl_toggle (tap_dance_state_t *state, void *user_data);
void shift_ctrl_reset (tap_dance_state_t *state, void *user_data);

//Initialize tap structure associated with example tap dance key
static tap lsft_tap_state = {
  .is_press_action = true,
  .state = 0
};

//single tap, single hold, double tap, double tap hold
//_         , Shift,     , _         , Ctrl
void shift_ctrl_toggle (tap_dance_state_t *state, void *user_data) {
  lsft_tap_state.state = cur_dance(state);
  switch (lsft_tap_state.state) {
    case SINGLE_TAP:
      register_code(KC_LSFT);
      break;
    case SINGLE_HOLD:
      register_code(KC_LSFT);
      break;
    case DOUBLE_TAP:
      register_code(KC_LSFT);
      break;
    case DOUBLE_HOLD:
      register_code(KC_LCTL);
      break;
  }
}

void shift_ctrl_reset (tap_dance_state_t *state, void *user_data) {
  // if key is held down it should work fine as LSFT
  if (lsft_tap_state.state==DOUBLE_HOLD) {
    tap_code(KC_LCTL);
  }
  if (lsft_tap_state.state==SINGLE_HOLD) {
    tap_code(KC_LSFT);
  }
  lsft_tap_state.state = 0;
}


// -------------------------------------------------

enum {
  TD_LE = 0,
  TD_GE,
  TD_GT, // ~ `
  TD_DU, // - _
  TD_QD, // ' "
  TD_SQ, // / ?
  TD_SC, // ; :
  TD_SHC, // Shift Ctrl
};

tap_dance_action_t tap_dance_actions[] = {
  // G Esc
  [TD_GE] = ACTION_TAP_DANCE_DOUBLE(KC_G, KC_ESC),
  // Left-Alt Esc
  [TD_LE] = ACTION_TAP_DANCE_DOUBLE(KC_LALT, KC_ESC),
  // ` ~
  [TD_GT] = ACTION_TAP_DANCE_DOUBLE(KC_GRV, KC_TILD),
  // - _
  [TD_DU] = ACTION_TAP_DANCE_DOUBLE(KC_MINS, KC_UNDS),
  // ' "
  [TD_QD] = ACTION_TAP_DANCE_DOUBLE(KC_QUOT, KC_DQUO),
  // / ?
  [TD_SQ] = ACTION_TAP_DANCE_DOUBLE(KC_SLSH, KC_QUES),
  // ; :
  [TD_SC] = ACTION_TAP_DANCE_DOUBLE(KC_SCLN, KC_COLN),
  // Shift Shift Ctrl _
  [TD_SHC] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, shift_ctrl_toggle, shift_ctrl_reset),
};

// Aliases for readability
#define QWERTY   DF(_QWR)

#define FKEYS    MO(_FUNCTION)

#define CTL_ESC  MT(MOD_LCTL, KC_ESC)
#define CTL_QUOT MT(MOD_RCTL, KC_QUOTE)
#define CTL_MINS MT(MOD_RCTL, KC_MINUS)
#define ALT_ENT  MT(MOD_LALT, KC_ENT)

#define BL_MED   LT(_MED, KC_ESC)
#define BL_NAV   LT(_NAV, KC_SPC)
#define BL_MSE   LT(_MSE, KC_TAB)
#define BL_SYM   LT(_SYM, KC_ENT)
#define BL_NUM   LT(_NUM, KC_BSPC)
#define BL_FUN   LT(_FUN, KC_DEL)

// home row mods
#define HR_AGUI LGUI_T(KC_A)
#define HR_SALT LALT_T(KC_S)
#define HR_DCTL LCTL_T(KC_D)
#define HR_FSFT LSFT_T(KC_F)
#define HR_JSFT LSFT_T(KC_J)
#define HR_KCTL LCTL_T(KC_K)
#define HR_LALT LALT_T(KC_L)
#define HR_SCGU LGUI_T(KC_SCLN)

// shift insert
#define SF_INS  LSFT(KC_INS)

// Note: LAlt/Enter (ALT_ENT) is not the same thing as the keyboard shortcut Alt+Enter.
// The notation `mod/tap` denotes a key that activates the modifier `mod` when held down, and
// produces the key `tap` when tapped (i.e. pressed and released).

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/*
 * Base Layer: QWERTY
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |  ` ~   |   Q  |   W  |   E  |   R  |   T  |                              |   Y  |   U  |   I  |   O  |   P  |  Bksp  |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |    Tab |   A  |   S  |   D  |   F  |   G  |                              |   H  |   J  |   K  |   L  | ;  : |Ctrl/' "|
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * | LShift |   Z  |   X  |   C  |   V  |   B  | LCTL |alt/esc|  |F-keys|  ] } |   N  |   M  | ,  < | . > | /  ? | RShift |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |   Up | Down | Bksp | Space| Nav  |  | Sym  | Space| AltGr| RGUI | Menu |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
  [_QWR] = LAYOUT(
  //┌─────────┬─────────┬─────────┬─────────┬─────────┬─────────┐                                             ┌─────────┬─────────┬─────────┬─────────┬─────────┬─────────┐
     TD(TD_GT), KC_Q    , KC_W    , KC_E    , KC_R    , KC_T    ,                                              KC_Y     , KC_U    , KC_I    , KC_O    , KC_P    ,TD(TD_DU),
  //├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤                                             ├─────────┼─────────┼─────────┼─────────┼─────────┤─────────┤
     KC_TAB   , HR_AGUI , HR_SALT , HR_DCTL , HR_FSFT , KC_G    ,                                              KC_H     , HR_JSFT , HR_KCTL , HR_LALT , HR_SCGU ,TD(TD_QD),                    
  //├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┼─────────┬─────────┐     ┌─────────┬─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤─────────┤
     KC_LSFT  , KC_Z    , KC_X    , KC_C    , KC_V    , KC_B    , KC_NO   , KC_NO   ,       KC_NO   , KC_NO   , KC_N    , KC_M    , KC_COMM , KC_DOT  ,TD(TD_SQ), KC_RSFT ,
  //└─────────┴─────────┴─────────┼─────────┼─────────┼─────────┼─────────┬─────────┤     ├─────────┬─────────┼─────────┼─────────┼─────────┼─────────┴─────────┴─────────┘
                                    KC_NO   , KC_NO   , BL_MED  , BL_NAV  , BL_MSE  ,       BL_SYM  , BL_NUM  , BL_FUN  , KC_NO   , KC_NO
  //                              └─────────┴─────────┴─────────┴─────────┴─────────┘     └─────────┴─────────┴─────────┴─────────┴─────────┘
  ),

  [_MED] = LAYOUT( // Media keys
  //┌─────────┬─────────┬─────────┬─────────┬─────────┬─────────┐                                             ┌─────────┬─────────┬─────────┬─────────┬─────────┬─────────┐
     KC_NO    , KC_NO   , KC_NO   , KC_NO   , KC_NO   , KC_NO   ,                                              KC_NO    , KC_NO   , KC_NO   , KC_O    , KC_NO   , KC_NO   ,
  //├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤                                             ├─────────┼─────────┼─────────┼─────────┼─────────┤─────────┤
     KC_NO    , KC_NO   , KC_NO   , KC_NO   , KC_NO   , KC_NO   ,                                              KC_NO    , KC_MPRV , KC_VOLD , KC_VOLU ,KC_MNXT  , KC_NO   ,                    
  //├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┼─────────┬─────────┐     ┌─────────┬─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤─────────┤
     _______  , KC_NO   , KC_NO   , KC_NO   , KC_NO   , KC_NO   , KC_NO   , KC_NO   ,       KC_NO   , KC_NO   , KC_NO   , KC_NO   , KC_NO   , KC_NO   , KC_NO   , _______ ,
  //└─────────┴─────────┴─────────┼─────────┼─────────┼─────────┼─────────┬─────────┤     ├─────────┬─────────┼─────────┼─────────┼─────────┼─────────┴─────────┴─────────┘
                                    KC_NO   , KC_NO   , KC_NO   , KC_NO   , KC_NO   ,       KC_MSTP , KC_MPLY , KC_MUTE , KC_NO   , KC_NO
  //                              └─────────┴─────────┴─────────┴─────────┴─────────┘     └─────────┴─────────┴─────────┴─────────┴─────────┘
  ),
  
  [_NAV] = LAYOUT( // Navigation keys
  //┌─────────┬─────────┬─────────┬─────────┬─────────┬─────────┐                                             ┌─────────┬─────────┬─────────┬─────────┬─────────┬─────────┐
     KC_NO    , KC_NO   , KC_NO   , KC_NO   , KC_NO   , KC_NO   ,                                              KC_AGAIN , KC_PASTE, KC_COPY , KC_CUT  , KC_UNDO , KC_NO   ,
  //├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤                                             ├─────────┼─────────┼─────────┼─────────┼─────────┤─────────┤
     KC_NO    , KC_NO   , KC_NO   , KC_NO   , KC_NO   , KC_NO   ,                                              KC_CAPS  , KC_LEFT , KC_DOWN , KC_UP   ,KC_RIGHT , KC_NO   ,                    
  //├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┼─────────┬─────────┐     ┌─────────┬─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤─────────┤
     _______  , KC_NO   , KC_NO   , KC_NO   , KC_NO   , KC_NO   , KC_NO   , KC_NO   ,       KC_NO   , KC_NO   , SF_INS  , KC_HOME , KC_PGDN , KC_PGUP , KC_END  , _______ ,
  //└─────────┴─────────┴─────────┼─────────┼─────────┼─────────┼─────────┬─────────┤     ├─────────┬─────────┼─────────┼─────────┼─────────┼─────────┴─────────┴─────────┘
                                    KC_NO   , KC_NO   , KC_NO   , KC_NO   , KC_NO   ,       KC_ENT , KC_BSPC  , KC_DEL  , KC_NO   , KC_NO
  //                              └─────────┴─────────┴─────────┴─────────┴─────────┘     └─────────┴─────────┴─────────┴─────────┴─────────┘
  ),
  
  [_MSE] = LAYOUT( // Mouse
  //┌─────────┬─────────┬─────────┬─────────┬─────────┬─────────┐                                             ┌─────────┬─────────┬─────────┬─────────┬─────────┬─────────┐
     KC_NO    , KC_NO   , KC_NO   , KC_NO   , KC_NO   , KC_NO   ,                                              KC_AGAIN , KC_PASTE, KC_COPY , KC_CUT  , KC_UNDO , KC_NO   ,
  //├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤                                             ├─────────┼─────────┼─────────┼─────────┼─────────┤─────────┤
     KC_NO    , KC_NO   , KC_NO   , KC_NO   , KC_NO   , KC_NO   ,                                              KC_NO    , MS_LEFT , MS_DOWN , MS_UP   , MS_RGHT , KC_NO   ,                    
  //├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┼─────────┬─────────┐     ┌─────────┬─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤─────────┤
     _______  , KC_NO   , KC_NO   , KC_NO   , KC_NO   , KC_NO   , KC_NO   , KC_NO   ,       KC_NO   , KC_NO   , KC_NO   , MS_WHLL , MS_WHLD , MS_WHLU , MS_WHLR , _______ ,
  //└─────────┴─────────┴─────────┼─────────┼─────────┼─────────┼─────────┬─────────┤     ├─────────┬─────────┼─────────┼─────────┼─────────┼─────────┴─────────┴─────────┘
                                    KC_NO   , KC_NO   , KC_NO   , KC_NO   , KC_NO   ,       MS_BTN2 , MS_BTN1 , MS_BTN3 , KC_NO   , KC_NO
  //                              └─────────┴─────────┴─────────┴─────────┴─────────┘     └─────────┴─────────┴─────────┴─────────┴─────────┘
  ),
  
  [_SYM] = LAYOUT( // Symbols
  //┌─────────┬─────────┬─────────┬─────────┬─────────┬─────────┐                                             ┌─────────┬─────────┬─────────┬─────────┬─────────┬─────────┐
     KC_NO    , KC_LBRC , KC_AMPR , KC_ASTR , KC_LPRN , KC_RBRC ,                                              KC_NO    , KC_NO   , KC_NO   , KC_NO   , KC_NO   , KC_NO   ,
  //├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤                                             ├─────────┼─────────┼─────────┼─────────┼─────────┤─────────┤
     KC_NO    , KC_COLN , KC_DLR  , KC_PERC , KC_CIRC , KC_PLUS ,                                              KC_NO    , KC_NO   , KC_NO   , KC_NO   , KC_NO   , KC_NO   ,                    
  //├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┼─────────┬─────────┐     ┌─────────┬─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤─────────┤
     _______  , KC_TILD , KC_EXLM , KC_AT   , KC_HASH , KC_PIPE , KC_ASTR , KC_BSLS ,       KC_NO   , KC_NO   , KC_NO   , KC_NO   , KC_NO   , KC_NO   , KC_NO   , _______ ,
  //└─────────┴─────────┴─────────┼─────────┼─────────┼─────────┼─────────┬─────────┤     ├─────────┬─────────┼─────────┼─────────┼─────────┼─────────┴─────────┴─────────┘
                                    KC_NO   , KC_NO   , KC_RPRN , KC_UNDS , KC_PLUS ,       KC_NO   , KC_NO   , KC_NO   , KC_NO   , KC_NO
  //                              └─────────┴─────────┴─────────┴─────────┴─────────┘     └─────────┴─────────┴─────────┴─────────┴─────────┘
  ),
  
  [_NUM] = LAYOUT( // Numbers
  //┌─────────┬─────────┬─────────┬─────────┬─────────┬─────────┐                                             ┌─────────┬─────────┬─────────┬─────────┬─────────┬─────────┐
     KC_PLUS  , KC_LBRC , KC_7    , KC_8    , KC_9    , KC_RBRC ,                                              KC_NO    , KC_NO   , KC_NO   , KC_NO   , KC_NO   , KC_NO   ,
  //├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤                                             ├─────────┼─────────┼─────────┼─────────┼─────────┤─────────┤
     KC_MINS  , KC_COLN , KC_4    , KC_5    , KC_6    , KC_EQL  ,                                              KC_NO    , KC_NO   , KC_NO   , KC_NO   , KC_NO   , KC_NO   ,                    
  //├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┼─────────┬─────────┐     ┌─────────┬─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤─────────┤
     _______  , KC_GRV  , KC_1    , KC_2    , KC_3    , KC_BSLS , KC_NO   , KC_NO   ,       KC_NO   , KC_NO   , KC_NO   , KC_NO   , KC_NO   , KC_NO   , KC_NO   , _______ ,
  //└─────────┴─────────┴─────────┼─────────┼─────────┼─────────┼─────────┬─────────┤     ├─────────┬─────────┼─────────┼─────────┼─────────┼─────────┴─────────┴─────────┘
                                    KC_NO   , KC_NO   , KC_0    , KC_MINS , KC_EQL  ,       KC_NO   , KC_NO   , KC_NO   , KC_NO   , KC_NO
  //                              └─────────┴─────────┴─────────┴─────────┴─────────┘     └─────────┴─────────┴─────────┴─────────┴─────────┘
  ),
  
  [_FUN] = LAYOUT( // Function keys
  //┌─────────┬─────────┬─────────┬─────────┬─────────┬─────────┐                                             ┌─────────┬─────────┬─────────┬─────────┬─────────┬─────────┐
     KC_NO    , KC_F12  , KC_F7   , KC_F8   , KC_F9   , KC_PSCR ,                                              KC_NO    , KC_NO   , KC_NO   , KC_NO   , KC_NO   , KC_NO   ,
  //├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤                                             ├─────────┼─────────┼─────────┼─────────┼─────────┤─────────┤
     KC_TAB   , KC_F11  , KC_F4   , KC_F5   , KC_F6   , KC_SCRL ,                                              KC_NO    , KC_NO   , KC_NO   , KC_NO   , KC_NO   , KC_NO   ,                    
  //├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┼─────────┬─────────┐     ┌─────────┬─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤─────────┤
     _______  , KC_F10  , KC_F1   , KC_F2   , KC_F3   , KC_PAUS , KC_NO   , KC_NO   ,       KC_NO   , KC_NO   , KC_NO   , KC_NO   , KC_NO   , KC_NO   , KC_NO   , _______ ,
  //└─────────┴─────────┴─────────┼─────────┼─────────┼─────────┼─────────┬─────────┤     ├─────────┬─────────┼─────────┼─────────┼─────────┼─────────┴─────────┴─────────┘
                                    KC_NO   , KC_NO   , KC_LGUI , KC_SPC  , KC_TAB  ,       KC_NO   , KC_NO   , KC_NO   , KC_NO   , KC_NO
  //                              └─────────┴─────────┴─────────┴─────────┴─────────┘     └─────────┴─────────┴─────────┴─────────┴─────────┘
  ),
};

/* The default OLED and rotary encoder code can be found at the bottom of qmk_firmware/keyboards/splitkb/kyria/rev1/rev1.c
 * These default settings can be overriden by your own settings in your keymap.c
 * For your convenience, here's a copy of those settings so that you can uncomment them if you wish to apply your own modifications.
 * DO NOT edit the rev1.c file; instead override the weakly defined default functions by your own.
 */

#ifdef OLED_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) { return OLED_ROTATION_180; }

bool oled_task_user(void) {
    if (is_keyboard_master()) {
        // revision and keymap
        oled_write_P(PSTR("#> ronajon rev0.6a\n"), false);

        // QMK Logo and version information
        // clang-format off
        static const char PROGMEM qmk_logo[] = {
            0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,0x90,0x91,0x92,0x93,0x94,
            0xa0,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa,0xab,0xac,0xad,0xae,0xaf,0xb0,0xb1,0xb2,0xb3,0xb4,
            0xc0,0xc1,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,0xcb,0xcc,0xcd,0xce,0xcf,0xd0,0xd1,0xd2,0xd3,0xd4,0};
        // clang-format on

        oled_write_P(qmk_logo, false);
        oled_write_P(PSTR("Kyria rev1.0\n\n"), false);

        // Host Keyboard Layer Status
        oled_write_P(PSTR("Layer: "), false);
        switch (get_highest_layer(layer_state|default_layer_state)) {
            case _QWR:
                oled_write_P(PSTR("QWERTY\n"), false);
                break;
            case _MED:
                oled_write_P(PSTR("Media\n"), false);
                break;
            case _NAV:
                oled_write_P(PSTR("Navigation\n"), false);
                break;
            case _MSE:
                oled_write_P(PSTR("Mouse\n"), false);
                break;
            case _SYM:
                oled_write_P(PSTR("Symbols\n"), false);
                break;
            case _NUM:
                oled_write_P(PSTR("Numbers\n"), false);
                break;
            case _FUN:
                oled_write_P(PSTR("Function\n"), false);
                break;
            default:
                oled_write_P(PSTR("Undefined\n"), false);
        }

        // Write host Keyboard LED Status to OLEDs
        led_t led_usb_state = host_keyboard_led_state();
        oled_write_P(led_usb_state.num_lock    ? PSTR("NUMLCK ") : PSTR("       "), false);
        oled_write_P(led_usb_state.caps_lock   ? PSTR("CAPLCK ") : PSTR("       "), false);
        oled_write_P(led_usb_state.scroll_lock ? PSTR("SCRLCK ") : PSTR("       "), false);
    } else {
        // oled_write_P(PSTR("$> "), false);
        // clang-format off
        static const char PROGMEM kyria_logo[] = {
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,192,224,240,112,120, 56, 60, 28, 30, 14, 14, 14,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7, 14, 14, 14, 30, 28, 60, 56,120,112,240,224,192,128,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,192,224,240,124, 62, 31, 15,  7,  3,  1,128,192,224,240,120, 56, 60, 28, 30, 14, 14,  7,  7,135,231,127, 31,255,255, 31,127,231,135,  7,  7, 14, 14, 30, 28, 60, 56,120,240,224,192,128,  1,  3,  7, 15, 31, 62,124,240,224,192,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,240,252,255, 31,  7,  1,  0,  0,192,240,252,254,255,247,243,177,176, 48, 48, 48, 48, 48, 48, 48,120,254,135,  1,  0,  0,255,255,  0,  0,  1,135,254,120, 48, 48, 48, 48, 48, 48, 48,176,177,243,247,255,254,252,240,192,  0,  0,  1,  7, 31,255,252,240,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,255,255,255,  0,  0,  0,  0,  0,254,255,255,  1,  1,  7, 30,120,225,129,131,131,134,134,140,140,152,152,177,183,254,248,224,255,255,224,248,254,183,177,152,152,140,140,134,134,131,131,129,225,120, 30,  7,  1,  1,255,255,254,  0,  0,  0,  0,  0,255,255,255,  0,  0,  0,  0,255,255,  0,  0,192,192, 48, 48,  0,  0,240,240,  0,  0,  0,  0,  0,  0,240,240,  0,  0,240,240,192,192, 48, 48, 48, 48,192,192,  0,  0, 48, 48,243,243,  0,  0,  0,  0,  0,  0, 48, 48, 48, 48, 48, 48,192,192,  0,  0,  0,  0,  0,
            0,  0,  0,255,255,255,  0,  0,  0,  0,  0,127,255,255,128,128,224,120, 30,135,129,193,193, 97, 97, 49, 49, 25, 25,141,237,127, 31,  7,255,255,  7, 31,127,237,141, 25, 25, 49, 49, 97, 97,193,193,129,135, 30,120,224,128,128,255,255,127,  0,  0,  0,  0,  0,255,255,255,  0,  0,  0,  0, 63, 63,  3,  3, 12, 12, 48, 48,  0,  0,  0,  0, 51, 51, 51, 51, 51, 51, 15, 15,  0,  0, 63, 63,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 48, 48, 63, 63, 48, 48,  0,  0, 12, 12, 51, 51, 51, 51, 51, 51, 63, 63,  0,  0,  0,  0,  0,
            0,  0,  0,  0, 15, 63,255,248,224,128,  0,  0,  3, 15, 63,127,255,239,207,141, 13, 12, 12, 12, 12, 12, 12, 12, 30,127,225,128,  0,  0,255,255,  0,  0,128,225,127, 30, 12, 12, 12, 12, 12, 12, 12, 13,141,207,239,255,127, 63, 15,  3,  0,  0,128,224,248,255, 63, 15,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  3,  7, 15, 62,124,248,240,224,192,128,  1,  3,  7, 15, 30, 28, 60, 56,120,112,112,224,224,225,231,254,248,255,255,248,254,231,225,224,224,112,112,120, 56, 60, 28, 30, 15,  7,  3,  1,128,192,224,240,248,124, 62, 15,  7,  3,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  3,  7, 15, 14, 30, 28, 60, 56,120,112,112,112,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,112,112,112,120, 56, 60, 28, 30, 14, 15,  7,  3,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
        };
        // clang-format on
        oled_write_raw_P(kyria_logo, sizeof(kyria_logo));
    }
    return false;
}
#endif

/* DELETE THIS LINE TO UNCOMMENT (1/2)
#ifdef ENCODER_ENABLE
bool encoder_update_user(uint8_t index, bool clockwise) {

    if (index == 0) {
        // Volume control
        if (clockwise) {
            tap_code(KC_VOLU);
        } else {
            tap_code(KC_VOLD);
        }
    } else if (index == 1) {
        // Page up/Page down
        if (clockwise) {
            tap_code(KC_PGDN);
        } else {
            tap_code(KC_PGUP);
        }
    }
    return false;
}
#endif
DELETE THIS LINE TO UNCOMMENT (2/2) */
