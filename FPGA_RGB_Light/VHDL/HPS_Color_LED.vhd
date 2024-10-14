library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;
use IEEE.std_logic_unsigned.all;

LIBRARY altera;
USE altera.altera_primitives_components.all;

entity HPS_Color_LED is
	port(clock            : in std_logic;
		  reset            : in std_logic;
		  avs_s1_read      : in std_logic;
		  avs_s1_write     : in std_logic;
		  avs_s1_address   : in std_logic_vector(2 downto 0);
		  avs_s1_readdata  : out std_logic_vector(31 downto 0);
		  avs_s1_writedata : in std_logic_vector(31 downto 0);
		  red_output       : out std_logic;
		  green_output		 : out std_logic;
		  blue_output      : out std_logic;
		  LED					 : out std_logic_vector(7 downto 0);
		  SW					 : in std_logic_vector(3 downto 0));
end entity;

architecture HPS_Color_LED_arch of HPS_Color_LED is
	component myPWM is
	port(clock 	      : in std_logic;
	     reset 	      : in std_logic;
	     SYS_CLKs_sec : in std_logic_vector(31 downto 0);
	     period	      : in std_logic_vector(20 downto 0); --weight 21 at fixed point 10
	     duty_cycle   : in std_logic_vector(17 downto 0); --weight 18 fixed point 14 -- given in as .50 for now
		  int_out		: out unsigned(86 downto 0);
	     output 	   : out std_logic);
	end component;
	
	component percent is
	port(clock 	     : in std_logic;
	     reset 	     : in std_logic;
	     SW    	     : in std_logic_vector(3 downto 0);
		  SW_Reg	     : in std_logic_vector(3 downto 0);
	     count 	     : in unsigned(86 downto 0);
		  count0 	  : in unsigned(86 downto 0);
		  count1      : in unsigned(86 downto 0);
	     LED 	     : out std_logic_vector(7 downto 0));
	end component;
	
	
	signal Led_red      : std_logic_vector(31 downto 0) := "00000000000000000010000000000000"; --duty cycle value
	signal Led_green    : std_logic_vector(31 downto 0) := "00000000000000000010000000000000"; --duty cycle value
	signal Led_blue     : std_logic_vector(31 downto 0) := "00000000000000000010000000000000"; --duty cycle value 50%
	signal period       : std_logic_vector(31 downto 0) := "00000000000000000100110000000000"; -- period value 19ms
	signal SYS_CLKs_sec : std_logic_vector(31 downto 0) := "00000010111110101111000010000000"; -- 50M
	signal SW_Reg		  : std_logic_vector(31 downto 0) := "00000000000000000000000000000000"; -- Percentage Selector
	signal int_1 : unsigned(86 downto 0) := (others => '0');
	signal int_2 : unsigned(86 downto 0) := (others => '0');
	signal int_3 : unsigned(86 downto 0) := (others => '0');


	--signal pb_signal    : std_logic;


	

	begin
	
	PWM_Controller_1 : myPWM port map (clock => clock, reset => reset, SYS_CLKs_sec => SYS_CLKs_sec,
												  period=> period(20 downto 0), duty_cycle => Led_red(17 downto 0),
												  output => red_output, int_out => int_1);
												  
	PWM_Controller_2 : myPWM port map (clock => clock, reset => reset, SYS_CLKs_sec => SYS_CLKs_sec,
												  period=> period(20 downto 0), duty_cycle => Led_green(17 downto 0),
												  output => green_output, int_out => int_2);
												  
	PWM_Controller_3 : myPWM port map (clock => clock, reset => reset, SYS_CLKs_sec => SYS_CLKs_sec,
												  period=> period(20 downto 0), duty_cycle => Led_blue(17 downto 0),
												  output => blue_output, int_out => int_3);
												  
	Percent_Led		  : percent port map (clock => clock, reset => reset, SW(3 downto 0) => SW(3 downto 0), SW_Reg(3 downto 0) => SW_Reg(3 downto 0), 
													 LED(7 downto 0) => LED(7 downto 0), count => int_1, count0 => int_2, count1 => int_3);
	
	--condition : conditioner port map (clock => clock,   reset =>   reset,  input =>  PB, output => pb_signal);
	--LED_Pat   : LED_Patterns port map(clock => clock,   reset =>   reset, PB => pb_signal, SW => SW, HPS_LED_Control => HPS(0),
						  					    --SYS_CLKs_Sec => SYS_CLKs_Sec, Base_rate => Base_rate(7 downto 0), LED_reg => LED_r(7 downto 0), LED => LED);
	
	avalon_register_read : process(clock)
	begin
	if rising_edge(clock) and avs_s1_read = '1' then
		case avs_s1_address is
			when "000"  => avs_s1_readdata <= Led_red;
			when "001"  => avs_s1_readdata <= Led_green;
			when "010"  => avs_s1_readdata <= Led_blue;
			when "011"  => avs_s1_readdata <= SW_Reg;
		   when "100"  => avs_s1_readdata <= period;
			when "101"  => avs_s1_readdata <= SYS_CLKs_sec;
			when others => avs_s1_readdata <= (others => '0');
		end case;
	end if;
	end process;
	
	avalon_register_write : process(clock, reset)
	begin
		if reset = '1' then
			Led_red      <= "00000000000000000010000000000000";
			Led_green    <= "00000000000000000010000000000000";
			Led_blue     <= "00000000000000000010000000000000";
			period       <= "00000000000000000100110000000000";
		   SYS_CLKs_sec <= "00000010111110101111000010000000";
			SW_Reg       <= "00000000000000000000000000000000";
		elsif rising_edge(clock) and avs_s1_write = '1' then
			case avs_s1_address is
				when "000"  => Led_red      <= avs_s1_writedata(31 downto 0);
				when "001"  => Led_green    <= avs_s1_writedata(31 downto 0);
				when "010"  => Led_blue     <= avs_s1_writedata(31 downto 0);
				when "011"  => SW_Reg       <= avs_s1_writedata(31 downto 0);
				when "100"  => period 	    <= avs_s1_writedata(31 downto 0);
				when "101"  => SYS_CLKs_sec <= avs_s1_writedata(31 downto 0);
				when others => null;
			end case;
		end if;
	end process;


end architecture;
