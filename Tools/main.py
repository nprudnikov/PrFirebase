import os
import shutil
import zipfile
from pathlib import Path
from distutils.dir_util import copy_tree

import tqdm
from tqdm import tqdm


def prepare_firebase():
    cwd = os.getcwd()

    print("Prepare target folder for libraries")
    target_dir = os.path.join(cwd, "Libraries")
    if os.path.exists(target_dir):
        shutil.rmtree(target_dir)
    Path(target_dir).mkdir(parents=True, exist_ok=True)

    temp_dir = os.path.join(cwd, "Temp")
    if True:
        print("Cleanup Temp folder if exist and use to keep files until it's done")
        if os.path.exists(temp_dir):
            shutil.rmtree(temp_dir)
        Path(temp_dir).mkdir(parents=True, exist_ok=True)

        # @TODO Add file search by mask *.zip
        with zipfile.ZipFile('Firebase.zip') as zf:
            for member in tqdm(zf.infolist(), desc='Extracting '):
                try:
                    zf.extract(member, temp_dir)
                except zipfile.error as e:
                    pass

    # Iterate throw sublibs
    firebase_dir = os.path.join(temp_dir, "Firebase")
    modules = [f for f in os.scandir(firebase_dir) if f.is_dir()]
    for module in list(modules):
        # Iterate each required libs
        sublibs = [f for f in os.scandir(module.path) if (f.is_dir() and '.xcframework' in f.name)]
        for sublib in list(sublibs):
            lib_name = sublib.name
            lib_name = lib_name.removesuffix('.xcframework')
            lib_name_embedded = lib_name + ".embeddedframework"

            # Create temp folders for each lib
            lib_path = os.path.join(temp_dir, lib_name_embedded)
            Path(lib_path).mkdir(parents=True, exist_ok=True)

            # Copy arm64 binaries to target folder
            src_path = os.path.join(sublib.path, "ios-arm64_armv7")
            if not os.path.exists(src_path):
                src_path = os.path.join(sublib.path, "ios-arm64")
            copy_tree(src_path, lib_path)

            # zip target folder
            target_zip = os.path.join(target_dir, lib_name_embedded)
            shutil.make_archive(target_zip, 'zip', base_dir=lib_name_embedded, root_dir=temp_dir)
            print(target_zip)

    print("Cleanup Temp folder now")
    shutil.rmtree(temp_dir)

    print("Done!")


# Press the green button in the gutter to run the script.
if __name__ == '__main__':
    prepare_firebase()

