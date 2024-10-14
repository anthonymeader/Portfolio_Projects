library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;
use IEEE.std_logic_unsigned.all;

LIBRARY altera;
USE altera.altera_primitives_components.all;

entity HPS_adc is
	port(clock            : in std_logic;
	     reset            : in std_logic;
	     avs_s1_read      : in std_logic;
	     avs_s1_write     : in std_logic;
	     avs_s1_address   : in std_logic_vector(4 downto 0);
	     avs_s1_readdata  : out std_logic_vector(31 downto 0);
	     avs_s1_writedata : in std_logic_vector(31 downto 0);
	     convst           : out std_logic;
	     SCLK	      : out std_logic;
	     SDI	      : out std_logic;
	     SDO	      : in std_logic);
end entity;

architecture HPS_adc_arch of HPS_adc is
component adc is
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
end component;
	signal config_reg     : std_logic_vector(15 downto 0) := "1111111111111111";
	signal data           : std_logic_vector(11 downto 0) := "000000000000";
	signal data_reg       : std_logic_vector(3 downto 0) := "0000";
	signal R1	      : std_logic_vector(11 downto 0) := "000000000000";
	signal R2	      : std_logic_vector(11 downto 0) := "000000000000";
	signal R3	      : std_logic_vector(11 downto 0) := "000000000000";
	signal R4	      : std_logic_vector(11 downto 0) := "000000000000";
	signal R5	      : std_logic_vector(11 downto 0) := "000000000000";
	signal R6	      : std_logic_vector(11 downto 0) := "000000000000";
	signal R7	      : std_logic_vector(11 downto 0) := "000000000000";
	signal R8	      : std_logic_vector(11 downto 0) := "000000000000";
	signal R9	      : std_logic_vector(11 downto 0) := "000000000000";
	signal R10	      : std_logic_vector(11 downto 0) := "000000000000";
	signal R11	      : std_logic_vector(11 downto 0) := "000000000000";
	signal R12	      : std_logic_vector(11 downto 0) := "000000000000";
	signal R13	      : std_logic_vector(11 downto 0) := "000000000000";
	signal R14	      : std_logic_vector(11 downto 0) := "000000000000";
	signal R15	      : std_logic_vector(11 downto 0) := "000000000000";
	signal R0	      : std_logic_vector(11 downto 0) := "000000000000";
--singals here
	begin
	adc_map : adc port map(clock => clock, reset => reset, config_reg => config_reg(15 downto 0), convst => convst, SCLK => SCLK, SDI => SDI, SDO => SDO,
			       data_out => data,data_register_index => data_reg);
-- seperate process outside avs use data register index for case statement and assign data to each variable.
	Register_Map : process(clock)
	begin
	if rising_edge(clock) then
		case data_reg is
			when "0000" => R0  <= data;			
			when "0001" => R8  <= data;
			when "0010" => R1  <= data;
			when "0011" => R9  <= data;
			when "0100" => R2  <= data;
			when "0101" => R10  <= data;
			when "0110" => R3  <= data;
			when "0111" => R11  <= data;
			when "1000" => R4  <= data;
			when "1001" => R12  <= data;
			when "1010" => R5 <= data;
			when "1011" => R13 <= data;
			when "1100" => R6 <= data;
			when "1101" => R14 <= data;
			when "1110" => R7 <= data;
			when "1111" => R15 <= data;
			when others => null;
		end case;
	end if;
	end process;

	avalon_register_read : process(clock)
	begin
	if rising_edge(clock) and avs_s1_read = '1' then
		case avs_s1_address is
			when "00000" => avs_s1_readdata <= "00000000000000000000" & R0;
			when "00001" => avs_s1_readdata <= "00000000000000000000" & R1;
			when "00010" => avs_s1_readdata <= "00000000000000000000" & R2;
			when "00011" => avs_s1_readdata <= "00000000000000000000" & R3;
			when "00100" => avs_s1_readdata <= "00000000000000000000" & R4;
			when "00101" => avs_s1_readdata <= "00000000000000000000" & R5;
			when "00110" => avs_s1_readdata <= "00000000000000000000" & R6;
			when "00111" => avs_s1_readdata <= "00000000000000000000" & R7;
			when "01000" => avs_s1_readdata <= "00000000000000000000" & R8;
			when "01001" => avs_s1_readdata <= "00000000000000000000" & R9;
			when "01010" => avs_s1_readdata <= "00000000000000000000" & R10;
			when "01011" => avs_s1_readdata <= "00000000000000000000" & R11;
			when "01100" => avs_s1_readdata <= "00000000000000000000" & R12;
			when "01101" => avs_s1_readdata <= "00000000000000000000" & R13;
			when "01110" => avs_s1_readdata <= "00000000000000000000" & R14;
			when "01111" => avs_s1_readdata <= "00000000000000000000" & R15;
			when "10000" => avs_s1_readdata <= "0000000000000000" & config_reg;
			when others => avs_s1_readdata <= (others => '0');
		end case;
	end if;
	end process;

	avalon_register_write : process(clock, reset)
	begin
		if reset = '1' then
			config_reg    <= "1111111111111111";
		elsif rising_edge(clock) and avs_s1_write = '1' then
			case avs_s1_address is
				when "10000" => config_reg <= avs_s1_writedata(15 downto 0);
				when others => null;	
			end case;
		end if;
	end process;
end architecture;