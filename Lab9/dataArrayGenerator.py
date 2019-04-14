# Script to read in a text file and output a formated header file of the data for importing and testing on the CyBot

# Returns true if the given string is a valid number, false otherwise
def is_number(str):
    try:
        float(str)
        return True
    except ValueError:
        return False

# Put filepaths here
dataFilePath = "cybot_data.txt"
outputFilePath = "object_data.h"

# Open data file and grab all the text
dataFile = open(dataFilePath, "r")
data = dataFile.readlines()
dataFile.close()

# Open the output file
outputFile = open(outputFilePath, "w")

# Set up the output file
header = """/**
 * @file object_data.h
 * @author Isaac Rex
 * @brief An array of scanned sensor data for repeatability in testing
 * @date 2019-04-14
 *
 */
 
#ifndef _OBJECT_DATA_H
#define _OBJECT_DATA_H
 """
outputFile.write(header)

# Setup the structure and start the array
outputFile.write("""
typedef struct distance {
  double ir_distance;
  double sonar_distance;
} distance_t;

distance_t data[] = {
""")

# Add all the data from the text file to the array
for line in data:
    numbers = line.split()
    if is_number(numbers[0]):
        outputFile.write("\t{")
        outputFile.write(".ir_distance = %s, .sonar_distance = %s" % (numbers[1], numbers[2]))
        outputFile.write("},\n")

outputFile.write("};\n\n")
outputFile.write("#endif")
outputFile.close
