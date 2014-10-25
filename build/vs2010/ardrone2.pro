HEADERS += \
    ../../src/entry/mainWindow.h \
    ../../src/common/functional.h \
    ../../src/common/config.h \
    ../../src/common/ardrone.h \
    ../../src/ctrl/control.h \
    ../../src/meters/rollmeter.h \
    ../../src/meters/qmeter.h \
    ../../src/meters/pitchmeter.h \
    ../../src/meters/navigater.h \
    ../../src/meters/altimeter.h \
    ../../src/navdata/navdata_keys.h \
    ../../src/navdata/navdata_common.h \
    ../../src/navdata/navdata.h \
    ../../src/navdata/navdata_update.h \
    ../../src/video/vision_common.h \
    ../../src/video/video.h \
    ../../src/ctrl/control_states.h

SOURCES += \
    ../../src/entry/mainWindow.cpp \
    ../../src/entry/main.cpp \
    ../../src/common/functional.cpp \
    ../../src/common/ardrone.cpp \
    ../../src/ctrl/control.cpp \
    ../../src/meters/rollmeter.cpp \
    ../../src/meters/qmeter.cpp \
    ../../src/meters/pitchmeter.cpp \
    ../../src/meters/navigater.cpp \
    ../../src/meters/altimeter.cpp \
    ../../src/navdata/navdata.cpp \
    ../../src/navdata/navdata_update.cpp \
    ../../src/video/video.cpp

OTHER_FILES += \
    ../../src/resource/ardrone.rc \
    ../../src/resource/logo.ico \
    ../../src/resource/roll.png \
    ../../src/resource/yaw.png

RC_FILE += \
    ../../src/resource/ardrone.rc
