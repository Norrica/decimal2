import itertools
import os
from ctypes import *
from typing import Any
import decimal

context = decimal.getcontext()
LIB = '/s21_decimallib.so'
C_decimal = CDLL(os.getcwd() + LIB)


def bits(a):
    return f"{a:b}"


class MyDecimal(Structure):
    _fields_ = [
        ("bits", (c_uint * 4))
    ]

    def __init__(self, *args: Any, **kw: Any):
        super().__init__(*args, **kw)
        if (kw.get('data')):
            self.bits = (c_uint * 4)(*kw['data'])
        if (kw.get('val')):
            a: int = kw.get('val')
            b = a.to_bytes(12, 'little')
            byte_type = (c_ubyte * 12)
            int_type = (c_uint * 4)
            bys = byte_type(*b)
            ints_val = int_type.from_address(addressof(bys))
            self.bits[0] = ints_val[0]
            self.bits[1] = ints_val[1]
            self.bits[2] = ints_val[2]
        if kw.get('exp'):
            if 0 <= kw.get('exp') <= 28:
                self.bits[3] |= kw.get('exp') << 16
        if kw.get('sign'):
            self.bits[3] |= kw.get('sign') << 31

    def full_value(self):
        sign = -1 if self.bits[3] >> 30 == 1 else 1
        exp = ((self.bits[3] << 1) >> 17) & 255  # первые 8 бит
        res = decimal.Decimal((self.bits[0] + self.bits[1] * 4294967296 + self.bits[2] * 4294967296 ** 2) * sign)
        if exp > 0:
            res /= 10 ** exp
        return res

    def uint_value(self):
        return ((self.bits[0] + self.bits[1] * 4294967296 + self.bits[2] * 4294967296 ** 2))

    def __eq__(self, other):
        if type(other) is decimal.Decimal:
            return 'ww'


# decimal.s21_add.restype = c_int
# decimal.s21_add.argtypes = [Decimal, Decimal, POINTER(Decimal)]
#
# decimal.s21_sub.restype = c_int
# decimal.s21_sub.argtypes = [Decimal, Decimal, POINTER(Decimal)]

math_funcs = ["s21_add",
              "s21_sub",
              "s21_mul",
              # "s21_div"
              # "s21_mod"
              ]

for f in math_funcs:
    func = getattr(C_decimal, f)
    # print(func)
    func.restype = c_int
    func.argtypes = [MyDecimal, MyDecimal, POINTER(MyDecimal)]

comp_funcs = [
    "s21_is_less",
    "s21_is_less_or_equal",
    "s21_is_greater",
    "s21_is_greater_or_equal",
    "s21_is_equal",
    "s21_is_not_equal"]

for f in comp_funcs:
    func = getattr(C_decimal, f)
    func.restype = c_int
    func.argtypes = [MyDecimal, MyDecimal]

# convert_funcs
C_decimal.s21_from_int_to_decimal.restype = c_int
C_decimal.s21_from_int_to_decimal.argtypes = [c_int, POINTER(MyDecimal)]
C_decimal.s21_from_float_to_decimal.restype = c_int
C_decimal.s21_from_float_to_decimal.argtypes = [c_float, POINTER(MyDecimal)]
C_decimal.s21_from_decimal_to_int.restype = c_int
C_decimal.s21_from_decimal_to_int.argtypes = [MyDecimal, POINTER(c_int)]
C_decimal.s21_from_decimal_to_float.restype = c_int
C_decimal.s21_from_decimal_to_float.argtypes = [MyDecimal, POINTER(c_float)]

round_funcs = [
    "s21_floor",
    "s21_round",
    "s21_truncate",
    "s21_negate"
]
for f in comp_funcs:
    func = getattr(C_decimal, f)
    func.restype = c_int
    func.argtypes = [MyDecimal, POINTER(MyDecimal)]

total = 0
succs = 0
fails = 0

EDGE1 = 0xFFFFFFFF
EDGE2 = 0xFFFFFFFFFFFFFFFF
EDGE3 = 0xFFFFFFFFFFFFFFFFFFFFFFFF

def edge_sum():
    global succs, fails, total
    signs = [1,0]
    exps = list(range(29))
    # exps = [26]
    cases = list(itertools.permutations([EDGE1, EDGE2, EDGE3], 2))
    for sign in signs:
        for case in cases:
            for e in exps:
                passed = False
                c1 = (decimal.Decimal(case[0]) / 10 ** e) * (-1 if sign else 1)
                c2 = (decimal.Decimal(case[1]) / 10 ** e) * (-1 if sign else 1)

                d1 = MyDecimal(val=case[0], sign=sign, exp=e)
                d2 = MyDecimal(val=case[1], sign=sign, exp=e)
                d3 = MyDecimal(val=0)

                err_code = C_decimal.s21_add(d1, d2, pointer(d3))

                if err_code == 0:
                    if d3.full_value() == c1 + c2:
                        passed = True
                        succs += 1
                    else:
                        passed = False
                        fails = fails + 1
                else:
                    if d1.uint_value() + d2.uint_value() > 2 ** 96 - 1:
                        passed = True
                        succs += 1
                    else:
                        passed = False
                        fails = fails + 1
                if not passed:
                    print(c1 + c2, "=", c1, "+", c2, "!=", f"{d3.full_value()}")
                    print(case,case[0]+case[1])
                    print(e)
    total += succs + fails


def test_float():
    pass




if __name__ == '__main__':
    # d1 = MyDecimal(val=1)
    # decimal.setcontext(decimal.BasicContext)
    # decimal.setcontext(decimal.Context(prec=28,Emax=28))
    # print(decimal.getcontext())
    # # print(decimal.MAX_EMAX)
    # # print(decimal.MAX_PREC)
    # d = decimal.Decimal(2**96-2)
    # d2 = decimal.Decimal(2)
    # print(d)
    # print(d2)
    # print(" ",18446744073709551615+4294967295)
    # print(decimal.Decimal((18446744073709551615+4294967295))/10**20)
    # print(("|"+" "*31)*4)
    # print(" "*31,bits((18446744073709551615+4294967295)))
    edge_sum()
    print(f"Total:{total}\nPassed:{succs}\nFailed:{fails}")
# 20    1000101011000111001000110000010010001001111010000000000000000000
# 19    0000110111100000101101101011001110100111011001000000000000000000
# 18    0000000101100011010001010111100001011101100010100000000000000000
# 17    0000000000100011100001101111001001101111110000010000000000000000
# 16    0000000000000011100011010111111010100100110001101000000000000000
# 15    0000000000000000010110101111001100010000011110100100000000000000
# 14    0000000000000000000010010001100001001110011100101010000000000000
# 13    0000000000000000000000001110100011010100101001010001000000000000
# 12    0000000000000000000000000001011101001000011101101110100000000000
# 11    0000000000000000000000000000001001010100000010111110010000000000
# 10    0000000000000000000000000000000000111011100110101100101000000000
# 9     0000000000000000000000000000000000000101111101011110000100000000
# 8     0000000000000000000000000000000000000000100110001001011010000000
# 7     0000000000000000000000000000000000000000000011110100001001000000
# 6     0000000000000000000000000000000000000000000000011000011010100000
# 5     0000000000000000000000000000000000000000000000000010011100010000
# 4     0000000000000000000000000000000000000000000000000000001111101000
# 3     0000000000000000000000000000000000000000000000000000000001100100
# 2     0000000000000000000000000000000000000000000000000000000000001010
# 1     0000000000000000000000000000000000000000000000000000000000000001
#   20  1000101011000111001000110000010010001001111010000000000000000000
#   19	0000110111100000101101101011001110100111011001000000000000000000
#   18	0000000101100011010001010111100001011101100010100000000000000000
#   17	0000000000100011100001101111001001101111110000010000000000000000
#   16	0000000000000011100011010111111010100100110001101000000000000000
#   15	0000000000000000010110101111001100010000011110100100000000000000
#   14	0000000000000000000010010001100001001110011100101010000000000000
#   13	0000000000000000000000001110100011010100101001010001000000000000
#   12	0000000000000000000000000001011101001000011101101110100000000000
#   11	0000000000000000000000000000001001010100000010111110010000000000
#   10	0000000000000000000000000000000000111011100110101100101000000000
#   9	0000000000000000000000000000000000000101111101011110000100000000
#   8	0000000000000000000000000000000000000000100110001001011010000000
#   7	0000000000000000000000000000000000000000000011110100001001000000
#   6	0000000000000000000000000000000000000000000000011000011010100000
#   5	0000000000000000000000000000000000000000000000000010011100010000
#   4	0000000000000000000000000000000000000000000000000000001111101000
#   3	0000000000000000000000000000000000000000000000000000000001100100
#   2	0000000000000000000000000000000000000000000000000000000000001010
#   1	0000000000000000000000000000000000000000000000000000000000000001