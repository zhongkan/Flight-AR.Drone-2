#include "functional.h"

/* int to string */
string itos(const int num)
{
    char tmp[16];
    sprintf(tmp, "%d", num);
    string str(tmp);
    return str;
}

/* int to QString */
QString itos(const int num, const int digitCount)
{
    char tmp[16];
    string format = "%0" + itos(digitCount) + "d";
    sprintf(tmp, format.c_str(), num);
    return QString::fromLocal8Bit(&tmp[0]);
}

QString ftos(const float num)
{
    char tmp[16];
    sprintf(tmp, "%.2f", num);
    return QString::fromLocal8Bit(&tmp[0]);
}

/* char * to string */
string ctos(const char * ch)
{
    string str(ch);
    return str;
}

QString reslut(const RESULT res)
{
    if (M_SUCCEEDED(res))
        return "OK";
    else if (M_ERROR(res))
        return "Error";
    else if(M_NOTNVAPKG(res))
        return "Dis-Match";
    else
        return "Failed";
}
