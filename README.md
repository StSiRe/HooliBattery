# HooliBattery
Библиотека для работы с аккумуляторами(Li-ion,Li-pol,LiFePO4)

Работает на Arduino платформах и на ESP32

Данная библиотека позволяет:
1. Оценивать текущий заряд батареи
    ```c++
    int GetCurrentBatteryPower();
    ```
2. Выполнять пользовательский код(если он есть) при определенных событиях:
    * При полном заряде
    ```c++
    void onCharged(callbackFunction charged);
    ```
    * При 30% заряде
    ```c++
    void on30Percent(callbackFunction on30);
    ```
    * При 20% заряде
    ```c++
    void on20Percent(callbackFunction on20);
    ```
    * При 10% заряде
    ```c++
    void on10Percent(callbackFunction on10);
    ```
    * При полной разрядке батареи(от 4 до 0 процентов)
    ```c++
    void onDischarged(callbackFunction discharged);
    ```
3. Узнавать какой процесс происходит на данный момент:
    * Зарядка
    ```c++
    LifeCycle GetLifeCycle();
    ```
    * Разрядка
    ```c++
    LifeCycle GetLifeCycle();
    ```
    * LifeCycle
        * Charging = 0
        * Discharging = 1
