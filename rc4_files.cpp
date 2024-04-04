//Encrypt files from directory
// Para compilar es g++ -std=c++17 -o main.exe rc4_files.cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

using namespace std;

// Se crea el vector de inicialización usando la key ingresada
vector<int> inicializar(const string &key){
    // Se crea un vector de enteros con longitud de 256
    vector<int> S(256);
    
    // Se itera del 0 al 255 llenando S de valores del 0...255
    for(int i = 0; i < 256; i++){
        S[i] = i;
    }
    // Variable auxiliar para hacer swap
    int j = 0;
    
    // Se vuelve a iterar de 0 al 255
    for(int i = 0; i < 256; i++){
        // Se asigna j a su mismo valor con el valor en la posición i de S y la posición i % tamaño de la key, a todo eso se le saca mod 256
        j = (j + S[i] + key[i % key.size()]) % 256;
        
        //Se intercambian los valores S[i] con S[j]
        swap(S[i], S[j]);
    }
    return S;
}
// Genera un flujo dependiendo del estado S y el tamaño del mensaje n
vector<int> generarFlujo(vector<int> &S, int n){
    // Se inicializa flujo de tamaño del mensaje y las variables 
    vector<int> flujo(n);
    int i = 0, j = 0, t = 0;
    for(int k = 0; k < n; k++){
        
        // Se calcula "i" a la suma de 1 a "i" y se le saca modulo para que no salga del rango 0-255
        i = (i + 1) % 256;

        // Se calcula j a el valor actual mas el valor de S en la posicion i calculada anteriormente y se saca mod 256
        j = (j + S[i]) % 256;
        
        // Se intercambian S[i] con S[j]
        swap(S[i], S[j]);

        //Se calcula t sumando S[i] con S[j] y sacando mod 256
        t = (S[i] + S[j]) % 256;

        // Al valor iterativo k se asigna a la posición t del estado S 
        flujo[k] = S[t];
    }
    return flujo;
}
//Convierte el mensaje en string a un vector en ASCII
vector<int> str2ASCII(string &mensaje){
    vector<int> mensajeASCII(mensaje.size());
    for(int i = 0; i < mensaje.size(); i++){
        // Se convierte implicitamente ya que ingresan chars a un vector de ints
        mensajeASCII[i] = mensaje[i];
    }
    return mensajeASCII;
}
vector<int> cifradoRC4(vector<int> &mensajeASCII, vector<int> &flujo){
    // Crea el mensaje del mensaje con tamano del mensaje ingresado
    vector<int> cifrado(mensajeASCII.size());
    // Se itera sobre cada valor del mensaje
    for(int i = 0; i < mensajeASCII.size(); i++){
        // Se realiza el XOR entre el valor del mensaje y el valor del flujo
        cifrado[i] = mensajeASCII[i] ^ flujo[i];
    }
    return cifrado;
}

// Function to encrypt the contents of a file using RC4
void encryptFile(const fs::path& filePath, const string& key) {
    // Read the contents of the file
    ifstream inputFile(filePath, ios::binary);
    if (!inputFile) {
        cerr << "Failed to open file: " << filePath << endl;
        return;
    }

    string fileContents((istreambuf_iterator<char>(inputFile)), istreambuf_iterator<char>());
    inputFile.close();

    // Convert the file contents to ASCII
    vector<int> mensajeASCII(fileContents.begin(), fileContents.end());

    // Initialize the RC4 cipher with the key
    vector<int> S = inicializar(key);

    // Generate the RC4 keystream
    vector<int> flujo = generarFlujo(S, mensajeASCII.size());

    // Encrypt the file contents using RC4
    vector<int> cifrado = cifradoRC4(mensajeASCII, flujo);

    // Write the encrypted contents back to the file
    ofstream outputFile(filePath, ios::binary);
    for (auto i : cifrado) {
        outputFile.put(static_cast<char>(i));
    }
    outputFile.close();

    cout << "File encrypted: " << filePath << endl;
}

int main() {
    string folderPath = "your_folder_path_here";
    string key;

    cout << "Enter encryption key: ";
    cin >> key;

    // Iterate over each file in the directory
    for (const auto& entry : fs::directory_iterator(folderPath)) {
        if (entry.is_regular_file()) {
            encryptFile(entry.path(), key);
        }
    }

    return 0;
}
