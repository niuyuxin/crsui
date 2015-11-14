#include <QSettings>
#include "command.h"

/*   命令名称 命令属性 最大值 最小值 缩放系数 地址 参数名称 参数列名称 注释 参数值 参数属性 表名称  */

command_attr_t userCommand[] = {
    {"OP1PID-T", WRITE, 99, 1, 1, 68, "PID-T", "OILPUMP1", QObject::tr("MAX, MIN = "), "25", READ|WRITE|SAVE, "PID PARAMETER"},
    {"OP1PID-P", WRITE, 99, 1, 1, 69, "PID-P", "OILPUMP1", QObject::tr("MAX, MIN = "), "80", READ|WRITE|SAVE, "PID PARAMETER"},
    {"OP1PID-I", WRITE, 99, 1, 1, 70, "PID-I", "OILPUMP1", QObject::tr("MAX, MIN = "), "15", READ|WRITE|SAVE, "PID PARAMETER"},
    {"OP1PID-D", WRITE, 99, 1, 1, 71, "PID-D", "OILPUMP1", QObject::tr("MAX, MIN = "), "20", READ|WRITE|SAVE, "PID PARAMETER"},
    {"OP2PID-T", WRITE, 99, 1, 1, 72, "PID-T", "OILPUMP2", QObject::tr("MAX, MIN = "), "15", READ|WRITE|SAVE, "PID PARAMETER"},
    {"OP2PID-P", WRITE, 99, 1, 1, 73, "PID-P", "OILPUMP2", QObject::tr("MAX, MIN = "), "80", READ|WRITE|SAVE, "PID PARAMETER"},
    {"OP2PID-I", WRITE, 99, 1, 1, 74, "PID-I", "OILPUMP2", QObject::tr("MAX, MIN = "), "20", READ|WRITE|SAVE, "PID PARAMETER"},
    {"OP2PID-D", WRITE, 99, 1, 1, 75, "PID-D", "OILPUMP2", QObject::tr("MAX, MIN = "), "20", READ|WRITE|SAVE, "PID PARAMETER"},

    {"OP3PID-T", WRITE, 99, 0, 1, 303, "PID-T", "OILPUMP3", QObject::tr("MAX, MIN = "), "0", READ|WRITE|SAVE, "PID PARAMETER"},
    {"OP3PID-P", WRITE, 99, 0, 1, 304, "PID-P", "OILPUMP3", QObject::tr("MAX, MIN = "), "0", READ|WRITE|SAVE, "PID PARAMETER"},
    {"OP3PID-I", WRITE, 99, 0, 1, 305, "PID-I", "OILPUMP3", QObject::tr("MAX, MIN = "), "0", READ|WRITE|SAVE, "PID PARAMETER"},
    {"OP3PID-D", WRITE, 99, 0, 1, 306, "PID-D", "OILPUMP3", QObject::tr("MAX, MIN = "), "0", READ|WRITE|SAVE, "PID PARAMETER"},

    {"DRVPID-T", WRITE, 99, 1, 1, 76, "PID-T", "DRV", QObject::tr("MAX, MIN = "), "30", READ|WRITE|SAVE, "PID PARAMETER"},
    {"DRVPID-P", WRITE, 99, 1, 1, 77, "PID-P", "DRV", QObject::tr("MAX, MIN = "), "80", READ|WRITE|SAVE, "PID PARAMETER"},
    {"DRVPID-I", WRITE, 99, 1, 1, 78, "PID-I", "DRV", QObject::tr("MAX, MIN = "), "20", READ|WRITE|SAVE, "PID PARAMETER"},
    {"DRVPID-D", WRITE, 99, 1, 1, 79, "PID-D", "DRV", QObject::tr("MAX, MIN = "), "20", READ|WRITE|SAVE, "PID PARAMETER"},

    {"SPEEDPID-T", WRITE, 99, 1, 1, 80, "PID-T", "SPEED", QObject::tr("MAX, MIN = "), "60", READ|WRITE|SAVE, "PID PARAMETER"},
    {"SPEEDPID-P", WRITE, 99, 1, 1, 81, "PID-P", "SPEED", QObject::tr("MAX, MIN = "), "80", READ|WRITE|SAVE, "PID PARAMETER"},
    {"SPEEDPID-I", WRITE, 99, 1, 1, 82, "PID-I", "SPEED", QObject::tr("MAX, MIN = "), "20", READ|WRITE|SAVE, "PID PARAMETER"},
    {"SPEEDPID-D", WRITE, 99, 1, 1, 83, "PID-D", "SPEED", QObject::tr("MAX, MIN = "), "20", READ|WRITE|SAVE, "PID PARAMETER"},

    //关于泵的控制信息， 此处有两种泵可供选择， 本程序未实现喷油泵的功能， 此代码地址不正确

    {"HP0CyclinderNumber", WRITE, 10, 1, 1,  1012, "CYCLINDER NUMBER", "HP0", "num == 4 or num == 6", "6", READ|WRITE|SAVE, "OILPUMP"},
    {"HP0GearNumber", WRITE, 10, 1, 1,  0, "GEAR NUMBER", "HP0", "xxxxxxxxxxxxxxxxx", "7", READ|WRITE|SAVE, "OILPUMP"},
    {"HP0MAX", WRITE, 99, 1, 1, 0, "MAXIMUM VALUE", "HP0", "xxxxxxxxxxxxxxxxx",  "14", READ|WRITE|SAVE, "OILPUMP"},
    {"HP0MIN", WRITE, 99, 1, 1, 0, "MINIMUM VALUE", "HP0", "xxxxxxxxxxxxxxxxx",  "21", READ|WRITE|SAVE, "OILPUMP"},
    {"HP0MID", WRITE, 99, 1, 1, 0, "MIDDLE VALUE", "HP0", "xxxxxxxxxxxxxxxxxx",  "50", READ|WRITE|SAVE, "OILPUMP"},
    {"HP0FlowPluse", WRITE, 99, 0, 1,  6, "OIL PUMP FLOW PULSE", "HP0", "COMMENT", "0", READ|WRITE|SAVE, "OILPUMP"},
    {"HP0FlowCoefficient", WRITE, 999, 0, 100, 7, "OIL PUMP FLOW COEFFICIENT", "HP0", "COMMENT",  "2800", READ|WRITE|SAVE, "OILPUMP"},
    {"HP0PumpFlowGearNumber", WRITE, 99, 1, 1, 8, "OIL PUMP  FLOW GEAR NUMBER", "HP0", "COMMENT",  "1", READ|WRITE|SAVE, "OILPUMP"},
    {"HP0FirstDrvFrequence", WRITE, 999, 0, 1,  9, "FIRST DRV FREQUENCE", "HP0", "COMMENT",   "200", READ|WRITE|SAVE, "OILPUMP"},
    {"HP0SecondDrvFrequence", WRITE, 999, 0, 1, 10, "SECOND DRV FREQUENCE", "HP0", "COMMENT",   "200", READ|WRITE|SAVE, "OILPUMP"},
    {"EUI/EPICylinderNumber", WRITE, 99, 0, 1, 11, "CYCLINDER NUMBER", "EUI/EUP", "COMMENT",  "0", READ|WRITE|SAVE, "OILPUMP"},
    {"EUI/EPIGearNumber", WRITE, 99, 1, 1, 12,"GEAR NUMBER", "EUI/EUP", "COMMENT",   "1", READ|WRITE|SAVE, "OILPUMP"},
    {"EUI/EPIMaxValue", WRITE, 99, 0, 1, 13, "MAXIMUM VALUE", "EUI/EUP", "COMMENT",  "0", READ|WRITE|SAVE, "OILPUMP"},
    {"EUI/EPIMinValue", WRITE, 99, 0, 1, 14,"MINIMUM VALUE", "EUI/EUP", "COMMENT",   "0", READ|WRITE|SAVE, "OILPUMP"},
    {"EUI/EPIMidValue", WRITE, 99, 0, 1, 15,"MIDDLE VALUE", "EUI/EUP", "COMMENT",  "0", READ|WRITE|SAVE, "OILPUMP"},
    {"EUI/EPIPumpFlowPulse", WRITE, 99, 0, 1, 16,"OIL PUMP FLOW PULSE", "EUI/EUP", "COMMENT",  "0", READ|WRITE|SAVE, "OILPUMP"},
    {"EUI/EPIPumpFlowCoe", WRITE, 99, 0, 1, 17,"OIL PUMP FLOW COEFFICIENT", "EUI/EUP", "COMMENT",   "0", READ|WRITE|SAVE, "OILPUMP"},
    {"EUI/EPIPumpFlowGearNumber", WRITE, 99, 0, 1, 18, "OIL PUMP  FLOW GEAR NUMBER", "EUI/EUP", "COMMENT",  "0", READ|WRITE|SAVE, "OILPUMP"},
    {"EUI/EPIFirstDrvFrequence", WRITE, 999, 0, 1, 19,"FIRST DRV FREQUENCE", "EUI/EUP", "COMMENT",  "200", READ|WRITE|SAVE, "OILPUMP"},
    {"EUI/EPISecondDrvFrequence", WRITE, 999, 0, 1, 20,"SECOND DRV FREQUENCE", "EUI/EUP", "COMMENT",  "200", READ|WRITE|SAVE, "OILPUMP"},


    {"InjectorPwmValue", WRITE, 99, 1, 1,  0,"PWM VALUE", "VALUE", "COMMENT",   "1", READ|WRITE, "INJECTOR/DRV/RAIL"},
    {"DrvValue", WRITE, 99, 1, 1,  0,"DRV VALUE", "VALUE", "COMMENT",   "10", READ|WRITE, "INJECTOR/DRV/RAIL"},
    {"InjFlowPulseNum", READ, 99, 0, 1, 460,"InjFlowPulseNum", "VALUE", "COMMENT",   "0", READ|WRITE, "INJECTOR/DRV/RAIL"},
    {"InjFlowSensorCoe", WRITE, 0, 0, 1, 0, "INJECTOR FLOW COEFFICIENT", "VALUE", "COMMENT",  2700, READ|WRITE|SAVE, "INJECTOR/DRV/RAIL"},
    {"InjectorCountValue", READ, 0, 0, 1, 410, "INJECTOR COUNT", "VALUE", "COMMENT",  0, READ, "INJECTOR/DRV/RAIL"},

    {"MotoDirection", NONE, 1, 0, 1, 0, "MOTOR DIRECTION", "VALUE", "Fwd = 0, Rev = 1", "0", READ|WRITE|SAVE, "INJECTOR/DRV/RAIL"},
    {"RailPressure", READ, 999, 0, 10, 430, "RailPressure", "VALUE", "COMMENT",  "0", READ|WRITE, "INJECTOR/DRV/RAIL"},
    {"RailPressureError", NONE, 10, 0, 1, 0, "RailPressureError", "VALUE", "COMMENT",  2, READ|WRITE|SAVE, "INJECTOR/DRV/RAIL"},
    {"RailPressureAD", NONE, 999, 0, 1, 510, "RailPressureAD", "VALUE", "COMMENT",  "0", READ|WRITE, "INJECTOR/DRV/RAIL"},
    {"RPStableTimes", NONE, 100000, 0, 1, 0, "RailPressureStableTimes", "VALUE", "COMMENT",  3000, READ|WRITE|SAVE, "INJECTOR/DRV/RAIL"},
    {"ClosePumpMinPWM", WRITE, 99, 1, 100, 89, "ClosePumpMinPWM", "VALUE", "COMMENT",  17, READ|WRITE|SAVE, "INJECTOR/DRV/RAIL"},
    {"ClosePumpMaxPWM", WRITE, 99, 1, 100, 90, "ClosePumpMaxPWM", "VALUE", "COMMENT",  35, READ|WRITE|SAVE, "INJECTOR/DRV/RAIL"},
    {"CRPumpType", WRITE, 10, 0, 1, 11, "CRPumpType", "VALUE", "5: open, 6: close",  "5", READ|WRITE, "INJECTOR/DRV/RAIL"}, // 常开或常闭泵

    {"InjectorFre", WRITE, 0, 0, 1, 2, "InjectorFre", "VALUE", "COMMENT", "0", READ|WRITE, "INJECTOR/DRV/RAIL"},
    {"CleanFre", WRITE, 99, 1, 1, 0,"CLEAN FREQUENCE", "VALUE", "COMMENT",   "17", READ|WRITE, "INJECTOR/DRV/RAIL"},
    {"LeakFre", WRITE, 99, 1, 1, 0,"LEAK FREQUENCE", "VALUE", "COMMENT",   "17", READ|WRITE, "INJECTOR/DRV/RAIL"},
    {"VLFre", WRITE, 99, 1, 1, 0,"VL FREQUENCE", "VALUE", "COMMENT",  "17", READ|WRITE, "INJECTOR/DRV/RAIL"},
    {"TLFre", WRITE, 99, 1, 1, 0,"TL FREQUENCE", "VALUE", "COMMENT",  "20", READ|WRITE, "INJECTOR/DRV/RAIL"},
    {"LLFre", WRITE, 99, 1, 1, 0,"LL FREQUENCE", "VALUE", "COMMENT",   "30", READ|WRITE, "INJECTOR/DRV/RAIL"},
    {"VEFre", WRITE, 99, 1, 1, 0,"VE FREQUENCE", "VALUE", "COMMENT",   "40", READ|WRITE, "INJECTOR/DRV/RAIL"},

//    {99, 1, 1, 12,"FLOW ERROR", "ITEM VALUE", "COMMENT",   "0.4", READ|WRITE, "INJECTOR/DRV/RAIL"},
//    {99, 1, 1,  6, "RAIL PRESSURE ERROR VALUE", "ITEM VALUE", "COMMENT",  "2", READ|WRITE, "INJECTOR/DRV/RAIL"},
//    {99, 1, 1, 7,"RAIL PRESSURE CONFIRM TIMES", "ITEM VALUE", "COMMENT",   "10", READ|WRITE, "INJECTOR/DRV/RAIL"},
//    {99, 1, 1, 8,"PWM MINIMUM VALUE", "ITEM VALUE", "COMMENT",   "21", READ|WRITE, "INJECTOR/DRV/RAIL"},
//    {99, 1, 1,  9, "PWM MAXIMUM VALUE", "ITEM VALUE", "COMMENT",   "35", READ|WRITE, "INJECTOR/DRV/RAIL"},

    //系统控制信息为一个16bits的字， 此处为其展开所有字节， 地址是每一位的确切定义

    {"SystemControlInfo", WRITE, 0, 0, 1, 1, "SystemControlInfo", "VALUE", "COMMENT", 0, READ|WRITE|SAVE, ""},
    {"MotoFwd", NONE, 1, 0, 1, 0, "MOTOR FORWARD", "VALUE", "COMMENT", 0, READ|WRITE, ""},
    {"MotoRev", NONE, 1, 0, 1, 1, "MOTOR REVERSE", "VALUE", "COMMENT", 0, READ|WRITE, ""},
    {"OilPumpMoto", NONE, 1, 0, 1, 2, "OIL PUMP MOTO", "VALUE", "COMMENT", "0", READ|WRITE, ""},
    {"CollectFuel", NONE, 1, 0, 1, 3, "COUNT", "VALUE", "COMMENT", "0", READ|WRITE, ""},
    {"TControl", NONE, 1, 0, 1, 4, "TEMPERATURE CONTROL", "VALUE", "COMMENT", "0", READ|WRITE, ""},
    {"AdvanceAngleClear", NONE, 1, 0, 1, 5, "AdvanceAngleClear", "VALUE", "COMMENT", "0", READ|WRITE, ""},
    {"RuleBarClear", NONE, 1, 0, 1, 6, "Rule bar claer", "VALUE", "COMMENT", "0", READ|WRITE, ""},
    {"FuelSupplyEMV", NONE, 1, 0, 1, 7, "FuelSupplyEMV", "VALUE", "COMMENT", "0", READ|WRITE, ""},
    {"InjectorControl", NONE, 1, 0, 1, 8, "Injector", "VALUE", "COMMENT", "0", READ|WRITE, ""},
    {"CRPControl", NONE, 1, 0, 1, 9, "Injector", "VALUE", "COMMENT", "0", READ|WRITE, ""},
    {"InjectorCountSwitch", NONE, 1, 0, 1, 10, "InjectorCount", "VALUE", "COMMENT", "0", READ|WRITE, ""},
    {"CRPControlType", NONE, 1, 0, 1, 11, "CRPumpControlType", "VALUE", "0, Normal Open; 1, Normal Close", "0", READ|WRITE, "INJECTOR/DRV/RAIL"},
    {"StopMachine", NONE, 1, 0, 1, 13, "StopMachine", "VALUE", "COMMENT", "0", READ|WRITE, ""},
    {"FuelReturnEMV", NONE, 1, 0, 1, 14, "FuelReturnEMV", "VALUE", "COMMENT", "0", READ|WRITE, ""},
    {"DRVSwitch", NONE, 1, 0, 1, 15, "DRVSwitch", "VALUE", "COMMENT", "0", READ|WRITE, ""},

    {"InjectorControlType", WRITE, 0, 0, 1, 3, "ControlType", "INJECTOR", "COMMENT", "0", READ, "InjectorFeature"},
    {"InjectorControlVolatge", WRITE, 0, 0, 1, 21, "ControlVolatge", "INJECTOR", "COMMENT", "0", READ, "InjectorFeature"},
    {"InjectorEnhanceVoltage", WRITE, 0, 0, 1, 22, "EnhanceVoltage", "INJECTOR", "COMMENT", "0", READ, "InjectorFeature"},
    {"InjectorEnhanceCurrent", WRITE, 0, 0, 1, 5, "EnhanceCurrent", "INJECTOR", "COMMENT", "0", READ, "InjectorFeature"},
    {"InjectorShakeCurrent", WRITE, 0, 0, 1, 6, "ShakeCurrent", "INJECTOR", "COMMENT", "0", READ, "InjectorFeature"},
    {"InjectorShakeCurrentError", WRITE, 0, 0, 1, 8, "ShakeCurrentError", "INJECTOR", "COMMENT", "0", READ, "InjectorFeature"},
    {"InjectorKeepCurrent", WRITE, 0, 0, 1, 7, "KeepCurrent", "INJECTOR", "COMMENT", "0", READ, "InjectorFeature"},
    {"InjectorKeepCurrentError", WRITE, 0, 0, 1, 9, "KeepCurrentError", "INJECTOR", "COMMENT", "0", READ, "InjectorFeature"},
    {"InjectorEnhanceTime", WRITE, 0, 0, 1, 13, "EnhanceTime", "INJECTOR", "COMMENT", "0", READ, "InjectorFeature"},
    {"InjectorShakeTime", WRITE, 0, 0, 1, 16, "ShakeTime", "INJECTOR", "COMMENT", "0", READ, "InjectorFeature"},
    {"InjectorMinKeepTime", WRITE, 0, 0, 1, 10, "MinKeepTime", "INJECTOR", "COMMENT", "0", READ, "InjectorFeature"},
    {"InjectorCloseType", WRITE, 0, 0, 1, 29, "CloseType", "INJECTOR", "COMMENT", "0",  READ, "InjectorFeature"},

    {"RailSensorType", WRITE, 1, 0, 1,  91,"WHICHONE", "VALUE", "BOSCH = 0, DENSO = 1", "0", READ|WRITE|SAVE, "RailSensor"},
    {"BoschRailSensorMAX", WRITE, 9999, 1, 1,  101,"BOSCH MAX",  "VALUE", "COMMENT", "950", READ|WRITE|SAVE, "RailSensor"},
    {"BoschRailSensorMIN", WRITE, 9999, 1, 1,  100, "BOSCH MIN", "VALUE", "COMMENT", "100", READ|WRITE|SAVE, "RailSensor"},
    {"DensoRailSensorMAX", WRITE, 9999, 1, 1, 103,  "DENSO MAX", "VALUE","COMMENT", "794", READ|WRITE|SAVE, "RailSensor"},
    {"DensoRailSensorMIN", WRITE, 9999, 1, 1, 102, "DENSO MIN", "VALUE", "COMMENT", "204", READ|WRITE|SAVE, "RailSensor"},

    {"Speed", WRITE, 5000, 1, 1, 48, "", "VALUE", "COMMENT",  1, READ|WRITE|SAVE, ""},
    {"CountSet", WRITE, 5000, 1, 1, 63, "", "VALUE", "COMMENT",  1, READ|WRITE|SAVE, ""},
    {"InjectorControlTime", WRITE, 0, 0, 1, 12, "", "", "",  "", READ, ""},
    {"RailPressureSetValue", WRITE, 0, 0, 1, 15, "", "", "", "", READ, ""},
    {"TemperatureSetValue", WRITE, 0, 0, 1, 84, "", "", "", "42", READ|WRITE|SAVE, ""},
    {"InjectorCountSet", WRITE, 0, 0, 1, 14, "INJECTOR COUNT", "VALUE", "COMMENT",  0, READ, ""},
    {"BenchTempSet", WRITE, 99, 0, 1, 84, "", "VALUE", "COMMENT",  0, READ|WRITE|SAVE, ""},
    {"TemperatureValue", READ, 999, 0, 10, 480, "TEMPERATURE", "VALUE", "temperature", "0", READ|WRITE, ""},
    {"InjFlowValue", NONE, 0, 0, 1, 0, "Injector Flow disp", "VALUE", "COMMENT", "0", READ, ""},
    {"SpeedValue", READ, 0, 0, 1, 450, "SpeedValue", "VALUE", "COMMENT", "0", READ|WRITE, ""},

};
/*
static Para_t SensorType[] = {

};
static Para_t Pizeo[] = {
    {999, 1, 1,  1,"HIGH VOLTAGE TIME", "ITEM VALUE", "COMMENT",   "80", READ|WRITE},
    {999, 1, 1,  2,"HIGH VOLTAGE CURRENT", "ITEM VALUE", "COMMENT",   "80", READ|WRITE},
    {999, 1, 1, 3,"CLOSE CURRENT", "ITEM VALUE", "COMMENT",   "1", READ|WRITE},
    {999, 1, 1, 4, "HOLD CURRENT", "ITEM VALUE", "COMMENT",  "99", READ|WRITE},
    {999, 1, 1,  5, "DISCHARGE TIME", "ITEM VALUE", "COMMENT",  "140", READ|WRITE},
    {999, 1, 1,  6, "CLEAN FREQUENCE", "ITEM VALUE", "COMMENT",  "17", READ|WRITE},
    {999, 1, 1, 7,"LEAK FREQUENCE", "ITEM VALUE", "COMMENT",   "17", READ|WRITE},
    {999, 1, 1, 8,"VL FREQUENCE", "ITEM VALUE", "COMMENT",   "17", READ|WRITE},
    {999, 1, 1,  9, "TL FREQUENCE", "ITEM VALUE", "COMMENT",   "20", READ|WRITE},
    {999, 1, 1, 10, "LL FREQUENCE", "ITEM VALUE", "COMMENT",   "30", READ|WRITE},
    {999, 1, 1, 11, "VE FREQUENCE", "ITEM VALUE", "COMMENT",  "40", READ|WRITE}
};
static Para_t Other[] = {
    {999, 1, 1,  1,"ENHANCE TIME", "ITEM VALUE", "COMMENT",   "100", READ|WRITE},
    {999, 1, 1,  2,"ENHANCE CURRENT", "ITEM VALUE", "COMMENT",   "25", READ|WRITE},
    {999, 1, 1, 3,"CLOSE CURRENT", "ITEM VALUE", "COMMENT",   "12", READ|WRITE},
    {999, 1, 1, 4, "CLOSE TIME", "ITEM VALUE", "COMMENT",  "350", READ|WRITE},
    {999, 1, 1,  5, "HOLD CURRENT", "ITEM VALUE", "COMMENT",  "12", READ|WRITE},
};

*/
quint16 const Command::crcTable[] = {
    0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241,
    0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440,
    0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40,
    0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,
    0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40,
    0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41,
    0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641,
    0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040,
    0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240,
    0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,
    0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41,
    0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840,
    0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41,
    0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40,
    0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640,
    0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041,
    0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240,
    0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,
    0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41,
    0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,
    0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41,
    0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,
    0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640,
    0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,
    0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241,
    0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440,
    0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40,
    0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841,
    0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40,
    0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,
    0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641,
    0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040
};

quint16 Command::commandSize() {
    return sizeof(userCommand)/sizeof(command_attr_t);
}

void Command::updateCommandValue(QString c, QVariant v)
{
    for (int j = 0; j < commandSize(); ++j) {
        if (userCommand[j].commandName == c) {
                userCommand[j].paraValue = v;
            return;
        }
    }
}
QVariant Command::getCommandValue(QString c)
{
    for (int j = 0; j < commandSize(); ++j) {
        if (userCommand[j].commandName == c) {
            return userCommand[j].paraValue.toDouble() * userCommand[j].paraCoe;
        }
    }
    return 0;
}
int Command::getCommandIndex(QString c)
{
    for (int j = 0; j < commandSize(); ++j) {
        if (userCommand[j].commandName == c) {
            return j;
        }
    }
    return -1;
}
command_attr_t* Command::getCommand(QString c)
{
    for (int j = 0; j < commandSize(); ++j) {
        if (userCommand[j].commandName == c) {
            return &userCommand[j];
        }
    }
    return 0;
}
/**
 * @brief Thread::requestToByteArray
 *                  转换请求为字节序
 *                  FIXME: 会不会有重入的问题出现
 * @param dataValue
 * @param dataAddr
 * @param rdwr
 * @param devAddr
 * @return
 */
command_value_t Command::requestToByteArray(QString command, QString userValue, int dataNum, int devAddr)
{
    QByteArray byteArray;
    int rdwr;
    quint16 addr;
    quint32 commandIndex;
    command_value_t cv;

    for (commandIndex = 0; commandIndex < commandSize(); ++commandIndex) {
        if (userCommand[commandIndex].commandName == command) {
            rdwr = userCommand[commandIndex].commandAttr == WRITE ? 0x06 :
                   userCommand[commandIndex].commandAttr == READ ? 0x03 : 0xffff; // 错误时暂时以ffff
            addr = userCommand[commandIndex].commandAddr;
            break;
        }
    }
    if (rdwr == 0xffff || commandIndex == commandSize()) { //容错处理 FIXME:
        cv.command = command;
        cv.value = "";
        return cv;
    }
    byteArray.append(devAddr&0xff).append(rdwr&0xff);   // 设备地址 读写标志
    byteArray.append(addr >> 8).append(addr & 0xff);    // 命令地址 高字节 低字节
    if ((rdwr & 0x03) == 0x03) { // read
        byteArray.append(dataNum >> 8).append(dataNum & 0xff);  //数据长度 字节为单位 FIXME: <此处有疑问>
    }
    if ((rdwr & 0x06) == 0x06) { //write
        if (userValue == "NONE") {
            quint16 value = userCommand[commandIndex].paraValue.toUInt();
            byteArray.append(value>>8).append(value&0xff);
        } else {
            quint16 value = userValue.toUInt();
            byteArray.append(value>>8).append(value&0xff);
        }

    }
    quint16 crc = GetCRC16(byteArray);
    byteArray.append(crc>>8).append(crc&0xff);
    cv.command = command;
    cv.value = byteArray;
    return cv;
}
/**
 * @brief GetRevCrc_16
 * @param ptable
 * @param pData
 * @param nLength
 * @param init
 * @return 返回值为校验结果
 */
quint16 Command::GetCRC16(QByteArray data, const quint16 *ptable, quint16 init)
{
    unsigned short cRc_16 = init;
    unsigned char temp;
    for (int i = 0; i < data.size(); ++i)
    {
      temp = cRc_16 & 0xFF;
      cRc_16 = (cRc_16 >> 8) ^ ptable[(temp ^ data.at(i)) & 0xFF];
    }
    cRc_16 = (cRc_16 >> 8) + (cRc_16 << 8);
    return cRc_16;
}
//quint16 Command::OrderPosition(QString ord)
//{
//    for (int i = 0; i < commandSize(); ++i) {
//        if (ord == userCommand[i].commandName) return i;
//    }
//    return 0xffff;
//}

quint16 Command::updateSystemControlInfo()
{
    QStringList systemControlInfo;
    quint16 systemControlInfoPosition = getCommandIndex("SystemControlInfo");
    systemControlInfo << "MotoFwd"
                        << "MotoRev"
                        << "OilPumpMoto"
                        << "CollectFuel"
                        << "TControl"
                        << "AdvanceAngleClear"
                        << "RuleBarClear"
                        << "FuelSupplyEMV"
                        << "InjectorControl"
                        << "InjectorCountSwitch"
                        << "CRPControl"
                        << "CRPControlType"
                        << "StopMachine"
                        << "FuelReturnEMV"
                        << "DRVSwitch";
    for (int i = 0; i < commandSize(); ++i) {
        if (systemControlInfo.contains(userCommand[i].commandName)) {
            if (userCommand[i].paraValue.toUInt()) {
                userCommand[systemControlInfoPosition].paraValue =
                userCommand[systemControlInfoPosition].paraValue.toUInt() | (1 << userCommand[i].commandAddr);
            } else {
                userCommand[systemControlInfoPosition].paraValue =
                userCommand[systemControlInfoPosition].paraValue.toUInt() & (~(1 << userCommand[i].commandAddr));
            }
        }
    }
    //::qDebug() << "SystemControlInfo" << userCommand[systemControlInfoPosition].paraValue.toString();
    return userCommand[systemControlInfoPosition].paraValue.toUInt();
}
void Command::readSettings()
{
    QSettings settings("DoLong Inc", "Command");
    for (int i = 0; i < commandSize(); ++i) {
        userCommand[i].paraValue =
                settings.value(userCommand[i].commandName, userCommand[i].paraValue);
    }

}

