# F28027_I2C_LCD
 This program connect F28027 to LCD via I2C 
 Pin map:
 J6.8(GPIO_33)----> SCL
 J6.7(GPIO_32) ----> SDA
 J5.1(5V) ----> VCC
 GND ----> GND
 Additional suport files:
 Add follow path from controlSUITE to C2000 compiler include options
 f2802x_common\include
 f2802x_common\source
 f2802x_headers\include
 f2802x_headers\source
 Add F28027.cmd to the project from controlSUITE
 Add F2802x_Headers_nonBIOS.cmd to the project from controlSUITE
 Add driverlib.lib to the project from controlSUITE
