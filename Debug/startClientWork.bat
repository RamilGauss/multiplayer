FOR /L %%i IN (9001,1,9042) DO start client.exe Gauss%%i 192.168.23.226 %%i
#FOR /L %%i IN (9001,1,9032) DO start client.exe