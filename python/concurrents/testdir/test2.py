from contextlib import contextmanager

class A:
    a = 1

@contextmanager
def test1():
    A.a += 1
    try:
        print(A.a)
        yield
    finally:
        print(A.a)


def main():
    with test1() as t:
        print("hello!")

if __name__ == '__main__':
    for i in range(10):
        main()
