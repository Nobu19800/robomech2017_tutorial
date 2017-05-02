cd /d %~dp0
set PATH=C:\Python27;%PATH%
start "" Components\dist\FloatSeqToVelocity.exe -f Components/rtc.conf
start "" Components\dist\TkJoyStickComp.exe -f Components/rtc.conf
