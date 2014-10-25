/**
 * @file    function.h
 * @author  kzhong1991<kzhong1991@gmail.com>
 * @date    2012-12-11
 * @brief   Some other function or typedef used by in program
 *******************************************************/
#ifndef FUNCTIONAL_H
#define FUNCTIONAL_H
#include <QtCore/QString>
#include <string>
#include <cstdio>
using namespace std;

typedef qint64    RESULT;
typedef qint32    bool_t;

typedef struct _matrix33_t
{
    float m11;
    float m12;
    float m13;
    float m21;
    float m22;
    float m23;
    float m31;
    float m32;
    float m33;
} matrix33_t;

typedef struct _vector31_t {
    union {
        float v[3];
        struct
        {
            float x;
            float y;
            float z;
        };
    };
} vector31_t;

typedef union _vector21_t {
    float  v[2];
    struct
    {
        float x;
        float y;
    };
} vector21_t;

typedef struct _screen_point_t {
    qint32 x;
    qint32 y;
} screen_point_t;

typedef union _float_to_int_u {
    float f_value;
    int   i_value;
} float_to_int_u;

#define A_OK					(1)
#define A_FAILED				(0)
#define A_ERROR					(-1)
#define A_NOTNVAPKG				(-2)

#define M_SUCCEEDED(res)		(1 == res)
#define M_FAILED(res)			(0 == res)
#define M_ERROR(res)			(-1 == res)
#define M_NOTNVAPKG(res)		(-2 == res)

#define VIDEO_INIT_OK			(0x11)
#define VIDEO_INIT_FAILED		(0x12)
#define NAV_DATA_INIT_OK		(0x13)
#define NAV_DATA_INIT_FAILED	(0x14)
#define ARDRONE_INIT_OK			(0x15)
#define ARDRONE_INIT_FAILED		(0x16)

//#define DEBUG_NAVDATA_C "debug"


/* int to string */
string itos(const int num);

/* int to QString */
QString itos(const int num, const int digitCount);

/* float to string */

QString ftos(const float num);

/* char * to string */
string ctos(const char * ch);

QString reslut(const RESULT res);

#endif // FUNCTIONAL_H
