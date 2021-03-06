//
// Created by wang_shuai on 2021/1/5.
//
// GUI service to help other process create avaliable graphics

#include "stdio.h"
#include <gui/gui.h>
#include "./lvgl/lvgl.h"
#include <ipc/ipc.h>
#include "function_table.h"
#include <ipc/signal.h>

//#include "./demo/lv_demo_widgets.h"
#include "callbacks.h"


static uint32_t buf[LV_HOR_RES_MAX * LV_VER_RES_MAX  / 10]; /*Declare a buffer for 1/10 screen size*/
static uint32_t buf_[LV_HOR_RES_MAX * LV_VER_RES_MAX / 10];
static int  touchpad_x = 200;
static int  touchpad_y = 200;

//int ssss[100] = {1};

static uint32_t left_pressed __attribute__((section(".data")));
static uint32_t right_pressed __attribute__((section(".data"))) ;

void static itoa(char str[], int num)/* 数字前面的 0 不被显示出来, 比如 0000B800 被显示成 B800 */
{
    char buf[100];
    int i, j;
    if(num == 0) {
        str[0] = '0';
        str[1] = 0;
        return ;
    }
    for(i = 0; num; num /= 10, i++) {
        buf[i] = num % 10;
    }
    for(j = 0, i = i-1; i >= 0; j++, i--) {
        str[j] = buf[i] + '0';
    }
    str[j] = 0;

}

void Broadcast() {
    IPC_MSG msg = {
        .src = -1,
        .dst = 2,
        .type = P2P_A,
        .data = {0}
	};
	send(&msg);
}

// uint32_t _Handler_t, _Test_t;
// lv_obj_t *_text;
// void CallBack(lv_obj_t * obj, lv_event_t event){
//     if(event == LV_EVENT_CLICKED) {
        // printf("Clicked\n");
//         lv_textarea_add_char(_text, '%');
//     }
//     else if(event == LV_EVENT_VALUE_CHANGED) {
        // printf("Toggled\n");
//     }
    // IPC_MSG sig_test ;
    // memset(sig_test.data, 0, sizeof(sig_test.data));
    // sig_test.dst = 2;
    // sig_test.type = Signal;
    // sig_test.data[0] = SIG_SEND; 
    // sig_test.data[1] = _Handler_t;
    // sig_test.data[2] = _Test_t;
    // send(&sig_test);
    // switch(event) {
    //     case LV_EVENT_CLICKED : 

    //         break;
    // }
// }

static void test_button()
{
   static lv_anim_path_t path_overshoot;
   lv_anim_path_init(&path_overshoot);
   lv_anim_path_set_cb(&path_overshoot, lv_anim_path_overshoot);

   static lv_anim_path_t path_ease_out;
   lv_anim_path_init(&path_ease_out);
   lv_anim_path_set_cb(&path_ease_out, lv_anim_path_ease_out);

   static lv_anim_path_t path_ease_in_out;
   lv_anim_path_init(&path_ease_in_out);
   lv_anim_path_set_cb(&path_ease_in_out, lv_anim_path_ease_in_out);

   /*Gum-like button*/
   static lv_style_t style_gum;
   lv_style_init(&style_gum);
   lv_style_set_transform_width(&style_gum, LV_STATE_PRESSED, 10);
   lv_style_set_transform_height(&style_gum, LV_STATE_PRESSED, -10);
   lv_style_set_value_letter_space(&style_gum, LV_STATE_PRESSED, 5);
   lv_style_set_transition_path(&style_gum, LV_STATE_DEFAULT, &path_overshoot);
   lv_style_set_transition_path(&style_gum, LV_STATE_PRESSED, &path_ease_in_out);
   lv_style_set_transition_time(&style_gum, LV_STATE_DEFAULT, 250);
   lv_style_set_transition_delay(&style_gum, LV_STATE_DEFAULT, 100);
   lv_style_set_transition_prop_1(&style_gum, LV_STATE_DEFAULT, LV_STYLE_TRANSFORM_WIDTH);
   lv_style_set_transition_prop_2(&style_gum, LV_STATE_DEFAULT, LV_STYLE_TRANSFORM_HEIGHT);
   lv_style_set_transition_prop_3(&style_gum, LV_STATE_DEFAULT, LV_STYLE_VALUE_LETTER_SPACE);

   lv_obj_t * btn4 = lv_btn_create(lv_scr_act(), NULL);
   lv_obj_align(btn4, NULL, LV_ALIGN_CENTER, 0, -80);
   lv_obj_add_style(btn4, LV_BTN_PART_MAIN, &style_gum);

   /*Instead of creating a label add a values string*/
   lv_obj_set_style_local_value_str(btn4, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, "Gum");

   /*Halo on press*/
   static lv_style_t style_halo;
   lv_style_init(&style_halo);
   lv_style_set_transition_time(&style_halo, LV_STATE_PRESSED, 400);
   lv_style_set_transition_time(&style_halo, LV_STATE_DEFAULT, 0);
   lv_style_set_transition_delay(&style_halo, LV_STATE_DEFAULT, 200);
   lv_style_set_outline_width(&style_halo, LV_STATE_DEFAULT, 0);
   lv_style_set_outline_width(&style_halo, LV_STATE_PRESSED, 20);
   lv_style_set_outline_opa(&style_halo, LV_STATE_DEFAULT, LV_OPA_COVER);
   lv_style_set_outline_opa(&style_halo, LV_STATE_FOCUSED, LV_OPA_COVER);   /*Just to be sure, the theme might use it*/
   lv_style_set_outline_opa(&style_halo, LV_STATE_PRESSED, LV_OPA_TRANSP);
   lv_style_set_transition_prop_1(&style_halo, LV_STATE_DEFAULT, LV_STYLE_OUTLINE_OPA);
   lv_style_set_transition_prop_2(&style_halo, LV_STATE_DEFAULT, LV_STYLE_OUTLINE_WIDTH);

   lv_obj_t * btn5 = lv_btn_create(lv_scr_act(), NULL);
   lv_obj_align(btn5, NULL, LV_ALIGN_CENTER, 0, 0);
   lv_obj_add_style(btn5, LV_BTN_PART_MAIN, &style_halo);
   lv_obj_set_style_local_value_str(btn5, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, "MiniX");

   lv_obj_set_x(btn4, 200);
   lv_obj_set_y(btn4, 70);

   lv_obj_set_x(btn5, 200);
   lv_obj_set_y(btn5, 140);
   /*Ripple on press*/
   static lv_style_t style_ripple;
   lv_style_init(&style_ripple);
   lv_style_set_transition_time(&style_ripple, LV_STATE_PRESSED, 300);
   lv_style_set_transition_time(&style_ripple, LV_STATE_DEFAULT, 0);
   lv_style_set_transition_delay(&style_ripple, LV_STATE_DEFAULT, 300);
   lv_style_set_bg_opa(&style_ripple, LV_STATE_DEFAULT, 0);
   lv_style_set_bg_opa(&style_ripple, LV_STATE_PRESSED, LV_OPA_80);
   lv_style_set_border_width(&style_ripple, LV_STATE_DEFAULT, 0);
   lv_style_set_outline_width(&style_ripple, LV_STATE_DEFAULT, 0);
   lv_style_set_transform_width(&style_ripple, LV_STATE_DEFAULT, -20);
   lv_style_set_transform_height(&style_ripple, LV_STATE_DEFAULT, -20);
   lv_style_set_transform_width(&style_ripple, LV_STATE_PRESSED, 0);
   lv_style_set_transform_height(&style_ripple, LV_STATE_PRESSED, 0);

   lv_style_set_transition_path(&style_ripple, LV_STATE_DEFAULT, &path_ease_out);
   lv_style_set_transition_prop_1(&style_ripple, LV_STATE_DEFAULT, LV_STYLE_BG_OPA);
   lv_style_set_transition_prop_2(&style_ripple, LV_STATE_DEFAULT, LV_STYLE_TRANSFORM_WIDTH);
   lv_style_set_transition_prop_3(&style_ripple, LV_STATE_DEFAULT, LV_STYLE_TRANSFORM_HEIGHT);

   lv_obj_t * btn3 = lv_btn_create(lv_scr_act(), NULL);
   lv_obj_align(btn3, NULL, LV_ALIGN_CENTER, 0, 80);
   lv_obj_add_style(btn3, LV_BTN_PART_MAIN, &style_ripple);
   lv_obj_set_style_local_value_str(btn3, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, "Ripple");
}

static void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    struct ROI roi = {
        .x1 = area->x1,
        .x2 = area->x2,
        .y1 = area->y1,
        .y2 = area->y2,
        .color = (Color *)color_p};
    GraphFlush(&roi);
    lv_disp_flush_ready(disp); /* Indicate you are ready with the flushing*/
}

bool my_input_read(lv_indev_drv_t *drv, lv_indev_data_t *data)
{
    data->point.x = touchpad_x;// > 0 ? (touchpad_x < 640 ? touchpad_x : 640): 0;
    data->point.y = touchpad_y;// > 0 ? (touchpad_y < 480 ? touchpad_y : 480): 0;
    data->state = left_pressed == 1 ? LV_INDEV_STATE_PR : LV_INDEV_STATE_REL;
    return false; /*No buffering now so no more data read*/
}

static int kb_buf_head, kb_buf_tail;
static char kb_buf[256];
static char last_key() {
    if((kb_buf_tail + 256 - kb_buf_head) % 256 > 0) {
        return kb_buf[kb_buf_head++];
    }
    return 0;
}
bool keyboard_read(lv_indev_drv_t * drv, lv_indev_data_t*data){
    data->key = last_key();            /*Get the last pressed or released key*/
    data->state = LV_INDEV_STATE_PR;
//   if(key_pressed()) data->state = LV_INDEV_STATE_PR;
//   else data->state = LV_INDEV_STATE_REL;

  return false; /*No buffering now so no more data read*/
}

int  main(int arg, char *argv[])
{
    int stdin = open("dev_tty0", O_RDWR);
    int stdout = open("dev_tty0", O_RDWR);
    int stderr = open("dev_tty0", O_RDWR);

    printf("o");

    InitLvFontMontserrat_14();
    lv_init();

    static lv_disp_buf_t disp_buf;
    lv_disp_buf_init(&disp_buf, buf, buf_, LV_HOR_RES_MAX * LV_VER_RES_MAX /10);

    /* Register disp*/
    lv_disp_drv_t disp_drv;            /*Descriptor of a display driver*/
    lv_disp_drv_init(&disp_drv);       /*Basic initialization*/
    disp_drv.flush_cb = my_disp_flush; /*Set your driver function*/
    disp_drv.buffer = &disp_buf;       /*Assign the buffer to the display*/
    lv_disp_drv_register(&disp_drv);   /*Finally register the driver*/

    /* Register Input MousePtr */
    lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);      /*Basic initialization*/
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = my_input_read;
    lv_indev_t * mouse_indev = lv_indev_drv_register(&indev_drv);

    LV_IMG_DECLARE(my_img_dsc);                          /*Declare the image file.*/
    lv_obj_t * cursor_obj =  lv_img_create(lv_scr_act(), NULL); /*Create an image object for the cursor */
    lv_img_set_src(cursor_obj, &my_img_dsc);             /*Set the image source*/
    lv_indev_set_cursor(mouse_indev, cursor_obj);

    lv_indev_drv_t indev_drv2;
    lv_indev_drv_init(&indev_drv2);      
    indev_drv2.type = LV_INDEV_TYPE_KEYPAD;
    indev_drv2.read_cb = keyboard_read;
    kb_buf_head = 0; kb_buf_tail = 0;
    //  ----------------------------------------------------------------------------
    //lv_demo_widgets();
    //while (1);

    lv_obj_t * label;
    lv_obj_t * label2;
    lv_obj_t *win = lv_win_create(lv_scr_act(), NULL);
    lv_win_set_title(win, "\t\t\t\t\t\t\t\t\t\t\t\t\t\tThe OS powered by LVGL");
    lv_win_set_drag(win, true);
    lv_obj_set_x(win, 10);
    lv_obj_set_y(win, 10);
    lv_obj_set_width(win, 400);
    lv_obj_set_height(win, 200);


    static lv_style_t style1;
    lv_style_set_bg_color(&style1, LV_STATE_DEFAULT, LV_COLOR_BLUE);
    //lv_style_set_bg_color(&style1, LV_STATE_PRESSED, LV_COLOR_GRAY);
    //lv_style_set_bg_color(&style1, LV_STATE_FOCUSED, LV_COLOR_RED);
    //lv_style_set_bg_color(&style1, LV_STATE_FOCUSED | LV_STATE_PRESSED, lv_color_hex(0xf88));
    lv_obj_add_style(win, LV_WIN_PART_BG, &style1);

    //lv_win_set_content_size(win, 480, 480);
//    test_button();


    lv_obj_t *btn1 = lv_btn_create(win, NULL);
    label = lv_label_create(btn1, NULL);
    lv_label_set_text(label, "Button");

    lv_obj_t *btn2 = lv_btn_create(win, NULL);
    label2 = lv_label_create(btn2, NULL);
    lv_label_set_text(label2, "Button2");

    lv_obj_set_x(btn1, 20);
    lv_obj_set_y(btn1, 20);

    lv_obj_set_x(btn2, 20);
    lv_obj_set_y(btn2, 70);    

    lv_obj_t * ta1;
    ta1 = lv_textarea_create(lv_scr_act(), NULL);
    // _text = ta1;
    lv_obj_set_size(ta1, 200, 100);
    lv_obj_align(ta1, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_textarea_set_text(ta1, "A text in a Text Area");    /*Set an initial text*/

    lv_group_t *g = lv_group_create();
    lv_group_add_obj(g, ta1);
    lv_indev_set_group(&indev_drv2, g);
    

    char buf_X[10];
    char buf_Y[10];
    IPC_MSG msg;

    Broadcast();


    int tick_T = 0;
    lv_obj_t *cur_btn;

    void* _func;
    int args;
    int (*_func_1)(int);
    int (*_func_2)(int, int);
    int (*_func_3)(int, int, int);

    uint32_t ack_data;
    while (1)
    {
        /* code */
        if(tick_T == 0) {
            lv_tick_inc(1);
        }
        tick_T = (tick_T + 1) % 20;

        int recv_code = recv(&msg);
        if (recv_code != SUCCESS)
        {
        }
        else
        {
            GUI *usr_data = (GUI*)(&msg.data);
            switch (usr_data->type)
            {
            case Button:
                cur_btn = lv_btn_create(lv_scr_act(), NULL);
                lv_obj_set_x(cur_btn, usr_data->data[0]);
                lv_obj_set_y(cur_btn, usr_data->data[1]);
                ack_data = (uint32_t)cur_btn;
                break;
            case Window:
                break;
            case Keyboard:
                // printf("%c", usr_data->data[0]);
                // lv_textarea_add_char(ta1, usr_data->data[0]);
                if((kb_buf_tail + 256 - kb_buf_head) % 256 < 255) {
                    kb_buf[kb_buf_tail++] = usr_data->data[0];
                }
                break;
            case Mouse:
            {
                touchpad_x = usr_data->data[2];
                touchpad_y = usr_data->data[3];



                left_pressed = usr_data->data[0];
                if(left_pressed)
                    lv_textarea_add_char(ta1, '*');
                int pos = 0;
                itoa(buf_X, usr_data->data[2]);
                itoa(buf_Y, usr_data->data[3]);
                lv_label_set_text(label, buf_X);
                lv_label_set_text(label2, buf_Y);


                break;
            }
            case Function :
                _func = FunctionTable[msg.data[1]];
                args = msg.data[2];
                switch (args) {
                    case 0 : break;
                    case 1 :
                        _func_1 = _func;
                        ack_data =  _func_1(msg.data[3]);
                        break;
                    case 2 :
                        _func_2 = _func;
                        ack_data = _func_2(msg.data[3], msg.data[4]);
                        break;
                    case 3 :
                        _func_3 = _func;
                        ack_data = _func_3(msg.data[3], msg.data[4], msg.data[5]);
                        break;
                }
                break;
            case RegisterCallback :

                if(_call_back_num < _max_call_back) {
                    _Handler[_call_back_num] = msg.data[2];
                    _Test[_call_back_num] = msg.data[3];
                    _dst[_call_back_num] = msg.src;
                    lv_obj_set_event_cb(msg.data[1], CallBackTable[_call_back_num]);
                    // lv_obj_set_event_cb(msg.data[1], CallBack);
                    _call_back_num++;
                }
                // lv_obj_set_event_cb(msg.data[1], CallBack);
                // _Handler_t = msg.data[2];
                // _Test_t = msg.data[3];
                // send(&sig_test);
                break;
            }
            if(msg.type == P2P_S) {
                IPC_MSG ack_msg = {
                    .src = -1,
                    .dst = msg.src,
                    .type = ACK,
                    .data = {ack_data}
                };
                send(&ack_msg);
            }
        }

        lv_task_handler();
    }

    return;
}
