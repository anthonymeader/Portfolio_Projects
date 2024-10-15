#!/bin/bash
CH2=/sys/devices/platform/ff200020.ADC/ch2
CH3=/sys/devices/platform/ff200020.ADC/ch3
CH4=/sys/devices/platform/ff200020.ADC/ch4

RED=/sys/devices/platform/ff200000.HPS_Color_LED/red
GREEN=/sys/devices/platform/ff200000.HPS_Color_LED/green
BLUE=/sys/devices/platform/ff200000.HPS_Color_LED/blue
while true
do
	ch2_r=$(cat "$CH2")	
	ch2_shift=$((ch2_r << 2))
	echo $ch2_shift > $RED
	sleep .10
	cat $RED

	ch3_r=$(cat "$CH3");
	ch3_shift=$((ch3_r << 2))
	echo $ch3_shift > $GREEN
	sleep .10
	cat $GREEN

	ch4_r=$(cat "$CH4");
	ch4_shift=$((ch4_r << 2))
	echo $ch4_shift > $BLUE
	sleep .10
	cat $BLUE
done
	
