﻿‼//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \page page03 Аппаратные сигналы генератора

    \code
    +----------+------+-------------+----------------------------------------------------------------------------------------------------------------+
    | Обозн.   |Порт  |  Куда идёт  | Назначение                                                                                                     |
    | по схеме |      |             |                                                                                                                |
    +----------+------+-------------+----------------------------------------------------------------------------------------------------------------+
    | SPI3_CS1 | PF6  | AD9952(1)   | Выбор DDS первого канала. Активный уровень - 0                                                                 |
    | SPI3_CS2 | PF7  | AD9952(2)   | Выбор DDS второго канала. Активный уровень - 0                                                                 |
    | IO_UPD   | PC7  | AD9952(1,2) | Засылка в регистр выбранной DDS ранее записанного по SPI значения. Запись происходит по положительному фронту  |
    | IOSYN1   | PF8  | AD9952(1)   | Сброс контроллера последовательных данных DDS. Активный уровень - 1. По этому сигналу прекращается приём       |
    | IOSYN2   | PF9  | AD9952(2)   | данных, работа посделовательного интерфейса начинается, когда 0.                                               |
    | RES_DDS  | PC8  | AD9952(1,2) | Сброс обоих DDS                                                                                                |
    +----------+------+-------------+----------------------------------------------------------------------------------------------------------------+
    | LDAC1    | PC2  | AD5697(1)   | Запись данных из буферного регистра в рабочий регистр. В исходном состоянии должен быть установлен в 1.        |
    | LDAC2    | PC3  | AD5697(2)   | Установка в 0 записывает информацию из входного регистра в регистр DAC                                         |
    | D_RS1    | PC5  | AD5697(1)   | Сброс. Происходит по отрицательному фронту                                                                     |
    | D_RS2    | PB0  | AD5697(2)   | Сброс. Происходит по отрицательному фронту                                                                     |
    | SDA      |      | AD5697(1,2) | Интерфейс I2C для записи в регистры                                                                            |
    | SCL      |      | AD5697(1,2) | Интерфейс I2C для записи в регистры                                                                            |
    +----------+------+-------------+----------------------------------------------------------------------------------------------------------------+
    | P1       | PE12 | ПЛИС/DDS 1  | 0 - работает ПЛИС, 1 - работает AD9952 для 1-го канала                                                         |
    | P2       | PE13 | ПЛИС/DDS 2  | 0 - работает ПЛИС, 1 - работает AD9952 для 2-го канала                                                         |
    | P3       | PE14 | откл/вкл 1  | 0 - откл, 1 - вкл канал 1                                                                                      |
    | P4       | PE15 | откл/вкл 2  | 0 - откл, 1 - вкл канал 2                                                                                      |
    +----------+------+-------------+----------------------------------------------------------------------------------------------------------------+
    | WR_RG    | PD11 | ПЛИС        | Сигнал записи последовательных данных в регистры ПЛИС                                                          |
    | CLK_RG   | PD12 | ПЛИС        | Тактирование данных для передачи в ПЛИС                                                                        |
    | DT_RG    | PD13 | ПЛИС        | Сигнал последовательных данных в регистры ПЛИС                                                                 |
    | A0_RG    | PG5  | ПЛИС        |\                                                                                                               |
    | A1_RG    | PG6  | ПЛИС        || Адресация регистров ПЛИС                                                                                      |
    | A2_RG    | PG7  | ПЛИС        ||                                                                                                               |
    | A3_RG    | PG8  | ПЛИС        |/                                                                                                               |
    +----------+------+-------------+----------------------------------------------------------------------------------------------------------------+
    \endcode

         Регистры ПЛИС. Запись в регистры начинается со старшего бита
    \code
    +----------+--------------------------------------------------------------------------------+
    | Бит      |                                                                                | 
    +----------+--------------------------------------------------------------------------------+ 
    | 0h             Регистр управления 16 бит                                                  |
    +----------+--------------------------------------------------------------------------------+ 
    | D0       | запись(1) / чтение (0) данных в/из памяти                                      |
    | D1       | 1 - импульсный режим работы 1                                                  |
    | D2       | 2 - импульсный режим работы 2 (второй канал сдвинут относительно первого)      |
    | D3       | полярность импульсов                                                           |
    +----------+--------------------------------------------------------------------------------+ 
    | 01h         Установка периода импульсного сигнала A                                       |
    +----------+--------------------------------------------------------------------------------+
    | 02h         Установка длительности импульсного сигнала A                                  |
    +----------+--------------------------------------------------------------------------------+
    | 03h         Установка периода импульсного сигнала B                                       |
    +----------+--------------------------------------------------------------------------------+
    | 04h         В импульсном режиме 1 - длительсность импульса B, в имп. режиме 2 - задержка  |
    +----------+--------------------------------------------------------------------------------+
    | 05h         Длительсность импульса B в импульсном режиме 2                                |
    +----------+--------------------------------------------------------------------------------+
    | 06h         Установка частоты сигнала 24 бита                                             |
    +----------+--------------------------------------------------------------------------------+
    | D0...D23 | Fs = M * F0 / 2^24, где F0 - тактовая частота                                  |
    +----------+--------------------------------------------------------------------------------+
    
    Периоды и длительности записываются в единицах, кратных 10e-8c.

    \endcode
*/