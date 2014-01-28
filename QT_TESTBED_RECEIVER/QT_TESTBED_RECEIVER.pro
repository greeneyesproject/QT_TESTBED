#-------------------------------------------------
#
# Project created by QtCreator 2014-01-13T10:22:29
#
#-------------------------------------------------

QT       += core gui

TARGET = QT_TESTBED_RECEIVER
TEMPLATE = app

SOURCES += ../testbed_framework/*.cpp \
           ../testbed_framework/*.c \
           ../testbed_framework/thirdparty/agast/src/*.cc \
           ../testbed_framework/thirdparty/coder/*.cpp \
           brisk_with_lookup.cpp \
           inverse_BRISK.cpp \
           main.cpp\
           receivergui.cpp \
           display.cpp \
           receivermanager.cpp

HEADERS  += receivergui.h \
            display.h \
            receivermanager.h \
            ui_display.h \            
            ../testbed_framework/includes/VisualFeatureExtraction.h \
            ../testbed_framework/includes/VisualFeatureEncoding.h \
            ../testbed_framework/includes/VisualFeatureDecoding.h \
            ../testbed_framework/includes/utilities.hpp \
            ../testbed_framework/includes/sort_like_matlab.h \
            ../testbed_framework/includes/serialsource.h \
            ../testbed_framework/includes/serialprotocol.h \
            ../testbed_framework/includes/objRecognition.h \
            ../testbed_framework/includes/ImageAcquisition.h \
            ../testbed_framework/includes/DetDescParams.h \
            ../testbed_framework/includes/DataTransmission.h \
            ../testbed_framework/includes/CodecParams.h \
            inverse_BRISK.h \
            include/brisk/brisk.h

FORMS    += receivergui.ui \
            display.ui

OBJECTS_DIR = obj

INCLUDEPATH += /home/tony/OpenCV/2.4.8/include \
               ../testbed_framework \
               ../testbed_framework/includes \
               ../testbed_framework/thirdparty/agast \
               ../testbed_framework/thirdparty/agast/include \
               ../testbed_framework/thirdparty/agast/include/agast \
               Eigen \
               include

LIBS += -L/home/tony/OpenCV/2.4.8/lib \
        -lopencv_core \
        -lopencv_highgui \
        -lopencv_imgproc \
        -lopencv_features2d \
        -lopencv_nonfree \
        -lopencv_video \
        -lopencv_calib3d
