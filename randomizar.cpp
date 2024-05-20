#include <random>
#include <string>
#include <vector>
#include <iostream>

struct Persona
{
    std::string dni;
    std::string nombres;
    std::string apellidos;
    std::string nacionalidad;
    std::string lugarNacimiento;
    std::string direccion;
    std::string telefono;
    std::string correo;
    std::string estadoCivil;
};

std::string generar_dni_aleatorio()
{
    static const std::string digitos = "0123456789";            // digitos posibles de un DNI
    std::random_device rd;                                      // número aleatorio de un dispositivo de generación de números aleatorios del sistema
    std::mt19937 gen(rd());                                     // generador de números aleatorio utilizando el número aleatorio obtenido del sistema
    std::uniform_int_distribution<> dis(0, digitos.size() - 1); // devuelve un numero en el rango de índices del vector
                                                                // nombres, en este caso un indice

    std::string resultado;
    for (size_t i = 0; i < 8; ++i)
    {
        resultado += digitos[dis(gen)];
    }
    return resultado;
}

std::string generar_nombre_aleatorio()
{
    static const std::vector<std::string> nombres = {"Pepito", "Juan", "Maria", "Andre", "Pedro", "Ana", "Evie", "Dennis", "Jesus", "Jorge", "Rosa",
                                                     "Luis", "Carlos", "Sofia", "Lucia", "Miguel", "Jose", "Manuel", "Ricardo", "Raul", "Luisa",
                                                     "Fernando", "Javier", "Joaquin", "Julieta", "Julia", "Diego", "Valentina", "Felipe", "Camila",
                                                     "Mateo", "Isabella", "Nicolas", "Sara", "Samuel", "Laura", "Daniel", "Mariana", "Alejandro", "Carolina"};
    // nombres posibles de personas

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, nombres.size() - 1); // devuelve un numero en el rango de índices del vector
                                                                // nombres, en este caso un indice

    return nombres[dis(gen)]; // con ese indice aleatorio obtenemos un nombre aleatorio
}

std::string generar_apellido_aleatorio()
{
    static const std::vector<std::string> apellidos = {"Gonzales", "Lopez", "Perez", "Rodriguez", "Garcia", "Diaz", "Ceballos", "Vargas", "Torres",
                                                       "Mendoza", "Carrasco", "Gutierrez", "Villanueva", "Cordova", "Cruz", "Vasquez", "Villegas",
                                                       "Vega", "Vera", "Velasquez", "Vargas", "Valencia", "Urbina", "Ugarte", "Toro", "Tello", "Tafur",
                                                       "Soto", "Silva", "Sifuentes", "Serrano", "Serna", "Segura", "Santos", "Santiago", "Sanchez",
                                                       "Salazar", "Ruiz", "Rojas", "Rivera", "Rios", "Rivas", "Rivas", "Reyes", "Reyna", "Requena",
                                                       "Ramirez", "Ramos", "Ramirez", "Quispe", "Quintana", "Puma", "Pizarro", "Pinto", "Pineda",
                                                       "Pinedo", "Pinedo", "Perez"};
    // apellidos posibles de personas
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, apellidos.size() - 1); // devuelve un numero en el rango de índices del vector
                                                                  // apellidos, en este caso un indice

    return apellidos[dis(gen)]; // con ese indice aleatorio obtenemos un apellido aleatorio
}

std::string generar_nacionalidad_aleatorio()
{
    static const std::vector<std::string> estados = {"Peruano(a)"};
    // nacionalidades posibles de personas
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, estados.size() - 1); // devuelve un numero en el rango de índices del vector
                                                                // nombres, en este caso un indice

    return estados[dis(gen)]; // con ese indice aleatorio obtenemos un estado aleatorio
}

std::string generar_lugar_nacimiento_aleatorio()
{
    static const std::vector<std::string> lugar_nacimiento = {"Lima", "Cajamarca", "Cusco", "Arequipa", "Puno", "Ica", "Tacna", "Piura", "Lambayeque",
                                                              "La Libertad", "Ancash", "Junin", "Huanuco", "Pasco", "Ucayali", "Madre de Dios", "Loreto",
                                                              "San Martin", "Amazonas", "Tumbes", "Moquegua", "Apurimac", "Ayacucho", "Huancavelica"};
    // lugares de nacimiento posibles de personas

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, lugar_nacimiento.size() - 1); // devuelve un numero en el rango de índices del vector lugares, aqui un indice

    return lugar_nacimiento[dis(gen)]; // con ese indice aleatorio obtenemos un nombre aleatorio
}

std::string generar_direccion_aleatorio()
{
    static const std::vector<std::string> direcciones = {"Calle Rosas 123", "Av.  Javier Prado 456", "Jirón Rickenbaker 789", "Jr. Ancash 143", "Av. Abancay 561", "Callejón Secreto",
                                                         "Calle Zurbaran 201", "Av. Primavera 232", "Jr. Puno 154", "Jr. Ayacucho 371", "Calle Luiggi Pirandello 314", "Av. Guardia Civil 1070",
                                                         "Jr. Lampa 101", "Psj. 106", "Calle 175", "Av. Tacna 971", "Jr. Huallaga 138", "Psj. Velasquez 211", "Calle Paita 347", "Av. Alfonso Ugarte 893",
                                                         "Jr. Carabaya 87", "Psj. Cotahuasi 55", "Calle Las Herramientas 166", "Av. Elmer Faucett 1124", "Jr. Galileo 19", "Psj. Venus 20"};
    // direcciones posibles de personas

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, direcciones.size() - 1); // devuelve un numero en el rango de índices del vector direcciones, en este caso un indice

    return direcciones[dis(gen)]; // con ese indice aleatorio obtenemos una dirección aleatoria
}

std::string generar_telefono_aleatorio()
{
    static const std::string digitos = "0123456789"; // digitos posibles de un telefono
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, digitos.size() - 1);

    std::string resultado;
    for (size_t i = 0; i < 8; ++i) // telefono de 9 digitos, el primer digito es 9 los demas son aleatorios
    {
        resultado += digitos[dis(gen)];
    }
    return "9" + resultado;
}

std::string generar_correo_aleatorio(std::string &nombre, std::string &apellido)
{
    static const std::vector<std::string> dominios = {"@gmail.com", "@hotmail.com", "@yahoo.com", "@outlook.com", "@icloud.com", "@yahoo.es", "@gmail.es", "@hotmail.es"};
    // correos posibles de personas
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, dominios.size() - 1); // devuelve un numero en el rango de índices del vector
                                                                 // dominios, en este caso un indice
    std::string nombreLower = nombre;
    std::transform(nombreLower.begin(), nombreLower.end(), nombreLower.begin(), ::tolower); // convertir nombre a minúsculas,
                                                                                            // nombreLower.begin() y nombreLower.end() indican el rango de
                                                                                            // caracteres a procesar
                                                                                            // nombreLower.begin() es el destino, es decir, donde se
                                                                                            // escribirán los caracteres convertidos.
                                                                                            // ::tolower indica que se debe aplicar la función tolower a
                                                                                            // cada caracter

    std::string apellidoLower = apellido;
    std::transform(apellidoLower.begin(), apellidoLower.end(), apellidoLower.begin(), ::tolower); // convertir apellido a minúsculas

    return nombreLower + apellidoLower + dominios[dis(gen)]; // con ese indice aleatorio obtenemos un correo aleatorio con
                                                             // ayuda de un nombre y apellido pasado por parametro
}

std::string generar_estado_civil_aleatorio()
{
    static const std::vector<std::string> estados = {"Soltero(a)", "Casado(a)", "Divorciado(a)", "Viudo(a)", "Conviviente"};
    // estados posibles de personas
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, estados.size() - 1); // devuelve un numero en el rango de índices del vector
                                                                // nombres, en este caso un indice

    return estados[dis(gen)]; // con ese indice aleatorio obtenemos un estado aleatorio
}

int main()
{
    std::vector<Persona> personas;
    personas.reserve(10); // muestra para 10 personas nomas XD

    for (int i = 0; i < 10; ++i)
    {
        Persona p;
        p.dni = generar_dni_aleatorio();
        std::cout << p.dni << " ";
        p.nombres = generar_nombre_aleatorio();
        std::cout << p.nombres << " ";
        p.apellidos = generar_apellido_aleatorio();
        std::cout << p.apellidos << " ";
        std::cout << generar_telefono_aleatorio() << " ";
        std::cout << generar_nacionalidad_aleatorio() << " ";
        std::cout << generar_lugar_nacimiento_aleatorio() << " ";
        std::cout << generar_direccion_aleatorio() << " ";
        std::cout << generar_correo_aleatorio(p.nombres, p.apellidos) << " ";
        std::cout << generar_estado_civil_aleatorio() << std::endl;
    }

    return 0;
}