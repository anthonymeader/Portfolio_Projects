library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;
use IEEE.std_logic_unsigned.all;

LIBRARY altera;
USE altera.altera_primitives_components.all;

entity HPS_LED_patterns is
	port(clock            : in std_logic;
		  reset            : in std_logic;
		  avs_s1_read      : in std_logic;
		  avs_s1_write     : in std_logic;
		  avs_s1_address   : in std_logic_vector(1 downto 0);
		  avs_s1_readdata  : out std_logic_vector(31 downto 0);
		  avs_s1_writedata : in std_logic_vector(31 downto 0);
		  PB               : in std_logic;
		  SW               : in std_logic_vector(3 downto 0);
		  LED              : out std_logic_vector(7 downto 0));
end entity;

architecture HPS_LED_patters_arch of HPS_LED_patterns is
component conditioner is
		port(clock  : in std_logic;
		     reset  : in std_logic;
		     input  : in std_logic;
		     output : out std_logic);
	end component;
	component LED_Patterns is
	port(clock 	     		: in std_logic;
	     reset 	     		: in std_logic;
	     PB    	     		: in std_logic;
	     SW    	     		: in std_logic_vector(3 downto 0);
	     HPS_LED_Control : in std_logic;
	     SYS_CLKs_Sec    : in std_logic_vector (31 downto 0);
	     Base_rate 	   : in std_logic_vector(7 downto 0);
	     LED_reg 	      : in std_logic_vector(7 downto 0);
	     LED 	         : out std_logic_vector(7 downto 0));	    
	end component;
	

	
	signal HPS  : std_logic_vector(31 downto 0)         := "00000000000000000000000000000001";
	signal Base_rate : std_logic_vector(31 downto 0)    := "00000000000000000000000000010000";
	signal SYS_CLKs_Sec : std_logic_vector(31 downto 0) := "00000010111110101111000010000000";
	signal LED_r : std_logic_vector(31 downto 0)        := "00000000000000000000000000000000";
	
	signal pb_signal    : std_logic;


	
	


	begin
	condition : conditioner port map (clock => clock,   reset =>   reset,  input =>  PB, output => pb_signal);
	LED_Pat   : LED_Patterns port map(clock => clock,   reset =>   reset, PB => pb_signal, SW => SW, HPS_LED_Control => HPS(0),
						  					    SYS_CLKs_Sec => SYS_CLKs_Sec, Base_rate => Base_rate(7 downto 0), LED_reg => LED_r(7 downto 0), LED => LED);
	
	avalon_register_read : process(clock)
	begin
	if rising_edge(clock) and avs_s1_read = '1' then
		case avs_s1_address is
			when "00" => avs_s1_readdata <= HPS;
			when "01" => avs_s1_readdata <= Base_rate;
			when "10" => avs_s1_readdata <= SYS_CLKs_Sec;
			when "11" => avs_s1_readdata <= LED_r;
			when others => avs_s1_readdata <= (others => '0');
		end case;
	end if;
	end process;
	
	avalon_register_write : process(clock, reset)
	begin
		if reset = '1' then
			HPS    <= "00000000000000000000000000000001";
			Base_rate  <= "00000000000000000000000000010000";
			SYS_CLKs_Sec <= "00000010111110101111000010000000";
			LED_r   <= "00000000000000000000000000000000";
		elsif rising_edge(clock) and avs_s1_write = '1' then
			case avs_s1_address is
				when "00" => HPS    <= avs_s1_writedata(31 downto 0);
				when "01" => Base_rate  <= avs_s1_writedata(31 downto 0);
				when "10" => SYS_CLKs_Sec <= avs_s1_writedata(31 downto 0);
				when "11" => LED_r   <= avs_s1_writedata(31 downto 0);
				
			end case;
		end if;
	end process;


end architecture;