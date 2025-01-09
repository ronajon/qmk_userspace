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


// declare function to detect SHIFT key pressed
bool is_shift_pressed = false;

// Process keycode to detect Shift press/release
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case KC_LSFT:
    case KC_RSFT:
      if (record->event.pressed) {
        is_shift_pressed = true;
      } else {
        is_shift_pressed = false;
      }
      break;
  }
  return true; // Continue processing the keycode
}

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
  _FUN,
  _RGB
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
#define BL_RGB   LT(_RGB, KC_NO)

// home row mods
#define HR_A_GUI LGUI_T(KC_A)
#define HR_S_ALT LALT_T(KC_S)
#define HR_D_CTL LCTL_T(KC_D)
#define HR_F_SFT LSFT_T(KC_F)
#define HR_J_SFT LSFT_T(KC_J)
#define HR_K_CTL LCTL_T(KC_K)
#define HR_L_ALT LALT_T(KC_L)
#define HR_S_CGU LGUI_T(KC_SCLN)

// shift insert
#define SF_INS  LSFT(KC_INS)

// CUT COPY PASTE
#define M_CUT    LCTL(KC_X)
#define M_COPY   LCTL(KC_C)
#define M_PASTE  LCTL(KC_P)
#define M_UNDO   LCTL(KC_Z)
#define M_REDO   LCTL(KC_Y)

// LIGHTING LAYERS
// Light LEDs 6 to 9 and 12 to 15 red when caps lock is active. Hard to ignore!
//  {0, 2, HSV_RED},       // Light 4 LEDs, starting with LED 6
//  {3, 1, HSV_GREEN},       // Light 4 LEDs, starting with LED 12
//  {9, 1, HSV_ORANGE},       // Light 4 LEDs, starting with LED 12
//  {13, 1, HSV_YELLOW}       // Light 									4 LEDs, starting with LED 12

//const rgblight_segment_t PROGMEM layer_0[] = RGBLIGHT_LAYER_SEGMENTS( {0, 1, HSV_RED}, {3, 1, HSV_GREEN}, {9, 1, HSV_ORANGE}, {13, 1, HSV_YELLOW} ); // _MED
const rgblight_segment_t PROGMEM layer_0[] = RGBLIGHT_LAYER_SEGMENTS( {0, 20, HSV_BLUE} ); // _MED
const rgblight_segment_t PROGMEM layer_1[] = RGBLIGHT_LAYER_SEGMENTS( {11, 1, HSV_CYAN} );  // _NAV
//const rgblight_segment_t PROGMEM layer_2[] = RGBLIGHT_LAYER_SEGMENTS( {0, 10, HSV_BLUE} ); // _MSE 0,10 works for right hand
const rgblight_segment_t PROGMEM layer_2[] = RGBLIGHT_LAYER_SEGMENTS( {0, 10, HSV_GOLD} ); // _MSE
const rgblight_segment_t PROGMEM layer_3[] = RGBLIGHT_LAYER_SEGMENTS( {4, 12, HSV_PINK} ); // _SYM
const rgblight_segment_t PROGMEM layer_4[] = RGBLIGHT_LAYER_SEGMENTS( {15, 2, HSV_GOLD} ); // _NUM
const rgblight_segment_t PROGMEM layer_5[] = RGBLIGHT_LAYER_SEGMENTS( {19, 2, HSV_YELLOW} ); //_FUN

// Now define the array of layers. Later layers take precedenceronst rgblight_segment_t* const PROGMEM my_rgb_layers[] = RGBLIGHT_LAYERS_LIST(
const rgblight_segment_t* const PROGMEM my_rgb_layers[] = RGBLIGHT_LAYERS_LIST(
  layer_0,
  layer_1,    // Overrides layer0 layer
  layer_2,    // Overrides other layers
  layer_3,    // Overrides other layers
  layer_4,    // Overrides other layers
  layer_5    // Overrides other layers
);

void keyboard_post_init_user(void) {
    // Enable the LED layers
    rgblight_layers = my_rgb_layers;
}

/*
bool led_update_user(led_t led_state) {
    rgblight_set_layer_state(0, led_state.caps_lock);
    return true;
}

layer_state_t default_layer_state_set_user(layer_state_t state) {
    rgblight_set_layer_state(1, layer_state_cmp(state, _QWR));
    return state;
}
*/

layer_state_t layer_state_set_user(layer_state_t state) {
  rgblight_set_layer_state(0, layer_state_cmp(state, _MED));
  rgblight_set_layer_state(1, layer_state_cmp(state, _NAV));
  rgblight_set_layer_state(2, layer_state_cmp(state, _MSE));
  rgblight_set_layer_state(3, layer_state_cmp(state, _SYM));
  rgblight_set_layer_state(4, layer_state_cmp(state, _NUM));
  rgblight_set_layer_state(5, layer_state_cmp(state, _FUN));
  return state;
}

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
     KC_TAB   , HR_A_GUI, HR_S_ALT, HR_D_CTL, HR_F_SFT, KC_G    ,                                              KC_H     , HR_J_SFT, HR_K_CTL, HR_L_ALT, HR_S_CGU,TD(TD_QD),                    
  //├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┼─────────┬─────────┐     ┌─────────┬─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤─────────┤
     KC_LSFT  , KC_Z    , KC_X    , KC_C    , KC_V    , KC_B    , BL_RGB  , KC_LGUI ,       KC_NO   , KC_NO   , KC_N    , KC_M    , KC_COMM , KC_DOT  ,TD(TD_SQ), KC_RSFT ,
  //└─────────┴─────────┴─────────┼─────────┼─────────┼─────────┼─────────┬─────────┤     ├─────────┬─────────┼─────────┼─────────┼─────────┼─────────┴─────────┴─────────┘
                                    KC_UP   , KC_DOWN , BL_MED  , BL_NAV  , BL_MSE  ,       BL_SYM  , BL_NUM  , BL_FUN  , KC_LEFT , KC_RGHT
  //                              └─────────┴─────────┴─────────┴─────────┴─────────┘     └─────────┴─────────┴─────────┴─────────┴─────────┘
  ),

  [_MED] = LAYOUT( // Media keys
  //┌─────────┬─────────┬─────────┬─────────┬─────────┬─────────┐                                             ┌─────────┬─────────┬─────────┬─────────┬─────────┬─────────┐
     KC_NO    , KC_NO   , KC_NO   , KC_NO   , KC_NO   , KC_NO   ,                                              KC_NO    , KC_NO   , KC_NO   , KC_NO   , KC_NO   , KC_NO   ,
  //├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤                                             ├─────────┼─────────┼─────────┼─────────┼─────────┤─────────┤
     KC_TAB   , KC_LGUI , KC_LALT , KC_LCTL , KC_LSFT , KC_NO   ,                                              KC_NO    , KC_MPRV , KC_VOLD , KC_VOLU ,KC_MNXT  , KC_NO   ,                    
  //├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┼─────────┬─────────┐     ┌─────────┬─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤─────────┤
     _______  , KC_NO   , KC_NO   , KC_NO   , KC_NO   , KC_NO   , KC_NO   , _______ ,       KC_NO   , KC_NO   , KC_NO    , KC_NO   , KC_NO   , KC_NO   , KC_NO   , KC_NO   ,
  //└─────────┴─────────┴─────────┼─────────┼─────────┼─────────┼─────────┬─────────┤     ├─────────┬─────────┼─────────┼─────────┼─────────┼─────────┴─────────┴─────────┘
                                    _______ , _______ , KC_NO   , KC_NO   , KC_NO   ,       KC_MSTP , KC_MPLY , KC_MUTE , _______ , _______
  //                              └─────────┴─────────┴─────────┴─────────┴─────────┘     └─────────┴─────────┴─────────┴─────────┴─────────┘
  ),
  
  [_NAV] = LAYOUT( // Navigation keys
  //┌─────────┬─────────┬─────────┬─────────┬─────────┬─────────┐                                             ┌─────────┬─────────┬─────────┬─────────┬─────────┬─────────┐
     KC_NO    , KC_NO   , KC_NO   , KC_NO   , KC_NO   , KC_NO   ,                                              KC_AGAIN , KC_PASTE, KC_COPY , KC_CUT  , KC_UNDO , KC_NO   ,
  //├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤                                             ├─────────┼─────────┼─────────┼─────────┼─────────┤─────────┤
     KC_TAB   , KC_LGUI , KC_LALT , KC_LCTL , KC_LSFT , KC_NO   ,                                              KC_CAPS  , KC_LEFT , KC_DOWN , KC_UP   ,KC_RIGHT , KC_NO   ,                    
  //├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┼─────────┬─────────┐     ┌─────────┬─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤─────────┤
     _______  , KC_NO   , KC_NO   , KC_NO   , KC_NO   , KC_NO   , KC_NO   , _______ ,       KC_NO   , KC_NO   , SF_INS  , KC_HOME , KC_PGDN , KC_PGUP , KC_END  , _______ ,
  //└─────────┴─────────┴─────────┼─────────┼─────────┼─────────┼─────────┬─────────┤     ├─────────┬─────────┼─────────┼─────────┼─────────┼─────────┴─────────┴─────────┘
                                    _______ , _______ , KC_NO   , KC_NO   , KC_NO   ,       KC_ENT , KC_BSPC  , KC_DEL  , _______ , _______
  //                              └─────────┴─────────┴─────────┴─────────┴─────────┘     └─────────┴─────────┴─────────┴─────────┴─────────┘
  ),
  
  [_MSE] = LAYOUT( // Mouse
  //┌─────────┬─────────┬─────────┬─────────┬─────────┬─────────┐                                             ┌─────────┬─────────┬─────────┬─────────┬─────────┬─────────┐
     KC_NO    , KC_NO   , KC_NO   , KC_NO   , KC_NO   , KC_NO   ,                                              M_REDO   , M_PASTE , M_COPY  , M_CUT   , M_UNDO  , KC_NO   ,
  //├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤                                             ├─────────┼─────────┼─────────┼─────────┼─────────┤─────────┤
     KC_TAB   , KC_LGUI , KC_LALT , KC_LCTL , KC_LSFT , KC_NO   ,                                              KC_NO    , MS_LEFT , MS_DOWN , MS_UP   , MS_RGHT , KC_NO   ,                    
  //├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┼─────────┬─────────┐     ┌─────────┬─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤─────────┤
     _______  , KC_NO   , KC_NO   , KC_NO   , KC_NO   , KC_NO   , KC_NO   , _______ ,       KC_NO   , KC_NO   , KC_NO   , MS_WHLL , MS_WHLD , MS_WHLU , MS_WHLR , _______ ,
  //└─────────┴─────────┴─────────┼─────────┼─────────┼─────────┼─────────┬─────────┤     ├─────────┬─────────┼─────────┼─────────┼─────────┼─────────┴─────────┴─────────┘
                                    MS_UP   , MS_DOWN , KC_NO   , KC_NO   , KC_NO   ,       MS_BTN2 , MS_BTN1 , MS_BTN3 , MS_LEFT , MS_RGHT
  //                              └─────────┴─────────┴─────────┴─────────┴─────────┘     └─────────┴─────────┴─────────┴─────────┴─────────┘
  ),
  
  [_SYM] = LAYOUT( // Symbols
  //┌─────────┬─────────┬─────────┬─────────┬─────────┬─────────┐                                             ┌─────────┬─────────┬─────────┬─────────┬─────────┬─────────┐
     KC_NO    , KC_LBRC , KC_AMPR , KC_ASTR , KC_LPRN , KC_RBRC ,                                              KC_NO    , KC_NO   , KC_NO   , KC_NO   , KC_NO   , KC_NO   ,
  //├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤                                             ├─────────┼─────────┼─────────┼─────────┼─────────┤─────────┤
     KC_NO    , KC_COLN , KC_DLR  , KC_PERC , KC_CIRC , KC_PLUS ,                                              KC_NO    , KC_RSFT , KC_RCTL , KC_RALT , KC_RGUI , KC_NO   ,                    
  //├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┼─────────┬─────────┐     ┌─────────┬─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤─────────┤
     _______  , KC_TILD , KC_EXLM , KC_AT   , KC_HASH , KC_PIPE , KC_ASTR , KC_BSLS ,       KC_NO   , KC_NO   , KC_NO   , KC_NO   , KC_NO   , KC_NO   , KC_NO   , _______ ,
  //└─────────┴─────────┴─────────┼─────────┼─────────┼─────────┼─────────┬─────────┤     ├─────────┬─────────┼─────────┼─────────┼─────────┼─────────┴─────────┴─────────┘
                                    _______ , _______ , KC_RPRN , KC_UNDS , KC_PLUS ,       KC_NO   , KC_NO   , KC_NO   , _______ , _______
  //                              └─────────┴─────────┴─────────┴─────────┴─────────┘     └─────────┴─────────┴─────────┴─────────┴─────────┘
  ),
  
  [_NUM] = LAYOUT( // Numbers
  //┌─────────┬─────────┬─────────┬─────────┬─────────┬─────────┐                                             ┌─────────┬─────────┬─────────┬─────────┬─────────┬─────────┐
     KC_PLUS  , KC_LBRC , KC_7    , KC_8    , KC_9    , KC_RBRC ,                                              KC_NO    , KC_NO   , KC_NO   , KC_NO   , KC_NO   , KC_NO   ,
  //├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤                                             ├─────────┼─────────┼─────────┼─────────┼─────────┤─────────┤
     KC_MINS  , KC_COLN , KC_4    , KC_5    , KC_6    , KC_EQL  ,                                              KC_NO    , KC_RSFT , KC_RCTL , KC_RALT , KC_RGUI , KC_NO   ,                    
  //├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┼─────────┬─────────┐     ┌─────────┬─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤─────────┤
     _______  , KC_GRV  , KC_1    , KC_2    , KC_3    , KC_BSLS , KC_NO   , _______ ,       KC_NO   , KC_NO   , KC_NO   , KC_NO   , KC_NO   , KC_NO   , KC_NO   , _______ ,
  //└─────────┴─────────┴─────────┼─────────┼─────────┼─────────┼─────────┬─────────┤     ├─────────┬─────────┼─────────┼─────────┼─────────┼─────────┴─────────┴─────────┘
                                    _______ , _______ , KC_0    , KC_MINS , KC_EQL  ,       KC_NO   , KC_NO   , KC_NO   , _______ , _______
  //                              └─────────┴─────────┴─────────┴─────────┴─────────┘     └─────────┴─────────┴─────────┴─────────┴─────────┘
  ),
  
  [_FUN] = LAYOUT( // Function keys
  //┌─────────┬─────────┬─────────┬─────────┬─────────┬─────────┐                                             ┌─────────┬─────────┬─────────┬─────────┬─────────┬─────────┐
     KC_NO    , KC_F12  , KC_F7   , KC_F8   , KC_F9   , KC_PSCR ,                                              KC_NO    , KC_NO   , KC_NO   , KC_NO   , KC_NO   , KC_NO   ,
  //├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤                                             ├─────────┼─────────┼─────────┼─────────┼─────────┤─────────┤
     KC_TAB   , KC_F11  , KC_F4   , KC_F5   , KC_F6   , KC_SCRL ,                                              KC_NO    , KC_RSFT , KC_RCTL , KC_RALT , KC_RGUI , KC_NO   ,                    
  //├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┼─────────┬─────────┐     ┌─────────┬─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤─────────┤
     _______  , KC_F10  , KC_F1   , KC_F2   , KC_F3   , KC_PAUS , KC_NO   , _______ ,       KC_NO   , KC_NO   , KC_NO   , KC_NO   , KC_NO   , KC_NO   , KC_NO   , _______ ,
  //└─────────┴─────────┴─────────┼─────────┼─────────┼─────────┼─────────┬─────────┤     ├─────────┬─────────┼─────────┼─────────┼─────────┼─────────┴─────────┴─────────┘
                                    _______ , _______ , KC_LGUI , KC_SPC  , KC_TAB  ,       KC_NO   , KC_NO   , KC_NO   , _______ , _______
  //                              └─────────┴─────────┴─────────┴─────────┴─────────┘     └─────────┴─────────┴─────────┴─────────┴─────────┘
  ),

  [_RGB] = LAYOUT( // RGB
  //┌─────────┬─────────┬─────────┬─────────┬─────────┬─────────┐                                             ┌─────────┬─────────┬─────────┬─────────┬─────────┬─────────┐ 
     KC_NO    , KC_NO   , KC_NO   , KC_NO   , KC_NO   , KC_NO   ,                                              UG_TOGG  , UG_NEXT , UG_HUEU , UG_SATU , UG_VALU , UG_SPDU , 
  //├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤                                             ├─────────┼─────────┼─────────┼─────────┼─────────┤─────────┤ 
     KC_NO    , KC_NO   , KC_NO   , KC_NO   , KC_NO   , KC_NO   ,                                              KC_NO    , KC_NO   , KC_NO   , KC_NO   , KC_NO   , KC_NO   ,               
  //├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┼─────────┬─────────┐     ┌─────────┬─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤─────────┤ 
     _______  , KC_NO   , KC_NO   , KC_NO   , KC_NO   , KC_NO   , KC_NO   , _______ ,       KC_NO   , KC_NO   , VK_TOGG , UG_PREV , UG_HUED , UG_SATD , UG_VALD , UG_SPDD ,
  //└─────────┴─────────┴─────────┼─────────┼─────────┼─────────┼─────────┬─────────┤     ├─────────┬─────────┼─────────┼─────────┼─────────┼─────────┴─────────┴─────────┘ 
                                    _______ , _______ , KC_NO   , KC_NO   , KC_NO   ,       KC_NO   , KC_NO   , KC_NO   , _______ , _______
  //                              └─────────┴─────────┴─────────┴─────────┴─────────┘     └─────────┴─────────┴─────────┴─────────┴─────────┘ 
  ),
};

#ifdef OLED_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) { return OLED_ROTATION_180; }

bool oled_task_user(void) {
  if (is_keyboard_master()) {
    // revision and keymap
    //oled_write_P(PSTR("#> ronajon rev0.6a\n"), false);

    // QMK Logo and version information
    // clang-format off
    static const char PROGMEM qmk_logo[] = {
      0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,0x90,0x91,0x92,0x93,0x94,
      0xa0,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa,0xab,0xac,0xad,0xae,0xaf,0xb0,0xb1,0xb2,0xb3,0xb4,
      0xc0,0xc1,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,0xcb,0xcc,0xcd,0xce,0xcf,0xd0,0xd1,0xd2,0xd3,0xd4,0};
    // clang-format on

    oled_write_P(qmk_logo, false);
//    static char rgbStatusLine1[26] = {0};
//    snprintf(rgbStatusLine1, sizeof(rgbStatusLine1), "RGB Mode: %d", rgblight_get_mode());
//    oled_write_ln(rgbStatusLine1, false);
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
      case _RGB:
        oled_write_P(PSTR("RGB\n"), false);
        break;
      default:
        oled_write_P(PSTR("Undefined\n"), false);
    }

    // Write host Keyboard LED Status to OLEDs
    led_t led_usb_state = host_keyboard_led_state();
    oled_write_P(led_usb_state.num_lock  ? PSTR("NUMLCK ") : PSTR("     "), false);
    oled_write_P(led_usb_state.caps_lock   ? PSTR("CAPLCK ") : PSTR("     "), false);
    oled_write_P(led_usb_state.scroll_lock ? PSTR("SCRLCK ") : PSTR("     "), false);
    oled_write_P(is_shift_pressed ? PSTR("SHFT ") : PSTR("     "), false);
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
