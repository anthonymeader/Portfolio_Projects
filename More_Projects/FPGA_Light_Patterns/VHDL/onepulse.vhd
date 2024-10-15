library ieee;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;
use IEEE.std_logic_unsigned.all;

LIBRARY altera;
USE altera.altera_primitives_components.all;

entity onepulse is
	port    (clock  : in std_logic;
		 button : in std_logic;
		 reset  : in std_logic;
		 output : out std_logic);
end entity;

architecture onepulse_arch of onepulse is
	type state_type is (idle, pulse);
	signal state : state_type := idle;
	begin
	process(clock, reset)
	begin



	if(rising_edge(clock)) then
	 if (reset = '1') then
		state <= idle;
		output <= '0';
	 else
	    case state is
		when idle =>
		  if button = '1' then
		      state <= pulse;
		      output <= '1';
		  elsif (button = '0') then
			state <= idle;
			output <= '0';
		  end if;
		when pulse =>
		  if (button = '1') then
		    state <= pulse;
		    output <= '0';
	          else
		    state <= idle;
		    output <= '0';
		  end if;
		when others => output <= '0';
	    end case;
	   end if;

	  end if;
	 end process;
			
end architecture;