﻿#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <conio.h>
#include <vector>
#include <sstream>
#include <cctype>
using namespace std;



/*int main() {
	
	regex patron("\\(\.[a-zA-Z\\W+\\w+]+\.\\)");
	string a = "(a->b)";

	if (regex_match(a, patron)) {
		cout<<a.substr(a.find("(")+1,a.find(")")-1);
	}
*/


vector<string> separador(string S, string T, vector<string> linea) {
	regex F("[a-zA-Z]+\\s->\\s[a-zA-Z]+");
	regex F2("\\(\.[a-zA-Z\\s->\\sa-zA-Z]+\.\\)\\s->\\s[a-zA-Z]+");

	string S1;
	string S2;
	string T1;
	string T2;

	if (regex_match(S, F)) {
		S1 = S.substr(0, T.find("->") - 1);
		S2 = S.substr(T.find("->") + 3);
		if (regex_match(T, F)) {
			T1 = T.substr(0, T.find("->") - 1);
			T2 = T.substr(T.find("->") + 3);
		}else {
			int pos = T.find("(");
			int pos2 = T.find(")");
			string T1 = T.substr(pos + 1, pos2 - 1);
			string aux = T.substr(pos2 + 1);
			string T2 = aux.substr(aux.find("->") + 3);
		}
	}else{
		int pos = S.find("(");
		int pos2 = S.find(")");
		 S1 = S.substr(pos + 1, pos2 - 1);
		string aux = S.substr(pos2 + 1);
	     S2 = aux.substr(aux.find("->") + 3);
		
		if (regex_match(T, F)) {
			T1 = T.substr(0, T.find("->") - 1);
			T2 = T.substr(T.find("->") + 3);
		}else{
			int pos = T.find("(");
			int pos2 = T.find(")");
			T1 = T.substr(pos + 1, pos2 - 1);
			string aux = T.substr(pos2 + 1);
			T2 = aux.substr(aux.find("->") + 3);
			
		}
	}

	string aux =  S1 + " = " + T1;
	string aux2 = S2 + " = " + T2;
	cout << "Se hace S1 = T1 : "<<aux <<" y S2 = T2 : "<<aux2<< endl;
	linea.push_back(aux);
	linea.push_back(aux2);
	return linea;
}

vector<string> replaces(vector<string> linea, string replace, string to_replace) {
	regex re(replace+"\\b");
	for (int i = 0; i < linea.size(); i++) {
		linea[i] = regex_replace(linea[i], re, to_replace);
		cout << linea[i]<<endl;
	}

	return linea;
}

string FV(string T) {
	regex X("[a-zA-Z]+([a-zA-Z] | [0-9])*");
	regex F("[a-zA-Z]+\\s->\\s[a-zA-Z]+");
	regex F2("\\(\.[a-zA-Z\\s->\\sa-zA-Z]+\.\\)\\s->\\s[a-zA-Z]+");

	string r;

	if (regex_match(T,X) && T != "int" && T != "bool") {
		r = r+T;
	}
	else if(T == "int") {
		r = r+"";
	}
	else if (T == "bool") {
		r = r+"";
	}
	else if (regex_match(T, F) || regex_match(T, F2)) {
			
		if (regex_match(T, F2)) {
			int pos = T.find("(");
			int pos2 = T.find(")");
			string T1 = T.substr(pos + 1, pos2 - 1);
			string aux = T.substr(pos2+1);
			string T2 = aux.substr(aux.find("->") + 3);
			return FV(T1) + FV(T2);
		}
		else {
			string T1 = T.substr(0, T.find("->") - 1);
			string T2 = T.substr(T.find("->") + 3);
			return FV(T1) + FV(T2);
		}
	}
	
	
	return r;
}

void unify(vector<string> linea) {
	
	string S;
	string T;
	regex X("[a-zA-Z]+([a-zA-Z] | [0-9])*");
	regex F("[a-zA-Z]+\\s->\\s[a-zA-Z]+");
	regex F2("\\(\.[a-zA-Z\\s->\\sa-zA-Z]+\.\\)\\s->\\s[a-zA-Z]+");

	if (linea.size() != 0) {
		int aux1 = linea[0].find("=");
		S = linea[0].substr(0, aux1 - 1);
		T = linea[0].substr(aux1 +2);
	}

	string r = FV(T);
	string r2 = FV(S);

	cout << r << endl;

	if (linea.size() == 0) {
		cout << "[]";
	}
	else {
		if (S==T) {
				linea.erase(linea.begin());
				unify(linea);
		}else if(regex_match(S, X) && r.rfind(S)>=100 && S != "int" && S!="bool") {
			cout << "Reemplazos = { " << S << "/" << T << " }\n";
			linea.erase(linea.begin());
			linea = replaces(linea, S, T);
			unify(linea);
		}
		else if (regex_match(T, X) && r2.rfind(T) && T != "int" && T != "bool") {
			cout << "Reemplazos = { " << S << "/" << T << " }\n";
			linea.erase(linea.begin());
			linea = replaces(linea, T, S);
			unify(linea);
		}
		else if ((regex_match(S,F) || regex_match(S,F2)) &&  (regex_match(T,F) || regex_match(T,F2))){
			linea = separador(S, T, linea);
			linea.erase(linea.begin());
			unify(linea);
		}else {
			cout << "Falla con la restriccion: ****  " << linea[0] << " **** NO SE PUEDE HACER VALIDO\n";
		}
	}
}



void leer() {

	vector<string> linea;
	string aux;
	ifstream archivo;
	
	
	// Son 5 casos de ejemplo, solo deben cambiar el nombre en el lugar "archivo.open("")" para acceder a cada uno
	// los ejemplos los pueden encontrar en 5 distintos TXT dentro de la carpeta del proyecto.
	archivo.open("ej1.txt");
	while (getline(archivo, aux)) {
		linea.push_back(aux);
	}
	unify(linea);

}

int main() {


	leer();
	return 0;
}