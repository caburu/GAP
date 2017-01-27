/** 
 		CASAMENTO DE PADROES - PATTERN MATCHING

Problema
- Encontrar substring em uma string
- Podemos querer encontrar um casamento exato ou aproximado

Motivação
- Pesquisar palavras em um editor de texto
- Sites de busca (ex: Google)
- Bioinformática (cadeias de DNA)
- Etc.

Considerações
- Formalismos (ver bibliografia)
- Vamos focar no casamento exato

Funções das linguagens:
- C++: Classe string - find	
		Nao consegui ter certeza qual algoritmo, padrao nao especifica.
- C: string.h - char *strstr(const char *haystack, const char *needle)

- Python: Classe string - find
 		Alguns lugares citam que implementa o algoritmo BMH.
- Java: Classe String - contains 
		Não consegui ter certeza, mas parece ser forca bruta.

Algoritmos implementados abaixo:
- Algoritmo Força Bruta ou Ingênua
- Algoritmo Boyer-Moore (BM)
- Algoritmo Boyer-Moore-Horspool (BMH)

Outros algoritmos:
- Existem variações como a Boyer-Moore-Horspool-Sunday
- Uso de autômatos
- Knuth-Morris-Pratt (KMP)
- Shift-And 
- Etc.

Referências
N. Ziviani, Projeto de Algoritmos com implementações em Java e C++, Thomson, 2007.
T.H. Cormen, C.E. Leiserson, R.L. Rivest, C. Stein, Introduction to Algorithms , 2ª ed., MIT Press & McGraw-Hill, 2001.
*/

/**
 * Implementação de algoritmos de Casamento de Cadeias (Pattern Matching)
 * - Algoritmo Força Bruta ou Ingênuo
 * - Algoritmo Boyer-Moore (BM)
 * - Algoritmo Boyer-Moore-Horspool (BMH)
 * 
 * Autor: Júlio César Alves
 * 
 * Parte do GAP: Grupo de Estudos em Algorimtos e Programação
 * DCC/UFLA - 2016-2
 * Coordenacao: prof. Julio Alves e prof. Dilson Pereira
 */

#include <iostream>

using namespace std;

const int TAMANHO_ALFABETO = 256;
const bool EXIBIR_PASSO_A_PASSO = true;

// procedimentos a serem definidos mais abaixo
void imprimeTextoComPadraoColorido(string texto, string padrao, unsigned int posPadrao);
void imprimeDeslocado(string s, unsigned int posInicial);

/**
 * Procura a primeira ocorrencia do 'padrao' em 'texto'.
 * Caso o padrao exista retorna a posicao do padrao no texto.
 * Caso contrario, retorna -1
 */
int casamentoForcaBruta(string texto, string padrao) {
	// Percorre o texto do comeco ate a ultima posicao na qual eh possivel
	// comparar com o padrao
	for (unsigned int i = 0; i < texto.size() - padrao.size() + 1; i++) {
		if (EXIBIR_PASSO_A_PASSO) { // apenas para visualizacao
			imprimeTextoComPadraoColorido(texto, padrao, i);
			imprimeDeslocado(padrao, i);
		}		
		
		// 'j' sera usado para acessar os caracteres do padrao.
		// Enquando os caracteres do texto e padrao forem iguais passa
		// para a proxima posicao.
		unsigned int j = 0;
		while (j < padrao.size() and padrao[j] == texto[i+j]) {
			j++;
		}
		// se 'j' for igual ao tamanho do padrao eh porque todos os
		// caracteres foram iguais
		if (j == padrao.size()) {
			// retorna a posicao onde o padrao casou
			return i;
		}
		// caso contrario o padrao nao foi encontrado e a busca tem que
		// recomecar a partir da proxima posicao do texto (i+1)
		// *** deslocando-se o padrao uma casa para a direita ***
	}
	// se fez todas as tentativas e nao encontrou eh porque o padrao nao
	// existe no texto
	return -1;
}

/**
  Algoritmo Boyer-Moore (BM)
  
  Ideia geral:
  - Começam alinhados a esquerda o texto e o padrao
  - A verificacao se o padrao casa comeca da direita para a esquerda
  - Se encontrar um caractere que não bate na posicao i:
    . Desloca-se o padrao para a direita até encontrar um caractere que case com o da posicao i do texto
    . Repete-se o processo
  - Caso contrario, o padrao foi encontrado
  - Obs: essa implementacao usa a heuristica ocorrencia.
  */
int casamentoBoyerMoore(string texto, string padrao) {
	// Percorre o texto do comeco ate a ultima posicao na qual eh possivel
	// comparar com o padrao
	unsigned int i = 0;
	while (i < texto.size() - padrao.size() + 1) {
		
		if (EXIBIR_PASSO_A_PASSO) { // apenas para visualizacao
			imprimeTextoComPadraoColorido(texto, padrao, i);
			imprimeDeslocado(padrao, i);
		}
		
		// 'j' sera usado para acessar os caracteres do padrao.
		// Enquando os caracteres do texto e padrao forem iguais passa
		// para a proxima posicao *** da direita para a esquerda ***.
		int j = padrao.size()-1;
		while (j >= 0 and padrao[j] == texto[i+j]) {
			j--;
		}
		// se 'j' for negativo eh porque todos os caracteres foram iguais ***
		if (j < 0) {
			// retorna a posicao onde o padrao casou
			return i;
		}
		// caso contrario o padrao nao foi encontrado e a busca tem que
		// recomecar a partir da proxima posicao do texto (i+1)
		// *** deslocando-se o padrao para a direita ate casar o caracter da
		//     posicao que deu errado ***
		else {
			// 'j' eh a posicao do padrao que deu errado. Ela corresponde a
			// posicao 'i+j' do texto.
			// Precisamos entao deslocar o padrao para a direita ate achar um
			// caractere que bata texto[i+j]
			
			if (EXIBIR_PASSO_A_PASSO) { // apenas para visualizacao
				imprimeDeslocado("^", i+j);
			}
			
			// 'k' vai guardar a posicao do elemento do padrao que casa
			int k = j-1;
			while (k >= 0 and texto[i+j] != padrao[k]) {
				k--;
			}
			// o deslocamento sera a diferenca entre a posicao que deu errado 'j'
			// e a posicao que foi achada 'k'
			i += j-k;
		}
		
	}
	// se fez todas as tentativas e nao encontrou eh porque o padrao nao
	// existe no texto
	return -1;	
}

/**
 Algoritmo Boyer-Moore-Horspool (BMH)
  - Simplifica o algoritmo original ja guardando o tamanho dos deslocamentos de cada letra do padrão.
  - Portanto, antes de comecar eh construida uma tabela que guarda o numero de deslocamentos de cada caractere
  - Depois executa o mesmo algorimto mas ao deslocar para a direita ja desloca a quantidade guardada.
  - Construcao da tabela: 
    . Um registro para cada caractere possivel (ou seja, o caractere ou seu valor inteiro eh a chave/posição).
    . O valor do registro sera o deslocamento a ser feito, para calcula-lo:
    . Inicializa o deslocamento de todo mundo com m (tamanho do padrao).
    . Percorre m-1 posicoes do padrao (sendo i posicao da letra) 
    .   e para cada letra atualiza o deslocamento  para m - i - 1
 */
int casamentoBoyerMooreHorspool(string texto, string padrao) {
	
	// monta a tabela de pre-processamento como explicado acima
	int tabela[TAMANHO_ALFABETO];
	for (unsigned int i = 0; i < TAMANHO_ALFABETO; i++) {
		tabela[i] = padrao.size();
	}
	for (unsigned int i = 0; i < padrao.size()-1; i++) {				
		tabela[(int)padrao[i]] = padrao.size() - i - 1;		
	}
	
	if (EXIBIR_PASSO_A_PASSO) { // apenas para visualizacao
		for (unsigned int i = 0; i < padrao.size(); i++) { 
			cout << padrao[i] << "=" << tabela[(int)padrao[i]] << " "; 
		} cout << endl;
	}	
	
	// Percorre o texto do comeco ate a ultima posicao na qual eh possivel
	// comparar com o padrao
	unsigned int i = 0;
	while (i < texto.size() - padrao.size() + 1) {
		
		if (EXIBIR_PASSO_A_PASSO) { // apenas para visualizacao
			imprimeTextoComPadraoColorido(texto, padrao, i);
			imprimeDeslocado(padrao, i);
		}
		
		// 'j' sera usado para acessar os caracteres do padrao.
		// Enquando os caracteres do texto e padrao forem iguais passa
		// para a proxima posicao *** da direita para a esquerda ***.
		int j = padrao.size()-1;
		while (j >= 0 and padrao[j] == texto[i+j]) {
			j--;
		}
		// se 'j' for negativo eh porque todos os caracteres foram iguais ***
		if (j < 0) {
			// retorna a posicao onde o padrao casou
			return i;
		}
		// caso contrario o padrao nao foi encontrado e a busca tem que
		// recomecar a partir da proxima posicao do texto (i+1)
		// *** deslocando-se o padrao para a direita ate casar o caracter da
		//     posicao que deu errado ***
		else {
			// Aqui esta a diferenca para o algoritmo Boyer Morre, pois nao
			// eh necessario calcular o deslocamento do padrao. Basta usar 
			// a tabela pre-processada
			
			if (EXIBIR_PASSO_A_PASSO) { // apenas para visualizacao
				imprimeDeslocado("^", i+padrao.size()-1);
			}
			
			i += tabela[(int)texto[i+padrao.size()-1]];
		}
		
	}
	// se fez todas as tentativas e nao encontrou eh porque o padrao nao
	// existe no texto
	return -1;	
}


/**
 * Apenas para ter uma aparencia mais bacana, o procedimento abaixo imprime
 * o texto destacando o padrao encontrado.
 */
void imprimeTextoComPadraoColorido(string texto, string padrao, 
								   unsigned int posPadrao) {
	
	// as strings abaixo sao codigos especiais que, quando impressas na
	// tela na verdade mudam a cor de tudo que for impresso depois disso
	// Ex: cout << "abc" << codigoTextoVerde << "def" << codigoTextoNormal << "ghi";
	// imprime as letras "def" em verde e o resto na cor normal
	string codigoTextoVerde = "\033[1;32m";
	string codigoTextoNormal = "\033[0m";
	
	for (unsigned int i = 0; i < texto.size(); i++) {
		if (i == posPadrao) {
			cout << codigoTextoVerde;
		}
		else if (i == posPadrao + padrao.size()) {
			cout << codigoTextoNormal;
		}
		cout << texto[i];
	}
	cout << codigoTextoNormal << endl;
}

void imprimeDeslocado(string s, unsigned int posInicial) {
	for (unsigned int i = 0; i < posInicial; i++) {
		cout << " ";
	}
	cout << s << endl;
}

int main() {
	// 'texto' eh o texto onde a substring vai ser procurada
	// 'padrao' eh a substring a ser procurada
	string texto, padrao;
	int metodo;
	
	// obtem o texto da entrada padrao
	getline(cin, texto);
	
	do {
		cout << endl << "Digite o padrao (ENTER para sair): ";
		getline(cin, padrao);
		
		if (not padrao.empty()) {
			cout << "Metodo: (1) Forca Bruta (2) Boyer-Moore (3) Boyer-Moore-Horspool: ";
			cin >> metodo;
			cin.ignore(); // para evitar problema na proxima chamada do getline
			
			int pos;
			if (metodo == 1) {
				pos = casamentoForcaBruta(texto, padrao);	
			}
			else if (metodo == 2) {
				pos = casamentoBoyerMoore(texto, padrao);
			}
			else if (metodo == 3) {
				pos = casamentoBoyerMooreHorspool(texto, padrao);
			}
			else {
				cout << "metodo invalido!" << endl;
				pos = -1;
			}
		
			if (pos == -1) {
				cout << "\nPadrao " << padrao << " nao encontrado no texto." << endl;
			}
			else {
				cout << "\nPadrao encontrado no texto na posicao " << pos << endl;
				imprimeTextoComPadraoColorido(texto, padrao, pos);
			}
		}
	} while (not padrao.empty());
}
