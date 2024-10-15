library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;
use IEEE.std_logic_unsigned.all;

LIBRARY altera;
USE altera.altera_primitives_components.all;

entity percent is
	port(clock 	     : in std_logic;
	     reset 	     : in std_logic;
	     SW    	     : in std_logic_vector(3 downto 0);
             SW_Reg	     : in std_logic_vector(3 downto 0);
	     count 	     : in unsigned(86 downto 0);
	     count0 	  : in unsigned(86 downto 0);
	     count1      : in unsigned(86 downto 0);
	     LED 	     : out std_logic_vector(7 downto 0)
	    );
end entity;

architecture percent_arch of percent is
	signal counter : integer := 0;
	signal store   : unsigned(86 downto 0)  := (others => '0');
	signal average : unsigned(102 downto 0) := (others => '0');
	signal third   : unsigned(15 downto 0)  := "0101010101010101";

	begin

	counting : process (clock,reset)
	begin
	if (rising_edge(clock)) then
		if (SW = "0000") and (SW_Reg = "000") then
			store <= (unsigned(count) + unsigned(count0) + unsigned(count1));
			average <= (unsigned(store) * unsigned(third));
			counter <= to_integer(average(102 downto 55));
		elsif (SW = "0001") and (SW_Reg = "000") then
			counter <= to_integer(count(86 downto 39));
		elsif (SW = "0010") and (SW_Reg = "000") then
			counter <= to_integer(count0(86 downto 39));
		elsif (SW = "0011") and (SW_Reg = "000") then
			counter <= to_integer(count1(86 downto 39));
		elsif (SW_Reg = "0001") then
			counter <= to_integer(count(86 downto 39));
		elsif (SW_Reg = "0010") then
			counter <= to_integer(count0(86 downto 39));
		elsif (SW_Reg = "0011") then
			counter <= to_integer(count1(86 downto 39));
		else
			counter <= 0;
		end if;
	end if;
	end process;
	
	LEDs: process (clock, reset)
	begin
	if (rising_edge(clock)) then
		if (counter > 0) and (counter <= 118850) then
			LED(7 downto 0) <= "00000000";
		elsif (counter > 118850) and (counter <= 237500) then
			LED(7 downto 0) <= "00000001";
		elsif (counter > 237500) and (counter <= 356250) then
			LED(7 downto 0) <= "00000011";
		elsif (counter > 356250) and (counter <= 475000) then
			LED(7 downto 0) <= "00000111";
		elsif (counter > 475000) and (counter <= 593750) then
			LED(7 downto 0) <= "00001111";
		elsif (counter > 593750) and (counter <= 712500) then
			LED(7 downto 0) <= "00011111";
		elsif (counter > 712500) and (counter <= 831250) then
			LED(7 downto 0) <= "00111111";
		elsif (counter > 831250) and (counter <= 950000) then
			LED(7 downto 0) <= "01111111";
		elsif (counter >950000) then
			LED(7 downto 0) <= "11111111";
		else
			LED(7 downto 0) <= "00000000";
		end if;
			
	end if;
	end process;
		

	end architecture;


	



	     

