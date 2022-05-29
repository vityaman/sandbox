echo "Package intallation..."
python3 setup.py install --user
rm -r build

echo "Runing unit tests..."
python3 test.py

echo "Running profiling..."
rm profiling-results.csv 2>/dev/null
python3 profile.py
echo "Profiling results ready! Open 'profiling-results.csv'"
