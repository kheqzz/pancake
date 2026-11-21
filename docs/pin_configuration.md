# Pin Configuration — Pancake (ESP32-C5) — VERIFIED

Status: **terverifikasi di hardware fisik** (layar tampil, touch terdeteksi, boot OK).
Sumber kebenaran pin untuk firmware JANOS (`ESP32C5/main/main.c`).

> Board: **Waveshare ESP32-C5-WIFI6-KIT** (modul `ESP32-C5-WROOM-1`, RISC-V, PSRAM 8 MB).
> Panel: **ST7796 3.5" 320×480**, modul 14 pin yang di-wire manual + touch
> **FT6336U** (I2C `0x38`).
>
> ⚠️ Pinout bawaan doc Waveshare (MOSI 24, CLK 23, DC 3, RST 2, SD_CS 7, BAT_ADC 6)
> **TIDAK dipakai** — wiring nyata di board ini memakai GPIO 8/10/0/1 seperti tabel
> di bawah. Ini sudah diuji fisik (layar tampil, touch terbaca), bukan asumsi.

## Tabel pin (dipakai di `main.c`)

| Fungsi | Makro | GPIO | Status |
|---|---|---|---|
| LCD SPI MOSI | `LCD_MOSI` | **8** | ✅ terverifikasi |
| LCD SPI CLK | `LCD_CLK` | **10** | ✅ terverifikasi |
| LCD CS | `LCD_CS` | **5** | ✅ terverifikasi |
| LCD DC/RS | `LCD_DC` | **0** | ✅ terverifikasi |
| LCD RST | `LCD_RST` | **1** | ✅ terverifikasi |
| LCD MISO | `LCD_MISO` | **25** | dipakai bersama MISO kartu SD (panel sendiri tanpa SDO) |
| Backlight | `LCD_BL_IO` | **26** | ⚠️ backlight menyala, tapi apakah benar digerakkan GPIO26 **belum dipastikan** (di board uji BL kemungkinan di-hardwire ke 3V3) |
| Touch SDA | `CTP_SDA` | **9** | ✅ terverifikasi (chip ID terbaca) |
| Touch SCL | `CTP_SCL` | **4** | ✅ terverifikasi |
| Touch RST | `CTP_RST` | **7** | ✅ terverifikasi |
| Touch INT | `CTP_INT` | **23** | interrupt mode (modul punya pull-up) |
| SD CS | — | **6?** | ❌ belum diketahui, lihat TODO |
| SD SCK/MOSI/MISO | — | ? | ❌ belum diketahui |
| GPS TX / RX | `GPS_TX_PIN` / `GPS_RX_PIN` | **2 / 3** | TX opsional (modul stream NMEA sendiri) |
| NeoPixel / RGB LED | — | 27 | ⚠️ GPIO27 = LED RGB onboard DevKitC-1 |
| Console UART | — | 11 / 12 | ✅ UART0, jangan dipakai lain |
| Baterai | MAX17048 over I2C | `0x36` di bus 9/4 | modul terpisah (rencana); tidak butuh pin ADC — GPIO6 dipakai GPS RX |

## Ringkasan pinout board (Waveshare ESP32-C5-WIFI6-KIT)

| GPIO | Fungsi hardware | Pakai sekarang |
|---|---|---|
| 0 | `LP_GPIO0` / XTAL_32K_P — aman | **LCD_DC** ✅ |
| 1 | `LP_GPIO1` / XTAL_32K_N / ADC1_CH0 — aman | **LCD_RST** ✅ |
| 2 | MTMS / FSPIQ / ADC1_CH1 — ⚠️ **strapping boot** | bebas (hati-hati) |
| 3 | MTDI / ADC1_CH2 — ⚠️ **strapping JTAG/boot** | bebas (hati-hati) |
| 4 | MTCK / FSPIHD / ADC1_CH3 — aman | **CTP_SCL** ✅ |
| 5 | MTDO / FSPIWP / ADC1_CH4 — aman | **LCD_CS** ✅ |
| 6 | FSPICLK / ADC1_CH5 | bebas (jalur native FSPI clock) |
| 7 | FSPID (native MOSI) | **CTP_RST** ✅ |
| 8 | FSPICS1 — aman | **LCD_MOSI** ✅ |
| 9 | FSPICS2 — aman | **CTP_SDA** ✅ |
| 10 | FSPICS0 (native) | **LCD_CLK** ✅ |
| 11 / 12 | U0TXD / U0RXD | console serial — jangan dipakai |
| 13 / 14 | USB_D- / USB_D+ | ❌ jangan dipakai |
| 15 | GPIO15 — doc: aman (MISO/INT) | ⚠️ **hindari** (lihat catatan) |
| 23 | GPIO23 — aman | bebas (MISO pindah ke 25) |
| 24 | GPIO24 — aman | **SD_CS** ✅ |
| 25 | SDIO_CMD — aman | **LCD_MISO + SD_MISO** ✅ |
| 26 | SDIO_CLK — aman | **LCD_BL** ⚠️ PWM belum terbukti |
| 27 | **BOOT button** — ⚠️ strapping | kandidat tombol |
| 28 | ⚠️ **strapping ROM boot/log** | hindari (sekarang `C_BTN` upstream) |

Catatan GPIO15: dokumentasi Waveshare menyebut aman, **tetapi** pengujian fisik di
Marauder dengan `TFT_MISO 15` menghasilkan `Load access fault` + flash app
terkorupsi. Empiris menang → jangan pakai dulu.

Kebijakan: fokus LCD dulu (pin yang sudah works dipakai apa adanya); fitur lain
(SD, tombol, LED, baterai, GPS) menyusul dan memakai pin bebas **24 / 25**.

## Catatan khusus TFT_eSPI (firmware Marauder)

Di TFT_eSPI, `TFT_MISO` **tidak boleh `-1`**: `spi.begin(sck, -1, mosi, -1)` tidak
pernah mengonfigurasi bus → backlight nyala tapi layar hitam tanpa error. Di
ESP-IDF (`esp_lcd`, dipakai JANOS) `miso_io_num = -1` **aman**, karena driver
memakai GPIO matrix dan hanya menginisialisasi sinyal yang dipakai.

Karena itu: JANOS pakai `LCD_MISO -1`, Marauder pakai `TFT_MISO 23` (pin bebas
sebagai dummy). Ini bukan inkonsistensi hardware, hanya perbedaan library.

## TODO

1. **SD card**: paling masuk akal SPI sharing dengan LCD (SCK 10, MOSI 8,
   MISO 23) + CS di **24 atau 25**. Alternatif SDIO memakai `GPIO25` (CMD) dan
   `GPIO26` (CLK) — tapi 26 sekarang `LCD_BL`, jadi harus tukar pin dulu.
2. **Driver panel**: JANOS memakai `esp_lcd_ili9341`, sedangkan panel yang
   terbukti bekerja di Marauder adalah **ST7796**. Kalau JANOS tampil rusak/gelap,
   ganti komponen ke `esp_lcd_st7796` sebelum menyalahkan wiring.
3. **Backlight**: pastikan GPIO26 benar-benar menggerakkan BL (uji `LCD_BL_IO -1`
   vs 26, bandingkan terang layar).
4. **Touch INT**: kalau INT ternyata terpasang, isi `CTP_INT` dengan GPIO yang benar
   (kandidat 25) dan ganti mode polling → interrupt.
5. **GPIO0 (LCD_DC)**: cek apakah GPIO0 termasuk strapping/boot pin di C5. Di Marauder
   sudah terbukti boot normal, tapi kalau nanti muncul masalah masuk download-mode,
   ini tersangka pertama.

## Referensi silang

- Dokumentasi lengkap + riwayat bug: `ESP32Marauder/PANCAKE_HARDWARE.md`
- Kode Marauder: `ESP32Marauder/User_Setup_marauder_pancake.h`, `esp32_marauder/configs.h`
- Kode JANOS: `ESP32C5/main/main.c` (blok "VERIFIED on ESP32-C5-DevKitC-1")

## Catu daya modul (penting)

Panel ST7796 + soket microSD berada di satu modul, **satu jalur VCC/GND**.

| VCC modul | Hasil |
|---|---|
| 3V3 board | kartu SD gagal mount (`0x107`, mati setelah CMD8) — rail turun saat burst arus init |
| **5V board** | **`Mount SUCCESS`**, stabil 20 MHz, 14 940 MB |

Modul punya regulator onboard (AMS1117/662K). Jangan kasih 5 V ke modul yang tidak
punya regulator.
