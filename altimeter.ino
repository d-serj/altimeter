#include <Wire.h>
#include <ssd1306.h>
#include <font6x8.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <Bounce2.h>

#define BUTTON1_PIN  5
#define MEASUREMENTS 10
#define DELAY_TO_SCREEN_MS     1000
#define DELAY_TO_MEASURE_MS    100
#define LONGPRESS              3000

void measurements(void);

enum screens
{
	FIRST_SCREEN,
	SECOND_SCREEN,
	THIRD_SCREEN,
	FOUR_SCREEN,
	SLEEP_SCREEN
};

Adafruit_BMP280 bme;
Bounce button1 = Bounce();

const uint8_t Socol[] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x0C, 0x08, 0x18, 0x18, 0x30, 0x70, 0x70, 0xE0, 0xE0, 0xE0,
	0xE0, 0xC0, 0xC0, 0xC0, 0xC0, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0xFC, 0x6C, 0x6C, 0x6C,
	0xD8, 0x80, 0x00, 0xF0, 0xF0, 0x80, 0x80, 0x00, 0x00, 0xF0, 0xF0, 0x00, 0x00, 0xC0, 0xE0, 0x30,
	0x30, 0x20, 0x00, 0x00, 0xC0, 0xE0, 0x30, 0x30, 0x20, 0xE0, 0x00, 0x00, 0x30, 0xF0, 0x30, 0x30,
	0x00, 0xC0, 0xE0, 0x30, 0x30, 0x20, 0xE0, 0x00, 0x00, 0xF0, 0x60, 0xC0, 0x80, 0xC0, 0x60, 0xF0,
	0x00, 0x00, 0xC0, 0xF0, 0x50, 0x50, 0x70, 0x60, 0x00, 0xC0, 0xF0, 0x10, 0x30, 0x30, 0xE0, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
	0x03, 0x03, 0x07, 0x07, 0x0F, 0x0F, 0x0F, 0x1F, 0x3F, 0x3F, 0x7F, 0xFF, 0xFE, 0xFE, 0xFE, 0xFC,
	0xFC, 0xFC, 0xF8, 0xF8, 0xF8, 0xF8, 0xF0, 0xF0, 0xF0, 0xF0, 0xE0, 0xE0, 0xC0, 0xC0, 0x80, 0x80,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03, 0x83, 0x83, 0x03,
	0x01, 0x00, 0x00, 0x03, 0x03, 0x02, 0x02, 0x03, 0x00, 0x03, 0x03, 0x00, 0x00, 0x00, 0x01, 0x03,
	0x03, 0x01, 0x01, 0x00, 0x00, 0x01, 0x03, 0x03, 0x01, 0x01, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00,
	0x00, 0x00, 0x01, 0x03, 0x03, 0x01, 0x01, 0x00, 0x00, 0x03, 0x00, 0x00, 0x01, 0x00, 0x00, 0x03,
	0x00, 0x00, 0x00, 0x03, 0x03, 0x03, 0x01, 0x00, 0x00, 0x0F, 0x0F, 0x01, 0x03, 0x03, 0x01, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x03,
	0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x3F, 0x7F, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFE, 0xFC, 0xF8, 0xF0, 0xE0, 0xC0, 0x80, 0x80, 0x00, 0x1E, 0x2D, 0x61, 0x61, 0x61,
	0x33, 0x00, 0x00, 0x18, 0x3C, 0x66, 0x66, 0x3C, 0x18, 0x00, 0x00, 0x7E, 0x18, 0x18, 0x66, 0x46,
	0x00, 0x18, 0x3C, 0x66, 0x66, 0x24, 0x3C, 0x00, 0x00, 0x20, 0x3E, 0x06, 0x06, 0x7E, 0x7E, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x03, 0x03, 0x07, 0x07,
	0x0F, 0x1F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFC, 0xFC, 0xFC, 0xFC,
	0xFC, 0xFC, 0xFC, 0xF8, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x80, 0x80, 0x80, 0x80, 0xC0, 0xC0, 0xC0, 0xE0, 0xE0, 0xE0, 0xF0, 0xF0, 0xF0, 0xF8, 0xF8,
	0xFC, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x1F, 0x1F,
	0x1F, 0x1F, 0x3F, 0x3F, 0x7F, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFE, 0xFE, 0xFC, 0xFC,
	0xFC, 0xF8, 0xF8, 0xF8, 0xF8, 0xF0, 0xF0, 0xE0, 0xE0, 0xC0, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
	0x03, 0x03, 0x07, 0x07, 0x0F, 0x1F, 0x1F, 0x1F, 0x1F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F,
	0x3F, 0x3F, 0x7F, 0x3F, 0x3F, 0x0F, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x07, 0x0F, 0x0F, 0x0F, 0x1F, 0x1F,
	0x1F, 0x3F, 0x3F, 0x3F, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFC, 0xF8,
	0xF0, 0xE0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x03, 0x03, 0x07, 0x0F, 0x0F, 0x1F, 0x3F,
	0x7F, 0x7F, 0x7F, 0xFF, 0xFF, 0xFE, 0xF8, 0xF0, 0xE0, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x03, 0x07, 0x07, 0x07, 0x0F, 0x1E, 0x1C, 0x38, 0x30,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

float preassureArray[MEASUREMENTS];
float temperatureArray[MEASUREMENTS];
float preassureAverage		  = 0;			
float temperatureAverage	  = 0;
float preassureAverageFinal   = 0;
float temperatureAverageFinal = 0;
float zeroPreassure = 0;
float maxAltitude   = 0;
float altitude      = 0;

void setup()
{
	pinMode(BUTTON1_PIN, INPUT_PULLUP);
	button1.attach(BUTTON1_PIN);
	button1.interval(10); // interval in ms

	ssd1306_setFixedFont(ssd1306xled_font6x8);
	ssd1306_128x64_i2c_init();

	ssd1306_clearScreen();
	ssd1306_drawBitmap(0, 0, 128, 64, Socol);
	delay(100);

	if (!bme.begin()) {
		ssd1306_printFixedN(10, 32, "ERROR BM280", STYLE_NORMAL, FONT_SIZE_2X);
		while (1);
	}
	delay(50);

	// Первое заполнение массива и измерение высоты
	for (uint8_t i = 0; i < MEASUREMENTS; i++)
	{
		preassureArray[i] = bme.readPressure();
		preassureAverage += preassureArray[i];
		delay(50);
	}
	preassureAverageFinal = preassureAverage / MEASUREMENTS;
	preassureAverage = 0;
	zeroPreassure = preassureAverageFinal / 100.0;
	altitude = calcAltitude(zeroPreassure, preassureAverageFinal);

	delay(1200);
	ssd1306_clearScreen();
	printToScreen(FIRST_SCREEN);
}

void loop()
{
	uint32_t currentMillis;
	static uint32_t screenMillis;
	static uint32_t measurementMillis;
	static uint32_t buttonMillis;
	static bool checkButton;
	static bool buttonPress1;
	static bool buttonLongPress1 = true;
	static uint8_t screen;

	currentMillis = millis();

	/*--------------------------------------------------------------------------*/
	// Обработка нажатия кнопки.
	buttonPress1 = button1.update();

	// Обработка долгого нажатия кнопки
	if (!button1.read())
	{
		// Начинаем отсчёт времени нажатия
		if (buttonLongPress1)
		{
			buttonMillis = currentMillis;
			buttonLongPress1 = false;
		}

		// Прошло LONGPRESS время, тогда сбрасываем ZERO точку и MAX высоту в 0
		else if (currentMillis - buttonMillis > LONGPRESS)
		{
			resetAltitude();

			screen = FIRST_SCREEN;
			buttonMillis = currentMillis;
			buttonLongPress1 = true;
		}
	}

	else
		buttonLongPress1 = true;

	// Меняем тип отображения информации на экране
	if (buttonPress1 && button1.rose())
	{
		++screen;

		if (screen > SLEEP_SCREEN)
		{
			screen = FIRST_SCREEN;
			ssd1306_displayOn();
		}

		ssd1306_clearScreen();
		printToScreen(screen);
	}

	/* Если время измерения, то измеряем температуру и давление */
	if (currentMillis - measurementMillis >= DELAY_TO_MEASURE_MS)
	{
		measurements();
		measurementMillis = currentMillis;
	}
	
	/*-------------------------------------------------------------------*/
	//Обновляем экран
	if (currentMillis - screenMillis >= DELAY_TO_SCREEN_MS)
	{
		// IN MM/RH
		//Serial.println(float((preassureAverage)*0.007501), 1);
		printToScreen(screen);
		screenMillis = currentMillis;
	}
}

/*---- Печать информации на экран ----*/
void printToScreen(uint8_t formatting)
{
	const uint8_t bufferSize = 20;
	char buffer[bufferSize];
	char screenBuffer[bufferSize];

	switch (formatting)
	{
	case FIRST_SCREEN:
		// Высота
		dtostrf(altitude, 4, 1, buffer);
		sprintf(screenBuffer, "BbICOTA     [%4.5s M]", buffer);
		//ssd1306_printFixedN(0, 16, "                    ", STYLE_NORMAL, FONT_SIZE_NORMAL);
		ssd1306_printFixedN(0, 16, screenBuffer, STYLE_NORMAL, FONT_SIZE_NORMAL);
		// Очистка буферов
		for (uint8_t i = 0; i < bufferSize; ++i)
		{
			screenBuffer[i] = 0;
			buffer[i] = 0;
		}

		// Максимальная высота
		dtostrf(maxAltitude, 4, 1, buffer);
		sprintf(screenBuffer, "MAX BbICOTA [%4.5s M]", buffer);
		//ssd1306_printFixedN(0, 32, "                    ", STYLE_NORMAL, FONT_SIZE_NORMAL);
		ssd1306_printFixedN(0, 32, screenBuffer, STYLE_NORMAL, FONT_SIZE_NORMAL);

		for (uint8_t i = 0; i < bufferSize; ++i)
		{
			screenBuffer[i] = 0;
			buffer[i] = 0;
		}

		// Температура
		dtostrf(temperatureAverageFinal, 4, 1, buffer);
		sprintf(screenBuffer, "TEMP        [%3.4s C]", buffer);
		//ssd1306_printFixedN(0, 48, "                    ", STYLE_NORMAL, FONT_SIZE_NORMAL);
		ssd1306_printFixedN(0, 48, screenBuffer, STYLE_NORMAL, FONT_SIZE_NORMAL);
		break;

	case SECOND_SCREEN:
		dtostrf(altitude, 4, 1, buffer);
		sprintf(screenBuffer, "%4.5s", buffer);
		ssd1306_printFixedN(10, 14, "BbICOTA", STYLE_NORMAL, FONT_SIZE_2X);
		ssd1306_printFixedN(0, 32, buffer, STYLE_NORMAL, FONT_SIZE_4X);
		break;

	case THIRD_SCREEN:
		dtostrf(maxAltitude, 4, 1, buffer);
		sprintf(screenBuffer, "%4.5s", buffer);
		ssd1306_printFixedN(0, 6, "MAX", STYLE_NORMAL, FONT_SIZE_2X);
		ssd1306_printFixedN(40, 15, "BbICOTA", STYLE_NORMAL, FONT_SIZE_2X);
		delay(1);
		//ssd1306_printFixedN(0, 32, "      ", STYLE_NORMAL, FONT_SIZE_4X);
		ssd1306_printFixedN(0, 32, buffer, STYLE_NORMAL, FONT_SIZE_4X);
		break;

	case FOUR_SCREEN:
		dtostrf(temperatureAverageFinal, 4, 1, buffer);
		sprintf(screenBuffer, "%4.5s", buffer);
		ssd1306_printFixedN(10, 14, "TEM-PA C", STYLE_NORMAL, FONT_SIZE_2X);
		delay(1);
		//ssd1306_printFixedN(0, 32, "      ", STYLE_NORMAL, FONT_SIZE_4X);
		ssd1306_printFixedN(0, 32, buffer, STYLE_NORMAL, FONT_SIZE_4X);
		break;

	case SLEEP_SCREEN:
		ssd1306_printFixedN(6, 16, "Screen sleeping...", STYLE_NORMAL, FONT_SIZE_NORMAL);
		sleepScreen();
		break;
	}
}


/*---- Расчет высоты в метрах в зависимости от текущего давления ----*/
float calcAltitude(float seaLevelhPa, float pressure)
{
	float f_altitude;
	pressure /= 100.0;
	f_altitude = 44330.0 * (1.0 - pow(pressure / seaLevelhPa, 0.190295));
	return f_altitude;
}

/*------------------ Измерения высоты и температуры -------------------------*/
// Измерения производятся MEASUREMENTS раз, после чего усредняются
// Если время измерять очередное показание давления, 
// то измеряем MEASUREMENTS - 1 раз
inline void measurements(void)
{
	static uint8_t countMeasurements;

	preassureArray[countMeasurements] = bme.readPressure();
	temperatureArray[countMeasurements] = bme.readTemperature();
	preassureAverage += preassureArray[countMeasurements];
	temperatureAverage += temperatureArray[countMeasurements];
	//Serial.println(preassureArray[countMeasurements]);			// DEBUG

	countMeasurements++;

	// Если последнее измерение
	if (countMeasurements > MEASUREMENTS - 1)
	{
		preassureAverageFinal = preassureAverage / MEASUREMENTS;
		temperatureAverageFinal = temperatureAverage / MEASUREMENTS;
		// Расчитываем высоту
		altitude = calcAltitude(zeroPreassure, preassureAverageFinal);
		// и максимально набранную высоту
		if (altitude > maxAltitude)
			maxAltitude = altitude;

		// Сбрасываем счетчик, среднее давление и температуру в ноль
		countMeasurements = 0;
		preassureAverage = 0;
		temperatureAverage = 0;
	}
}

/* Сбрасываем высоту, ZERO точку и максимальную высоту в 0 */
void resetAltitude(void)
{
	zeroPreassure = preassureAverageFinal / 100.0;
	maxAltitude = 0;
	altitude = 0;

	// Вывод инфо на экран
	ssd1306_clearScreen();
	ssd1306_printFixedN(10, 6, "BbICOTA", STYLE_NORMAL, FONT_SIZE_2X);
	ssd1306_printFixedN(10, 16, " / MAX", STYLE_NORMAL, FONT_SIZE_2X);
	ssd1306_printFixedN(30, 32, "BbICOTA", STYLE_NORMAL, FONT_SIZE_2X);
	ssd1306_printFixedN(10, 48, " RESET", STYLE_BOLD, FONT_SIZE_2X);
	delay(3000);
	ssd1306_clearScreen();
}

void sleepScreen(void)
{
	const uint32_t screenSleep = 2000;
	const uint32_t screenWork = 1000;

	static uint8_t needOff = 0;
	static uint32_t lastMillis;
	uint32_t nowMillis;

	nowMillis = millis();

	if (nowMillis - lastMillis <= screenWork)
	{
		ssd1306_displayOn();
	}
	
	if ((nowMillis - lastMillis > screenWork) && (nowMillis - lastMillis < screenSleep))
	{
		ssd1306_displayOff();
	}

	else
	{
		lastMillis = millis();
	}

}
