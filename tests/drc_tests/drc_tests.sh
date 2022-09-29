echo 'Testing erb-44.1.drc configuration'
python ../../drc.py --BCInFile impulse.wav --PSPointsFile ../../target/44100/pa-44.1.txt --MCPointsFile ../../mic/ecm8000.txt ../../config/44100/erb-44.1.drc
python plot_spectrum.py erb_44100/
echo 'Testing extreme-44.1.drc configuration'
python ../../drc.py --BCInFile impulse.wav --PSPointsFile ../../target/44100/pa-44.1.txt --MCPointsFile ../../mic/ecm8000.txt ../../config/44100/extreme-44.1.drc
python plot_spectrum.py extreme_44100/
echo 'Testing insane-44.1.drc configuration'
python ../../drc.py --BCInFile impulse.wav --PSPointsFile ../../target/44100/pa-44.1.txt --MCPointsFile ../../mic/ecm8000.txt ../../config/44100/insane-44.1.drc
python plot_spectrum.py insane_44100/
echo 'Testing minimal-44.1.drc configuration'
python ../../drc.py --BCInFile impulse.wav --PSPointsFile ../../target/44100/pa-44.1.txt --MCPointsFile ../../mic/ecm8000.txt ../../config/44100/minimal-44.1.drc
python plot_spectrum.py minimal_44100/
echo 'Testing normal-44.1.drc configuration'
python ../../drc.py --BCInFile impulse.wav --PSPointsFile ../../target/44100/pa-44.1.txt --MCPointsFile ../../mic/ecm8000.txt ../../config/44100/normal-44.1.drc
python plot_spectrum.py normal_44100/
echo 'Testing optimized-44.1.drc configuration'
python ../../drc.py --BCInFile impulse.wav --PSPointsFile ../../target/44100/pa-44.1.txt --MCPointsFile ../../mic/ecm8000.txt ../../config/44100/optimized-44.1.drc
python plot_spectrum.py optimized_44100/
echo 'Testing soft-44.1.drc configuration'
python ../../drc.py --BCInFile impulse.wav --PSPointsFile ../../target/44100/pa-44.1.txt --MCPointsFile ../../mic/ecm8000.txt ../../config/44100/soft-44.1.drc
python plot_spectrum.py soft_44100/
echo 'Testing strong-44.1.drc configuration'
python ../../drc.py --BCInFile impulse.wav --PSPointsFile ../../target/44100/pa-44.1.txt --MCPointsFile ../../mic/ecm8000.txt ../../config/44100/strong-44.1.drc
python plot_spectrum.py strong_44100/
