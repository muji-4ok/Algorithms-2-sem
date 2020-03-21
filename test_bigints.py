from subprocess import run
from random import randint
import io


def test(a, b, r1, op):
    data = str(op) + ' ' + str(a) + ' ' + str(b)
    ret = run(['./cmake-build-release/RunBigInts'], input=data, encoding='ascii',
              capture_output=True)
    out = ret.stdout
    err = ret.stderr
    print(out)

    if err:
        print("err")

    assert (r1 == int(out))


for i in range(10000):
    print('---------------', i, '---------------')
    a = randint(-(10 ** 5), 10 ** 5)
    b = randint(-(10 ** 5), 10 ** 5)
    # a = randint(-(10 ** 5), 10 ** 5)
    # b = randint(-(10 ** 5), 10 ** 5)
    print('a: ', a)
    print('b: ', b)

    r1 = a + b
    print('a + b: ', r1)
    test(a, b, r1, '+')

    r2 = a - b
    print('a - b: ', r2)
    test(a, b, r2, '-')

    r3 = a * b
    print('a * b: ', r3)
    test(a, b, r3, '*')

    r4 = abs(a) // abs(b) * (1 if a >= 0 else -1)
    print('a / b: ', r4)
    test(a, b, r4, '/')

    r5 = abs(a) % abs(b)
    print('a % b: ', r5)
    test(a, b, r5, '%')
