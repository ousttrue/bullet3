import types
import pathlib
HERE = pathlib.Path(__file__).absolute().parent


def gen(path: pathlib.Path, m: types.ModuleType):

    path.parent.mkdir(exist_ok=True, parents=True)
    with path.open('w') as w:
        for k in dir(m):
            v = getattr(m, k)
            match v:
                case types.BuiltinFunctionType():
                    w.write(f'def {k}(*args):\n')
                    indent = '     '
                    if v.__doc__:
                        w.write(f"{indent}'''\n")
                        for l in v.__doc__.splitlines():
                            w.write(f"{indent}{l}\n")
                        w.write(f"{indent}'''\n")
                    w.write(f"{indent}...\n")
                case int():
                    w.write(f'{k}: int\n')
                case _:
                    print(k, type(v))


if __name__ == '__main__':
    import pybullet
    gen(HERE.parent / 'typings/pybullet/__init__.py', pybullet)
