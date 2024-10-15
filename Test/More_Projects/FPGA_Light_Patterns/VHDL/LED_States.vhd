
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;
use IEEE.std_logic_unsigned.all;

LIBRARY altera;
USE altera.altera_primitives_components.all;

entity LED_States is
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
  end entity;

architecture LED_States_arch of LED_States is


	type state_type is (rotating, LEDo, transition, tworotate, up_counter, down_counter, person_shift);
	signal current_state, next_state, previous_state : state_type := LEDo;
	
	attribute keep : boolean;
	attribute preserve : string;
	attribute keep of next_state : signal is true;
	attribute preserve of next_state : signal is "yes";
	
	signal ledr : std_logic_vector(6 downto 0);
	signal inc : integer := 0;
	signal inc2 : integer := 0;
	signal count : integer := 0;
	
	--LED vector and counter for transition
	signal display : std_logic_vector(3 downto 0);
	signal trscount1 : integer := 0;
	
	
	begin
	STATE_MEMORY : process (clock, reset)
	begin
	if (rising_edge(clock)) then
		if (reset = '1') then
			current_state <= LEDo;
		else
			current_state <= next_state;
		end if;
	end if;
	end process;

	NEXT_STATE_LOGIC : process (current_state, clock, reset)
	begin
	if rising_edge(clock) then
	  case (current_state) is 
	    when LEDo => if (reset = '0') then
						    if (PB = '1') then 
								inc2 <= 0;
							   inc <= 0;
								count <= 0;
								next_state <= transition;
							 else
								next_state <= rotating;
							 end if;
						  else
							   next_state <= LEDo;
								count <= 0;
								inc <= 0;
								inc2 <= 0;
						  end if;
						
	    when rotating => if (PB = '1') then
								  inc <= 0;
								  count <= 0;
								  next_state <= transition;
								elsif (count < cnt0 ) then
									count <= count + 1;
									--inc <= 0;
								  elsif (count = cnt0) then
									if (inc = 0) then
										ledr <= "0000001";
										inc <= inc + 1;
										
									elsif (inc = 1) then
											 ledr <= "0000010";
											 inc <= inc + 1;
									elsif (inc = 2) then
											 ledr <= "0000100";
											 inc <= inc + 1;
									elsif (inc = 3) then
											 ledr <= "0001000";
											 inc <= inc + 1;
									elsif (inc = 4) then
											 ledr <= "0010000";
											 inc <= inc + 1;
									elsif (inc = 5) then
											 ledr <= "0100000";
											 inc <= inc + 1;
									elsif (inc = 6) then
											 ledr <= "1000000";
											 inc <= 0;
									end if;
									count <= 0;							
								if reset = '1' then
									next_state <= LEDo;
								else
									next_state <= rotating;
									previous_state <= rotating;
								end if;
								
								
									
								end if;
		when tworotate => if (PB = '1') then
								  inc <= 0;
								  count <= 0;
								  next_state <= transition;
								elsif (count < cnt1) then
									count <= count + 1;
								elsif (count = cnt1) then
									if (inc = 0) then
										ledr <= "0000011";
										inc <= inc+1;
									elsif (inc = 1) then
											 ledr <= "0000110";
											 inc <= inc + 1;
									elsif (inc = 2) then
											 ledr <= "0001100";
											 inc <= inc + 1;
									elsif (inc = 3) then
											 ledr <= "0011000";
											 inc <= inc + 1;
									elsif (inc = 4) then
											 ledr <= "0110000";
											 inc <= inc + 1;
									elsif (inc = 5) then
											 ledr <= "1100000";
											 inc <= inc + 1;
									elsif (inc = 6) then
											 ledr <= "1000001";	
											 inc <= 0;
									end if;
									count <= 0;
									next_state <= tworotate;
									previous_state <= tworotate;
								elsif (reset = '1') then
									 next_state <= LEDo;
								end if;
								
		when up_counter => if (PB = '1') then
								  ledr <= "0000000";
								  count <= 0;
								  next_state <= transition;
								elsif (count < cnt2) then
									count <= count + 1;
								elsif (count = cnt2) then
									ledr <= ledr + x"1";

									count <= 0;
									next_state <= up_counter;
									previous_state <= up_counter;
								elsif (reset = '1') then
									 next_state <= LEDo;
								end if;
		when down_counter => if (PB = '1') then
								  ledr <= "0000000";
								  count <= 0;
								  next_state <= transition;
								elsif (count < cnt3) then
									count <= count + 1;
								elsif (count = cnt3) then
									ledr <= ledr - x"1";

									count <= 0;
									next_state <= down_counter;
									previous_state <= down_counter;
								elsif (reset = '1') then
									 next_state <= LEDo;
								end if;
	    when person_shift => if (PB = '1') then
								  inc2 <= 0;
								  count <= 0;
								  next_state <= transition;
								elsif (count < cnt4 ) then
									count <= count + 1;
								  elsif (count = cnt4) then
									if (inc2 = 0) then
										ledr <= "0000001";
										inc2 <= inc2 + 1;
										
									elsif (inc2 = 1) then
											 ledr <= "0000100";
											 inc2 <= inc2 + 1;
									elsif (inc2 = 2) then
											 ledr <= "0000010";
											 inc2 <= inc2 + 1;
									elsif (inc2 = 3) then
											 ledr <= "0001000";
											 inc2 <= inc2 + 1;
									elsif (inc2 = 4) then
											 ledr <= "0000100";
											 inc2 <= inc2 + 1;
									elsif (inc2 = 5) then
											 ledr <= "0010000";
											 inc2 <= inc2 + 1;
									elsif (inc2 = 6) then
											 ledr <= "0001000";
											 inc2 <= inc2 + 1;
									elsif (inc2 = 7) then
											 ledr <= "0100000";
											 inc2 <= inc2 + 1;
									elsif (inc2 = 8) then
											 ledr <= "0010000";
											 inc2 <= inc2 + 1;
									elsif (inc2 = 9) then
											 ledr <= "1000000";
											 inc2 <= 0;
									end if;
									count <= 0;							
									
									next_state <= person_shift;
									previous_state <= person_shift;
								elsif (reset = '1') then
									 next_state <= LEDo;
								end if;

		-- TRANSITION
		when transition => if (trscount1 < 50000000) then
									ledr <= "000" & SW(3 downto 0);
									trscount1 <= trscount1 + 1;
								 elsif (trscount1 = 50000000) then
										trscount1 <= 0;
										if (SW = "0000") then
											next_state <= rotating;
										elsif (SW = "0001") then
											next_state <= tworotate;
										elsif (SW = "0010") then
											next_state <= up_counter;
										elsif (SW = "0011") then
											next_state <= down_counter;
										elsif (SW = "0100") then
											next_state <= person_shift;
										else
											next_state <= previous_state;
									end if;
									end if;
								  
							end case;
						end if;		
				end process;


	OUTPUT_LOGIC : process(current_state)
	begin
	  case (current_state) is
	    when LEDo => LED_r     <= "0000000";
	    when rotating => LED_r     <= ledr;
		 when tworotate => LED_r     <= ledr;
		 when transition => LED_r     <= ledr;
		 when up_counter => LED_r <= ledr;
		 when down_counter => LED_r <= ledr;
		 when person_shift => LED_r <= ledr;
	  end case;
	end process;
end architecture;
	