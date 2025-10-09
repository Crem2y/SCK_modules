#pragma once
// This uses the empty space in <keyboard.h>.
// unused 0x00 (1)
#define K___ 0x00 // key_none (there is no key)
// mouse and repeat speed 0x01~0x17 (23)
#define M_LB 0x01 // left_mouse
#define M_RB 0x02 // right_mouse
#define M_MB 0x03 // middle_mouse
#define M_4B 0x04 // mouse_button_4 (prev)
#define M_5B 0x05 // mouse_button_5 (next)
#define M_6B 0x06 // mouse_button_6
#define M_7B 0x07 // mouse_button_7
#define M_U  0x08 // mouse_up
#define M_D  0x09 // mouse_down
#define M_L  0x0A // mouse_left
#define M_R  0x0B // mouse_right
#define M_UL 0x0C // mouse_ul
#define M_UR 0x0D // mouse_ur
#define M_DL 0x0E // mouse_dl
#define M_DR 0x0F // mouse_dr
#define MH_U 0x10 // mouse_wheel_up
#define MH_D 0x11 // mouse_wheel_down
#define M_F  0x12 // mouse_faster
#define M_S  0x13 // mouse_slower
#define MH_F 0x14 // mouse_wheel_faster
#define MH_S 0x15 // mouse_wheel_slower
#define R_F  0x16 // repeat_faster
#define R_S  0x17 // repeat_slower
// FNkey, LED 0x18~0x1F (8)
#define FK_1 0x18 // function_key_01
#define FK_2 0x19 // function_key_02
#define L_KC 0x1A // LED_key_change
#define L_SC 0x1B // LED_side_change
#define L_KL 0x1C // LED_key_lighter
#define L_KD 0x1D // LED_key_darker
#define L_SL 0x1E // LED_side_lighter
#define L_SD 0x1F // LED_side_darker

// user function 0x88~0xAF (40)
#define F_01 0x88 // function_01
#define F_02 0x89 // function_02
#define F_03 0x8A // function_03
#define F_04 0x8B // function_04
#define F_05 0x8C // function_05
#define F_06 0x8D // function_06
#define F_07 0x8E // function_07
#define F_08 0x8F // function_08
#define F_09 0x90 // function_09
#define F_10 0x91 // function_10
#define F_11 0x92 // function_11
#define F_12 0x93 // function_12
#define F_13 0x94 // function_13
#define F_14 0x95 // function_14
#define F_15 0x96 // function_15
#define F_16 0x97 // function_16
#define F_17 0x98 // function_17
#define F_18 0x99 // function_18
#define F_19 0x9A // function_19
#define F_20 0x9B // function_20
#define F_21 0x9C // function_21
#define F_22 0x9D // function_22
#define F_23 0x9E // function_23
#define F_24 0x9F // function_24
#define F_25 0xA0 // function_25
#define F_26 0xA1 // function_26
#define F_27 0xA2 // function_27
#define F_28 0xA3 // function_28
#define F_29 0xA4 // function_29
#define F_30 0xA5 // function_30
#define F_31 0xA6 // function_31
#define F_32 0xA7 // function_32
#define F_33 0xA8 // function_33
#define F_34 0xA9 // function_34
#define F_35 0xAA // function_35
#define F_36 0xAB // function_36
#define F_37 0xAC // function_37
#define F_38 0xAD // function_38
#define F_39 0xAE // function_39
#define F_40 0xAF // function_40

// consumer key 0xB4~0xC0 (13)
#define C_VM 0xB4 // con_volume_mute
#define C_VU 0xB5 // con_volume_up
#define C_VD 0xB6 // con_volume_down
#define C_MP 0xB7 // con_media_play_pause
#define C_MS 0xB8 // con_media_stop
#define C_MN 0xB9 // con_media_next
#define C_MR 0xBA // con_media_prev
#define C_CA 0xBB // con_calculator
#define C_EM 0xBC // con_email
#define C_BH 0xBD // con_browser_home
#define C_BB 0xBE // con_browser_back
#define C_BF 0xBF // con_browser_forward
#define C_FI 0xC0 // con_find (not working well)

// surface dial 0xEC~0xEE (3)
#define S_B  0xEC // surface_button
#define S_CW 0xED // surface_clockwise
#define S_CC 0xEE // surface_counter_clockwise

// debug 0xFC~0xFF (4)
#define D_RS 0xFC // debug_reset
#define D_PG 0xFD // debug_program
#define D_01 0xFE // debug_01
#define D_02 0xFF // debug_02