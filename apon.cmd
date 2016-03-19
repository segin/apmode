@echo off

netsh wlan set hostednetwork mode=allow ssid="AndroidAP" key="dong1234"
netsh wlan start hostednetwork
netsh wlan show hostednetwork


