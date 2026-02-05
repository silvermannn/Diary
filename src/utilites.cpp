#include "utilites.h"

#include <QRegularExpressionValidator>

QValidator* createUniversalDoubleValidator()
{
    return new QRegularExpressionValidator(QRegularExpression("\\d+([,.]\\d+)?"));
}
