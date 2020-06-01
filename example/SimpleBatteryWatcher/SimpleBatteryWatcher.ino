//Подключаем библиотеку для работы с батареей
#include <HooliBattery.h>
//Номер пина, подключенный к делителю
const int pin = A0;
HooliBattery Battery(pin,HooliBattery::BatteryType::LiIon);
//Выполняется в случае полного заряда
void Charged()
{  
  Serial.println("Charged");
}
//Выполняется в случае полного разряда(меньше 4%)
void Discharged()
{  
  Serial.println("Discharged");
}
//Выполняется при достижении 10 % заряда на аккумуляторе
void o10()
{
  Serial.println("10%");
}
void setup() {
    Serial.begin(9600);
    //Назначаем события:
    Battery.onCharged(Charged);
    Battery.onDischarged(Discharged);
    Battery.on10Percent(o10);
}
void loop() 
{
    //Опрашиваем батарею
    Battery.CheckBattery();
    //Узнаем текущий заряд:
    int power = Battery.GetCurrentBatteryPower();
    Serial.print("Current power:");
    Serial.print(power);
    Serial.println("%");
    //Делаем задержку
    delay(1000);
}