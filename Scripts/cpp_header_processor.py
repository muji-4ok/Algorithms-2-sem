import sys
import argparse
import os
from pathlib import Path
import re


# Checks in PATH as well
def file_exists_anywhere(file):
    if os.path.exists(file):
        return True

    for path in os.environ['PATH'].split(os.pathsep):
        if (Path(path) / file).exists():
            return True

    return False


# Checks in PATH as well
def open_file_anywhere(file, *args, **kwargs):
    if os.path.exists(file):
        return open(file, *args, **kwargs)

    for path in os.environ['PATH'].split(os.pathsep):
        extended_file = Path(path) / file

        if extended_file.exists():
            return open(extended_file, *args, **kwargs)

    raise FileNotFoundError(file)


def add_to_path(path):
    paths = os.environ['PATH'].split(os.pathsep)
    paths.append(path)
    os.environ['PATH'] = os.pathsep.join(paths)


def remove_from_path(path):
    paths = os.environ['PATH'].split(os.pathsep)

    # Remove last equal element, as to not ruin the search order somehow
    # (list.remove can only remove the first)
    for i, val in reversed(list(enumerate(paths))):
        if val == path:
            paths.pop(i)
            break

    os.environ['PATH'] = os.pathsep.join(paths)


def is_excluded(path, excluded):
    for exclude in excluded:
        if exclude.endswith(path):
            return True

    return False


def preprocess_file(file, skip_start_msg=False, exclude_headers=None):
    if exclude_headers is None:
        exclude_headers = []

    file = Path(file)
    output = ''

    with open_file_anywhere(file) as f:
        skipped = False

        for line in f.readlines():
            if line.startswith('//'):
                if skip_start_msg and not skipped:
                    continue
            else:
                skipped = True

            if line.startswith('#include'):
                path = re.fullmatch(r'#include\s*[<"](.*)[>"]\n', line).groups()[0]
                impl_path = Path(path).with_suffix('.cpp')

                # Check if we are a subpath of exluded_headers
                if is_excluded(path, exclude_headers):
                    continue

                exclude_headers.append(path)
                exclude_headers.append(str(impl_path))

                if file_exists_anywhere(path):
                    path_dir = str(Path(path).parent.absolute())
                    add_to_path(path_dir)

                    output += preprocess_file(path, True, exclude_headers)
                    if file_exists_anywhere(impl_path):
                        output += preprocess_file(impl_path, True, exclude_headers)

                    remove_from_path(path_dir)
                else:
                    output += line
            else:
                output += line

    return output


if __name__ == '__main__':
    parser = argparse.ArgumentParser(
        description='Utility for substituting c++ from includes into one file.')
    parser.add_argument('input', help='Input file.')
    parser.add_argument('output', help='Output file.')
    parser.add_argument('-i', '--include', type=str, default='',
                        help='Additional include directories, separated by ' + os.pathsep + '.')
    args = parser.parse_args()
    in_filename = Path(args.input)
    out_filename = Path(args.output)
    includes = args.include
    os.environ['PATH'] += os.pathsep + includes
    out = preprocess_file(in_filename)

    with open(out_filename, 'w') as f:
        f.write(out)
