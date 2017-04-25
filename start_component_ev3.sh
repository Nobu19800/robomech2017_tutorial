#!/bin/sh
python Components/FloatSeqToVelocity/FloatSeqToVelocity.py -f Components/rtc.conf&
python Components/TkJoyStick/TkJoyStickComp.py -f Components/rtc.conf&
