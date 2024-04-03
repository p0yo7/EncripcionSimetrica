#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

vector<int> inicializar(string &key){
    vector<int> S(256);
    for(int i = 0; i < 256; i++){
        S[i] = i;
    }
    int j = 0;
    for(int i = 0; i < 256; i++){
        j = (j + S[i] + key[i % key.size()]) % 256;
        swap(S[i], S[j]);
    }
    return S;
}

vector<int> generarFlujo(vector<int> &S, int n){
    vector<int> flujo(n);
    int i = 0, j = 0, t = 0;
    for(int k = 0; k < n; k++){
        i = (i + 1) % 256;
        j = (j + S[i]) % 256;
        swap(S[i], S[j]);
        t = (S[i] + S[j]) % 256;
        flujo[k] = S[t];
    }
    return flujo;
}

vector<int> str2ASCII(string &mensaje){
    vector<int> mensajeASCII(mensaje.size());
    for(int i = 0; i < mensaje.size(); i++){
        mensajeASCII[i] = mensaje[i];
    }
    return mensajeASCII;
}
vector<int> cifradoRC4(vector<int> &mensajeASCII, vector<int> &flujo){
    vector<int> cifrado(mensajeASCII.size());
    for(int i = 0; i < mensajeASCII.size(); i++){
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


void rc4(){
    string key  = "", mensaje = "";
    cout << "Ingrese la clave: ";
    cin >> key;
    cout << "Ingrese el mensaje: ";
    cin.ignore(); // Ignore previous newline character
    getline(cin, mensaje); // Read entire line including spaces
    
    // Inicializar el estado S
    vector<int> S = inicializar(key);
    // Convierte el mensaje a un vector de enteros
    vector<int> mensajeASCII = str2ASCII(mensaje);
    // Generar flujo
    vector<int> flujo = generarFlujo(S, mensaje.size());
    // Cifrado
    vector<int> cifrado = cifradoRC4(mensajeASCII, flujo);
    cout << "Mensaje cifrado: ";
    for(auto i : cifrado){
        cout << static_cast<char>(i); // Convertir a caracteres imprimibles
    }
    cout << endl;
    
    // Descifrado de rc4
    vector<int> mensaje_descifrado = descifradoRC4(cifrado, flujo);
    cout << "Mensaje descifrado: ";
    for (auto i  : mensaje_descifrado){
        cout << static_cast<char>(i); // Convertir a caracteres imprimibles
    }
    cout << endl;
}

int main() {
    rc4();
    return 0;
}
