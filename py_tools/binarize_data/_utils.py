
def ensure_32bit_int(value: int) -> int:
    return value & 0xFFFFFFFF
