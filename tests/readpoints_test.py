import pyDRC
from numpy import *


Freq=zeros(0)
Mag=zeros(0)
Phase=zeros(0)

print pyDRC.STL_ReadPoints("../target/subultra.txt", 'D', Freq, Mag, Phase, 6, 44100)