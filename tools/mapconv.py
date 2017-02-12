#!/usr/bin/env python
import sys
import xml.etree.ElementTree as ET
import struct

CHUNK_SIZE = 16
WALL_X = 3
WALL_Y = 4

def parseData(textData, data, width, radius):
    i = 0
    textData.replace('\n', '')
    elems = textData.split(",")
    for e in elems:
        val = int(e)
        if val != 0:
            data[i] = (val, radius)
        i += 1

def parseWalls(textData, walls, lower, higher, width):
    textData.replace('\n', '')
    elems = textData.split(',')
    ind = -1

    for e in elems:
        ind += 1
        val = int(e)
        if val != 0:
            y = ind // width
            x = ind % width
            walls.append((val, lower, higher, x, y))

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
                parseWalls(textData, walls, lower, higher, width)
            elif ltype == "floor":
                parseData(textData, data, width, radius)


    numXChunk = width // CHUNK_SIZE
    numYChunk = height // CHUNK_SIZE

    headerfmt = "<4siiii"
    datafmt = "<If"
    headerData = struct.pack(headerfmt, "MAPF", width, height, numXChunk, numYChunk)
    out = open(sys.argv[2], "wb")
    out.write(headerData)

    for y in range(numYChunk):
        for x in range(numXChunk):
            chunkData = [None]*(CHUNK_SIZE*CHUNK_SIZE)
            for yy in range(CHUNK_SIZE):
                for xx in range(CHUNK_SIZE):
                    chunkData[xx + yy*CHUNK_SIZE] = data[(x*CHUNK_SIZE+xx) + (y*CHUNK_SIZE+yy)*width]


            chunkWalls = []
            for wall in walls:
                if wall[WALL_X] >= x*CHUNK_SIZE and wall[WALL_X] < (x+1)*CHUNK_SIZE and wall[WALL_Y] >= y*CHUNK_SIZE and wall[WALL_Y] < (y+1)*CHUNK_SIZE:
                    chunkWalls.append(wall)

            
            wallLen = struct.pack("<i", len(chunkWalls))
            out.write(wallLen)

            for wall in chunkWalls:
                wallData = struct.pack("<Iffii", wall[0], wall[1], wall[2], wall[3], wall[4])
                out.write(wallData)

            for tile in chunkData:
                tileData = struct.pack(datafmt, tile[0], tile[1])
                out.write(tileData)

    out.close()

if __name__ == "__main__":
    sys.exit(main())
