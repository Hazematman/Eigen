#!/usr/bin/env python
import sys
import xml.etree.ElementTree as ET
import struct

def parseData(textData, data, width, radius):
    i = 0
    textData.replace('\n', '')
    elems = textData.split(",")
    for e in elems:
        val = int(e)
        if val != 0:
            data[i] = (val, radius)
        i += 1

def parseWalls(textData, walls, lower, higher):
    textData.replace('\n', '')
    elems = textData.split(',')

    for e in elems:
        val = int(e)
        if val != 0:
            walls.append((val, lower, higher))

def main():
    if(len(sys.argv) < 3):
        print("Usage: {} <in-file> <out-file>".format(sys.argv[0]))
        return 0

    tree = ET.parse(sys.argv[1])
    root = tree.getroot()

    width = int(root.attrib["width"])
    height = int(root.attrib["height"])
    data = [(0,0)]*(width*height)
    walls = [];


    for child in root:
        if child.tag == 'layer':
            radius = 0.0
            ltype = ""
            lower = 0.0
            higher = 0.0
            for prop in child.find("properties"):
                if prop.attrib["name"] == "type":
                    ltype = prop.attrib["value"]
                elif prop.attrib["name"] == "radius":
                    radius = float(prop.attrib["value"])
                elif prop.attrib["name"] == "lower":
                    lower = float(prop.attrib["value"])
                elif prop.attrib["name"] == "higher":
                    higher = float(prop.attrib["value"])

            textData = child.find("data").text

            if ltype == "wall":
                parseWalls(textData, walls, lower, higher)
            elif ltype == "floor":
                parseData(textData, data, width, radius)


    headerfmt = "<4siii"
    datafmt = "<Hf"
    wallfmt = "<Hff"
    headerData = struct.pack(headerfmt, "MAPF", width, height, len(walls))
    out = open(sys.argv[2], "wb")
    out.write(headerData)

    for wall in walls:
        wallData = struct.pack(wallfmt, wall[0], wall[1], wall[2])
        out.write(wallData)

    for tile in data:
        tileData = struct.pack(datafmt, tile[0], tile[1])
        out.write(tileData)

    out.close()

if __name__ == "__main__":
    sys.exit(main())
