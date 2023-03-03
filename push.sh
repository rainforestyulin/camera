#!/bin/bash
scp  ../GxTest/GxTest.cc  root@172.20.30.92:/data/yulin/Galaxy_Linux-x86_Gige-U3_32bits-64bits_1.3.2206.9161/Galaxy_camera/sample/GxTest
scp  ../GxTest/GxTest.h  root@172.20.30.92:/data/yulin/Galaxy_Linux-x86_Gige-U3_32bits-64bits_1.3.2206.9161/Galaxy_camera/sample/GxTest
#scp -r  ../GxTest/stb  root@172.20.30.92:/data/yulin/Galaxy_Linux-x86_Gige-U3_32bits-64bits_1.3.2206.9161/Galaxy_camera/sample/GxTest
scp  ../GxTest/DHCameraControl.c  root@172.20.30.92:/data/yulin/Galaxy_Linux-x86_Gige-U3_32bits-64bits_1.3.2206.9161/Galaxy_camera/sample/GxTest
scp  ../GxTest/DHCameraControl.h  root@172.20.30.92:/data/yulin/Galaxy_Linux-x86_Gige-U3_32bits-64bits_1.3.2206.9161/Galaxy_camera/sample/GxTest
scp  ../GxTest/CameraParam.h root@172.20.30.92:/data/yulin/Galaxy_Linux-x86_Gige-U3_32bits-64bits_1.3.2206.9161/Galaxy_camera/sample/GxTest
scp  ../GxTest/Makefile root@172.20.30.92:/data/yulin/Galaxy_Linux-x86_Gige-U3_32bits-64bits_1.3.2206.9161/Galaxy_camera/sample/GxTest
scp  ../GxTest/APITest.c root@172.20.30.92:/data/yulin/Galaxy_Linux-x86_Gige-U3_32bits-64bits_1.3.2206.9161/Galaxy_camera/sample/GxTest
#rsync -av -e ssh --exclude='.ccls-cache' ./  industai@172.20.50.34:/home/industai/Downloads/Galaxy_Linux-x86_Gige-U3_32bits-64bits_1.3.2206.9161/Galaxy_camera/sampl/GxTeste

