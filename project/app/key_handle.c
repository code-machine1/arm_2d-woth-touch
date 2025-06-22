#include "key_handle.h"
#include "work_handle.h"
#include "beep_handle.h"
#include "dwin_handle.h"
#include "FWG2_handle.h"
static handle_event EVENT = END_EVENT;


static KEY_VALUE get_key_hel1()
{
    if (READ_KEY_HEL1 == 0)
    {
        return K_PRESS;
    }
    else
    {
        return K_RELEASE;
    }
}

static KEY_VALUE get_key_hel2()
{
    if (READ_KEY_HEL2 == 0)
    {
        return K_PRESS;
    }
    else
    {
        return K_RELEASE;
    }
}

static KEY_VALUE get_key_hel3()
{
    if (READ_KEY_HEL3 == 0)
    {
        return K_PRESS;
    }
    else
    {
        return K_RELEASE;
    }
}

static KEY_VALUE get_key_hel4()
{
    if (READ_KEY_HEL4 == 0)
    {
        return K_PRESS;
    }
    else
    {
        return K_RELEASE;
    }
}

static KEY_VALUE get_key_hel5()
{
    if (READ_KEY_HEL5 == 0)
    {
        return K_PRESS;
    }
    else
    {
        return K_RELEASE;
    }
}

static KEY_VALUE get_key_hel6()
{
    if (READ_KEY_HEL6 == 0)
    {
        return K_PRESS;
    }
    else
    {
        return K_RELEASE;
    }
}

static KEY keys[] =
{
    {KS_RELEASE, 0, KEY_CYCLE_TIME, get_key_hel1},//function1
    {KS_RELEASE, 0, KEY_CYCLE_TIME, get_key_hel2},//reduce1
    {KS_RELEASE, 0, KEY_CYCLE_TIME, get_key_hel3},//add1
    {KS_RELEASE, 0, KEY_CYCLE_TIME, get_key_hel4},//function
    {KS_RELEASE, 0, KEY_CYCLE_TIME, get_key_hel5},//reduce2
    {KS_RELEASE, 0, KEY_CYCLE_TIME, get_key_hel6},//add2
};

KEY_EVENT key_event[KEY_NUMBER] =
{
    KE_PRESS,
    KE_RELEASE,
    KE_LONG_PRESS,
    KE_LONG_RELEASE,
    KE_NONE,
};


void KeyProc(void)
{
    static uint16_t direct_set_temp_time;
    static bool direct_set_temp_flag = false;
    static uint16_t direct_set_wind_time;
    static bool direct_set_wind_flag = false;
    static uint16_t cyclone_set_temp_time;
    static bool cyclone_set_temp_flag = false;
    static uint16_t cyclone_set_wind_time;
    static bool cyclone_set_wind_flag = false;
    static bool set_done = false;

    if (get_key_hel1()         == K_RELEASE     && \
            get_key_hel2()         == K_RELEASE && \
            get_key_hel3()         == K_RELEASE && \
            get_key_hel4()         == K_RELEASE     && \
            get_key_hel5()         == K_RELEASE && \
            get_key_hel6()         == K_RELEASE
       )
    {
        set_done = false;
    }

    /* scan key begin */
    for (uint8_t i = 0; i < KEY_NUMBER; i++)
    {
        key_event[i] = key_event_check(&keys[i], LONG_PRESS_TIME);
    }

    /* scan key end */

    /* direct setting time control begin */
    if (direct_set_temp_flag)
    {
        direct_set_temp_time++;

        if (direct_set_temp_time >= 68)
        {
            direct_set_temp_time = 0;
            direct_set_temp_flag = false;
        }
    }

    if (direct_set_wind_flag)
    {
        direct_set_wind_time++;

        if (direct_set_wind_time >= 68)
        {
            direct_set_wind_time = 0;
            direct_set_wind_flag = false;
        }
    }

    /* direct setting time control end */

    /* cyclone setting time control begin */
    if (cyclone_set_temp_flag)
    {
        cyclone_set_temp_time++;

        if (cyclone_set_temp_time >= 68)
        {
            cyclone_set_temp_time = 0;
            cyclone_set_temp_flag = false;
        }
    }

    if (cyclone_set_wind_flag)
    {
        cyclone_set_wind_time++;

        if (cyclone_set_wind_time >= 68)
        {
            cyclone_set_wind_time = 0;
            cyclone_set_wind_flag = false;
        }
    }

    /* cyclone setting time control end */

    if (sFWG2_t.general_parameter.work_mode == NORMAL)
    {
        if (!set_done)
        {
            /*get event*/
            if (key_event[0] == KE_PRESS)
            {
                if (direct_set_temp_flag)
                {
                    direct_set_wind_flag = true;
                    direct_set_temp_flag = false;
                    sbeep.status = BEEP_SHORT;
                    set_done = TRUE;
                }
                else if (direct_set_wind_flag)
                {
                    direct_set_wind_flag = false;
                    sbeep.status = BEEP_SHORT;
                    set_done = TRUE;
                }
                else if (direct_set_temp_flag == false                     && \
                         direct_set_temp_flag == false                      && \
                         sFWG2_t.general_parameter.countdown_flag == false  && \
                         sFWG2_t.Direct_handle_work_mode != COLD_WIND_MODE   && \
                         sFWG2_t.Direct_handle_work_mode != EN_WORKING_MODE)
                {
					if(sFWG2_t.general_parameter.fwg2_page == PAGE_MAIN                 || \
                       sFWG2_t.general_parameter.fwg2_page == PAGE_DIRECT_CYCLONE_CURVE || \
					   sFWG2_t.general_parameter.fwg2_page == PAGE_SHOW_DIRECT_WORK     || \
                       sFWG2_t.general_parameter.fwg2_page == PAGE_SHOW_DIRECT_CURVE)
					{
					EVENT = SET_CH_EVENT;
                    set_done = TRUE;
					}
                    
                }
                else if (sFWG2_t.general_parameter.countdown_flag)
                {
                    sFWG2_t.general_parameter.countdown_flag = false;
                    sbeep.status = BEEP_LONG;
                    set_done = TRUE;
                }
                else if (sFWG2_t.Direct_handle_work_mode == COLD_WIND_MODE)
                {
                    sFWG2_t.Direct_handle_work_mode = NORMAL_MODE;
                    sbeep.status = BEEP_LONG;
                    set_done = TRUE;
                }
                else if (sFWG2_t.Direct_handle_work_mode == EN_WORKING_MODE)
                {
                    sFWG2_t.Direct_handle_work_mode = NORMAL_MODE;
                    sbeep.status = BEEP_LONG;
                    set_done = TRUE;
                }
            }
            else if (key_event[1] == KE_PRESS)
            {
                if (sFWG2_t.Direct_handle_work_mode == NORMAL_MODE)
                {
                    if (direct_set_temp_flag == false && direct_set_wind_flag == false)
                    {
                        direct_set_temp_flag = true;
                    }

                    if (direct_set_temp_flag)
                    {
                        EVENT = DIRECT_TEMP_REDUCE_EVENT;
                        set_done = TRUE;
                        direct_set_temp_time = 0;
                    }

                    if (direct_set_wind_flag)
                    {
                        EVENT = DIRECT_WIND_REDUCE_EVENT;
                        set_done = TRUE;
                        direct_set_wind_time = 0;
                    }
                }
                else if (sFWG2_t.Direct_handle_work_mode == COLD_WIND_MODE)
                {
                    EVENT = DIRECT_WIND_REDUCE_EVENT;
                    set_done = TRUE;
                }
            }
            else if (key_event[2] == KE_PRESS)
            {
                if (sFWG2_t.Direct_handle_work_mode == NORMAL_MODE)
                {
                    if (direct_set_temp_flag == false && direct_set_wind_flag == false)
                    {
                        direct_set_temp_flag = true;
                    }

                    if (direct_set_temp_flag)
                    {
                        EVENT = DIRECT_TEMP_ADD_EVENT;
                        set_done = TRUE;
                        direct_set_temp_time = 0;
                    }

                    if (direct_set_wind_flag)
                    {
                        EVENT = DIRECT_WIND_ADD_EVENT;
                        set_done = TRUE;
                        direct_set_wind_time = 0;
                    }
                }
                else if (sFWG2_t.Direct_handle_work_mode == COLD_WIND_MODE)
                {
                    EVENT = DIRECT_WIND_ADD_EVENT;
                    set_done = TRUE;
                }
            }
            else if (key_event[3] == KE_PRESS)
            {
                if (cyclone_set_temp_flag)
                {
                    cyclone_set_wind_flag = true;
                    cyclone_set_temp_flag = false;
                    sbeep.status = BEEP_SHORT;
                    set_done = true;
                }
                else if (cyclone_set_wind_flag)
                {
                    cyclone_set_wind_flag = false;
                    sbeep.status = BEEP_SHORT;
                    set_done = TRUE;
                }
                else if (cyclone_set_temp_flag == false                   && \
                         cyclone_set_temp_flag == false                    && \
                         sFWG2_t.general_parameter.countdown_flag == false && \
                         sFWG2_t.Cyclone_handle_work_mode != COLD_WIND_MODE && \
                         sFWG2_t.Cyclone_handle_work_mode != EN_WORKING_MODE)
                {
					if(sFWG2_t.general_parameter.fwg2_page == PAGE_MAIN                 || \
                       sFWG2_t.general_parameter.fwg2_page == PAGE_DIRECT_CYCLONE_CURVE || \
					   sFWG2_t.general_parameter.fwg2_page == PAGE_SHOW_CYCLONE_WORK     || \
                       sFWG2_t.general_parameter.fwg2_page == PAGE_SHOW_CYCLONE_CURVE)
					{
					    EVENT = SET_CH_EVENT;
                        set_done = TRUE;
					}
                    
                }
                else if (sFWG2_t.general_parameter.countdown_flag)
                {
                    sFWG2_t.general_parameter.countdown_flag = false;
                    sbeep.status = BEEP_LONG;
                    set_done = TRUE;
                }
                else if (sFWG2_t.Cyclone_handle_work_mode == COLD_WIND_MODE)
                {
                    sFWG2_t.Cyclone_handle_work_mode  = NORMAL_MODE;
                    sbeep.status = BEEP_LONG;
                    set_done = TRUE;
                }
                else if (sFWG2_t.Cyclone_handle_work_mode == EN_WORKING_MODE)
                {
                    sFWG2_t.Cyclone_handle_work_mode  = NORMAL_MODE;
                    sbeep.status = BEEP_LONG;
                    set_done = TRUE;
                }
            }
            else if (key_event[4] == KE_PRESS)
            {
                if (sFWG2_t.Cyclone_handle_work_mode == NORMAL_MODE)
                {
                    if (cyclone_set_temp_flag == false && cyclone_set_wind_flag == false)
                    {
                        cyclone_set_temp_flag = true;
                    }

                    if (cyclone_set_temp_flag)
                    {
                        EVENT = CYCLONE_TEMP_REDUCE_EVENT;
                        set_done = TRUE;
                        cyclone_set_temp_time = 0;
                    }

                    if (cyclone_set_wind_flag)
                    {
                        EVENT = CYCLONE_WIND_REDUCE_EVENT;
                        set_done = TRUE;
                        cyclone_set_wind_time = 0;
                    }
                }
                else if (sFWG2_t.Cyclone_handle_work_mode == COLD_WIND_MODE)
                {
                    EVENT = CYCLONE_WIND_REDUCE_EVENT;
                    set_done = TRUE;
                }
            }
            else if (key_event[5] == KE_PRESS)
            {
                if (sFWG2_t.Cyclone_handle_work_mode == NORMAL_MODE)
                {
                    if (cyclone_set_temp_flag == false && cyclone_set_wind_flag == false)
                    {
                        cyclone_set_temp_flag = true;
                    }

                    if (cyclone_set_temp_flag)
                    {
                        EVENT = CYCLONE_TEMP_ADD_EVENT;
                        set_done = TRUE;
                        cyclone_set_temp_time = 0;
                    }

                    if (cyclone_set_wind_flag)
                    {
                        EVENT = CYCLONE_WIND_ADD_EVENT;
                        set_done = TRUE;
                        cyclone_set_wind_time = 0;
                    }
                }
                else if (sFWG2_t.Cyclone_handle_work_mode == COLD_WIND_MODE)
                {
                    EVENT = CYCLONE_WIND_ADD_EVENT;
                    set_done = TRUE;
                }
            }

            /* key long perss */
            if (key_event[0] == KE_LONG_PRESS)
            {
                /* enter cold mode */
                if (sFWG2_t.general_parameter.fn_key_set == SELECT_COLD_WIN_MODE)
                {
                    sFWG2_t.Direct_handle_work_mode = COLD_WIND_MODE;
                    sbeep.status = BEEP_LONG;
                    set_done = TRUE;
                }
                /* enter enhance mode */
                else if (sFWG2_t.general_parameter.fn_key_set == SELECT_QUICK_MODE)
                {
                    sFWG2_t.Direct_handle_work_mode = EN_WORKING_MODE;
                    sbeep.status = BEEP_LONG;
                    set_done = TRUE;
                }
                /* start countdown  */
                else if (sFWG2_t.general_parameter.fn_key_set == SELECT_COUNTDOWN_MODE && \
                         (sFWG2_t.general_parameter.fwg2_page == PAGE_SHOW_DIRECT_WORK || \
                          sFWG2_t.general_parameter.fwg2_page == PAGE_SHOW_CYCLONE_WORK))
                {
                    sFWG2_t.general_parameter.countdown_flag = true;
                    sbeep.status = BEEP_LONG;
                    set_done = TRUE;
                }
            }
            else  if (key_event[1] == KE_LONG_PRESS)
            {
                if (sFWG2_t.Direct_handle_work_mode == NORMAL_MODE)
                {
                    if (direct_set_temp_flag == false && direct_set_wind_flag == false)
                    {
                        direct_set_temp_flag = true;
                    }

                    if (direct_set_temp_flag)
                    {
                        EVENT = DIRECT_TEMP_REDUCE_EVENT;
                        //EVENT = DIRECT_TEMP_REDUCE_FIVE_EVENT;
                        direct_set_temp_time = 0;
                    }

                    if (direct_set_wind_flag)
                    {
                        EVENT = DIRECT_WIND_REDUCE_EVENT;
                        //EVENT = DIRECT_WIND_REDUCE_FIVE_EVENT;
                        direct_set_wind_time = 0;
                    }
                }
                else if (sFWG2_t.Direct_handle_work_mode == COLD_WIND_MODE)
                {
                    EVENT = DIRECT_WIND_REDUCE_EVENT;
                    //EVENT = DIRECT_WIND_REDUCE_FIVE_EVENT;
                }
            }
            else if (key_event[2] == KE_LONG_PRESS)
            {
                if (sFWG2_t.Direct_handle_work_mode == NORMAL_MODE)
                {
                    if (direct_set_temp_flag == false && direct_set_wind_flag == false)
                    {
                        direct_set_temp_flag = true;
                    }

                    if (direct_set_temp_flag)
                    {
                        EVENT = DIRECT_TEMP_ADD_EVENT;
                        //EVENT = DIRECT_TEMP_ADD_FIVE_EVENT;
                        direct_set_temp_time = 0;
                    }

                    if (direct_set_wind_flag)
                    {
                        EVENT = DIRECT_WIND_ADD_EVENT;
                        //EVENT = DIRECT_WIND_ADD_FIVE_EVENT;
                        direct_set_wind_time = 0;
                    }
                }
                else if (sFWG2_t.Direct_handle_work_mode == COLD_WIND_MODE)
                {
                    EVENT = DIRECT_WIND_ADD_EVENT;
                    //EVENT = DIRECT_WIND_ADD_FIVE_EVENT;
                }
            }
            else if (key_event[3] == KE_LONG_PRESS)
            {
                /* enter cold mode */
                if (sFWG2_t.general_parameter.fn_key_set == SELECT_COLD_WIN_MODE)
                {
                    sFWG2_t.Cyclone_handle_work_mode = COLD_WIND_MODE;
                    sbeep.status = BEEP_LONG;
                    set_done = TRUE;
                }
                /* enter enhance mode */
                else if (sFWG2_t.general_parameter.fn_key_set == SELECT_QUICK_MODE)
                {
                    sFWG2_t.Cyclone_handle_work_mode = EN_WORKING_MODE;
                    sbeep.status = BEEP_LONG;
                    set_done = TRUE;
                }
                /* start countdown  */
                else if (sFWG2_t.general_parameter.fn_key_set == SELECT_COUNTDOWN_MODE && \
                         (sFWG2_t.general_parameter.fwg2_page == PAGE_SHOW_DIRECT_WORK || \
                          sFWG2_t.general_parameter.fwg2_page == PAGE_SHOW_CYCLONE_WORK))
                {
                    sFWG2_t.general_parameter.countdown_flag = true;
                    sbeep.status = BEEP_LONG;
                    set_done = TRUE;
                }
            }
            else if (key_event[4] == KE_LONG_PRESS)
            {
                if (sFWG2_t.Cyclone_handle_work_mode == NORMAL_MODE)
                {
                    if (cyclone_set_temp_flag == false && cyclone_set_wind_flag == false)
                    {
                        cyclone_set_temp_flag = true;
                    }

                    if (cyclone_set_temp_flag)
                    {
                        EVENT = CYCLONE_TEMP_REDUCE_EVENT;
                        //EVENT = CYCLONE_TEMP_REDUCE_FIVE_EVENT;
                        cyclone_set_temp_time = 0;
                    }

                    if (cyclone_set_wind_flag)
                    {
                        EVENT = CYCLONE_WIND_REDUCE_EVENT;
                        //EVENT = CYCLONE_WIND_REDUCE_FIVE_EVENT;
                        cyclone_set_wind_time = 0;
                    }
                }
                else if (sFWG2_t.Cyclone_handle_work_mode == COLD_WIND_MODE)
                {
                    EVENT = CYCLONE_WIND_REDUCE_EVENT;
                    //EVENT = CYCLONE_WIND_REDUCE_FIVE_EVENT;
                }
            }
            else if (key_event[5] == KE_LONG_PRESS)
            {
                if (sFWG2_t.Cyclone_handle_work_mode == NORMAL_MODE)
                {
                    if (cyclone_set_temp_flag == false && cyclone_set_wind_flag == false)
                    {
                        cyclone_set_temp_flag = true;
                    }

                    if (cyclone_set_temp_flag)
                    {
                        EVENT = CYCLONE_TEMP_ADD_EVENT;
                        //EVENT = CYCLONE_TEMP_ADD_FIVE_EVENT;
                        cyclone_set_temp_time = 0;
                    }

                    if (cyclone_set_wind_flag)
                    {
                        EVENT = CYCLONE_WIND_ADD_EVENT;
                        //EVENT = CYCLONE_WIND_ADD_FIVE_EVENT;
                        cyclone_set_wind_time = 0;
                    }
                }
                else if (sFWG2_t.Cyclone_handle_work_mode == COLD_WIND_MODE)
                {
                    EVENT = CYCLONE_WIND_ADD_EVENT;
                    //EVENT = CYCLONE_WIND_ADD_FIVE_EVENT;
                }
            }
        }
    }
    else if (sFWG2_t.general_parameter.work_mode == CODE)
    {
        if (!set_done)
        {
            if (key_event[0] == KE_PRESS)
            {
                if (sFWG2_t.general_parameter.code_mode_handle_select == SELECT_DIRECT_HANDLE)
                {
                    if (sFWG2_t.general_parameter.code_mode_state == CODE_MODE_START)
                    {
                        sFWG2_t.general_parameter.code_mode_state = CODE_MODE_STOP;
                        sFWG2_t.general_parameter.code_mode_countdown_time_display = sFWG2_t.general_parameter.code0_pre_time;
                    }
                    else if (sFWG2_t.general_parameter.code_mode_state == CODE_MODE_STOP)
                    {
                        sFWG2_t.general_parameter.code_mode_state = CODE_MODE_START;
						sFWG2_t.general_parameter.code_mode_step = CODE_PRE_HEAT;
                    }

                    sbeep.status = BEEP_SHORT;
                    set_done = TRUE;
                }
            }

            if (key_event[3] == KE_PRESS)
            {
                if (sFWG2_t.general_parameter.code_mode_handle_select == SELECT_CYCLONE_HANDLE)
                {
                    if (sFWG2_t.general_parameter.code_mode_state == CODE_MODE_START)
                    {
                        sFWG2_t.general_parameter.code_mode_state = CODE_MODE_STOP;
                        sFWG2_t.general_parameter.code_mode_countdown_time_display = sFWG2_t.general_parameter.code0_pre_time;
                    }
                    else if (sFWG2_t.general_parameter.code_mode_state == CODE_MODE_STOP)
                    {
                        sFWG2_t.general_parameter.code_mode_state = CODE_MODE_START;
						sFWG2_t.general_parameter.code_mode_step = CODE_PRE_HEAT;
                    }

                    sbeep.status = BEEP_SHORT;
                    set_done = TRUE;
                }
            }
        }
    }
}

static void key_event_handle(void)
{
    switch (EVENT)
    {
    case DIRECT_TEMP_ADD_EVENT:
        if (sFWG2_t.general_parameter.temp_uint == CELSIUS)
        {
            if (sFWG2_t.Direct_handle_parameter.set_temp >= MAX_SET_TEMP_VAL)
            {
                sFWG2_t.Direct_handle_parameter.set_temp = MAX_SET_TEMP_VAL;
            }
            else
            {
                sFWG2_t.Direct_handle_parameter.set_temp++;
            }

            sFWG2_t.Direct_handle_parameter.set_temp_f_display  =  9 * sFWG2_t.Direct_handle_parameter.set_temp  / 5  + 32;
        }
        else if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
        {
            if (sFWG2_t.Direct_handle_parameter.set_temp_f_display >= 932)
            {
                sFWG2_t.Direct_handle_parameter.set_temp_f_display = 932;
            }
            else
            {
                sFWG2_t.Direct_handle_parameter.set_temp_f_display++;
            }

            sFWG2_t.Direct_handle_parameter.set_temp = (sFWG2_t.Direct_handle_parameter.set_temp_f_display - 32) * 5 / 9;
        }

        sbeep.status = BEEP_SHORT;
        EVENT = END_EVENT;
        break;

    case DIRECT_TEMP_ADD_FIVE_EVENT:
        if (sFWG2_t.Direct_handle_parameter.set_temp >= MAX_SET_TEMP_VAL - 5)
        {
            sFWG2_t.Direct_handle_parameter.set_temp = MAX_SET_TEMP_VAL;
        }
        else
        {
            sFWG2_t.Direct_handle_parameter.set_temp += 5;
        }

        sbeep.status = BEEP_SHORT;
        EVENT = END_EVENT;
        break;

    case DIRECT_TEMP_REDUCE_EVENT:
        if (sFWG2_t.general_parameter.temp_uint == CELSIUS)
        {
            if (sFWG2_t.Direct_handle_parameter.set_temp <= MIN_SET_TEMP_VAL)
            {
                sFWG2_t.Direct_handle_parameter.set_temp = MIN_SET_TEMP_VAL;
            }
            else
            {
                sFWG2_t.Direct_handle_parameter.set_temp--;
            }

            sFWG2_t.Direct_handle_parameter.set_temp_f_display  =  9 * sFWG2_t.Direct_handle_parameter.set_temp  / 5  + 32;
        }
        else if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
        {
            if (sFWG2_t.Direct_handle_parameter.set_temp_f_display <= 212)
            {
                sFWG2_t.Direct_handle_parameter.set_temp_f_display = 212;
            }
            else
            {
                sFWG2_t.Direct_handle_parameter.set_temp_f_display --;
            }

            sFWG2_t.Direct_handle_parameter.set_temp = (sFWG2_t.Direct_handle_parameter.set_temp_f_display - 32) * 5 / 9;
        }

        sbeep.status = BEEP_SHORT;
        EVENT = END_EVENT;
        break;

    case DIRECT_TEMP_REDUCE_FIVE_EVENT:
        if (sFWG2_t.Direct_handle_parameter.set_temp <= MIN_SET_TEMP_VAL + 5)
        {
            sFWG2_t.Direct_handle_parameter.set_temp = MIN_SET_TEMP_VAL;
        }
        else
        {
            sFWG2_t.Direct_handle_parameter.set_temp -= 5;
        }

        sbeep.status = BEEP_SHORT;
        EVENT = END_EVENT;
        break;

    case CYCLONE_TEMP_ADD_EVENT:
        if (sFWG2_t.general_parameter.temp_uint == CELSIUS)
        {
            if (sFWG2_t.Cyclone_handle_parameter.set_temp >= MAX_SET_TEMP_VAL)
            {
                sFWG2_t.Cyclone_handle_parameter.set_temp = MAX_SET_TEMP_VAL;
            }
            else
            {
                sFWG2_t.Cyclone_handle_parameter.set_temp++;
            }

            sFWG2_t.Cyclone_handle_parameter.set_temp_f_display =  9 * sFWG2_t.Cyclone_handle_parameter.set_temp  / 5  + 32;
        }
        else if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
        {
            if (sFWG2_t.Cyclone_handle_parameter.set_temp_f_display >= 932)
            {
                sFWG2_t.Cyclone_handle_parameter.set_temp_f_display = 932;
            }
            else
            {
                sFWG2_t.Cyclone_handle_parameter.set_temp_f_display ++;
            }

            sFWG2_t.Cyclone_handle_parameter.set_temp = (sFWG2_t.Cyclone_handle_parameter.set_temp_f_display - 32) * 5 / 9;
        }

        sbeep.status = BEEP_SHORT;
        EVENT = END_EVENT;
        break;

    case CYCLONE_TEMP_ADD_FIVE_EVENT:
        if (sFWG2_t.Cyclone_handle_parameter.set_temp >= MAX_SET_TEMP_VAL - 5)
        {
            sFWG2_t.Cyclone_handle_parameter.set_temp = MAX_SET_TEMP_VAL;
        }
        else
        {
            sFWG2_t.Cyclone_handle_parameter.set_temp += 5;
        }

        sbeep.status = BEEP_SHORT;
        EVENT = END_EVENT;
        break;

    case CYCLONE_TEMP_REDUCE_EVENT:
        if (sFWG2_t.general_parameter.temp_uint == CELSIUS)
        {
            if (sFWG2_t.Cyclone_handle_parameter.set_temp <= MIN_SET_TEMP_VAL)
            {
                sFWG2_t.Cyclone_handle_parameter.set_temp = MIN_SET_TEMP_VAL;
            }
            else
            {
                sFWG2_t.Cyclone_handle_parameter.set_temp--;
            }

            sFWG2_t.Cyclone_handle_parameter.set_temp_f_display =  9 * sFWG2_t.Cyclone_handle_parameter.set_temp  / 5  + 32;
        }
        else if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
        {
            if (sFWG2_t.Cyclone_handle_parameter.set_temp_f_display <= 212)
            {
                sFWG2_t.Cyclone_handle_parameter.set_temp_f_display = 212;
            }
            else
            {
                sFWG2_t.Cyclone_handle_parameter.set_temp_f_display --;
            }

            sFWG2_t.Cyclone_handle_parameter.set_temp = (sFWG2_t.Cyclone_handle_parameter.set_temp_f_display - 32) * 5 / 9;
        }

        sbeep.status = BEEP_SHORT;
        EVENT = END_EVENT;
        break;

    case CYCLONE_TEMP_REDUCE_FIVE_EVENT:
        if (sFWG2_t.Cyclone_handle_parameter.set_temp <= MIN_SET_TEMP_VAL + 5)
        {
            sFWG2_t.Cyclone_handle_parameter.set_temp = MIN_SET_TEMP_VAL;
        }
        else
        {
            sFWG2_t.Cyclone_handle_parameter.set_temp -= 5;
        }

        sbeep.status = BEEP_SHORT;
        EVENT = END_EVENT;
        break;

    case DIRECT_WIND_REDUCE_EVENT:
        if (sFWG2_t.Direct_handle_work_mode  ==  NORMAL_MODE)
        {
            if (sFWG2_t.Direct_handle_parameter.set_wind <= MIN_SET_WIND_VAL)
            {
                sFWG2_t.Direct_handle_parameter.set_wind = MIN_SET_WIND_VAL;
            }
            else
            {
                sFWG2_t.Direct_handle_parameter.set_wind --;
            }
        }
        else if (sFWG2_t.Direct_handle_work_mode  ==  COLD_WIND_MODE)
        {
            if (sFWG2_t.Direct_handle_parameter.cold_mode_set_wind <= MIN_SET_WIND_VAL)
            {
                sFWG2_t.Direct_handle_parameter.cold_mode_set_wind = MIN_SET_WIND_VAL;
            }
            else
            {
                sFWG2_t.Direct_handle_parameter.cold_mode_set_wind --;
            }
        }

        sbeep.status = BEEP_SHORT;
        EVENT = END_EVENT;
        break;

    case DIRECT_WIND_ADD_EVENT:
        if (sFWG2_t.Direct_handle_work_mode == NORMAL_MODE)
        {
            if (sFWG2_t.Direct_handle_parameter.set_wind >= MAX_SET_WIND_VAL)
            {
                sFWG2_t.Direct_handle_parameter.set_wind = MAX_SET_WIND_VAL;
            }
            else
            {
                sFWG2_t.Direct_handle_parameter.set_wind ++;
            }
        }
        else if (sFWG2_t.Direct_handle_work_mode == COLD_WIND_MODE)
        {
            if (sFWG2_t.Direct_handle_parameter.cold_mode_set_wind >= MAX_SET_WIND_VAL)
            {
                sFWG2_t.Direct_handle_parameter.cold_mode_set_wind = MAX_SET_WIND_VAL;
            }
            else
            {
                sFWG2_t.Direct_handle_parameter.cold_mode_set_wind ++;
            }
        }

        sbeep.status = BEEP_SHORT;
        EVENT = END_EVENT;
        break;

    case DIRECT_WIND_REDUCE_FIVE_EVENT:
        if (sFWG2_t.Direct_handle_work_mode == NORMAL_MODE)
        {
            if (sFWG2_t.Direct_handle_parameter.set_wind <= MIN_SET_WIND_VAL + 5)
            {
                sFWG2_t.Direct_handle_parameter.set_wind = MIN_SET_WIND_VAL;
            }
            else
            {
                sFWG2_t.Direct_handle_parameter.set_wind -= 5;
            }
        }
        else if (sFWG2_t.Direct_handle_work_mode == COLD_WIND_MODE)
        {
            if (sFWG2_t.Direct_handle_parameter.cold_mode_set_wind <= MIN_SET_WIND_VAL + 5)
            {
                sFWG2_t.Direct_handle_parameter.cold_mode_set_wind = MIN_SET_WIND_VAL;
            }
            else
            {
                sFWG2_t.Direct_handle_parameter.cold_mode_set_wind -= 5;
            }
        }

        sbeep.status = BEEP_SHORT;
        EVENT = END_EVENT;
        break;

    case DIRECT_WIND_ADD_FIVE_EVENT:
        if (sFWG2_t.Direct_handle_work_mode == NORMAL_MODE)
        {
            if (sFWG2_t.Direct_handle_parameter.set_wind >= MAX_SET_WIND_VAL - 5)
            {
                sFWG2_t.Direct_handle_parameter.set_wind = MAX_SET_WIND_VAL;
            }
            else
            {
                sFWG2_t.Direct_handle_parameter.set_wind += 5;
            }
        }
        else if (sFWG2_t.Direct_handle_work_mode == COLD_WIND_MODE)
        {
            if (sFWG2_t.Direct_handle_parameter.cold_mode_set_wind >= MAX_SET_WIND_VAL - 5)
            {
                sFWG2_t.Direct_handle_parameter.cold_mode_set_wind = MAX_SET_WIND_VAL;
            }
            else
            {
                sFWG2_t.Direct_handle_parameter.cold_mode_set_wind += 5;
            }
        }

        sbeep.status = BEEP_SHORT;
        EVENT = END_EVENT;
        break;

    case CYCLONE_WIND_REDUCE_EVENT:
        if (sFWG2_t.Cyclone_handle_work_mode == NORMAL_MODE)
        {
            if (sFWG2_t.Cyclone_handle_parameter.set_wind <= MIN_SET_WIND_VAL)
            {
                sFWG2_t.Cyclone_handle_parameter.set_wind = MIN_SET_WIND_VAL;
            }
            else
            {
                sFWG2_t.Cyclone_handle_parameter.set_wind --;
            }
        }
        else if (sFWG2_t.Cyclone_handle_work_mode == COLD_WIND_MODE)
        {
            if (sFWG2_t.Cyclone_handle_parameter.cold_mode_set_wind <= MIN_SET_WIND_VAL)
            {
                sFWG2_t.Cyclone_handle_parameter.cold_mode_set_wind = MIN_SET_WIND_VAL;
            }
            else
            {
                sFWG2_t.Cyclone_handle_parameter.cold_mode_set_wind --;
            }
        }

        sbeep.status = BEEP_SHORT;
        EVENT = END_EVENT;
        break;

    case CYCLONE_WIND_ADD_EVENT:
        if (sFWG2_t.Cyclone_handle_work_mode == NORMAL_MODE)
        {
            if (sFWG2_t.Cyclone_handle_parameter.set_wind >= MAX_SET_WIND_VAL)
            {
                sFWG2_t.Cyclone_handle_parameter.set_wind = MAX_SET_WIND_VAL;
            }
            else
            {
                sFWG2_t.Cyclone_handle_parameter.set_wind ++;
            }
        }
        else if (sFWG2_t.Cyclone_handle_work_mode == COLD_WIND_MODE)
        {
            if (sFWG2_t.Cyclone_handle_parameter.cold_mode_set_wind >= MAX_SET_WIND_VAL)
            {
                sFWG2_t.Cyclone_handle_parameter.cold_mode_set_wind = MAX_SET_WIND_VAL;
            }
            else
            {
                sFWG2_t.Cyclone_handle_parameter.cold_mode_set_wind ++;
            }
        }

        sbeep.status = BEEP_SHORT;
        EVENT = END_EVENT;
        break;

    case CYCLONE_WIND_REDUCE_FIVE_EVENT:
        if (sFWG2_t.Cyclone_handle_work_mode == NORMAL_MODE)
        {
            if (sFWG2_t.Cyclone_handle_parameter.set_wind <= MIN_SET_WIND_VAL + 5)
            {
                sFWG2_t.Cyclone_handle_parameter.set_wind = MIN_SET_WIND_VAL;
            }
            else
            {
                sFWG2_t.Cyclone_handle_parameter.set_wind -= 5;
            }
        }
        else if (sFWG2_t.Cyclone_handle_work_mode == COLD_WIND_MODE)
        {
            if (sFWG2_t.Cyclone_handle_parameter.cold_mode_set_wind <= MIN_SET_WIND_VAL + 5)
            {
                sFWG2_t.Cyclone_handle_parameter.cold_mode_set_wind = MIN_SET_WIND_VAL;
            }
            else
            {
                sFWG2_t.Cyclone_handle_parameter.cold_mode_set_wind -= 5;
            }
        }

        sbeep.status = BEEP_SHORT;
        EVENT = END_EVENT;
        break;

    case CYCLONE_WIND_ADD_FIVE_EVENT:
        if (sFWG2_t.Cyclone_handle_work_mode == NORMAL_MODE)
        {
            if (sFWG2_t.Cyclone_handle_parameter.set_wind >= MAX_SET_WIND_VAL - 5)
            {
                sFWG2_t.Cyclone_handle_parameter.set_wind = MAX_SET_WIND_VAL;
            }
            else
            {
                sFWG2_t.Cyclone_handle_parameter.set_wind += 5;
            }
        }
        else if (sFWG2_t.Cyclone_handle_work_mode == COLD_WIND_MODE)
        {
            if (sFWG2_t.Cyclone_handle_parameter.cold_mode_set_wind >= MAX_SET_WIND_VAL - 5)
            {
                sFWG2_t.Cyclone_handle_parameter.cold_mode_set_wind = MAX_SET_WIND_VAL;
            }
            else
            {
                sFWG2_t.Cyclone_handle_parameter.cold_mode_set_wind  += 5;
            }
        }

        sbeep.status = BEEP_SHORT;
        EVENT = END_EVENT;
        break;

    case SET_CH_EVENT:
        if (sFWG2_t.general_parameter.ch >= 4)
        {
            sFWG2_t.general_parameter.ch = 1;
        }
        else
        {
            sFWG2_t.general_parameter.ch ++;
        }

        if (sFWG2_t.general_parameter.fwg2_page == PAGE_MAIN || \
               sFWG2_t.general_parameter.fwg2_page == PAGE_DIRECT_CYCLONE_CURVE)
        {
            if (sFWG2_t.general_parameter.ch == 1)
            {
                sFWG2_t.Cyclone_handle_parameter.set_temp = sFWG2_t.general_parameter.ch1_set_temp;
                sFWG2_t.Cyclone_handle_parameter.set_wind = sFWG2_t.general_parameter.ch1_set_wind;
                sFWG2_t.general_parameter.countdown_time  = sFWG2_t.general_parameter.ch1_set_time;
                sFWG2_t.Direct_handle_parameter.set_temp = sFWG2_t.general_parameter.ch1_set_temp;
                sFWG2_t.Direct_handle_parameter.set_wind = sFWG2_t.general_parameter.ch1_set_wind;
            }
            else if (sFWG2_t.general_parameter.ch == 2)
            {
                sFWG2_t.Cyclone_handle_parameter.set_temp = sFWG2_t.general_parameter.ch2_set_temp;
                sFWG2_t.Cyclone_handle_parameter.set_wind = sFWG2_t.general_parameter.ch2_set_wind;
                sFWG2_t.Direct_handle_parameter.set_temp = sFWG2_t.general_parameter.ch2_set_temp;
                sFWG2_t.Direct_handle_parameter.set_wind = sFWG2_t.general_parameter.ch2_set_wind;
                sFWG2_t.general_parameter.countdown_time  = sFWG2_t.general_parameter.ch2_set_time;
            }
            else if (sFWG2_t.general_parameter.ch == 3)
            {
                sFWG2_t.Cyclone_handle_parameter.set_temp = sFWG2_t.general_parameter.ch3_set_temp;
                sFWG2_t.Cyclone_handle_parameter.set_wind = sFWG2_t.general_parameter.ch3_set_wind;
                sFWG2_t.Direct_handle_parameter.set_temp = sFWG2_t.general_parameter.ch3_set_temp;
                sFWG2_t.Direct_handle_parameter.set_wind = sFWG2_t.general_parameter.ch3_set_wind;
                sFWG2_t.general_parameter.countdown_time  = sFWG2_t.general_parameter.ch3_set_time;
            }
            else if (sFWG2_t.general_parameter.ch == 4)
            {
                sFWG2_t.Cyclone_handle_parameter.set_temp = sFWG2_t.general_parameter.ch4_set_temp;
                sFWG2_t.Cyclone_handle_parameter.set_wind = sFWG2_t.general_parameter.ch4_set_wind;
                sFWG2_t.Direct_handle_parameter.set_temp = sFWG2_t.general_parameter.ch4_set_temp;
                sFWG2_t.Direct_handle_parameter.set_wind = sFWG2_t.general_parameter.ch4_set_wind;
                sFWG2_t.general_parameter.countdown_time  = sFWG2_t.general_parameter.ch4_set_time;
            }
        }
        else if (sFWG2_t.general_parameter.fwg2_page == PAGE_SHOW_DIRECT_WORK   || \
                 sFWG2_t.general_parameter.fwg2_page == PAGE_SHOW_DIRECT_CURVE)
        {
            if (sFWG2_t.general_parameter.ch == 1)
            {
                sFWG2_t.general_parameter.countdown_time  = sFWG2_t.general_parameter.ch1_set_time;
                sFWG2_t.Direct_handle_parameter.set_temp = sFWG2_t.general_parameter.ch1_set_temp;
                sFWG2_t.Direct_handle_parameter.set_wind = sFWG2_t.general_parameter.ch1_set_wind;
            }
            else if (sFWG2_t.general_parameter.ch == 2)
            {
                sFWG2_t.Direct_handle_parameter.set_temp = sFWG2_t.general_parameter.ch2_set_temp;
                sFWG2_t.Direct_handle_parameter.set_wind = sFWG2_t.general_parameter.ch2_set_wind;
                sFWG2_t.general_parameter.countdown_time  = sFWG2_t.general_parameter.ch2_set_time;
            }
            else if (sFWG2_t.general_parameter.ch == 3)
            {
                sFWG2_t.Direct_handle_parameter.set_temp = sFWG2_t.general_parameter.ch3_set_temp;
                sFWG2_t.Direct_handle_parameter.set_wind = sFWG2_t.general_parameter.ch3_set_wind;
                sFWG2_t.general_parameter.countdown_time  = sFWG2_t.general_parameter.ch3_set_time;
            }
            else if (sFWG2_t.general_parameter.ch == 4)
            {
                sFWG2_t.Direct_handle_parameter.set_temp = sFWG2_t.general_parameter.ch4_set_temp;
                sFWG2_t.Direct_handle_parameter.set_wind = sFWG2_t.general_parameter.ch4_set_wind;
                sFWG2_t.general_parameter.countdown_time  = sFWG2_t.general_parameter.ch4_set_time;
            }
        }
        else if (sFWG2_t.general_parameter.fwg2_page == PAGE_SHOW_CYCLONE_WORK  || \
                 sFWG2_t.general_parameter.fwg2_page == PAGE_SHOW_CYCLONE_CURVE)
        {
            if (sFWG2_t.general_parameter.ch == 1)
            {
                sFWG2_t.Cyclone_handle_parameter.set_temp = sFWG2_t.general_parameter.ch1_set_temp;
                sFWG2_t.Cyclone_handle_parameter.set_wind = sFWG2_t.general_parameter.ch1_set_wind;
                sFWG2_t.general_parameter.countdown_time  = sFWG2_t.general_parameter.ch1_set_time;
            }
            else if (sFWG2_t.general_parameter.ch == 2)
            {
                sFWG2_t.Cyclone_handle_parameter.set_temp = sFWG2_t.general_parameter.ch2_set_temp;
                sFWG2_t.Cyclone_handle_parameter.set_wind = sFWG2_t.general_parameter.ch2_set_wind;
                sFWG2_t.general_parameter.countdown_time  = sFWG2_t.general_parameter.ch2_set_time;
            }
            else if (sFWG2_t.general_parameter.ch == 3)
            {
                sFWG2_t.Cyclone_handle_parameter.set_temp = sFWG2_t.general_parameter.ch3_set_temp;
                sFWG2_t.Cyclone_handle_parameter.set_wind = sFWG2_t.general_parameter.ch3_set_wind;
                sFWG2_t.general_parameter.countdown_time  = sFWG2_t.general_parameter.ch3_set_time;
            }
            else if (sFWG2_t.general_parameter.ch == 4)
            {
                sFWG2_t.Cyclone_handle_parameter.set_temp = sFWG2_t.general_parameter.ch4_set_temp;
                sFWG2_t.Cyclone_handle_parameter.set_wind = sFWG2_t.general_parameter.ch4_set_wind;
                sFWG2_t.general_parameter.countdown_time  = sFWG2_t.general_parameter.ch4_set_time;
            }
        }
		
		 if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
        {
			sFWG2_t.Direct_handle_parameter.set_temp_f_display  =  9 * sFWG2_t.Direct_handle_parameter.set_temp  / 5  + 32;
            sFWG2_t.Cyclone_handle_parameter.set_temp_f_display  =  9 * sFWG2_t.Cyclone_handle_parameter.set_temp  / 5  + 32;
        }
		

        sbeep.status = BEEP_SHORT;
        EVENT = END_EVENT;
        break;

    case TIME_COUNTDOWN_EVENT:
        break;

    case VALUE_RESET_EVENT:
        break;

    case END_EVENT:
        break;
    }
}



void key_handle(void)
{
    static  handle_event event = END_EVENT;
    static  uint8_t set_done = FALSE;
#if 1

    if (get_key_hel1()         == K_PRESS || \
            get_key_hel2()         == K_PRESS || \
            get_key_hel3()         == K_PRESS)
    {
        if (sFWG2_t.Direct_handle_state == HANDLE_SLEEP)
        {
            sFWG2_t.Direct_handle_state = HANDLE_WORKING;
        }
        else if (sFWG2_t.Direct_handle_state == HANDLE_WORKING)
        {
            sFWG2_t.Direct_handle_parameter.sleep_time = 0;
        }
    }

    if (get_key_hel4()         == K_PRESS || \
            get_key_hel5()         == K_PRESS || \
            get_key_hel6()         == K_PRESS)
    {
        if (sFWG2_t.Cyclone_handle_state == HANDLE_SLEEP)
        {
            sFWG2_t.Cyclone_handle_state = HANDLE_WORKING;
        }
        else if (sFWG2_t.Cyclone_handle_state == HANDLE_WORKING)
        {
            sFWG2_t.Cyclone_handle_parameter.sleep_time = 0;
        }
    }

#endif
    KeyProc();
    key_event_handle();
}

