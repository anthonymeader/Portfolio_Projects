library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;
use IEEE.std_logic_unsigned.all;

LIBRARY altera;
USE altera.altera_primitives_components.all;

entity debouncer is
	generic (clock_period : integer := 4500000);

	port    (clock  : in std_logic;
		 reset  : in std_logic;
		 switch : in std_logic;
		 output : out std_logic);

end entity;

architecture debouncer_arch of debouncer is
	type state_type is (idle, debouncing);
	signal current_state, next_state : state_type := idle;
	signal count : integer range 0 to clock_period := 0;
	begin
--STATE MEMORY
        STATE_MEMORY : process(clock, reset)
	begin
	if (rising_edge(clock)) then
	  if (reset = '1') then
		current_state <= idle;
	  else 
		current_state <= next_state;
	  end if;
	end if;
	end process;
--NEXT STATE LOGIC
	NEXT_STATE_LOGIC : process (current_state, switch, clock, reset)
	begin
	 if rising_edge(clock) then
	  case (current_state) is
	    when idle => if (switch = '1') then
			   next_state <= debouncing;
		         else
			   next_state <= idle;
			   count <= 0;
			 end if;
	    when debouncing => if (count = clock_period) then
				 if (switch = '1') then
			           next_state <= debouncing;
				 elsif (switch = '0') then
				   next_state <= idle;
				   count <= 0;
				 end if;
			       elsif (count < clock_period) then
				 count <= count + 1;
			       elsif (reset = '1') then
			         next_state <= idle;	
			         count <= 0;
			       end if;
	    when others => next_state <= idle;
	  end case;
	 end if;
	end process;
--OUTPUT LOGIC
	OUTPUT_LOGIC : process (current_state)
	begin
	  case (current_state) is
	    when idle => output <= '0';
	    when debouncing => output <= '1';
	    when others => output <= '0';
	  end case;
	end process;
			       
end architecture;






		

