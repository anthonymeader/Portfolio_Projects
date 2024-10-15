library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;
use IEEE.std_logic_unsigned.all;

LIBRARY altera;
USE altera.altera_primitives_components.all;

entity rate is
	port(clock 	         : in std_logic;
	     reset 	         : in std_logic;
	     SW    	         : in std_logic_vector(3 downto 0);
	     SYS_CLKs_Sec    : in std_logic_vector (31 downto 0);
	     Base_rate 	   : in std_logic_vector(7 downto 0);
	     counter	      : out integer;
	     cnt0            : out integer;
		  cnt1				: out integer;
		  cnt2				: out integer;
		  cnt3            : out integer;
		  cnt4				: out integer;
		  cnt5				: out integer);
	  
	
end entity;


architecture rate_arch of rate is

	signal product : unsigned(39 downto 0):= (others => '0');
	
begin
	process (clock)
	begin
	if rising_edge(clock) then
		product <= (unsigned(Base_rate) * unsigned(SYS_CLKs_Sec));
		counter <= to_integer(product(39 downto 4));
		    cnt0 <= to_integer(product(39 downto 5));
		    cnt1 <= to_integer(product(39 downto 6));
		    cnt2 <= to_integer(product(39 downto 3));
		    cnt3 <= to_integer(product(39 downto 7));
			cnt4 <= to_integer(product(39 downto 6));
		    cnt5 <= to_integer(product(39 downto 4));
	end if;
	end process;
end architecture;
