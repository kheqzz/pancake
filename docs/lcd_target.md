components/espressif__esp_lcd_ili9341/README.md
25:    const spi_bus_config_t bus_config = ILI9341_PANEL_BUS_SPI_CONFIG(EXAMPLE_PIN_NUM_LCD_PCLK, EXAMPLE_PIN_NUM_LCD_MOSI,
27:    ESP_ERROR_CHECK(spi_bus_initialize(EXAMPLE_LCD_HOST, &bus_config, SPI_DMA_CH_AUTO));
31:    const esp_lcd_panel_io_spi_config_t io_config = ILI9341_PANEL_IO_SPI_CONFIG(EXAMPLE_PIN_NUM_LCD_CS, EXAMPLE_PIN_NUM_LCD_DC,
33:    ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)EXAMPLE_LCD_HOST, &io_config, &io_handle));
54:        .reset_gpio_num = EXAMPLE_PIN_NUM_LCD_RST,      // Set to -1 if not use
63:    ESP_ERROR_CHECK(esp_lcd_new_panel_ili9341(io_handle, &panel_config, &panel_handle));

components/espressif__esp_lcd_ili9341/esp_lcd_ili9341.c
48:esp_err_t esp_lcd_new_panel_ili9341(const esp_lcd_panel_io_handle_t io, const esp_lcd_panel_dev_config_t *panel_dev_config, esp_lcd_panel_handle_t *ret_panel)

components/espressif__esp_lcd_ili9341/include/esp_lcd_ili9341.h
58:esp_err_t esp_lcd_new_panel_ili9341(const esp_lcd_panel_io_handle_t io, const esp_lcd_panel_dev_config_t *panel_dev_config, esp_lcd_panel_handle_t *ret_panel);

main/main.c
124:#define LCD_MOSI 24
126:#define LCD_CLK  23
128:#define LCD_DC   3
129:#define LCD_RST  2  
148:#define LCD_HOST SPI2_HOST
1848:    spi_bus_config_t buscfg = {
1849:        .mosi_io_num = LCD_MOSI,
1851:        .sclk_io_num = LCD_CLK,
1857:    ESP_ERROR_CHECK(spi_bus_initialize(LCD_HOST, &buscfg, SPI_DMA_CH_AUTO));
1860:        .dc_gpio_num = LCD_DC,
1869:    ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi(LCD_HOST, &io_config, &lcd_io_handle));
1872:        .reset_gpio_num = LCD_RST,
1877:    ESP_ERROR_CHECK(esp_lcd_new_panel_ili9341(lcd_io_handle, &panel_config, &panel_handle));
3196:    buf1 = spi_bus_dma_memory_alloc(LCD_HOST, buf_size, 0);
3197:    buf2 = spi_bus_dma_memory_alloc(LCD_HOST, buf_size, 0);

components/espressif__esp_lcd_ili9341/test_apps/main/test_esp_lcd_ili9341.c
24:#define TEST_LCD_HOST               SPI2_HOST
32:#define TEST_PIN_NUM_LCD_DC         (GPIO_NUM_4)
34:#define TEST_PIN_NUM_LCD_RST        (GPIO_NUM_48)
36:#define TEST_PIN_NUM_LCD_RST        (GPIO_NUM_1)
94:    const spi_bus_config_t buscfg = ILI9341_PANEL_BUS_SPI_CONFIG(TEST_PIN_NUM_LCD_PCLK, TEST_PIN_NUM_LCD_DATA0,
96:    TEST_ESP_OK(spi_bus_initialize(TEST_LCD_HOST, &buscfg, SPI_DMA_CH_AUTO));
100:    const esp_lcd_panel_io_spi_config_t io_config = ILI9341_PANEL_IO_SPI_CONFIG(TEST_PIN_NUM_LCD_CS, TEST_PIN_NUM_LCD_DC,
103:    TEST_ESP_OK(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)TEST_LCD_HOST, &io_config, &io_handle));
108:        .reset_gpio_num = TEST_PIN_NUM_LCD_RST, // Shared with Touch reset
116:    TEST_ESP_OK(esp_lcd_new_panel_ili9341(io_handle, &panel_config, &panel_handle));
132:    TEST_ESP_OK(spi_bus_free(TEST_LCD_HOST));

## LCD INIT
 uint8_t addr[6];
    int8_t rssi;
    char name[32];
    uint16_t company_id;
    bool is_airtag;
    bool is_smarttag;
} bt_device_info_t;

static bt_device_info_t bt_devices[BT_MAX_DEVICES];
static int bt_device_count = 0;

// ============================================================================

// Pin configuration
#define LCD_MOSI 24
#define LCD_MISO 4
#define LCD_CLK  23
#define LCD_CS   5  // Changed from 6 - GPIO6 is now used for battery ADC
#define LCD_DC   3
#define LCD_RST  2  

// Capacitive touch I2C pins (FT6336U)
#define CTP_SDA  9
#define CTP_SCL  10
#define CTP_INT  25
#define CTP_RST  8

// Backlight control (set to -1 if LED is tied to 3V3 / no GPIO)
#define LCD_BL_IO 26   // -1
#define LCD_BL_ACTIVE_LEVEL 1

#define SCREEN_INACTIVITY_TIMEOUT_MS 30000
#define SCREEN_INACTIVITY_CHECK_MS 1000
#define SCREEN_BACKLIGHT_ACTIVE_PERCENT 80
#define SCREEN_BACKLIGHT_DIM_PERCENT 0

#define LCD_H_RES 480
#define LCD_V_RES 320
#define LCD_HOST SPI2_HOST

// MAX17048 fuel gauge configuration — I2C_NUM_0, SDA=9, SCL=10 (shared with FT6336U touch)
#define MAX17048_ADDR              0x36
#define MAX17048_REG_VCELL         0x02   // Cell voltage (12-bit, 1.25mV/LSB, upper 12 bits)
#define MAX17048_REG_SOC           0x04   // State of charge (upper byte = %, lower byte = 1/256%)
#define BATTERY_UPDATE_INTERVAL_MS 30000  // 30 seconds
