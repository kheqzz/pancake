#!/usr/bin/env bash
# esp_lcd_st7796 1.0.0 is the only release that runs on ESP32-C5, but it was
# written for the old esp_lcd API (`color_space`). ESP-IDF >= 5.3 replaced it
# with `rgb_ele_order`, so the component fails to compile with IDF 6.0:
#   error: 'esp_lcd_panel_dev_config_t' has no member named 'color_space'
# The component manager re-extracts managed_components/ on a dependency change,
# so re-run this script after `idf.py fullclean` / manifest edits.
# Idempotent.
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
F="$ROOT/managed_components/espressif__esp_lcd_st7796/esp_lcd_st7796.c"
[ -f "$F" ] || { echo "missing $F (run idf.py build once to fetch deps)"; exit 1; }
if grep -q "panel_dev_config->color_space" "$F"; then
python3 - "$F" <<'PY'
import sys
p = sys.argv[1]; s = open(p).read()
old = """    switch (panel_dev_config->color_space) {
    case ESP_LCD_COLOR_SPACE_RGB:
        st7796->madctl_val = 0;
        break;
    case ESP_LCD_COLOR_SPACE_BGR:
        st7796->madctl_val |= LCD_CMD_BGR_BIT;
        break;
    default:
        ESP_GOTO_ON_FALSE(false, ESP_ERR_NOT_SUPPORTED, err, TAG, "unsupported color space");
        break;
    }"""
new = """    /* IDF >= 5.3 API: color_space was replaced by rgb_ele_order */
    switch (panel_dev_config->rgb_ele_order) {
    case LCD_RGB_ELEMENT_ORDER_RGB:
        st7796->madctl_val = 0;
        break;
    case LCD_RGB_ELEMENT_ORDER_BGR:
        st7796->madctl_val |= LCD_CMD_BGR_BIT;
        break;
    default:
        ESP_GOTO_ON_FALSE(false, ESP_ERR_NOT_SUPPORTED, err, TAG, "unsupported color space");
        break;
    }"""
assert old in s, "patch context not found"
open(p, 'w').write(s.replace(old, new))
PY
  echo "patched $F"
else
  echo "already patched"
fi
