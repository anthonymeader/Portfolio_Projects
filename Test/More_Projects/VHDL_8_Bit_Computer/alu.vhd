library IEEE;
use IEEE.std_logic_1164.all; 
use IEEE.numeric_std.all;

entity alu is
	  port (ALU_Sel    : in  std_logic_vector(2 downto 0);
		NZVC	   : out std_logic_vector(3 downto 0);
		ALU_Result : out std_logic_vector(7 downto 0);
		A          : in  std_logic_vector(7 downto 0);
		B	   : in  std_logic_vector(7 downto 0));
end entity;

architecture alu_arch of alu is
	begin
	ALU_PROCESS : process (A, B, ALU_Sel)
	  variable Sum_uns : unsigned(8 downto 0);
	  begin
	    if (ALU_Sel = "000") then
	      Sum_uns := unsigned('0' & A) + unsigned('0' & B);
	      ALU_Result <= std_logic_vector(Sum_uns(7 downto 0));

	      NZVC(3) <= Sum_uns(7); -- Negative Flag (N)

	      if (Sum_uns(7 downto 0) = x"00") then -- Zero Flag (Z)
	      	NZVC(2) <= '1';
	      else
	      	NZVC(2) <= '0';
	      end if;
	      
	      if (A(7) = '0' and B(7) = '0' and Sum_uns(7) = '1')
	      or (A(7) = '1' and B(7) = '1' and Sum_uns(7) = '0') then
		NZVC(1) <= '1';
	      else
		NZVC(1) <= '0';
	     end if;
		NZVC(0) <= Sum_uns(8);

	   end if;
	
	    

	end process;

end architecture;