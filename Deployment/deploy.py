# encoding : utf-8

import glob
import os
import re
import inspect
import pathlib
import shutil
import subprocess


def filterListWithRegex(inVar: list, regex: str):
    return list(filter(re.compile(regex).match, inVar))


def ls(rootPath: str = ".", regex: str = None) -> list:
    if (not rootPath.endswith("/*")):
        rootPath += "/*"
    tmpList = glob.glob(rootPath, recursive=False)
    if (regex == None):
        return tmpList
    return filterListWithRegex(tmpList, regex)


def errorOccured(what: str = None, quitProg: bool = False):
    print("------- {} -------- An error occurred :\n\t{}".format(inspect.stack()
          [1].function, what))
    if (quitProg):
        exit(1)


def mkpathOverwrite(path: str) -> bool:
    """
    This function search for a dir, DELETE it if found
    then re-create it (empty)

    BE CAREFUL WHEN USING IT
    """
    if (path == "./"):
        errorOccured("You should not delete current dir", True)
    dirPath = pathlib.Path(path)
    if (dirPath.exists() and dirPath.is_dir()):
        shutil.rmtree(dirPath)
    os.makedirs(path)
    return True


def checkFile(path: str) -> bool:
    tmpPath = pathlib.Path(path)
    isFile = tmpPath.is_file()
    exis = tmpPath.exists()
    if ((not tmpPath.is_file()) or (not tmpPath.exists())):
        return False
    return True

# -------------------------------------------------------------


def getReleaseDir(rootPath: str = ".") -> str:
    tmpList = ls(rootPath, ".*Desktop_Qt_6.*MinGW.*_64.*-Release")
    for i, dir in enumerate(tmpList):
        tmpList[i] = dir + "/bin"
    if (len(tmpList) != 1):
        errorOccured(
            "Too many build-release folder :\n{}".format(str(tmpList)), False)
        return None
    if (len(tmpList) == 0):
        errorOccured("No build release folder found")
    return tmpList[0]


def getExePath(releaseDir: str, exeRegex: str = ".*BrokenTC2_2\\.exe") -> str:
    tmpList = ls(releaseDir, exeRegex)
    # tmpList = ls(releaseDir)
    if (len(tmpList) > 1):
        errorOccured("Too many exe found :\n{}".format(str(tmpList)), False)
        return None
    elif len(tmpList) == 0:
        errorOccured("Exe not found", False)
        return None
    return tmpList[0]


def main():
    CONFIG_outputDir = "AUTO"  # the dir will have the name of the found executable
    CONFIG_WINDEPLOYQT_PATH = "C:/Qt/6.8.0/mingw_64/bin/windeployqt.exe"
    CONFIG_DEPENDENCY_DIR = "../BrokenTC2_2/assets/bin"
    CONFIG_INNOSETUP_SCRIPT = "./buildSetup.iss"
    CONFIG_DEPLOY_OUTPUT_DIR = "./DEPLOY_OUTPUT"

    print("------------------------------------------------------")

    releaseDir = getReleaseDir("../BrokenTC2_2/build")
    if (not releaseDir):
        errorOccured("Cannot find release dir", True)
    print("Found build release dir at : {}".format(releaseDir))

    exePath = getExePath(releaseDir)
    if (not exePath):
        errorOccured("Cannot find exe path", True)
    print("Found exe at : {}\n".format(exePath))

    if (CONFIG_outputDir == "AUTO"):
        CONFIG_outputDir = "./{}/{}".format(
            CONFIG_DEPLOY_OUTPUT_DIR, pathlib.Path(exePath).stem+"_release")

    print("Creating deploy dir <{}> ...".format(CONFIG_outputDir))
    mkpathOverwrite(CONFIG_outputDir)  # erase and re-create deploy dir
    print("Done\n")

    print("Copying exe...")
    try:
        shutil.copy(exePath, CONFIG_outputDir)
    except:
        errorOccured("Cannot copy exe file", True)
    print("Done\n")

    print("Running windeployqt...")
    if (not checkFile(CONFIG_WINDEPLOYQT_PATH)):
        errorOccured("Cannot find windeployqt exe at {}".format(
            CONFIG_WINDEPLOYQT_PATH), True)
# C:\Qt\6.5.3\mingw_64\bin\windeployqt.exe --qmldir src build/Desktop_Qt_6_5_3_MinGW_64_bit-Release/bin/BrokenTC2_2.exe build/Desktop_Qt_6_5_3_MinGW_64_bit-Release/bin
    windeployCmd = "{} --qmldir ../BrokenTC2_2/src {} {}".format(
        CONFIG_WINDEPLOYQT_PATH, CONFIG_outputDir, exePath)
    print("\tusing command <{}>".format(windeployCmd))
    if (os.system(windeployCmd)):  # if windeployqt failed
        errorOccured("windeployqt failed to execute properly.", True)
    print("Done")

    print("Adding dependencies")

    dependencies = []
    dependencies.append(
        (f"{CONFIG_DEPENDENCY_DIR}/SDL2.dll", CONFIG_outputDir))

    for dependency in dependencies:
        dependencyPath = dependency[0]
        outputDir = dependency[1]
        print("\tAdding : <{}>".format(dependencyPath))
        if (not checkFile(dependencyPath)):
            errorOccured("Cannot find following dependency : <{}>".format(
                dependencyPath), False)
        try:
            shutil.copy(dependencyPath, outputDir)
        except:
            errorOccured("Cannot copy dependency <{}> to <{}>".format(
                dependencyPath, outputDir), False)
    print("Done\n")

    print("Deleting useless files")
    uselessFileList = []
    uselessFileList.append(f"{CONFIG_outputDir}/opengl32sw.dll")
    uselessFileList.append(f"{CONFIG_outputDir}/D3Dcompiler_47.dll")
    uselessFileList.append(f"{CONFIG_outputDir}/avcodec-61.dll")
    uselessFileList.append(f"{CONFIG_outputDir}/avformat-61.dll")

    for uselessFilePath in uselessFileList:
        if (checkFile(uselessFilePath)):
            print("\tDeleting : <{}>".format(uselessFilePath))
            try:
                os.remove(uselessFilePath)
            except:
                # non fatal error, keep going
                errorOccured("Cannot delete <{}>".format(
                    uselessFilePath), False)
    print("Done\n")

    print("##########################################")
    print("\nCreating setup...")

    os.system(" {}".format(CONFIG_INNOSETUP_SCRIPT))

    innoSetup = f"C:\\Program Files (x86)\\Inno Setup 6\\iscc.exe"
    args = [
        innoSetup,
        CONFIG_INNOSETUP_SCRIPT
    ]
    rval = subprocess.run(args)
    if rval.returncode != 0:
        errorOccured("Failed to run the inno setup compiler", False)
    print("##########################################\n")

    print("------------------------------------------------------")


if (__name__ == "__main__"):
    main()
