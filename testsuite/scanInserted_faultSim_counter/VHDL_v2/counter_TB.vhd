LIBRARY IEEE;
USE IEEE.std_logic_1164.ALL;
USE STD.TEXTIO.ALL;
USE IEEE.STD_LOGIC_TEXTIO.ALL;
USE IEEE.numeric_std.ALL;
USE IEEE.std_logic_unsigned.ALL;
USE IEEE.math_real.ALL; 

ENTITY counter_TB_vhd IS
END ENTITY counter_TB_vhd;

ARCHITECTURE test OF counter_TB_vhd IS

	CONSTANT numDFF    : INTEGER := 4;
	CONSTANT sizePI    : INTEGER := 1;
	CONSTANT sizePO    : INTEGER := 5;
	CONSTANT clkPeriod : time := 10 ns;
	
	SIGNAL clk : STD_LOGIC := '1';
	SIGNAL rst : STD_LOGIC := '0';
	SIGNAL NbarT, Si, global_reset, So : STD_LOGIC;
	SIGNAL PI : STD_LOGIC_VECTOR (0 TO sizePI-1);
	SIGNAL PO : STD_LOGIC_VECTOR (0 TO sizePO-1);
	SIGNAL faultInjection : STD_LOGIC := '0';

BEGIN	
	FUT : ENTITY WORK.counter 
			PORT MAP (
				clk => clk, 
				rst => rst, 
				en  => PI(0),
				global_reset => global_reset,
				NbarT => NbarT,
				Si => Si,
				So => So,
				co  => PO(0), 
				counter => PO(1 TO 4)
			);
	
	clk <= NOT clk AFTER clkPeriod/2;
	
	FI:PROCESS 
		VARIABLE detected : STD_LOGIC := '0';
		VARIABLE flag : STD_LOGIC := '0';
		VARIABLE numOfDetecteds : INTEGER := 0;
		VARIABLE numOfFaults : INTEGER := 0;
		VARIABLE pre_expected_st, cur_expected_st, load_st, saved_st : STD_LOGIC_VECTOR (1 TO numDFF);
		VARIABLE expected_PO, sampledPO : STD_LOGIC_VECTOR (0 TO sizePO-1);
		VARIABLE index : INTEGER;
	    FILE faultFile, testFile, reportFile : TEXT; 
        VARIABLE fstatusR, fstatusW : FILE_OPEN_STATUS;
	    VARIABLE lbufR, lbufW : LINE;
	    VARIABLE testLine : STD_LOGIC_VECTOR (1 TO sizePO + sizePI + (2 * numDFF));
		variable str : string(1 to 100);
		variable strSize : INTEGER;
		VARIABLE wireName : STRING(1 TO 100);
		VARIABLE stuckAtVal : STD_LOGIC;
		VARIABLE coverage : REAL;
	BEGIN
		
		FILE_OPEN (fstatusW, reportFile, "reportFile.txt", write_mode);
		FILE_OPEN (fstatusR, faultFile, "fault_list__counter.flt", read_mode);
		numOfFaults := 0;
		numOfDetecteds := 0;
		WHILE (NOT ENDFILE (faultFile)) LOOP
			READLINE (faultFile, lbufR);
			assert lbufR'length < str'length;  
			str := (others => ' '); 
			strSize := lbufR'length;
			read(lbufR, str(1 to strSize));
			wireName := (others => ' ');
			wireName(1 TO strSize-2) := str(1 to strSize-2);
			stuckAtVal := STD_LOGIC'value(str(strSize to strSize));
			faultInjection <= '1';
			numOfFaults := numOfFaults + 1;
			
			WRITE (lbufW, string'("faultNum = "));
			WRITE (lbufW, numOfFaults);
			WRITE (lbufW, string'(" is injected @ "));
			WRITE (lbufW, NOW);
			
			FILE_OPEN (fstatusR, testFile, "counter.pat", read_mode);
		    detected := '0';
			flag := '0';
			global_reset <= '1'; 
			WAIT UNTIL clk = '1';
			global_reset <= '0';
			
			WHILE (NOT ENDFILE (testFile) AND detected = '0') LOOP
				
				pre_expected_st := cur_expected_st;
				
				READLINE (testFile, lbufR);
				READ (lbufR, testLine);
				load_st := testLine(1 TO numDFF);
				PI(0) <= testLine(numDFF+sizePI); --testLine(numDFF+1 TO numDFF+sizePI);
				expected_PO := testLine(numDFF+sizePI+1 TO numDFF+sizePI+sizePO);
				cur_expected_st := testLine(numDFF+sizePI+sizePO+1 TO 2*numDFF+sizePI+sizePO);
				
				NbarT <= '1';
				index := numDFF;
				FOR i IN 0 TO numDFF-1 LOOP   
					Si	<=  load_st (index);
					WAIT UNTIL clk = '1';
					saved_st(index) := So;
					index := index - 1;
				END LOOP; 
				
				NbarT <= '0';
				WAIT UNTIL clk = '1';
				sampledPO := PO;
				
				IF (flag = '0') THEN
					flag := '1';
					IF (expected_PO /= sampledPO) THEN
						detected := '1';
						WRITE (lbufW, string'(", detected by testVector = "));
						WRITE (lbufW, testLine);
						WRITE (lbufW, string'(" @ "));
						WRITE (lbufW, NOW);
						WRITELINE (reportFile, lbufW);
					END IF;
--					REPORT "I am in the flag part @" & TIME'IMAGE(NOW) & " detected is " &  STD_LOGIC'IMAGE(detected)
--					& " expected_PO = " &  INTEGER'IMAGE(to_integer(unsigned(expected_PO)))
--					& " sampledPO = " &  INTEGER'IMAGE(to_integer(unsigned(sampledPO)));
				ELSE
					IF ((pre_expected_st & expected_PO) /= (saved_st & SampledPO)) THEN
						detected := '1';
						WRITE (lbufW, string'(", detected by testVector = "));
						WRITE (lbufW, testLine);
						WRITE (lbufW, string'(" @ "));
						WRITE (lbufW, NOW);
						WRITELINE (reportFile, lbufW);
					END IF;
--					REPORT "I am in the non-flag part @" & TIME'IMAGE(NOW) & "detected is " & STD_LOGIC'IMAGE(detected)
--					& " expected_PO = " &  INTEGER'IMAGE(to_integer(unsigned(expected_PO)))
--					& " sampledPO = " &  INTEGER'IMAGE(to_integer(unsigned(sampledPO)))
--					& " pre_expected_st = " &  INTEGER'IMAGE(to_integer(unsigned(pre_expected_st)))
--					& " saved_st = " &  INTEGER'IMAGE(to_integer(unsigned(saved_st)));
				END IF;
			END LOOP; 	
			
			IF (detected /= '1') THEN
--				REPORT "I am in the part for the last test pattern @" & TIME'IMAGE(NOW);
				NbarT <= '1';
				index := numDFF;
				FOR i IN 0 TO numDFF-1 LOOP         
					Si	<=  load_st (index);
					WAIT UNTIL clk = '1';  
					saved_st(index) := So;
					index := index - 1;
				END LOOP; 
				IF (saved_st /= cur_expected_st) THEN
					detected := '1';
					WRITE (lbufW, string'(", detected by testVector = "));
					WRITE (lbufW, testLine);
					WRITE (lbufW, string'(" @ "));
					WRITE (lbufW, NOW);
					WRITELINE (reportFile, lbufW);
				END IF;
			END IF;
			
			FILE_CLOSE (testFile);
			IF (detected = '1') THEN
				numOfDetecteds := numOfDetecteds + 1;
			END IF;
			
			faultInjection <= '0';
			WAIT FOR 0 ns;
		END LOOP;
		FILE_CLOSE(faultFile);
		FILE_CLOSE (reportFile);
		
		coverage := REAL(numOfDetecteds / numOfFaults);
		REPORT "numOfDetecteds: " &INTEGER'IMAGE(numOfDetecteds); 
		REPORT "numOfFaults: " &INTEGER'IMAGE(numOfFaults); 
		REPORT "coverage: " &REAL'IMAGE(coverage);
		WAIT;
	END PROCESS;
END ARCHITECTURE test;