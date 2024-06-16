#pragma once
#include <iostream>
#include <random>
#include <cstring>
#include <unordered_set>
#include "Person.h"
#include "PageManager.h"
class DataGenerator
{
public:
    Personita generar_persona_aleatoria(int pageID) {
        std::string dni = generar_dni_aleatorio();
        std::string nombre = generar_nombre_aleatorio();
        std::string apellido = generar_apellido_aleatorio();
        int edad = 18 + (std::rand() % 60); // Generar una edad aleatoria entre 18 y 77
        std::string nacionalidad = generar_nacionalidad_aleatorio();
        std::string lugarNacimiento = generar_lugar_nacimiento_aleatorio();
        std::string direccion = generar_direccion_aleatorio();
        std::string telefono = generar_telefono_aleatorio();
        std::string correo = generar_correo_aleatorio(nombre, apellido);
        std::string estadoCivil = generar_estado_civil_aleatorio();

        // Crear el objeto Personita
        Personita persona(
            nombre.c_str(),
            edad,
            dni.c_str(),
            pageID,
            apellido.c_str(),
            nacionalidad.c_str(),
            lugarNacimiento.c_str(),
            direccion.c_str(),
            telefono.c_str(),
            correo.c_str(),
            estadoCivil.c_str()
        );

        return persona;
    }

    Personita GenerateRandomPersonita() {
        Personita persona = generar_persona_aleatoria(-1);
        return persona;
    }

    void GenerateNRecordsData(long numberOfRecordsToGenerate, PageManager& pageManager)
    {
        for (long i = 0; i < numberOfRecordsToGenerate; ++i)
        {
            Personita persona = generar_persona_aleatoria(i);
            pageManager.WritePersonitaInDisk(i, persona);

            if (i % 100000 == 0)
            {
                // show progress by 100,000 records
                std::cout << "Progreso: " << i << " registros generados." << std::endl;
            }
        }

        std::cout << "Generacion completada." << std::endl;
        long fileSize = pageManager.GetFileSize();

        // calculate the total of recods in the file
        long totalRecords = fileSize / sizeof(Personita);

        // calculate 't' of BTree
    }
private:
    std::unordered_set<std::string> generatedDNIs;

    std::string generar_dni_aleatorio() {
        static const std::string digitos = "0123456789";
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, digitos.size() - 1);

        std::string resultado;
        
        do 
        {
            resultado.clear();
            for (size_t i = 0; i < 8; ++i) 
            {
                resultado += digitos[dis(gen)];
            }
        } while (generatedDNIs.find(resultado) != generatedDNIs.end());

        generatedDNIs.insert(resultado);

        return resultado;
    }

    std::string generar_nombre_aleatorio() {
        static const std::vector<std::string> nombres = {
            "Pepito", "Juan", "Maria", "Andre", "Pedro", "Ana", "Evie", "Dennis",
            "Jesus", "Jorge", "Rosa", "Luis", "Carlos", "Sofia", "Lucia", "Miguel",
            "Jose", "Manuel", "Ricardo", "Raul", "Luisa", "Fernando", "Javier",
            "Joaquin", "Julieta", "Julia", "Diego", "Valentina", "Felipe", "Camila",
            "Mateo", "Isabella", "Nicolas", "Sara", "Samuel", "Laura", "Daniel",
            "Mariana", "Alejandro", "Carolina"
        };

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, nombres.size() - 1);

        return nombres[dis(gen)];
    }

    std::string generar_apellido_aleatorio() {
        static const std::vector<std::string> apellidos = {
            "Gonzales", "Lopez", "Perez", "Rodriguez", "Garcia", "Diaz", "Ceballos",
            "Vargas", "Torres", "Mendoza", "Carrasco", "Gutierrez", "Villanueva",
            "Cordova", "Cruz", "Vasquez", "Villegas", "Vega", "Vera", "Velasquez",
            "Vargas", "Valencia", "Urbina", "Ugarte", "Toro", "Tello", "Tafur",
            "Soto", "Silva", "Sifuentes", "Serrano", "Serna", "Segura", "Santos",
            "Santiago", "Sanchez", "Salazar", "Ruiz", "Rojas", "Rivera", "Rios",
            "Rivas", "Rivas", "Reyes", "Reyna", "Requena", "Ramirez", "Ramos",
            "Ramirez", "Quispe", "Quintana", "Puma", "Pizarro", "Pinto", "Pineda",
            "Pinedo", "Pinedo", "Perez"
        };

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, apellidos.size() - 1);

        return apellidos[dis(gen)];
    }

    std::string generar_nacionalidad_aleatorio() {
        static const std::vector<std::string> estados = { "Peruano(a)" };
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, estados.size() - 1);

        return estados[dis(gen)];
    }

    std::string generar_lugar_nacimiento_aleatorio() {
        static const std::vector<std::string> lugar_nacimiento = {
            "Lima", "Cajamarca", "Cusco", "Arequipa", "Puno", "Ica", "Tacna",
            "Piura", "Lambayeque", "La Libertad", "Ancash", "Junin", "Huanuco",
            "Pasco", "Ucayali", "Madre de Dios", "Loreto", "San Martin", "Amazonas",
            "Tumbes", "Moquegua", "Apurimac", "Ayacucho", "Huancavelica"
        };

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, lugar_nacimiento.size() - 1);

        return lugar_nacimiento[dis(gen)];
    }

    std::string generar_direccion_aleatorio() {
        static const std::vector<std::string> direcciones = {
            "Calle Rosas 123", "Av. Javier Prado 456", "Jirón Rickenbaker 789",
            "Jr. Ancash 143", "Av. Abancay 561", "Callejón Secreto", "Calle Zurbaran 201",
            "Av. Primavera 232", "Jr. Puno 154", "Jr. Ayacucho 371", "Calle Luiggi Pirandello 314",
            "Av. Guardia Civil 1070", "Jr. Lampa 101", "Psj. 106", "Calle 175",
            "Av. Tacna 971", "Jr. Huallaga 138", "Psj. Velasquez 211", "Calle Paita 347",
            "Av. Alfonso Ugarte 893", "Jr. Carabaya 87", "Psj. Cotahuasi 55",
            "Calle Las Herramientas 166", "Av. Elmer Faucett 1124", "Jr. Galileo 19", "Psj. Venus 20"
        };

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, direcciones.size() - 1);

        return direcciones[dis(gen)];
    }

    std::string generar_telefono_aleatorio() {
        static const std::string digitos = "0123456789";
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, digitos.size() - 1);

        std::string resultado;
        for (size_t i = 0; i < 7; ++i) {
            resultado += digitos[dis(gen)];
        }
        return "9" + resultado;
    }

    std::string generar_correo_aleatorio(std::string& nombre, std::string& apellido) {
        static const std::vector<std::string> dominios = {
            "@gmail.com", "@hotmail.com", "@yahoo.com", "@outlook.com", "@icloud.com",
            "@yahoo.es", "@gmail.es", "@hotmail.es"
        };

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, dominios.size() - 1);

        std::string nombreLower = nombre;
        std::transform(nombreLower.begin(), nombreLower.end(), nombreLower.begin(), ::tolower);

        std::string apellidoLower = apellido;
        std::transform(apellidoLower.begin(), apellidoLower.end(), apellidoLower.begin(), ::tolower);

        return nombreLower + apellidoLower + dominios[dis(gen)];
    }

    std::string generar_estado_civil_aleatorio() {
        static const std::vector<std::string> estados = {
            "So", "Ca", "Di", "Vi", "Co"
        };

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, estados.size() - 1);

        return estados[dis(gen)];
    }
};