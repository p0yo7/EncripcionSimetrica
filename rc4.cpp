#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>

using namespace std;

// Se crea el vector de inicialización usando la key ingresada
vector<int> inicializar(string &key){
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

//make a descifradoRC4 function
vector<int> descifradoRC4(vector<int> &cifrado, vector<int> &S){
    vector<int> mensaje_descifrado(cifrado.size());
    for(int i = 0; i < cifrado.size(); i++){
        mensaje_descifrado[i] = cifrado[i] ^ S[i];
    }
    return mensaje_descifrado;
}

void write_file(vector<int> &cifrado){
    ofstream file("output.txt");
    file.open(filename);
    for(auto i : cifrado){
        file << static_cast<char>(i);
    }
    file.close();
}
// Brute force attack for RC4 encryption
std::vector<unsigned char> brute_force_attack(const std::vector<unsigned char>& encrypted_text) {
    std::vector<unsigned char> recovered_plaintext;
    for (int i = 0; i < 256; ++i) { // Iterate over all possible key values (0x00 to 0xFF)
        std::vector<unsigned char> candidate_key = {static_cast<unsigned char>(i)};
        std::vector<unsigned char> candidate_plaintext = rc4_encrypt(encrypted_text, candidate_key);
        // Check if the candidate plaintext looks valid (e.g., contains meaningful text)
        // If so, consider it as the recovered plaintext
        if (is_valid_plaintext(candidate_plaintext)) {
            recovered_plaintext = candidate_plaintext;
            break; // Stop the attack once a valid plaintext is found
        }
    }
    return recovered_plaintext;
}

void rc4(){
    string key  = "", mensaje = "", decryption_key = "";
    cout << "Ingrese la clave: ";
    cin >> key;
    cout << "Ingrese el mensaje: ";
    cin.ignore(); // ignora el newline anterior
    getline(cin, mensaje); // lee espacios dentro del mensaje

    // *********************************** CIFRADO ***********************************
    // Inicializar el estado S
    vector<int> S = inicializar(key);
    // Convierte el mensaje a un vector de enteros ascii
    vector<int> mensajeASCII = str2ASCII(mensaje);
    // Generar flujo dependiendo de la llave y el tamaño del mensaje
    vector<int> flujo = generarFlujo(S, mensaje.size());
    // Cifrado
    vector<int> cifrado = cifradoRC4(mensajeASCII, flujo);
    cout << "Mensaje cifrado: ";
    for(auto i : cifrado){
        cout << static_cast<char>(i); // Convertir a caracteres imprimibles
    }
    cout << endl;
    write_file(cifrado);
    // Descifrado de rc4
    vector<int> mensaje_descifrado = descifradoRC4(cifrado, flujo);
    cout << "Mensaje descifrado: ";
    for (auto i  : mensaje_descifrado){
        cout << static_cast<char>(i); // Convertir a caracteres imprimibles
    }
    cout << endl;
    vector<int> recovered_plaintext = bar_mitzvah_attack(cifrado);
    cout << "Recovered plaintext: ";
    for (auto i : recovered_plaintext) {
        cout << static_cast<char>(i);
    }
}

int main() {
    rc4();
    return 0;
}
