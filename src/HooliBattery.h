//#include<Arduino.h>
#ifndef HooliBattery_h
#define HooliBattery_h

extern "C" {
typedef void (*callbackFunction)(void);
}
class HooliBattery{
    public:
        enum BatteryType{
            LiIon = 0,
            LiPol = 1,
            LiFePO4 = 2
        };
        
        HooliBattery(int pin,BatteryType type);
        //resS - сопротивление верхнего плеча 
        //resG - сопротивление нижнего плеча
        HooliBattery(int pin,BatteryType type,int resS,int resG);
        
        //resS - сопротивление верхнего плеча 
        //resG - сопротивление нижнего плеча
        //sourseV - напряжение подаваемое на верхнее плечо
        HooliBattery(int pin,BatteryType type,int resS,int resG,float sourseV);

        //resS - сопротивление верхнего плеча 
        //resG - сопротивление нижнего плеча
        //sourseV - напряжение подаваемое на верхнее плечо
        //groundV - напряжение подаваемое на нижнее плечо
        HooliBattery(int pin,BatteryType type,int resS,int resG,float sourseV,float groundV);

        //Минимальное напряжение для АКБ - 100%
        void ChangeMinVoltage(float voltage);
        //Максимальное напряжение для АКБ - 0%
        void ChangeMaxVoltage(float voltage);
        //Текущий заряд батареи в процентах
        int GetCurrentBatteryPower();

        void onDischarged(callbackFunction discharged);
        void onCharged(callbackFunction charged);
        void on30Percent(callbackFunction on30);
        void on20Percent(callbackFunction on20);
        void on10Percent(callbackFunction on10);
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
        float resS = 10000;
        //Сопротивление нижнего резистора
        float resG = 5000;
        //Подсчет заряда батареи в процентах
        int CalculateBatteryPower();
        //События,которые мы отслеживаем
        callbackFunction _onDischarged = NULL;

        callbackFunction _onCharged = NULL;

        callbackFunction _on30 = NULL;

        callbackFunction _on20 = NULL;

        callbackFunction _on10 = NULL;

        int pin;
        void Tick();
        float MinVoltage = 2.5;
        float MaxVoltage = 4.25;
        void SelectMinMaxByType(HooliBattery::BatteryType type);
};
#endif
