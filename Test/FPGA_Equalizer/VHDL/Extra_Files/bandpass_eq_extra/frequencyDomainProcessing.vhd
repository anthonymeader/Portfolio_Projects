-- -------------------------------------------------------------
-- 
-- File Name: C:\Users\Antho\Desktop\EELE_468\eq\hdlsrc\bandpass_eq\frequencyDomainProcessing.vhd
-- 
-- Generated by MATLAB 9.14 and HDL Coder 4.1
-- 
-- -------------------------------------------------------------


-- -------------------------------------------------------------
-- 
-- Module: frequencyDomainProcessing
-- Source Path: bandpass_eq/fftAnalysisSynthesis/frequencyDomainProcessing
-- Hierarchy Level: 1
-- 
-- -------------------------------------------------------------
LIBRARY IEEE;
USE IEEE.std_logic_1164.ALL;
USE IEEE.numeric_std.ALL;
USE work.fftAnalysisSynthesis_pkg.ALL;

ENTITY frequencyDomainProcessing IS
  PORT( clk                               :   IN    std_logic;
        reset                             :   IN    std_logic;
        enb                               :   IN    std_logic;
        gain_1                            :   IN    std_logic_vector(15 DOWNTO 0);  -- ufix16_En14
        gain_2                            :   IN    std_logic_vector(15 DOWNTO 0);  -- ufix16_En14
        gain_3                            :   IN    std_logic_vector(15 DOWNTO 0);  -- ufix16_En14
        gain_4                            :   IN    std_logic_vector(15 DOWNTO 0);  -- ufix16_En14
        gain_5                            :   IN    std_logic_vector(15 DOWNTO 0);  -- ufix16_En14
        volume                            :   IN    std_logic_vector(15 DOWNTO 0);  -- ufix16_En16
        fftData_re                        :   IN    std_logic_vector(31 DOWNTO 0);  -- sfix32_En23
        fftData_im                        :   IN    std_logic_vector(31 DOWNTO 0);  -- sfix32_En23
        fftValid                          :   IN    std_logic;
        fftFramePulse                     :   IN    std_logic;
        passthrough                       :   IN    std_logic;  -- ufix1
        fftModifiedData_re                :   OUT   std_logic_vector(31 DOWNTO 0);  -- sfix32_En23
        fftModifiedData_im                :   OUT   std_logic_vector(31 DOWNTO 0);  -- sfix32_En23
        fftValidOut                       :   OUT   std_logic;
        fftFramePulseOut                  :   OUT   std_logic
        );
END frequencyDomainProcessing;


ARCHITECTURE rtl OF frequencyDomainProcessing IS

  -- Component Declarations
  COMPONENT applyComplexGains
    PORT( clk                             :   IN    std_logic;
          reset                           :   IN    std_logic;
          enb                             :   IN    std_logic;
          gain_1                          :   IN    std_logic_vector(15 DOWNTO 0);  -- ufix16_En14
          gain_2                          :   IN    std_logic_vector(15 DOWNTO 0);  -- ufix16_En14
          gain_3                          :   IN    std_logic_vector(15 DOWNTO 0);  -- ufix16_En14
          gain_4                          :   IN    std_logic_vector(15 DOWNTO 0);  -- ufix16_En14
          gain_5                          :   IN    std_logic_vector(15 DOWNTO 0);  -- ufix16_En14
          fftData_re                      :   IN    std_logic_vector(31 DOWNTO 0);  -- sfix32_En23
          fftData_im                      :   IN    std_logic_vector(31 DOWNTO 0);  -- sfix32_En23
          fftValid                        :   IN    std_logic;
          fftFramePulse                   :   IN    std_logic;
          volume                          :   IN    std_logic_vector(15 DOWNTO 0);  -- ufix16_En16
          fftModifiedData_re              :   OUT   std_logic_vector(31 DOWNTO 0);  -- sfix32_En23
          fftModifiedData_im              :   OUT   std_logic_vector(31 DOWNTO 0);  -- sfix32_En23
          fftValidOut                     :   OUT   std_logic;
          fftFramePulseOut                :   OUT   std_logic
          );
  END COMPONENT;

  -- Signals
  SIGNAL reduced_reg                      : std_logic_vector(0 TO 2);  -- ufix1 [3]
  SIGNAL passthrough_1                    : std_logic;  -- ufix1
  SIGNAL reduced_reg_1                    : std_logic_vector(0 TO 6);  -- ufix1 [7]
  SIGNAL passthrough_2                    : std_logic;  -- ufix1
  SIGNAL switch_compare_1                 : std_logic;
  SIGNAL applyComplexGains_out1_re        : std_logic_vector(31 DOWNTO 0);  -- ufix32
  SIGNAL applyComplexGains_out1_im        : std_logic_vector(31 DOWNTO 0);  -- ufix32
  SIGNAL applyComplexGains_out2           : std_logic;
  SIGNAL applyComplexGains_out3           : std_logic;
  SIGNAL applyComplexGains_out1_re_signed : signed(31 DOWNTO 0);  -- sfix32_En23
  SIGNAL applyComplexGains_out1_im_signed : signed(31 DOWNTO 0);  -- sfix32_En23
  SIGNAL fftData_re_signed                : signed(31 DOWNTO 0);  -- sfix32_En23
  SIGNAL fftData_im_signed                : signed(31 DOWNTO 0);  -- sfix32_En23
  SIGNAL delayMatch2_reg_re               : vector_of_signed32(0 TO 6);  -- sfix32_En23 [7]
  SIGNAL delayMatch2_reg_im               : vector_of_signed32(0 TO 6);  -- sfix32_En23 [7]
  SIGNAL fftData_re_1                     : signed(31 DOWNTO 0);  -- sfix32_En23
  SIGNAL fftData_im_1                     : signed(31 DOWNTO 0);  -- sfix32_En23
  SIGNAL Switch_out1_re                   : signed(31 DOWNTO 0);  -- sfix32_En23
  SIGNAL Switch_out1_im                   : signed(31 DOWNTO 0);  -- sfix32_En23
  SIGNAL switch_compare_1_1               : std_logic;
  SIGNAL Switch1_out1                     : std_logic;
  SIGNAL switch_compare_1_2               : std_logic;
  SIGNAL Switch2_out1                     : std_logic;

BEGIN
  -- Switch between passthrough mode 
  -- (no processing of FFT data)
  -- and frequency domain processing mode.  
  -- Passthrough mode when passthrough = 1.

  u_applyComplexGains : applyComplexGains
    PORT MAP( clk => clk,
              reset => reset,
              enb => enb,
              gain_1 => gain_1,  -- ufix16_En14
              gain_2 => gain_2,  -- ufix16_En14
              gain_3 => gain_3,  -- ufix16_En14
              gain_4 => gain_4,  -- ufix16_En14
              gain_5 => gain_5,  -- ufix16_En14
              fftData_re => fftData_re,  -- sfix32_En23
              fftData_im => fftData_im,  -- sfix32_En23
              fftValid => fftValid,
              fftFramePulse => fftFramePulse,
              volume => volume,  -- ufix16_En16
              fftModifiedData_re => applyComplexGains_out1_re,  -- sfix32_En23
              fftModifiedData_im => applyComplexGains_out1_im,  -- sfix32_En23
              fftValidOut => applyComplexGains_out2,
              fftFramePulseOut => applyComplexGains_out3
              );

  reduced_process : PROCESS (clk, reset)
  BEGIN
    IF reset = '1' THEN
      reduced_reg <= (OTHERS => '0');
    ELSIF rising_edge(clk) THEN
      IF enb = '1' THEN
        reduced_reg(0) <= passthrough;
        reduced_reg(1 TO 2) <= reduced_reg(0 TO 1);
      END IF;
    END IF;
  END PROCESS reduced_process;

  passthrough_1 <= reduced_reg(2);

  reduced_1_process : PROCESS (clk, reset)
  BEGIN
    IF reset = '1' THEN
      reduced_reg_1 <= (OTHERS => '0');
    ELSIF rising_edge(clk) THEN
      IF enb = '1' THEN
        reduced_reg_1(0) <= passthrough_1;
        reduced_reg_1(1 TO 6) <= reduced_reg_1(0 TO 5);
      END IF;
    END IF;
  END PROCESS reduced_1_process;

  passthrough_2 <= reduced_reg_1(6);

  
  switch_compare_1 <= '1' WHEN passthrough_2 > '0' ELSE
      '0';

  applyComplexGains_out1_re_signed <= signed(applyComplexGains_out1_re);

  applyComplexGains_out1_im_signed <= signed(applyComplexGains_out1_im);

  fftData_re_signed <= signed(fftData_re);

  fftData_im_signed <= signed(fftData_im);

  delayMatch2_process : PROCESS (clk, reset)
  BEGIN
    IF reset = '1' THEN
      delayMatch2_reg_re <= (OTHERS => to_signed(0, 32));
      delayMatch2_reg_im <= (OTHERS => to_signed(0, 32));
    ELSIF rising_edge(clk) THEN
      IF enb = '1' THEN
        delayMatch2_reg_im(0) <= fftData_im_signed;
        delayMatch2_reg_im(1 TO 6) <= delayMatch2_reg_im(0 TO 5);
        delayMatch2_reg_re(0) <= fftData_re_signed;
        delayMatch2_reg_re(1 TO 6) <= delayMatch2_reg_re(0 TO 5);
      END IF;
    END IF;
  END PROCESS delayMatch2_process;

  fftData_re_1 <= delayMatch2_reg_re(6);
  fftData_im_1 <= delayMatch2_reg_im(6);

  
  Switch_out1_re <= applyComplexGains_out1_re_signed WHEN switch_compare_1 = '0' ELSE
      fftData_re_1;
  
  Switch_out1_im <= applyComplexGains_out1_im_signed WHEN switch_compare_1 = '0' ELSE
      fftData_im_1;

  fftModifiedData_re <= std_logic_vector(Switch_out1_re);

  fftModifiedData_im <= std_logic_vector(Switch_out1_im);

  
  switch_compare_1_1 <= '1' WHEN passthrough_1 > '0' ELSE
      '0';

  
  Switch1_out1 <= applyComplexGains_out2 WHEN switch_compare_1_1 = '0' ELSE
      fftValid;

  
  switch_compare_1_2 <= '1' WHEN passthrough > '0' ELSE
      '0';

  
  Switch2_out1 <= applyComplexGains_out3 WHEN switch_compare_1_2 = '0' ELSE
      fftFramePulse;

  fftValidOut <= Switch1_out1;

  fftFramePulseOut <= Switch2_out1;

END rtl;

