library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;
use IEEE.std_logic_unsigned.all;

LIBRARY altera;
USE altera.altera_primitives_components.all;

entity LED_Patterns is
	port(clock 	     : in std_logic;
	     reset 	     : in std_logic;
	     PB    	     : in std_logic;
	     SW    	     : in std_logic_vector(3 downto 0);
	     HPS_LED_Control : in std_logic;
	     SYS_CLKs_Sec    : in std_logic_vector (31 downto 0);
	     Base_rate 	     : in std_logic_vector(7 downto 0);
	     LED_reg 	     : in std_logic_vector(7 downto 0);
	     LED 	     : out std_logic_vector(7 downto 0)
	    );
end entity;

architecture LED_Patterns_arch of LED_Patterns is
	component rate is
	port(clock 	     : in std_logic;
	     reset 	     : in std_logic;
	     SW    	     : in std_logic_vector(3 downto 0);
	     SYS_CLKs_Sec    : in std_logic_vector (31 downto 0);
	     Base_rate 	     : in std_logic_vector(7 downto 0);
	     counter	     : out integer;
	     cnt0             : out integer;
		  cnt1             : out integer;
		  cnt2             : out integer;
		  cnt3             : out integer;
		  cnt4             : out integer;
		  cnt5				 : out integer);
	end component;

	component LED_States is
	port(clock : in std_logic;
	     reset : in std_logic;
	     PB    : in std_logic;
	     SW    : in std_logic_vector(3 downto 0);
	     cnt0   : in integer;
		  cnt1   : in integer;
		  cnt2   : in integer;
		  cnt3   : in integer;
		  cnt4   : in integer;
        LED_r   : out std_logic_vector(6 downto 0));
	end component;
	     	  
	signal count7 : integer := 0;
	signal count0 : integer := 0;
	signal count1 : integer := 0;
	signal count2 : integer := 0;
	signal count3 : integer := 0;
	signal count4 : integer := 0;
	signal led7count : integer := 0;
	signal one_second : std_logic := '0';
	signal LED_ph : std_logic_vector(6 downto 0);


begin

	rates : rate port map(clock => clock, reset => reset, SW => SW, SYS_CLKs_Sec => SYS_CLKs_Sec, Base_rate => Base_rate,
			      counter => count7, cnt0 => count0, cnt1 => count1, cnt2 => count2, cnt3 => count3, cnt4 => count4);
	patterns : LED_States port map(clock => clock, reset => reset, PB => PB, SW => SW, cnt0 => count0, cnt1 => count1,
											 cnt2 => count2, cnt3 => count3, cnt4 => count4, LED_r => LED_ph);

	LED7 : process (clock, reset)
	begin
	if (rising_edge(clock)) then
		if (led7count < count7) then
			led7count <= led7count + 1;
		elsif (led7count = count7) then
			led7count <= 0;
			one_second <= not one_second;
		elsif(led7count > count7) then
			led7count <= 0;
		end if;	
		if HPS_LED_Control = '1' then
			LED(7 downto 0) <= Led_Reg;
		elsif HPS_LED_Control = '0' then
			LED(7 downto 0) <= one_second & LED_ph;
		end if;
	end if;
	end process;

	end architecture;


	



	     

