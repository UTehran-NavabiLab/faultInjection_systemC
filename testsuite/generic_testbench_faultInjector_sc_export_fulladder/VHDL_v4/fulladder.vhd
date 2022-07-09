LIBRARY IEEE;
USE IEEE.std_logic_1164.ALL;
USE IEEE.numeric_std.ALL;

entity fulladder_RTL is
    -- `i0`, `i1`, and the carry-in `ci` are inputs of the fulladder.
    -- `s` is the sum output, `co` is the carry-out.
    port (i0, i1 : in STD_LOGIC; ci : in STD_LOGIC; s : out STD_LOGIC; co : out STD_LOGIC);
  end fulladder_RTL;
  
architecture rtl of fulladder_RTL is
begin
  --  This full-adder architecture contains two concurrent assignments.
  --  Compute the sum.
  s <= i0 xor i1 xor ci;
  --  Compute the carry.
  co <= (i0 and i1) or (i0 and ci) or (i1 and ci);
end rtl;
