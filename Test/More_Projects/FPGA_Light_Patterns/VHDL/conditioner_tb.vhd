library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;
use IEEE.std_logic_unsigned.all;

entity conditioner_tb is
end entity;

architecture conditioner_tb_arch of conditioner_tb is
	component conditioner
		port    (clock  : in std_logic;
		 	 reset  : in std_logic;
		 	 input : in std_logic;
		 	 output : out std_logic);
	end component;
	
	signal clock_tb, reset_tb, switch_tb : std_logic := '0';

	signal output_tb : std_logic:= '0';
begin

	DUT1 : conditioner port map (clock => clock_tb,
				   reset => reset_tb,
				   input => switch_tb,
				   output => output_tb);

	
	clock_tb <= not(clock_tb) after 10 ns;

STIMULUS : process
	begin
	switch_tb <= '0'; reset_tb <= '0'; wait for 1 ms;
	switch_tb <= '1'; reset_tb <= '0'; wait for 1 ms;
	switch_tb <= '0'; reset_tb <= '0'; wait for 250 ms;

	switch_tb <= '0'; wait for 1 ms;
	switch_tb <= '1'; wait for 3 ms;
	switch_tb <= '0'; wait for 5 ms;
	switch_tb <= '1'; wait for 7 ms;
	switch_tb <= '0'; wait for 5 ms;

	switch_tb <= '0'; reset_tb <= '0'; wait for 150 ms;

	switch_tb <= '1'; reset_tb <= '0'; wait for 1 ms;
	switch_tb <= '0'; reset_tb <= '0'; wait for 30 ms;
	switch_tb <= '0'; reset_tb <= '1'; wait for 50 ms;

	--switch_tb <= '1'; reset_tb <= '1'; wait for 20 ms;

	switch_tb <= '0'; reset_tb <= '0'; wait for 50 ms;
	switch_tb <= '1'; wait for 150 ms;

	
	
	

	  
	
end process;
	
end architecture;