#include <stdio.h>
#include <stdlib.h>

#include "stm32f7xx_hal.h"
#include "bsp.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "../lvgl/lvgl/lvgl.h"




extern void tft_flush(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const lv_color_t * color_p);
extern bool touchpad_read(lv_indev_data_t *data);

void task_GUI( void *parameter )
{
  lv_disp_drv_t disp_drv;
  lv_indev_drv_t indev_drv;

  lv_init();

  lv_indev_drv_init(&indev_drv);
  indev_drv.read = touchpad_read;
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  lv_indev_drv_register(&indev_drv);
  
  lv_disp_drv_init(&disp_drv);
  disp_drv.disp_flush = tft_flush;
  lv_disp_drv_register(&disp_drv);
  
  lv_theme_t *th = lv_theme_material_init(360,NULL);
  lv_test_theme_1(th);
  for(;;)
  {
    lv_task_handler();
    vTaskDelay(10);
  }
}

