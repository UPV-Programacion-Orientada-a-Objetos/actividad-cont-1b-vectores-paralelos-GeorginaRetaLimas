#include <iostream>
#include <string>

#include <fstream>
#include <sstream>

void menu();
int validarEntradaEnteros(std::string texto);
int buscarIndiceCodigo(int cod_entrada, int* codigo, int tam);
int buscarIndiceUbicacion(std::string  ubi, std::string * ubicacion, int tam);
int validarEntradaFloat(std::string texto);
void indexCero(int tam);

int main() {
    // Tamaño de los vectores predefinido en variable
    int tam = 0;

    // Variables para validaciones del programa
    int accion, cod_entrada, index;
    std::string ubi_entrada;
    bool valor_valido;
    float precio_barato;

    // Vectores paralelos
    int codigo[100] = {};
    std::string nombre[100] = {};
    int stock[100] = {};
    float precio[100] = {};
    std::string ubicacion[100] = {};

    // Abrir un archivo para leer
    std::ifstream archivoLeer;
    archivoLeer.open("archivo.txt");
    std::string linea;

    // Variables para pasar lo del txt
    int cod, stk; std::string nom, ubi; float pre;
    int contar;

    if (archivoLeer.is_open()){
        bool primeraLinea = true;
        bool llegarlimite = false;

        while(std::getline(archivoLeer, linea)){ // Leer el archivo linea por linea
            if(primeraLinea){
                primeraLinea = false;
                continue; // Asi pasamos del encabezado
            }

            if(tam == 100){
                std::cout<<"Se llego al limite de registros"<<std::endl;
                llegarlimite = true;
                break;
            }

            //std::cout<<"Tamaño: "<<tam<<std::endl;

            std::stringstream texto(linea); // crea stringstream a partir de la linea
            std::string campo;

            contar = 0;

            // Leer el stringstream (texto) hasta que encuentre ',' la subcadena se guarda en campo
            while(std::getline(texto, campo, ',')){
                switch(contar){
                    case 0: // codigo
                        cod = std::stoi(campo); // string -> int
                    break;
                    case 1: // ubicacion
                        ubi = campo;
                    break;
                    case 2: // nombre
                        nom = campo;
                    break; //cantidad
                    case 3:
                        stk = std::stoi(campo);
                    break;
                    case 4: // precio
                        pre = std::stof(campo);
                    break;
                    default:
                    break;    
                }

                contar++;
            }

            // Pasar los datos a los vectores
            codigo[tam] = cod;
            nombre[tam] = nom;
            stock[tam] = stk;

            if(stk > 50){
                std::cout<<"Advertencia: Sobre stock de <"<<nom<<"> con ->"<<stk<<std::endl;
            }

            precio[tam] = pre;
            ubicacion[tam] = ubi;

            tam++;
        }
    } else {
        std::cout<<"Error: No se pudo abrir el archivo"<<std::endl;
    }

    archivoLeer.close();

    std::cout<<"\tBienvenido a Ferretería el Martillo"<<std::endl;

    do{menu();
        std::cin>>accion;

        if(std::cin.fail()){
            std::cout<<"Error: Valor invalido, no entero. Favor de ingresar numero entero"<<std::endl;
            std::cin.clear();
            std::cin.ignore(1024, '\n');
            continue;
        }

        switch(accion){
            case 1: // consulta
                indexCero(tam);

                cod_entrada = validarEntradaEnteros("Ingrese el código del producto");
                index = buscarIndiceCodigo(cod_entrada, codigo, tam);

                // Si retorno tam es porque no encontro el codigo
                if(index == tam){
                   std::cout<<"\nError: No hay un producto con el codigo "<<cod_entrada<<std::endl; 
                } else { // Lo encontro
                    std::cout<<"\nInformación del producto - - - -"<<std::endl;
                    std::cout<<"Código: "<<codigo[index]<<std::endl;
                    std::cout<<"Nombre: "<<nombre[index]<<std::endl;
                    std::cout<<"Cantidad en stock: "<<stock[index]<<std::endl;
                    std::cout<<"Precio Unitario: $"<<precio[index]<<std::endl;
                    std::cout<<"Ubicación: "<<ubicacion[index]<<std::endl;
                }
            break;
            case 2: // Actualizar inventario
                std::cin.clear();
                std::cin.ignore(1024, '\n');

                std::cout<<"\nIngrese ubicacion inventario: ";
                std::getline(std::cin, ubi);

                index = buscarIndiceUbicacion(ubi, ubicacion, tam);

                if(index == tam){
                   std::cout<<"\nError: No hay un producto con el codigo "<<cod_entrada<<std::endl; 
                } else {
                    std::cout<<"\nInformación del producto - - - -"<<std::endl;
                    std::cout<<"Nombre: "<<nombre[index]<<std::endl;
                    std::cout<<"Cantidad actual en stock: "<<stock[index]<<std::endl;

                    // Ciclo para validar que no se ingrese un stock negativo
                    do{
                        int cambio = validarEntradaEnteros("Ingrese la cantidad de cambio");
                        int nuevo_stock = stock[index] + cambio;

                        if(nuevo_stock < 0){
                            std::cout<<"Error: No se puede tener un stock negativo"<<std::endl;
                            valor_valido = false;
                        } else {
                            // En caso que no sea menor a 0 se actualiza
                            stock[index] = nuevo_stock;
                            valor_valido = true;
                        }

                    } while(!valor_valido);

                    std::cout<<"Stock actualizado correctamente..."<<std::endl;
                }
            break;
            case 3: // Registrar nuevo producto
                if(tam == 100){
                    std::cout<<"Ya hay 100 elementos en el stock :("<<std::endl;
                    break;
                }

                cod = codigo[tam-1] + 1;

                std::cout<<"Nuevo producto ---"<<std::endl;
                std::cout<<"Codigo: "<<cod<<std::endl;
                
                std::cin.clear();
                std::cin.ignore(1024, '\n');
                std::cout<<"\nIngrese nombre: ";
                std::getline(std::cin, nom);

                stk = validarEntradaEnteros("Ingrese stock: ");
                pre = validarEntradaFloat("Ingrese el precio: ");

                std::cin.clear();
                std::cin.ignore(1024, '\n');
                std::cout<<"\nIngrese ubicacion: ";
                std::getline(std::cin, ubi);

                codigo[tam] = cod;
                nombre[tam] = nom;
                stock[tam] = cod;
                precio[tam] = pre;
                ubicacion[tam] = ubi;

                tam++;
                std::cout<<"Ingreso exitoso"<<std::endl;

            break;
            case 4: // Generación de reportes
                indexCero(tam);

                std::cout<<"\n-- Reporte de productos con stock bajo --"<<std::endl;
                std::cout<<"+-------+-----------------------+-------+-------+"<<std::endl;
                for(int i = 0; i < tam; i++){
                    //codigo nombre stock precio
                    if(stock[i] < 10){
                        std::cout<<"| "<<codigo[i]<<"\t| "<< nombre[i] <<"\t| "<<stock[i]<<"\t| "<< precio[i] <<"\t|" << ubicacion[i] <<"\t|"<<std::endl;
                    }
                }
                std::cout<<"+-------+-----------------------+-------+-------+"<<std::endl;
            break;
            case 5: // Producto mas barato
                indexCero(tam);    

                index = 0;
                precio_barato = precio[0];

                for(int i = 1; i < tam; i++){

                    if(precio[i] < precio_barato){
                        precio_barato = precio[i];
                        index = i;
                    }
                }

                std::cout <<"El producto más barato es: " << nombre[index] << " con un precio de $" << precio[index] << std::endl;
            break;
            case 6: 
            {
                // Crear archivo para escribir
                std::ofstream archivoEscribir("archivo.txt");

                if(archivoEscribir.is_open()){
                    archivoEscribir << "Código,Ubicación,Nombre,Cantidad,Precio"<<std::endl;

                    for(int i = 0; i < tam ; i++){
                        archivoEscribir << codigo[i] << ",";
                        archivoEscribir << ubicacion[i] << ",";
                        archivoEscribir << nombre[i] << ",";
                        archivoEscribir << stock[i] << ",";
                        archivoEscribir << precio[i] << ","<<std::endl;
                    }
                }
                archivoEscribir.close();
                std::cout<<"Guardando datos...."<<std::endl;
                std::cout<<"¡Datos guardados exitosamente!"<<std::endl;
            }
            break;
            default:
                std::cout<<"Error: La opción no esta en el menú"<<std::endl;
            break;
        }

        // Limpialos el cin cada que iteramos para evitar errores

    } while(accion != 6);



    
    return 0;
}

void menu(){
    std::cout<<"\n- - - M e n ú - - -"<<std::endl<<std::endl;
    std::cout<<"1. Consultar un producto"<<std::endl;
    std::cout<<"2. Actualizar inventario por ubicacion"<<std::endl;
    std::cout<<"3. Registrar producto"<<std::endl;
    std::cout<<"4. Generar reporte de stock bajo"<<std::endl;
    std::cout<<"5. Encontrar el producto más barato"<<std::endl;
    std::cout<<"6. Salir"<<std::endl;

    std::cout<<"\nOpción seleccionada: ";
}

int validarEntradaEnteros(std::string texto){
    int cod_entrada;
    bool entrada_valida;

    // Ciclo de Validación del codigo ingresado
    do{
        std::cout<<"\n"<<texto<<": ";
        std::cin>>cod_entrada;

        // Validación de fallo de lectura
        if(std::cin.fail()){
            std::cout<<"Error: Valor invalido, no entero. Favor de ingresar numero entero"<<std::endl;
            std::cin.clear();
            std::cin.ignore(1024, '\n');

            entrada_valida = false;
        } else { // Lectura correcta
            entrada_valida = true;
        }
    } while(!entrada_valida);

    return cod_entrada;
}

int validarEntradaFloat(std::string texto){
    float cod_entrada;
    bool entrada_valida;

    // Ciclo de Validación del codigo ingresado
    do{
        std::cout<<"\n"<<texto<<": ";
        std::cin>>cod_entrada;

        // Validación de fallo de lectura
        if(std::cin.fail()){
            std::cout<<"Error: Valor invalido, no entero. Favor de ingresar numero entero"<<std::endl;
            std::cin.clear();
            std::cin.ignore(1024, '\n');

            entrada_valida = false;
        } else { // Lectura correcta
            entrada_valida = true;
        }
    } while(!entrada_valida);

    return cod_entrada;
}

int buscarIndiceCodigo(int cod_entrada, int* codigo, int tam){
    int index;
    
    bool cod_encontrado = false;

    // Usamos un for para buscar una coincidencia en la entrada y codigo
    for(int i = 0; i < tam; i++){
        if(cod_entrada == codigo[i]){
            cod_encontrado = true;
            index = i; // Pasamos el index
            break; // Cerramos el ciclo
        } else {
            cod_encontrado = false;
        }
    }

    // Si no encuentra nada asignamos el valor del tamaño
    if (!cod_encontrado){
        index = tam;
    }

    return index;
}

int buscarIndiceUbicacion(std::string ubi, std::string * ubicacion, int tam){
    int index;
    
    std::cout<<"\n"<<ubi<<std::endl;

    bool ubi_encontrada = false;

    // Usamos un for para buscar una coincidencia en la entrada y codigo
    for(int i = 0; i < tam; i++){
        if(ubi == ubicacion[i]){
            ubi_encontrada = true;
            index = i; // Pasamos el index
            break; // Cerramos el ciclo
        } else {
            ubi_encontrada = false;
        }
    }

    // Si no encuentra nada asignamos el valor del tamaño
    if (!ubi_encontrada){
        index = tam;
    }

    return index;
}

void indexCero(int tam){
    if(tam == 0){
        std::cout<<"Advertencia: No hay registro de productos, no marcara error, pero registre un producto"<<std::endl;
    }
}