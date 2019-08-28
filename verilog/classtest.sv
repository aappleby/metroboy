class C;
  int x;
  task set (int i);
    x = i;
  endtask
  function int get;
    return x;
  endfunction
endclass
