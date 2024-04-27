# MS4525DO_change_address
Change I2C address of MS4525DO using Arduino.  
MS4525DO can be used as air speed sensor. Use this program to change its i2c address to certain I2C address.  
default address is 0x28.  
a relay is also required and connected to PIN34, used to control power supply of MS4525DO.  
(Changing address requires rapid operation after power MS4525DO on)  
本程序用于使用Arduino修改MS4525DO的I2C地址。  
出场默认地址为0x28,程序中将地址改为0x36。  
需要一个接到34引脚的继电器控制MS4525DO的开关以在传感器启动后极短时间内进行设置。  
