-- -------------------------------------------------------------
-- 
-- File Name: C:\Users\Antho\Desktop\EELE_468\eq\hdlsrc\bandpass_eq\fftAnalysisSynthesis.vhd
-- 
-- Generated by MATLAB 9.14 and HDL Coder 4.1
-- 
-- 
-- -------------------------------------------------------------
-- Rate and Clocking Details
-- -------------------------------------------------------------
-- Model base rate: 1.01725e-08
-- Target subsystem base rate: 1.01725e-08
-- 
-- 
-- Clock Enable  Sample Time
-- -------------------------------------------------------------
-- ce_out        2.08333e-05
-- -------------------------------------------------------------
-- 
-- 
-- Output Signal                 Clock Enable  Sample Time
-- -------------------------------------------------------------
-- audioOut                      ce_out        2.08333e-05
-- -------------------------------------------------------------
-- 
-- -------------------------------------------------------------


-- -------------------------------------------------------------
-- 
-- Module: fftAnalysisSynthesis
-- Source Path: bandpass_eq/fftAnalysisSynthesis
-- Hierarchy Level: 0
-- 
-- -------------------------------------------------------------
LIBRARY IEEE;
USE IEEE.std_logic_1164.ALL;
USE IEEE.numeric_std.ALL;

ENTITY fftAnalysisSynthesis IS
  PORT( clk                               :   IN    std_logic;
        reset                             :   IN    std_logic;
        clk_enable                        :   IN    std_logic;
        gain_1                            :   IN    std_logic_vector(15 DOWNTO 0);  -- ufix16_En14
        gain_2                            :   IN    std_logic_vector(15 DOWNTO 0);  -- ufix16_En14
        gain_3                            :   IN    std_logic_vector(15 DOWNTO 0);  -- ufix16_En14
        gain_4                            :   IN    std_logic_vector(15 DOWNTO 0);  -- ufix16_En14
        gain_5                            :   IN    std_logic_vector(15 DOWNTO 0);  -- ufix16_En14
        audioIn                           :   IN    std_logic_vector(23 DOWNTO 0);  -- sfix24_En23
        passthrough                       :   IN    std_logic;  -- ufix1
        volume                            :   IN    std_logic_vector(15 DOWNTO 0);  -- ufix16_En16
        ce_out                            :   OUT   std_logic;
        audioOut                          :   OUT   std_logic_vector(23 DOWNTO 0)  -- sfix24_En23
        );
END fftAnalysisSynthesis;


ARCHITECTURE rtl OF fftAnalysisSynthesis IS

  -- Component Declarations
  COMPONENT fftAnalysisSynthesis_tc
    PORT( clk                             :   IN    std_logic;
          reset                           :   IN    std_logic;
          clk_enable                      :   IN    std_logic;
          enb                             :   OUT   std_logic;
          enb_1_1_1                       :   OUT   std_logic;
          enb_1_2048_0                    :   OUT   std_logic;
          enb_1_2048_1                    :   OUT   std_logic;
          enb_1_2048_11                   :   OUT   std_logic
          );
  END COMPONENT;

  COMPONENT analysis
    PORT( clk                             :   IN    std_logic;
          reset                           :   IN    std_logic;
          enb                             :   IN    std_logic;
          enb_1_2048_0                    :   IN    std_logic;
          enb_1_2048_1                    :   IN    std_logic;
          enb_1_1_1                       :   IN    std_logic;
          audioIn                         :   IN    std_logic_vector(23 DOWNTO 0);  -- sfix24_En23
          passthrough                     :   IN    std_logic;  -- ufix1
          fftData_re                      :   OUT   std_logic_vector(31 DOWNTO 0);  -- sfix32_En23
          fftData_im                      :   OUT   std_logic_vector(31 DOWNTO 0);  -- sfix32_En23
          fftValid                        :   OUT   std_logic;
          fftFramePulse                   :   OUT   std_logic;
          passthroughFast                 :   OUT   std_logic  -- ufix1
          );
  END COMPONENT;

  COMPONENT frequencyDomainProcessing
    PORT( clk                             :   IN    std_logic;
          reset                           :   IN    std_logic;
          enb                             :   IN    std_logic;
          gain_1                          :   IN    std_logic_vector(15 DOWNTO 0);  -- ufix16_En14
          gain_2                          :   IN    std_logic_vector(15 DOWNTO 0);  -- ufix16_En14
          gain_3                          :   IN    std_logic_vector(15 DOWNTO 0);  -- ufix16_En14
          gain_4                          :   IN    std_logic_vector(15 DOWNTO 0);  -- ufix16_En14
          gain_5                          :   IN    std_logic_vector(15 DOWNTO 0);  -- ufix16_En14
          volume                          :   IN    std_logic_vector(15 DOWNTO 0);  -- ufix16_En16
          fftData_re                      :   IN    std_logic_vector(31 DOWNTO 0);  -- sfix32_En23
          fftData_im                      :   IN    std_logic_vector(31 DOWNTO 0);  -- sfix32_En23
          fftValid                        :   IN    std_logic;
          fftFramePulse                   :   IN    std_logic;
          passthrough                     :   IN    std_logic;  -- ufix1
          fftModifiedData_re              :   OUT   std_logic_vector(31 DOWNTO 0);  -- sfix32_En23
          fftModifiedData_im              :   OUT   std_logic_vector(31 DOWNTO 0);  -- sfix32_En23
          fftValidOut                     :   OUT   std_logic;
          fftFramePulseOut                :   OUT   std_logic
          );
  END COMPONENT;

  COMPONENT synthesis
    PORT( clk                             :   IN    std_logic;
          reset                           :   IN    std_logic;
          enb                             :   IN    std_logic;
          enb_1_2048_0                    :   IN    std_logic;
          enb_1_2048_1                    :   IN    std_logic;
          enb_1_1_1                       :   IN    std_logic;
          enb_1_2048_11                   :   IN    std_logic;
          fftModifiedData_re              :   IN    std_logic_vector(31 DOWNTO 0);  -- sfix32_En23
          fftModifiedData_im              :   IN    std_logic_vector(31 DOWNTO 0);  -- sfix32_En23
          fftValid                        :   IN    std_logic;
          fftFramePulse                   :   IN    std_logic;
          audioOut                        :   OUT   std_logic_vector(23 DOWNTO 0)  -- sfix24_En23
          );
  END COMPONENT;

  -- Signals
  SIGNAL enb                              : std_logic;
  SIGNAL enb_1_2048_0                     : std_logic;
  SIGNAL enb_1_2048_1                     : std_logic;
  SIGNAL enb_1_1_1                        : std_logic;
  SIGNAL enb_1_2048_11                    : std_logic;
  SIGNAL analysis_out1_re                 : std_logic_vector(31 DOWNTO 0);  -- ufix32
  SIGNAL analysis_out1_im                 : std_logic_vector(31 DOWNTO 0);  -- ufix32
  SIGNAL analysis_out2                    : std_logic;
  SIGNAL analysis_out3                    : std_logic;
  SIGNAL analysis_out4                    : std_logic;  -- ufix1
  SIGNAL frequencyDomainProcessing_out1_re : std_logic_vector(31 DOWNTO 0);  -- ufix32
  SIGNAL frequencyDomainProcessing_out1_im : std_logic_vector(31 DOWNTO 0);  -- ufix32
  SIGNAL frequencyDomainProcessing_out2   : std_logic;
  SIGNAL frequencyDomainProcessing_out3   : std_logic;
  SIGNAL synthesis_out1                   : std_logic_vector(23 DOWNTO 0);  -- ufix24

BEGIN
  u_fftAnalysisSynthesis_tc : fftAnalysisSynthesis_tc
    PORT MAP( clk => clk,
              reset => reset,
              clk_enable => clk_enable,
              enb => enb,
              enb_1_1_1 => enb_1_1_1,
              enb_1_2048_0 => enb_1_2048_0,
              enb_1_2048_1 => enb_1_2048_1,
              enb_1_2048_11 => enb_1_2048_11
              );

  u_analysis : analysis
    PORT MAP( clk => clk,
              reset => reset,
              enb => enb,
              enb_1_2048_0 => enb_1_2048_0,
              enb_1_2048_1 => enb_1_2048_1,
              enb_1_1_1 => enb_1_1_1,
              audioIn => audioIn,  -- sfix24_En23
              passthrough => passthrough,  -- ufix1
              fftData_re => analysis_out1_re,  -- sfix32_En23
              fftData_im => analysis_out1_im,  -- sfix32_En23
              fftValid => analysis_out2,
              fftFramePulse => analysis_out3,
              passthroughFast => analysis_out4  -- ufix1
              );

  u_frequencyDomainProcessing : frequencyDomainProcessing
    PORT MAP( clk => clk,
              reset => reset,
              enb => enb,
              gain_1 => gain_1,  -- ufix16_En14
              gain_2 => gain_2,  -- ufix16_En14
              gain_3 => gain_3,  -- ufix16_En14
              gain_4 => gain_4,  -- ufix16_En14
              gain_5 => gain_5,  -- ufix16_En14
              volume => volume,  -- ufix16_En16
              fftData_re => analysis_out1_re,  -- sfix32_En23
              fftData_im => analysis_out1_im,  -- sfix32_En23
              fftValid => analysis_out2,
              fftFramePulse => analysis_out3,
              passthrough => analysis_out4,  -- ufix1
              fftModifiedData_re => frequencyDomainProcessing_out1_re,  -- sfix32_En23
              fftModifiedData_im => frequencyDomainProcessing_out1_im,  -- sfix32_En23
              fftValidOut => frequencyDomainProcessing_out2,
              fftFramePulseOut => frequencyDomainProcessing_out3
              );

  u_synthesis : synthesis
    PORT MAP( clk => clk,
              reset => reset,
              enb => enb,
              enb_1_2048_0 => enb_1_2048_0,
              enb_1_2048_1 => enb_1_2048_1,
              enb_1_1_1 => enb_1_1_1,
              enb_1_2048_11 => enb_1_2048_11,
              fftModifiedData_re => frequencyDomainProcessing_out1_re,  -- sfix32_En23
              fftModifiedData_im => frequencyDomainProcessing_out1_im,  -- sfix32_En23
              fftValid => frequencyDomainProcessing_out2,
              fftFramePulse => frequencyDomainProcessing_out3,
              audioOut => synthesis_out1  -- sfix24_En23
              );

  ce_out <= enb_1_2048_1;

  audioOut <= synthesis_out1;

END rtl;

