#ifndef COMMAND_H
#define COMMAND_H

#include "usertype.h"

extern command_attr_t userCommand[];

class Command {
public:
    static const quint16 crcTable[];
    static quint16 commandSize();
    static void updateCommandValue(QString c, QVariant v);
    static QVariant getCommandValue(QString c);
    static int getCommandIndex(QString c);
    static quint16 GetCRC16(QByteArray data, const quint16 *ptable = crcTable, quint16 init = 0xffff);
    static quint16 updateSystemControlInfo();
    static command_value_t requestToByteArray(QString command, QString value = "NONE", int dataNum = 0x02, int devAddr = 0x22);
    static void readSettings();
    static command_attr_t* getCommand(QString c);
};

#endif // COMMAND_H
