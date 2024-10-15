library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;
use IEEE.std_logic_unsigned.all;

LIBRARY altera;
USE altera.altera_primitives_components.all;

entity conditioner is
	port(clock : in std_logic; 
	     reset : in std_logic;
             input : in std_logic;
	     output : out std_logic);
end entity;

architecture conditioner_arch of conditioner is
-- Component Sync
component sync is
	port(clock  	   : in std_logic;
	     input	   : in std_logic;
	     output        : out std_logic);
end component;
--Component Debouncer
component debouncer is
	generic (clock_period : integer := 4500000);

	port    (clock  : in std_logic;
		 reset  : in std_logic;
		 switch : in std_logic;
		 output : out std_logic);
end component;
--Component Onepulse
component onepulse is
	port    (clock  : in std_logic;
		 button : in std_logic;
		 reset  : in std_logic;
		 output : out std_logic);
end component;
	signal sync_out, deb_out : std_logic := '0';
begin
	sync1 : sync port map(clock => clock, input => input, output => sync_out);

	debounce: debouncer port map(switch => sync_out, clock => clock, reset => reset, output => deb_out);

	pulse : onepulse port map(button => deb_out, clock => clock, reset => reset, output => output);

end architecture;