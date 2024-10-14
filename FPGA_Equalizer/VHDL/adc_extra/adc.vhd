library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;
use IEEE.std_logic_unsigned.all;

LIBRARY altera;
USE altera.altera_primitives_components.all;

entity adc is
	generic (clock_period : integer := 100);
	
	port(clock               : in  std_logic;
	     reset               : in  std_logic;
	     config_reg          : in  std_logic_vector(15 downto 0);
	     SDO	         : in  std_logic;
	     SDI                 : out std_logic; 
	     SCLK	         : out std_logic; -- generates when shifting bits out
	     convst              : out std_logic;
	     data_register_index : out std_logic_vector(3 downto 0);
	     data_out            : out std_logic_vector(11 downto 0)
	     );
end entity;

architecture adc_arch of adc is
	signal start      : std_logic; --convst enable
	signal counter 	   : integer range 0 to 23 := 0;
	signal clock_start : std_logic;
	signal SCLK_Buffer : std_logic := '0';

	signal clock_count : integer range 0 to 22 := 0;
	signal config_bit  : integer := 0;
	signal count_1    : integer range 0 to clock_period := 0;
	signal count_2    : std_logic_vector(3 downto 0);
	
	signal SDI_con : std_logic_vector(5 downto 0);
	signal data_buff : std_logic_vector(11 downto 0) := "000000000000";

	type state_type is (waiting, save_count_2, walk_config, configure, clock_pulse);
	signal current_state, next_state, previous_state : state_type := waiting;
	
	begin

	CounterProcess: process(clock)
	begin
		if reset ='1' then
			count_1 <= 0;
		elsif rising_edge(clock) then
			if count_1 < 100 then
				if count_1 = 0 then
					clock_start <= '0';
					convst <= '1';
					start <= '1';
				elsif count_1 = 61 then
					clock_start <= '1';
					start <= '0';
					convst <= '0';
				else
					clock_start <= '0';
					convst <= '0';
					start <= '0';
				end if;
				count_1 <= count_1 + 1;
			elsif count_1 >= 100 then
				count_1 <= 0;

			end if;
		end if;
	end process;
			
	SDO_Proc : process(SCLK_Buffer)
	begin
		if rising_edge(SCLK_Buffer) then
			data_buff <= data_buff(10 downto 0) & SDO;
			
		end if;
		
	end process;	

	STATE_MEMORY : process(clock)
	begin
		if (reset = '1') then
			current_state <= waiting;
		elsif rising_edge(clock) then
			current_state <= next_state;
		end if;
	end process;
	
	NEXT_STATE_LOGIC : process(current_state, clock ,reset)
	  begin
	    if (rising_edge(clock)) then
		case (current_state) is
			when waiting =>
				if (start = '1') then
					data_out <= data_buff;
					data_register_index <= count_2;
					next_state <= walk_config;
				end if;
				if (clock_start = '1') then
					--if previous_state = walk_config then
						SCLK_Buffer <= '0';
					    	counter <= 0;
						next_state <= clock_pulse;
					--end if;
				end if;

			when walk_config =>
			 --count_2 <= "0000";
			 if (config_reg(config_bit) = '1') then
				count_2 <= std_logic_vector(to_unsigned(config_bit,4));
				next_state <= waiting;
				case config_bit is
					when 0 => SDI_con <= "000010"; --r0
					when 1 => SDI_con <= "100010"; --r8
					when 2 => SDI_con <= "000110"; --r1
					when 3 => SDI_con <= "100110"; --r9
					when 4 => SDI_con <= "001010"; --r2
					when 5 => SDI_con <= "101010"; --r10
					when 6 => SDI_con <= "001110"; --r3
					when 7 => SDI_con <= "101110"; --r11
					when 8 => SDI_con <= "010010"; --r4
					when 9 => SDI_con <= "110010"; --r12
					when 10 => SDI_con <= "010110"; --r5
					when 11 => SDI_con <= "110110"; --r13
					when 12 => SDI_con <= "011010";  --r6
					when 13 => SDI_con <= "111010"; --r14
					when 14 => SDI_con <= "011110"; --r7
					when 15 => SDI_con <= "111110"; --r15
					when others => SDI_con <= "000010";
				end case;
				--config_bit <= config_bit + 1;

				end if;
				if (config_bit >= 15) then
					config_bit <= 0;
			    else
					config_bit <= config_bit + 1;
				end if;
				
				

			when clock_pulse =>
				if counter < 23 then

				   SCLK_Buffer <= not SCLK_Buffer;
				   SCLK <= SCLK_Buffer;
				   if (SCLK_Buffer = '1') then
				   	SDI <= SDI_con(5);
				   	SDI_con <= SDI_con(4 downto 0) & '0';
				   end if;
				   counter <= counter + 1;
				elsif counter >= 21 then
				  SCLK_Buffer <= not SCLK_Buffer;
				  SCLK <= SCLK_Buffer;
				  counter <= 0;
				  next_state <= waiting;
				  
				end if;					
	    		when others => next_state <= waiting;
		end case;		
	    end if;	
	end process;

end architecture;

				--case(config_bit) is
					--when 0 => 
			-- for i in 0 to 15 loop
				--if config_reg(i) = '1' then
					--count_2 <= std_logic_vector(to_unsigned(i,4));
					--config_bit <= to_integer(unsigned(count_2(3 downto 0)));
					--exit;
				--end if;
			-- end loop;
			-- next_state <= configure;
			--when configure =>

			--if (config_bit >= 15) then
				--	config_bit <= 0;
			    --else
				--	config_bit <= config_bit + 1;
				--end if;
				--if (next_state = waiting) then
					--config_bit <= config_bit;
				--else
				--end if;
				--config_bit <= config_bit + 1;

				--if (config_bit >= 15) then
				--	config_bit <= 0;
				--end if;

				
					--if (config_bit < 15) then
					--	config_bit <= config_bit + 1;
					--else
					--	config_bit <= 0;
					--end if;
				--end if;
				
				--next_state <= waiting;
