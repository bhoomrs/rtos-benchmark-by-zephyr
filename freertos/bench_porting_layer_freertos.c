#include "bench_api.h"
#include "bench_porting_layer_freertos.h"

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "semphr.h"

/* Freescale includes. */
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "fsl_common.h"
#include "board.h"

#include "pin_mux.h"
#include "clock_config.h"

#define MAX_SEMAPHORES 2
static SemaphoreHandle_t semaphores[MAX_SEMAPHORES];

#define benchmark_task_PRIORITY (configMAX_PRIORITIES - 1)

void bench_test_init(void (*test_init_function)(void *))
{
	/* Init board hardware. */
	BOARD_InitBootPins();
	BOARD_InitBootClocks();
	BOARD_InitDebugConsole();
	if (xTaskCreate(test_init_function, "benchmark", configMINIMAL_STACK_SIZE + 100, NULL, benchmark_task_PRIORITY, NULL) !=
		pdPASS)
	{
		PRINTF("Task creation failed!.\r\n");
		while (1)
			;
	}
	vTaskStartScheduler();
	for (;;)
		;
}

void bench_timing_init(void)
{
	KIN1_InitCycleCounter();
}

void bench_timing_start(void)
{
	KIN1_ResetCycleCounter();
}

void bench_timing_stop(void)
{
	KIN1_DisableCycleCounter();
}

bench_time_t bench_timing_counter_get(void)
{
	bench_time_t cycles = KIN1_GetCycleCounter(); // Copy from volatile
	return cycles;
}

bench_time_t bench_timing_cycles_get(bench_time_t *time_start, bench_time_t *time_end)
{
	return (*time_end - *time_start);
}

bench_time_t bench_timing_cycles_to_ns(bench_time_t cycles)
{
	// unsigned int timing_freq = 
	// unsigned int timing_freq_mhz = (unsigned int)(timing_freq / 1000000);

	// return (uint32_t)(arch_timing_freq_get() / 1000000);
	// return (cycles) * (NSEC_PER_USEC) / arch_timing_freq_get_mhz();
	return 0;
}

int bench_sem_create(int sem_id, int initial_count, int maximum_count)
{
	semaphores[sem_id] = xSemaphoreCreateCounting(maximum_count, initial_count);
	return BENCH_SUCCESS;
}

void bench_sem_give(int sem_id)
{
	xSemaphoreGive(semaphores[sem_id]);
	return;
}

int bench_sem_take(int sem_id)
{
	xSemaphoreTake(semaphores[sem_id], portMAX_DELAY);
	return BENCH_SUCCESS;
}

void *bench_malloc(size_t size)
{
	return pvPortMalloc(size);
}

void bench_free(void *ptr)
{
	vPortFree(ptr);
}

void bench_thread_start(int thread_id)
{
	ARG_UNUSED(thread_id);
}

void bench_thread_set_priority(int priority)
{
	ARG_UNUSED(priority);
}

int bench_thread_create(int thread_id, const char *thread_name, int priority,
	void (*entry_function)(void *), void *args)
{
	ARG_UNUSED(thread_id);
	ARG_UNUSED(thread_name);
	ARG_UNUSED(priority);
	ARG_UNUSED(entry_function);
	ARG_UNUSED(args);
}

void bench_thread_resume(int thread_id)
{
	ARG_UNUSED(thread_id);
}

void bench_thread_suspend(int thread_id)
{
	ARG_UNUSED(thread_id);
}

void bench_thread_abort(int thread_id)
{
	ARG_UNUSED(thread_id);
}

void bench_yield(void)
{
}

int bench_mutex_create(int mutex_id)
{
	ARG_UNUSED(mutex_id);
}

int bench_mutex_lock(int mutex_id)
{
	ARG_UNUSED(mutex_id);
}

int bench_mutex_unlock(int mutex_id)
{
	ARG_UNUSED(mutex_id);
}

void bench_irq_offload(const void *irq_offload_routine, const void *parameter)
{
	ARG_UNUSED(irq_offload_routine);
	ARG_UNUSED(parameter);
}

void bench_sync_ticks(void)
{
}
