#!/bin/bash
bass=/sys/devices/platform/ff207900.bandpassEQ_0/gain_1
lowmids=/sys/devices/platform/ff207900.bandpassEQ_0/gain_2
highmids=/sys/devices/platform/ff207900.bandpassEQ_0/gain_3
presence=/sys/devices/platform/ff207900.bandpassEQ_0/gain_4
brilliance=/sys/devices/platform/ff207900.bandpassEQ_0/gain_5
enable=/sys/devices/platform/ff207900.bandpassEQ_0/passthrough

while true
do
    echo 0x0 > $enable
    sleep 0.25
    echo 0x100 > $bass
    sleep 0.25
    echo 0x4000 > $lowmids
    sleep 0.25
    echo 0x6000 > $highmids
    sleep 0.25
    echo 0x0 > $presence
    sleep 0.25
    echo 0x0 > $brilliance
    sleep 0.25
done
