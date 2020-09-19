start
    int a = 192;
    int b = 0x80;
    int c = 0b110;

    printf("Hexidecimal representation of 192: %X", a);
    printf("Binary representation of 192: %b", a);
    printf("Binary representation of 0X80: %b", b);
    printf("Decimal representation of 0X80: %i", b);
    printf("Hexidecimal representation of 0b110: %X", c);
    printf("Decimal representation of 0b110: %i", c);

    #
    p "Some basic tests.." 
    p UTF8_CONT(128) != 0 
    p UTF8_2B(192) != 0
    p UTF8_3B(224) != 0
    p UTF8_4B(240) != 0
    #
    p "Testing UTF8_CONT.."
    p UTF8_CONT(128 + 1) != 0 
    p UTF8_CONT(128 | 1) != 0 
    p UTF8_CONT(128 | 63) != 0 
    p UTF8_CONT(128 | 63) > 0
    p UTF8_CONT(128 + 64) == 0
    p UTF8_CONT(128 | 64) == 0
    #
    p "Testing UTF8_2B.."
    p UTF8_2B(128 + 64) != 0 
    p UTF8_2B(128 | 64) != 0 
    p UTF8_2B(128 | 64 | 31) != 0
    p UTF8_2B(128 | 64 | 31) > 0
    p UTF8_2B(128 + 64 + 32) == 0
    p UTF8_2B(128 | 64 | 32) == 0
    #
    p "Testing UTF8_3B.."
    p UTF8_3B(128 + 64 + 32) != 0
    p UTF8_3B(128 | 64 | 32) != 0
    p UTF8_3B(128 | 64 | 32 | 15) != 0
    p UTF8_3B(128 | 64 | 32 | 15) > 0
    p UTF8_3B(128 + 64 + 32 + 16) == 0
    p UTF8_3B(128 | 64 | 32 | 16) == 0
    #
    p "Testing UTF8_4B.."
    p UTF8_4B(128 + 64 + 32 + 16) != 0
    p UTF8_4B(128 | 64 | 32 | 16) != 0
    p UTF8_4B(128 | 64 | 32 | 16 | 7) != 0 
    p UTF8_4B(128 | 64 | 32 | 16 | 7) > 0 
    p UTF8_4B(128 + 64 + 32 + 16 + 8) == 0 
    p UTF8_4B(128 | 64 | 32 | 16 | 8) == 0 
    #
    p "More student tests.."
    # TODO: Add more tests here.
q