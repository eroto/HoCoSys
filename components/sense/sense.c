#include <stdio.h>
#include "esp_log.h"
#include "driver/temperature_sensor.h"
#include "driver/gpio.h"
#include "soc/soc_caps.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"
#include "sdkconfig.h"
#include "sense.h"
#include "sense_if.h"


/*---------------------------------------------------------------
        ADC General Macros
---------------------------------------------------------------*/
//ADC1 Channels
#if CONFIG_IDF_TARGET_ESP32S3
#define EXAMPLE_ADC1_CHAN0          ADC_CHANNEL_4
#define EXAMPLE_ADC1_CHAN1          ADC_CHANNEL_5
#else
#define EXAMPLE_ADC1_CHAN0          ADC_CHANNEL_2
#define EXAMPLE_ADC1_CHAN1          ADC_CHANNEL_3
#endif

#define ADC_ATTENUATION           ADC_ATTEN_DB_11

adc_oneshot_unit_init_cfg_t ADC_OneShotCfg =
{
		.unit_id = ADC_UNIT_1,
		.ulp_mode =  ADC_ULP_MODE_FSM
};

//-------------ADC1 Config---------------//
adc_oneshot_chan_cfg_t config =
{
		.bitwidth = ADC_BITWIDTH_DEFAULT,
        .atten = ADC_ATTENUATION,
};

//-------------ADC1 Calibration Init---------------//
adc_cali_handle_t adc1_cali_handle = NULL;

bool do_calibration1 = 0;

adc_channel_t ADC1_Channels[MAX_ADC_SENSORS] =
{
	    ADC_CHANNEL_0,
	    ADC_CHANNEL_1,
	    ADC_CHANNEL_2,
	    ADC_CHANNEL_3,
	    ADC_CHANNEL_4,
	    ADC_CHANNEL_5,
};

adc_oneshot_unit_handle_t ADC_Handler;

/* ADC variable to store readings 2 ADC 10 channels each*/
int adc_raw [2][10];

static const char *TAG = "Sense";
static temperature_sensor_handle_t IntTempSensor = NULL;
static temperature_sensor_config_t temp_sensor_config = TEMPERATURE_SENSOR_CONFIG_DEFAULT(10, 50);
//static float tsens_value;

//zero-initialize the config structure.
gpio_config_t io_conf = {};

uint8_t sense_read_AI(sensorADC_t sensor, int *out)
{
	uint8_t ret = 1;

	if (sensor < MAX_ADC_SENSORS)
	{
		ESP_ERROR_CHECK(adc_oneshot_read(ADC_Handler, ADC1_Channels[sensor], &adc_raw[0][sensor]));
		*out = adc_raw[0][sensor];
		ret = 0;
	}
	else
	{
		*out = 0;
		ret = 1;
	}

return ret;
}

esp_err_t sense_init(void)
{
	sense_init_IntTempSensor(&IntTempSensor);
	sense_init_AIs();
	sense_init_DIs();

return ESP_OK;
}

void sense_init_IntTempSensor(temperature_sensor_handle_t* temp_sensor)
{
    ESP_LOGI(TAG, "Install temperature sensor, expected temp ranger range: 10~50 ℃");
    ESP_ERROR_CHECK(temperature_sensor_install(&temp_sensor_config, temp_sensor));
    ESP_LOGI(TAG, "Enable temperature sensor");
    ESP_ERROR_CHECK(temperature_sensor_enable(*temp_sensor));
}

void sense_init_AIs(void)
{
	uint8_t i =0;
	ESP_LOGI(TAG, "Init. Analog sensors");
	ESP_ERROR_CHECK(adc_oneshot_new_unit(&ADC_OneShotCfg, &ADC_Handler));

	for(i = 0; i < MAX_ADC_SENSORS; i++)
	{
		ESP_ERROR_CHECK(adc_oneshot_config_channel(ADC_Handler, ADC1_Channels[i], &config));
	}

	// --------------------- ADC1 Calibration ---------------------//
	do_calibration1 = example_adc_calibration_init(ADC_UNIT_1, ADC_ATTENUATION, &adc1_cali_handle);

}

void sense_init_DIs(void)
{
	/*Configure Outputs*/
	ESP_LOGI(TAG, "Init. Digital sensors");
    //disable interrupt
    io_conf.intr_type = GPIO_INTR_DISABLE;
    //set as output mode
    io_conf.mode = GPIO_MODE_OUTPUT;

#if CONFIG_OutputsConfig == 1
    //bit mask of the pins that you want to set,e.g.GPIO18/19
    io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
#endif

    //disable pull-down mode
    io_conf.pull_down_en = 0;
    //disable pull-up mode
    io_conf.pull_up_en = 0;
    //configure GPIO with the given settings
    gpio_config(&io_conf);

    /*Configure Inputs*/
    //interrupt of rising edge
	io_conf.intr_type = GPIO_INTR_POSEDGE;
#if CONFIG_InputsConfig == 1
	//bit mask of the pins, use GPIO4/5 here
	io_conf.pin_bit_mask = GPIO_INPUT_PIN_SEL;
#endif
	//set as input mode
	io_conf.mode = GPIO_MODE_INPUT;
	//enable pull-up mode
	io_conf.pull_up_en = 1;
	gpio_config(&io_conf);
}

float sense_getIntTempCelcius(void)
{
	ESP_LOGI(TAG, "Read temperature");
	float tsens_value = 0;
	ESP_ERROR_CHECK(temperature_sensor_get_celsius(IntTempSensor, &tsens_value));

	return tsens_value;
}

/*---------------------------------------------------------------
        ADC Calibration
---------------------------------------------------------------*/
static bool example_adc_calibration_init(adc_unit_t unit, adc_atten_t atten, adc_cali_handle_t *out_handle)
{
    adc_cali_handle_t handle = NULL;
    esp_err_t ret = ESP_FAIL;
    bool calibrated = false;

#if ADC_CALI_SCHEME_CURVE_FITTING_SUPPORTED
    if (!calibrated) {
        ESP_LOGI(TAG, "calibration scheme version is %s", "Curve Fitting");
        adc_cali_curve_fitting_config_t cali_config = {
            .unit_id = unit,
            .atten = atten,
            .bitwidth = ADC_BITWIDTH_DEFAULT,
        };
        ret = adc_cali_create_scheme_curve_fitting(&cali_config, &handle);
        if (ret == ESP_OK) {
            calibrated = true;
        }
    }
#endif

#if ADC_CALI_SCHEME_LINE_FITTING_SUPPORTED
    if (!calibrated) {
        ESP_LOGI(TAG, "calibration scheme version is %s", "Line Fitting");
        adc_cali_line_fitting_config_t cali_config = {
            .unit_id = unit,
            .atten = atten,
            .bitwidth = ADC_BITWIDTH_DEFAULT,
        };
        ret = adc_cali_create_scheme_line_fitting(&cali_config, &handle);
        if (ret == ESP_OK) {
            calibrated = true;
        }
    }
#endif

    *out_handle = handle;
    if (ret == ESP_OK) {
        ESP_LOGI(TAG, "Calibration Success");
    } else if (ret == ESP_ERR_NOT_SUPPORTED || !calibrated) {
        ESP_LOGW(TAG, "eFuse not burnt, skip software calibration");
    } else {
        ESP_LOGE(TAG, "Invalid arg or no memory");
    }

    return calibrated;
}
