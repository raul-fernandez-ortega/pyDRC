import pyDRC
import sys
import xml.etree.ElementTree as ET
 
def main():
    if len(sys.argv) < 2:
        print("Usage:")
        print("python sweepgen.py sweep.xml [-s sweep_filename] [-i inverse_filename]")
        sys.exit()
    tree = ET.parse(sys.argv[1])
    root = tree.getroot()
    if root.tag != 'generate_sweep':
        print("generate_sweep tag not found")
        sys.exit()
    for child in root:
        if child.tag=='params':
            sample_rate = int(child.find('sample_rate').text)
            amplitude = float(child.find('amplitude').text)
            Hzstart = float(child.find('Hzstart').text)
            Hzend = float(child.find('Hzend').text)
            length = float(child.find('length').text)
            silence= float(child.find('silence').text)
            leadin = float(child.find('leadin').text)
            leadout = float(child.find('leadout').text)
    sweep_filename = root.find('sweep_filename').text
    inverse_filename = root.find('inverse_filename').text
    for i in range(2,len(sys.argv)):
        if sys.argv[i] == "-s":
            sweep_filename = sys.argv[i+1]
        elif sys.argv[i] == "-i":
            inverse_filename = sys.argv[i+1]
                   

    print("Saving sweep signal at %s and inverse signal at %s"%(sweep_filename, inverse_filename))
    print("Sweep parameters:")
    print("\tSample rate:%i Hz."%sample_rate)
    print("\tAmplitude:%.2f."%amplitude)
    print("\tStarts at %.2f Hz."%Hzstart)
    print("\tEnds at %.2f Hz."%Hzstart)
    print("\tLength %.2f s."%length)
    print("\tSilence %.2f s at start and end."%silence)
    print("\tLead in time fraction %.3f"%leadin)
    print("\tLead out time fraction %.3f"%leadout)
    
    pyDRC.GlSweep(sample_rate, amplitude, Hzstart, Hzend, length, silence, leadin, leadout, sweep_filename, inverse_filename)

    print("Done.")

if __name__ == "__main__":
    main()
   
