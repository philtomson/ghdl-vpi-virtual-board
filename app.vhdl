
-- Generated with IPGen v0.0.1 on Mon Nov 23 11:48:52 2020
-- https://gitlab.com/theotime_bollengier/vhdl_ip_generator

-- Input bit width: 14
-- Number of output decimal digits: 4
-- Leading zeros are hidden: true
-- Last decimal digit is omitted: true
-- Latency: 16
-- Throughput: 15
-- Entity name: binary_to_BCD_i14_o4_hld


library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;


entity binary_to_BCD_i14_o4_hld is
	port ( clk            : in  std_logic;
		   rstn           : in  std_logic;
		   data_in        : in  std_logic_vector(13 downto 0);
		   data_in_valid  : in  std_logic;
	       bcd_0          : out std_logic_vector(3 downto 0);
	       bcd_1          : out std_logic_vector(3 downto 0);
	       bcd_2          : out std_logic_vector(3 downto 0);
	       bcd_3          : out std_logic_vector(3 downto 0);
		   output_valid   : out std_logic);
end entity;


architecture behavioral of binary_to_BCD_i14_o4_hld is

	constant cst_3    : unsigned(3 downto 0) := "0011";
	constant cst_5    : unsigned(3 downto 0) := "0101";
	constant hide_msk : std_logic_vector(3 downto 0) := "1111";
	type   state_t is (wait_s, add_shift_s);
	signal state      : state_t := wait_s;
	signal counter    : integer range 0 to 13;
	signal in_shftr   : std_logic_vector(13 downto 0);
	signal bcd        : std_logic_vector(15 downto 0) := (others => '0');
	signal bcd_u_0    : unsigned(3 downto 0) := (others => '0');
	signal bcd_u_1    : unsigned(3 downto 0) := (others => '0');
	signal bcd_u_2    : unsigned(3 downto 0) := (others => '0');
	signal bcd_u_3    : unsigned(3 downto 0) := (others => '0');
	signal bcd_add_0  : unsigned(3 downto 0) := (others => '0');
	signal bcd_add_1  : unsigned(3 downto 0) := (others => '0');
	signal bcd_add_2  : unsigned(3 downto 0) := (others => '0');
	signal bcd_add_3  : unsigned(3 downto 0) := (others => '0');
	signal bcd_shft   : std_logic_vector(15 downto 0);
	signal valid_pipe : std_logic := '0';
	signal bcd_pipe_0 : std_logic_vector(3 downto 0) := (others => '0');
	signal bcd_pipe_1 : std_logic_vector(3 downto 0) := (others => '0');
	signal bcd_pipe_2 : std_logic_vector(3 downto 0) := (others => '0');
	signal bcd_pipe_3 : std_logic_vector(3 downto 0) := (others => '0');
	signal data_in_ready_r : std_logic := '1';

begin

	bcd_u_0 <= unsigned(bcd(3 downto 0));
	bcd_u_1 <= unsigned(bcd(7 downto 4));
	bcd_u_2 <= unsigned(bcd(11 downto 8));
	bcd_u_3 <= unsigned(bcd(15 downto 12));

	bcd_add_0 <= bcd_u_0 + cst_3 when bcd_u_0 >= cst_5 else bcd_u_0;
	bcd_add_1 <= bcd_u_1 + cst_3 when bcd_u_1 >= cst_5 else bcd_u_1;
	bcd_add_2 <= bcd_u_2 + cst_3 when bcd_u_2 >= cst_5 else bcd_u_2;
	bcd_add_3 <= bcd_u_3 + cst_3 when bcd_u_3 >= cst_5 else bcd_u_3;

	bcd_shft(3 downto 0) <= std_logic_vector(bcd_add_0(2 downto 0)) & in_shftr(13);
	bcd_shft(7 downto 4) <= std_logic_vector(bcd_add_1(2 downto 0)) & bcd_add_0(3);
	bcd_shft(11 downto 8) <= std_logic_vector(bcd_add_2(2 downto 0)) & bcd_add_1(3);
	bcd_shft(15 downto 12) <= std_logic_vector(bcd_add_3(2 downto 0)) & bcd_add_2(3);

	
	process (clk)
	begin
		if rising_edge(clk) then
			if rstn = '0' then
				output_valid <= '0';
				valid_pipe <= '0';
				data_in_ready_r <= '1';
				bcd_pipe_0 <= (others => '0');
				bcd_pipe_1 <= (others => '0');
				bcd_pipe_2 <= (others => '0');
				bcd_pipe_3 <= (others => '0');
				bcd_0 <= (others => '0');
				bcd_1 <= (others => '0');
				bcd_2 <= (others => '0');
				bcd_3 <= (others => '0');
				state <= wait_s;
			else
				case state is
					when wait_s =>
						valid_pipe <= '0';
						in_shftr <= data_in;
						bcd <= (others => '0');
						counter <= 0;
						if data_in_valid = '1' and data_in_ready_r = '1' then
							data_in_ready_r <= '0';
							state <= add_shift_s;
						else 
							data_in_ready_r <= '1';
						end if;

					when add_shift_s =>
						in_shftr <= in_shftr(12 downto 0) & '0';
						bcd <= bcd_shft;
						if counter = 13 then
							counter <= 0;
							bcd_pipe_0 <= bcd_shft(3 downto 0);
							bcd_pipe_1 <= bcd_shft(7 downto 4);
							bcd_pipe_2 <= bcd_shft(11 downto 8);
							bcd_pipe_3 <= bcd_shft(15 downto 12);
							valid_pipe <= '1';
							data_in_ready_r <= '1';
							state <= wait_s;
						else
							counter <= counter + 1;
						end if;
				end case;

				if bcd_pipe_3 = x"0" then
					bcd_3 <= hide_msk;
					if bcd_pipe_2 = x"0" then
						bcd_2 <= hide_msk;
						if bcd_pipe_1 = x"0" then
							bcd_1 <= hide_msk;
						else
							bcd_1 <= bcd_pipe_1;
						end if;
					else
						bcd_2 <= bcd_pipe_2;
						bcd_1 <= bcd_pipe_1;
					end if;
				else
					bcd_3 <= bcd_pipe_3;
					bcd_2 <= bcd_pipe_2;
					bcd_1 <= bcd_pipe_1;
				end if;
				bcd_0 <= bcd_pipe_0;
				output_valid <= valid_pipe;
			end if;
		end if;
	end process;

end architecture;


--------------------------------------------------------------------------------


library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;


-- Display numbers from 0 to 15 as hexadecimal digits, 
-- Use anode multiplexing display mode: a '0' is segment on, a '1' is segment off.
entity num_to_7seg_muxing is
	port ( num      : in  std_logic_vector(3 downto 0);
	       segments : out std_logic_vector(7 downto 0));
end entity;


architecture rtl of num_to_7seg_muxing is
begin
	with num select segments <=
	x"03" when x"0",
	x"9f" when x"1",
	x"25" when x"2",
	x"0d" when x"3",
	x"99" when x"4",
	x"49" when x"5",
	x"41" when x"6",
	x"1f" when x"7",
	x"01" when x"8",
	x"09" when x"9",
	x"11" when x"a",
	x"c1" when x"b",
	x"e5" when x"c",
	x"85" when x"d",
	x"61" when x"e",
	x"71" when others;
end architecture;


--------------------------------------------------------------------------------


library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;


-- Display numbers from 0 to 15 as hexadecimal digits, 
-- Use direct display mode: a '1' is segment on, a '0' is segment off.
entity num_to_7seg_direct is
	port ( num      : in  std_logic_vector(3 downto 0);
	       segments : out std_logic_vector(7 downto 0));
end entity;


architecture rtl of num_to_7seg_direct is
begin
	with num select segments <=
	x"fc" when x"0",
	x"60" when x"1",
	x"da" when x"2",
	x"f2" when x"3",
	x"66" when x"4",
	x"b6" when x"5",
	x"be" when x"6",
	x"e0" when x"7",
	x"fe" when x"8",
	x"f6" when x"9",
	x"ee" when x"a",
	x"3e" when x"b",
	x"1a" when x"c",
	x"7a" when x"d",
	x"9e" when x"e",
	x"8e" when others;
end architecture;


--------------------------------------------------------------------------------


library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;


-- Display numbers from 0 to 9 as decimal digits, 
-- numbers from 10 to 15 are off.
-- Use anode multiplexing display mode: a '0' is segment on, a '1' is segment off.
entity num_to_7seg_dec_muxing is
	port ( num      : in  std_logic_vector(3 downto 0);
	       segments : out std_logic_vector(7 downto 0));
end entity;


architecture rtl of num_to_7seg_dec_muxing is
begin
	with num select segments <=
	x"03" when x"0",
	x"9f" when x"1",
	x"25" when x"2",
	x"0d" when x"3",
	x"99" when x"4",
	x"49" when x"5",
	x"41" when x"6",
	x"1f" when x"7",
	x"01" when x"8",
	x"09" when x"9",
	x"ff" when others;
end architecture;


--------------------------------------------------------------------------------


library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;


-- Display numbers from 0 to 9 as decimal digits,
-- numbers from 10 to 15 are off.
-- Use direct display mode: a '1' is segment on, a '0' is segment off.
entity num_to_7seg_dec_direct is
	port ( num      : in  std_logic_vector(3 downto 0);
	       segments : out std_logic_vector(7 downto 0));
end entity;


architecture rtl of num_to_7seg_dec_direct is
begin
	with num select segments <=
	x"fc" when x"0",
	x"60" when x"1",
	x"da" when x"2",
	x"f2" when x"3",
	x"66" when x"4",
	x"b6" when x"5",
	x"be" when x"6",
	x"e0" when x"7",
	x"fe" when x"8",
	x"f6" when x"9",
	x"00" when others;
end architecture;


--------------------------------------------------------------------------------


library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;


entity display_annode_multiplexer is
	generic ( cnt_max : natural := 10 );
	port ( clk      : in  std_logic;
		   rstn     : in  std_logic;
	       display0 : in  std_logic_vector(7 downto 0);
	       display1 : in  std_logic_vector(7 downto 0);
	       display2 : in  std_logic_vector(7 downto 0);
	       display3 : in  std_logic_vector(7 downto 0);
	       display4 : in  std_logic_vector(7 downto 0);
	       display5 : in  std_logic_vector(7 downto 0);
	       display6 : in  std_logic_vector(7 downto 0);
	       display7 : in  std_logic_vector(7 downto 0);
	       anodes   : out std_logic_vector(7 downto 0);
	       cathodes : out std_logic_vector(7 downto 0));
end entity;


architecture rtl of display_annode_multiplexer is

	signal cnt : integer range 0 to cnt_max - 1;
	signal sel : std_logic_vector(2 downto 0);

begin

	process (clk) is
	begin
		if rising_edge(clk) then
			if rstn = '0' then
				cnt <= 0;
				sel <= "000";
			else
				if cnt = (cnt_max - 1) then
					cnt <= 0;
					sel <= std_logic_vector(unsigned(sel) + 1);
				else
					cnt <= cnt + 1;
				end if;
			end if;
		end if;
	end process;

	process (clk) is
	begin
		if rising_edge(clk) then
			if rstn = '0' then
				cathodes <= x"ff";
				anodes <= x"ff";
			else
				case sel is
					when "001" =>
						cathodes <= display1;
						anodes <= "11111101";
					when "010" =>
						cathodes <= display2;
						anodes <= "11111011";
					when "011" =>
						cathodes <= display3;
						anodes <= "11110111";
					when "100" =>
						cathodes <= display4;
						anodes <= "11101111";
					when "101" =>
						cathodes <= display5;
						anodes <= "11011111";
					when "110" =>
						cathodes <= display6;
						anodes <= "10111111";
					when "111" =>
						cathodes <= display7;
						anodes <= "01111111";
					when others =>
						cathodes <= display0;
						anodes <= "11111110";
				end case;
			end if;
		end if;
	end process;

end architecture;


--------------------------------------------------------------------------------


library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;


entity rising_edge_detect is
	port ( clk    : in  std_logic;
	       input  : in  std_logic;
	       strobe : out std_logic);
end entity;


architecture rtl of rising_edge_detect is
	signal r : std_logic;
begin
	process (clk) is
	begin
		if rising_edge(clk) then
			r <= input;
		end if;
	end process;
	strobe <= input and not(r);
end architecture;


--------------------------------------------------------------------------------


library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;


entity button_events is
	port ( clk      : in  std_logic;
		   button_l : in  std_logic;
		   button_r : in  std_logic;
		   button_c : in  std_logic;
		   button_u : in  std_logic;
		   button_d : in  std_logic;
	       bel      : out std_logic;
	       ber      : out std_logic;
	       bec      : out std_logic;
	       beu      : out std_logic;
	       bed      : out std_logic);
end entity;


architecture rtl of button_events is
begin
	rising_edge_l: entity work.rising_edge_detect
	port map ( clk    => clk,
	           input  => button_l,
			   strobe => bel);

	rising_edge_r: entity work.rising_edge_detect
	port map ( clk    => clk,
	           input  => button_r,
			   strobe => ber);

	rising_edge_c: entity work.rising_edge_detect
	port map ( clk    => clk,
	           input  => button_c,
			   strobe => bec);

	rising_edge_u: entity work.rising_edge_detect
	port map ( clk    => clk,
	           input  => button_u,
			   strobe => beu);

	rising_edge_d: entity work.rising_edge_detect
	port map ( clk    => clk,
	           input  => button_d,
			   strobe => bed);

end architecture;


--------------------------------------------------------------------------------


library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;


-- use anode multiplexing for displays
entity example_mux is
	generic ( cnt_max : natural := 500 );
	port ( clk            : in  std_logic;
		   rstn           : in  std_logic;
	       switches       : in  std_logic_vector(15 downto 0);
	       leds           : out std_logic_vector(15 downto 0);
		   button_l       : in  std_logic;
		   button_r       : in  std_logic;
		   button_c       : in  std_logic;
		   button_u       : in  std_logic;
		   button_d       : in  std_logic;
	       anodes         : out std_logic_vector(7 downto 0);
	       cathodes       : out std_logic_vector(7 downto 0);
	       rgb0           : out std_logic_vector(2 downto 0);
	       rgb1           : out std_logic_vector(2 downto 0));
end entity;


architecture rtl of example_mux is

	signal c           : std_logic;
	signal l           : std_logic;
	signal r           : std_logic;
	signal u           : std_logic;
	signal d           : std_logic;
	signal sr          : std_logic_vector(14 downto 0) := "100000000000000";
	signal cnt         : integer range 0 to 9999 := 42;
	signal cntv        : std_logic_vector(13 downto 0);
	signal dv          : std_logic := '1';
	signal bcd0        : std_logic_vector(3 downto 0);
	signal bcd1        : std_logic_vector(3 downto 0);
	signal bcd2        : std_logic_vector(3 downto 0);
	signal bcd3        : std_logic_vector(3 downto 0);
	signal bcd0r       : std_logic_vector(3 downto 0);
	signal bcd1r       : std_logic_vector(3 downto 0);
	signal bcd2r       : std_logic_vector(3 downto 0);
	signal bcd3r       : std_logic_vector(3 downto 0);
	signal bcdv        : std_logic;
	signal display_cnt : std_logic;
	signal blink       : std_logic;
	signal bcnt        : integer range 0 to cnt_max - 1;
	signal rgbcnt      : std_logic_vector(3 downto 0);
	signal display0    : std_logic_vector(7 downto 0);
	signal display1    : std_logic_vector(7 downto 0);
	signal display2    : std_logic_vector(7 downto 0);
	signal display3    : std_logic_vector(7 downto 0);
	signal display4    : std_logic_vector(7 downto 0);
	signal display5    : std_logic_vector(7 downto 0);
	signal display6    : std_logic_vector(7 downto 0);
	signal display7    : std_logic_vector(7 downto 0);

begin

	cntv <= std_logic_vector(to_unsigned(cnt, 14));

	button_detects: entity work.button_events
	port map ( clk      => clk,
		       button_l => button_l,
		       button_r => button_r,
		       button_c => button_c,
		       button_u => button_u,
		       button_d => button_d,
	           bel      => l,
	           ber      => r,
	           bec      => c,
	           beu      => u,
	           bed      => d);

	-- led shift register
	process(clk)
	begin
		if rising_edge(clk) then
			if rstn = '0' then
				sr <= "100000000000000";
			elsif l = '1' then
				sr <= sr(13 downto 0) & sr(14);
			elsif r = '1' then
				sr <= sr(0) & sr(14 downto 1);
			end if;
		end if;
	end process;

	-- switch led output between display counter and shift register
	process(clk)
	begin
		if rising_edge(clk) then
			if rstn = '0' then
				display_cnt <= '0';
			elsif c = '1' then
				display_cnt <= not(display_cnt);
			end if;
		end if;
	end process;
	leds(14 downto 0) <= ('0' & cntv) when display_cnt = '1' else sr;
	--leds(14 downto 0) <= "100110-HLWZ10XU";
	leds(15) <= blink;

	-- blink counter
	process(clk)
	begin
		if rising_edge(clk) then
			if rstn = '0' then
				bcnt <= 0;
				blink <= '0';
				rgbcnt <= (others => '0');
			elsif bcnt >= (cnt_max - 1) then
				bcnt <= 0;
				blink <= not(blink);
				rgbcnt <= std_logic_vector(unsigned(rgbcnt) + 1);
			else
				bcnt <= bcnt + 1;
			end if;
		end if;
	end process;

	rgb0 <= rgbcnt(3 downto 1) when (rgbcnt(0) = '0' and rstn = '1') else "000";
	rgb1 <= switches(2 downto 0);

	disp4: entity work.num_to_7seg_muxing
	port map ( num      => switches(3 downto 0),
	           segments => display4);

	disp5: entity work.num_to_7seg_muxing
	port map ( num      => switches(7 downto 4),
	           segments => display5);

	disp6: entity work.num_to_7seg_muxing
	port map ( num      => switches(11 downto 8),
	           segments => display6);

	disp7: entity work.num_to_7seg_muxing
	port map ( num      => switches(15 downto 12),
	           segments => display7);

	-- display counter
	process(clk)
	begin
		if rising_edge(clk) then
			if rstn = '0' then
				cnt <= 42;
				dv <= '1';
			elsif u = '1' then
				if cnt >= 9999 then
					cnt <= 0;
				else
					cnt <= cnt + 1;
				end if;
				dv <= '1';
			elsif d = '1' then
				if cnt = 0 then
					cnt <= 9999;
				else
					cnt <= cnt - 1;
				end if;
				dv <= '1';
			else
				dv <= '0';
			end if;
		end if;
	end process;

	bin2bcd: entity work.binary_to_BCD_i14_o4_hld
	port map ( clk            => clk,
		       rstn           => rstn,
		       data_in        => cntv,
		       data_in_valid  => dv,
	           bcd_0          => bcd0,
	           bcd_1          => bcd1,
	           bcd_2          => bcd2,
	           bcd_3          => bcd3,
		       output_valid   => bcdv);

	process(clk)
	begin
		if rising_edge(clk) then
			if rstn = '0' then
				bcd0r <= (others => '0');
				bcd1r <= (others => '0');
				bcd2r <= (others => '0');
				bcd3r <= (others => '0');
			elsif bcdv = '1' then
				bcd0r <= bcd0;
				bcd1r <= bcd1;
				bcd2r <= bcd2;
				bcd3r <= bcd3;
			end if;
		end if;
	end process;

	disp0: entity work.num_to_7seg_dec_muxing
	port map (num      => bcd0r,
	          segments => display0);

	disp1: entity work.num_to_7seg_dec_muxing
	port map (num      => bcd1r,
	          segments => display1);

	disp2: entity work.num_to_7seg_dec_muxing
	port map (num      => bcd2r,
	          segments => display2);

	disp3: entity work.num_to_7seg_dec_muxing
	port map (num      => bcd3r,
	          segments => display3);

	annode_multiplexer: entity work.display_annode_multiplexer
	generic map ( cnt_max => 4 ) -- takes 4*8=32 ms to display the 8 digits
	port map ( clk      => clk,
		       rstn     => rstn,
	           display0 => display0,
	           display1 => display1,
	           display2 => display2,
	           display3 => display3,
	           display4 => display4,
	           display5 => display5,
	           display6 => display6,
	           display7 => display7,
	           anodes   => anodes,
	           cathodes => cathodes);

end architecture;


--------------------------------------------------------------------------------


library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;


-- use direct mode for displays
entity example_direct is
	generic ( cnt_max : natural := 500 );
	port ( clk            : in  std_logic;
		   rstn           : in  std_logic;
	       switches       : in  std_logic_vector(15 downto 0);
	       leds           : out std_logic_vector(15 downto 0);
		   button_l       : in  std_logic;
		   button_r       : in  std_logic;
		   button_c       : in  std_logic;
		   button_u       : in  std_logic;
		   button_d       : in  std_logic;
	       display0       : out std_logic_vector(7 downto 0);
	       display1       : out std_logic_vector(7 downto 0);
	       display2       : out std_logic_vector(7 downto 0);
	       display3       : out std_logic_vector(7 downto 0);
	       display4       : out std_logic_vector(7 downto 0);
	       display5       : out std_logic_vector(7 downto 0);
	       display6       : out std_logic_vector(7 downto 0);
	       display7       : out std_logic_vector(7 downto 0);
	       rgb0           : out std_logic_vector(2 downto 0);
	       rgb1           : out std_logic_vector(2 downto 0));
end entity;


architecture rtl of example_direct is

	signal c           : std_logic;
	signal l           : std_logic;
	signal r           : std_logic;
	signal u           : std_logic;
	signal d           : std_logic;
	signal sr          : std_logic_vector(14 downto 0) := "100000000000000";
	signal cnt         : integer range 0 to 9999 := 42;
	signal cntv        : std_logic_vector(13 downto 0);
	signal dv          : std_logic := '1';
	signal bcd0        : std_logic_vector(3 downto 0);
	signal bcd1        : std_logic_vector(3 downto 0);
	signal bcd2        : std_logic_vector(3 downto 0);
	signal bcd3        : std_logic_vector(3 downto 0);
	signal bcd0r       : std_logic_vector(3 downto 0);
	signal bcd1r       : std_logic_vector(3 downto 0);
	signal bcd2r       : std_logic_vector(3 downto 0);
	signal bcd3r       : std_logic_vector(3 downto 0);
	signal bcdv        : std_logic;
	signal display_cnt : std_logic;
	signal blink       : std_logic;
	signal bcnt        : integer range 0 to cnt_max - 1;
	signal rgbcnt      : std_logic_vector(3 downto 0);

begin

	cntv <= std_logic_vector(to_unsigned(cnt, 14));

	button_detects: entity work.button_events
	port map ( clk      => clk,
		       button_l => button_l,
		       button_r => button_r,
		       button_c => button_c,
		       button_u => button_u,
		       button_d => button_d,
	           bel      => l,
	           ber      => r,
	           bec      => c,
	           beu      => u,
	           bed      => d);

	-- led shift register
	process(clk)
	begin
		if rising_edge(clk) then
			if rstn = '0' then
				sr <= "100000000000000";
			elsif l = '1' then
				sr <= sr(13 downto 0) & sr(14);
			elsif r = '1' then
				sr <= sr(0) & sr(14 downto 1);
			end if;
		end if;
	end process;

	-- switch led output between display counter and shift register
	process(clk)
	begin
		if rising_edge(clk) then
			if rstn = '0' then
				display_cnt <= '0';
			elsif c = '1' then
				display_cnt <= not(display_cnt);
			end if;
		end if;
	end process;
	leds(14 downto 0) <= ('0' & cntv) when display_cnt = '1' else sr;
	--leds(14 downto 0) <= "100110-HLWZ10XU";
	leds(15) <= blink;

	-- blink counter
	process(clk)
	begin
		if rising_edge(clk) then
			if rstn = '0' then
				bcnt <= 0;
				blink <= '0';
				rgbcnt <= (others => '0');
			elsif bcnt >= (cnt_max - 1) then
				bcnt <= 0;
				blink <= not(blink);
				rgbcnt <= std_logic_vector(unsigned(rgbcnt) + 1);
			else
				bcnt <= bcnt + 1;
			end if;
		end if;
	end process;

	rgb0 <= rgbcnt(3 downto 1) when (rgbcnt(0) = '0' and rstn = '1') else "000";
	rgb1 <= switches(2 downto 0);

	disp4: entity work.num_to_7seg_direct
	port map ( num      => switches(3 downto 0),
	           segments => display4);

	disp5: entity work.num_to_7seg_direct
	port map ( num      => switches(7 downto 4),
	           segments => display5);

	disp6: entity work.num_to_7seg_direct
	port map ( num      => switches(11 downto 8),
	           segments => display6);

	disp7: entity work.num_to_7seg_direct
	port map ( num      => switches(15 downto 12),
	           segments => display7);

	-- display counter
	process(clk)
	begin
		if rising_edge(clk) then
			if rstn = '0' then
				cnt <= 42;
				dv <= '1';
			elsif u = '1' then
				if cnt >= 9999 then
					cnt <= 0;
				else
					cnt <= cnt + 1;
				end if;
				dv <= '1';
			elsif d = '1' then
				if cnt = 0 then
					cnt <= 9999;
				else
					cnt <= cnt - 1;
				end if;
				dv <= '1';
			else
				dv <= '0';
			end if;
		end if;
	end process;

	bin2bcd: entity work.binary_to_BCD_i14_o4_hld
	port map ( clk            => clk,
		       rstn           => rstn,
		       data_in        => cntv,
		       data_in_valid  => dv,
	           bcd_0          => bcd0,
	           bcd_1          => bcd1,
	           bcd_2          => bcd2,
	           bcd_3          => bcd3,
		       output_valid   => bcdv);

	process(clk)
	begin
		if rising_edge(clk) then
			if rstn = '0' then
				bcd0r <= (others => '0');
				bcd1r <= (others => '0');
				bcd2r <= (others => '0');
				bcd3r <= (others => '0');
			elsif bcdv = '1' then
				bcd0r <= bcd0;
				bcd1r <= bcd1;
				bcd2r <= bcd2;
				bcd3r <= bcd3;
			end if;
		end if;
	end process;

	disp0: entity work.num_to_7seg_dec_direct
	port map (num      => bcd0r,
	          segments => display0);

	disp1: entity work.num_to_7seg_dec_direct
	port map (num      => bcd1r,
	          segments => display1);

	disp2: entity work.num_to_7seg_dec_direct
	port map (num      => bcd2r,
	          segments => display2);

	disp3: entity work.num_to_7seg_dec_direct
	port map (num      => bcd3r,
	          segments => display3);

end architecture;


