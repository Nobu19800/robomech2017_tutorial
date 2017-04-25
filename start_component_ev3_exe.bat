cd /d %~dp0
set PATH=C:\Python27;%PATH%
start "" python Components/FloatSeqToVelocity/FloatSeqToVelocity.bat -f Components/rtc.conf
start "" python Components/TkJoyStick/TkJoyStickComp.bat -f Components/rtc.conf
