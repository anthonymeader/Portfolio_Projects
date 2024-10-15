library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;
use IEEE.std_logic_unsigned.all;

entity onepulse_tb is
end entity;

architecture onepulse_tb_arc of onepulse_tb is
	component onepulse
		port    (clock  : in std_logic;
			 reset : in std_logic;
		 	 button : in std_logic;
		 	 output : out std_logic);
	end component;
	
	signal clock_tb, reset_tb, button_tb : std_logic := '0';

	signal output_tb : std_logic:= '0';
begin

	DUT1 : onepulse port map (clock => clock_tb,
				   reset => reset_tb,
				   button => button_tb,
				   output => output_tb);

	
	clock_tb <= not(clock_tb) after 10 ns;

STIMULUS : process
	begin
	button_tb <= '0'; reset_tb <= '0'; wait for 100 ns;
	button_tb <= '0'; reset_tb <= '1'; wait for 200 ns;
	button_tb <= '0'; reset_tb <= '0'; wait for 200 ns;
	button_tb <= '1'; reset_tb <= '0'; wait for 200 ns;

	button_tb <= '0'; reset_tb <= '0'; wait for 200 ns;
	button_tb <= '1'; reset_tb <= '1'; wait for 200 ns;

	  
	
end process;
	
end architecture;
