#include<HooliBattery.h>
HooliBattery::HooliBattery(int pin,HooliBattery::BatteryType type)
{
    HooliBattery::pin = pin;
    pinMode(pin,INPUT);
    #if defined(ESP32)
    xTaskCreate(this->TaskCheckBattery,"HooliBattery",4096,NULL,1,NULL);
    #endif
}
#if defined(ESP32)
void HooliBattery::TaskCheckBattery(void *pvParam)
{
    while(true)
    {
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
void HooliBattery::ChangeMaxVoltage(float voltage)
{
    HooliBattery::MinVoltage = voltage;
}
void HooliBattery::ChangeMinVoltage(float voltage)
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
    int value = analogRead(HooliBattery::pin);
    //3.3 - 4096
    // x  - value
    // Преобразовать считанное значение в напряжение
    float voltagePin = MCUADCVoltage*float(value) / Pow(2,ADCWidthBIT);
    // Просчитать напряжение на входе
    float battV = voltagePin * (resS+resG) / resG;
    //U = U2*(R1+R2)/R2
    //Считаем процент заряда
    float battP = (MaxVoltage - battV) / MaxVoltage * 100;
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
int Pow(int base,int st)
{
    for (size_t i = 0; i < st; i++)
    {
        base = base*base;
    }
    return base;
}
int HooliBattery::GetCurrentBatteryPower()
{
    return batteryPower;
}