from subprocess import run
from random import randint


def sign(x):
    return 1 if x >= 0 else -1


def test(a, b, r1, op):
    data = str(op) + ' ' + str(a) + ' ' + str(b)
    ret = run(['./cmake-build-release/RunBigInts'], input=data, encoding='ascii',
              capture_output=True)
    out = ret.stdout
    err = ret.stderr
    print(out)

    if err:
        print('err', err)

    assert (r1 == int(out))


def test_io(a):
    data = str(a)
    ret = run(['./cmake-build-release/RunBigIntsIO'], input=data, encoding='ascii',
              capture_output=True)
    out = ret.stdout
    err = ret.stderr
    print(out)

    if err:
        print('err', err)

    assert (a == int(out))


# for i in range(100000):
#     a = randint(-(10 ** 5000), 10 ** 5000)
#     print('---------------', i, '---------------')
#     test_io(a)

for i in range(1000000):
    # for i in range(-10000, 10001):
    # for i in range(-100, 101):
    #     for j in range(-100, 101):
    print('---------------', i, '---------------')
    # print('---------------', i, j, '---------------')
    # a = i
    # b = j
    # a = i
    # b = 537
    a = randint(-(10 ** 5), 10 ** 5)
    b = randint(-(10 ** 500), 10 ** 500)
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
        r4 = abs(a) // abs(b) * sign(a) * sign(b)
        print('a / b: ', r4)
        test(a, b, r4, '/')

        r5 = a - r4 * b
        print('a % b: ', r5)
        test(a, b, r5, '%')
