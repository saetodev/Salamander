import argparse

# parse command line

parser = argparse.ArgumentParser(description="IDK YET")

parser.add_argument("inputFile", help="Input File")
parser.add_argument("outputName", help="Output File")

args = parser.parse_args()

delim    = args.inputFile.rfind('.')
fileName = args.inputFile[:delim]

# read input file as binary file

fileBytes = None

with open(args.inputFile, "rb") as file:
    fileBytes = file.read()

# write to c header file
with open(f"{args.outputName}.h", "w") as file:
    fileNameUpper = fileName.upper()

    file.write(f"#ifndef {fileNameUpper}_EMBEDDED_H\n")
    file.write(f"#define {fileNameUpper}_EMBEDDED_H\n")
    file.write("\n")

    file.write(f"#define {fileNameUpper}_DATA_SIZE {len(fileBytes)}\n")
    file.write("\n")

    file.write(f"    inline const char {fileName}Data[] = {{\n")
    
    for byte in fileBytes:
        file.write(f"        {byte},\n")

    file.write("        0,")

    file.write("    };\n\n")

    file.write(f"#endif\n")
