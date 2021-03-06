
## Готовый проект для запуска gt911 тач панели как usb hid устройства

За основу были взяты библиотеки:
* HID (взята в исходном виде) - https://github.com/ilufang/HID
* GT911 (модифицирована) - https://github.com/spanner-and-code/GT911

**Тестирование проводилось на arduino pro micro 3.3В 8MhZ**

Подключение:
* TX  -> INT
* RX  -> RST
* GND -> GND
* 2   -> SDA
* 3   -> SCL
* VCC -> VCC

Для подключения тача к МК на 5В следует использовавть конверторы уровней сигналов

Эмуляция USB HID возможна не только на Arduino на базе МК 32U4 (подробнее о списке подерживаемых МК на странице https://github.com/ilufang/HID) 

PS в данном случае можно подключать панели под любое разрешение, касания будут корректно отмасштабированы на экране устройства

![Иллюстрация к проекту](https://github.com/mazahaker/gt911-arduino-capacitive-touch/blob/master/Touch%20connection.jpg)
