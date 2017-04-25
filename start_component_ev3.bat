cd /d %~dp0
set PATH=C:\Python27;%PATH%
start "" python Components/FloatSeqToVelocity/FloatSeqToVelocity.py -f Components/rtc.conf
start "" python Components/TkJoyStick/TkJoyStickComp.py -f Components/rtc.conf
