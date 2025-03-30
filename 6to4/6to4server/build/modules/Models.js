"use strict";
var __importDefault = (this && this.__importDefault) || function (mod) {
    return (mod && mod.__esModule) ? mod : { "default": mod };
};
Object.defineProperty(exports, "__esModule", { value: true });
exports.writeOptions = exports.Package = exports.PackageToSend = exports.BlackBox = exports.Conf = void 0;
/* eslint-disable @typescript-eslint/camelcase */
var influxdb_client_1 = require("@influxdata/influxdb-client");
var dotenv_1 = __importDefault(require("dotenv"));
// Load the environment variables from a custom .env file
dotenv_1.default.config({ path: '/home/pi/6to4/.env' });
// Get the InfluxDB credentials from the environment variables
var influxDBCredentials = {
    token: process.env.LOCAL_TOKEN,
    org: process.env.LOCAL_ORG,
    bucket: process.env.BUCKET_NAME,
    url: process.env.LOCAL_URL,
};
exports.Conf = {
    /*InfluxDB credentials */
    BlackBox: {
        token: influxDBCredentials.token || '',
        org: influxDBCredentials.org || '',
        bucket: influxDBCredentials.bucket || '',
        url: influxDBCredentials.url || '',
    },
    hujiCloud: {
        token: '',
        org: '',
        bucket: '',
        url: '',
    },
    Device_Owner: {
        Owner: process.env.Device_Owner,
    }
};
exports.BlackBox = new influxdb_client_1.InfluxDB({
    url: exports.Conf.BlackBox.url,
    token: exports.Conf.BlackBox.token,
});
// export const Cloud = new InfluxDB({
//   url: Conf.hujiCloud.url,
//   token: Conf.hujiCloud.token,
// });
var PackageToSend = function (pkg, timeStamp, bufferNumber, exp_name) {
    var readyPoint = new influxdb_client_1.Point(pkg.ipv6)
        .tag('Exp_Name', exp_name)
        .floatField('buffer_number', bufferNumber)
        .timestamp(timeStamp);
    if (pkg.battery !== undefined) {
        readyPoint.floatField('battery', pkg.battery);
    }
    else {
        console.log("Warning: battery is undefined");
    }
    if (pkg.rssi !== undefined) {
        readyPoint.intField('rssi', pkg.rssi);
    }
    // Handle the first package type with light, battery_t, bmp, hdc, tmp107 fields
    if (pkg.light !== undefined) {
        console.log("Processing normal package with light");
        readyPoint
            .floatField('light', pkg.light)
            .floatField('battery_t', pkg.battery_t || 0) // Handle undefined battery_t
            .floatField('bmp_press', pkg.bmp_press || 0) // Handle undefined bmp_press
            .floatField('bmp_temp', pkg.bmp_temp || 0) // Handle undefined bmp_temp
            .floatField('hdc_temp', pkg.hdc_temp || 0) // Handle undefined hdc_temp
            .floatField('hdc_humidity', pkg.hdc_humidity || 0) // Handle undefined hdc_humidity
            .floatField('tmp107_amb', pkg.tmp107_amb || 0) // Handle undefined tmp107_amb
            .floatField('tmp107_obj', pkg.tmp107_obj || 0); // Handle undefined tmp107_obj
    }
    // Handle the second package type with opt_3001, batmon, and multiple hdc fields
    else if (pkg.bmp_390_u18_pressure !== undefined) {
        console.log("Processing new package without light");
        readyPoint
            .floatField('bmp_390_u18_pressure', pkg.bmp_390_u18_pressure || 0)
            .floatField('bmp_390_u18_temperature', pkg.bmp_390_u18_temperature || 0)
            .floatField('bmp_390_u19_pressure', pkg.bmp_390_u19_pressure || 0)
            .floatField('bmp_390_u19_temperature', pkg.bmp_390_u19_temperature || 0)
            .floatField('hdc_2010_u13_temperature', pkg.hdc_2010_u13_temperature || 0)
            .floatField('hdc_2010_u13_humidity', pkg.hdc_2010_u13_humidity || 0)
            .floatField('hdc_2010_u16_temperature', pkg.hdc_2010_u16_temperature || 0)
            .floatField('hdc_2010_u16_humidity', pkg.hdc_2010_u16_humidity || 0)
            .floatField('hdc_2010_u17_temperature', pkg.hdc_2010_u17_temperature || 0)
            .floatField('hdc_2010_u17_humidity', pkg.hdc_2010_u17_humidity || 0)
            .floatField('opt_3001_u1_light_intensity', pkg.opt_3001_u1_light_intensity || 0)
            .floatField('opt_3001_u2_light_intensity', pkg.opt_3001_u2_light_intensity || 0)
            .floatField('opt_3001_u3_light_intensity', pkg.opt_3001_u3_light_intensity || 0)
            .floatField('opt_3001_u4_light_intensity', pkg.opt_3001_u4_light_intensity || 0)
            .floatField('opt_3001_u5_light_intensity', pkg.opt_3001_u5_light_intensity || 0)
            .floatField('batmon_temperature', pkg.batmon_temperature || 0)
            .floatField('batmon_battery_voltage', pkg.batmon_battery_voltage || 0);
    }
    // Handle the CO2 and Air Velocity package type
    else if (pkg.co2_ppm !== undefined && pkg.air_velocity !== undefined) {
        console.log("Processing CO2 and Air Velocity package");
        readyPoint
            .floatField('co2_ppm', pkg.co2_ppm)
            .floatField('air_velocity', pkg.air_velocity);
    }
    else {
        console.log("Unknown package type");
        throw new Error('Unknown package structure');
    }
    return {
        Ready: readyPoint,
    };
};
exports.PackageToSend = PackageToSend;
var Package = function (obj) {
    var Package;
    // Handle the first JSON structure with light
    if (obj.light !== undefined) {
        Package = {
            ipv6: obj.ipv6,
            packet_number: obj.packet_number,
            light: obj.light,
            battery_t: obj.battery_t,
            battery: obj.battery,
            bmp_press: obj.bmp_press,
            bmp_temp: obj.bmp_temp,
            hdc_temp: obj.hdc_temp,
            hdc_humidity: obj.hdc_humidity,
            tmp107_amb: obj.tmp107_amb,
            tmp107_obj: obj.tmp107_obj,
            rssi: obj.rssi,
        };
    }
    // Handle the second JSON structure without light
    else if (obj.bmp_390_u18_pressure !== undefined) {
        Package = {
            ipv6: obj.ipv6,
            packet_number: obj.package_number,
            battery: obj.batmon_battery_voltage,
            rssi: obj.rssi,
            bmp_390_u18_pressure: obj.bmp_390_u18_pressure,
            bmp_390_u18_temperature: obj.bmp_390_u18_temperature,
            bmp_390_u19_pressure: obj.bmp_390_u19_pressure,
            bmp_390_u19_temperature: obj.bmp_390_u19_temperature,
            hdc_2010_u13_temperature: obj.hdc_2010_u13_temperature,
            hdc_2010_u13_humidity: obj.hdc_2010_u13_humidity,
            hdc_2010_u16_temperature: obj.hdc_2010_u16_temperature,
            hdc_2010_u16_humidity: obj.hdc_2010_u16_humidity,
            hdc_2010_u17_temperature: obj.hdc_2010_u17_temperature,
            hdc_2010_u17_humidity: obj.hdc_2010_u17_humidity,
            opt_3001_u1_light_intensity: obj.opt_3001_u1_light_intensity,
            opt_3001_u2_light_intensity: obj.opt_3001_u2_light_intensity,
            opt_3001_u3_light_intensity: obj.opt_3001_u3_light_intensity,
            opt_3001_u4_light_intensity: obj.opt_3001_u4_light_intensity,
            opt_3001_u5_light_intensity: obj.opt_3001_u5_light_intensity,
            batmon_temperature: obj.batmon_temperature,
            batmon_battery_voltage: obj.batmon_battery_voltage,
        };
    }
    // Handle the CO2 and Air Velocity package type
    else if (obj.co2_ppm !== undefined && obj.air_velocity !== undefined) {
        Package = {
            ipv6: obj.ipv6,
            packet_number: obj.packet_number,
            co2_ppm: obj.co2_ppm,
            air_velocity: obj.air_velocity,
            battery: obj.battery,
            rssi: obj.rssi,
        };
    }
    else {
        throw new Error('Unknown package structure');
    }
    return {
        DB: Package,
        ADDR: String(obj.ipv6),
        NUM: parseFloat(obj.packet_number || obj.package_number || 0),
        TIME: new Date(),
    };
};
exports.Package = Package;
// export const PackageToSend =  (
//   pkg: PackageObj,
//   timeStamp: number,
//   bufferNumber: number,
//   exp_name: string
// ) => {
//   return {
//     Ready: new Point(pkg.ipv6)
//       .tag('Exp_Name', exp_name)
//       .floatField('buffer_number', bufferNumber)
//       .floatField('light', pkg.light)
//       .floatField('battery_t', pkg.battery_t)
//       .floatField('battery', pkg.battery)
//       .floatField('bmp_press', pkg.bmp_press)
//       .floatField('bmp_temp', pkg.bmp_temp)
//       .floatField('hdc_temp', pkg.hdc_temp)
//       .floatField('hdc_humidity', pkg.hdc_humidity)
//       .floatField('tmp107_amb', pkg.tmp107_amb)
//       .floatField('tmp107_obj', pkg.tmp107_obj)
//       .intField('rssi', pkg.rssi)
//       .timestamp(timeStamp),
//   };
// };
// export const Package = (obj: any) => {
//   const Package: PackageObj = {
//     ipv6: obj.ipv6,
//     packet_number: obj.packet_number,
//     light: obj.light,
//     battery_t: obj.battery_t,
//     battery: obj.battery,
//     bmp_press: obj.bmp_press,
//     bmp_temp: obj.bmp_temp,
//     hdc_temp: obj.hdc_temp,
//     hdc_humidity: obj.hdc_humidity,
//     tmp107_amb: obj.tmp107_amb,
//     tmp107_obj: obj.tmp107_obj,
//     rssi: obj.rssi,
//   };
//   return {
//     DB: Package,
//     ADDR: String(obj.ipv6),
//     NUM: parseFloat(obj.packet_number),
//     TIME: new Date(),
//   };
// };
exports.writeOptions = {
    /* the maximum points/line to send in a single batch to InfluxDB server */
    batchSize: 1001,
    /* default tags to add to every point */
    defaultTags: { GW: 'RPI4' },
    /* maximum size of the retry buffer - it contains items that could not be sent for the first time */
    maxBufferLines: 2958500,
    /* the count of retries, the delays between retries follow an exponential backoff strategy if there is no Retry-After HTTP header */
    maxRetries: 20,
    /* maximum delay between retries in milliseconds */
    maxRetryDelay: 3600000,
    /* minimum delay between retries in milliseconds */
    minRetryDelay: 60000,
    /* a random value of up to retryJitter is added when scheduling next retry */
    retryJitter: 10000,
    // ... or you can customize what to do on write failures when using a writeFailed fn, see the API docs for details
    writeFailed: function (error, lines, failedAttempts) {
        console.error(error);
        // if (failedAttempts > 5) {
        //   try {
        //     let array = fs.readFileSync('data.txt').toString().split("\n");
        //     const reWriteApi = client.getWriteApi(org, bucket, 's', writeOptions)
        //     reWriteApi.writeRecords(array);
        //   } catch (e) {
        //     console.error(e)
        //     // console.log('\nFinished ERROR')
        //   }
        // }
    },
    writeSuccess: function (lines) {
        console.log(lines);
    },
};
