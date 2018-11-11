#if 0
#define GPIO_ON(X)  nrf_drv_gpiote_out_task_force(X, 1)
#define GPIO_OFF(X) nrf_drv_gpiote_out_task_force(X, 0)
#else
#define GPIO_ON(X)  printf("")
#define GPIO_OFF(X) printf("")
#endif
