import os
import glob
import re
import sys


def process_files(folder):
    file_list = glob.glob(os.path.join(folder, '*.qpb.cpp'))
    file_list2 = glob.glob(os.path.join(folder, '*.qpb.h'))
    file_list = file_list + file_list2

    for file_path in file_list:
        print(f"Fixing: {file_path}")
        with open(file_path, 'r') as file:
            content = file.read()

        # Perform regex substitution
        prev_content = content
        # content = pattern.sub(r'$1$3', content)
        pattern = r'(#include.*"google.*)(\.qpb)(\.h[">])$'
        matches = re.finditer(pattern, content, flags=re.MULTILINE)
        for match in matches:
            print("Match:", match.group(0))

        content = re.sub(pattern, r"\1\3", content, 0, re.MULTILINE)
        if prev_content != content:
            print("Fixed file")
        else:
            print("Nothing to fix")

        # Write the modified content back to the file
        with open(file_path, 'w') as file:
            file.write(content)


def main():
    if len(sys.argv) < 2:
        print("Usage: python script.py folder1 folder2 ...")
        sys.exit(1)

    folders_to_check = sys.argv[1:]

    for folder in folders_to_check:
        if not os.path.exists(folder):
            print(f"Error: Folder '{folder}' does not exist.")
            continue

        process_files(folder)
        print(f"Processed files in '{folder}'.")


if __name__ == "__main__":
    print("============== Fixing protobuf include")
    main()
    print("============== END Fixing protobuf include")
