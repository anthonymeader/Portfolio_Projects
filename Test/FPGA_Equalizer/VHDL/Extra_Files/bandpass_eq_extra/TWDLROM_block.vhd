-- -------------------------------------------------------------
-- 
-- File Name: C:\Users\Antho\Desktop\EELE_468\eq\hdlsrc\bandpass_eq\TWDLROM_block.vhd
-- 
-- Generated by MATLAB 9.14 and HDL Coder 4.1
-- 
-- -------------------------------------------------------------


-- -------------------------------------------------------------
-- 
-- Module: TWDLROM_block
-- Source Path: bandpass_eq/fftAnalysisSynthesis/synthesis/iFFT/TWDLROM
-- Hierarchy Level: 3
-- 
-- -------------------------------------------------------------
LIBRARY IEEE;
USE IEEE.std_logic_1164.ALL;
USE IEEE.numeric_std.ALL;
USE work.fftAnalysisSynthesis_pkg.ALL;

ENTITY TWDLROM_block IS
  PORT( clk                               :   IN    std_logic;
        reset                             :   IN    std_logic;
        enb                               :   IN    std_logic;
        dMemOutDly_vld                    :   IN    std_logic;
        stage                             :   IN    std_logic_vector(2 DOWNTO 0);  -- ufix3
        initIC                            :   IN    std_logic;
        twdl_re                           :   OUT   std_logic_vector(31 DOWNTO 0);  -- sfix32_En30
        twdl_im                           :   OUT   std_logic_vector(31 DOWNTO 0)  -- sfix32_En30
        );
END TWDLROM_block;


ARCHITECTURE rtl OF TWDLROM_block IS

  -- Constants
  CONSTANT Twiddle_re_table_data          : vector_of_signed32(0 TO 31) := 
    (to_signed(1073741824, 32), to_signed(1073418433, 32), to_signed(1072448455, 32), to_signed(1070832474, 32),
     to_signed(1068571464, 32), to_signed(1065666786, 32), to_signed(1062120190, 32), to_signed(1057933813, 32),
     to_signed(1053110176, 32), to_signed(1047652185, 32), to_signed(1041563127, 32), to_signed(1034846671, 32),
     to_signed(1027506862, 32), to_signed(1019548121, 32), to_signed(1010975242, 32), to_signed(1001793390, 32),
     to_signed(992008094, 32), to_signed(981625251, 32), to_signed(970651112, 32), to_signed(959092290, 32),
     to_signed(946955747, 32), to_signed(934248793, 32), to_signed(920979082, 32), to_signed(907154608, 32),
     to_signed(892783698, 32), to_signed(877875009, 32), to_signed(862437520, 32), to_signed(846480531, 32),
     to_signed(830013654, 32), to_signed(813046808, 32), to_signed(795590213, 32), to_signed(777654384, 32));  -- sfix32 [32]
  CONSTANT Twiddle_im_table_data          : vector_of_signed32(0 TO 31) := 
    (to_signed(0, 32), to_signed(-26350943, 32), to_signed(-52686014, 32), to_signed(-78989349, 32),
     to_signed(-105245103, 32), to_signed(-131437462, 32), to_signed(-157550647, 32), to_signed(-183568930, 32),
     to_signed(-209476638, 32), to_signed(-235258165, 32), to_signed(-260897982, 32), to_signed(-286380643, 32),
     to_signed(-311690799, 32), to_signed(-336813204, 32), to_signed(-361732726, 32), to_signed(-386434353, 32),
     to_signed(-410903207, 32), to_signed(-435124548, 32), to_signed(-459083786, 32), to_signed(-482766489, 32),
     to_signed(-506158392, 32), to_signed(-529245404, 32), to_signed(-552013618, 32), to_signed(-574449320, 32),
     to_signed(-596538995, 32), to_signed(-618269338, 32), to_signed(-639627258, 32), to_signed(-660599890, 32),
     to_signed(-681174602, 32), to_signed(-701339000, 32), to_signed(-721080937, 32), to_signed(-740388522, 32));  -- sfix32 [32]

  -- Signals
  SIGNAL stage_unsigned                   : unsigned(2 DOWNTO 0);  -- ufix3
  SIGNAL minResRX2FFTTwdlMapping_baseAddr : unsigned(6 DOWNTO 0);  -- ufix7
  SIGNAL minResRX2FFTTwdlMapping_cnt      : unsigned(6 DOWNTO 0);  -- ufix7
  SIGNAL minResRX2FFTTwdlMapping_octantReg1 : unsigned(2 DOWNTO 0);  -- ufix3
  SIGNAL minResRX2FFTTwdlMapping_twdlAddr_raw : unsigned(7 DOWNTO 0);  -- ufix8
  SIGNAL minResRX2FFTTwdlMapping_twdlAddrMap : unsigned(4 DOWNTO 0);  -- ufix5
  SIGNAL minResRX2FFTTwdlMapping_twdl45Reg : std_logic;
  SIGNAL minResRX2FFTTwdlMapping_dvldReg1 : std_logic;
  SIGNAL minResRX2FFTTwdlMapping_dvldReg2 : std_logic;
  SIGNAL minResRX2FFTTwdlMapping_maxCnt   : unsigned(6 DOWNTO 0);  -- ufix7
  SIGNAL minResRX2FFTTwdlMapping_baseAddr_next : unsigned(6 DOWNTO 0);  -- ufix7
  SIGNAL minResRX2FFTTwdlMapping_cnt_next : unsigned(6 DOWNTO 0);  -- ufix7
  SIGNAL minResRX2FFTTwdlMapping_octantReg1_next : unsigned(2 DOWNTO 0);  -- ufix3
  SIGNAL minResRX2FFTTwdlMapping_twdlAddr_raw_next : unsigned(7 DOWNTO 0);  -- ufix8
  SIGNAL minResRX2FFTTwdlMapping_twdlAddrMap_next : unsigned(4 DOWNTO 0);  -- ufix5
  SIGNAL minResRX2FFTTwdlMapping_twdl45Reg_next : std_logic;
  SIGNAL minResRX2FFTTwdlMapping_dvldReg1_next : std_logic;
  SIGNAL minResRX2FFTTwdlMapping_dvldReg2_next : std_logic;
  SIGNAL minResRX2FFTTwdlMapping_maxCnt_next : unsigned(6 DOWNTO 0);  -- ufix7
  SIGNAL twdlAddr                         : unsigned(4 DOWNTO 0);  -- ufix5
  SIGNAL twdlAddrVld                      : std_logic;
  SIGNAL twdlOctant                       : unsigned(2 DOWNTO 0);  -- ufix3
  SIGNAL twdl45                           : std_logic;
  SIGNAL twiddleS_re                      : signed(31 DOWNTO 0);  -- sfix32_En30
  SIGNAL twiddleReg_re                    : signed(31 DOWNTO 0);  -- sfix32_En30
  SIGNAL twiddleS_im                      : signed(31 DOWNTO 0);  -- sfix32_En30
  SIGNAL twiddleReg_im                    : signed(31 DOWNTO 0);  -- sfix32_En30
  SIGNAL twdlOctantReg                    : unsigned(2 DOWNTO 0);  -- ufix3
  SIGNAL twdl45Reg                        : std_logic;
  SIGNAL twdl_re_tmp                      : signed(31 DOWNTO 0);  -- sfix32_En30
  SIGNAL twdl_im_tmp                      : signed(31 DOWNTO 0);  -- sfix32_En30

BEGIN
  stage_unsigned <= unsigned(stage);

  -- minResRX2FFTTwdlMapping
  minResRX2FFTTwdlMapping_process : PROCESS (clk, reset)
  BEGIN
    IF reset = '1' THEN
      minResRX2FFTTwdlMapping_octantReg1 <= to_unsigned(16#0#, 3);
      minResRX2FFTTwdlMapping_twdlAddr_raw <= to_unsigned(16#00#, 8);
      minResRX2FFTTwdlMapping_twdlAddrMap <= to_unsigned(16#00#, 5);
      minResRX2FFTTwdlMapping_twdl45Reg <= '0';
      minResRX2FFTTwdlMapping_dvldReg1 <= '0';
      minResRX2FFTTwdlMapping_dvldReg2 <= '0';
      minResRX2FFTTwdlMapping_baseAddr <= to_unsigned(16#00#, 7);
      minResRX2FFTTwdlMapping_cnt <= to_unsigned(16#7F#, 7);
      minResRX2FFTTwdlMapping_maxCnt <= to_unsigned(16#00#, 7);
    ELSIF rising_edge(clk) THEN
      IF enb = '1' THEN
        minResRX2FFTTwdlMapping_baseAddr <= minResRX2FFTTwdlMapping_baseAddr_next;
        minResRX2FFTTwdlMapping_cnt <= minResRX2FFTTwdlMapping_cnt_next;
        minResRX2FFTTwdlMapping_octantReg1 <= minResRX2FFTTwdlMapping_octantReg1_next;
        minResRX2FFTTwdlMapping_twdlAddr_raw <= minResRX2FFTTwdlMapping_twdlAddr_raw_next;
        minResRX2FFTTwdlMapping_twdlAddrMap <= minResRX2FFTTwdlMapping_twdlAddrMap_next;
        minResRX2FFTTwdlMapping_twdl45Reg <= minResRX2FFTTwdlMapping_twdl45Reg_next;
        minResRX2FFTTwdlMapping_dvldReg1 <= minResRX2FFTTwdlMapping_dvldReg1_next;
        minResRX2FFTTwdlMapping_dvldReg2 <= minResRX2FFTTwdlMapping_dvldReg2_next;
        minResRX2FFTTwdlMapping_maxCnt <= minResRX2FFTTwdlMapping_maxCnt_next;
      END IF;
    END IF;
  END PROCESS minResRX2FFTTwdlMapping_process;

  minResRX2FFTTwdlMapping_output : PROCESS (dMemOutDly_vld, initIC, minResRX2FFTTwdlMapping_baseAddr,
       minResRX2FFTTwdlMapping_cnt, minResRX2FFTTwdlMapping_dvldReg1,
       minResRX2FFTTwdlMapping_dvldReg2, minResRX2FFTTwdlMapping_maxCnt,
       minResRX2FFTTwdlMapping_octantReg1, minResRX2FFTTwdlMapping_twdl45Reg,
       minResRX2FFTTwdlMapping_twdlAddrMap,
       minResRX2FFTTwdlMapping_twdlAddr_raw, stage_unsigned)
    VARIABLE octant : unsigned(2 DOWNTO 0);
    VARIABLE sub_cast : signed(17 DOWNTO 0);
    VARIABLE sub_temp : signed(17 DOWNTO 0);
    VARIABLE sub_cast_0 : signed(9 DOWNTO 0);
    VARIABLE sub_temp_0 : signed(9 DOWNTO 0);
    VARIABLE sub_cast_1 : signed(9 DOWNTO 0);
    VARIABLE sub_temp_1 : signed(9 DOWNTO 0);
    VARIABLE sub_cast_2 : signed(17 DOWNTO 0);
    VARIABLE sub_temp_2 : signed(17 DOWNTO 0);
    VARIABLE sub_cast_3 : signed(17 DOWNTO 0);
    VARIABLE sub_temp_3 : signed(17 DOWNTO 0);
  BEGIN
    sub_temp := to_signed(16#00000#, 18);
    sub_temp_0 := to_signed(16#000#, 10);
    sub_temp_1 := to_signed(16#000#, 10);
    sub_temp_2 := to_signed(16#00000#, 18);
    sub_temp_3 := to_signed(16#00000#, 18);
    sub_cast_3 := to_signed(16#00000#, 18);
    sub_cast_2 := to_signed(16#00000#, 18);
    sub_cast_1 := to_signed(16#000#, 10);
    sub_cast_0 := to_signed(16#000#, 10);
    sub_cast := to_signed(16#00000#, 18);
    minResRX2FFTTwdlMapping_baseAddr_next <= minResRX2FFTTwdlMapping_baseAddr;
    minResRX2FFTTwdlMapping_cnt_next <= minResRX2FFTTwdlMapping_cnt;
    minResRX2FFTTwdlMapping_maxCnt_next <= minResRX2FFTTwdlMapping_maxCnt;
    minResRX2FFTTwdlMapping_dvldReg2_next <= minResRX2FFTTwdlMapping_dvldReg1;
    minResRX2FFTTwdlMapping_dvldReg1_next <= dMemOutDly_vld;
    CASE minResRX2FFTTwdlMapping_twdlAddr_raw IS
      WHEN "00100000" =>
        octant := to_unsigned(16#0#, 3);
        minResRX2FFTTwdlMapping_twdl45Reg_next <= '1';
      WHEN "01000000" =>
        octant := to_unsigned(16#1#, 3);
        minResRX2FFTTwdlMapping_twdl45Reg_next <= '0';
      WHEN "01100000" =>
        octant := to_unsigned(16#2#, 3);
        minResRX2FFTTwdlMapping_twdl45Reg_next <= '1';
      WHEN "10000000" =>
        octant := to_unsigned(16#3#, 3);
        minResRX2FFTTwdlMapping_twdl45Reg_next <= '0';
      WHEN "10100000" =>
        octant := to_unsigned(16#4#, 3);
        minResRX2FFTTwdlMapping_twdl45Reg_next <= '1';
      WHEN OTHERS => 
        octant := minResRX2FFTTwdlMapping_twdlAddr_raw(7 DOWNTO 5);
        minResRX2FFTTwdlMapping_twdl45Reg_next <= '0';
    END CASE;
    minResRX2FFTTwdlMapping_octantReg1_next <= octant;
    CASE octant IS
      WHEN "000" =>
        minResRX2FFTTwdlMapping_twdlAddrMap_next <= minResRX2FFTTwdlMapping_twdlAddr_raw(4 DOWNTO 0);
      WHEN "001" =>
        sub_cast_0 := signed(resize(minResRX2FFTTwdlMapping_twdlAddr_raw, 10));
        sub_temp_0 := to_signed(16#040#, 10) - sub_cast_0;
        minResRX2FFTTwdlMapping_twdlAddrMap_next <= unsigned(sub_temp_0(4 DOWNTO 0));
      WHEN "010" =>
        sub_cast_1 := signed(resize(minResRX2FFTTwdlMapping_twdlAddr_raw, 10));
        sub_temp_1 := sub_cast_1 - to_signed(16#040#, 10);
        minResRX2FFTTwdlMapping_twdlAddrMap_next <= unsigned(sub_temp_1(4 DOWNTO 0));
      WHEN "011" =>
        sub_cast_2 := signed(resize(minResRX2FFTTwdlMapping_twdlAddr_raw & '0' & '0' & '0' & '0' & '0', 18));
        sub_temp_2 := to_signed(16#01000#, 18) - sub_cast_2;
        minResRX2FFTTwdlMapping_twdlAddrMap_next <= unsigned(sub_temp_2(9 DOWNTO 5));
      WHEN "100" =>
        sub_cast_3 := signed(resize(minResRX2FFTTwdlMapping_twdlAddr_raw & '0' & '0' & '0' & '0' & '0', 18));
        sub_temp_3 := sub_cast_3 - to_signed(16#01000#, 18);
        minResRX2FFTTwdlMapping_twdlAddrMap_next <= unsigned(sub_temp_3(9 DOWNTO 5));
      WHEN OTHERS => 
        sub_cast := signed(resize(minResRX2FFTTwdlMapping_twdlAddr_raw & '0' & '0' & '0' & '0' & '0', 18));
        sub_temp := to_signed(16#01800#, 18) - sub_cast;
        minResRX2FFTTwdlMapping_twdlAddrMap_next <= unsigned(sub_temp(9 DOWNTO 5));
    END CASE;
    minResRX2FFTTwdlMapping_twdlAddr_raw_next <= resize(unsigned'(minResRX2FFTTwdlMapping_baseAddr(0) & minResRX2FFTTwdlMapping_baseAddr(1) & minResRX2FFTTwdlMapping_baseAddr(2) & minResRX2FFTTwdlMapping_baseAddr(3) & minResRX2FFTTwdlMapping_baseAddr(4) & minResRX2FFTTwdlMapping_baseAddr(5) & minResRX2FFTTwdlMapping_baseAddr(6)), 8);
    IF ( NOT initIC) = '1' THEN 
      IF dMemOutDly_vld = '1' AND (minResRX2FFTTwdlMapping_cnt = to_unsigned(16#00#, 7)) THEN 
        minResRX2FFTTwdlMapping_baseAddr_next <= minResRX2FFTTwdlMapping_baseAddr + to_unsigned(16#01#, 7);
      END IF;
    ELSE 
      minResRX2FFTTwdlMapping_baseAddr_next <= to_unsigned(16#00#, 7);
    END IF;
    IF ( NOT initIC) = '1' THEN 
      IF dMemOutDly_vld = '1' THEN 
        IF minResRX2FFTTwdlMapping_cnt = to_unsigned(16#00#, 7) THEN 
          minResRX2FFTTwdlMapping_cnt_next <= minResRX2FFTTwdlMapping_maxCnt;
        ELSE 
          minResRX2FFTTwdlMapping_cnt_next <= minResRX2FFTTwdlMapping_cnt - to_unsigned(16#01#, 7);
        END IF;
      END IF;
    ELSIF stage_unsigned = to_unsigned(16#0#, 3) THEN 
      minResRX2FFTTwdlMapping_maxCnt_next <= to_unsigned(16#7F#, 7);
      minResRX2FFTTwdlMapping_cnt_next <= to_unsigned(16#7F#, 7);
    ELSE 
      minResRX2FFTTwdlMapping_cnt_next <= minResRX2FFTTwdlMapping_maxCnt srl 1;
      minResRX2FFTTwdlMapping_maxCnt_next <= minResRX2FFTTwdlMapping_maxCnt srl 1;
    END IF;
    twdlAddr <= minResRX2FFTTwdlMapping_twdlAddrMap;
    twdlAddrVld <= minResRX2FFTTwdlMapping_dvldReg2;
    twdlOctant <= minResRX2FFTTwdlMapping_octantReg1;
    twdl45 <= minResRX2FFTTwdlMapping_twdl45Reg;
  END PROCESS minResRX2FFTTwdlMapping_output;


  -- Twiddle ROM1
  twiddleS_re <= Twiddle_re_table_data(to_integer(twdlAddr));

  TWIDDLEROM_RE_process : PROCESS (clk, reset)
  BEGIN
    IF reset = '1' THEN
      twiddleReg_re <= to_signed(0, 32);
    ELSIF rising_edge(clk) THEN
      IF enb = '1' THEN
        twiddleReg_re <= twiddleS_re;
      END IF;
    END IF;
  END PROCESS TWIDDLEROM_RE_process;


  -- Twiddle ROM2
  twiddleS_im <= Twiddle_im_table_data(to_integer(twdlAddr));

  TWIDDLEROM_IM_process : PROCESS (clk, reset)
  BEGIN
    IF reset = '1' THEN
      twiddleReg_im <= to_signed(0, 32);
    ELSIF rising_edge(clk) THEN
      IF enb = '1' THEN
        twiddleReg_im <= twiddleS_im;
      END IF;
    END IF;
  END PROCESS TWIDDLEROM_IM_process;


  intdelay_process : PROCESS (clk, reset)
  BEGIN
    IF reset = '1' THEN
      twdlOctantReg <= to_unsigned(16#0#, 3);
    ELSIF rising_edge(clk) THEN
      IF enb = '1' THEN
        twdlOctantReg <= twdlOctant;
      END IF;
    END IF;
  END PROCESS intdelay_process;


  intdelay_1_process : PROCESS (clk, reset)
  BEGIN
    IF reset = '1' THEN
      twdl45Reg <= '0';
    ELSIF rising_edge(clk) THEN
      IF enb = '1' THEN
        twdl45Reg <= twdl45;
      END IF;
    END IF;
  END PROCESS intdelay_1_process;


  -- Radix22TwdlOctCorr
  Radix22TwdlOctCorr_output : PROCESS (twdl45Reg, twdlOctantReg, twiddleReg_im, twiddleReg_re)
    VARIABLE twdlIn_re : signed(31 DOWNTO 0);
    VARIABLE twdlIn_im : signed(31 DOWNTO 0);
    VARIABLE cast : signed(32 DOWNTO 0);
    VARIABLE cast_0 : signed(32 DOWNTO 0);
    VARIABLE cast_1 : signed(32 DOWNTO 0);
    VARIABLE cast_2 : signed(32 DOWNTO 0);
    VARIABLE cast_3 : signed(32 DOWNTO 0);
    VARIABLE cast_4 : signed(32 DOWNTO 0);
    VARIABLE cast_5 : signed(32 DOWNTO 0);
    VARIABLE cast_6 : signed(32 DOWNTO 0);
    VARIABLE cast_7 : signed(32 DOWNTO 0);
    VARIABLE cast_8 : signed(32 DOWNTO 0);
    VARIABLE cast_9 : signed(32 DOWNTO 0);
    VARIABLE cast_10 : signed(32 DOWNTO 0);
  BEGIN
    cast_0 := to_signed(0, 33);
    cast_2 := to_signed(0, 33);
    cast_4 := to_signed(0, 33);
    cast_6 := to_signed(0, 33);
    cast_8 := to_signed(0, 33);
    cast_10 := to_signed(0, 33);
    cast_3 := to_signed(0, 33);
    cast_9 := to_signed(0, 33);
    cast_1 := to_signed(0, 33);
    cast_7 := to_signed(0, 33);
    cast := to_signed(0, 33);
    cast_5 := to_signed(0, 33);
    twdlIn_re := twiddleReg_re;
    twdlIn_im := twiddleReg_im;
    IF twdl45Reg = '1' THEN 
      CASE twdlOctantReg IS
        WHEN "000" =>
          twdlIn_re := to_signed(759250125, 32);
          twdlIn_im := to_signed(-759250125, 32);
        WHEN "010" =>
          twdlIn_re := to_signed(-759250125, 32);
          twdlIn_im := to_signed(-759250125, 32);
        WHEN "100" =>
          twdlIn_re := to_signed(-759250125, 32);
          twdlIn_im := to_signed(759250125, 32);
        WHEN OTHERS => 
          twdlIn_re := to_signed(759250125, 32);
          twdlIn_im := to_signed(-759250125, 32);
      END CASE;
    ELSE 
      CASE twdlOctantReg IS
        WHEN "000" =>
          NULL;
        WHEN "001" =>
          cast := resize(twiddleReg_im, 33);
          cast_0 :=  - (cast);
          twdlIn_re := cast_0(31 DOWNTO 0);
          cast_5 := resize(twiddleReg_re, 33);
          cast_6 :=  - (cast_5);
          twdlIn_im := cast_6(31 DOWNTO 0);
        WHEN "010" =>
          twdlIn_re := twiddleReg_im;
          cast_7 := resize(twiddleReg_re, 33);
          cast_8 :=  - (cast_7);
          twdlIn_im := cast_8(31 DOWNTO 0);
        WHEN "011" =>
          cast_1 := resize(twiddleReg_re, 33);
          cast_2 :=  - (cast_1);
          twdlIn_re := cast_2(31 DOWNTO 0);
          twdlIn_im := twiddleReg_im;
        WHEN "100" =>
          cast_3 := resize(twiddleReg_re, 33);
          cast_4 :=  - (cast_3);
          twdlIn_re := cast_4(31 DOWNTO 0);
          cast_9 := resize(twiddleReg_im, 33);
          cast_10 :=  - (cast_9);
          twdlIn_im := cast_10(31 DOWNTO 0);
        WHEN OTHERS => 
          twdlIn_re := twiddleReg_im;
          twdlIn_im := twiddleReg_re;
      END CASE;
    END IF;
    twdl_re_tmp <= twdlIn_re;
    twdl_im_tmp <= twdlIn_im;
  END PROCESS Radix22TwdlOctCorr_output;


  twdl_re <= std_logic_vector(twdl_re_tmp);

  twdl_im <= std_logic_vector(twdl_im_tmp);

END rtl;

