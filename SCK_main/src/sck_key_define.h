#pragma once

#define KEY_LAYERS 3
#define KM_V 6
#define KM_H 14
#define FM_V 5
#define FM_H 3
#define PM_V 5
#define PM_H 4
#define MM_V 8
#define MM_H 5

unsigned char SCK_KM_buf[KM_H] = {0,}; // Keyboard Module buffer
bool SCK_KM_pressed[KM_V][KM_H] = {0,}; // Keyboard Module key press flags
//bool SCK_KM_toggled[KM_V][KM_H] = {0,}; // Keyboard Module key toggle flags

unsigned char SCK_FM_buf[FM_H] = {0,}; // Keyboard Module buffer
bool SCK_FM_pressed[FM_V][FM_H] = {0,}; // Fnkey Module key press flags
//bool SCK_FM_toggled[FM_V][FM_H] = {0,}; // Fnkey Module key toggle flags

unsigned char SCK_PM_buf[PM_H] = {0,}; // Keyboard Module buffer
bool SCK_PM_pressed[PM_V][PM_H] = {0,}; // keyPad Module key press flags
//bool SCK_PM_toggled[PM_V][PM_H] = {0,}; // keyPad Module key toggle flags

unsigned char SCK_MM_buf[1] = {0,}; // Keyboard Module buffer
bool SCK_MM_pressed[MM_V][MM_H] = {0,}; // Macro Module key press flags
bool SCK_MM_toggled[MM_V][MM_H] = {0,}; // Macro Module key toggle flags
