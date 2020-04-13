from subprocess import run
from random import randint
from fractions import Fraction


def test(a, b, r1, op):
    data = str(op) + ' ' + str(a.numerator) + ' ' + str(a.denominator) + ' ' + str(
        b.numerator) + ' ' + str(b.denominator)
    ret = run(['./cmake-build-release/RunRationals'], input=data, encoding='ascii',
              capture_output=True)
    out = ret.stdout
    err = ret.stderr
    print(out)

    if err:
        print("err")

    try:
        nom, denom = map(int, out.split('/'))
    except ValueError:
        nom = int(out)
        denom = 1

    assert (r1 == Fraction(nom, denom))


# for i in range(1000000):
for i in range(-10, 11):
    for j in range(-10, 11):
        for k in range(-10, 11):
            for l in range(-10, 11):
                # print('---------------', i, '---------------')
                print('---------------', i, j, k, l, '---------------')
                a1 = i
                a2 = j
                b1 = k
                b2 = l
                # a1 = randint(-(10 ** 5), 10 ** 5)
                # a2 = randint(-(10 ** 5), 10 ** 5)
                # b1 = randint(-(10 ** 5), 10 ** 5)
                # b2 = randint(-(10 ** 5), 10 ** 5)
                try:
                    a = Fraction(a1, a2)
                    b = Fraction(b1, b2)
                except ZeroDivisionError:
                    continue
                # a = randint(-(10 ** 3), 10 ** 3)
                # b = randint(-(10 ** 3), 10 ** 3)
                print('a: ', a)
                print('b: ', b)
                # a = int(bin(a)[3:])
                # b = int(bin(b)[3:])
                # print('a: ', a)
                # print('b: ', b)

                r1 = a + b
                print('a + b: ', r1)
                test(a, b, r1, '+')

                r2 = a - b
                print('a - b: ', r2)
                test(a, b, r2, '-')

                r3 = a * b
                print('a * b: ', r3)
                test(a, b, r3, '*')

                if b:
                    r4 = a / b
                    print('a / b: ', r4)
                    test(a, b, r4, '/')

                r5 = a < b
                print('a < b: ', r5)
                test(a, b, r5, '<')

                r6 = a > b
                print('a > b: ', r6)
                test(a, b, r6, '>')

                r7 = a <= b
                print('a <= b: ', r7)
                test(a, b, r7, 'l')

                r8 = a >= b
                print('a >= b: ', r8)
                test(a, b, r8, 'g')

                r9 = a == b
                print('a == b: ', r9)
                test(a, b, r9, '=')

                r10 = a != b
                print('a != b: ', r10)
                test(a, b, r10, '!')

                r11 = -a
                print('-a: ', r11)
                test(a, a, r11, 'm')
