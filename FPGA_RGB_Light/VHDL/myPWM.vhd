library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;
use IEEE.std_logic_unsigned.all;

LIBRARY altera;
USE altera.altera_primitives_components.all;

entity myPWM is  --period of 19 msec
	port(
		clock 	     : in std_logic;
		reset 	     : in std_logic;
		SYS_CLKs_sec : in std_logic_vector(31 downto 0);
		period	     : in std_logic_vector(20 downto 0); --weight 21 at fixed point 10
		duty_cycle   : in std_logic_vector(17 downto 0); --weight 18 fixed point 14 -- given in as .50 for now
		output 	     : out std_logic;
		int_out	     : out unsigned(86 downto 0));
		
end entity;

architecture myPWM_arch of myPWM is
	-- Period Signals
	signal p2dscalar    : std_logic_vector(15 downto 0) := "0000000000100001"; -- Divide by 1000 Scalar
	signal period_ms    : unsigned(36 downto 0) := (others => '0'); -- Period in Miliseconds
	signal periodClocks : unsigned(68 downto 0) := (others => '0'); -- System Clocks Per ms
	signal periodCount  : integer := 0; -- Counter Variable

	-- Duty Cycle Signals
	--signal duty_ms : unsigned(17 downto 0) := (others => '0');
	signal dutyClocks : unsigned(86 downto 0) := (others => '0');
	signal dutyCount : integer := 0;
	

	signal count1 : integer := 0;
	signal count2 : integer := 0;
	
begin
	process (clock, reset)
	begin
	if (rising_edge(clock)) then
	
	-- Assign PWM PERIOD
	  period_ms    <= (unsigned(period) * unsigned(p2dscalar));
	  periodClocks <= (unsigned(period_ms) * unsigned(SYS_CLKs_Sec));
	  periodCount  <=  to_integer(periodClocks(68 downto 25)); --fixed point
	  
	-- Assign Duty cycle   
	  dutyClocks <= (unsigned(duty_cycle) * unsigned(periodClocks));
	  dutyCount  <=  to_integer(dutyClocks(86 downto 39)); --fixed point
	  int_out <= dutyClocks;

	end if;
	end process;
	
	process (clock, reset)
	begin
	if (rising_edge(clock)) then
	
	 if (count1 < dutyCount) then
	   output <= '1';
	   count1 <= count1 + 1;
	   count2 <= count2 + 1;
	 elsif ((count1 >= dutyCount) and (count2 < periodCount)) then
	   output <= '0'; 
	   count2 <= count2 + 1;
	 else
		output <= '0';
		count1 <= 0;
		count2 <= 0;
	 end if;
	 
	end if;
	end process;

end architecture;
