-- SPDX-License-Identifier: MIT
-- Copyright (c) 2022 Ross K. Snider.  All rights reserved.
---------------------------------------------------------------------------
-- This file is used in the book: Advanced Digital System Design using
-- System-on-Chip Field Programmable Gate Arrays
-- An Integrated Hardware/Software Approach
-- by Ross K. Snider
---------------------------------------------------------------------------
-- Authors:          Ross K. Snider
-- Company:          Montana State University
-- Create Date:      April 22, 2022
-- Revision:         1.0
-- License: MIT      (opensource.org/licenses/MIT)
-- Target Device(s): Terasic D1E0-Nano Board
-- Tool versions:    Quartus Prime 20.1
---------------------------------------------------------------------------
--
-- Design Name:      combFilterProcessor.vhd
--
-- Description:      VHDL file to be used by Platform Designer to create
--                   the combFilterProcessor component that has the
--                   following three interfaces:
--                       1. Avalon Streaming Sink
--                       2. Avalon Streaming Source
--                       1. Avalon Memory Mapped
--                   This file is a wrapper for the VHDL code
--                   combFilterSystem.vhd that was created by HDL Coder
--                   from the Simulink model combFilterFeedforward.slx
--
---------------------------------------------------------------------------
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
entity EQProcessor is
  port (
    clk                      : in    std_logic;
    reset                    : in    std_logic;
    avalon_st_sink_valid     : in    std_logic;
    avalon_st_sink_data      : in    std_logic_vector(23 downto 0);
    avalon_st_sink_channel   : in    std_logic_vector(0 downto 0);
    avalon_st_source_valid   : out   std_logic;
    avalon_st_source_data    : out   std_logic_vector(23 downto 0);
    avalon_st_source_channel : out   std_logic_vector(0 downto 0);
    avalon_mm_address        : in    std_logic_vector(2 downto 0);
    avalon_mm_read           : in    std_logic;
    avalon_mm_readdata       : out   std_logic_vector(31 downto 0);
    avalon_mm_write          : in    std_logic;
    avalon_mm_writedata      : in    std_logic_vector(31 downto 0)
  );
end entity EQProcessor;

architecture behavioral of EQProcessor is

  -- HDL Coder component we are interfacing with
  component fftAnalysisSynthesis is
    port (
      clk          : in    std_logic;
      reset        : in    std_logic;
      clk_enable   : in    std_logic;
      audioin      : in    std_logic_vector(23 downto 0);
      passthrough  : in    std_logic_vector(0 downto 0);
      gain_1       : in    std_logic_vector(15 downto 0);  -- ufix16_En14
      gain_2       : in    std_logic_vector(15 downto 0);  -- ufix16_En14
      gain_3       : in    std_logic_vector(15 downto 0);  -- ufix16_En14
      gain_4       : in    std_logic_vector(15 downto 0);  -- ufix16_En14
      gain_5       : in    std_logic_vector(15 downto 0);  -- ufix16_En14
      volume       : in    std_logic_vector(15 downto 0);  -- ufix16_En14
      ce_out       : out   std_logic;
      audioout     : out   std_logic_vector(23 downto 0)
    );
  end component fftAnalysisSynthesis;

  -- Avalon Streaming (Avalon-ST) to Left/Right
  component ast2lr is
    port (
      clk                 : in    std_logic;
      avalon_sink_data    : in    std_logic_vector(23 downto 0);
      avalon_sink_channel : in    std_logic;
      avalon_sink_valid   : in    std_logic;
      data_left           : out   std_logic_vector(23 downto 0);
      data_right          : out   std_logic_vector(23 downto 0)
    );
  end component ast2lr;

  -- Left/Right to Avalon Streaming (Avalon-ST)
  component lr2ast is
    port (
      clk                   : in    std_logic;
      avalon_sink_channel   : in    std_logic;
      avalon_sink_valid     : in    std_logic;
      data_left             : in    std_logic_vector(23 downto 0);
      data_right            : in    std_logic_vector(23 downto 0);
      avalon_source_data    : out   std_logic_vector(23 downto 0);
      avalon_source_channel : out   std_logic;
      avalon_source_valid   : out   std_logic
    );
  end component lr2ast;

  -- streaming internal signals
  signal left_data_sink    : std_logic_vector(23 downto 0);
  signal right_data_sink   : std_logic_vector(23 downto 0);
  signal left_data_source  : std_logic_vector(23 downto 0);
  signal right_data_source : std_logic_vector(23 downto 0);

  -- register signals
  -- Note: Left/right channels will be controlled from the same registers
  --       For independent control, create twice as many registers
  --       and prefix with left_ and right_
  --       This will require twice as many entries in the
  --       associated linux device driver
  signal passthrough    : std_logic_vector(0 downto 0) := "0";
  signal gain_1         : std_logic_vector(15 downto 0) := "0100000000000000";  -- ufix16_En14
  signal gain_2         : std_logic_vector(15 downto 0) := "0100000000000000";  -- ufix16_En14
  signal gain_3         : std_logic_vector(15 downto 0) := "0100000000000000";  -- ufix16_En14
  signal gain_4         : std_logic_vector(15 downto 0) := "0100000000000000";  -- ufix16_En14
  signal gain_5         : std_logic_vector(15 downto 0) := "0100000000000000";  -- ufix16_En14
  signal volume         : std_logic_vector(15 downto 0) := "1111111111111111";  -- ufix16_En14




begin

  -- Avalon Streaming (Avalon-ST) to Left/Right
  u_ast2lr : component ast2lr
    port map (
      clk                 => clk,
      avalon_sink_data    => avalon_st_sink_data,
      avalon_sink_channel => avalon_st_sink_channel(0),
      avalon_sink_valid   => avalon_st_sink_valid,
      data_left           => left_data_sink,
      data_right          => right_data_sink
    );

  -- Left/Right to Avalon Streaming (Avalon-ST)
  u_lr2ast : component lr2ast
    port map (
      clk                   => clk,
      avalon_sink_channel   => avalon_st_sink_channel(0),
      avalon_sink_valid     => avalon_st_sink_valid,
      data_left             => left_data_source,
      data_right            => right_data_source,
      avalon_source_data    => avalon_st_source_data,
      avalon_source_channel => avalon_st_source_channel(0),
      avalon_source_valid   => avalon_st_source_valid
    );

  -- HDL Coder components (left channel)
  left_fftanalysissynthesis : component fftAnalysisSynthesis
    port map (
      clk        => clk,
      reset      => reset,
      clk_enable => '1',
      audioin    => left_data_sink,
      gain_1    => gain_1,
      gain_2    => gain_2,
      gain_3    => gain_3,
      gain_4    => gain_4,
      gain_5    => gain_5,
      volume    => volume,
      passthrough  => passthrough,
      ce_out     => open,
      audioout   => left_data_source
    );

  -- HDL Coder components (right channel)
  right_fftanalysissynthesis : component fftAnalysisSynthesis
    port map (
      clk        => clk,
      reset      => reset,
      clk_enable => '1',
      audioin    => right_data_sink,
      gain_1    => gain_1,
      gain_2    => gain_2,
      gain_3    => gain_3,
      gain_4    => gain_4,
      gain_5    => gain_5,
      volume    => volume,
      passthrough  => passthrough,
      ce_out     => open,
      audioout   => right_data_source
    );

  -- Avalon Memory Mapped interface (CPU reading from registers)
  bus_read : process (clk) is
  begin

    if rising_edge(clk) and avalon_mm_read = '1' then

      case avalon_mm_address is
        when "000" =>
          avalon_mm_readdata <= std_logic_vector(resize(unsigned(passthrough), 32));
        when "001" =>
          avalon_mm_readdata <= std_logic_vector(resize(unsigned(gain_1), 32));
        when "010" =>
          avalon_mm_readdata <= std_logic_vector(resize(unsigned(gain_2), 32));
        when "011" =>
          avalon_mm_readdata <= std_logic_vector(resize(unsigned(gain_3), 32));
        when "100" =>
          avalon_mm_readdata <= std_logic_vector(resize(unsigned(gain_4), 32));
        when "101" =>
          avalon_mm_readdata <= std_logic_vector(resize(unsigned(gain_5), 32));
        when "110" =>
          avalon_mm_readdata <= std_logic_vector(resize(unsigned(volume), 32));
        when others =>
          avalon_mm_readdata <= (others => '0');
      end case;

    end if;

  end process bus_read;

  -- Avalon Memory Mapped interface (CPU writing to registers)
  bus_write : process (clk, reset) is
  begin

    if reset = '1' then
      passthrough    <= "0"; 
      gain_1         <= "0100000000000000"; 
      gain_2         <= "0100000000000000"; 
      gain_3         <= "0100000000000000"; 
      gain_4         <= "0100000000000000"; 
      gain_5         <= "0100000000000000";
      volume         <= "1111111111111111";
    elsif rising_edge(clk) and avalon_mm_write = '1' then

      case avalon_mm_address is

        when "000" =>
          passthrough <= std_logic_vector(resize(unsigned(avalon_mm_writedata), 1));
        when "001" =>
          gain_1 <= std_logic_vector(resize(unsigned(avalon_mm_writedata), 16));
        when "010" =>
          gain_2 <= std_logic_vector(resize(unsigned(avalon_mm_writedata), 16));
        when "011" =>
          gain_3 <= std_logic_vector(resize(unsigned(avalon_mm_writedata), 16));
        when "100" =>
          gain_4 <= std_logic_vector(resize(unsigned(avalon_mm_writedata), 16));
        when "101" =>
          gain_5 <= std_logic_vector(resize(unsigned(avalon_mm_writedata), 16));
        when "110" =>
          volume <= std_logic_vector(resize(unsigned(avalon_mm_writedata), 16));             
        when others =>
          null;

      end case;

    end if;

  end process bus_write;

end architecture behavioral;

