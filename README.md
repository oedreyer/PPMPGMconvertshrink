# PPMPGMconvertshrink
Software to convert PPM to PGM as well as compressing PGM files, run on the command line.

Usage: ./interface.exe copy-ppm input.ppm output.ppm //copy contents of one ppm file to another
       ./interface.exe copy-pgm input.pgm output.pgm //copy contents of one pgm file to another
       ./interface.exe grayscale input.ppm output.pgm //convert ppm file to pgm file, in essence grayscaling the pgm
       ./interface.exe shrink-pgm input.pgm output.pgm //compress pgm file
       ./interface.exe shrink-ppm input.ppm output.ppm //compress ppm file
