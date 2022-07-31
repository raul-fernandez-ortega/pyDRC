import pyDRC
import sys
 
def main():
    if len(sys.argv) < 4:
        print("Usage:")
        print("python lsconv.py measurement.wav inverse.wav impulse.wav")
        sys.exit()
    measurement_file = sys.argv[1]
    inverse_file = sys.argv[2]
    impulse_file = sys.argv[3]
    pyDRC.LsConv(measurement_file, inverse_file, impulse_file)

if __name__ == "__main__":
    main()
   
