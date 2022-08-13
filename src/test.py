import os
from ctypes import *
from typing import Any

LIB = '/s21_decimallib.so'
print(os.getcwd() + LIB)
decimal = CDLL(os.getcwd() + LIB)

def bits(a):
    return f"{a:b}"


class Decimal(Structure):
    _fields_ = [
        ("bits", (c_uint*4))
    ]

    def __init__(self, *args: Any, **kw: Any):
        super().__init__(*args, **kw)
        if (kw.get('data')):
            self.bits = (c_uint * 4)(*kw['data'])
        if (kw.get('val')):
            a:int = kw.get('val')
            b = a.to_bytes(12,'little')
            byte_type = (c_ubyte * 12)
            int_type = (c_uint * 4)
            bys =byte_type(*b)
            ints_val =int_type.from_address(addressof(bys))
            self.bits[0] = ints_val[0]
            self.bits[1] = ints_val[1]
            self.bits[2] = ints_val[2]
            #TODO set exp and sign
        if kw.get('exp'):
            pass
        if kw.get('neg'):
            pass


    def value(self):
        sign = -1 if self.bits[3] >> 30 == 1 else 1
        exp = ((self.bits[3] << 1) >> 17) & 255  # первые 8 бит
        return ((self.bits[0] + self.bits[1] * 4294967296 + self.bits[2] * 4294967296 ** 2) * sign) #/ 10 ** exp





decimal.s21_add.restype = c_int
decimal.s21_add.argtypes = [Decimal, Decimal, POINTER(Decimal)]

decimal.s21_sub.restype = c_int
decimal.s21_sub.argtypes = [Decimal, Decimal, POINTER(Decimal)]
# d1 = Decimal(data=[130, 0, 0, 0])
# d2 = Decimal(value=5349)
# print(d2)
# d2 = Decimal(data=[130, 0, 0, 0])
d1 = Decimal(val=79228162514264337593543950335)
d2 = Decimal(val=79228162514264337593543950334)
d3 = Decimal(val=0)
decimal.s21_sub(d1,d3,pointer(d3))
print(d3.value())




