#include QMK_KEYBOARD_H

enum chimera_ortho_layers
{
	_BASE,
	_NAV,
	_SYM,
	_FUNC,
	_MOUSE
};

#define KC_NAV MO(_NAV)
#define KC_SYM MO(_SYM)
#define KC_FUNC MO(_FUNC)
#define KC_MOUSE TT(_MOUSE)

#define KC_DELSHFT SFT_T(KC_DEL)
#define KC_CTLENT CTL_T(KC_ENT)
#define KC_SYMSPC LT(_SYM, KC_SPC)

#define KC_WK_LEFT LCA(KC_LEFT)
#define KC_WK_DOWN LCA(KC_DOWN)
#define KC_WK_UP LCA(KC_UP)
#define KC_WK_RGHT LCA(KC_RGHT)

#define KC_QUAKE LCTL(KC_GRAVE)

#define LONGPRESS_DELAY 150

// These are needed because of the 'KC_'-adding macro
// This macro can be found in ../../chimera_ortho.h
#define KC_RESET RESET
#define KC_ KC_TRNS 

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  [_BASE] = LAYOUT_kc(
      //,-------+-------+-------+-------+-------+-------+-------.    ,-------+-------+-------+-------+-------+-------+-------.
         LALT   ,TAB    ,QUOT   ,COMM   ,DOT    ,P      ,Y           ,F      ,G      ,C      ,R      ,L      ,SLSH   ,FUNC   
      //|-------+-------+-------+-------+-------+-------+-------|    |-------+-------+-------+-------+-------+-------+-------|
        ,LGUI   ,ESC    ,A      ,O      ,E      ,U      ,I           ,D      ,H      ,T      ,N      ,S      ,MINS   ,MOUSE  
      //|-------+-------+-------+-------+-------+-------+-------|    |-------+-------+-------+-------+-------+-------+-------|
        ,LCTL   ,DELSHFT,SCLN   ,Q      ,J      ,K      ,X           ,B      ,M      ,W      ,V      ,Z      ,BSPC   ,ENTER
      //|-------+-------+-------+-------+-------+-------+-------|    |-------+-------+-------+-------+-------+-------+-------|
                                        ,LSHIFT ,NAV                         ,SYMSPC ,CTLENT
      //\-------------------------------+-------+-------+-------/    \-------+-------+---------------------------------------/
  ),

  [_NAV] = LAYOUT_kc(
      //,-------+-------+-------+-------+-------+-------+-------.    ,-------+-------+-------+-------+-------+-------+-------.
                ,       ,       ,       ,       ,       ,            ,       ,HOME   ,PGDOWN ,PGUP   ,END    ,       ,       
      //|-------+-------+-------+-------+-------+-------+-------|    |-------+-------+-------+-------+-------+-------+-------|
        ,       ,PSCR   ,MENU   ,       ,       ,       ,            ,       ,LEFT   ,DOWN   ,UP     ,RIGHT  ,       ,       
      //|-------+-------+-------+-------+-------+-------+-------|    |-------+-------+-------+-------+-------+-------+-------|
        ,       ,CAPS   ,NLCK   ,INS    ,       ,       ,            ,       ,WK_LEFT,WK_DOWN,WK_UP  ,WK_RGHT,       ,     
      //|-------+-------+-------+-------+-------+-------+-------|    |-------+-------+-------+-------+-------+-------+-------|
                                        ,       ,                            ,       ,    
      //\-------------------------------+-------+-------+-------/    \-------+-------+---------------------------------------/
  ),

  [_SYM] = LAYOUT_kc(
      //,-------+-------+-------+-------+-------+-------+-------.    ,-------+-------+-------+-------+-------+-------+-------.
                ,QUAKE  ,GRAVE  ,TILDE  ,BSLASH ,PIPE   ,LPRN        ,RPRN   ,7      ,8      ,9      ,SLSH   ,EQUAL  ,       
      //|-------+-------+-------+-------+-------+-------+-------|    |-------+-------+-------+-------+-------+-------+-------|
        ,       ,       ,EXLM   ,AT     ,HASH   ,DLR    ,LCBR        ,RCBR   ,4      ,5      ,6      ,ASTR   ,PLUS   ,       
      //|-------+-------+-------+-------+-------+-------+-------|    |-------+-------+-------+-------+-------+-------+-------|
        ,       ,       ,PERC   ,CIRC   ,AMPR   ,ASTR   ,LBRC        ,RBRC   ,1      ,2      ,3      ,MINUS  ,       ,     
      //|-------+-------+-------+-------+-------+-------+-------|    |-------+-------+-------+-------+-------+-------+-------|
                                        ,       ,                            ,       ,0
      //\-------------------------------+-------+-------+-------/    \-------+-------+---------------------------------------/
  ),

  [_FUNC] = LAYOUT_kc(
      //,-------+-------+-------+-------+-------+-------+-------.    ,-------+-------+-------+-------+-------+-------+-------.
                ,RESET  ,SLEP   ,MRWD   ,MPLY   ,MFFD   ,            ,       ,F9     ,F10    ,F11    ,F12    ,       ,       
      //|-------+-------+-------+-------+-------+-------+-------|    |-------+-------+-------+-------+-------+-------+-------|
        ,       ,       ,PWR    ,MUTE   ,VOLD   ,VOLU   ,            ,       ,F5     ,F6     ,F7     ,F8     ,       ,       
      //|-------+-------+-------+-------+-------+-------+-------|    |-------+-------+-------+-------+-------+-------+-------|
        ,       ,       ,WAKE   ,MPRV   ,MPLY   ,MNXT   ,            ,       ,F1     ,F2     ,F3     ,F4     ,       ,     
      //|-------+-------+-------+-------+-------+-------+-------|    |-------+-------+-------+-------+-------+-------+-------|
                                        ,       ,                            ,       ,    
      //\-------------------------------+-------+-------+-------/    \-------+-------+---------------------------------------/
  ),

  [_MOUSE] = LAYOUT_kc(
      //,-------+-------+-------+-------+-------+-------+-------.    ,-------+-------+-------+-------+-------+-------+-------.
                ,       ,       ,       ,       ,       ,            ,       ,       ,       ,       ,       ,       ,       
      //|-------+-------+-------+-------+-------+-------+-------|    |-------+-------+-------+-------+-------+-------+-------|
        ,       ,       ,       ,BTN1   ,BTN3   ,BTN2   ,            ,       ,MS_L   ,MS_D   ,MS_U   ,MS_R   ,       ,       
      //|-------+-------+-------+-------+-------+-------+-------|    |-------+-------+-------+-------+-------+-------+-------|
        ,       ,       ,       ,ACL0   ,ACL1   ,ACL2   ,            ,       ,WH_L   ,WH_D   ,WH_U   ,WH_R   ,       ,     
      //|-------+-------+-------+-------+-------+-------+-------|    |-------+-------+-------+-------+-------+-------+-------|
                                        ,       ,                            ,       ,    
      //\-------------------------------+-------+-------+-------/    \-------+-------+---------------------------------------/
  ),
};

// These control the color of the LED on the receiver
// For color reference, see ../../chimera_ortho.h
void matrix_scan_user(void) {
    uint8_t layer = biton32(layer_state);
    
    switch (layer) {
        case _BASE:
            set_led_cyan;
            break;
        case _NAV:
            set_led_blue;
            break;
        case _SYM:
            set_led_magenta;
            break;
        case _FUNC:
            set_led_yellow;
        default:
            set_led_white;
            break;
    }
};