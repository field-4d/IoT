// sensor_keys.h

#ifndef SENSOR_KEYS_H
#define SENSOR_KEYS_H

#define NEW_SP_KEYS_SIZE 19

// Array for raw sensor keys
static const char *raw_sensor_keys[NEW_SP_KEYS_SIZE] = {
    "bmp_390_u18_pressure_raw",
    "bmp_390_u18_temperature_raw",
    "bmp_390_u19_pressure_raw",
    "bmp_390_u19_temperature_raw",
    "hdc_2010_u13_temperature_raw",
    "hdc_2010_u13_humidity_raw",
    "hdc_2010_u16_temperature_raw",
    "hdc_2010_u16_humidity_raw",
    "hdc_2010_u17_temperature_raw",
    "hdc_2010_u17_humidity_raw",
    "opt_3001_u1_light_intensity_raw",
    "opt_3001_u2_light_intensity_raw",
    "opt_3001_u3_light_intensity_raw",
    "opt_3001_u4_light_intensity_raw",
    "opt_3001_u5_light_intensity_raw",
    "batmon_temperature_raw",
    "batmon_battery_voltage_raw",
    "package_number",
    "rssi"
};

// Array for decimal sensor keys
static const char *decimal_sensor_keys[NEW_SP_KEYS_SIZE] = {
    "bmp_390_u18_pressure",
    "bmp_390_u18_temperature",
    "bmp_390_u19_pressure",
    "bmp_390_u19_temperature",
    "hdc_2010_u13_temperature",
    "hdc_2010_u13_humidity",
    "hdc_2010_u16_temperature",
    "hdc_2010_u16_humidity",
    "hdc_2010_u17_temperature",
    "hdc_2010_u17_humidity",
    "opt_3001_u1_light_intensity",
    "opt_3001_u2_light_intensity",
    "opt_3001_u3_light_intensity",
    "opt_3001_u4_light_intensity",
    "opt_3001_u5_light_intensity",
    "batmon_temperature",
    "batmon_battery_voltage",
    "package_number",
    "rssi"
};

#endif /* SENSOR_KEYS_H */
