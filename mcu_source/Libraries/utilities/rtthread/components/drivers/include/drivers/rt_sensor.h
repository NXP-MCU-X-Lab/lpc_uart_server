#ifndef __CH_RT_SENSOR_H__
#define __CH_RT_SENSOR_H__

#include <stdint.h>

//#define  RT_SENSOR_CTRL_GET_ID            (0)
//#define  RT_SENSOR_CTRL_GET_INFO          (1)
//#define  RT_SENSOR_CTRL_SET_RANGE         (2)
//#define  RT_SENSOR_CTRL_SET_ODR           (3)
//#define  RT_SENSOR_CTRL_SET_MODE          (4)
//#define  RT_SENSOR_CTRL_SET_POWER         (5)
//#define  RT_SENSOR_CTRL_SELF_TEST         (6)
//#define  RT_SENSOR_CTRL_SET_BW            (8)



#define RT_SENSOR_POS_ACC                  (0)
#define RT_SENSOR_POS_GYR                  (1)
#define RT_SENSOR_POS_MAG                  (2)
#define RT_SENSOR_POS_GYR_TEMPERATURE      (3)
#define RT_SENSOR_POS_ACC_FIFO             (9)
#define RT_SENSOR_POS_GYR_FIFO             (10)
#define RT_SENSOR_POS_MAG_FIFO             (11)
#define RT_SENSOR_POS_PRS                  (14)
#define RT_SENSOR_POS_PRS_TEMPERATURE      (15)
#define RT_SENSOR_POS_ACC_BW              (20)
#define RT_SENSOR_POS_ACC_RG              (21)
#define RT_SENSOR_POS_GYR_BW              (22)
#define RT_SENSOR_POS_GYR_RG              (23)
#define RT_SENSOR_POS_MAG_BW              (24)
#define RT_SENSOR_POS_MAG_RG              (25)
#define RT_SENSOR_POS_ACC_CLEAR_FIFO      (27)
#define RT_SENSOR_POS_GYR_CLEAR_FIFO      (28)
#define RT_SENSOR_POS_ACC_WM_FIFO_INT     (29)
#define RT_SENSOR_POS_GYR_WM_FIFO_INT     (30)
#define RT_SENSOR_POS_CUSTOM              (255)


#endif


