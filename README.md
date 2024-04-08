# 1. Install git and cpp compiler if not installed
Made for academic and research purposes only
For Fedora
```
sudo  dnf install -y git gcc-c++
```
For Debian / Ubuntu
```
sudo dnf install -y git g++
```

# 2. Clone the repository
```
cd ~/Documents && git clone https://github.com/p0yo7/EncripcionSimetrica.git
```
# 3. Navigate to folder
```
cd EncripcionSimetrica/Algorithms
```
# 3A Read information in folders
```
cat ../Test/info.json
```
```
cat ../Test/Link.txt
```
# 3B Navigate back to Algorithms Folder
```
cd ../Algorithms
```
# 4. Compile and make executable for rc4_files.cpp 
```
g++ -std=c++17 -o main.exe rc4_files.cpp
```
# 5. Run main.exe
```
./main.exe
```
