#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

struct Filmes{

    int identificador;
    string nome;
    string diretor;
    int ano;
    string sinopse;

    bool removido;
};

void menuPrincipal() {
    cout << "------------------------------------------------------------------------" << endl;
    cout << "|                              MENU PRINCIPAL                            |" << endl;
    cout << "|------------------------------------------------------------------------|" << endl;
    cout << "| 1 - Importar ou exportar                                               |" << endl;
    cout << "| 2 - Registrar novo filme                                               |" << endl;
    cout << "| 3 - Remover filme                                                      |" << endl;
    cout << "| 4 - Buscar filme                                                       |" << endl;
    cout << "| 5 - Mostrar todos os filmes na tela                                    |" << endl;
    cout << "| 6 - Fechar Programa                                                    |" << endl;
    cout << "------------------------------------------------------------------------" << endl;
}

void menuFilmes(Filmes f) {
    cout << "------------------------------------------------------------------------" << endl;
    cout << "|Filme: " << f.nome << endl;
    cout << "|Diretor: " << f.diretor << endl;
    cout << "|Ano do Oscar: " << f.ano << endl;
    cout << "|Sinopse: " << f.sinopse << endl;
    cout << "------------------------------------------------------------------------" << endl;

}
Filmes* filmes = nullptr;


//Cria um vetor e quando necessário aumenta o tamanho em 10
void aumentarTamanho (Filmes*& filmes, int& capacidade, int& tamanho, const int aumento){ 
    int novaCapacidade = capacidade + aumento;
    Filmes* novoVetor = new Filmes[novaCapacidade]; 

    for(int i=0; i<tamanho; i++){
        novoVetor[i] = filmes[i]; 
    }

    delete[] filmes;
    filmes = novoVetor;
    capacidade = novaCapacidade;
}


//Função ImportarCSV
Filmes* importarCSV(const string& nomeArquivo, int& tamanho, int& capacidade) { //passa informações do arquivo csv para um vetor dinamico filmes
    ifstream arquivo(nomeArquivo); 

    capacidade=40;
    Filmes* filmes = new Filmes[capacidade];
    tamanho = 0;

    string linha;
    getline(arquivo, linha);

    while(getline(arquivo, linha)){
        if(tamanho == capacidade){
            aumentarTamanho(filmes, capacidade, tamanho, 10);
        }

        int pos = 0;

        pos = linha.find(','); 
        filmes[tamanho].identificador = stoi(linha.substr(0,pos)); 
        linha.erase(0, pos + 1); 

        pos = linha.find(',');
        filmes[tamanho].nome = linha.substr(0, pos);
        linha.erase(0, pos + 1);

        pos = linha.find(',');
        filmes[tamanho].diretor = linha.substr(0, pos);
        linha.erase(0, pos + 1);

        pos = linha.find(',');
        filmes[tamanho].ano = stoi(linha.substr(0, pos));
        linha.erase(0, pos + 1);

        filmes[tamanho].sinopse = linha;

        filmes[tamanho].removido = false;
        tamanho++; 
    }
    cout << "Fimes carregados: " << tamanho << endl;
    return filmes;
}



void mostrarTodosFilmes(Filmes* filmes, int tamanho){
    for(int i=0; i<tamanho; i++){
        if(!filmes[i].removido){
            cout << "Identificador: " << filmes[i].identificador << endl;
            menuFilmes(filmes[i]);
        }
    }
}



void exportarCSV(Filmes* filmes, int tamanho, const string& nomeArquivo){
    ofstream arquivo(nomeArquivo); //abrindo o nosso arquivo

    arquivo << "identificador,nome,diretor,ano,sinopse" << endl; //ta escrevendo os elementos do nosso cabeçalho

    for(int i=0; i<tamanho; i++){
        if(!filmes[i].removido){
            arquivo << filmes[i].identificador << ","
                    << filmes[i].nome << ","
                    << filmes[i].diretor << ","
                    << filmes[i].ano << ","
                    << " \"" << filmes[i]. sinopse << "\"" << endl;
        }

        } 
        arquivo.close();
    }

Filmes* importarBinario(const string& nome_arquivo, int& tamanho, int& capacidade){
    ifstream ler(nome_arquivo, ios::binary);

    ler.read(reinterpret_cast<char*>(&tamanho), sizeof(int));
    capacidade = tamanho + 10;
    Filmes* filmes = new Filmes[capacidade];

    for (int i = 0; i < tamanho; i++){
        Filmes f;
        
        //Le identificador, ano e ve se o filme foi removido
        ler.read(reinterpret_cast<char*>(&f.identificador), sizeof(int));
        ler.read(reinterpret_cast<char*>(&f.ano), sizeof(int));
        ler.read(reinterpret_cast<char*>(&f.removido), sizeof(bool));
        
        //Le o nome do filme
        int tamanho_Nome;
        ler.read(reinterpret_cast<char*>(&tamanho_Nome), sizeof(int));
        f.nome.resize(tamanho_Nome);
        ler.read(&f.nome[0], tamanho_Nome);

        //Le o nome do diretor
        int tamanho_Diretor;
        ler.read(reinterpret_cast<char*>(&tamanho_Diretor), sizeof(int));
        f.diretor.resize(tamanho_Diretor);
        ler.read(&f.diretor[0], tamanho_Diretor);

        //Le a sinopse
        int tamanho_Sinopse;
        ler.read(reinterpret_cast<char*>(&tamanho_Sinopse), sizeof(int));
        f.sinopse.resize(tamanho_Sinopse);
        ler.read(&f.sinopse[0], tamanho_Sinopse);

        filmes[i] = f;
    
    }
    
    ler.close();
    cout << "Filmes importados do arquivo binario com sucesso: " << tamanho << endl;   
    return filmes;

}

void exportarBinario(Filmes* filmes, int tamanho, const string& nome_arquivo){
    ofstream escrever(nome_arquivo, ios::binary);

    int FilmesNaoRemovidos = 0;
    for (int i = 0; i < tamanho; ++i) {
        if (!filmes[i].removido) FilmesNaoRemovidos++;
    }

    escrever.write(reinterpret_cast<char*>(&FilmesNaoRemovidos), sizeof(int));
    
    for (int i = 0; i < tamanho; i++){
        
        // Escreve os tipos primitivos direto
        escrever.write(reinterpret_cast<char*>(&filmes[i].identificador), sizeof(int));
        escrever.write(reinterpret_cast<char*>(&filmes[i].ano), sizeof(int));
        escrever.write(reinterpret_cast<char*>(&filmes[i].removido), sizeof(bool));

        // Escreve as strings com tamanho
        int tamanho_Nome = filmes[i].nome.size();
        escrever.write(reinterpret_cast<char*>(&tamanho_Nome), sizeof(int));
        escrever.write(filmes[i].nome.c_str(), tamanho_Nome);

        int tamanho_Diretor = filmes[i].diretor.size();
        escrever.write(reinterpret_cast<char*>(&tamanho_Diretor), sizeof(int));
        escrever.write(filmes[i].diretor.c_str(), tamanho_Diretor);

        int tamanho_Sinopse = filmes[i].sinopse.size();
        escrever.write(reinterpret_cast<char*>(&tamanho_Sinopse), sizeof(int));
        escrever.write(filmes[i].sinopse.c_str(), tamanho_Sinopse);
    }

    escrever.close();
    cout << "Filmes exportados para binario com sucesso!" << endl;
}
    



    // busca um filme de acordo com o identificador
int buscaBinariaporID(Filmes* filmes, int& tamanho, int& identificador){
    int inicio = 0;
    int fim = tamanho - 1;

    while(inicio <= fim){
        int meio = (inicio + fim) / 2;
        if(filmes[meio].identificador == identificador){
            return meio; // achou na sorte
        } else if(filmes[meio].identificador < identificador){
            inicio = meio + 1; // procura na parte direita do numero
        } else {
            fim = meio - 1; // procura na parte esquerda do numero
        }
    }
    
    return -1; //se nao achar
}

    // busca o filme de acordo com o nome digitado
int buscaBinariaporNome(Filmes* filmes, int& tamanho, string& nomeFilme){
    int inicio = 0;
    int fim = tamanho - 1;

    while(inicio <= fim){
        int meio = (inicio + fim) / 2;
        if (filmes[meio].nome == nomeFilme and !filmes[meio].removido){
            return meio;
        } else if(filmes[meio].nome < nomeFilme){
            inicio = meio + 1;
        } else{
            fim = meio - 1;
        }
    }
    
    return -1;
}

//ETAPA 6- Ordenar dois campos diferentes

//Ordenando por Nome do Filme
int particionarNome(Filmes* filmes, int inicio, int fim){
    Filmes pivo = filmes[fim];
    int i = inicio-1;

    for(int j=inicio; j<fim; j++){
if(filmes[j].nome < pivo.nome){
    i++;
    swap(filmes[i],filmes[j]);
    }
}

swap (filmes[i+1], filmes[fim]);
return i+1;
}

void ordenarNome (Filmes* filmes, int inicio, int fim){
    if(inicio<fim){
        int pos= particionarNome(filmes, inicio, fim);
        ordenarNome(filmes, inicio, pos-1);
        ordenarNome(filmes, pos+1, fim);
    }
}


//Ordenando por IdCrescente
 int particionarId(Filmes* filmes, int inicio, int fim){   
    Filmes pivo = filmes[fim];
    int i = inicio - 1;

    for(int j=inicio; j<fim; j++){
        if(filmes[j].identificador < pivo.identificador){
            i++;
            swap (filmes[i], filmes[j]);
        }
    }

    swap(filmes[i+1], filmes[fim]);
    return i+1;
}

void ordenarId (Filmes* filmes, int inicio, int fim){
    if(inicio<fim){
        int pos = particionarId(filmes, inicio, fim);
        ordenarId(filmes, inicio, pos-1);
        ordenarId(filmes, pos+1, fim);
    }
}

//Funcao para registrar novo filme no case 1
void registrarFilme(Filmes*& filmes, int& tamanho, int& capacidade){ 
if(tamanho==capacidade){
aumentarTamanho(filmes,capacidade,tamanho,10);
}

Filmes novo; //variavel que armazena um novo filme vazio

cout << "Digite o novo ID do filme: ";
cin >> novo.identificador;
cin.ignore(); //Para nao ler uma string vazia na quebra de linha

//Verificar se o ID ja existe
bool idDuplicado=false;
for(int i=0; i<tamanho; i++){
    if(filmes[i].identificador==novo.identificador and !filmes[i].removido){
        idDuplicado=true;
        break;
    }
}

if(idDuplicado){
    cout << "ID já cadastrado!!!" << endl;
    return;
}

cout << "Digite o nome do filme: ";
getline(cin, novo.nome);

cout << "Digite o diretor do filme: ";
getline(cin, novo.diretor);

cout << "Digite o ano que ganhou o Oscar: ";
cin >> novo.ano;
cin.ignore();

cout << "Digite a sinopse: ";
getline(cin, novo.sinopse);

novo.removido=false;

filmes[tamanho]=novo; //Agora que "novo" está preenchido, adiciona ele no vetor posiçao tamanho
tamanho++;

cout << "Filme cadastrado" << endl;
}

//Função para remover os dados
void removerFilme(Filmes* filmes, int tamanho) {
    if (tamanho == 0) {
        cout << "Nao ha filmes cadastrados para remover!" << endl;
        return;
    }

    int opcao;
    cout << "\nComo deseja buscar o filme para remover:" << endl;
    cout << "1 - Por ID" << endl;
    cout << "2 - Por Nome" << endl;
    cout << "Escolha: ";
    cin >> opcao;

    if (opcao == 1) {
        int identificador;
        cout << "Digite o ID do filme que deseja remover: ";
        cin >> identificador;

        ordenarId(filmes, 0, tamanho - 1);
        int pos = buscaBinariaporID(filmes, tamanho, identificador);

        if (pos != -1 && !filmes[pos].removido) {
            filmes[pos].removido = true;
            cout << "Filme removido com sucesso!" << endl;
        } else {
            cout << "Filme nao encontrado ou ja esta removido." << endl;
        }

    } else if (opcao == 2) {
        string nome;
        cin.ignore();
        cout << "Digite o nome do filme que deseja remover: ";
        getline(cin, nome);

        ordenarNome(filmes, 0, tamanho - 1);
        int pos = buscaBinariaporNome(filmes, tamanho, nome);

        if (pos != -1 && !filmes[pos].removido) {
            filmes[pos].removido = true;
            cout << "Filme removido com sucesso!" << endl;
        } else {
            cout << "Filme nao encontrado ou ja esta removido." << endl;
        }

    } else {
        cout << "Opcao invalida. Retornando ao menu." << endl;
    }

    cout << "Deseja salvar as alteracoes no arquivo CSV e BINARIO?" << endl;
    cout << "1 - Sim" << endl;
    cout << "2 - Nao" << endl;
    cout << "Escolha:";
    int salvar;
    cin >> salvar;

    if (salvar == 1) {
        exportarCSV(filmes, tamanho, "filmes.csv");
        exportarBinario(filmes, tamanho, "filmes.bin");
        cout << "Alteracoes salvas com sucesso!" << endl;
    } else {
        cout << "Alteracoes não foram salvas, como requisitado." << endl;
    }
}


void avaliarTrabalho (){
    float nota;

    do{
        cout << "Antes de sair, qual nota de 0 a 16 voce daria para o trabalho: " << endl;
        cin >> nota;
        if(nota < 0 or nota > 16){
            cout << "Nota invalida, digite outra! " << endl;
            continue;
        }
         else if(nota != 16) {
            cout << "Nota muito baixa, tente outra! " << endl;
        }
    } while(nota != 16);
        cout << "Muito obrigado pela nota maxima!" << endl;
        cout << "------------------------------------------------------------------------\n" << endl;
}

void limparTela() {
    #ifdef _WIN32
        system("cls");  // Windows
    #else
        system("clear");  // Linux / macOS
    #endif
}

    int main () {
    int tamanho = 0;
    int capacidade = 10;
    Filmes* filmes = new Filmes[capacidade];

    capacidade = tamanho + 10;
    int escolha;
    bool sair_programa = false;
    bool voltar_menu = false;
    bool rodando = true;

    while (rodando) {
        limparTela(); // Limpa no início do loop principal
        menuPrincipal();   
        cout << "Favor importar as informacoes do arquivo CSV ou do arquivo BINARIO antes de comecar!" << endl;
        cout << "Digite sua opcao: ";
        cin >> escolha; 
        cout << "Filmes carregados: " << tamanho << endl;

        switch (escolha) {
            case 1: {
                limparTela(); // limpa no início do case 1
                int numero;
                cout << "Qual formato de arquivo voce deseja utilizar?" << endl;
                cout << "1 - Binario" << endl;
                cout << "2 - CSV" << endl;
                cout << "Digite a sua escolha: ";
                cin >> numero;

                if (numero == 1) {
                    limparTela();
                    int opcao;
                    cout << "O que voce deseja fazer?" << endl;
                    cout << "1 - Importar filmes do arquivo BINARIO" << endl;
                    cout << "2 - Exportar filmes para arquivo BINARIO" << endl;
                    cout << "Escolha: ";
                    cin >> opcao;

                    if (opcao == 1) {
                        delete[] filmes;
                        filmes = importarBinario("filmes.bin", tamanho, capacidade);
                        cout << "Importacao concluida com sucesso!" << endl;
                    } else if (opcao == 2) {
                        if (tamanho == 0) {
                            cout << "Nao ha filmes para exportar. Cadastre ou importe primeiro!" << endl;
                        } else {
                            exportarBinario(filmes, tamanho, "filmes.bin");
                            cout << "Exportacao concluida com sucesso!" << endl;
                        }
                    } else {
                        cout << "Opcao invalida. Retornando ao menu." << endl;
                    }

                } else if (numero == 2) {
                    limparTela();
                    int opcao;
                    cout << "O que voce deseja fazer?" << endl;
                    cout << "1 - Importar filmes do arquivo CSV" << endl;
                    cout << "2 - Exportar filmes para arquivo CSV" << endl;
                    cout << "Escolha: ";
                    cin >> opcao;

                    if (opcao == 1) {
                        delete[] filmes;
                        filmes = importarCSV("filmes.csv", tamanho, capacidade);
                        cout << "Importacao concluida com sucesso!" << endl;
                    } else if (opcao == 2) {
                        if (tamanho == 0) {
                            cout << "Nao ha filmes para exportar. Cadastre ou importe primeiro!" << endl;
                        } else {
                            exportarCSV(filmes, tamanho, "filmes.csv");
                            cout << "Exportacao concluida com sucesso!" << endl;
                        }
                    } else {
                        cout << "Opcao invalida. Retornando ao menu." << endl;
                    }

                } else {
                    cout << "Opcao invalida. Retornando ao menu." << endl;
                }

                while (true) {
                    int opcao;
                    cout << "Digite 1 para voltar ao menu ou 2 para sair: ";
                    cin >> opcao;

                    if (opcao == 1) {
                        voltar_menu = true;
                        break;
                    } else if (opcao == 2) {
                        sair_programa = true;
                        break;
                    } else {
                        cout << "Opcao invalida. Tente novamente." << endl;
                    }
                }

                break;
            }

            case 2: {
                limparTela();
                registrarFilme(filmes, tamanho, capacidade);

                int opcao;
                cout << "Deseja exportar os filmes para o CSV e BINARIO agora?" << endl;
                cout << "1 - Sim" << endl;
                cout << "2 - Nao" << endl;
                cout << "Escolha: ";
                cin >> opcao;

                if (opcao == 1) {
                    exportarBinario(filmes, tamanho, "filmes.bin");
                    exportarCSV(filmes, tamanho, "filmes.csv");
                    cout << "Dados exportados com sucesso!" << endl;
                }

                break;
            }

            case 3: {
                limparTela();

                if (tamanho == 0) {
                    cout << "Nao ha filmes cadastrados para remover!" << endl;
                    break;
                }

                removerFilme(filmes, tamanho);

                while (rodando) {
                    int opcao;
                    cout << "Qual opcao voce deseja? Digite 1 para voltar ao menu principal ou 2 para sair do programa." << endl;
                    cout << "Escolha: ";
                    cin >> opcao;

                    if (opcao == 1) {
                        voltar_menu = true;
                        break;
                    } else if (opcao == 2) {
                        sair_programa = true;
                        break;
                    } else {
                        cout << "Numero invalido! Tente novamente." << endl;
                    }
                }
                break;
            }

            case 4: {
                limparTela();

                int escolha;
                cout << "Como voce quer buscar os filmes:" << endl;
                cout << "1 - Buscar por ID" << endl;
                cout << "2 - Buscar por Nome" << endl;
                cout << "Escolha: ";
                cin >> escolha;

                if (escolha == 1) {
                    int identificador;
                    cout << "Digite o identificador do filme: ";
                    cin >> identificador;

                    ordenarId(filmes, 0, tamanho - 1);
                    int pos = buscaBinariaporID(filmes, tamanho, identificador);

                    if (pos != -1 and !filmes[pos].removido) {
                        cout << "Filme encontrado:" << endl;
                        menuFilmes(filmes[pos]);
                    } else {
                        cout << "Filme nao encontrado! Tente outro identificador." << endl;
                    }

                } else if (escolha == 2) {
                    string nome;
                    cin.ignore();
                    cout << "Digite o nome do filme: ";
                    getline(cin, nome);

                    ordenarNome(filmes, 0, tamanho - 1);
                    int pos = buscaBinariaporNome(filmes, tamanho, nome);

                    if (pos != -1) {
                        cout << "Filme encontrado:" << endl;
                        menuFilmes(filmes[pos]);
                    } else {
                        cout << "Filme nao encontrado! Tente outro nome." << endl;
                    }

                } else {
                    cout << "Opcao invalida. Retornando ao menu..." << endl;
                }

                while (rodando) {
                    int opcao;
                    cout << "Digite 1 para voltar ao menu ou 2 para sair: ";
                    cin >> opcao;

                    if (opcao == 1) {
                        voltar_menu = true;
                        break;
                    } else if (opcao == 2) {
                        sair_programa = true;
                        break;
                    } else {
                        cout << "Numero invalido! Tente novamente." << endl;
                    }
                }
                break;
            }

            case 5: {
                limparTela();

                if (tamanho == 0) {
                    cout << "Nao ha filmes cadastrados para exibir!" << endl;
                    break;
                }

                int escolha;
                cout << "Como voce quer exibir os filmes:" << endl;
                cout << "1 - Ordenar por ID" << endl;
                cout << "2 - Ordenar por Nome (Alfabetica)" << endl;
                cout << "3 - Ordem Normal (Sem Ordenar)" << endl;
                cout << "Escolha: ";
                cin >> escolha;

                Filmes* copia = new Filmes[tamanho];
                for (int i = 0; i < tamanho; i++) {
                    copia[i] = filmes[i];
                }

                if (escolha == 1) {
                    ordenarId(copia, 0, tamanho - 1);
                    cout << "Filmes ordenados por ID:" << endl;
                } else if (escolha == 2) {
                    ordenarNome(copia, 0, tamanho - 1);
                    cout << "Filmes ordenados por Nome:" << endl;
                } else if (escolha == 3) {
                    cout << "Filmes em ordem aleatoria:" << endl;
                } else {
                    cout << "Opcao invalida! Exibindo sem ordenar..." << endl;
                }

                mostrarTodosFilmes(copia, tamanho);
                delete[] copia;

                while (true) {
                    int opcao;
                    cout << "Digite 1 para voltar ao menu ou 2 para sair: ";
                    cin >> opcao;

                    if (opcao == 1) {
                        voltar_menu = true;
                        break;
                    } else if (opcao == 2) {
                        sair_programa = true;
                    break;
                    } else {
                        cout << "Opcao invalida! Tente novamente." << endl;
                    }
                }
                break;
        }
        case 6: {
            limparTela();
            avaliarTrabalho();
            cout << "Encerrando o programa. Obrigado!" << endl;
            rodando = false;
            break;
        }

        default:
            cout << "Opcao invalida! Tente novamente." << endl;
            break;
    }

    if (sair_programa) {
        rodando = false;
        limparTela();
        avaliarTrabalho();
    }

}

delete[] filmes;
return 0;
}