
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


entity num_to_7seg is
	port ( num      : in  std_logic_vector(3 downto 0);
	       segments : out std_logic_vector(7 downto 0));
end entity;


architecture rtl of num_to_7seg is
begin
	with num select segments <=
	x"3f" when x"0",
	x"06" when x"1",
	x"5b" when x"2",
	x"4f" when x"3",
	x"66" when x"4",
	x"6d" when x"5",
	x"7d" when x"6",
	x"07" when x"7",
	x"7f" when x"8",
	x"6f" when x"9",
	x"77" when x"a",
	x"7c" when x"b",
	x"58" when x"c",
	x"5e" when x"d",
	x"79" when x"e",
	x"71" when others;
end architecture;


--------------------------------------------------------------------------------


library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;


entity num_to_7seg_dec is
	port ( num      : in  std_logic_vector(3 downto 0);
	       segments : out std_logic_vector(7 downto 0));
end entity;


architecture rtl of num_to_7seg_dec is
begin
	with num select segments <=
	x"3f" when x"0",
	x"06" when x"1",
	x"5b" when x"2",
	x"4f" when x"3",
	x"66" when x"4",
	x"6d" when x"5",
	x"7d" when x"6",
	x"07" when x"7",
	x"7f" when x"8",
	x"6f" when x"9",
	x"00" when others;
end architecture;


--------------------------------------------------------------------------------


library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;


entity test is
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


architecture rtl of test is
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

	-- led shift register
	process(clk)
	begin
		if rising_edge(clk) then
			l <= button_l;
			r <= button_r;
			if rstn = '0' then
				sr <= "100000000000000";
			elsif button_l = '1' and l = '0' then
				sr <= sr(13 downto 0) & sr(14);
			elsif button_r = '1' and r = '0' then
				sr <= sr(0) & sr(14 downto 1);
			end if;
		end if;
	end process;

	-- switch led output between display counter and shift register
	process(clk)
	begin
		if rising_edge(clk) then
			c <= button_c;
			if rstn = '0' then
				display_cnt <= '0';
			elsif button_c = '1' and c = '0' then
				display_cnt <= not(display_cnt);
			end if;
		end if;
	end process;
	leds(14 downto 0) <= ('0' & cntv) when display_cnt = '1' else sr;
	--leds(14 downto 0) <= "100110000001111";
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

	disp4: entity work.num_to_7seg
	port map ( num      => switches(3 downto 0),
	           segments => display4);

	disp5: entity work.num_to_7seg
	port map ( num      => switches(7 downto 4),
	           segments => display5);

	disp6: entity work.num_to_7seg
	port map ( num      => switches(11 downto 8),
	           segments => display6);

	disp7: entity work.num_to_7seg
	port map ( num      => switches(15 downto 12),
	           segments => display7);

	-- display counter
	process(clk)
	begin
		if rising_edge(clk) then
			u <= button_u;
			d <= button_d;
			if rstn = '0' then
				cnt <= 42;
				dv <= '1';
			elsif button_u = '1' and u = '0' then
				if cnt >= 9999 then
					cnt <= 0;
				else
					cnt <= cnt + 1;
				end if;
				dv <= '1';
			elsif button_d = '1' and d = '0' then
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

	disp0: entity work.num_to_7seg_dec
	port map (num      => bcd0r,
	          segments => display0);

	disp1: entity work.num_to_7seg_dec
	port map (num      => bcd1r,
	          segments => display1);

	disp2: entity work.num_to_7seg_dec
	port map (num      => bcd2r,
	          segments => display2);

	disp3: entity work.num_to_7seg_dec
	port map (num      => bcd3r,
	          segments => display3);

end architecture;


