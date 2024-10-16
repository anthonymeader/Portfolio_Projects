library IEEE;
use IEEE.std_logic_1164.all; 
use IEEE.numeric_std.all;
entity data_path is
	port (clock       : in  std_logic;
	      reset       : in  std_logic;
	      address	  : out std_logic_vector(7 downto 0);
	      from_memory : in  std_logic_vector(7 downto 0);
	      to_memory   : out std_logic_vector(7 downto 0);
	      IR_Load     : in  std_logic;
	      IR	  : out std_logic_vector(7 downto 0);
	      MAR_Load    : in  std_logic;
	      PC_Load     : in  std_logic;
	      PC_Inc      : in  std_logic;
	      A_Load      : in  std_logic;
	      B_Load      : in  std_logic;
	      ALU_Sel     : in  std_logic_vector(2 downto 0);
	      CCR_Result  : out std_logic_vector(3 downto 0);
	      CCR_Load    : in  std_logic;
	      Bus2_Sel    : in  std_logic_vector(1 downto 0);
	      Bus1_Sel    : in  std_logic_vector(1 downto 0));
	      
end entity;

architecture data_path_arch of data_path is
	signal NZVC       : std_logic_vector(3 downto 0);
	signal ALU_Result : std_logic_vector(7 downto 0);
	signal BUS1	  : std_logic_vector(7 downto 0);
	signal BUS2	  : std_logic_vector(7 downto 0);
	signal MAR        : std_logic_vector(7 downto 0);
	signal PC         : std_logic_vector(7 downto 0);
	signal A          : std_logic_vector(7 downto 0);
	signal B          : std_logic_vector(7 downto 0);
	signal PC_uns     : unsigned(7 downto 0);

	begin

	ALU_Result <= x"00";

	MUX_BUS1 : process (Bus1_Sel, PC, A, B)
	  begin
	    case (Bus1_Sel) is
	      when "00" => BUS1 <= PC;
	      when "01" => BUS1 <= A;
	      when "10" => BUS1 <= B;
	      when others => BUS1 <= x"00";
	    end case;
	end process;

	MUX_BUS2 : process (Bus2_Sel, BUS1, from_memory)
	  begin
	    case(Bus2_Sel) is
	      when "00" => BUS2 <= x"00";  --aluresult
	      when "01" => BUS2 <= BUS1;
	      when "10" => BUS2 <= from_memory;
	      when others => BUS2 <= x"00";
	   end case;
	end process;

	address <= MAR;
	to_memory <= BUS1;



	INSTRUCTION_REGISTER : process (clock, reset)
	  begin
	    if (reset = '0') then
	      IR <= x"00";
	    elsif (clock'event and clock = '1') then
	      if (IR_Load = '1') then
	        IR <= BUS2;
	      end if;
	    end if;
	end process;

	MEMORY_ADDRESS_REGISTER : process (clock, reset)
	  begin
	    if (reset = '0') then
	      MAR <= x"00";
	    elsif (clock'event and clock='1') then
	      if (MAR_Load = '1') then
	        MAR <= BUS2;
	      end if;
	    end if;
	end process;
	address <= MAR;

	PROGRAM_COUNTER : process (clock, reset)
	  begin
	    if (reset = '0') then
	      PC_uns <= x"00";
	    elsif (clock'event and clock='1') then
	      if (PC_Load = '1') then
	        PC_uns <= unsigned(BUS2);
	      elsif (PC_Inc = '1') then
		PC_uns <= PC_uns + 1;
	      end if;
	    end if;
	end process;
	PC <= std_logic_vector(PC_uns);

	A_REGISTER : process (clock, reset)
	  begin
	    if (reset = '0') then
	      A <= x"00";
	    elsif (clock'event and clock = '1') then
	      if (A_Load = '1') then
	        A <= BUS2;
	      end if;
	    end if;
	end process;

	B_REGISTER : process (clock, reset)
	  begin
	    if (reset = '0') then
	      B <= x"00";
	    elsif (clock'event and clock = '1') then
	      if (B_Load = '1') then
	        B <= BUS2;
	      end if;
	    end if;
	end process;

	CONDITION_CODE_REGISTER : process (clock, reset)
	  begin
	    if (reset = '0') then
	      CCR_Result <= x"0";
	    elsif (clock'event and clock='1') then
	      if (CCR_Load = '1') then
	        CCR_Result <= NZVC;
	      end if;
	    end if;
	end process;
	

	    


	    

end architecture;