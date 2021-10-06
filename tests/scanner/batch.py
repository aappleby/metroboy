import os;

#=============

def test_sample(addr, x, y):
  print(F"test_sample 0x{addr:04x} at ({x},{y})")

  source = F"""
  .include "header.inc"

  main:
    lcd_off_unsafe

    reset_lcd_for_test
    long_delay ({x} + (114 * {y}) - 7)
    ld a, $55
    ld a, (${addr:04X})

    test_display_a
  """
  return source

#=============

def test_read(addr, x, y):
  print(F"test_read 0x{addr:04x} at ({x},{y})")

  source = F"""
  .include "header.inc"

  main:
    lcd_off_unsafe
    ld a, $55
    ld (${addr:04X}), a

    reset_lcd_for_test
    long_delay ({x} + (114 * {y}) - 7)
    ld a, $55
    ld a, (${addr:04X})

    test_display_a
  """
  return source

#=============

def test_write(addr, x, y):
  print(F"test_write 0x{addr:04x} at ({x},{y})")

  source = F"""
  .include "header.inc"

  main:
    reset_lcd_for_test
    long_delay ({x} + (114 * {y}) - 7)
    ld a, $55
    ld (${addr:04X}), a

    lcd_off_unsafe
    ld a, (${addr:04X})
    ld b, a
    lcd_on
    ld a, b
    test_display_a
  """
  return source

# y 10 x 110 = +
# y 10 x 111 = +
# y 10 x 112 = +
# y 10 x 113 = +
# y 10 x 114 = +
# y 10 x 115 = +
# y 10 x 116 = +
# y 10 x 117 = +

# 0x8000 write
# y  10 x   0 = +
# y  10 x  20 = +
# y  10 x  21 = X
# y  10 x  63 = X
# y  10 x  64 = +
# y  10 x 113 = +

# 0x8000 write
# y   0 x   0 =
# y   0 x  20 = +
# y   0 x  21 = x
# y   0 x  63 =
# y   0 x  64 =
# y   0 x 113 =

#=============

def compile_and_upload(source):
  file = open("sample.s", "w")
  file.write(source)
  file.close()

  os.system("wla-gb -o sample.o sample.s")
  os.system("wlalink sample.link sample.gb")
  os.system("./gb-live32.exe -u sample.gb > /dev/null")
  input("Press Enter to continue...")

#=============

y = 154
for x in [0, 1, 2, 3, 18, 19, 20, 21, 63, 64, 113, 114]:
  compile_and_upload(test_write(0x9000, x, y))


"""
oam read  : y  20 -|  0:X   |  1:    | 19:X    | 20:X   | 21:X   | 63:X   | 64:O   |113:O   |114:X   |
oam write : y  20 -|  0:O   |  1:    | 19:O    | 20:O   | 21:O   | 63:O   | 64:O   |113:O   |114:O   | // wtf is oam write lock just broken on dmg?
vram read : y  20 -|  0:O   |  1:    | 19:O    | 20:X   | 21:X   | 63:X   | 64:O   |113:O   |114:O   |
vram write: y  20 -|  0:O   |  1:    | 19:O    | 20:O   | 21:X   | 63:X   | 64:O   |113:O   |114:O   |
ly        : y  20 -|  0:20  |  1:20  | 19:20   | 20:20  | 21:20  | 63:20  | 64:20  |113:20  |114:21  |
stat      : y  20 -|  0:x80 |  1:x82 | 19:x82  | 20:x82 | 21:x83 | 63:x83 | 64:x80 |113:x80 |114:x80 |
oam read  : y 153 -|  0:    |  1:    | 19:     | 20:    | 21:    | 63:    | 64:    |113:    |114:    |
oam write : y 153 -|  0:O   |  1:    | 19:O    | 20:O   | 21:O   | 63:O   | 64:O   |113:O   |114:O   |
vram read : y 153 -|  0:    |  1:    | 19:     | 20:    | 21:    | 63:    | 64:    |113:    |114:    |
vram write: y 153 -|  0:O   |  1:    | 19:O    | 20:O   | 21:O   | 63:O   | 64:O   |113:O   |114:O   |
stat      : y 153 -|  0:x81 |  1:x81 | 19:x85  | 20:    | 21:    | 63:    | 64:    |113:    |114:    |


vram write: y 154 -|  0:+   |  1:    | 19:+    | 20:+   | 21:X   | 63:X   | 64:+   |113:+   |114:+   |


ags:
oam write : y  20 - 0:X 19:X 20:X 21:X 63:X 64:+ 113:+ 114:X // more reasonable on ags
oam write : y 153 - 0:+ 19:+ 20:+ 21:+ 63:+ 64:+ 113:+ 114:X
oam write : y 154 - 0:X 19:X 20:X 21:X 63:X 64:+ 113:+ 114:X

"""
