#ifndef SENSOR_KEYS_H
#define SENSOR_KEYS_H

#define IMF4D_BASE_SENSORS_COUNT 19

#define IMF4D_ADVANCED_SENSORS_COUNT 2

#define OLD_SP_KEYS_SIZE 11  

static const char *old_sensor_keys[OLD_SP_KEYS_SIZE] = {
    "light",
    "battery_t",
    "battery",
    "bmp_press",
    "bmp_temp",
    "hdc_temp",
    "hdc_humidity",
    "tmp107_amb",
    "tmp107_obj",
    "packet_number",
    "rssi"
};

// Array for raw sensor keys
static const char *raw_sensor_keys[IMF4D_BASE_SENSORS_COUNT] = {
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


static const char *raw_adv_sensor_keys[IMF4D_ADVANCED_SENSORS_COUNT] = {
    "co2_ppm_raw",
     "air_velocity_raw"


};

// Array for decimal sensor keys
static const char *decimal_sensor_keys[IMF4D_BASE_SENSORS_COUNT] = {
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


static const char *decimal_adv_sensor_keys[IMF4D_ADVANCED_SENSORS_COUNT] = {
 "co2_ppm",
    "air_velocity"

};

    // Flags for sensors that should be displayed as floating-point
static const bool *is_float[OLD_SP_KEYS_SIZE] = {
        true,  // "light"
        false, // "battery_t"
        false, // "battery"
        true,  // "bmp_press"
        true,  // "bmp_temp"
        true,  // "hdc_temp"
        true,  // "hdc_humidity"
        true,  // "tmp107_amb"
        true,  // "tmp107_obj"
        false, // "packet_number"
        false  // "rssi"
    };


#endif /* SENSOR_KEYS_H */
