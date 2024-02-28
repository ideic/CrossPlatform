import argparse
from pathlib import Path
import csv
import humanize
import time
import subprocess

def main():
    print("Hello World!")
    parser = argparse.ArgumentParser(description='Collect stattisticall info about assemblies')
    parser.add_argument('--folder', help='Provide a folder that will be parsed for collectiong stat info')
    args=parser.parse_args()
    
    folder: Path
    if args.folder is None:
        folder = Path.cwd()
    else:
        folder = Path(args.folder.replace('"', '')) 

    print(folder.absolute())
    exeFiles = list(folder.rglob('*.exe'))

    with  open("stat.csv", "w") as statFile:
        csvWiter =csv.writer(statFile)
        exeFiles.sort( key=lambda item: item.name, reverse=False)
        csvWiter.writerow(['Name', 'size', 'Exceution Time'])
        for item in exeFiles:
            execTime = '0ms'
            if (item.name.find('Test') and item.stat().st_size > 40):
                print(item.absolute())
                start = time.time()
                subprocess.Popen(item.absolute()).communicate()
                stop = time.time()
                execTime = str((stop - start).real)
                print(execTime)

            csvWiter.writerow([str(item.absolute()),  humanize.naturalsize(item.stat().st_size), execTime])

if __name__ == "__main__":
    main()
