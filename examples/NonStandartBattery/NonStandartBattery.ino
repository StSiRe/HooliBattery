/*
    HooliBattery NonStandartBattery
    Writed by StSiRe(github)
    Скетч-пример работы с несмтандартным аккумулятором, у которого минимальное напряжение - 3.7 вольта
    Если вы будете использовать сборку аккумуляторов,то просто установите минимальные и максимальные напряжения
    И все будет работать

*/
//Подключаем библиотеку для работы с батареей
#include <HooliBattery.h>

//Номер пина, подключенный к делителю
const int pin = A0;

// BattIn ---
//  |        |
//  |       [ 10000] ResS
//  |        | 
//  |        |-------To Pin
//  U        |           |
//  |       [1000] ResG |
//  |        |           U2
//  |        |           |
// Ground ---------------
//Сопротивление верхнего ключа
const int ResS = 10000;
//Сопротивление нижнего ключа
const int ResG = 1000;
//Объект батареи,но добавляем еще и сопротивления,так как у нас они не стандартные 
HooliBattery Battery(pin,HooliBattery::BatteryType::LiIon,ResS,ResG);
//Максимальное напряжение аккумулятора
const float MaxV = 4.25;
//Минимальное напряжение аккумулятора
const float MinV = 3.7;
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
    //Для начала подскажем бибилотеке,от каких напряжений ей отталкиваться
    //Устанавливаем минимальное напряжение аккумулятора
    Battery.SetMinVoltage(MinV);
    //Устанавливаем максимальное напряжение аккумулятора
    Battery.SetMaxVoltage(MaxV);
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