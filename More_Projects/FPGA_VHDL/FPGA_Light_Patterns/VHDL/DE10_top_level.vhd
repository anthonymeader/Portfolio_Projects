-- SPDX-License-Identifier: MIT
-- Copyright (c) 2017 Ross K. Snider.  All rights reserved.
----------------------------------------------------------------------------
-- Description:  Top level VHDL file for the DE10-Nano
----------------------------------------------------------------------------
-- Author:       Ross K. Snider
-- Company:      Montana State University
-- Create Date:  September 1, 2017
-- Revision:     1.0
-- License: MIT  (opensource.org/licenses/MIT)
----------------------------------------------------------------------------

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;
use IEEE.std_logic_unsigned.all;

LIBRARY altera;
USE altera.altera_primitives_components.all;

-----------------------------------------------------------
-- Signal Names are defined in the DE10-Nano User Manual
-- http://de10-nano.terasic.com
-----------------------------------------------------------
 entity DE10_Top_Level is
	port(
		----------------------------------------
		--  CLOCK Inputs
		--  See DE10 Nano User Manual page 23
		----------------------------------------
		FPGA_CLK1_50  :  in std_logic;										--! 50 MHz clock input #1
		FPGA_CLK2_50  :  in std_logic;										--! 50 MHz clock input #2
		FPGA_CLK3_50  :  in std_logic;										--! 50 MHz clock input #3
		
		
		----------------------------------------
		--  Push Button Inputs (KEY) 
		--  See DE10 Nano User Manual page 24
		--  The KEY push button inputs produce a '0' 
		--  when pressed (asserted)
		--  and produce a '1' in the rest (non-pushed) state
		--  a better label for KEY would be Push_Button_n 
		----------------------------------------
		KEY : in std_logic_vector(1 downto 0);								--! Two Pushbuttons (active low)
		
		
		----------------------------------------
		--  Slide Switch Inputs (SW) 
		--  See DE10 Nano User Manual page 25
		--  The slide switches produce a '0' when
		--  in the down position 
		--  (towards the edge of the board)
		----------------------------------------
		SW  : in std_logic_vector(3 downto 0);								--! Four Slide Switches 
		
		
		----------------------------------------
		--  LED Outputs 
		--  See DE10 Nano User Manual page 26
		--  Setting LED to 1 will turn it on
		----------------------------------------
		LED : out std_logic_vector(7 downto 0);							--! Eight LEDs
		
		
		----------------------------------------
		--  GPIO Expansion Headers (40-pin)
		--  See DE10 Nano User Manual page 27
		--  Pin 11 = 5V supply (1A max)
		--  Pin 29 - 3.3 supply (1.5A max)
		--  Pins 12, 30 GND
		--  Note: the DE10-Nano GPIO_0 & GPIO_1 signals
		--  have been replaced by
		--  Audio_Mini_GPIO_0 & Audio_Mini_GPIO_1
		--  since some of the DE10-Nano GPIO pins
		--  have been dedicated to the Audio Mini
		--  plug-in card.  The new signals 
		--  Audio_Mini_GPIO_0 & Audio_Mini_GPIO_1 
		--  contain the available GPIO.
		----------------------------------------
		--GPIO_0 : inout std_logic_vector(35 downto 0);					--! The 40 pin header on the top of the board
		--GPIO_1 : inout std_logic_vector(35 downto 0);					--! The 40 pin header on the bottom of the board 
		Audio_Mini_GPIO_0 : inout std_logic_vector(33 downto 0);	--! 34 available I/O pins on GPIO_0
		Audio_Mini_GPIO_1 : inout std_logic_vector(12 downto 0)		--! 13 available I/O pins on GPIO_1 
	
		
	);
end entity DE10_Top_Level;


architecture DE10Nano_arch of DE10_Top_Level is
	component conditioner is
		port(clock  : in std_logic;
		     reset  : in std_logic;
		     input  : in std_logic;
		     output : out std_logic);
	end component;
	component LED_Patterns is
	port(clock 	     		: in std_logic;
	     reset 	     		: in std_logic;
	     PB    	     		: in std_logic;
	     SW    	     		: in std_logic_vector(3 downto 0);
	     HPS_LED_Control : in std_logic;
	     SYS_CLKs_Sec    : in std_logic_vector (31 downto 0);
	     Base_rate 	   : in std_logic_vector(7 downto 0);
	     LED_reg 	      : in std_logic_vector(7 downto 0);
	     LED 	         : out std_logic_vector(7 downto 0));	    
	end component;
	
	component sampleclock is
		port(clock : in std_logic;
		sample_clock : out std_logic);
	end component;
	
	signal pb_signal    : std_logic;
	signal HPS          : std_logic := '1';
	signal SYS_CLKs_Sec : std_logic_vector(31 downto 0) := "00000010111110101111000010000000";  --50 MIL IN BINARY
	signal Base_rate 	  : std_logic_vector(7 downto 0) := "00010000";
	signal LED_r        : std_logic_vector(7 downto 0) := "00000000";
	signal key0  	     :  std_logic;
	signal key1 		  :  std_logic;
	signal sample1		  : std_logic := '1';


	begin

	
	key0 <= not(KEY(0));
	key1 <= not(KEY(1));
	
	sample : sampleclock port map(clock => FPGA_CLK1_50, sample_clock => sample1);
	


	
end architecture DE10Nano_arch;




