/*
    menu include file
    Copyright (C) 2011 Pavel Semerad

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef _MENU_INCLUDED
#define _MENU_INCLUDED


#include "gt3b.h"



// trims/subtrims limits
#define TRIM_MAX	99
#define SUBTRIM_MAX	99
#define EXPO_MAX	100

// amount of step when fast encoder rotate
#define MODEL_FAST	2
#define ENDPOINT_FAST	5
#define TRIM_FAST	5
#define SUBTRIM_FAST	5
#define DUALRATE_FAST	5
#define EXPO_FAST	5
#define CHANNEL_FAST	5
#define MIX_FAST	5
#define SPEED_FAST	5
#define TIMER_ALARM_FAST 5

// delay in seconds of popup menu (trim, dualrate, ...)
#define POPUP_DELAY	5

// pause in 5ms when reset value is reached during trim popup
#define RESET_VALUE_DELAY	(500 / 5)




// variables to be used in CALC task
extern u8  menu_force_value_channel;	// set PPM value for this channel
extern s16 menu_force_value;		//   to this value (-500..500)
extern s8  menu_channel3_8[MAX_CHANNELS - 2];	// values -100..100 for channels >=3
#define menu_channel3  menu_channel3_8[0]
#define menu_channel4  menu_channel3_8[1]
#define menu_channel5  menu_channel3_8[2]
#define menu_channel6  menu_channel3_8[3]
#define menu_channel7  menu_channel3_8[4]
#define menu_channel8  menu_channel3_8[5]
extern u8  menu_channels_mixed;	    // channel with 1 here will not be set from
					//   menu_channel3_8
extern s8  menu_4WS_mix;		// mix -100..100
extern _Bool menu_4WS_crab;		// when 1, crab steering
extern s8  menu_DIG_mix;		// mix -100..100
extern u8  menu_MP_index;		// index of MultiPosition channel
extern _Bool menu_brake;		// when 1, full brake is applied






// task MENU will be alias for task OPER
#define MENU OPER


// menu tasks will be handling ADC now (for calibrate)
extern _Bool menu_wants_adc;
// flag low battery voltage
extern _Bool menu_battery_low;
extern _Bool battery_low_shutup;  // stop bat low beeping
// raw battery ADC value for check to battery low
extern u16 battery_low_raw;
// don't stop main loop and check keys
extern u8 menu_check_keys;
// temporary flag used when doing reset (global/all models/model)
extern _Bool menu_tmp_flag;



// internal functions, used in split menu files
extern void menu_stop(void);
extern void menu_clear_symbols(void);
extern void menu_calibrate(u8 at_poweron);
extern void menu_key_test(void);
extern void menu_global_setup(void);
extern s16  menu_change_val(s16 val, s16 min, s16 max, u8 amount_fast, u8 rotate);
extern void apply_global_config(void);
extern void menu_load_model(void);
extern void apply_model_config(void);
extern u8 menu_electronic_trims(void);
extern u8 menu_buttons(void);
extern u8 *menu_et_function_name(u8 n);
extern s8 menu_et_function_idx(u8 n);
extern u8 menu_et_function_long_special(u8 n);
extern u8 menu_et_function_is_list(u8 n);
extern u8 menu_et_function_is_allowed(u8 n);
extern u8 *menu_key_function_name(u8 n);
extern s8 menu_key_function_idx(u8 n);
extern u8 menu_key_function_2state(u8 n);
extern u8 menu_key_function_is_allowed(u8 n);
extern const u8 steps_map[];
#define STEPS_MAP_SIZE  11
extern const u16 et_buttons[][2];
extern void menu_mix(void);
extern void menu_key_mapping(void);
extern void menu_key_mapping_prepare(void);
extern void menu_buttons_initialize(void);
extern u8 menu_main_screen;
#define MS_NAME		0
#define MS_BATTERY	1
#define MS_TIMER0	2
#define MS_TIMER1	3
#define MS_MAX		4
// common menus, select item in 7SEG and then modify its setting at CHR3
//   val_id: 1..num_values - which param of this item to change
//   action: 0=show, 1=change, 2=get_next_val_id
typedef u8 (*menu_list_t)(u8 val_id, u8 action, u8 *chars_blink);
extern void menu_list(menu_list_t *menu_funcs, u8 menu_nitems, u8 use_stop);



// timers
// types
#define TIMER_NUM	2
#define TIMER_OFF	0
#define TIMER_UP	1
#define TIMER_DOWN	2
#define TIMER_LAP	3
#define TIMER_LAPCNT	4
#define TIMER_TYPE_MAX	4

#define TIMER_TYPE(tid)  ((u8)(tid ? cg.timer2_type : cg.timer1_type))
#define TIMER_TYPE_SET(tid, val) \
    if (tid) cg.timer2_type = (u8)val; \
    else cg.timer1_type = (u8)val;

#define TIMER_ALARM(tid)  ((u8)(tid ? cg.timer2_alarm : cg.timer1_alarm))
#define TIMER_ALARM_SET(tid, val) \
    if (tid) cg.timer2_alarm = (u8)val; \
    else cg.timer1_alarm = (u8)val;

// menu task will be waked-up periodically to show timer value
extern _Bool	 menu_timer_wakeup;
extern u8	 menu_timer_running;		// running timers, one bit for one timer
extern u8	 menu_timer_direction;		// up(0)/down(1) timer direction
extern u8	 menu_timer_alarmed;		// down timer was over, one bit for one timer
extern @near u8  menu_timer_throttle;		// throttle start, one bit for one timer

typedef struct {
    u16	sec;					// timer seconds
    u8	hdr;					// timers 0.01 seconds
} menu_timer_s;
extern @near menu_timer_s menu_timer[];		// actual timer values
#define TIMER_READ(pt, tsec, thdr) \
    sim(); \
    tsec = pt->sec; \
    thdr = pt->hdr; \
    rim();
extern @near u16 menu_timer_alarm[];		// alarm in seconds

void menu_timer_show(u8 tid);
void menu_timer_setup(u8 tid);
void menu_timer_lap_times(u8 tid);
void menu_timer_clear(u8 tid, u8 laps);
void kf_menu_timer_start(u8 *id, u8 *param, u8 flags, s16 *pv);
void kf_menu_timer_reset(u8 *id, u8 *param, u8 flags, s16 *pv);


#endif

