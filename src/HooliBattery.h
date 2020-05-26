#ifndef HooliBattery_h
#define HooliBattery_h

#if defined(ESP32)
#include<ESP.h>
#include<FreeRTOS.h>
#elif defined(ARDUINO)
#include <Arduino.h>
#endif

extern "C" {
typedef void (*callbackFunction)(void);
}
class HooliBattery{
    public:
        //Тип батареи
        enum BatteryType{
            LiIon = 0,
            LiPol = 1,
            LiFePO4 = 2
        };
        //Состояния батареи
        enum LifeCycle{
            Charging = 0,
            Discharging = 1
        };

        //pin - Номер пина, к которому подключен резисторный делитель
        //type - Тип батареи
        HooliBattery(int pin,BatteryType type);

        //pin - Номер пина, к которому подключен резисторный делитель
        //type - Тип батареи
        //resS - сопротивление верхнего плеча 
        //resG - сопротивление нижнего плеча
        HooliBattery(int pin,BatteryType type,int resS,int resG);

        //pin - Номер пина, к которому подключен резисторный делитель
        //type - Тип батареи
        //resS - сопротивление верхнего плеча 
        //resG - сопротивление нижнего плеча
        //sourseV - напряжение подаваемое на верхнее плечо
        HooliBattery(int pin,BatteryType type,int resS,int resG,float sourseV);

        //pin - Номер пина, к которому подключен резисторный делитель
        //type - Тип батареи
        //resS - сопротивление верхнего плеча 
        //resG - сопротивление нижнего плеча
        //sourseV - напряжение подаваемое на верхнее плечо
        //groundV - напряжение подаваемое на нижнее плечо
        HooliBattery(int pin,BatteryType type,int resS,int resG,float sourseV,float groundV);

        //Установить минимальное напряжение при котором заряд батареи - 0%
        void ChangeMinVoltage(float voltage);

        //Установить максимальное напряжение при котором заряд батареи - 100%
        void ChangeMaxVoltage(float voltage);

        //[Returns] Возвращяет текущий заряд батареи в процентах
        int GetCurrentBatteryPower();

        //Происходит при заряде батареи меньше 4%
        //discharged - функция выполняющаяся при данном событии
        void onDischarged(callbackFunction discharged);

        //Происходит при 100% заряде батареи
        //charged - функция выполняющаяся при данном событии        
        void onCharged(callbackFunction charged);

        //Происходит при заряде батареи на 30%
        //on30 - функция выполняющаяся при данном событии
        void on30Percent(callbackFunction on30);

        //Происходит при заряде батареи на 20%
        //on20 - функция выполняющаяся при данном событии
        void on20Percent(callbackFunction on20);

        //Происходит при заряде батареи на 10%
        //on10 - функция выполняющаяся при данном событии
        void on10Percent(callbackFunction on10);

        //[Returns] Возвращает текущий процесс (зарядка и разрядка)
        LifeCycle GetLifeCycle();

        #if defined(ARDUINO)
        //Проверка состояния батареи
        void CheckBattery();
        #endif
        
    private:
        float MCUADCVoltage = 3.3;
        int ADCWidthBIT = 12;
        //Полуконсанты минимальных и максимальных напряжений на АКБ
        float LiPolMaxVoltage = 4.2;
        float LiIonMaxVoltage = 4.25;
        float LiFePO4MaxVoltage = 3.65;
        float LiPolMinVoltage = 3.3;
        float LiIonMinVoltage = 2.5;
        float LiFePO4MinVoltage = 2.5;
        //Текущй зарядв процентах
        int batteryPower = 0;
        //Напряжение на подаваемое на нижнее плечо
        float groundV = 0;
        //напряжение подаваемое на верхнее плечо
        float sourseV = 3.3;
        //Сопротивление верхнего резистора
        float resS = 100;//10000
        //Сопротивление нижнего резистора
        float resG = 270;//5000
        //События,которые мы отслеживаем
        callbackFunction _onDischarged = NULL;

        callbackFunction _onCharged = NULL;

        callbackFunction _on30 = NULL;

        callbackFunction _on20 = NULL;

        callbackFunction _on10 = NULL;
        //Проверка текущего заряда
        void _checkBattery();

        //Текущее состояние батареи
        //В начале считаем,что идет разрядка(информация обновиться через 10 измерений)
        LifeCycle batteryLifeCycle = LifeCycle::Discharging;
        #if defined(ESP32)
        void TaskCheckBattery(void *pvParam);
        #elif defined(ARDUINO)
        // Используется для подсчета кол-ва проверок состояния батареи(для обхода необходимости делать задержку) 
        int ticksCounter = 0;
        // Используется для подсчета напряжения в промежутках между оценкой заряда(для обхода необходимости делать задержку) 
        int ticksValue = 0;
        #endif
        int pin;
        float MinVoltage = 2.5;
        float MaxVoltage = 4.25;
        //Устанавливает лимиты напряжений для данного типа аккумулятора
        //type - Тип аккумулятора
        void SelectMinMaxByType(HooliBattery::BatteryType type);

        //Возведение в степень числа
        int Pow(int base,int st);
        
};
#endif
