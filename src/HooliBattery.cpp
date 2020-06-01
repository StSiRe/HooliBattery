#include<HooliBattery.h>
HooliBattery::HooliBattery(int pin,HooliBattery::BatteryType type)
{
    HooliBattery::pin = pin;
    SelectMinMaxByType(type);
    pinMode(pin,INPUT);
    #if defined(ESP32)
    xTaskCreate(this->TaskCheckBattery,"HooliBattery",4096,NULL,1,NULL);
    HooliBattery::ADCWidthBIT = 12;
    HooliBattery::MCUADCVoltage = 3.3;
    #elif defined(__SAM3X8E__)//Arduino Due
    HooliBattery::ADCWidthBIT = 12;
    HooliBattery::MCUADCVoltage = 3.3;
    #elif defined(__AVR__)//All Arduino Uno/nano/mega/etc
    HooliBattery::ADCWidthBIT = 10;
    HooliBattery::MCUADCVoltage = 5.0;
    #endif
}

HooliBattery::HooliBattery(int pin,HooliBattery::BatteryType type,int resS,int resG)
{
    HooliBattery(pin,type);
    HooliBattery::resS = resS;
    HooliBattery::resG = resG;
}


#if defined(ESP32)
void HooliBattery::TaskCheckBattery(void *pvParam)
{
    while(true)
    {
        //Проверка состояния батареи каждые 5 секунд
        HooliBattery::_checkBattery();
        vTaskDelay(5000/portTICK_PERIOD_MS);
    }
    vTaskDelay(NULL);
}
#else 
void HooliBattery::CheckBattery()
{
    HooliBattery::_checkBattery();
}
#endif

void HooliBattery::SelectMinMaxByType(HooliBattery::BatteryType type)
{
    if(type == HooliBattery::LiPol)
    {
        HooliBattery::MinVoltage = HooliBattery::LiPolMinVoltage;
        HooliBattery::MaxVoltage = HooliBattery::LiPolMaxVoltage;
    }
    else if(type == HooliBattery::LiIon)
    {
        HooliBattery::MinVoltage = HooliBattery::LiIonMinVoltage;
        HooliBattery::MaxVoltage = HooliBattery::LiIonMaxVoltage;
    }
    else
    {
        HooliBattery::MinVoltage = HooliBattery::LiFePO4MinVoltage;
        HooliBattery::MaxVoltage = HooliBattery::LiFePO4MaxVoltage;
    }    
}
void HooliBattery::SetMaxVoltage(float voltage)
{
    HooliBattery::MinVoltage = voltage;
}
void HooliBattery::SetMinVoltage(float voltage)
{
    HooliBattery::MaxVoltage = voltage;
}
void HooliBattery::onDischarged(callbackFunction disharged)
{
    HooliBattery::_onDischarged = disharged;
}
void HooliBattery::onCharged(callbackFunction charged)
{
    HooliBattery::_onCharged = charged;
}
void HooliBattery::on30Percent(callbackFunction on30)
{
    HooliBattery::_on30 = on30;
}
void HooliBattery::on20Percent(callbackFunction on20)
{
    HooliBattery::_on20 = on20;
}
void HooliBattery::on10Percent(callbackFunction on10)
{
    HooliBattery::_on10 = on10;
}
// BattIn ---
//  |        |
//  |       [ ] ResS
//  |        | 
//  |        |-------To Pin
//  U        |           |
//  |       [ ] ResG     |
//  |        |           U2
//  |        |           |
// Ground ---------------

void HooliBattery::_checkBattery()
{

    //#if defined(ESP32)
    //int res = 0;
    //for(int i = 0;i< 5;i++)
    //{
    //    res += analogRead(HooliBattery::pin);
    //    vTaskDelay(500/portTICK_PERIOD_MS);
    //}
    //int value = res / 5;//Вычисляем средне-арифмитическое
    //#elif defined(ARDUINO)
    //if(ticksCounter < 5)
    //{
    //    ticksValue += analogRead(HooliBattery::pin);
    //    ticksCounter++;
    //    return;
    //}
    //ticksCounter = 0;
    //int value = ticksValue / 5;
    //ticksValue = 0;
    //#endif
    int value = analogRead(HooliBattery::pin);
    //3.3 - 4096
    // x  - value
    // Преобразовать считанное значение в напряжение
    float voltagePin = MCUADCVoltage*float(value) / float(Pow(2,ADCWidthBIT));

    // Просчитать напряжение на входе
    float battV = voltagePin * (resS+resG) / resG;
    
    //U = U2*(R1+R2)/R2
    //Считаем процент заряда
    //float battP = 100 - (-MaxVoltage + battV) / MaxVoltage * 100;
    float battP = (battV - MinVoltage) / (MaxVoltage - MinVoltage) * 100;
    batteryPower = battP;
    //-----------Отностительно результатов вызываем события
    if(batteryPower >= 96)
    {
        if(_onCharged)  _onCharged();
    }
    else if(batteryPower <= 4)
    {
        if(_onDischarged)   _onDischarged();
    }
    else
    {
        if(batteryPower == 30)
            if(_on30)   _on30();
        else if(batteryPower == 20)
            if(_on20)   _on20();
        else if(batteryPower == 10)
            if(_on10)   _on10();
    }
    // It`s all:)
}
int HooliBattery::Pow(int base,int st)
{
    int res = 1;
    for (size_t i = 0; i < st; i++)
    {
        res = res*base;
    }
    return res;
}
int HooliBattery::GetCurrentBatteryPower()
{
    return batteryPower;
}


//Анализ данных напряжений:
//Варианты систем слежения
//Смотрим на изменение заряда(по 20 измеренияем(доп память 64 байт под массив))

HooliBattery::LifeCycle HooliBattery::GetLifeCycle()
{
    return getPowerStatus();
}
void HooliBattery::addPowerValue(int value)
{
    for (size_t i = 0; i < powerValueSize - 1; i++) 
    {
        PowerValues[i] = PowerValues[i+1];
    }
    PowerValues[powerValueSize - 1] = value;
}
HooliBattery::LifeCycle HooliBattery::getPowerStatus()
{
    int sum = 0;    
    for (size_t i = 0; i < powerValueSize; i++)
    {
        sum += PowerValues[i];
    }
    float average = (float)sum / powerValueSize;
    if(average > PowerValues[0])
        return LifeCycle::Charging;
    else
        return LifeCycle::Discharging;
}