library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;
use IEEE.std_logic_unsigned.all;

LIBRARY altera;
USE altera.altera_primitives_components.all;

entity sampleclock is
	port(clock : in std_logic;
        sample_clock   : out std_logic);
end entity;

architecture sampleclock of sampleclock is
	signal sample : std_logic := '1';
	signal counter : integer := 6250000;
	signal count : integer := 0;
	begin
	process(clock)
	begin 
	if rising_edge(clock) then
		if count < counter then
			count <= count + 1;
		elsif (count = counter) then
			sample <= not(sample);
			count <= 0;
		end if;
	end if;
	end process;
	sample_clock <= sample;

end architecture;