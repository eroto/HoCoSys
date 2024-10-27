#ifndef __APPTASK_H__
#define __APPTASK_H__

#define TASK_STACK_SIZE		4096
//#define tskIDLE_PRIORITY	2u
#define CORE0				0
#define CORE1				1

//extern SemaphoreHandle_t print_mux;

StaticTask_t xTask5Buffer;
StaticTask_t xTask10Buffer;
StaticTask_t xTask20Buffer;
StaticTask_t xTask50Buffer;
StaticTask_t xTask100Buffer;
StaticTask_t xTask500Buffer;

StackType_t xStack5msTask[TASK_STACK_SIZE];
StackType_t xStack10msTask[TASK_STACK_SIZE];
StackType_t xStack20msTask[TASK_STACK_SIZE];
StackType_t xStack50msTask[TASK_STACK_SIZE];
StackType_t xStack100msTask[TASK_STACK_SIZE];
StackType_t xStack500msTask[TASK_STACK_SIZE];

TaskHandle_t TaskHandle5ms = NULL;
TaskHandle_t TaskHandle10ms = NULL;
TaskHandle_t TaskHandle20ms = NULL;
TaskHandle_t TaskHandle50ms = NULL;
TaskHandle_t TaskHandle100ms = NULL;
TaskHandle_t TaskHandle500ms = NULL;
TaskHandle_t *const TaskHandleConsole = NULL;


#define TASK_PERIOD_5		5
#define TASK_PERIOD_10		10
#define TASK_PERIOD_20		20
#define TASK_PERIOD_50		50
#define TASK_PERIOD_100		100
#define TASK_PERIOD_500		500


void apptask_5ms(void *pvParameters );
void apptask_10ms(void *pvParameters );
void apptask_20ms(void *pvParameters );
void apptask_50ms(void *pvParameters );
void apptask_100ms(void *pvParameters );
void apptask_500ms(void *pvParameters );
//void ConsoleTask(void *pvParameters);

#endif /*__APPTASK_H__*/
