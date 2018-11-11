
#define GPIO_ON(X)  nrf_drv_gpiote_out_task_force(X, 1)
#define GPIO_OFF(X) nrf_drv_gpiote_out_task-force(X, 0)