// Copyright (c) 2022 Cesanta Software Limited
// All rights reserved

#include <inttypes.h>
#include <stdbool.h>

//SYS
#include "ss_makros.h"
#include "ss_gpio.h"
#include "ss_delay.h"
#include "ss_systick.h"
#include "ss_timer.h"
#include "ss_can.h"

// USR_CONFIGS
#include "systick_handles.h"
#include "systick_isr.h"
#include "pin_declarations.h"

extern Systick_Handle handle1;
extern Systick_Handle handle2;

extern uint16_t pin_heartbeat;
extern uint16_t pin_error;
extern uint16_t pin_blue_one;
extern uint16_t pin_blue_two;

extern uint16_t can1_tx;
extern uint16_t can1_rx;
extern uint16_t can2_tx;
extern uint16_t can2_rx;

extern struct Fifo can_receive_fifos[2];




int main(void) {

  inti_pb_pins();

  uint16_t pa2 = PIN('A', 2);
  gpio_set_mode(pa2, GPIO_MODE_AF);
  gpio_init_pwm(pa2, 159, 2000);

  // 1MBAUD
  CAN_Init(can1_tx, can1_rx, CAN1);
  CAN_Init(can2_tx, can2_rx, CAN2);

  systick_init(16000000/1000);


  gpio_write(pa2, 1000);

  for (;;) {


    if (handle_timer(&handle1)) {

      if(!is_fifo_empty(&can_receive_fifos[0])) {
        struct CanFrame can_frame;
        fifo_remove_can_frame(&can_receive_fifos[0], &can_frame);
        can_send(&can_frame, CAN1);
        gpio_write(pin_blue_one, GPIO_TOGGLE);
      }
    }


  }
  return 0;
}

void DefaultHandler(void) {
    gpio_write(pin_heartbeat, GPIO_TOGGLE);
}


