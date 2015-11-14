#ifndef USERTYPE_H
#define USERTYPE_H

#include <QString>
#include <QVariant>

enum {
      NONE = 0,
      READ = 0x01,  // 只读
      WRITE = 0x02, // 只写
      SELECT = 0x04, // 可选择
      SELDEF = 0x08, // 默认选中
      SAVE = 0x10,
     SREAD = 0x03,
    SWRITE = 0x06,
     };

struct command_value_t{
    QString command;
    QByteArray value;
};

struct command_attr_t{
    QString commandName; //命令名称
    quint8 commandAttr; //命令属性
    int paraMax;//最大值
    int paraMin;//最小值
    int paraCoe;//缩放系数
    int commandAddr;//地址
    QString paraName;//参数名称
    QString columnName;//参数列名称
    QString comment;//注释
    QVariant paraValue;//参数值
    quint8 paraAttr;//参数属性
    QString tableName; //表名称 用于建立参数表
};



#endif // USERTYPE_H

