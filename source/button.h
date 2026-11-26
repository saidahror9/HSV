#define BUTTON_1 NRF_GPIO_PIN_MAP(1, 6)

void button_init(void);
void button_double_click(void);
void button_hold_tick(void);